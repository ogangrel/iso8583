#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include "iso8583.h"
#include "functions.h"

#define IS_ALPHA(  CHAR)  ((CHAR >= 'a' && CHAR <= 'z') || (CHAR >= 'A' && CHAR <= 'Z') || CHAR == ' ')
#define IS_NUMERIC(CHAR)   (CHAR >= '0' && CHAR <= '9')
#define IS_SPECIAL(CHAR)    CHAR != '\0'

// <DEV>
#include <stdio.h>
// </DEV>

int iso8583_data_get_a(const unsigned char *input, int size, int length, iso8583_field_t *element)
{
	unsigned char c;

printf("iso8583_data_get_a()        Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	for(int i = 0; i < length; i++) {
		c = input[i];
		if(IS_ALPHA(c)) {
			continue;
		}
		errno = ERR_ISO8583_INVALID_FIELD_A;
		return -1;
	}

	element->value.string = (unsigned char *)malloc((length + 1) * sizeof(unsigned char));
	if(element->value.string == NULL) {
		errno = ERR_ISO8583_NO_MEMORY;
		return -1;
	}
	element->type = string;
	memcpy(element->value.string, input, length);
	element->value.string[length] = '\0';

printf("iso8583_data_get_a()        String [%s]\n", element->value.string);

	return length;
}

int iso8583_data_get_n(const unsigned char *input, int size, int length, iso8583_field_t *element, int force_string)
{
	unsigned char c;

printf("iso8583_data_get_n()        Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	for(int i = 1; i < length; i++) {
		c = input[i];
		if(IS_NUMERIC(c)) {
			continue;
		}
		errno = ERR_ISO8583_INVALID_FIELD_N;
		return -1;
	}

	if(length <= 9 && !force_string) {
		element->type = number;
		element->value.number = atol_sec(input, size, length);
printf("iso8583_data_get_n()        Number [%ld]\n", element->value.number);

	} else {
		element->value.string = (unsigned char *)malloc((length + 1) * sizeof(unsigned char));
		if(element->value.string == NULL) {
			errno = ERR_ISO8583_NO_MEMORY;
			return -1;
		}
		element->type = string;
		memcpy(element->value.string, input, length);
		element->value.string[length] = '\0';

printf("iso8583_data_get_n()        String [%s]\n", element->value.string);
	}

	return length;
}

int iso8583_data_get_s(const unsigned char *input, int size, int length, iso8583_field_t *element)
{
	unsigned char c;

printf("iso8583_data_get_s()        Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	for(int i = 0; i < length; i++) {
		c = input[i];
		if(IS_SPECIAL(c)) {
			continue;
		}
		errno = ERR_ISO8583_INVALID_FIELD_S;
		return -1;
	}

	element->value.string = (unsigned char *)malloc((length + 1) * sizeof(unsigned char));
	if(element->value.string == NULL) {
		errno = ERR_ISO8583_NO_MEMORY;
		return -1;
	}
	element->type = string;
	memcpy(element->value.string, input, length);
	element->value.string[length] = '\0';

printf("iso8583_data_get_s()        String [%s]\n", element->value.string);

	return length;
}

int iso8583_data_get_an(const unsigned char *input, int size, int length, iso8583_field_t *element)
{
	unsigned char c;

printf("iso8583_data_get_an()       Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	for(int i = 0; i < length; i++) {
		c = input[i];
		if(IS_ALPHA  (c)
		|| IS_NUMERIC(c)
		) {
			continue;
		}
		errno = ERR_ISO8583_INVALID_FIELD_AN;
		return -1;
	}

	element->value.string = (unsigned char *)malloc((length + 1) * sizeof(unsigned char));
	if(element->value.string == NULL) {
		errno = ERR_ISO8583_NO_MEMORY;
		return -1;
	}
	element->type = string;
	memcpy(element->value.string, input, length);
	element->value.string[length] = '\0';

printf("iso8583_data_get_an()       String [%s]\n", element->value.string);

	return length;
}

int iso8583_data_get_as (const unsigned char *input, int size, int length, iso8583_field_t *element)
{
	unsigned char c;

printf("iso8583_data_get_as()       Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	for(int i = 0; i < length; i++) {
		c = input[i];
		if(IS_ALPHA  (c)
		|| IS_SPECIAL(c)
		) {
			continue;
		}
		errno = ERR_ISO8583_INVALID_FIELD_AS;
		return -1;
	}

	element->value.string = (unsigned char *)malloc((length + 1) * sizeof(unsigned char));
	if(element->value.string == NULL) {
		errno = ERR_ISO8583_NO_MEMORY;
		return -1;
	}
	element->type = string;
	memcpy(element->value.string, input, length);
	element->value.string[length] = '\0';

printf("iso8583_data_get_as()       String [%s]\n", element->value.string);

	return length;
}

