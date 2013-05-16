
struct buffer {
	const char *position, *end_position;
};

struct ragel_parser {
	struct buffer buffer;
	int cs;
};

static struct buffer *getBuffer(struct ragel_parser *parser) {
		return &parser->buffer;
}

static void attach(struct buffer *buffer, const char *start, const char *stop) {
	buffer->position = start;
	buffer->end_position = stop;
}

static int remaining(struct buffer *buffer) {
	return buffer->end_position != buffer->position;
}
