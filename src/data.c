#include <stdlib.h>
#include <errno.h>
#include "iso8583.h"
#include "functions.h"

// <DEV>
#include "test/test.h"
#include "test/strings.h"
#include <stdio.h>
// </DEV>


static const struct {
	iso8583_value_type_t value_type;
	iso8583_field_type_t type;
	int                  digits;
	int                  length;
} field_type_info[] = {
	{ unknown, unknown              , 0,   0 }, // [  0] (Don't exists!)
	{ unknown, binary_field         , 0,  64 }, // [  1] Bitmap
	{ unknown, numeric              , 2,  19 }, // [  2] Primary account number (PAN)
	{ unknown, numeric              , 0,   6 }, // [  3] Processing Code
	{ unknown, numeric              , 0,  12 }, // [  4] Amount Transaction
	{ unknown, numeric              , 0,  12 }, // [  5] Amount, settlement
	{ unknown, numeric              , 0,  12 }, // [  6] Amount, cardholder billing
	{ unknown, numeric              , 0,  10 }, // [  7] Transmission date & time
	{ unknown, numeric              , 0,   8 }, // [  8] Amount, cardholder billing fee
	{ unknown, numeric              , 0,   8 }, // [  9] Conversion rate, settlement
	{ unknown, numeric              , 0,   8 }, // [ 10] Conversion rate, cardholder billing
	{ unknown, numeric              , 0,   6 }, // [ 11] System trace audit number (STAN)
	{ string , numeric              , 0,   6 }, // [ 12] Local transaction time (hhmmss)
	{ string , numeric              , 0,   4 }, // [ 13] Local transaction date (MMDD)
	{ unknown, numeric              , 0,   4 }, // [ 14] Expiration date
	{ unknown, numeric              , 0,   4 }, // [ 15] Settlement date
	{ unknown, numeric              , 0,   4 }, // [ 16] Currency conversion date
	{ unknown, numeric              , 0,   4 }, // [ 17] Capture date
	{ unknown, numeric              , 0,   4 }, // [ 18] Merchant type, or merchant category code
	{ unknown, numeric              , 0,   3 }, // [ 19] Acquiring institution (country code)
	{ unknown, numeric              , 0,   3 }, // [ 20] PAN extended (country code)
	{ unknown, numeric              , 0,   3 }, // [ 21] Forwarding institution (country code)
	{ unknown, numeric              , 0,   3 }, // [ 22] Point of service entry mode
	{ unknown, numeric              , 0,   3 }, // [ 23] Application PAN sequence number
	{ unknown, numeric              , 0,   3 }, // [ 24] Function code (ISO 8583:1993), or network international identifier (NII)
	{ unknown, numeric              , 0,   2 }, // [ 25] Point of service condition code
	{ unknown, numeric              , 0,   2 }, // [ 26] Point of service capture code
	{ unknown, numeric              , 0,   1 }, // [ 27] Authorizing identification response length
	{ unknown, x_numeric            , 0,   8 }, // [ 28] Amount, transaction fee
	{ unknown, x_numeric            , 0,   8 }, // [ 29] Amount, settlement fee
	{ unknown, x_numeric            , 0,   8 }, // [ 30] Amount, transaction processing fee
	{ unknown, x_numeric            , 0,   8 }, // [ 31] Amount, settlement processing fee
	{ unknown, numeric              , 2,  11 }, // [ 32] Acquiring institution identification code
	{ unknown, numeric              , 2,  11 }, // [ 33] Forwarding institution identification code
	{ unknown, numeric_special      , 2,  28 }, // [ 34] Primary account number, extended
	{ unknown, code_set             , 2,  37 }, // [ 35] Track 2 data
	{ unknown, numeric              , 3, 104 }, // [ 36] Track 3 data
	{ unknown, alpha_numeric        , 0,  12 }, // [ 37] Retrieval reference number
	{ unknown, alpha_numeric        , 0,   6 }, // [ 38] Authorization identification response
	{ unknown, alpha_numeric        , 0,   2 }, // [ 39] Response code
	{ unknown, alpha_numeric        , 0,   3 }, // [ 40] Service restriction code
	{ unknown, alpha_numeric_special, 0,   8 }, // [ 41] Card acceptor terminal identification
	{ unknown, alpha_numeric_special, 0,  15 }, // [ 42] Card acceptor identification code
	{ unknown, alpha_numeric_special, 0,  40 }, // [ 43] Card acceptor name/location (1–23 street address, –36 city, –38 state, 39–40 country)
	{ unknown, alpha_numeric        , 2,  25 }, // [ 44] Additional response data
	{ unknown, alpha_numeric        , 2,  76 }, // [ 45] Track 1 data
	{ unknown, alpha_numeric        , 3, 999 }, // [ 46] Additional data (ISO)
	{ unknown, alpha_numeric        , 3, 999 }, // [ 47] Additional data (national)
	{ unknown, alpha_numeric        , 3, 999 }, // [ 48] Additional data (private)
	{ unknown, alpha_numeric        , 0,   3 }, // [ 49] Currency code, transaction         (It is alpha OR numeric!)
	{ unknown, alpha_numeric        , 0,   3 }, // [ 50] Currency code, settlement          (It is alpha OR numeric!)
	{ unknown, alpha_numeric        , 0,   3 }, // [ 51] Currency code, cardholder billing  (It is alpha OR numeric!)
	{ unknown, binary_field         , 0,  64 }, // [ 52] Personal identification number data
	{ unknown, numeric              , 0,  16 }, // [ 53] Security related control information
	{ unknown, alpha_numeric        , 3, 120 }, // [ 54] Additional amounts
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 55] ICC data – EMV having multiple tags
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 56] Reserved (ISO)
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 57] Reserved (national)
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 58] Reserved (national)
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 59] Reserved (national)
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 60] Reserved (national) (e.g. settlement request: batch number, advice transactions: original transaction amount, batch upload: original MTI plus original RRN plus original STAN, etc.)
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 61] Reserved (private)  (e.g. CVV2/service code   transactions)
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 62] Reserved (private)  (e.g. transactions: invoice number, key exchange transactions: TPK key, etc.)
	{ unknown, alpha_numeric_special, 3, 999 }, // [ 63] Reserved (private)
	{ unknown, binary_field         , 0,  64 }, // [ 64] Message authentication code (MAC)
	{ unknown, binary_field         , 0,   1 }, // [ 65] Extended bitmap indicator
	{ unknown, numeric              , 0,   1 }, // [ 66] Settlement code
	{ unknown, numeric              , 0,   2 }, // [ 67] Extended payment code
	{ unknown, numeric              , 0,   3 }, // [ 68] Receiving institution country code
	{ unknown, numeric              , 0,   3 }, // [ 69] Settlement institution country code
	{ unknown, numeric              , 0,   3 }, // [ 70] Network management information code
	{ unknown, numeric              , 0,   4 }, // [ 71] Message number
	{ unknown, numeric              , 0,   4 }, // [ 72] Last message's number
	{ string , numeric              , 0,   6 }, // [ 73] Action date (YYMMDD)
	{ unknown, numeric              , 0,  10 }, // [ 74] Number of credits
	{ unknown, numeric              , 0,  10 }, // [ 75] Credits, reversal number
	{ unknown, numeric              , 0,  10 }, // [ 76] Number of debits
	{ unknown, numeric              , 0,  10 }, // [ 77] Debits, reversal number
	{ unknown, numeric              , 0,  10 }, // [ 78] Transfer number
	{ unknown, numeric              , 0,  10 }, // [ 79] Transfer, reversal number
	{ unknown, numeric              , 0,  10 }, // [ 80] Number of inquiries
	{ unknown, numeric              , 0,  10 }, // [ 81] Number of authorizations
	{ unknown, numeric              , 0,  12 }, // [ 82] Credits, processing fee amount
	{ unknown, numeric              , 0,  12 }, // [ 83] Credits, transaction fee amount
	{ unknown, numeric              , 0,  12 }, // [ 84] Debits, processing fee amount
	{ unknown, numeric              , 0,  12 }, // [ 85] Debits, transaction fee amount
	{ unknown, numeric              , 0,  16 }, // [ 86] Total amount of credits
	{ unknown, numeric              , 0,  16 }, // [ 87] Credits, reversal amount
	{ unknown, numeric              , 0,  16 }, // [ 88] Total amount of debits
	{ unknown, numeric              , 0,  16 }, // [ 89] Debits, reversal amount
	{ unknown, numeric              , 0,  42 }, // [ 90] Original data elements
	{ unknown, alpha_numeric        , 0,   1 }, // [ 91] File update code
	{ unknown, alpha_numeric        , 0,   2 }, // [ 92] File security code
	{ unknown, alpha_numeric        , 0,   5 }, // [ 93] Response indicator
	{ unknown, alpha_numeric        , 0,   7 }, // [ 94] Service indicator
	{ unknown, alpha_numeric        , 0,  42 }, // [ 95] Replacement amounts
	{ unknown, binary_field         , 0,  64 }, // [ 96] Message security code
	{ unknown, x_numeric            , 0,  16 }, // [ 97] Net settlement amount
	{ unknown, alpha_numeric_special, 0,  25 }, // [ 98] Payee
	{ unknown, numeric              , 2,  11 }, // [ 99] Settlement institution identification code
	{ unknown, numeric              , 2,  11 }, // [100] Receiving institution identification code
	{ unknown, alpha_numeric_special, 2,  17 }, // [101] File name
	{ unknown, alpha_numeric_special, 2,  28 }, // [102] Account identification 1
	{ unknown, alpha_numeric_special, 2,  28 }, // [103] Account identification 2
	{ unknown, alpha_numeric_special, 3, 100 }, // [104] Transaction description
	{ unknown, alpha_numeric_special, 3, 999 }, // [105] Reserved for ISO use
	{ unknown, alpha_numeric_special, 3, 999 }, // [106] Reserved for ISO use
	{ unknown, alpha_numeric_special, 3, 999 }, // [107] Reserved for ISO use
	{ unknown, alpha_numeric_special, 3, 999 }, // [108] Reserved for ISO use
	{ unknown, alpha_numeric_special, 3, 999 }, // [109] Reserved for ISO use
	{ unknown, alpha_numeric_special, 3, 999 }, // [110] Reserved for ISO use
	{ unknown, alpha_numeric_special, 3, 999 }, // [111] Reserved for ISO use
	{ unknown, alpha_numeric_special, 3, 999 }, // [112] Reserved for national use
	{ unknown, alpha_numeric_special, 3, 999 }, // [113] Reserved for national use
	{ unknown, alpha_numeric_special, 3, 999 }, // [114] Reserved for national use
	{ unknown, alpha_numeric_special, 3, 999 }, // [115] Reserved for national use
	{ unknown, alpha_numeric_special, 3, 999 }, // [116] Reserved for national use
	{ unknown, alpha_numeric_special, 3, 999 }, // [117] Reserved for national use
	{ unknown, alpha_numeric_special, 3, 999 }, // [118] Reserved for national use
	{ unknown, alpha_numeric_special, 3, 999 }, // [119] Reserved for national use
	{ unknown, alpha_numeric_special, 3, 999 }, // [120] Reserved for private use
	{ unknown, alpha_numeric_special, 3, 999 }, // [121] Reserved for private use
	{ unknown, alpha_numeric_special, 3, 999 }, // [122] Reserved for private use
	{ unknown, alpha_numeric_special, 3, 999 }, // [123] Reserved for private use
	{ unknown, alpha_numeric_special, 3, 999 }, // [124] Reserved for private use
	{ unknown, alpha_numeric_special, 3, 999 }, // [125] Reserved for private use
	{ unknown, alpha_numeric_special, 3, 999 }, // [126] Reserved for private use
	{ unknown, alpha_numeric_special, 3, 999 }, // [127] Reserved for private use
	{ unknown, binary_field         , 0,  64 }, // [128] Message authentication code
	{ unknown, unknown              , 0,   0 }  // [129] (Don't exist!)
};

