 /**
  * @file   uart-test.c
  * @brief  This file is only used to check your UART in your ARM board
  *         is well enough to work.
  * @brief  I will use too much annotations to show every action in UART.
  * @author Yang Liuming <dicksonliuming@gmail.com>
  *
  *************************************************************/
#include <stdio.h>          // standard input/output header
#include <termios.h>        // POSIX terminal input/output header
#include <memory.h>         // memory control
#include <fcntl.h>          // file control
#include <unistd.h>         // Unix standard

int setOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop) {
    struct termios newtio, oldtio;
    if (tcgetattr(fd, &oldtio) != 0) {
        perror("Backup SerialSetting");
        return -1;
    }

    printf("old cflag = %x\n", oldtio.c_cflag);
    printf("old iflag = %x\n", oldtio.c_iflag);
    printf("old lflag = %x\n", oldtio.c_lflag);
    printf("old oflag = %x\n", oldtio.c_oflag);

    newtio.c_cflag = 0;
    newtio.c_iflag = 0;
    newtio.c_lflag = 0;
    newtio.c_oflag = 0;

    newtio.c_cflag |= CLOCAL | CREAD ;

    // set data bits
    switch(nBits) {
    case 7:
        newtio.c_cflag &= ~CSIZE;           // zero data bits control
        newtio.c_cflag |= CS7;              // set data bits to 7 bits
        break;
    case 8:
        newtio.c_cflag &= ~CSIZE;           // zero data bits control;
        newtio.c_cflag |= CS8;              // set data bits to 8 bits
        break;
    default:
        perror("Set_opt wrong bits");
        return -1;
    }

    // set parity bit
    switch(nEvent) {
    case 'O':
        newtio.c_cflag &= ~PARENB;          // clear even mode
        newtio.c_cflag |= PARODD;           // set parity bit to odd mode
        newtio.c_iflag |= (INPCK |          // allow to execute parity
                                            //error if a parity error occured.
                            ISTRIP);        // reset normal data to 7 bits
                                            //and leave last bit to parity bit.
        break;
    case 'E':
        newtio.c_cflag |= PARENB;           // set parity bit to even mode
        newtio.c_cflag &= ~PARODD;          // clear odd mode
        newtio.c_iflag |= (INPCK | ISTRIP); // same with odd setting code
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;              // no check bit
        break;
    default:
        perror("Set_opt wrong event");
        return -1;
    }

    // set bandrate
    switch(nSpeed) {
    case 9600:
        cfsetispeed(&newtio, B9600);    // input speed
        cfsetospeed(&newtio, B9600);    // output speed
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
        // you can add other speed items at here.
    default:
        perror("Set_opt wrong speed OR other speed");
        return -1;
    }

    // set stop bits
    if ( 1 == nStop)
        newtio.c_cflag &= ~CSTOPB;      // stop 1 bit
    else if ( 2 == nStop)
        newtio.c_cflag |= CSTOPB;       // stop 2 bits

    newtio.c_cc[VTIME] = 0;             // waitting time
    newtio.c_cc[VMIN] = 0;              // min receive data

    tcflush(fd, TCIOFLUSH);             // before clean input & output buffer

    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {   // set argument and take effect at once
        tcsetattr(fd, TCSANOW, &oldtio);    // if wrong, get the old settings
        perror("Set_opt com set error");
        return -1;
    }

    tcflush(fd, TCIOFLUSH);             // after clean input & output buffer
    printf("new cflag = %x\n", newtio.c_cflag);
    printf("new iflag = %x\n", newtio.c_iflag);
    printf("new lflag = %x\n", newtio.c_lflag);
    printf("new oflag = %x\n", newtio.c_oflag);

    printf("set down!\n");
    return 0;
}

int openPort() {
    int fd;
    int flag;
    if (-1 == (fd = open("/dev/ttymxc3",
                        O_RDWR |            // file open with read & write
                        O_NOCTTY |          // if device is a tty, do not
                                            //allow it as a control tty.
                        O_NONBLOCK)) ) {    // set non block mode
        perror ("Can't Open Serial Port");
        return -1;
    }

    if (isatty(STDIN_FILENO) == 0) {        // check tty
        printf("standard input is not a terminal device \n");
    }
    else {
        printf("isatty success\n");
    }
    return fd;
}

