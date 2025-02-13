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

#define error_injection(name) \
	if (unlikely(var_int_of(&config()->name))) \
		error("error injection: %s", #name)
