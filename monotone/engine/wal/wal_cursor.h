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

typedef struct WalCursor WalCursor;

struct WalCursor
{
	Buf      buf;
	WalFile* file;
	uint64_t file_offset;
	Wal*     wal;
};

void wal_cursor_init(WalCursor*);
void wal_cursor_open(WalCursor*, Wal*, uint64_t);
void wal_cursor_close(WalCursor*);
bool wal_cursor_next(WalCursor*);
LogWrite*
wal_cursor_at(WalCursor*);
