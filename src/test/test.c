#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "iso8583.h"
#include "test.h"
#include "strings.h"
#include "functions.h"
#include "print.h"

// Definitions

// Global Variables

int _result;

// Functions

void usage(const char *argv_0)
{
	printf("Usage: %s [ <data> | --file <filename> | - ]\n", basename(argv_0));
	printf("    <data>    Four first bytes are the code,\n");
	printf("              the next bytes are in hexa format.\n");
	printf("    --file    Four first bytes are the code,.\n");
	printf("              the next bytes are in binary format.\n");
	printf("    -         The stdin works like --file.\n");

	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	iso8583_t      transaction;
	unsigned char *input = NULL;
	         int   size;

	printf("Liso8583 - ISO8583 test\n");

	memset(&transaction, '\0', sizeof(iso8583_t));

	if(argc == 3 && memcmp(argv[1], "--file", 7) == 0) {
		IF_OK((input = read_file(argv[2], &size)) != NULL) {
			test_input(&transaction, input,size , 1);
			free(input);
		}

	} else if(argc == 2 && memcmp(argv[1], "-", 2) == 0) {
		IF_OK((input = read_input(&size)) != NULL) {
			test_input(&transaction, input, size, 1);
			free(input);
		}

	} else if(argc == 2 && memcmp(argv[1], "-h", 3) != 0 && memcmp(argv[1], "--help", 7) != 0) {
		input = T(argv[1]);
		size  = strlen_sec(input, 32767);
		test_input(&transaction, input, size, 0);

	} else if(argc == 1) {
		tests(&transaction);

	} else {
		usage(argv[0]);
	}

	iso8583_free_elements(&transaction.elements);

	printf("\n" COLOR_GREEN "Done!" COLOR_RESET "\n");

	return EXIT_SUCCESS;
}

unsigned char *read_input(int *input_size)
{
	*input_size = 0;

	// TODO: read from stdin.

	return NULL;
}

unsigned char *read_file(char *filename, int *input_size)
{
	         FILE   *fp        = fopen(filename, "rb");
	unsigned char   *input     = NULL;
	         int     r, readed = 0;
	         size_t  size;

	*input_size = 0;

	if(fp != NULL) {
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		if(size < 34767) {
			input = (unsigned char *)malloc(size * sizeof(unsigned char));
			if(input != NULL) {
				do {
					r = fread(&input[readed], sizeof(char), size - readed, fp);
					if(r > 0) {
						readed+= r;
					}
				} while(readed < (int)size);
				*input_size = readed;
			}
		}

		fclose(fp);
	}

	return input;
}

void test_input(iso8583_t *transaction, unsigned char *input, int size, int is_bin)
{
	// MTI
	IF_OK(iso8583_mti(input, &transaction->mti) == 0) {
		print_mti(&transaction->mti);
	}
	input = &input[4]; // Shift MTI on input
	size -=        4 ;

	// Bitmaps
	if(is_bin) {
		IF_OK(iso8583_bitmaps_bin(input, size, &transaction->bitmaps) == 0) {
			print_bitmap(&transaction->bitmaps);
		}
		input = &input[ 8 * transaction->bitmaps.count]; // Shift bitmaps on input
		size -=         8 * transaction->bitmaps.count ;

	} else if(strlen_sec(input, 16) == 16) {
		IF_OK(iso8583_bitmaps_hex(input,         &transaction->bitmaps) == 0) {
			print_bitmap(&transaction->bitmaps);
		}
		input = &input[16 * transaction->bitmaps.count]; // Shift bitmaps on input
		size -=        16 * transaction->bitmaps.count ;

	} else {
		printf(COLOR_RED "Invalid bitmap." COLOR_RESET);

		exit(EXIT_FAILURE);
	}

	// Data Elements
	if(is_bin) {
		IF_OK(iso8583_data_bin(input, size, &transaction->bitmaps, &transaction->elements) == 0) {
			print_elements(&transaction->elements, 1);
		}
	} else {
		IF_OK(iso8583_data_hex(input, size, &transaction->bitmaps, &transaction->elements) == 0) {
			print_elements(&transaction->elements, 1);
		}
	}
}

