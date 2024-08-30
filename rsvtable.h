#if !defined(RSVTABLE_H)
#define RSVTABLE_H

#include "header.h"
#include "rsmalloc.h"

#define RSVT_NODE_KVSIZE 16
#define RSVT_NODESIZE 128

struct _vt_kv_s
{
  char *name;
  void *val;
};

struct _vt_node_s
{
  struct _vt_kv_s *vals; /* size of array is 16 initially */
  size_t size;
  size_t capacity; /* initially `RSVT_NODE_KVSIZE` */
};

struct _rs_vt_s
{
  struct _vt_node_s nodes[RSVT_NODESIZE];
};

typedef struct _rs_vt_s rsvt_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  RS_API rsvt_t *rsvt_new (void);
  RS_API void rsvt_free (rsvt_t *_VTable);
  RS_API int rsvt_add_key (rsvt_t *_Vt, const char *_Name, void *_Val);
  RS_API void *rsvt_get_key (rsvt_t *_Vt, const char *_Key);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // RSVTABLE_H
