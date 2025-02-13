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

static inline void*
mn_malloc(size_t size)
{
	auto ptr = malloc(size);
	if (unlikely(ptr == NULL))
		error_system();
	return ptr;
}

static inline void*
mn_realloc(void* pointer, size_t size)
{
	auto ptr = realloc(pointer, size);
	if (unlikely(ptr == NULL))
		error_system();
	return ptr;
}

static inline void
mn_free(void* pointer)
{
	free(pointer);
}
