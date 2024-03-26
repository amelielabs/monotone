#pragma once

//
// monotone
//
// time-series storage
//

typedef struct Uuid Uuid;

#define UUID_SZ 37

struct Uuid
{
	uint64_t a;
	uint64_t b;
};

void uuid_init(Uuid*);
void uuid_generate(Uuid*, Random*);
void uuid_to_string(Uuid*, char*, int);
void uuid_from_string(Uuid*, Str*);
int  uuid_from_string_nothrow(Uuid*, Str*);

static inline bool
uuid_empty(Uuid* self)
{
	return self->a == self->b == 0;
}

static inline bool
uuid_compare(Uuid *l, Uuid *r)
{
	return l->a == r->a && l->b == r->b;
}
