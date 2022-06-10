#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include "iso8583.h"
#include "functions.h"

iso8583_t *iso8583(iso8583_t *transaction, unsigned char *input, int size)
{
	int allocated = 0;

	if(input == NULL || size <= 0) {
		errno = ERR_ISO8583_INVALID_ARG;
		return NULL;
	}

	if(transaction == NULL) {
		transaction = (iso8583_t *)calloc(1, sizeof(iso8583_t));
		if(transaction == NULL) {
			errno = ERR_ISO8583_NO_MEMORY;
			return NULL;
		}
		allocated = 1;
	}

	// MTI
	if(iso8583_mti(input, &transaction->mti) != 0) {
		if(allocated) {
			free(transaction);
		}
		// errno come from iso8583_mti()
		return NULL;
	}

	// Shift MTI on input
	input = &input[4];
	size -=        4 ;

	// Bitmaps
	if(iso8583_bitmaps_bin(input, size, &transaction->bitmaps) != 0) {
		if(allocated) {
			free(transaction);
		}
		// errno come from iso8583_bitmaps_bin()
		return NULL;
	}

	// Shift bitmaps on input
	input = &input[8 * transaction->bitmaps.count];
	size -=        8 * transaction->bitmaps.count ;

	// Data
	if(iso8583_data_bin(input, size, &transaction->bitmaps, &transaction->elements) != 0) {
		if(allocated) {
			free(transaction);
		}
		// errno come from iso8583_data_bin()
		return NULL;
	}

	return transaction;
}

iso8583_t *iso8583_hex(iso8583_t *transaction, unsigned char *input, int size)
{
	         iso8583_t *t;
	         int        bin_size;
	unsigned char      *bin_input;

	if(size % 2 == 1) {
		errno = ERR_ISO8583_INVALID_ARG;
		return NULL;
	}

	bin_input = hex2bin_sec(input, size, &bin_size);
	if(bin_input == NULL) {
		// errno come from hex2bin_sec()
		return NULL;
	}

	t = iso8583(transaction, bin_input, bin_size);

	free(bin_input);

	return t;
}

void iso8583_clear(iso8583_t *transaction)
{
	if(transaction != NULL) {
		iso8583_clear_mti     (&transaction->mti     );
		iso8583_clear_bitmap  (&transaction->bitmaps );
		iso8583_clear_elements(&transaction->elements);
	}
}

void iso8583_free(iso8583_t *transaction)
{
	if(transaction != NULL) {
		iso8583_free_elements(&transaction->elements);
		free(transaction);
	}
}
