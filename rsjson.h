#if !defined(RSJSON_H)
#define RSJSON_H

#include "header.h"
#include "rsmalloc.h"
#include "rsvtable.h"

#define JSON_BUFFER_CAP 1024

typedef char *json_string_t;
typedef long long json_int_t;
typedef double json_float_t;
typedef char json_bool_t;

#define JSON_STRING(X) ((char *)(X))
#define JSON_INT(X) (*(json_int_t *)(X))
#define JSON_FLOAT(X) (*(json_float_t *)(X))
#define JSON_BOOL(X) (*(json_bool_t *)(X))

enum
{
  JTYPE_STRING = 0,
  JTYPE_INT = 1,
  JTYPE_FLOAT = 2,
  JTYPE_BOOL = 3,
  JTYPE_OBJ = 4,
  JTYPE_EOF,
};

struct _json_instance_s;
struct _val_s
{
  int type;

  union
  {
    json_string_t strv;
    json_int_t intv;
    json_float_t floatv;
    json_bool_t boolv;
    struct _json_instance_s *objv;

  } v;
};

struct _json_instance_s
{
  rsvt_t *vt;
};

typedef struct _val_s jval_t;
typedef struct _json_instance_s json_t;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  RS_API json_t *rs_json_new (void);
  RS_API char *readfile (const char *_PathToFile);
  RS_API int rs_json_parse (const char *_B, json_t *);
  RS_API void _json_print_val (jval_t);

  RS_API void *rs_json_query (const char *_Q, json_t *_Inst);
  RS_API void *rs_valcast (jval_t *);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // RSJSON_H
