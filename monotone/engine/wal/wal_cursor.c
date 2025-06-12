
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

void
wal_cursor_init(WalCursor* self)
{
	self->file_offset = 0;
	self->file        = NULL;
	self->wal         = NULL;
	buf_init(&self->buf);
}

WalStatus
wal_cursor_open(WalCursor* self, Wal* wal, uint64_t lsn)
{
	self->file        = NULL;
	self->file_offset = 0;
	self->wal         = wal;

	// find nearest file with id <= lsn
	uint64_t id;
	if (lsn == 0)
		id = wal_id_min(&wal->list);
	else
		id = wal_id_find(&wal->list, lsn);
	if (id == UINT64_MAX)
		return WAL_EOF;
	self->file = wal_file_allocate(id);
	wal_file_open(self->file);

	// rewind to the start lsn
	for (;;)
	{
		auto status = wal_cursor_next(self);
		if (status != WAL_OK)
			return status;
		auto write = wal_cursor_at(self);
		if (write->lsn >= lsn)
		{
			// rewind
			self->file_offset -= write->size;
			break;
		}
	}
	return WAL_OK;
}

void
wal_cursor_close(WalCursor* self)
{
	if (self->file)
	{
		wal_file_close(self->file);
		wal_file_free(self->file);
		self->file = NULL;
	}
	buf_free(&self->buf);
}

WalStatus
wal_cursor_next(WalCursor* self)
{
	if (unlikely(self->file == NULL))
		return WAL_EOF;

	auto wal = self->wal;
	for (;;)
	{
		auto file = self->file;
		auto status = wal_file_pread(file, self->file_offset, &self->buf);
		switch (status) {
		case WAL_OK:
		{
			auto write = wal_cursor_at(self);
			self->file_offset += write->size;
			return status;
		}
		case WAL_CORRUPTED:
			return status;
		case WAL_EOF:
			break;
		}

		// eof

		// get to the next file id
		uint64_t id;
		id = wal_id_next(&wal->list, file->id);
		if (id == UINT64_MAX)
			break;

		// close previous file
		wal_file_close(file);
		wal_file_free(file);

		// open next file
		self->file_offset = 0;
		self->file        = NULL;
		self->file        = wal_file_allocate(id);
		wal_file_open(self->file);
	}

	return WAL_EOF;
}
