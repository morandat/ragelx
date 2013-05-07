#ifndef __PACKAGE__
#	define __PACKAGE__ fr.labri.jazzy.parser
#endif
#ifndef __RUNTIME__
#   define __RUNTIME__ fr.labri.jazzy.runtime
#endif

#ifndef __CLASS_NAME__
#	define __CLASS_NAME__ __MACHINE_NAME__
#endif

#define RAGEL_DEFS      \
 	alphtype byte;        \
	getkey data.get(fpc);

#define DO_BEGIN_REQUEST \
		on_new_request();
#define DO_END_REQUEST      \
	  on_request_completed(); \
	  fired = true;           \
		fbreak;

#define USE_MARK(__name, type, ...) \
  USE_MARK_ ## type (__name, type, __VA_ARGS__)

#define USE_MARK_number(__name, type, ...) /*
	*/ protected int __name ; /*
	*/ protected abstract void on_ ## __name (ByteBuffer data, int value);

#define USE_MARK_dependent(__name, type, ...) /*
	*/ protected int __name ; /*
	*/ protected int __##__name ## _reg ; /*
	*/ protected abstract void on_ ## __name (ByteBuffer data, int value);

#define USE_MARK_string(__name, type, ...) /*
	*/ protected final Mark __name = new Mark(); /*
	*/ protected abstract void on_ ## __name (ByteBuffer data, int pos);

#define TEST_IMPL(__name, type, ...) \
	TEST_IMPL_ ## type (__name, __VA_ARGS__)

#define TEST_IMPL_dependent(__name, ...) /*
	*/ protected void on_ ## __name (ByteBuffer data, int value) { /*
	*/     write(S(__name), value); /*
	*/ }

#define TEST_IMPL_number(__name, ...) /*
	*/ protected void on_ ## __name (ByteBuffer data, int value) { /*
	*/     write(S(__name), value); /*
	*/ }

#define TEST_IMPL_string(__name, ...) /*
	*/ protected void on_ ## __name (ByteBuffer data, int pos) { /*
	*/     write(S(__name), __name.get(data, pos)); /*
	*/ }

#define ACTIONS(__name, type, ...) \
	ACTIONS_ ## type(__name, __VA_ARGS__)

#define ACTIONS_number(__name, ...) \
 START_number (__name)                  \
 INCREMENT_number (__name)              \
 TRIGGER_number (__name)

#define ACTIONS_dependent(__name, ...) \
 START_number (__name)                  \
 INCREMENT_number (__name)              \
 TRIGGER_dependent (__name, __VA_ARGS__)

#define ACTIONS_string(__name, ...) \
 START_MARK_string (__name)             \
 TRIGGER_string (__name)

#define START_number(__name) \
 action mark_ ## __name {    \
	 __name = 0;               \
 }

#define INCREMENT_number(__name)   \
 action increment_ ## __name {          \
	 __name *= 10;                   \
	 __name += fc - Utils.ASCII7ZERO;\
 }

#define TRIGGER_number(__name)   \
 action __name {                 \
	on_ ## __name(data, __name);   \
 }

#define TRIGGER_dependent(__name, ...) \
 action __name {                       \
    __##__name##_reg = __name;             \
    on_ ## __name(data, __name);       \
 }


#define START_MARK_string(__name) \
 action mark_ ## __name {         \
	__name.set(data, fpc);          \
 }

#define TRIGGER_string(__name)   \
 action __name {                 \
	on_ ## __name(data, fpc + 1);  \
 }

#define LOOP_ON(__reg, __target) \
 if(--__reg > 0) fgoto __target;

END_HEADER
