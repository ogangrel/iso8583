#include <errno.h>
#include "iso8583.h"
#include "functions.h"

// <DEV>
#include <stdio.h>
// </DEV>

int iso8583_bitmaps_hex(const unsigned char *input, iso8583_bitmap_t *bitmaps)
{
	         int  bytes   = 16;
	unsigned char bin[24] = {
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
	};

	if(input == NULL || bitmaps == NULL) {
		errno = ERR_ISO8583_NULL_ARG;
		return -1;
	}
	if(strlen_sec(input, bytes) != bytes) {
		errno = ERR_ISO8583_INVALID_ARG;
		return -1;
	}

	bitmaps->count = 1;
	for(int i = 0; i < bytes && i < (16 * 3); i+= 2) {
		bin[i / 2] = hex2dec_sec(&input[i], bytes - i, 2);

		if(i % 16 == 0 && bin[i / 2] & 0x80) {
			bytes+= 16;
			if(strlen_sec(input, bytes) != bytes) {
				return - 1;
			}
			bitmaps->count++;
		}
	}

	return iso8583_bitmaps_bin(bin, bytes / 2, bitmaps);
}

int iso8583_bitmaps_bin(const unsigned char *input, int size, iso8583_bitmap_t *bitmaps)
{
	int bytes = 8;

	if(input == NULL || bitmaps == NULL) {
		errno = ERR_ISO8583_NULL_ARG;
		return -1;
	}
	if(size < 8) {
		errno = ERR_ISO8583_INVALID_ARG;
		return -1;
	}

	bitmaps->count = 1;
	bitmaps->field_count = 0;
	for(int i = 0; i < bytes && i < 24; i++) {
		if(input[i] & 0x80) { bitmaps->field[(i * 8) + 1] = 1; bitmaps->field_count++; }
		if(input[i] & 0x40) { bitmaps->field[(i * 8) + 2] = 1; bitmaps->field_count++; }
		if(input[i] & 0x20) { bitmaps->field[(i * 8) + 3] = 1; bitmaps->field_count++; }
		if(input[i] & 0x10) { bitmaps->field[(i * 8) + 4] = 1; bitmaps->field_count++; }
		if(input[i] & 0x08) { bitmaps->field[(i * 8) + 5] = 1; bitmaps->field_count++; }
		if(input[i] & 0x04) { bitmaps->field[(i * 8) + 6] = 1; bitmaps->field_count++; }
		if(input[i] & 0x02) { bitmaps->field[(i * 8) + 7] = 1; bitmaps->field_count++; }
		if(input[i] & 0x01) { bitmaps->field[(i * 8) + 8] = 1; bitmaps->field_count++; }

		if(i % 8 == 0 && input[i] & 0x80) {
			if(i == 16) {
printf("Unexpected quaternary bitmap page\n"); // That should be handled as an error or ignore it?
				errno = ERR_ISO8583_QUATERNARY_BITMAP;
				return -1;
			}
			bytes+= 8;
			if(size < bytes) {
				errno = ERR_ISO8583_INVALID_ARG;
				return -1;
			}
			bitmaps->count++;
		}
	}

	return 0;
}

void iso8583_clear_bitmap(iso8583_bitmap_t *bitmaps)
{
	if(bitmaps != NULL) {
		for(int i = 0; i < 192 + 1; i++) {
			bitmaps->field[i] = 0;
		}
		bitmaps->count = 0;
	}
}

