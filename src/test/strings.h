#ifndef STRINGS_H
#define STRINGS_H

#include "iso8583.h"

// Prototypes

const char *str_version          (iso8583_version_t    n);
const char *str_message          (iso8583_message_t    n);
const char *str_function         (iso8583_function_t   n);
const char *str_origin           (iso8583_origin_t     n);
const char *str_field_type       (iso8583_field_type_t n);
const char *str_value_type       (iso8583_value_type_t n);
const char *str_field_description(int                  n);

#endif // STRINGS_H
