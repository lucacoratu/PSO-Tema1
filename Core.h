#ifndef CORE_H
#define CORE_H

#include <stdlib.h>

//Checks if the pointer returned by the malloc is NULL and if it is exits the program with status 12 (precizat in tema)

#define MALLOC_ASSERT(ptr) if(ptr == NULL) \
							exit(12)

#endif // !
