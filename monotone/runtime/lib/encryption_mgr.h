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

typedef struct EncryptionMgr EncryptionMgr;

struct EncryptionMgr
{
	Cache cache_aes;
};

void encryption_mgr_init(EncryptionMgr*);
void encryption_mgr_free(EncryptionMgr*);
Encryption*
encryption_mgr_pop(EncryptionMgr*, int);
void encryption_mgr_push(EncryptionMgr*, Encryption*);
int  encryption_mgr_of(Str*);
