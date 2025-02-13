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

always_inline static inline int64_t
compare_u64(uint64_t a, uint64_t b)
{
	return (int64_t)a - (int64_t)b;
}
