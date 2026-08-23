#ifndef _RFID_H_
#define _RFID_H_

#include <stdint.h>
#include <stdbool.h>

#define RFID_UID_LEN				4

bool Rfid_TryReadUid(uint8_t uid_out[RFID_UID_LEN]);

#endif // _RFID_H_