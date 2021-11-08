#include "math.h"
#define ANDREI 10

int do_something();

#if 1
void print_something();
#endif

#if 0
	void false_if_test(void);
#elif 0
		void true_elif_test(void);
#elif 1
void false_if_false_elif_true_elif(void);
#elif 0
		void false_if_false_elif_true_elif_false_elif(void);
#else
	void false_if_false_elif_true_else(void);
#endif