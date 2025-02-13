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

typedef struct S3 S3;

struct S3
{
	Cloud cloud;
	Cache cache;
};

always_inline static inline S3*
s3_of(Cloud* self)
{
	return (S3*)self;
}

extern CloudIf cloud_s3;
