#include "modbus.h"

u16 modbus_crc(u8 *buf, size_t len)
{
	u16 crc = 0xFFFF;
	size_t i;

	while(len--) {
		crc ^= (u16) *buf++;

		for (i = 0; i < 8; i++) {
			if (crc & 1) 
				crc = (crc >> 1) ^ 0xA001;
			else
				crc = (crc >> 1);
		}
	}

	return crc;
}
