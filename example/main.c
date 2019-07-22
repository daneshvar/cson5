#include <omp.h>
#include <stdio.h>

#include "json5.h"

static size_t vsize(void* p) {
  return p ? 0 [(size_t*)p - 2] : 0;
}
typedef size_t (*msize_t)(void*);
// typedef void* (*realloc_t)(void*, size_t);

static __thread msize_t MSIZE = vsize;

// #define array_init(t) (t = 0)
// #define array_append(t, x) (t = REALLOC(t, (array_count(t) + 1) * sizeof(t[0])), t[array_count(t) - 1] = (x))
#define array_count(t) (MSIZE(t) / sizeof(t[0]))
// #define array_free(t) (REALLOC(t, 0), t = 0)

char* readfile(const char* file) {
  char* buf = 0;
  FILE* fp = fopen(file, "rb");
  if (fp) {
    fseek(fp, 0L, SEEK_END);
    size_t pos = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    buf = (char*)malloc(pos + 1);
    fread(buf, 1, pos, fp);
    buf[pos] = 0;
    fclose(fp);
  }
  return buf;
}

void dump_json_contents(json5_object* o, int indent);

#define ind(x) \
  for (int i = 0; i < x; ++i) printf(" ");

void dump_value(json5_object* o, int indent, bool is_inline, bool is_last) {
  json5_object* node = o;
  indent += 4;

  if (!is_inline) {
    ind(indent);
    printf("\"%s\": ", node->name);
  }

  /**/ if (node->type == json5_type_string)
    printf("\"%s\"", node->string);
  else if (node->type == json5_type_integer)
    printf("%lld", node->integer);
  else if (node->type == json5_type_real)
    printf("%.3llf", node->real);
  else if (node->type == json5_type_object)
    dump_json_contents(node, indent);
  else if (node->type == json5_type_true)
    printf("true");
  else if (node->type == json5_type_false)
    printf("false");
  else if (node->type == json5_type_null)
    printf("null");
  else /* json5_type_array */ {
    printf("[");
    int elemn = array_count(node->elements);
    for (int j = 0; j < elemn; ++j) {
      dump_value(node->elements + j, -4, true, true);

      if (j < elemn - 1) {
        printf(", ");
      }
    }
    printf("]");
  }

  if (!is_inline) {
    if (!is_last) {
      printf(",\n");
    } else {
      printf("\n");
    }
  }
}

void dump_json_contents(json5_object* o, int indent) {
  ind(indent - 4);
  printf("{\n");
  int cnt = array_count(o->nodes);

  for (int i = 0; i < cnt; ++i) {
    if (i < cnt - 1) {
      dump_value(o->nodes + i, indent, false, false);
    } else {
      dump_value(o->nodes + i, indent, false, true);
    }
  }

  ind(indent);

  if (indent > 0) {
    printf("}");
  } else {
    printf("}\n");
  }
}

char* source = "/* this is a comment */"
               " nil: null, "
               "\"+ľščťžýáíé=\": true, "
               "\"huge\": 2.2239333e5, "
               "// Hello, new comment \n "
               "\"array\": [+1,2,-3,4,5],     "
               "\"hello\": \"world\", "
               "\"abc\": 42.67, "
               "\"children\" : { \"a\": 1, \"b\": 2 }";

int main() {
  char* content = readfile("example.json5");
  json5_object root = { 0 };

  double time = omp_get_wtime();
  json5_error rc = json5_parse(&root, content, true);
  double delta = omp_get_wtime() - time;

  dump_json_contents(&root, 2);

  json5_free(&root);
  free(content);

  for (; 0;)
    ;

  return 0;
}
