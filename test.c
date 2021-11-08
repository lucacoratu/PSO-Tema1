#include <stdio.h>

#define ANA 1

#include "test.h"

#define MARIUS (ANA + ANA)

#ifdef ANA
	#define ALTCEVA 1
#endif

#ifdef ANA
	#define SOMETHING 5
#else
	#define SOMETHING 7
#endif


#ifndef ANA
	#define OTHER 8
#else
	#define OTHER 10
#endif



#include "test2.h"

#include "IncludeDir.h"

int main(int argc, char** argv) {

	printf("ANA=%d", ANA);
	return 0;
}