int iso8583_data_setup(const unsigned char *input, iso8583_bitmap_t *bitmaps, iso8583_fields_t *elements)
{
	if(input == NULL || bitmaps == NULL || elements == NULL) {
		errno = ERR_ISO8583_NULL_ARG;
		return -1;
	}

	if(elements->fields == NULL) {

		elements->count = bitmaps->field_count;

		elements->fields = (iso8583_field_t *)calloc(elements->count, sizeof(iso8583_field_t));
		if(elements->fields == NULL) {
			errno = ERR_ISO8583_NO_MEMORY;
			return -1;
		}

		for(int i = 1, j = 0; i <= 192; i++) {
			if(bitmaps->field[i]) {
				elements->fields[j].field = i;
				j++;
			}
		}
	}

	return 0;
}

int iso8583_data_hex(const unsigned char *input, int size, iso8583_bitmap_t *bitmaps, iso8583_fields_t *elements)
{
	unsigned char *bin;
	         int   bin_size;
	         int   result;

	if(iso8583_data_setup(input, bitmaps, elements) != 0) {
		// errno come from iso8583_data_setup()
		return -1;
	}

	bin = hex2bin_sec(input, size, &bin_size);
	if(bin == NULL) {
		// errno come from hex2bin()
		return -1;
	}

	result = iso8583_data_bin(bin, bin_size, bitmaps, elements);

	free(bin);

	return result;
}

