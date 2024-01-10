#pragma once

//
// monotone
//
// time-series storage
//

typedef struct Json Json;

struct Json
{
	const char* json;
	int         json_size;
	int         pos;
	Buf         buf;
};

void json_init(Json*);
void json_free(Json*);
void json_reset(Json*);
void json_parse(Json*, Str*);
void json_export(Buf*, uint8_t**);
