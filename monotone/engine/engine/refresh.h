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

typedef struct Refresh Refresh;

struct Refresh
{
	Ref*      ref;
	Part*     origin;
	Part*     part;
	Memtable* memtable;
	Storage*  storage_origin;
	Storage*  storage;
	Merger    merger;
	Engine*   engine;
};

void refresh_init(Refresh*, Engine*);
void refresh_free(Refresh*);
void refresh_reset(Refresh*);
void refresh_run(Refresh*, uint64_t, Str*, bool);
