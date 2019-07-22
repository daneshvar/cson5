#ifndef JSON5_H
#define JSON5_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef JSON5_API
#  define JSON5_API
#endif

#ifndef JSON5_ASSERT
#  define JSON5_ASSERT assert(0)
#endif

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum json5_type {
  json5_type_object,
  json5_type_string,
  json5_type_multistring,
  json5_type_array,
  json5_type_integer,
  json5_type_real,
  json5_type_null,
  json5_type_false,
  json5_type_true,
} json5_type;

typedef enum json5_error {
  json5_error_none,
  json5_error_invalid_name,
  json5_error_invalid_value,
} json5_error;

typedef enum json5_style {
  json5_style_double_quote,
  json5_style_single_quote,
  json5_style_no_quotes,
} json5_style;

typedef struct json5_object {
  char* name;
  uint8_t type : 6;
  uint8_t quote_style : 2;
  union {
    struct json5_object* elements;
    struct json5_object* nodes;
    int64_t integer;
    double real;
    char* string;
  };
} json5_object;

JSON5_API int json5_parse(json5_object* root, char* source, bool strip_comments);
JSON5_API void json5_free(json5_object* root);

JSON5_API char* json5__parse_object(json5_object* obj, char* base, int* err_code);
JSON5_API char* json5__parse_value(json5_object* obj, char* base, int* err_code);
JSON5_API char* json5__parse_array(json5_object* obj, char* base, int* err_code);

#ifdef __cplusplus
}
#endif

#endif  // JSON5_H
