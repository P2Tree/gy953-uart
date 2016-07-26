/**
 * @file    gy953_main.c
 * @brief   simple test for gy953 control. This file is used to contect with gy953 module with the dependent files in the follow list; Basic test is request euler angles of three axle from gy953 module.  * @dependent files:    gy953_uart.c gy953_com.c
 * @author  PWE dicksonliuming@gmail.com
 *
 * *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>        // POSIX terminal input/output header
#include "gy953_uart.h"
#include "gy953_com.h"

int main(int argc, char *argv[])
{
    int fd;
    int i;
    char recData[MAXLEN];
    int receiveLen = 0;
    unsigned char sendData[WRITELEN] = "\0"; 
    int accData1, accData2, accData3;
    float angle_x, angle_y, angle_z;
    int wflag;
    fd = uartInit();

    /* constructCommand(AUTODATA_ACCELEROMETER, sendData); */
    /* constructCommand(AUTODATA_EULERANGLE, sendData); */
    constructCommand(EULERANGLE, sendData);
    while(1) {
        sleep(1);
        wflag = sendCommand(fd, sendData, WRITELEN);
        if (-1 == wflag) {
            printf("wflag = %d", wflag);
            printf("send error\n");
            for (i=0; i<wflag; i++)
                printf("%02x ", sendData[i]);
            printf("\n");
            printf("---\n");
            continue;
        }
        /* printf("send.\n"); */
        receiveLen = receiveData(fd, recData, MAXLEN);
        if ( -1 == analysisEulerangle(recData, receiveLen, &angle_x, &angle_y, &angle_z)) {
            printf("receive error\n");
            printf("receiveLen: %d\n", receiveLen);
            for (i=0; i<receiveLen; i++)
                printf("%02x ", recData[i]);
            printf("\n");
            printf("---\n");
            continue;
        }
        //analysisAccelerometer(recData, receiveLen, &accData1,&accData2, &accData3);
        printf("angle x: %.2f ", angle_x);
        printf("angle y: %.2f ", angle_y);
        printf("angle z: %.2f\n", angle_z);
        /* accData1 = 0; */
        /* accData2 = 0; */
        /* accData3 = 0; */
        angle_x = 0;
        angle_y = 0;
        angle_z = 0;
        
    }

    uartClose(fd);
    return 0;
}
