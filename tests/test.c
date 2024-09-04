#include <rsjson.h>

int
main (int argc, char const *argv[])
{
  json_t *t = rs_json_new ();

  char *fd = readfile ("../../tests/test.json");
  rs_json_parse (fd, t);

  // for (size_t i = 0; i < RSVT_NODESIZE; i++)
  //   {
  //     if (t->vt->nodes[i].size)
  //       {
  //         for (size_t j = 0; j < t->vt->nodes[i].size; j++)
  //           {
  //             char *key = t->vt->nodes[i].vals[j].name;
  //             jval_t *val = (jval_t *)t->vt->nodes[i].vals[j].val;

  //             printf ("key: %s\nval: ", key);
  //             _json_print_val (*val);
  //           }
  //       }
  //   }

  printf ("%d\n", (int)JSON_INT (rs_json_query ("$['arr'][0]", t)));
  printf ("%s\n", JSON_STRING (rs_json_query ("$['arr'][4]['7']", t)));

  json_t *j = rs_json_query ("$['arr'][4]", t);
  _json_print_val ((jval_t){ .type = JTYPE_OBJ, .v.objv = j });

  return printf ("Program Ended.\n") && 0;
}