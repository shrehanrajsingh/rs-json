#include "rsjson.h"

jval_t *__parse_val (char *_B);

RS_API json_t *
rs_json_new (void)
{
  json_t *t = rsmalloc (sizeof (*t));
  t->vt = rsvt_new ();

  return t;
}

RS_API char *
readfile (const char *_PathToFile)
{
  FILE *f = fopen (_PathToFile, "r");

  /**
   * `p` is capacity
   * `d` is size
   */
  size_t p = JSON_BUFFER_CAP, d = 0;

  char *result = rsmalloc (p);
  char c;

  while ((c = fgetc (f)) != EOF)
    {
      if (d == p)
        {
          p += 1024;
          result = rsrealloc (result, p);
        }

      result[d++] = c;
    }

  if (d == p)
    {
      p++;
      result = rsrealloc (result, p);
    }
  result[d++] = '\0';

  fclose (f);
  return result;
}

RS_API int
rs_json_parse (const char *b, json_t *inst)
{
  int i = 0;

  char key[32] = { 0 };
  size_t kl = 0;

  char saw_colon = 0;

  int curr_tok_type = JTYPE_EOF;

  size_t bsize = 0;
  size_t bcap = JSON_BUFFER_CAP;
  char *buf = rsmalloc (bcap * sizeof (*buf));

  int gb = 0;
  char in_str = 0;

  while (*b != '{')
    b++; /* Eat anything before `{` */
  b++;   /* Eat `{` */

  char reading_key = 0; /* stuff before colon */

  while (b[i])
    {
      char c = b[i];
      // printf ("%c %d %d\n", c, reading_key, saw_colon);

      if (!in_str && c == '}' && !gb)
        goto _ps_vl;

      if (c == '{' || c == '[')
        gb++;

      else if (c == '}' || c == ']')
        gb--;

      if (c == '\"')
        {
          if (!saw_colon) /* `key` section */
            {
              if (reading_key) /* stop reading key */
                key[kl] = '\0';

              reading_key = 1 - reading_key;
              goto end;
            }
          else /* value section */
            {
              if (i && b[i - 1] != '\\')
                in_str = 1 - in_str;
            }
        }

      if (reading_key)
        {
          assert (kl < 32);
          key[kl++] = c;

          goto end;
        }

      if (in_str)
        {
          if (bcap == bsize)
            {
              bcap += JSON_BUFFER_CAP;
              buf = rsrealloc (buf, bcap * sizeof (*buf));
            }

          buf[bsize++] = c;
          goto end;
        }

      if (c == ':' && !reading_key && !saw_colon)
        {
          saw_colon = 1;

          goto end;
        }

      if (c == ',' && saw_colon && !gb && !in_str)
        {
        _ps_vl:;
          // parse value
          jval_t *r = __parse_val (buf);

          rsvt_add_key (inst->vt, key, (void *)r);
          // _json_print_val (*r);

          /**
           * Do NOT free `buf`.
           * Instead, set its size to 0
           */
          bsize = 0;
          saw_colon = 0;
          kl = 0;

          goto end;
        }

      if (saw_colon)
        {
          if (bcap == bsize)
            {
              bcap += JSON_BUFFER_CAP;
              buf = rsrealloc (buf, bcap * sizeof (*buf));
            }

          buf[bsize++] = c;
        }

    end:
      i++;
    }

  return JSON_ESUCCESS;
}

jval_t *
__parse_val (char *b)
{
  jval_t *t = rsmalloc (sizeof (*t));
  t->type = JTYPE_EOF;

  while (*b == ' ' || *b == '\t' || *b == '\n')
    b++;

  if (*b == '\"')
    {
      b++;
      b[strlen (b) - 1] = '\0';

      t->type = JTYPE_STRING;
      t->v.strv = (json_string_t)rsstrdup (b);

      goto ret;
    }

  if (*b >= '0' && *b <= '9')
    {
      double num = 0.0f;
      double dpart = 0.0f;
      char saw_float;

      while (*b >= '0' && *b <= '9')
        {
          num = 10 * num + (*b - '0');
          b++;
        }

      if (*b == '.')
        {
          saw_float = 1;
          b++;
          int p = 0;
          while (*b >= '0' && *b <= '9')
            {
              dpart = dpart * 10 + (*b - '0');
              b++;
              p++;
            }

          dpart /= pow (10, p);
        }

      // printf ("%f %f\n", num, dpart);

      if (saw_float)
        {
          t->type = JTYPE_FLOAT;
          t->v.floatv = (float)num + dpart;
        }
      else
        {
          t->type = JTYPE_INT;
          t->v.intv = (int)num;
        }

      goto ret;
    }

  int i = 0;
  while (b[i])
    {
      char c = b[i];

      switch (c)
        {
        case '{':
          {
            json_t *vl_inst = rs_json_new ();

            if (rs_json_parse (b, vl_inst) != JSON_ESUCCESS)
              e_printf ("cannot parse json '%s'.\n", b);

            t->type = JTYPE_OBJ;
            t->v.objv = vl_inst;
            goto ret;
          }
          break;

        case '[':
          {
            // TODO
          }
          break;

        default:
          break;
        }

    end:
      i++;
    }

ret:
  return t;
}

