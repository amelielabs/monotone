
//
// monotone.
//
// embeddable cloud-native storage for events
// and time-series data.
//
// Copyright (c) 2023-2025 Dmitry Simonenko
//
// MIT Licensed.
//

#include <monotone_runtime.h>
#include <monotone_lib.h>
#include <monotone_config.h>
#include <monotone_cloud.h>
#include <monotone_io.h>
#include <monotone_storage.h>
#include <monotone_wal.h>
#include <monotone_engine.h>

void
cron_init(Cron* self, Service* service)
{
	self->service  = service;
	self->context  = NULL;
	self->shutdown = false;
	mutex_init(&self->lock);
	cond_var_init(&self->cond_var);
	thread_init(&self->thread);
}

void
cron_free(Cron* self)
{
	mutex_free(&self->lock);
	cond_var_free(&self->cond_var);
}

static void
cron_shutdown(Cron* self)
{
	mutex_lock(&self->lock);
	self->shutdown = true;
	cond_var_signal(&self->cond_var);
	mutex_unlock(&self->lock);
}

static bool
cron_shutdown_wait(Cron* self, int time_ms)
{
	mutex_lock(&self->lock);
	auto shutdown = self->shutdown;
	if (! shutdown)
	{
		cond_var_timedwait(&self->cond_var, &self->lock, time_ms);
		shutdown = self->shutdown;
	}
	mutex_unlock(&self->lock);
	return shutdown;
}

static void*
cron_main(void* arg)
{
	Cron* self = arg;
	runtime_init(self->context);
	thread_set_name(&self->thread, "cron");

	// schedule periodic wal sync
	auto interval_ms = var_int_of(&config()->wal_sync_interval);
	auto lsn = config_lsn();
	while (! cron_shutdown_wait(self, interval_ms))
	{
		auto lsn_current = config_lsn();
		if (lsn == lsn_current)
			continue;
		service_add(self->service, UINT64_MAX, 1, ACTION_SYNC);
		lsn = lsn_current;
	}

	return NULL;
}

void
cron_start(Cron* self)
{
	if (! config_sync())
		return;
	auto interval_ms = var_int_of(&config()->wal_sync_interval);
	if (! interval_ms)
		return;

	self->context = mn_runtime.context;
	int rc;
	rc = thread_create(&self->thread, cron_main, self);
	if (unlikely(rc == -1))
		error_system();
}

void
cron_stop(Cron* self)
{
	cron_shutdown(self);
	if (thread_is_active(&self->thread))
		thread_join(&self->thread);
}
