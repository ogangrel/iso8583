#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include "iso8583.h"

void print_error(void);
void print_transaction(iso8583_t *transaction);

int main(int argc, char **argv)
{
	unsigned char *input = (unsigned char *)
		// Code
			"1234"
		// Bitmaps
			"\x22\x10\x00\x11\x02\xC0\x48\x04"
		// Data
			"0000010000000002000003D00000041100000000005A6Abc 0007Abc 000000000 8a0900000000000000100160123456789abcdef"
		;
	int size = 118;
	iso8583_t *transaction;

	transaction = iso8583(NULL, input, size);
	if(transaction == NULL) {
		print_error();
		return EXIT_FAILURE;
	}

	print_transaction(transaction);

	iso8583_free(transaction);

	return EXIT_SUCCESS;
}

void print_error(void)
{
	switch(errno) {
		case ERR_ISO8583_INVALID_ARG                 : fprintf(stderr, "Error: Passed an invalid argument.\n"); break;
		case ERR_ISO8583_NO_MEMORY                   : fprintf(stderr, "Error: Insufficient memory.\n"); break;
		case ERR_ISO8583_NULL_ARG                    : fprintf(stderr, "Error: Passed a null argument.\n"); break;
		case ERR_ISO8583_QUATERNARY_BITMAP           : fprintf(stderr, "Error: Unexpected a quaternary bitmap.\n"); break;
		case ERR_ISO8583_INSUFFICIENT_INPUT          : fprintf(stderr, "Error: Input data is short then expected.\n"); break;
		case ERR_ISO8583_EXCESS_INPUT                : fprintf(stderr, "Error: Input data is bigger then expected.\n"); break;
		case ERR_ISO8583_SUPERIOR_INPUT_FIELD_LENGTH : fprintf(stderr, "Error: An input field length is informed with a size superior over the protocol definition.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_LENGTH        : fprintf(stderr, "Error: Field length is corruped. Invalid catacter of compressed on a 1 digit length.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_A             : fprintf(stderr, "Error: Invalid field alpha.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_N             : fprintf(stderr, "Error: Invalid field numeric.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_S             : fprintf(stderr, "Error: Invalid field special.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_AN            : fprintf(stderr, "Error: Invalid field alpha numerical.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_AS            : fprintf(stderr, "Error: Invalid field alpha special.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_NS            : fprintf(stderr, "Error: Invalid field numeric special.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_ANS           : fprintf(stderr, "Error: Invalid field alpha numeric special.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_XN            : fprintf(stderr, "Error: Invalid field amount (xn).\n"); break;
		case ERR_ISO8583_INVALID_FIELD_B             : fprintf(stderr, "Error: Invalid field binary.\n"); break;
		case ERR_ISO8583_INVALID_FIELD_Z             : fprintf(stderr, "Error: Invalid field code.\n"); break;
		default:
			fprintf(stderr, "Error (undefined by ISO8583): %d %s\n", errno, strerror(errno));
	}
}

void print_transaction(iso8583_t *transaction)
{
	int n, i, j;
	
	// The Points of Interest are marked with: POI

	printf("MTI\n");
	printf("    mti.version  = %d\n",   /* POI */ transaction->mti.version );
	printf("    mti.message  = %d\n",   /* POI */ transaction->mti.message );
	printf("    mti.function = %d\n",   /* POI */ transaction->mti.function);
	printf("    mti.origin   = %d\n",   /* POI */ transaction->mti.origin  );
	printf("\n");

	printf("Bitmaps\n");
	printf("    Bitmap Count: %d\n", transaction->bitmaps.count);
	printf("    Fields:");
	for(i = 1, n = 0; i <= 192; i++ ) {
		if(transaction->bitmaps.field[i]) {
			printf("%s %d", n++>0?",":"", i);
		}
	}
	printf(".\n");
	printf("    Field Count: %d\n", transaction->bitmaps.field_count);
	printf("\n");

	printf("Fields Data\n");
	for(i = 0; i < transaction->elements.count; i++) {

		printf("    Field %3d ",        /* POI */ transaction->elements.fields[i].field);

		switch(/* POI */ transaction->elements.fields[i].type) {
			case number:
				printf("number %ld",    /* POI */ transaction->elements.fields[i].value.number);
			break;

			case string:
				printf("string \"%s\"", /* POI */ transaction->elements.fields[i].value.string);
			break;

			case binary:
				printf("binary (%d)",   /* POI */ transaction->elements.fields[i].value.binary.size);
				for(j = 0; j < transaction->elements.fields[i].value.binary.size; j++) {
					printf(" %02x",     /* POI */ transaction->elements.fields[i].value.binary.data[j]);
				}
				printf("\n");
			break;

			default:
				printf("unknown type\n");
		}
		printf("\n");
	}
}