RS_API void
_json_print_val (jval_t v)
{
  switch (v.type)
    {
    case JTYPE_BOOL:
      {
        printf ("bool: %d\n", v.v.boolv);
      }
      break;

    case JTYPE_EOF:
      {
        printf ("eof\n");
      }
      break;

    case JTYPE_FLOAT:
      {
        printf ("float: %f\n", v.v.floatv);
      }
      break;

    case JTYPE_INT:
      {
        printf ("int: %lld\n", v.v.intv);
      }
      break;

    case JTYPE_STRING:
      {
        printf ("string: %s\n", v.v.strv);
      }
      break;

    case JTYPE_OBJ:
      {
        printf ("object\n");
      }
      break;

    default:
      printf ("unknown json value `%d`\n", v.type);
      break;
    }
}

RS_API void *
rs_json_query (const char *q, json_t *t)
{
  json_t *ci = t; /* current instance */
  int i = 0;

  void *res;

#if defined(RSJSON_OPT_1) /* optimizations revision 1 */
  char buf[JSON_BUFFER_CAP] = { 0 };
#else
  char *buf = rsmalloc (JSON_BUFFER_CAP);
  int bcap = JSON_BUFFER_CAP;
#endif

  int bs = 0;

  while (q[i])
    {
      char c = q[i];

      if (c == '$')
        ci = t;

      if (c == '[')
        {
          int gb = 0;
          char saw_quote = 0;
          char saw_num = 0;
          i++;

          while (q[i])
            {
              if (q[i] == ' ' || q[i] == '\t')
                continue;

              if (q[i] == '\'' || q[i] == '\"')
                {
                  if (saw_quote
                      && q[i - 1] != '\\') /* we don't need to check anymore */
                    break;

                  else
                    {
                      /**
                       * Key is string.
                       * Set bs to so buffer
                       * only contains key contents.
                       */
                      saw_quote = 1;
                      bs = 0;
                      goto l2;
                    }
                }

              if (saw_quote)
                goto l1;

              if (q[i] >= '0' && q[i] <= '9')
                saw_num = 1;

              else
                {
                  /**
                   * Probably a syntax error where
                   * two numbers are written consequtively
                   * separated by whitespace characters.
                   * In that case, reset buffer
                   */
                  if (saw_num)
                    {
                      saw_num = 0;
                      bs = 0;
                    }
                }
#if defined(RSJSON_OPT_1)
              if (bs >= JSON_BUFFER_CAP)
                {
                  e_printf ("max buffer limit (%d) reached in '%s'.\n",
                            JSON_BUFFER_CAP, __FUNCTION__);
                }
#else
              if (bs == bcap)
                {
                  bcap += JSON_BUFFER_CAP;
                  buf = rsrealloc (buf, bcap * sizeof (*buf));
                }
#endif

              if (q[i] == ']' && !gb)
                break;

              if (q[i] == '(' || q[i] == '{' || q[i] == '[')
                gb++;

              else if (q[i] == ')' || q[i] == '}' || q[i] == ']')
                gb--;

            l0:
              if (saw_quote || saw_num)
                {
                l1:
                  buf[bs++] = q[i];
                }
            l2:
              i++;
            }

          assert (bs);

          buf[bs++] = '\0';
          // printf ("%s %d\n", buf, bs);

          if (saw_quote)
            {
              jval_t *g = rsvt_get_key (ci->vt, buf);

              if (g == NULL)
                e_printf ("key '%s' not found.\n", buf);

              else
                {
                  if (g->type == JTYPE_OBJ)
                    ci = g->v.objv;

                  res = rs_valcast (g);
                }
            }

          else if (saw_num)
            {
              // TODO
            }

          /* reset flags */
          bs = 0;
          goto end;
        }

    end:
      i++;
    }

ret:
  return res;
}

RS_API void *
rs_valcast (jval_t *v)
{
  void *r = NULL;

  switch (v->type)
    {
    case JTYPE_BOOL:
      r = (void *)&v->v.boolv;
      break;

    case JTYPE_FLOAT:
      r = (void *)&v->v.floatv;
      break;

    case JTYPE_INT:
      r = (void *)&v->v.intv;
      break;

    case JTYPE_OBJ:
      r = (void *)v->v.objv;
      break;

    case JTYPE_STRING:
      r = (void *)v->v.strv;
      break;

    default:
      break;
    }

  return r;
}