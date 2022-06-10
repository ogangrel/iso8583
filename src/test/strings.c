#include "strings.h"
#include "globals.h"

// These "else" are there only to supress compiling warnings.

const char *str_version(iso8583_version_t n)
{
	if(n < 0) return iso8583_version_string_negative [n * -1];
	     else return iso8583_version_string          [n     ];
}

const char *str_message(iso8583_message_t n)
{
	if(n < 0) return iso8583_message_string_negative [n * -1];
	     else return iso8583_message_string          [n     ];
}

const char *str_function(iso8583_function_t n)
{
	if(n < 0) return iso8583_function_string_negative[n * -1];
	     else return iso8583_function_string         [n     ];
}

const char *str_origin(iso8583_origin_t n)
{
	if(n < 0) return iso8583_origin_string_negative  [n * -1];
	     else return iso8583_origin_string           [n     ];
}

const char *str_field_type(iso8583_field_type_t n)
{
	          return iso8583_field_type_string       [n     ];
}

const char *str_value_type(iso8583_value_type_t n)
{
	          return iso8583_value_type_string       [n     ];
}

const char *str_field_description(int n)
{
	          return iso8583_field_description_string[n     ];
}
