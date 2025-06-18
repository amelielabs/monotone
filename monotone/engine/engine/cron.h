#pragma once

//
// amelie.
//
// Real-Time SQL OLTP Database.
//
// Copyright (c) 2024 Dmitry Simonenko.
// Copyright (c) 2024 Amelie Labs.
//
// AGPL-3.0 Licensed.
//

typedef struct Cron Cron;

struct Cron
{
	Mutex    lock;
	CondVar  cond_var;
	bool     shutdown;
	Service* service;
	Context* context;
	Thread   thread;
};

void cron_init(Cron*, Service*);
void cron_free(Cron*);
void cron_start(Cron*);
void cron_stop(Cron*);
