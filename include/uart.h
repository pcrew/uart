#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

 int uart_open(char *dev);
 int uart_set(int fd, int br, int bits, char par, int stopb);
void uart_close(int fd);
