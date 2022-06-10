// To work with ISO 8583:2003

#ifndef ISO8583_H
#define ISO8583_H

// Definitions

#define reserved_by_iso  -1
#define unknown           0

#define ERR_ISO8583_INVALID_ARG                    EINVAL
#define ERR_ISO8583_NO_MEMORY                      ENOMEM
#define ERR_ISO8583_NULL_ARG                       231
#define ERR_ISO8583_QUATERNARY_BITMAP              232
#define ERR_ISO8583_INSUFFICIENT_INPUT             233
#define ERR_ISO8583_EXCESS_INPUT                   234
#define ERR_ISO8583_SUPERIOR_INPUT_FIELD_LENGTH    235
#define ERR_ISO8583_INVALID_FIELD_LENGTH           236
#define ERR_ISO8583_INVALID_FIELD_A                237
#define ERR_ISO8583_INVALID_FIELD_N                238
#define ERR_ISO8583_INVALID_FIELD_S                239
#define ERR_ISO8583_INVALID_FIELD_AN               240
#define ERR_ISO8583_INVALID_FIELD_AS               241
#define ERR_ISO8583_INVALID_FIELD_NS               242
#define ERR_ISO8583_INVALID_FIELD_ANS              243
#define ERR_ISO8583_INVALID_FIELD_XN               244
#define ERR_ISO8583_INVALID_FIELD_B                245
#define ERR_ISO8583_INVALID_FIELD_Z                246

#define ERR_INTERNAL_ISO8583_INVALID_FIELD_DIGITS  251
#define ERR_INTERNAL_ISO8583_INVALID_FIELD_TYPE    252

// Types definition

typedef enum {
	private_use             = -3,
	national_use            = -2,
	version_reserved_by_iso = -1,
	version_unknown,
	version_1987,
	version_1993,
	version_2003
} iso8583_version_t;

typedef enum {
	message_reserved_by_iso = -1,
	message_unknown,
	authorization,
	financial,
	file_actions,
	reverses_previous,
	charges_back,
	reconciliation,
	administrative,
	fee_collection,
	network_management
} iso8583_message_t;

typedef enum {
	function_reserved_by_iso = -1,
	function_unknown,
	request,
	request_response,
	advice,
	advice_response,
	notification,
	notification_acknowledgement,
	instruction,
	instruction_acknowledgement
} iso8583_function_t;

typedef enum {
	origin_reserved_by_iso = -1,
	origin_unknown,
	acquirer,
	acquirer_repeat,
	issuer,
	issuer_repeat,
	other,
	other_repeat
} iso8583_origin_t;

typedef enum {
	field_type_unknown,
	alpha,
	numeric,
	special,
	alpha_numeric,
	alpha_special,
	numeric_special,
	alpha_numeric_special,
	x_numeric,
	binary_field,
	code_set
} iso8583_field_type_t;

typedef enum {
	field_type_value_unknown,
	number,
	string,
	binary
} iso8583_value_type_t;

typedef struct {
	iso8583_version_t  version;
	iso8583_message_t  message;
	iso8583_function_t function;
	iso8583_origin_t   origin;
} iso8583_mti_t;

typedef struct {
	int field[192 + 1]; // Field from 1 to 192
	int count;
	int field_count;
} iso8583_bitmap_t;

typedef struct {
	int                    field;
	iso8583_value_type_t   type;
	union {
		    long int       number;
		unsigned char     *string;
		struct {
			unsigned char *data;
			         int   size;
		}                  binary;
	}                      value;
} iso8583_field_t;

typedef struct {
	iso8583_field_t *fields;
	int              count;
} iso8583_fields_t;

typedef struct {
	iso8583_mti_t     mti;
	iso8583_bitmap_t  bitmaps;
	iso8583_fields_t  elements;
} iso8583_t;

// Prototypes

iso8583_t *iso8583    (iso8583_t *transaction, unsigned char *input, int size);
iso8583_t *iso8583_hex(iso8583_t *transaction, unsigned char *input, int size);

int                iso8583_mti              (const unsigned char *input, iso8583_mti_t *mti);
iso8583_version_t  iso8583_mti_version      (      unsigned char code);
iso8583_message_t  iso8583_mti_message_class(      unsigned char code, unsigned char code_origin);
iso8583_function_t iso8583_mti_function     (      unsigned char code);
iso8583_origin_t   iso8583_mti_origin       (      unsigned char code);

int iso8583_bitmaps_hex    (const unsigned char  *input,            iso8583_bitmap_t *bitmaps);
int iso8583_bitmaps_bin    (const unsigned char  *input, int  size, iso8583_bitmap_t *bitmaps);

int iso8583_data_setup     (const unsigned char  *input,            iso8583_bitmap_t *bitmaps,      iso8583_fields_t *elements);
int iso8583_data_hex       (const unsigned char  *input, int  size, iso8583_bitmap_t *bitmaps,      iso8583_fields_t *elements);
int iso8583_data_bin       (const unsigned char  *input, int  size, iso8583_bitmap_t *bitmaps,      iso8583_fields_t *elements);

int iso8583_data_add       (const unsigned char  *input, int  size,                                 iso8583_field_t  *element );

int iso8583_data_get_length(const unsigned char **input, int *size, int *length_length, int digits, iso8583_field_type_t type );

int iso8583_data_get_a     (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );
int iso8583_data_get_n     (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element , int force_string);
int iso8583_data_get_s     (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );
int iso8583_data_get_an    (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );
int iso8583_data_get_as    (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );
int iso8583_data_get_ns    (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );
int iso8583_data_get_ans   (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );
int iso8583_data_get_xn    (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );
int iso8583_data_get_b     (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );
int iso8583_data_get_z     (const unsigned char  *input, int  size, int  length,                    iso8583_field_t  *element );

void iso8583_clear         (iso8583_t        *transaction);
void iso8583_clear_mti     (iso8583_mti_t    *mti        );
void iso8583_clear_bitmap  (iso8583_bitmap_t *bitmaps    );
void iso8583_clear_elements(iso8583_fields_t *elements   );

void iso8583_free          (iso8583_t        *transaction);
void iso8583_free_elements (iso8583_fields_t *elements   );

#endif //  ISO8583_H
