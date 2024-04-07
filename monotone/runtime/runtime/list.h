#pragma once

//
// monotone.
//
// embeddable cloud-native storage for events
// and time-series data.
//
// Copyright (c) 2023-2024 Dmitry Simonenko
// Copyright (c) 2023-2024 Monotone
//
// Distributed under the terms of GNU LGPL v3 or
// Monotone Commercial License.
//

typedef struct List List;

struct List
{
	List* next;
	List* prev;
};

static inline void
list_init(List* self)
{
	self->next = self->prev = self;
}

static inline bool
list_empty(List* self)
{
	return self->next == self &&
	       self->prev == self;
}

static inline bool
list_is_last(List* self, List* link)
{
	return link->next == self;
}

static inline void
list_append(List* self, List* link)
{
	link->next = self;
	link->prev = self->prev;
	link->prev->next = link;
	link->next->prev = link;
}

static inline void
list_unlink(List* link)
{
	link->prev->next = link->next;
	link->next->prev = link->prev;
}

static inline void
list_push(List* self, List* link)
{
	link->next = self->next;
	link->prev = self;
	link->prev->next = link;
	link->next->prev = link;
}

static inline List*
list_pop(List* self)
{
	List* pop = self->next;
	list_unlink(pop);
	return pop;
}

#define list_foreach(list) \
	for (typeof(list) _i = (list)->next; _i != list; _i = _i->next)

#define list_foreach_safe(list) \
	for (typeof(list) _i_copy, _i = (list)->next; \
	     _i != list && (_i_copy = _i->next); \
	     _i = _i_copy)

#define list_foreach_after(list, link) \
	for (typeof(list) _i = (link)->next; _i != list; _i = _i->next)

#define list_foreach_reverse(list) \
	for (typeof(list) _i = (list)->prev; _i != list; _i = _i->prev)

#define list_foreach_reverse_safe(list) \
	for (typeof(list) _i_copy, _i = (list)->prev; \
	     _i != list && (_i_copy = _i->prev); \
	     _i = _i_copy)

#define list_at(type, link) container_of(_i, type, link)
