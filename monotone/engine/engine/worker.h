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

typedef struct Worker Worker;

struct Worker
{
	Refresh       refresh;
	Engine*       engine;
	ServiceFilter filter;
	Context*      context;
	Thread        thread;
};

void worker_init(Worker*, Engine*, ServiceFilter);
void worker_free(Worker*);
void worker_start(Worker*);
void worker_stop(Worker*);
