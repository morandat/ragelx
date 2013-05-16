
#define RAGEL_DEFS \
    alphtype char; \
    getkey *(fpc);

#define __PARSER_NAME__ \
	C(__MACHINE_NAME__, Parser)

#define DO_BEGIN_REQUEST                           \
    if((action_req = parser->on_new_request))      \
        action_req(parser);
#define DO_END_REQUEST                               \
    if((action_req = parser->on_request_completed))  \
        action_req(parser);                          \
    fired = 1;                                       \
    fbreak;

#define ACTIONS(__name, type, ...) \
	ACTIONS_ ## type(__name, __VA_ARGS__)

#define ACTIONS_number(__name, ...) \
 START_number (__name)              \
 INCREMENT_number (__name)          \
 TRIGGER_number(__name)
#define ACTIONS_dependent(__name, ...) \
 START_number(__name)                  \
 INCREMENT_number(__name)              \
 TRIGGER_dependent(__name)

#define ACTIONS_string(__name, ...) \
 START_MARK_string (__name)         \
 TRIGGER_string(__name)

#define START_number(__name) \
 action mark_ ## __name {    \
	 parser->__name = 0;     \
 }

#define INCREMENT_number(__name) \
 action increment_ ## __name {   \
	 parser->__name *= 10;       \
	 parser->__name += fc - '0'; \
 }

#define TRIGGER_number(__name)                \
 action __name {                              \
	if((action_nb = parser->on_ ## __name)) { \
	 action_nb(parser, parser->__name);       \
	}                                         \
 }
#define TRIGGER_dependent(__name)             \
 action __name {                              \
    parser->__##__name##_reg = parser->__name;\
    if((action_nb = parser->on_ ## __name)) { \
     action_nb(parser, parser->__name);       \
    }                                         \
 }

#define START_MARK_string(__name) \
 action mark_ ## __name {         \
	parser->__name = fpc;         \
 }

#define TRIGGER_string(__name)                 \
 action __name {                               \
	if((action_str = parser->on_ ## __name)) { \
	 action_str(parser, parser->__name, fpc);  \
	}                                          \
 }

#define LOOP_ON(__reg, __target)   \
if(--parser->__reg) fgoto __target;

END_HEADER
