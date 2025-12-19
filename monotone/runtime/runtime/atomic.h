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

typedef volatile uint32_t atomic_u32;
typedef volatile uint64_t atomic_u64;

static inline uint32_t
atomic_u32_of(atomic_u32* self)
{
	return __sync_fetch_and_add(self, 0);
}

static inline uint32_t
atomic_u32_inc(atomic_u32* self)
{
	return __sync_fetch_and_add(self, 1);
}

static inline uint32_t
atomic_u32_dec(atomic_u32* self)
{
	return __sync_fetch_and_sub(self, 1);
}

static inline uint32_t
atomic_u32_set(atomic_u32* self, uint32_t value)
{
	return __sync_lock_test_and_set(self, value);
}

static inline uint64_t
atomic_u64_of(atomic_u64* self)
{
	return __sync_fetch_and_add(self, 0);
}

static inline uint64_t
atomic_u64_add(atomic_u64* self, uint64_t value)
{
	return __sync_fetch_and_add(self, value);
}

static inline uint64_t
atomic_u64_inc(atomic_u64* self)
{
	return __sync_fetch_and_add(self, 1);
}

static inline uint64_t
atomic_u64_dec(atomic_u64* self)
{
	return __sync_fetch_and_sub(self, 1);
}

static inline uint64_t
atomic_u64_set(atomic_u64* self, uint64_t value)
{
	return __sync_lock_test_and_set(self, value);
}

static inline bool
atomic_u64_cas(atomic_u64* self, uint64_t old_value, uint64_t new_value)
{
	return __sync_bool_compare_and_swap(self, old_value, new_value);
}
