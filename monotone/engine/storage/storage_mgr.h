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

typedef struct StorageMgr StorageMgr;

struct StorageMgr
{
	List      list;
	int       list_count;
	CloudMgr* cloud_mgr;
};

void storage_mgr_init(StorageMgr*, CloudMgr*);
void storage_mgr_free(StorageMgr*);
void storage_mgr_open(StorageMgr*);
bool storage_mgr_create_main(StorageMgr*);
bool storage_mgr_create(StorageMgr*, Source*, bool);
void storage_mgr_drop(StorageMgr*, Str*, bool);
void storage_mgr_alter(StorageMgr*, Source*, int, bool);
void storage_mgr_rename(StorageMgr*, Str*, Str*, bool);
void storage_mgr_rename_cloud(StorageMgr*, Str*, Str*);
void storage_mgr_show(StorageMgr*, Buf*, bool);

Storage*
storage_mgr_find(StorageMgr*, Str*);
Part*
storage_mgr_find_part(StorageMgr*, Storage*, uint64_t);

static inline Storage*
storage_mgr_first(StorageMgr* self)
{
	assert(self->list_count > 0);
	return container_of(self->list.next, Storage, link);
}
