#pragma once

//
// monotone
//
// time-series storage
//

typedef struct WalId WalId;

struct WalId
{
	Spinlock lock;
	int      list_count;
	Buf      list;
};

static inline void
wal_id_init(WalId* self)
{
	self->list_count = 0;
	spinlock_init(&self->lock);
	buf_init(&self->list);
}

static inline void
wal_id_free(WalId* self)
{
	spinlock_free(&self->lock);
	buf_free(&self->list);
}

static inline void
wal_id_add(WalId* self, uint64_t id)
{
	spinlock_lock(&self->lock);

	Exception e;
	if (try(&e)) {
		buf_reserve(&self->list, sizeof(id));
	}
	if (catch(&e)) {
		spinlock_unlock(&self->lock);
		rethrow();
	}

	auto list = buf_u64(&self->list);
	int i = 0;
	for (; i < self->list_count && list[i] < id; i++);
	if (i < self->list_count)
		memmove(&list[i + 1], &list[i], (self->list_count - i) * sizeof(uint64_t));
	list[i] = id;

	self->list_count++;
	buf_advance(&self->list, sizeof(id));

	spinlock_unlock(&self->lock);
}

static inline void
wal_id_delete(WalId* self, uint64_t id)
{
	spinlock_lock(&self->lock);

	auto list = buf_u64(&self->list);
	for (int i = 0; i < self->list_count; i++)
	{
		if (list[i] != id)
			continue;
		if (likely((i + 1) < self->list_count))
			memmove(&list[i], &list[i + 1], (self->list_count - (i + 1)) * sizeof(uint64_t));
		buf_truncate(&self->list, sizeof(uint64_t));
		self->list_count--;
		break;
	}

	spinlock_unlock(&self->lock);
}

static inline int
wal_id_copy(WalId* self, Buf* dest, uint64_t limit)
{
	int count = 0;
	spinlock_lock(&self->lock);

	Exception e;
	if (try(&e))
	{
		// copy <= limit
		auto list = (uint64_t*)self->list.start;
		for (int i = 0; i < self->list_count; i++)
		{
			if (list[i] > limit)
				break;
			buf_write(dest, &list[i], sizeof(uint64_t));
			count++;
		}
	}

	spinlock_unlock(&self->lock);
	if (catch(&e))
		rethrow();

	return count;
}

static inline uint64_t
wal_id_min(WalId* self)
{
	uint64_t id;
	spinlock_lock(&self->lock);
	if (likely(self->list_count > 0)) {
		id = buf_u64(&self->list)[0];
	} else {
		id = UINT64_MAX;
	}
	spinlock_unlock(&self->lock);
	return id;
}

static inline uint64_t
wal_id_max(WalId* self)
{
	uint64_t id;
	spinlock_lock(&self->lock);
	if (likely(self->list_count > 0)) {
		id = buf_u64(&self->list)[self->list_count - 1];
	} else {
		id = UINT64_MAX;
	}
	spinlock_unlock(&self->lock);
	return id;
}

static inline void
wal_id_stats(WalId* self, int* count, uint64_t* min)
{
	spinlock_lock(&self->lock);

	if (count)
		*count = self->list_count;

	if (min)
	{
		if (likely(self->list_count > 0))
			*min = buf_u64(&self->list)[0];
		else
			*min = UINT64_MAX;
	}

	spinlock_unlock(&self->lock);
}

static inline uint64_t
wal_id_find(WalId* self, uint64_t value)
{
	spinlock_lock(&self->lock);

	auto list = buf_u64(&self->list);
	uint64_t id = UINT64_MAX;
	for (int i = 0; i < self->list_count; i++)
	{
		if (list[i] > value)
			break;
		id = list[i];
	}

	spinlock_unlock(&self->lock);
	return id;
}

static inline uint64_t
wal_id_next(WalId* self, uint64_t value)
{
	spinlock_lock(&self->lock);

	auto list = buf_u64(&self->list);
	uint64_t id = UINT64_MAX;
	for (int i = 0; i < self->list_count; i++)
	{
		if (list[i] > value)
		{
			id = list[i];
			break;
		}
	}

	spinlock_unlock(&self->lock);
	return id;
}

static inline int
wal_id_gc_between(WalId* self, Buf* dest, uint64_t id)
{
	spinlock_lock(&self->lock);

	auto list = buf_u64(&self->list);

	int i = 0;
	for (; i < self->list_count - 1; i++)
	{
		// [i] < id < [i + 1]
		if (id >= list[i + 1])
			continue;
		break;
	}

	if (i == 0)
	{
		spinlock_unlock(&self->lock);
		return 0;
	}

	// copy removed region
	Exception e;
	if (try(&e))
	{
		int size = i * sizeof(uint64_t);
		buf_write(dest, self->list.start, size);

		assert(self->list_count > i);
		memmove(&list[0], &list[i], (self->list_count - i) * sizeof(uint64_t));

		buf_truncate(&self->list, size);
		self->list_count -= i;
	}

	spinlock_unlock(&self->lock);
	if (catch(&e))
		rethrow();

	return i;
}
