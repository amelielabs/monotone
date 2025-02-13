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

typedef struct Token Token;
typedef struct Lex   Lex;

enum
{
	KEOF,
	// values
	KREAL,
	KINT,
	KSTRING,
	KNAME,
	KTRUE,
	KFALSE,
	// modificators
	KUSEC,
	KMSEC,
	KSEC,
	KMIN,
	KHOUR,
	KDAY,
	KWEEK,
	KMONTH,
	KYEAR,
	KKB,
	KKIB,
	KMB,
	KMIB,
	KGB,
	KGIB,
	KTB,
	KTIB,
	// keywowrds
	KRESET,
	KSET,
	KTO,
	KSHOW,
	KALL,
	KCONFIG,
	KCHECKPOINT,
	KSERVICE,
	KCREATE,
	KSTORAGE,
	KSTORAGES,
	KPARTITION,
	KPARTITIONS,
	KIF,
	KNOT,
	KEXISTS,
	KDROP,
	KPIPELINE,
	KALTER,
	KRENAME,
	KMOVE,
	KBETWEEN,
	KAND,
	KINTO,
	KREFRESH,
	KREBALANCE,
	KON,
	KCLOUD,
	KCLOUDS,
	KDOWNLOAD,
	KUPLOAD,
	KVERBOSE,
	KDEBUG,
	KWAL,
	KMEMORY
};

struct Token
{
	int id;
	union {
		uint64_t integer;
		double   real;
		struct {
			Str  string;
			bool string_escape;
		};
	};
};

struct Lex
{
	char* pos;
	char* end;
	bool  keywords;
	Token backlog[4];
	int   backlog_size;
};

static inline void
token_init(Token* tk)
{
	memset(tk, 0, sizeof(*tk));
}

void lex_init(Lex*);
void lex_start(Lex*, Str*);
void lex_keywords(Lex*, bool);
void lex_next(Lex*, Token*);
void lex_push(Lex*, Token*);
bool lex_if(Lex*, int, Token*);
bool lex_if_name(Lex*, Token*);
