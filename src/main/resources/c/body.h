REM ifndef __AS_RAGEL_HEADER__
REM  include <errno.h>
REM  include <string.h>
REM  include <stdio.h>
REM  include <assert.h>
REM  include <stdlib.h>
REM endif
REM  include <stddef.h>
REM  include <ragelx.h>

#define STRUCT_DEF(__name, type, __dummy) \
  STRUCT_DEF_ ## type (__name, type, __dummy)

#define STRUCT_DEF_number(__name, type, __dummy) /*
	*/ int __name;                                 /*
	*/ void (*on_##__name) (struct __PARSER_NAME__ *, int);

#define STRUCT_DEF_dependent(__name, type, __dummy)       /*
	*/ int __name;                                        /*
	*/ int __##__name ## _reg ;                           /*
	*/ void (*on_##__name) (struct __PARSER_NAME__ *, int);

#define STRUCT_DEF_string(__name, type, __dummy) /*
	*/ const char *__name;                        /*
	*/ void (*on_##__name) (struct __PARSER_NAME__*, const char*, const char*);

struct __PARSER_NAME__ {
	struct ragel_parser parser;
	void (*on_new_request)(struct __PARSER_NAME__ *);
	void (*on_request_completed)(struct __PARSER_NAME__ *);
	MARKS(STRUCT_DEF,)
};

struct __PARSER_NAME__* C(clone, __PARSER_NAME__)(struct __PARSER_NAME__ *templ, void *(*alloc)(size_t));
int C(resume, __MACHINE_NAME__)(struct __PARSER_NAME__ *parser, struct buffer *buffer);
struct __PARSER_NAME__* C(init, __MACHINE_NAME__)(struct __PARSER_NAME__ *parser);

REM ifndef __AS_RAGEL_HEADER__
%% write data;

struct __PARSER_NAME__ * C(clone, __PARSER_NAME__)(struct __PARSER_NAME__ *templ, void *(*alloc)(size_t)){
	return memcpy(alloc(sizeof(struct __PARSER_NAME__)), templ, sizeof(struct __PARSER_NAME__));
}

struct __PARSER_NAME__ * C(alloc, __PARSER_NAME__)(void *(*alloc)(size_t)){
	return alloc(sizeof(struct __PARSER_NAME__));
}

struct __PARSER_NAME__* C(init, __MACHINE_NAME__)(struct __PARSER_NAME__ *parser) {
	int cs;
	%% write init;
	parser->parser.cs = cs;
	return parser;
}

int C(resume, __MACHINE_NAME__)(struct __PARSER_NAME__ *parser, struct buffer *buffer) {
	int cs = parser->parser.cs;
	const char *p = buffer->position;
	const char *pe = buffer->position + buffer->length;
	const char *eof = NULL;
	int fired = 0;
	void (*action_req) (struct __PARSER_NAME__ *);
	void (*action_nb) (struct __PARSER_NAME__ *, int);
	void (*action_str) (struct __PARSER_NAME__ *, const char*, const char*);
    /* Only to remove unused */
    (void)eof; (void) action_nb; (void) action_str; (void) action_req;

	%% write exec;

	parser->parser.cs = cs;
	buffer->position = p;
	buffer->length = pe - p;
	return fired ? !buffer->length : 0;
}


REM ifdef __DUMP_BINARY
	REM define DUMP_FORMAT "%x, "
	REM define DUMP_FORMAT_LAST "%x"
REM else
	REM define DUMP_FORMAT "%c"
	REM define DUMP_FORMAT_LAST "%c"
REM endif

static void dumpNumber(FILE *out, char* prefix, int nb) {
	fprintf(out, "%s: %d\n", prefix, nb);
}

static void dumpBuffer(FILE *out, const char* prefix, const char *p, const char *pe) {
	fprintf(out, "%s: [", prefix);
	while(p != pe) {
		fprintf(out, DUMP_FORMAT, *p++);
	}
	fprintf(out, DUMP_FORMAT_LAST, *p);
	fprintf(out, "]\n");
}

#define DEBUG_TMPL_INIT(__name, type, ...) \
	parser->on_ ## __name = C(debug_on_, __name) ;

#define DEBUG_TMPL_FUN(__name, type, ...) \
	DEBUG_TMPL_FUN_## type(__name, __VA_ARGS__)
#define DEBUG_TMPL_FUN_string(__name, ...)           \
	static void debug_on_##__name(struct __PARSER_NAME__ *parser, const char* start, const char* stop) { \
		dumpBuffer(stderr, S(__name), start, stop);      \
	}
#define DEBUG_TMPL_FUN_number(__name, ...) \
	static void debug_on_##__name(struct __PARSER_NAME__*parser, int nb) {         \
		dumpNumber(stderr, S(__name), nb);     \
	}
#define DEBUG_TMPL_FUN_dependent(__name, ...) \
	static void debug_on_##__name(struct __PARSER_NAME__*parser, int nb) {         \
		dumpNumber(stderr, S(__name), nb);     \
	}
static void debug_on_new_request(struct __PARSER_NAME__ *parser) {
	fprintf(stderr, "NEW REQUEST: " S(__MACHINE_NAME__) "\r\n");
}
static void debug_on_request_completed(struct __PARSER_NAME__ *parser) {
	fprintf(stderr, "END REQUEST: " S(__MACHINE_NAME__) "\r\n");
}

MARKS(DEBUG_TMPL_FUN, )

void C(C(init, __MACHINE_NAME__), Debug)(struct __PARSER_NAME__ *parser) {
	C(init, __MACHINE_NAME__)(parser);
	(void)dumpNumber;
	(void)dumpBuffer;
	parser->on_new_request = debug_on_new_request;
	parser->on_request_completed = debug_on_request_completed;
	MARKS(DEBUG_TMPL_INIT, )
}
REM endif