int iso8583_data_bin(const unsigned char *input, int size, iso8583_bitmap_t *bitmaps, iso8583_fields_t *elements)
{
	int data_size;

	if(iso8583_data_setup(input, bitmaps, elements) != 0) {
		// errno come from iso8583_data_setup()
		return -1;
	}

printf("\n\n");
	for(int i = 0; i < elements->count; i++) {
//printf("[%d]", i);
		data_size = iso8583_data_add(input, size, &elements->fields[i]);
		if(data_size < 0) {
			// errno come from iso8583_data_add()
			return -1;
		}
		size-= data_size;
		if(size < 0) {
			errno = ERR_ISO8583_INSUFFICIENT_INPUT;
			return -1;
		}
		input = &input[data_size];
	}
printf("input size remaining {%d}\n", size);
	if(size != 0) {
		errno = ERR_ISO8583_EXCESS_INPUT;
		return -1;
	}

	return 0;
}

void print_bin(const unsigned char *input, int size);
void print_bin(const unsigned char *input, int size)
{
	printf("   +--------------------------------------------------------------+-----------------------+\n");
	for(int i = 0; i < size; i++) {
		if(i % 20 == 0) printf("   |");
		printf(" %02x", input[i]);
		if((i + 1) % 10 == 0) printf(" ");
		if((i + 1) % 20 == 0) {
			printf("|");
			for(int j = i - 19; j <= i; j++) printf("%s%c", j % 10 == 0 ? " " : "", input[j] < 32 || input[j] > 126 ? '.' : input[j]);
			printf(" | %3d~%d\n", i - 18, (i + 1));
		}
	}
	if(size % 20 > 0) {
		int rest = 20 - (size % 20);
		printf("%*s |", (rest * 3) + ( rest > 10 ? 1 : 0 ), "");
		for(int j = size - (size % 20); j < size; j++) printf("%s%c", j % 10 == 0 ? " " : "", input[j] < 32 || input[j] > 126 ? '.' : input[j]);
		printf("%*s | %3d~%d\n", rest + ( rest >= 10 ? 1 : 0 ), "", size - (size % 20) + 1, size);
	}
	printf("   +--------------------------------------------------------------+-----------------------+\n");
}

