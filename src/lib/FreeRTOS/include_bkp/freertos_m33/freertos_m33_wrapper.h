#ifndef FREERTOS_M33_WRAPPER_H
#define FREERTOS_M33_WRAPPER_H

/* Include standard headers */
#include <stddef.h>
#include <stdint.h>

/* Include critical FreeRTOS types */
#include "../projdefs.h"  /* Relative path to original FreeRTOS projdefs.h */

/* Define TaskFunction_t if not already defined */
#ifndef TaskFunction_t
typedef void (*TaskFunction_t)( void * );
#endif

/* Define StackType_t and other types if needed */
#ifndef StackType_t
typedef uint32_t StackType_t;
#endif

#ifndef BaseType_t
typedef long BaseType_t;
#endif

#ifndef UBaseType_t
typedef unsigned long UBaseType_t;
#endif

/* Now include our M33 portable.h */
#include "portable.h"

#endif /* FREERTOS_M33_WRAPPER_H */