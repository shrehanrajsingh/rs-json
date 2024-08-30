#if !defined(RSMALLOC_H)
#define RSMALLOC_H

#include "header.h"

#define rsmalloc(X) __rs_Malloc (X)
#define rsrealloc(X, Y) __rs_Realloc (X, Y)
#define rsfree(X) __rs_Free (X)
#define rsstrdup(X) strdup (X)

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  RS_API void *__rs_Malloc (size_t _Size);
  RS_API void *__rs_Realloc (void *_Old, size_t _Size);
  RS_API void __rs_Free (void *_Ptr);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // RSMALLOC_H
