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

typedef struct Region Region;

struct Region
{
	uint32_t size;
	uint32_t events;
	uint32_t reserve[4];
	// u32 offset[]
	// data
} packed;

static inline Event*
region_get(Region* self, int pos)
{
	assert(pos < (int)self->events);
	auto start  = (char*)self + sizeof(Region);
	auto offset = (uint32_t*)start;
	return (Event*)(start + (sizeof(uint32_t) * self->events) +
	                offset[pos]);
}
