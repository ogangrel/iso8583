#ifndef PRINT_H
#define PRINT_H

#include "iso8583.h"

void print_mti     (iso8583_mti_t    *mti     );
void print_mti_test(iso8583_mti_t    *mti     , iso8583_version_t version, iso8583_message_t message, iso8583_function_t function, iso8583_origin_t origin);
void print_bitmap  (iso8583_bitmap_t *bitmap  );
void print_elements(iso8583_fields_t *elements, int with_descriptions);
void print_iso8583 (iso8583_t        *iso8583 );

#endif // PRINT_H
