#include "rfid.h"
#include "rc522.h"

bool Rfid_TryReadUid(uint8_t uid_out[RFID_UID_LEN])
{
	uint8_t uid[RFID_UID_LEN];

	if (MFRC522_Request(PICC_REQIDL, uid) != MI_OK) {
		return false;
	}

	if (MFRC522_Anticoll(uid) != MI_OK) {
		return false;
	}

	for (uint8_t i = 0; i < RFID_UID_LEN; i++) {
		uid_out[i] = uid[i];
	}

	return true;
}