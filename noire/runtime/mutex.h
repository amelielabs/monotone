#pragma once

//
// noire
//
// time-series storage
//

typedef struct Mutex Mutex;

struct Mutex
{
	pthread_mutex_t lock;
};

static inline void
mutex_init(Mutex* self)
{
	pthread_mutex_init(&self->lock, NULL);
}

static inline void
mutex_free(Mutex* self)
{
	pthread_mutex_destroy(&self->lock);
}

static inline void
mutex_lock(Mutex* self)
{
	pthread_mutex_lock(&self->lock);
}

static inline void
mutex_unlock(Mutex* self)
{
	pthread_mutex_unlock(&self->lock);
}
