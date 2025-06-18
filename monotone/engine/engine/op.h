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

// operations
void engine_fill_nolock(Engine*, uint64_t, uint64_t);
void engine_fill(Engine*, uint64_t, uint64_t);
void engine_drop(Engine*, uint64_t, bool, bool, int);
void engine_drop_range(Engine*, uint64_t, uint64_t, int);

// cloud
void engine_download(Engine*, uint64_t, bool, bool);
void engine_download_range(Engine*, uint64_t, uint64_t, bool);
void engine_upload(Engine*, uint64_t, bool, bool);
void engine_upload_range(Engine*, uint64_t, uint64_t, bool);
void engine_refresh(Engine*, Refresh*, uint64_t, Str*, bool);
void engine_refresh_range(Engine*, Refresh*, uint64_t, uint64_t, Str*);

// general
void engine_rebalance(Engine*, Refresh*);
void engine_checkpoint(Engine*);
void engine_gc(Engine*);
void engine_sync(Engine*);

// service
bool engine_service(Engine*, Refresh*, ServiceFilter, bool);
void engine_resume(Engine*);
