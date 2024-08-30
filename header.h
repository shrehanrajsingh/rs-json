#if !defined(RSHEADER_H)
#define RSHEADER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#define RS_API
#define e_printf(...) printf (__VA_ARGS__)

#define here printf ("%s (%s): %d\n", __FILE__, __FUNCTION__, __LINE__)

#define JSON_ESUCCESS 0

#endif // RSHEADER_H
