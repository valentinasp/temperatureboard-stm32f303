//
// generic data types
//

#ifndef TYPE_H_
#define TYPE_H_

#ifndef __cplusplus  //bool is standard type in C++
#include "stdint.h"

typedef enum {
	false = 0,
	true = 1
} bool;

typedef int32_t     Int32;
typedef int16_t     Int16;
typedef int8_t      Int8;

typedef uint32_t    Uint32;
typedef uint16_t    Uint16;
typedef uint8_t     Uint8;

#endif

#endif       //TYPE_H_

