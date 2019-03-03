#include "uart.h"

int uart_open(char *dev)
{
	int r;
	int fd;

	fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
	if (-1 == fd) {
		return 0;
	}

	r = tcflush(fd, TCIOFLUSH);
		if (-1 == r)
			return 1;

	return fd;
}

int uart_set(int fd, int br, int bits, char par, int stopb)
{
	struct termios options;
	speed_t baud;
	tcflag_t data;
	int r;

	r = tcgetattr(fd, &options);
	if (-1 == r) {
		return 1;
	}

	switch (br) {
		case 2400:
			baud = B2400;
			break;

		case 4800:
			baud = B4800;
			break;

		case 9600:
			baud = B9600;
			break;

		case 19200:
			baud = B19200;
			break;
			
		case 38400:
			baud = B38400;
			break;
	
		case 57600:
			baud = B57600;
			break;

		case 115200:
			baud = B115200;
			break;
		
		default:
			return 1;
	}

	cfsetispeed(&options, baud);
	cfsetospeed(&options, baud);

	switch (bits) {
		case 5:
			data = CS5;
			break;
		
		case 6:
			data = CS6;
			break;
		
		case 7:
			data = CS7;
			break;
	
		case 8:
			data = CS8;
			break;

		default:
			return 1;
	}

	options.c_cflag &= ~CSIZE;
	options.c_cflag |=  data;

	switch (par) {
		case 'n':
		case 'N':
			options.c_cflag &= ~(PARENB | PARODD);
			break;

		case 'e':
		case 'E':
			options.c_cflag |=  PARENB;
			break;

		case 'o':
		case 'O':
			options.c_cflag |= PARODD;
			break;

		default:
			return 1;
	}

	switch (stopb) {
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;

		case 2:
			options.c_cflag |=  CSTOPB;
			break;

		default:
			return 1;
	}

	options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	options.c_cflag |= (CLOCAL | CREAD);

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | IEXTEN);
	options.c_oflag &= ~(OPOST);

	options.c_lflag = 0;
	options.c_oflag = 0;

	r = tcsetattr(fd, TCSANOW, &options);
	if (-1 == r) {
		return 1;
	}

	return 0;
}

void uart_close(int fd)
{
	close(fd);
}

