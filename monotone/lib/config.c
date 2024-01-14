
//
// monotone
//
// time-series storage
//

#include <monotone_runtime.h>
#include <monotone_lib.h>

typedef struct
{
	const char* name;
	VarType     type;
	int         flags;
	Var*        var;
	char*       default_string;
	uint64_t    default_int;
} ConfigDef;

static inline void
config_add(Config* self, ConfigDef* def)
{
	auto var = def->var;
	var_init(var, def->name, def->type, def->flags);
	list_append(&self->list, &var->link);
	self->count++;

	if (! (var_is(var, VAR_H) || var_is(var, VAR_H)))
		self->count_visible++;
	if (var_is(var, VAR_C))
		self->count_config++;
	if (! var_is(var, VAR_E))
	{
		list_append(&self->list_persistent, &var->link_persistent);
		self->count_persistent++;
	}

	switch (def->type) {
	case VAR_BOOL:
	case VAR_INT:
		var_int_set(var, def->default_int);
		break;
	case VAR_STRING:
		if (def->default_string)
			var_string_set_raw(var, def->default_string, strlen(def->default_string));
		break;
	case VAR_DATA:
		break;
	}
}

void
config_init(Config* self)
{
	memset(self, 0, sizeof(*self));
	list_init(&self->list);
	list_init(&self->list_persistent);
}

void
config_free(Config* self)
{
	list_foreach_safe(&self->list) {
		auto var = list_at(Var, link);
		var_free(var);
	}
}

void
config_prepare(Config* self)
{
	ConfigDef defaults[] =
	{
		// main
		{ "version",                 VAR_STRING, VAR_E,                &self->version,                 "0.0",       0                },
		{ "uuid",                    VAR_STRING, VAR_C,                &self->uuid,                    NULL,        0                },
		{ "online",                  VAR_BOOL,   VAR_E,                &self->online,                  NULL,        false            },
		{ "directory",               VAR_STRING, VAR_E,                &self->directory,               NULL,        0                },
		// log
		{ "log_enable",              VAR_BOOL,   VAR_C,                &self->log_enable,              NULL,        true             },
		{ "log_to_file",             VAR_BOOL,   VAR_C,                &self->log_to_file,             NULL,        true             },
		{ "log_to_stdout",           VAR_BOOL,   VAR_C,                &self->log_to_stdout,           NULL,        false            },
		// engine
		{ "interval",                VAR_INT,    VAR_C,                &self->interval,                NULL,        3000000          },
		{ "workers",                 VAR_INT,    VAR_C,                &self->workers,                 NULL,        3                },
		// state
		{ "psn",                     VAR_INT,    VAR_E,                &self->psn,                     NULL,        0                },
		{ "storages",                VAR_DATA,   VAR_C|VAR_H,          &self->storages,                NULL,        0                },
		{ "conveyor",                VAR_DATA,   VAR_C|VAR_H,          &self->conveyor,                NULL,        0                },
		// testing
		{ "test_bool",               VAR_BOOL,   VAR_H|VAR_R|VAR_E,    &self->test_bool,               NULL,        false            },
		{ "test_int",                VAR_INT,    VAR_H|VAR_R|VAR_E,    &self->test_int,                NULL,        0                },
		{ "test_string",             VAR_STRING, VAR_H|VAR_R|VAR_E,    &self->test_string,             NULL,        0                },
		{ "test_data",               VAR_DATA,   VAR_H|VAR_R|VAR_E,    &self->test_data,               NULL,        0                },
		{  NULL,                     0,             0,                 NULL,                           NULL,        0                },
	};
	for (int i = 0; defaults[i].name != NULL; i++)
		config_add(self, &defaults[i]);
}

