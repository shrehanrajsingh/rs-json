#include "rsvtable.h"

// hash method
unsigned long
djb2 (const char *str)
{
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  hash ^= (hash >> 16);

  return hash % RSVT_NODESIZE;
}

RS_API rsvt_t *
rsvt_new (void)
{
  rsvt_t *t = rsmalloc (sizeof (*t));

  for (size_t i = 0; i < RSVT_NODESIZE; i++)
    {
      t->nodes[i] = (struct _vt_node_s){
        .capacity = RSVT_NODE_KVSIZE,
        .size = 0,
        .vals = rsmalloc (RSVT_NODE_KVSIZE * sizeof (struct _vt_kv_s))
      };
    }

  return t;
}

RS_API void
rsvt_free (rsvt_t *t)
{
  for (size_t i = 0; i < RSVT_NODESIZE; i++)
    {
      for (size_t j = 0; j < t->nodes[i].size; j++)
        {
          rsfree (t->nodes[i].vals[j].name);
        }

      rsfree (t->nodes[i].vals);
    }
}

RS_API int
rsvt_add_key (rsvt_t *vt, const char *name, void *val)
{
  unsigned long hs = djb2 (name);

  struct _vt_kv_s v = { .name = rsstrdup (name), .val = val };
  struct _vt_node_s *r = &vt->nodes[hs];

  if (r->capacity == r->size)
    {
      r->capacity += RSVT_NODE_KVSIZE;
      r->vals = rsrealloc (r->vals, r->capacity * sizeof (*r->vals));
    }

  r->vals[r->size++] = v;

  return 0; // success
}

RS_API void *
rsvt_get_key (rsvt_t *vt, const char *name)
{
  unsigned long hs = djb2 (name);

  struct _vt_node_s *r = &vt->nodes[hs];
  int i = r->size - 1;

  if (!r->size)
    return NULL;

  while (i > -1 && strcmp (r->vals[i].name, name))
    i--;

  return r->vals[i].val;
}