int iso8583_data_get_ns (const unsigned char *input, int size, int length, iso8583_field_t *element)
{
	unsigned char c;

printf("iso8583_data_get_ns()       Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	for(int i = 0; i < length; i++) {
		c = input[i];
		if(IS_NUMERIC(c)
		|| IS_SPECIAL(c)
		) {
			continue;
		}
		errno = ERR_ISO8583_INVALID_FIELD_NS;
		return -1;
	}

	element->value.string = (unsigned char *)malloc((length + 1) * sizeof(unsigned char));
	if(element->value.string == NULL) {
		errno = ERR_ISO8583_NO_MEMORY;
		return -1;
	}
	element->type = string;
	memcpy(element->value.string, input, length);
	element->value.string[length] = '\0';

printf("iso8583_data_get_ns()       String [%s]\n", element->value.string);

	return length;
}

int iso8583_data_get_ans(const unsigned char *input, int size, int length, iso8583_field_t *element)
{
	unsigned char c;

printf("iso8583_data_get_ans()      Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	for(int i = 0; i < length; i++) {
		c = input[i];
		if(IS_ALPHA  (c)
		|| IS_NUMERIC(c)
		|| IS_SPECIAL(c)
		) {
			continue;
		}
		errno = ERR_ISO8583_INVALID_FIELD_ANS;
		return -1;
	}

	element->value.string = (unsigned char *)malloc((length + 1) * sizeof(unsigned char));
	if(element->value.string == NULL) {
		errno = ERR_ISO8583_NO_MEMORY;
		return -1;
	}
	element->type = string;
	memcpy(element->value.string, input, length);
	element->value.string[length] = '\0';

printf("iso8583_data_get_ans()      String [%s]\n", element->value.string);

	return length;
}

int iso8583_data_get_xn(const unsigned char *input, int size, int length, iso8583_field_t *element)
{
	unsigned char c;

printf("iso8583_data_get_xn()       Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	if(input[0] != 'D'
	&& input[0] != 'C'
	&& input[0] != 'd'
	&& input[0] != 'c'
	) {
		errno = ERR_ISO8583_INVALID_FIELD_XN;
		return -1;
	}

	for(int i = 1; i < length; i++) {
		c = input[i];
		if(IS_NUMERIC(c)) {
			continue;
		}
		errno = ERR_ISO8583_INVALID_FIELD_XN;
		return -1;
	}
	if(length <= 10) {
		element->type = number;
		element->value.number = atol_sec(&input[1], size - 1, length - 1);
		if(input[0] == 'D'
		|| input[0] == 'd'
		) {
			element->value.number*= -1l;
		}
printf("iso8583_data_get_xn()       Number [%ld]\n", element->value.number);

	} else {
		element->value.string = (unsigned char *)malloc((length + 1) * sizeof(unsigned char));
		if(element->value.string == NULL) {
			errno = ERR_ISO8583_NO_MEMORY;
			return -1;
		}
		element->type = string;
		if(input[0] == 'D'
		|| input[0] == 'd'
		) {
			memcpy(&element->value.string[1], &input[1], length - 1);
			element->value.string[0         ] = '-';
			element->value.string[length    ] = '\0';
		} else {
			memcpy(element->value.string, &input[1], length - 1);
			element->value.string[length - 1] = '\0';
		}
printf("iso8583_data_get_xn()       String [%s]\n", element->value.string);
	}

	return length;
}

int iso8583_data_get_b(const unsigned char *input, int size, int length, iso8583_field_t *element)
{
printf("iso8583_data_get_b()        Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	element->value.binary.data = (unsigned char *)malloc(length * sizeof(unsigned char));
	if(element->value.binary.data == NULL) {
		errno = ERR_ISO8583_NO_MEMORY;
		return -1;
	}
	element->type = binary;
	element->value.binary.size = length;
	memcpy(element->value.binary.data, input, length);

	return length;
}

int iso8583_data_get_z(const unsigned char *input, int size, int length, iso8583_field_t *element)
{
printf("iso8583_data_get_z()        Data {%d}[", length); for(int i = 0; i < length; i++) printf("%s%02x", i>0?" ":"", input[i]); printf("]\n");

	// TODO
printf("iso8583_data_get_z()        TODO\n");

	return length;
}

