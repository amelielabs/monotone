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

typedef struct Control Control;

struct Control
{
	void  (*save_config)(void*);
	void  (*lock)(void*, bool);
	void  (*unlock)(void*);
	void*  arg;
};

static inline void
control_init(Control* self)
{
	memset(self, 0, sizeof(*self));
}