int iso8583_data_add(const unsigned char *input, int size, iso8583_field_t *element)
{
	int length        = 0;
	int length_length = 0;

	if(input == NULL || element == NULL) {
		errno = ERR_ISO8583_NULL_ARG;
		return -1;
	}

int cols = ( getenv("COLUMNS") == NULL ? 99 : atoi(getenv("COLUMNS")) );
int len = printf("---[%3d]---[" COLOR_CYAN "%s" COLOR_RESET "]", element->field, str_field_description(element->field))
		- snprintf(NULL, 0, COLOR_CYAN COLOR_RESET);
for(int i = len; i < cols; i++) { printf("-"); } printf("\n");

printf("iso8583_data_add()          Field [%3d] Type " COLOR_WHITE "%s" COLOR_RESET " / Digits %s%d" COLOR_RESET " / Length %s%d" COLOR_RESET "\n", 
	element->field,
																str_field_type(field_type_info[element->field].type  ),
	( field_type_info[element->field].digits > 0 ? COLOR_RED  : COLOR_GREEN ), field_type_info[element->field].digits ,
	( field_type_info[element->field].digits > 0 ? COLOR_BLUE : COLOR_CYAN  ), field_type_info[element->field].length
);

printf("iso8583_data_add()          Input {%d}:\n", size); print_bin(input, size);

	switch(field_type_info[element->field].digits) {
		// Fixed
		case 0:
			length = field_type_info[element->field].length;
		break;

		// Digits
		case 1:
		case 2:
		case 3:
			length = iso8583_data_get_length(&input, &size, &length_length, field_type_info[element->field].digits, field_type_info[element->field].type);
			if(length == -1) {
printf("iso8583_data_add()          -> -1\n");
for(int i = 0; i < cols; i++) { printf("-"); } printf("\n");
				// errno come from iso8583_data_get_length()
				return -1;
			}
printf("iso8583_data_add()          Length received " COLOR_CYAN "%d" COLOR_RESET " / Field info length " COLOR_BLUE "%d" COLOR_RESET, length, field_type_info[element->field].length);
			if(length > field_type_info[element->field].length) {
printf(" " COLOR_ALERT "Informed length oversized" COLOR_RESET "\n");
for(int i = 0; i < cols; i++) { printf("-"); } printf("\n");
				errno = ERR_ISO8583_SUPERIOR_INPUT_FIELD_LENGTH;
				return -1;
			}
printf(" -> " COLOR_GREEN "Ok" COLOR_RESET "\n");
		break;

		default:
for(int i = 0; i < cols; i++) { printf("-"); } printf("\n");
			errno = ERR_INTERNAL_ISO8583_INVALID_FIELD_DIGITS;
			return -1;
		break;
	}
printf("iso8583_data_add()          Length to read " COLOR_CYAN "%d" COLOR_RESET "\n", length);

printf("iso8583_data_add()          Size %d / Length %d", size, length);
	if(size < length) {
printf(" " COLOR_ALERT "Corrupted data. Missing data." COLOR_RESET "\n");
for(int i = 0; i < cols; i++) { printf("-"); } printf("\n");
		errno = ERR_ISO8583_INSUFFICIENT_INPUT;
		return -1;
	}
printf(" -> " COLOR_GREEN "Ok" COLOR_RESET "\n");

printf("iso8583_data_add()          Data to processes {%d}:\n", length); print_bin(input, length);

	switch(field_type_info[element->field].type) {
		case alpha                : length = length_length + iso8583_data_get_a  (input, size, length, element); break;
		case numeric              : length = length_length + iso8583_data_get_n  (input, size, length, element, field_type_info[element->field].value_type != string); break;
		case special              : length = length_length + iso8583_data_get_s  (input, size, length, element); break;
		case alpha_numeric        : length = length_length + iso8583_data_get_an (input, size, length, element); break;
		case alpha_special        : length = length_length + iso8583_data_get_as (input, size, length, element); break;
		case numeric_special      : length = length_length + iso8583_data_get_ns (input, size, length, element); break;
		case alpha_numeric_special: length = length_length + iso8583_data_get_ans(input, size, length, element); break;
		case x_numeric            : length = length_length + iso8583_data_get_xn (input, size, length, element); break;
		case binary_field         : length = length_length + iso8583_data_get_b  (input, size, length, element); break;
		case code_set             : length = length_length + iso8583_data_get_z  (input, size, length, element); break;
		// Fail
		default:
printf("iso8583_data_add()          -> -1\n\n");
for(int i = 0; i < cols; i++) { printf("-"); } printf("\n");
			errno = ERR_INTERNAL_ISO8583_INVALID_FIELD_TYPE;
			return -1;
		break;
	}
printf("iso8583_data_add()          -> %d\n", length);

for(int i = 0; i < cols; i++) { printf("-"); } printf("\n");
printf("\n");

	return length;
}

