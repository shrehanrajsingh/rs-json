#include "rsmalloc.h"

RS_API void *
__rs_Malloc (size_t _Size)
{
  if (!_Size)
    return NULL;

  void *n = malloc (_Size);

  if (n == NULL)
    e_printf ("rsmalloc() Memory allocation failed");

  return n;
}

RS_API void *
__rs_Realloc (void *_Old, size_t _Size)
{
  if (!_Size)
    {
      rsfree (_Old);
      return NULL;
    }

  void *n = realloc (_Old, _Size);

  if (n == NULL)
    e_printf ("rsrealloc() Memory allocation failed");

  return n;
}

RS_API void
__rs_Free (void *_Ptr)
{
  if (_Ptr == NULL)
    return;

  free (_Ptr);
}