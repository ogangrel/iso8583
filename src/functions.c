#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <errno.h>
#include "functions.h"

int strlen_sec(const unsigned char *str, int max_len)
{
	int i = 0;

	if(str == NULL || max_len < 0) {
		return -1;
	}

	for(; i < max_len && str[0] != '\0'; i++, *str++);

	return i;
}

int atoi_sec(const unsigned char *str, int max_len, int num_len)
{
	int value = 0;

	if(str == NULL || max_len < 0 || num_len < 0) {
		return -1;
	}

	if(max_len < num_len) {
		num_len = max_len;
	}

	for(int i = 0; i < num_len && str[0] != '\0'; i++, *str++) {
		if(str[0] < '0' || str[0] > '9') {
			return -1;
		}
		value = (value * 10) + str[0] - (int)'0';
	}

	return value;
}

long int atol_sec(const unsigned char *str, int  max_len, int num_len)
{
	long int value = 0l;

	if(str == NULL || max_len < 0 || num_len < 0) {
		return -1;
	}

	if(max_len < num_len) {
		num_len = max_len;
	}

	for(int i = 0; i < num_len && str[0] != '\0'; i++, *str++) {
		if(str[0] < '0' || str[0] > '9') {
			return -1;
		}
		value = (value * 10l) + str[0] - (long int)'0';
	}

	return value;
}

int hex2dec_sec(const unsigned char *str, int str_len, int length)
{
	int str_length = strlen_sec(str, length);
	int value      = 0;
	int dec;

	if(str == NULL || str_length == -1 || str_length % 2 != 0) {
		errno = EINVAL;
		return -1;
	}

	for(int i = 0; i < str_length; i+= 2) {
		dec = hexdec(str[i], str[i + 1]);
		if(dec < 0) {
			errno = EINVAL;
			return -1;
		}
		value = value << 1;
		value+= dec;
	}

	return value;
}

unsigned char *hex2bin_sec(const unsigned char *str, int str_len, int *length)
{
	unsigned char  bin[INT_MAX];
	unsigned char *p;
	         int   dec;

	if(str == NULL || str_len % 2 == 1) {
		errno = EINVAL;
		return NULL;
	}

	*length = 0;

	for(int i = 0; i < str_len && str[0] != '\0'; i+= 2, str = &str[2]) {
		dec = hexdec(str[0], str[1]);
		if(dec < 0) {
			errno = EINVAL;
			return NULL;
		}
		bin[(*length)++] = dec;
	}

	p = (unsigned char *)malloc(*length * sizeof(unsigned char));
	if(p == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	memcpy(p, bin, *length);

	return p;
}

int hexdec(unsigned char c1, unsigned char c2)
{
	int value = 0;

	if(c1 >= '0' && c1 <= '9') {
		value+=  (c1 - (int)'0')       * 0x10;

	} else if(c1 >= 'a' && c1 <= 'f') {
		value+= ((c1 - (int)'a') + 10) * 0x10;

	} else if(c1 >= 'A' && c1 <= 'F') {
		value+= ((c1 - (int)'A') + 10) * 0x10;
	} else {
		errno = EINVAL;
		return -1;
	}

	if(c2 >= '0' && c2 <= '9') {
		value+=  (c2 - (int)'0');

	} else if(c2 >= 'a' && c2 <= 'f') {
		value+= ((c2 - (int)'a') + 10);

	} else if(c2 >= 'A' && c2 <= 'F') {
		value+= ((c2 - (int)'A') + 10);
	} else {
		errno = EINVAL;
		return -1;
	}

	return value;
}
