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
    unsigned char *sendData; 
    int accData1, accData2, accData3;
    float angle_x, angle_y, angle_z;
    int wflag;
    fd = uartInit();
    sendData = (unsigned char*)calloc(3, sizeof(unsigned char));

    /* constructCommand(AUTODATA_ACCELEROMETER, sendData); */
    /* constructCommand(AUTODATA_EULERANGLE, sendData); */
    constructCommand(EULERANGLE, sendData);
    while(1) {
        sleep(1);
        wflag = sendCommand(fd, sendData, WRITELEN);
        if (-1 == wflag) {
            printf("wflag = %d", wflag);
            printf("send error\n");
            continue;
        }
        printf("wflag = %d", wflag);
        printf("send.\n");
        /* usleep(100000); */
        receiveLen = receiveData(fd, recData, MAXLEN);
        if ( -1 == analysisEulerangle(recData, receiveLen, &angle_x, &angle_y, &angle_z)) {
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
