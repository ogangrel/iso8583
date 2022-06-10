#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "test.h"
#include "functions.h"
#include "strings.h"

void print_mti(iso8583_mti_t *mti)
{
	printf("    mti.version  = %d -> %s\n", mti->version ,  str_version(mti->version ));
	printf("    mti.message  = %d -> %s\n", mti->message ,  str_message(mti->message ));
	printf("    mti.function = %d -> %s\n", mti->function, str_function(mti->function));
	printf("    mti.origin   = %d -> %s\n", mti->origin  ,   str_origin(mti->origin  ));
	fflush(stdout);
}

void print_mti_test(iso8583_mti_t *mti, iso8583_version_t version, iso8583_message_t message, iso8583_function_t function, iso8583_origin_t origin)
{
#ifdef WITH_COLORS
	printf("    mti.version  = %d -> %28s %s" COLOR_RESET " %-28s\n", mti->version , str_version (mti->version ), mti->version  == version  ? COLOR_GREEN "==" : COLOR_RED "!=" , str_version (version ));
	printf("    mti.message  = %d -> %28s %s" COLOR_RESET " %-28s\n", mti->message , str_message (mti->message ), mti->message  == message  ? COLOR_GREEN "==" : COLOR_RED "!=" , str_message (message ));
	printf("    mti.function = %d -> %28s %s" COLOR_RESET " %-28s\n", mti->function, str_function(mti->function), mti->function == function ? COLOR_GREEN "==" : COLOR_RED "!=" , str_function(function));
	printf("    mti.origin   = %d -> %28s %s" COLOR_RESET " %-28s\n", mti->origin  , str_origin  (mti->origin  ), mti->origin   == origin   ? COLOR_GREEN "==" : COLOR_RED "!=" , str_origin  (origin  ));
#else
	printf("    mti.version  = %d -> %28s %s %s\n", mti->version , str_version (mti->version ), mti->version  == version  ? "==" : "!=" , str_version (version ));
	printf("    mti.message  = %d -> %28s %s %s\n", mti->message , str_message (mti->message ), mti->message  == message  ? "==" : "!=" , str_message (message ));
	printf("    mti.function = %d -> %28s %s %s\n", mti->function, str_function(mti->function), mti->function == function ? "==" : "!=" , str_function(function));
	printf("    mti.origin   = %d -> %28s %s %s\n", mti->origin  , str_origin  (mti->origin  ), mti->origin   == origin   ? "==" : "!=" , str_origin  (origin  ));
#endif
	fflush(stdout);
}

void print_bitmap(iso8583_bitmap_t *bitmap)
{
	printf("    Bitmap Count: %d\n", bitmap->count);
	printf("    field[%3d] = %d (Unused)\n", 0, bitmap->field[0]);
	printf(
		"    ------- Prinary Bitmap --------"
		 "   ----- Secondary Bitmap --------"
		 "   ------ Tertiary Bitmap --------"
	"\n");
	for(int i = 1; i <= 32; i++ ) {
		printf("    "
			"field[%3d] = %s   " COLOR_RESET
			"field[%3d] = %s   " COLOR_RESET
			"field[%3d] = %s   " COLOR_RESET
			"field[%3d] = %s   " COLOR_RESET
			"field[%3d] = %s   " COLOR_RESET
			"field[%3d] = %s"    COLOR_RESET
			"\n",
			i      , bitmap->field[i      ] ? COLOR_GREEN "1" : COLOR_DARK "0",
			i +  32, bitmap->field[i +  32] ? COLOR_GREEN "1" : COLOR_DARK "0",
			i +  64, bitmap->field[i +  64] ? COLOR_GREEN "1" : COLOR_DARK "0",
			i +  96, bitmap->field[i +  96] ? COLOR_GREEN "1" : COLOR_DARK "0",
			i + 128, bitmap->field[i + 128] ? COLOR_GREEN "1" : COLOR_DARK "0",
			i + 160, bitmap->field[i + 160] ? COLOR_GREEN "1" : COLOR_DARK "0" 
		);
	}
	fflush(stdout);
}

void print_elements(iso8583_fields_t *elements, int fields_description)
{
	printf("    Count: %d\n", elements->count);

int show = 0;
for(int i = 0; i < elements->count && (show = (elements->fields[i].type == unknown ? 0 : 1)) == 0; i++) {}
if(show) {
	if(fields_description) {
		for(int i = 0; i < elements->count; i++) {
			printf("    [%3d] %s\n", i, str_field_description(elements->fields[i].field));
		}
		if(elements->count > 0) printf("\n");
	}
	for(int i = 0; i < elements->count; i++) {
		printf("    [%3d] Field %3d / Type %-7s", i, elements->fields[i].field, str_value_type(elements->fields[i].type));
		switch(elements->fields[i].type) {
			case number: printf(" / Value       %ld",                                                    elements->fields[i].value.number); break;
			case string: printf(" / Value {%3d}[%s]", strlen_sec(elements->fields[i].value.string, 999), elements->fields[i].value.string); break;
			case binary: printf(" / Value {%3d}["   ,            elements->fields[i].value.binary.size                                   ); 
				for(int j = 0; j < elements->fields[i].value.binary.size; j++) {
					printf("%s%02x", i>0?" ":"", elements->fields[i].value.binary.data[j]);
				}
				printf("]\n");
			break;
			default:
		}
		printf("\n");
	}
}

	fflush(stdout);
}

void print_iso8583(iso8583_t *iso8583)
{
	printf("MTI - Message Type Indicator\n"); print_mti     (&iso8583->mti     );
	printf("Bitmaps - \"Fields maps\"\n"   ); print_bitmap  (&iso8583->bitmaps );
	printf("Fields\n"                      ); print_elements(&iso8583->elements, 1);
}
