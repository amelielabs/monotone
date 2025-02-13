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

typedef struct Context Context;
typedef struct Runtime Runtime;

struct Context
{
	LogFunction log;
	void*       log_arg;
	void*       global;
};

struct Runtime
{
	ExceptionMgr exception_mgr;
	Error        error;
	Context*     context;
};

extern __thread Runtime mn_runtime;

void runtime_init(Context*);
