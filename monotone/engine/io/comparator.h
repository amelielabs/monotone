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

typedef struct Comparator Comparator;

struct Comparator
{
	int unused;
};

static inline void
comparator_init(Comparator* self)
{
	self->unused = 0;
}

always_inline static inline int
compare_int64(int64_t a, int64_t b)
{
    return (a > b) - (a < b);
}

hot static inline int64_t
compare(Comparator* self, Event* a, Event* b)
{
	unused(self);
	// compare by [id, key]
	int64_t diff = compare_int64(a->id, b->id);
	if (likely(diff != 0))
		return diff;
	if (a->key_size == 0 && b->key_size == 0)
		return 0;
	int size;
	if (a->key_size < b->key_size)
		size = a->key_size;
	else
		size = b->key_size;
	int rc = memcmp(a->data, b->data, size);
	if (rc == 0)
		return compare_int64(a->key_size, b->key_size);
	return rc;
}
