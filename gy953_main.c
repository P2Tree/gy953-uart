#include <stdio.h>
#include <stdlib.h>
#include "gy953_uart.h"
#include "gy953_com.h"

int main(int argc, char *argv[])
{
    int fd;
    int i;
    char recData[MAXLEN] = "\0";
    int receiveLen = 0;
    unsigned char *sendData; 
    int accData1, accData2, accData3;
    float angle_x, angle_y, angle_z;
    fd = uartInit();
    sendData = (unsigned char*)calloc(3, sizeof(unsigned char));

    /* constructCommand(AUTODATA_ACCELEROMETER, sendData); */
    /* constructCommand(AUTODATA_EULERANGLE, sendData); */
    constructCommand(EULERANGLE, sendData);
    while(1) {
        sleep(1);
        if (sendCommand(fd, sendData, WRITELEN)) {
            printf("send error\n");
            continue;
        }
        printf("send.\n");
        receiveLen = receiveData(fd, recData, MAXLEN);
        // Must not to check receiveLen, why?
        /*
        for (i=0; i<receiveLen; i++)
            printf("%02x ", recData[i]);
        printf("\n");
        */
       
        if ( -1 == analysisEulerangle(recData, receiveLen, &angle_x, &angle_y, &angle_z)) {
            printf("---\n");
            continue;
        }
        //analysisAccelerometer(recData, receiveLen, &accData1,&accData2, &accData3);
        printf("angle x: %.2f\n", angle_x);
        printf("angle y: %.2f\n", angle_y);
        printf("angle z: %.2f\n", angle_z);
        printf("---\n");
        accData1 = 0;
        accData2 = 0;
        accData3 = 0;
        angle_x = 0;
        angle_y = 0;
        angle_z = 0;
        
    }

    uartClose(fd);
    return 0;
}
