/*****************************************************************
 *
 * @file    gy953_com.c
 * @brief   functions packet of gy953 control.
 * @author  PWE dicksonliuming@gmail.com
 * @date    2016-7-19
 *
 * ***************************************************************/

#include <stdio.h>
#include <termios.h>        // POSIX terminal input/output header
#include "gy953_uart.h"
#include "gy953_com.h"

int analysisEulerangle(unsigned char *originData, int len, float *data1, float *data2, float *data3) {
    int da1, da2, da3;
    if (-1 == analysisData(originData, len, &da1, &da2, &da3)) {
        printf("analysis eulerangle wrong\n");
        return -1;
    }
    if (da1 > 32768 || da1 == 32768)
        da1 = -(65535-da1);
    if (da2 > 32768 || da2 == 32768)
        da2 = -(65535-da2);
    if (da3 > 32768 || da2 == 32768)
        da3 = -(65535-da3);
    *data1 = (float)da1 / 100;
    *data2 = (float)da2 / 100;
    *data3 = (float)da3 / 100;
    return 0;
}

int analysisAccelerometer(unsigned char *originData, int len, int *data1, int *data2, int *data3) {
    if ( -1 == analysisData(originData, len, data1, data2, data3)) {
        printf("analysis accelerometer wrong\n");
        return -1;
    }
    if (*data1 > 32768 || *data1 == 32768) {
        *data1 = -(65535-*data1);
    }
    if (*data2 > 32768 || *data2 == 32768) {
        *data2 = -(65535-*data2);
    }
    return 0;
}

int analysisData(unsigned char *originData, int len, int *data1, int *data2, int *data3) {
    int i=0;
    if (checkCS(originData, len)) {
        printf("data wrong\n");
        return -1;
    }
    *data1 = (unsigned int)(originData[i+4] << 8 | originData[i+5]);
    *data2 = (unsigned int)(originData[i+6] << 8 | originData[i+7]);
    *data3 = (unsigned int)(originData[i+8] << 8 | originData[i+9]);
    return 0;
}

int checkCS(char *data, int len) {
    int i;
    char add = 0x00;
    if (0x5a != data[0] || 0x5a != data[1]) {
        printf("frame header wrong\n");
        return -1;
    }

    for (i=0; i<len-1; i++) {
        add = add + data[i];
    }
    if (add == data[i]) {
        return 0;
    }
    else {
        printf("cs bit wrong\n");
        return -1;
    }
}

int constructCommand(int hexCommand, unsigned char *command) {
    int i;
    for (i=0; i<3; i++) {
        command[i] = (unsigned char)((hexCommand >> ((2-i) * 8)) & 0x0000ff);
        /* printf("%02x\n", command[i]); */
    }
    return 0;
}

int sendCommand(int fd, unsigned char *command, unsigned int len) {
    if (!write(fd, command, len))
        return -1;
    tcflush(fd, TCOFLUSH);
    return 0;
}

int receiveData(int fd, unsigned char *command, unsigned int maxlen) {
    unsigned int nread = 0;
    nread = read(fd, command, maxlen);
    if (!nread)
        return -1;
    tcflush(fd, TCIFLUSH);
    return nread;
}

int uartInit(void) {
    int fd;
    if ((fd = openPort()) < 0) {
        perror("open_port error");
        return -1;
    }
    if (setOpt(fd, 115200, 8, 'N', 1) < 0) {
        perror("set_opt error");
        return -1;
    }
    printf("init uart down\nfd = %d\n", fd);
    return fd;
}

int uartClose(int fd) {
    close(fd);
    return 0;
}

void testReadWrite(int fd, char *recData, char *sendData) {
    int i;
    /* Test receive */
    int receiveLen;
    receiveLen = receiveData(fd, recData, MAXLEN);
    if (receiveLen > 0) {
        printf("receive: %s\n", recData);
    }
    sleep(1);
    /*Test send*/
    constructCommand(BAUDRATE_9600, sendData);
    printf("command is: 0x");
    for (i=0; i<3; i++)
        printf("%02x", sendData[i]);
    printf("\n");
    if (sendCommand(fd, sendData, WRITELEN)) {
        printf("send error\n");
    }
    sleep(1);
}
