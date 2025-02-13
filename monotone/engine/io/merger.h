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

typedef struct MergerReq MergerReq;
typedef struct Merger    Merger;

struct MergerReq
{
	Part*     origin;
	Memtable* memtable;
	Source*   source;
};

struct Merger
{
	Part*            part;
	PartIterator     part_iterator;
	MemtableIterator memtable_iterator;
	MergeIterator    merge_iterator;
	Writer           writer;
};

void merger_init(Merger*);
void merger_free(Merger*);
void merger_reset(Merger*);
void merger_execute(Merger*, MergerReq*);
