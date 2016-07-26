/**
 *
 * Copyright (C) 2016 Yang Liuming <dicksonliuming@gmail.com>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GUN General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * alonfg with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *************************************************************/

 /**
  * \file   uart-test.c
  * \brief  This file is only used to check your UART in your ARM board
  *         is well enough to work.
  * \brief  I will use too much annotations to show every action in UART.
  * \author Yang Liuming <dicksonliuming@gmail.com>
  *
  *************************************************************/
#include <stdio.h>          // standard input/output header
#include <termios.h>        // POSIX terminal input/output header
#include <memory.h>         // memory control
#include <fcntl.h>          // file control
#include <unistd.h>         // Unix standard
//#include <string.h>           //

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop) {
    struct termios newtio, oldtio;
    if (tcgetattr(fd, &oldtio) != 0) {  // catch current device mode and arguments
        perror("Backup SerialSetting");
        return -1;
    }
    newtio.c_cflag = 0;
    newtio.c_iflag = 0;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;

    newtio.c_cflag |= CLOCAL |          // set local tty, to ignore
                                        //moderm status line to open
                                        //tty in local without block.
                        CREAD;          //allow to read


    // set data bits
    switch(nBits) {
    case 7:
        newtio.c_cflag &= ~CSIZE;           // zero data bits control
        newtio.c_cflag |= CS7;              // set data bits to 7 bits
        break;
    case 8:
        newtio.c_cflag &= ~CSIZE;           // zero data bits control
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
    //tcflush(fd, TCIFLUSH);
    //tcflush(fd, TCOFLUSH);

    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {   // set argument and take effect at once
        tcsetattr(fd, TCSANOW, &oldtio);    // if wrong, get the old settings
        perror("Set_opt com set error");
        return -1;
    }

    tcflush(fd, TCIOFLUSH);             // after clean input & output buffer

    printf("set down!\n");
    return 0;
}

int open_port() {
    int fd;
    int flag;
    if (-1 == (fd = open("/dev/ttymxc2",
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

int main(void) {
    int fd;
    int nread;
    char buff[10];
    char testbf[] = {"abcde"};
    int ntestbf = 5;

    memset(buff, 0, 10);

    if ((fd = open_port()) < 0) {
        perror("open_port error");
        return -1;
    }
    if (set_opt(fd, 9600, 8, 'N', 1) < 0) {
        perror("set_opt error");
        return -1;
    }
    printf("fd = %d\n", fd);

    while(1) {
        nread = read(fd, buff, 10);
        printf("nread = %d\n", nread);
        tcflush(fd, TCIFLUSH);
        if (nread > 0)
            printf("result: %d %s\n", nread, buff);
        //memset(buff, 0, sizeof(buff));
        //write(fd, buff, nread);
        write(fd, testbf, ntestbf);
        sleep(1);
        tcflush(fd, TCOFLUSH);
    }
    close(fd);
    return 0;
}
