REM include <errno.h>
REM include <string.h>
REM include <stdio.h>
REM include <assert.h>
REM include <stdlib.h>

struct buffer {
	const char *position, *end_position;
};

struct ragel_parser {
	struct buffer buffer;
	int cs;
};

#define STRUCT_DEF(__name, type, __dummy) \
  STRUCT_DEF_ ## type (__name, type, __dummy)

#define STRUCT_DEF_number(__name, type, __dummy) /*
	*/ int __name;                                 /*
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

%% write data;

struct __PARSER_NAME__ * C(alloc, __PARSER_NAME__)(void *(*alloc)(size_t)){
	return alloc(sizeof(struct __PARSER_NAME__));
}

void C(init, __MACHINE_NAME__)(struct __PARSER_NAME__ *parser) {
	int cs;
	%% write init;
	parser->parser.cs = cs;
	parser->parser.buffer.position = parser->parser.buffer.end_position = 0;
}

void attach(struct buffer *buffer, const char *start, const char *stop) {
	buffer->position = start;
	buffer->end_position = stop;
}

int remaining(struct buffer *buffer) {
	return buffer->end_position != buffer->position;
}

struct buffer *getBuffer(struct __PARSER_NAME__ *parser) {
	return &parser->parser.buffer;
}

int C(resume, __MACHINE_NAME__)(struct __PARSER_NAME__ *parser) {
	int cs = parser->parser.cs;
	const char *p = parser->parser.buffer.position;
	const char *pe = parser->parser.buffer.end_position;
	const char *eof = NULL;
	int fired = 0;
	void (*action_req) (struct __PARSER_NAME__ *);
	void (*action_nb) (struct __PARSER_NAME__ *, int);
	void (*action_str) (struct __PARSER_NAME__ *, const char*, const char*);

	%% write exec;

	parser->parser.buffer.position = p;
	return fired;
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

static void debug_on_new_request(struct __PARSER_NAME__ *parser) {
	fprintf(stderr, "NEW REQUEST: " S(__MACHINE_NAME__) "\r\n");
}
static void debug_on_request_completed(struct __PARSER_NAME__ *parser) {
	fprintf(stderr, "END REQUEST: " S(__MACHINE_NAME__) "\r\n");
}

MARKS(DEBUG_TMPL_FUN, )

void C(C(init, __MACHINE_NAME__), Debug)(struct __PARSER_NAME__ *parser) {
	C(init, __MACHINE_NAME__)(parser);
	parser->on_new_request = debug_on_new_request;
	parser->on_request_completed = debug_on_request_completed;
	MARKS(DEBUG_TMPL_INIT, )
}
