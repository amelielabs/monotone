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

typedef struct Wal Wal;

struct Wal
{
	Mutex    lock;
	WalId    list;
	WalFile* current;
	int      dirfd;
};

void wal_init(Wal*);
void wal_free(Wal*);
void wal_open(Wal*);
void wal_rotate(Wal*, uint64_t);
void wal_gc(Wal*, uint64_t);
bool wal_write(Wal*, Log*);
bool wal_write_op(Wal*, LogWrite*);
void wal_show(Wal*, Buf*);