int iso8583_data_get_length(const unsigned char **input, int *size, int *length_length, int digits, iso8583_field_type_t type)
{
	int length;

int old_size = *size;
//printf("iso8583_data_get_length()   Type %s / Digits %d\n", str_field_type(type), digits);
printf("iso8583_data_get_length()   Data to processes {%d}:\n", digits); print_bin(*input, digits);

	// Non compressed
	if((*input)[0] != '\0') {
printf("iso8583_data_get_length()   " COLOR_YELLOW "Non compressed" COLOR_RESET "\n");
printf("iso8583_data_get_length()   Size %d / Digits %d", *size, digits);
		if(*size < digits) {
printf(" " COLOR_ALERT "Corrupted data. Missing data." COLOR_RESET "\n");
			errno = ERR_ISO8583_INSUFFICIENT_INPUT;
			return -1;
		}
printf(" -> " COLOR_GREEN "Ok" COLOR_RESET "\n");
printf("iso8583_data_get_length()   Length Data {%d}[", digits); for(int i = 0; i < digits; i++) printf("%c", (*input)[i]); printf("]\n");
		length = atoi_sec(*input, *size, digits);
		if(length == -1) {
printf("iso8583_data_get_length()   Unexpected length data value. " COLOR_ALERT "Corrupted data. Missing data." COLOR_RESET "\n");
			errno = ERR_ISO8583_INVALID_FIELD_LENGTH;
			return -1;
		}
		*input         = &(*input)[digits];
		*size         -= digits;
		*length_length = digits;

	// Compressed
	} else {
printf("iso8583_data_get_length()   " COLOR_PURPLE "Compressed" COLOR_RESET "\n");

		length = 0;
		if(digits == 1) {
			if(*size < 1) {
				errno = ERR_ISO8583_INSUFFICIENT_INPUT;
			} else {
printf("iso8583_data_get_length()   Unexpected compressed field length on 1 digit field. " COLOR_ALERT "Corrupted data. Missing data." COLOR_RESET "\n");
				errno = ERR_ISO8583_INVALID_FIELD_LENGTH;
			}
			return -1;
		}
		if(digits >= 2) {
			if(*size < 1) {
				errno = ERR_ISO8583_INSUFFICIENT_INPUT;
				return -1;
			}
printf("iso8583_data_get_length()    %3d            + %3d = ", length,              (*input)[1]);
			length =  length       + (*input)[1];
printf("%d\n", length);
		}
		if(digits == 3) {
			if(*size < 3) {
				errno = ERR_ISO8583_INSUFFICIENT_INPUT;
				return -1;
			}
printf("iso8583_data_get_length()   (%3d < 8 = %3d) + %3d = ", length, length << 8, (*input)[2]);
			length = (length << 8) + (*input)[2];
printf("%d\n", length);
		}
		*input         = &(*input)[digits];
		*size         -= digits;
		*length_length = digits;
	}

printf("iso8583_data_get_length()   Length Data processed {%d}[", old_size - *size); for(int i = *size - old_size; i < 0; i++) printf("%02x%s", (*input)[i], i<-1?" ":""); printf("]\n");

printf("iso8583_data_get_length()   -> " COLOR_CYAN "%d" COLOR_RESET "\n", length);
	return length;
}

void iso8583_clear_elements(iso8583_fields_t *elements)
{
	iso8583_free_elements(elements);
}

void iso8583_free_elements(iso8583_fields_t *elements)
{
	if(elements != NULL) {
		for(int i = 0; i < elements->count; i++) {
			switch(elements->fields[i].type) {
				case string: free(elements->fields[i].value.string     ); break;
				case binary: free(elements->fields[i].value.binary.data); break;
				default:
			}
		}
		free(elements->fields);
		elements->fields = NULL;
		elements->count  = 0;
	}
}

