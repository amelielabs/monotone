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

typedef struct CompressionMgr CompressionMgr;

struct CompressionMgr
{
	Cache cache_zstd;
	Cache cache_lz4;
};

void compression_mgr_init(CompressionMgr*);
void compression_mgr_free(CompressionMgr*);
Compression*
compression_mgr_pop(CompressionMgr*, int);
void compression_mgr_push(CompressionMgr*, Compression*);
int  compression_mgr_of(Str*);
