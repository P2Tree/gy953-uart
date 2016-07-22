# gy953-uart
a simple project of embedded linux to control gy953 module

# Brief

GY953 is a 9 shaft AHRS module. AHRS is the name of Attitude and Heading Reference System, means that you can acquire different attitude value from the module. GY953 can obtain the accelerometer, gyroscope, geomagnetism of the posture your device set. The module have two ways to connect with mcu, uart and spi interface, and in this project, uart connecting was used.

My platform is embedded linux, so driver of uart was already uploaded to the platform. The project not contain any drivers of linux.

# Platform

    - Software:     embedded linux 3.0.2
    - Hardware:     freescale i.mx6q
    - Remote development system:    Ubuntu 14.04
    - Gcc:          arm-fsl-linux-gnueabi-gcc

# Using

You can buy a new GY953 and connect it with your device in uart interface, and then, easy to run software in embedded linux device.

# Aboutme

You can report bugs or problems to me by email: `dicksonliuming@gmail.com`, or easy to issue in github: `PWESiberiaBear`.
