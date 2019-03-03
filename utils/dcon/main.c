#include "uart.h"
#include "log.h"

#include <string.h>
#include <malloc.h>
#include <stdlib.h>

char tx[1024];
char rx[1024];
 int br;
 int bits;
char parity = 0;
 int stop = 0;
 int cnt, k, fd, r, n, j;

int main(int argc, char *argv[]) 
{
	if (argc != 6) {
		printf("Usage: %s ttyS* baudrate mode data count.\n", argv[0]);
		printf("All parameter are mendatory\n");
		printf("tty     : /dev/ttyS*\n");
		printf("baudrate: 9600 for example\n");
		printf("mode    : 8N1 for example\n");
		printf("data    : for example '$01M'\n");
		printf("count	: iterations\n");
		exit(1);
	}

	r = sscanf(argv[2], "%d", &br);
	if (r != 1) {
		log_err_msg("illegal  baudrate %s, use 9600 etc \n", argv[2]);
		exit(1);
	}

	r = sscanf(argv[3], "%d%c%d", &bits, &parity, &stop);
	if (r != 3) {
		log_err_msg("illegal mode '%s', use 8N1 for ex.\n", argv[3]);
		exit(1);
	}

	r = sscanf(argv[5], "%d", &cnt);
	if (r != 1) {
		log_err_msg("illegal count '%s', use size_t number\n", argv[5]);
		exit(1);
	}

	if (cnt < 0) {
		log_err_msg("use size_t number in count\n");
		exit(1);
	}

	n = strlen(argv[4]);

	for (j = 0; j < n; j++)
		tx[j] = argv[4][j];
	tx[n] = '\r';
	tx[n + 1] = '\0';

	fd = uart_open(argv[1]);
	log_dbg_msg("openning '%s\n", argv[1]);
	uart_set(fd, br, bits, parity, stop);
	log_dbg_msg("Ok, flusshing...\n");
	log_dbg_msg("Start\n");

	for (;;) {
		usleep(1000000);
		printf("TX: ");
		for (j = 0; j < n; j++)
			printf("%c", tx[j]);

		r = write(fd, tx, n + 1);
		if (-1 == r)
			break;
		printf(" (%d bytes)\n", r);

		usleep(1000000);

		r = read(fd, rx, 1024);
		if (-1 == r) {
			printf("r = -1\n");
			break;
		}

		printf("RX: ");
		for (j = 0; j < r - 1; j++)
			printf("%c", rx[j]);
		printf("\n");

		k++;
		if (k == cnt)
			break;
	}

	return 0;
}
