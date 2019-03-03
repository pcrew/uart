#include "uart.h"
#include "log.h"
#include "numeric.h"
#include "modbus.h"

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>

 u16 crc;
  u8 tx[1024], rx[1024];
 int br = 0;
 int bits = 0;
char parity = 0;
 int stop = 0;
 int cnt,
       k,
      fd,
       r,
       n,
       j;

int main(int argc, char *argv[])
{
	if (argc != 6) {
		printf("Usage: %s tty baudrate mode data count. For ex. ./test /dev/ttyS0 9600 8n1 '010000' 2\n", argv[0]);
		exit(1);
	}

	r = sscanf(argv[2], "%d", &br);
	if (r != 1) {
		log_err_msg("illegal baudrate ('%s')\n", argv[2]);
		exit(1);
	}

	r = sscanf(argv[3], "%d%c%d", &bits, &parity, &stop);
	if (r != 3) {
		log_err_msg("illegal mode ('%s')\n", argv[3]);
		exit(1);
	}

	r = sscanf(argv[5], "%d", &cnt);
	if (r != 1) {
		log_err_msg("illegal count ('%s')\n", argv[5]);
		exit(1);
	}

	if (cnt < 0) {
		log_err_msg("Use size_t type for count\n");
		exit(1);
	}

	n = strlen(argv[4]);
	if (n & 1) {
		log_err_msg("odd number or symbol in '%s'\n", argv[4]);
		exit(1);
	}

	n /= 2;

	for (k = 0; k < n; k++) {
		r = sscanf(argv[4] + 2 * k, "%2hhX", &tx[k]);
		if (r != 1) {
			log_err_msg("illegal data ('%s')\n", argv[4]);
			exit(1);
		}
	}

	crc = modbus_crc((u8 *) tx, n);
	tx[n] = crc & 0x00FF;
	tx[n + 1] = (crc >> 8) & 0x00FF;
	n += 2;

	fd = uart_open(argv[1]);
	uart_set(fd, br, bits, parity, stop);

	k = 0;
	for (;;) {
		usleep(1000000);
		printf("TX: ");
		for (j = 0; j < n; j++)
			printf("%.2X ", tx[j]);
		
		r = write(fd, tx, n);
		if (-1 == r) {
			log_err_msg("write(): %s\n", strerror(errno));
			break;
		}
		printf("(%d bytes)\n", r);

		usleep(1000000);
		r = read(fd, rx, 1024);
		if (-1 == r) {
			log_err_msg("read(): %s\n", strerror(errno));
			break;
		}

		printf("RX: ");
		for (j = 0; j < r; j++)
			printf("%.2X ", rx[j]);
		printf("(%d bytes)\n", r);

		k++;
		if (k == cnt)
			break;
	}
	
	return 0;
}
