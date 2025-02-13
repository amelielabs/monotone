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

typedef struct S3Io S3Io;

typedef size_t (*S3Function)(void* ptr, size_t len, size_t nmemb, void* arg);

struct S3Io
{
	void*  handle;
	Cloud* cloud;
	List   link;
};

S3Io* s3_io_allocate(Cloud*);
void  s3_io_free(S3Io*);