static void
config_set_data(Config* self, uint8_t** pos)
{
	int count;
	data_read_map(pos, &count);
	for (int i = 0; i < count; i++)
	{
		// key
		Str name;
		data_read_string(pos, &name);

		// find variable and set value
		auto var = config_find(self, &name);
		if (unlikely(var == NULL))
			error("config: unknown option '%.*s'",
			      str_size(&name), str_of(&name));

		if (unlikely(! var_is(var, VAR_C)))
			error("config: option '%.*s' cannot be changed",
			      str_size(&name), str_of(&name));

		switch (var->type) {
		case VAR_BOOL:
		{
			if (unlikely(! data_is_bool(*pos)))
				error("config: bool expected for option '%.*s'",
				      str_size(&name), str_of(&name));
			bool value;
			data_read_bool(pos, &value);
			var_int_set(var, value);
			break;
		}
		case VAR_INT:
		{
			if (unlikely(! data_is_integer(*pos)))
				error("config: integer expected for option '%.*s'",
				      str_size(&name), str_of(&name));
			int64_t value;
			data_read_integer(pos, &value);
			var_int_set(var, value);
			break;
		}
		case VAR_STRING:
		{
			if (unlikely(! data_is_string(*pos)))
				error("config: string expected for option '%.*s'",
				      str_size(&name), str_of(&name));
			Str value;
			data_read_string(pos, &value);
			var_string_set(var, &value);
			break;
		}
		case VAR_DATA:
		{
			auto start = *pos;
			data_skip(pos);
			var_data_set(var, start, *pos - start);
			break;
		}
		default:
			error("config: bad option '%.*s' value",
			      str_size(&name), str_of(&name));
		}
	}
}

static void
config_set(Config* self, Str* options)
{
	Json json;
	json_init(&json);
	guard(json_guard, json_free, &json);
	json_parse(&json, options);
	uint8_t* pos = json.buf.start;
	config_set_data(self, &pos);
}

static void
config_list_persistent(Config* self, Buf* buf)
{
	encode_map(buf, self->count_persistent);
	list_foreach(&self->list)
	{
		auto var = list_at(Var, link);
		if (var_is(var, VAR_E))
			continue;
		encode_string(buf, &var->name);
		var_encode(var, buf);
	}
}

static void
config_save_to(Config* self, const char* path)
{
	// get a list of variables
	Buf buf;
	buf_init(&buf);
	guard(guard, buf_free, &buf);
	config_list_persistent(self, &buf);

	// convert to json
	Buf text;
	buf_init(&text);
	guard(guard_text, buf_free, &text);
	uint8_t* pos = buf.start;
	json_export_pretty(&text, &pos);

	// create config file
	File file;
	file_init(&file);
	guard(file_guard, file_close, &file);
	file_create(&file, path);
	file_write_buf(&file, &text);

	// sync
	file_sync(&file);
}

void
config_save(Config* self, const char* path)
{
	// remove prevv saved config, if exists
	Buf buf;
	buf_init(&buf);
	guard(guard, buf_free, &buf);
	if (fs_exists("%s.prev", path))
		fs_unlink("%s.prev", path);

	// save existing config as a previous
	fs_rename(path,  "%s.prev", path);

	// create config file
	config_save_to(self, path);

	// remove prev config file
	fs_unlink("%s.prev", path);
}

void
config_open(Config* self, const char* path)
{
	if (fs_exists("%s", path))
	{
		Buf buf;
		buf_init(&buf);
		guard(guard, buf_free, &buf);
		file_import(&buf, "%s", path);
		Str options;
		str_init(&options);
		buf_str(&buf, &options);
		config_set(self, &options);
		return;
	}

	// create config file
	config_save_to(self, path);
}

void
config_print_log(Config* self)
{
	list_foreach(&self->list)
	{
		auto var = list_at(Var, link);
		if (var_is(var, VAR_H) || var_is(var, VAR_S))
			continue;
		var_print_log(var);
	}
}

void
config_print(Config* self, Buf* buf)
{
	list_foreach(&self->list)
	{
		auto var = list_at(Var, link);
		if (var_is(var, VAR_H) || var_is(var, VAR_S))
			continue;
		var_print(var, buf);
	}
}

Var*
config_find(Config* self, Str* name)
{
	list_foreach(&self->list) {
		auto var = list_at(Var, link);
		if (str_compare(&var->name, name))
			return var;
	}
	return NULL;
}
