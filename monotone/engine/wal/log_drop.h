#pragma once

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

typedef struct LogDrop LogDrop;

struct LogDrop
{
	LogWrite write;
	uint64_t id;
} packed;

static inline void
log_drop_init(LogDrop* self, uint64_t id)
{
	auto write = &self->write;
	write->crc   = 0;
	write->lsn   = 0;
	write->type  = LOG_DROP;
	write->count = 0;
	write->size  = sizeof(LogDrop);
	self->id     = id;
	// calculate crc
	if (var_int_of(&config()->wal_crc))
		self->write.crc = crc32(self->write.crc, &id, sizeof(id));
}
