CC = arm-fsl-linux-gnueabi-gcc
OBJ = gy953_com.o gy953_uart.o gy953_main.o
TAG = gy953

$(TAG) : $(OBJ)
	$(CC) -o $(TAG) $(OBJ)

gy953_main.o : gy953_com.h gy953_uart.h

gy953_com.o : gy953_com.h gy953_uart.h

gy953_uart.o : gy953_uart.h

.PHONY : clean

clean :
	-rm $(TAG) $(OBJ)


