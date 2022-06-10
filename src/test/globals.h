#ifndef GLOBALS_H
#define GLOBALS_H

// Global Constants

// Length 15
static const char *iso8583_version_string_negative[] = {
	"-",
	"reserved_by_iso",
	"national_use",
	"private_use"
};

static const char *iso8583_version_string[] = {
	"unknown",
	"version_1987",
	"version_1993",
	"version_2003"
};

// Length 18
static const char *iso8583_message_string_negative[] = {
	"-",
	"reserved_by_iso",
};

static const char *iso8583_message_string[] = {
	"unknown",
	"authorization",
	"financial",
	"file_actions",
	"reverses_previous",
	"charges_back",
	"reconciliation",
	"administrative",
	"fee_collection",
	"network_management"
};

// Length 28
static const char *iso8583_function_string_negative[] = {
	"-",
	"reserved_by_iso",
};

static const char *iso8583_function_string[] = {
	"unknown",
	"request",
	"request_response",
	"advice",
	"advice_response",
	"notification",
	"notification_acknowledgement",
	"instruction",
	"instruction_acknowledgement"
};

// Length 15
static const char *iso8583_origin_string_negative[] = {
	"-",
	"reserved_by_iso",
};

static const char *iso8583_origin_string[] = {
	"unknown",
	"acquirer",
	"acquirer_repeat",
	"issuer",
	"issuer_repeat",
	"other",
	"other_repeat"
};

// Length 21
static const char *iso8583_field_type_string[] = {
	"unknown",
	"alpha",
	"numeric",
	"special",
	"alpha_numeric",
	"alpha_special",
	"numeric_special",
	"alpha_numeric_special",
	"x_numeric",
	"binary_field",
	"code_set"
};

// Length 7
static const char *iso8583_value_type_string[] = {
	"unknown",
	"number",
	"string",
	"binary"
};

// Length 72
static const char *iso8583_field_description_string[] = {
	"-",
	"Bitmap",
	"Primary account number (PAN)",
	"Processing Code",
	"Amount Transaction",
	"Amount, settlement",
	"Amount, cardholder billing",
	"Transmission date & time",
	"Amount, cardholder billing fee",
	"Conversion rate, settlement",
	"Conversion rate, cardholder billing",
	"System trace audit number (STAN)",
	"Local transaction time (hhmmss)",
	"Local transaction date (MMDD)",
	"Expiration date",
	"Settlement date",
	"Currency conversion date",
	"Capture date",
	"Merchant type, or merchant category code",
	"Acquiring institution (country code)",
	"PAN extended (country code)",
	"Forwarding institution (country code)",
	"Point of service entry mode",
	"Application PAN sequence number",
	"Function code (ISO 8583:1993), or network international identifier (NII)",
	"Point of service condition code",
	"Point of service capture code",
	"Authorizing identification response length",
	"Amount, transaction fee",
	"Amount, settlement fee",
	"Amount, transaction processing fee",
	"Amount, settlement processing fee",
	"Acquiring institution identification code",
	"Forwarding institution identification code",
	"Primary account number, extended",
	"Track 2 data",
	"Track 3 data",
	"Retrieval reference number",
	"Authorization identification response",
	"Response code",
	"Service restriction code",
	"Card acceptor terminal identification",
	"Card acceptor identification code",
	"Card acceptor name/location", // (1–23 street address, –36 city, –38 state, 39–40 country)",
	"Additional response data",
	"Track 1 data",
	"Additional data (ISO)",
	"Additional data (national)",
	"Additional data (private)",
	"Currency code, transaction (It's alpha OR numeric!)",
	"Currency code, settlement (It's alpha OR numeric!)",
	"Currency code, cardholder billing  (It's alpha OR numeric!)",
	"Personal identification number data",
	"Security related control information",
	"Additional amounts",
	"ICC data – EMV having multiple tags",
	"Reserved (ISO)",
	"Reserved (national)",
	"Reserved (national)",
	"Reserved (national)",
	"Reserved (national)", // (e.g. settlement request: batch number, advice transactions: original transaction amount, batch upload: original MTI plus original RRN plus original STAN, etc.)",
	"Reserved (private)",  // (e.g. CVV2/service code transactions)",
	"Reserved (private)",  // (e.g. transactions: invoice number, key exchange transactions: TPK key, etc.)",
	"Reserved (private)",
	"Message authentication code (MAC)",
	"Extended bitmap indicator",
	"Settlement code",
	"Extended payment code",
	"Receiving institution country code",
	"Settlement institution country code",
	"Network management information code",
	"Message number",
	"Last message's number",
	"Action date (YYMMDD)",
	"Number of credits",
	"Credits, reversal number",
	"Number of debits",
	"Debits, reversal number",
	"Transfer number",
	"Transfer, reversal number",
	"Number of inquiries",
	"Number of authorizations",
	"Credits, processing fee amount",
	"Credits, transaction fee amount",
	"Debits, processing fee amount",
	"Debits, transaction fee amount",
	"Total amount of credits",
	"Credits, reversal amount",
	"Total amount of debits",
	"Debits, reversal amount",
	"Original data elements",
	"File update code",
	"File security code",
	"Response indicator",
	"Service indicator",
	"Replacement amounts",
	"Message security code",
	"Net settlement amount",
	"Payee",
	"Settlement institution identification code",
	"Receiving institution identification code",
	"File name",
	"Account identification 1",
	"Account identification 2",
	"Transaction description",
	"Reserved for ISO use",
	"Reserved for ISO use",
	"Reserved for ISO use",
	"Reserved for ISO use",
	"Reserved for ISO use",
	"Reserved for ISO use",
	"Reserved for ISO use",
	"Reserved for national use",
	"Reserved for national use",
	"Reserved for national use",
	"Reserved for national use",
	"Reserved for national use",
	"Reserved for national use",
	"Reserved for national use",
	"Reserved for national use",
	"Reserved for private use",
	"Reserved for private use",
	"Reserved for private use",
	"Reserved for private use",
	"Reserved for private use",
	"Reserved for private use",
	"Reserved for private use",
	"Reserved for private use",
	"Message authentication code",
	"-"
};

#endif // GLOBALS_H
