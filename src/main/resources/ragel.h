#ifndef __MACHINE_NAME__
#	define __MACHINE_NAME__ UnknownkStateMachine
#endif

#define REM #

#define D(__x) REM __x

#define P(__t1, __t2) \
 __t1 ## __t2
#define C(__t1, __t2) \
 P(__t1, __t2)

#define SS(__text) \
	#__text
#define S(__text) \
 SS(__text)



#define NEW_MACHINE(__machine)  \
	action sw ## __machine {      \
		fnext MyBody;               \
	}

#define GOTO(__machine) \
	@sw ## __machine

#define SWITCH(__machine) \
	GOTO(__machine) @end_request 
	
#define REQUEST(...) \
	>start_request __VA_ARGS__ @end_request

#define EXTRACT(__mark, __type) \
	EXTRACT_ ## __type (__mark)

#define EXTRACT_string(__name) \
	>mark_##__name %__name

#define EXTRACT_sstring(__name) \
	>mark_##__name @__name

#define EXTRACT_number(__name) \
	>mark_##__name  $increment_##__name %__name

#ifndef BEGIN_REQUEST
# define BEGIN_REQUEST
#endif
#ifndef END_REQUEST
# define END_REQUEST
#endif

#define ACTION_REQUEST       \
	action start_request {     \
		BEGIN_REQUEST            \
		DO_BEGIN_REQUEST         \
	}                          \
	action end_request {       \
		DO_END_REQUEST           \
		END_REQUEST              \
	}


#define END_HEADER          \
	%%{                       \
	machine __MACHINE_NAME__; \
  RAGEL_DEFS                \
  ACTION_REQUEST            \
}%%
