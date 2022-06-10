#include <stddef.h>
#include <errno.h>
#include "iso8583.h"
#include "functions.h"

int iso8583_mti(const unsigned char *input, iso8583_mti_t *mti)
{
	if(input == NULL || mti == NULL) {
		errno = ERR_ISO8583_NULL_ARG;
		return -1;
	}
	if(strlen_sec(input, 4) != 4) {
		errno = ERR_ISO8583_INVALID_ARG;
		return -1;
	}

	mti->version  = iso8583_mti_version      (input[0]);
	mti->message  = iso8583_mti_message_class(input[1], input[3]);
	mti->function = iso8583_mti_function     (input[2]);
	mti->origin   = iso8583_mti_origin       (input[3]);

	return 0;
}

iso8583_version_t iso8583_mti_version(unsigned char code)
{
	switch(code) {
		case '0': return version_1987;  // ISO 8583:1987
		case '1': return version_1993;  // ISO 8583:1993
		case '2': return version_2003;  // ISO 8583:2003
		case '3':
		case '4':
		case '5':
		case '6':
		case '7': return reserved_by_iso;
		case '8': return national_use;
		case '9': return private_use;
	}

	return unknown;
}

iso8583_message_t iso8583_mti_message_class(unsigned char code, unsigned char code_origin)
{
	switch(code) {
		case '0': return reserved_by_iso;
		case '1': return authorization; 
		case '2': return financial;
		case '3': return file_actions;
		case '4':                                    // Reversal and chargeback messages
			switch(code_origin) {
				case '0':
				case '1': return reverses_previous;  // Reverses the action of a previous authorization
				case '2':
				case '3': return charges_back;       // Charges back a previously cleared financial message
			}
		break;
		case '5': return reconciliation;
		case '6': return administrative;
		case '7': return fee_collection;
		case '8': return network_management;
		case '9': return reserved_by_iso;
	}

	return unknown;
}

iso8583_function_t iso8583_mti_function(unsigned char code)
{
	switch(code) {
		case '0': return request;
		case '1': return request_response;
		case '2': return advice;
		case '3': return advice_response;
		case '4': return notification;
		case '5': return notification_acknowledgement;
		case '6': return instruction;
		case '7': return instruction_acknowledgement;
		case '8': return reserved_by_iso;
		case '9': return reserved_by_iso;
	}

	return unknown;
}

iso8583_origin_t iso8583_mti_origin(unsigned char code)
{
	switch(code) {
		case '0': return acquirer;
		case '1': return acquirer_repeat;
		case '2': return issuer;
		case '3': return issuer_repeat;
		case '4': return other;
		case '5': return other_repeat;
		case '6': return reserved_by_iso;
		case '7': return reserved_by_iso;
		case '8': return reserved_by_iso;
		case '9': return reserved_by_iso;
	}

	return unknown;
}

void iso8583_clear_mti(iso8583_mti_t *mti)
{
	if(mti != NULL) {
		mti->version  = unknown;
		mti->message  = unknown;
		mti->function = unknown;
		mti->origin   = unknown;
	}
}
