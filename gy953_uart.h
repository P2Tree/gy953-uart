#ifndef GY953_UART_H_
#define GY953_UART_H_

/**
 * @func: setOpt    use to setting uart options
 * @param: fd       file descriptor
 * @param: nSpeed   uart transfer speed
 * @param: nBits    uart transfer bits
 * @param: nEvent   uart parity bit
 * @param: nStop    uart stop bit
 * @param:          0 is down
 **/
int setOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop);

/**
 * @func: openPort  use to open uart
 * @retval:         0 is down
 **/
int openPort();

#endif /* ifndef GY953_UART_H_ */