void tests(iso8583_t *transaction)
{
	TEST_MTI(&transaction->mti, "0100", version_1987, authorization     , request         , acquirer         , "Authorization Request\nRequest from a point-of-sale terminal for authorization for a cardholder purchase")
	TEST_MTI(&transaction->mti, "0110", version_1987, authorization     , request_response, acquirer         , "Authorization Response\nRequest response to a point-of-sale terminal for authorization for a cardholder purchase")
	TEST_MTI(&transaction->mti, "0120", version_1987, authorization     , advice          , acquirer         , "Authorization Advice\nWhen the point-of-sale device breaks down and you have to sign a voucher")
	TEST_MTI(&transaction->mti, "0121", version_1987, authorization     , advice          , acquirer_repeat  , "Authorization Advice Repeat\nIf the advice times out")
	TEST_MTI(&transaction->mti, "0130", version_1987, authorization     , advice_response , acquirer         , "Issuer Response to Authorization Advice\nConfirmation of receipt of authorization advice")
	TEST_MTI(&transaction->mti, "0200", version_1987, financial         , request         , acquirer         , "Acquirer Financial Request\nRequest for funds, typically from an ATM or pinned point-of-sale device")
	TEST_MTI(&transaction->mti, "0210", version_1987, financial         , request_response, acquirer         , "Issuer Response to Financial Request\nIssuer response to request for funds")
	TEST_MTI(&transaction->mti, "0220", version_1987, financial         , advice          , acquirer         , "Acquirer Financial Advice\ne.g. Checkout at a hotel. Used to complete transaction initiated with authorization request")
	TEST_MTI(&transaction->mti, "0221", version_1987, financial         , advice          , acquirer_repeat  , "Acquirer Financial Advice Repeat\nIf the advice times out")
	TEST_MTI(&transaction->mti, "0230", version_1987, financial         , advice_response , acquirer         , "Issuer Response to Financial Advice\nConfirmation of receipt of financial advice")
	TEST_MTI(&transaction->mti, "0320", version_1987, file_actions      , advice          , acquirer         , "Batch Upload\nFile update/transfer advice")
	TEST_MTI(&transaction->mti, "0330", version_1987, file_actions      , advice_response , acquirer         , "Batch Upload Response\nFile update/transfer advice response")
	TEST_MTI(&transaction->mti, "0400", version_1987, reverses_previous , request         , acquirer         , "Acquirer Reversal Request\nReverses a transaction")
	TEST_MTI(&transaction->mti, "0420", version_1987, reverses_previous , advice          , acquirer         , "Acquirer Reversal Advice")
	TEST_MTI(&transaction->mti, "0430", version_1987, reverses_previous , advice_response , acquirer         , "Acquirer Reversal Advice Response")
	TEST_MTI(&transaction->mti, "0510", version_1987, reconciliation    , request_response, acquirer         , "Batch Settlement Response\nCard acceptor reconciliation request response")
	TEST_MTI(&transaction->mti, "0800", version_1987, network_management, request         , acquirer         , "Network Management Request\nHypercom terminals initialize request. Echo test, logon, logoff etc.")
	TEST_MTI(&transaction->mti, "0810", version_1987, network_management, request_response, acquirer         , "Network Management Response\nHypercom terminals initialize response. Echo test, logon, logoff etc.")
	TEST_MTI(&transaction->mti, "0820", version_1987, network_management, advice          , acquirer         , "Network Management Advice\nKey change")

	TEST_BITMAP(&transaction->bitmaps, 3, "...", 2, VALUES(1, 2), "Desc...")
	TEST_BITMAP(&transaction->bitmaps, 3, "...", 2, VALUES(1, 2), "Desc...")

	// Reuse last Bitmap.
	TEST_DATA(&transaction->bitmaps, &transaction->elements, 3, "...", "Desc...")
	TEST_DATA(&transaction->bitmaps, &transaction->elements, 3, "...", "Desc...")
	TEST_DATA(&transaction->bitmaps, &transaction->elements, 3, "...", "Desc...")

	TEST_INPUT(3, "...")
	TEST_INPUT(3, "...")
	TEST_INPUT(3, "...")
}

void test_mti(iso8583_mti_t *mti, const unsigned char *code, const iso8583_mti_t *test, const char *desc)
{
	iso8583_clear_mti(mti);
	IF_OK_DESC(iso8583_mti(T(code), mti) == 0, desc) {
		printf("%s\n", desc);
		print_mti_test(mti, test->version, test->message, test->function, test->origin);
		if(mti->version  != test->version
		|| mti->message  != test->message
		|| mti->function != test->function
		|| mti->origin   != test->origin
		) {
			exit(EXIT_FAILURE);
		}
	}
}

void test_bitmap(iso8583_bitmap_t *bitmaps, int size, const unsigned char *input, const iso8583_bitmap_t *test, const char *desc)
{
	iso8583_clear_bitmap(bitmaps);
	IF_OK_DESC(iso8583_bitmaps_bin(input, size, bitmaps) == 0, desc) {
		printf("%s\n", desc);
		print_bitmap(bitmaps);
	}
}

void test_data(iso8583_bitmap_t *bitmaps, iso8583_fields_t *elements, int size, const unsigned char *input, const iso8583_fields_t *test, const char *desc)
{
	iso8583_clear_elements(elements);
	IF_OK_DESC(iso8583_data_bin(input, size, bitmaps, elements) == 0, desc) {
		printf("%s\n", desc);
		print_elements(elements, 1);
	}
}

const char *basename(const char *name)
{
	const char *p = strrchr(name, '/');

	if(p == NULL) return name;

	return &p[1];
}
