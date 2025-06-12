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

typedef struct WalFile WalFile;

typedef enum
{
	WAL_OK,
	WAL_EOF,
	WAL_CORRUPTED
} WalStatus;

struct WalFile
{
	uint64_t id;
	File     file;
};

static inline WalFile*
wal_file_allocate(uint64_t id)
{
	WalFile* self = mn_malloc(sizeof(WalFile));
	self->id = id;
	file_init(&self->file);
	return self;
}

static inline void
wal_file_free(WalFile* self)
{
	mn_free(self);
}

static inline void
wal_file_open(WalFile* self)
{
	char path[PATH_MAX];
	snprintf(path, sizeof(path), "%s/wal/%020" PRIu64 ".wal",
	         config_directory(),
	         self->id);
	file_open(&self->file, path);
}

static inline void
wal_file_create(WalFile* self)
{
	char path[PATH_MAX];
	snprintf(path, sizeof(path), "%s/wal/%020" PRIu64 ".wal",
	         config_directory(),
	         self->id);
	file_create(&self->file, path);
	if (var_int_of(&config()->wal_sync_on_create) && config_sync())
		file_sync(&self->file);
}

static inline void
wal_file_close(WalFile* self)
{
	file_close(&self->file);
}

static inline void
wal_file_truncate(WalFile* self, uint64_t size)
{
	file_truncate(&self->file, size);
	if (config_sync())
		file_sync(&self->file);
}

static inline void
wal_file_write(WalFile* self, void* data, int data_size)
{
	file_write(&self->file, data, data_size);
	if (var_int_of(&config()->wal_sync_on_write) && config_sync())
		file_sync(&self->file);
}

static inline void
wal_file_writev(WalFile* self, struct iovec* iov, int iovc)
{
	file_writev(&self->file, iov, iovc);
	if (var_int_of(&config()->wal_sync_on_write) && config_sync())
		file_sync(&self->file);
}

static inline bool
wal_file_eof(WalFile* self, uint32_t offset, uint32_t size)
{
	return (offset + size) > self->file.size;
}

static inline WalStatus
wal_file_pread(WalFile* self, uint64_t offset, Buf* buf)
{
	buf_reset(buf);

	// check for eof
	if (self->file.size == offset)
		return WAL_EOF;

	// ensure next record fits header
	if (wal_file_eof(self, offset, sizeof(LogWrite)))
		return WAL_CORRUPTED;

	// read header
	uint32_t size_header = sizeof(LogWrite);
	int start = buf_size(buf);
	file_pread_buf(&self->file, buf, size_header, offset);
	uint32_t size = ((LogWrite*)(buf->start + start))->size;

	// ensure file size fits the record
	if (wal_file_eof(self, offset, size))
		return WAL_CORRUPTED;

	// read body
	uint32_t size_data;
	size_data = size - size_header;
	file_pread_buf(&self->file, buf, size_data, offset + size_header);

	// check crc
	if (var_int_of(&config()->wal_crc))
	{
		auto write = (LogWrite*)(buf->start + start);
		uint32_t crc = 0;
		crc = global()->crc(crc, buf->start + start + size_header, size_data);
		crc = global()->crc(crc, (char*)write + sizeof(uint32_t), size_header - sizeof(uint32_t));
		if (crc != write->crc)
			return WAL_CORRUPTED;
	}

	return WAL_OK;
}
