#ifndef TEST_H
#define TEST_H

// Includes

#include "iso8583.h"

// Definitions

#define WITH_COLORS

#define T(STR)  ((unsigned char *)(STR))

#define IF_OK(CONDITION)  \
	IF_OK_DESC(CONDITION, "")

#define IF_OK_DESC(CONDITION, DESC)  \
	printf("\n" COLOR_YELLOW "Test: " COLOR_RESET COLOR_WHITE # CONDITION COLOR_RESET COLOR_YELLOW " -> " COLOR_RESET); \
	_result = (CONDITION); \
	printf("%s" COLOR_RESET  "\n", _result ? COLOR_GREEN "Success" : COLOR_RED "Fail" ); \
	fflush(stdout); \
	if(!_result) { \
		printf("%s\n", DESC); \
		printf("%s:%d\n", basename(__FILE__), __LINE__); \
		\
		exit(EXIT_FAILURE); \
	} else

#define TEST_MTI(MTI_VAR, CODE, VERSION, MESSAGE, FUNCTION, ORIGIN, DESC)  { \
	iso8583_mti_t test = { VERSION, MESSAGE, FUNCTION, ORIGIN }; \
	test_mti(MTI_VAR, T(CODE), &test, DESC); \
}

#define TEST_BITMAP(BITMAP_VAR, BITMAP_SIZE, BITMAP, FIELDS_COUNT, FIELDS, DESC)  { \
	int field[FIELDS_COUNT] = FIELDS; \
	iso8583_bitmap_t test; \
	test.count = (FIELDS_COUNT / 64) + 1; \
	for(int i = 1; i <= 192; i++) \
		for(int j = 0; j < FIELDS_COUNT; j++) \
			if(field[j] == i) \
				test.field[i] = 1; \
	test_bitmap(BITMAP_VAR, BITMAP_SIZE, T(BITMAP), &test, DESC "\n" # FIELDS); \
}

#define TEST_DATA(BITMAP_VAR, DATA_VAR, DATA_SIZE, DATA, DESC)  \
	iso8583_clear_elements(DATA_VAR); \
	IF_OK_DESC(iso8583_data_bin(T(DATA), DATA_SIZE, BITMAP_VAR, DATA_VAR) == 0, DESC) { \
		printf("%s\n", DESC); \
		print_elements(DATA_VAR, 1); \
	}

#define TEST_INPUT(SIZE, INPUT)  { \
	iso8583_t t; \
	iso8583_clear(&t); \
	IF_OK(iso8583(&t, T(INPUT), SIZE) == 0) { \
		print_mti     (&transaction->mti     ); \
		print_bitmap  (&transaction->bitmaps ); \
		print_elements(&transaction->elements, 1); \
	} \
}

#define VALUES(...)  { __VA_ARGS__ }

// Colors

#ifdef WITH_COLORS

#	define COLOR_RESET   "\x1B[0m"
#	define COLOR_DARK    "\x1B[90m"
#	define COLOR_RED     "\x1B[91m"
#	define COLOR_GREEN   "\x1B[92m"
#	define COLOR_YELLOW  "\x1B[93m"
#	define COLOR_BLUE    "\x1B[94m"
#	define COLOR_PURPLE  "\x1B[95m"
#	define COLOR_CYAN    "\x1B[96m"
#	define COLOR_WHITE   "\x1B[97m"

#	define COLOR_ALERT   "\x1B[93;41m"

#else

#	define COLOR_RESET   ""
#	define COLOR_DARK    ""
#	define COLOR_RED     ""
#	define COLOR_GREEN   ""
#	define COLOR_YELLOW  ""
#	define COLOR_BLUE    ""
#	define COLOR_PURPLE  ""
#	define COLOR_CYAN    ""
#	define COLOR_WHITE   ""

#	define COLOR_ALERT   ""

#endif

// Prototypes

void usage(const char *argv_0);

unsigned char *read_input(                int *input_size);
unsigned char *read_file (char *filename, int *input_size);

void test_input(iso8583_t *transaction, unsigned char *input, int size, int is_bin);
void tests     (iso8583_t *transaction);

void test_mti   (                           iso8583_mti_t    *mti     ,           const unsigned char *code , const iso8583_mti_t    *test, const char *desc);
void test_bitmap(                           iso8583_bitmap_t *bitmaps , int size, const unsigned char *input, const iso8583_bitmap_t *test, const char *desc);
void test_data  (iso8583_bitmap_t *bitmaps, iso8583_fields_t *elements, int size, const unsigned char *input, const iso8583_fields_t *test, const char *desc);

const char *basename(const char *name);

#endif // TEST_H
