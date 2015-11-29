#pragma once

#define BREAK_IF(e) if(e){break;}
#define NOT_FIND_KEY(e, s) ((e).find(s) == (e).end())
#define FIND_KEY(e, s) ((e).find(s) != (e).end())

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef bool bit;
typedef unsigned long long ullong;
typedef long long llong;
typedef int int32;
typedef long long int64;

#define IsNull(p)		((p) == NULL || (p) == nullptr)
#define IsNotNull(p)	((p) != NULL && (p) != nullptr)
#define IsZero(n)		((n) == 0)
#define IsNotZero(n)	((n) != 0)
