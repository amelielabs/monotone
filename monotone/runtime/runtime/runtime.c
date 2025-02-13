
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

__thread Runtime mn_runtime;

void
runtime_init(Context* context)
{
	exception_mgr_init(&mn_runtime.exception_mgr);
	error_init(&mn_runtime.error, context->log, context->log_arg);
	mn_runtime.context = context;
}
