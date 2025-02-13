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

static inline bool
parse_if_not_exists(Lex* self)
{
	if (! lex_if(self, KIF, NULL))
		return false;
	if (! lex_if(self, KNOT, NULL))
		error("IF 'NOT' EXISTS expected");
	if (! lex_if(self, KEXISTS, NULL))
		error("IF NOT 'EXISTS' expected");
	return true;
}

static inline bool
parse_if_exists(Lex* self)
{
	if (! lex_if(self, KIF, NULL))
		return false;
	if (! lex_if(self, KEXISTS, NULL))
		error("IF 'EXISTS' expected");
	return true;
}

static inline bool
parse_if_cloud(Lex* self)
{
	if (! lex_if(self, KIF, NULL))
		return false;
	if (! lex_if(self, KCLOUD, NULL))
		error("IF 'CLOUD' expected");
	return true;
}

static inline void
parse_int(Lex* self, Token* name, int64_t* value)
{
	// int
	Token tk;
	if (! lex_if(self, KINT, &tk))
		error("%.*s 'integer' expected", str_size(&name->string),
		      str_of(&name->string));

	*value = tk.integer;
}

static inline void
parse_bool(Lex* self, Token* name, bool* value)
{
	// [true/false]
	if (lex_if(self, KTRUE, NULL))
	{
		*value = true;
	} else
	if (lex_if(self, KFALSE, NULL))
	{
		*value = false;
	} else {
		error("%.*s 'true|false' expected", str_size(&name->string),
		      str_of(&name->string));
	}
}

static inline void
parse_string(Lex* self, Token* name, Str* value)
{
	// string
	Token tk;
	if (! lex_if(self, KSTRING, &tk))
		error("%.*s 'string'expected", str_size(&name->string),
		      str_of(&name->string));

	str_free(value);
	str_copy(value, &tk.string);
}

static inline void
parse_uuid(Lex* self, Token* name, Uuid* value)
{
	// string
	Token tk;
	if (! lex_if(self, KSTRING, &tk))
		error("%.*s 'string' expected", str_size(&name->string),
		      str_of(&name->string));

	uuid_from_string(value, &tk.string);
}
