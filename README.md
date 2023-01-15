# Purpose
The purpose of this repository is to store all code for the encryption device driver.

The program is a Linux character device driver that encrypts or decrypts the input string. The program will read and write a string, use ioctl to determine to encrypt or decrypt and to set a key. If ioctl set to encrypt, write a string, then read back the encrypted data. Switch ioctl to decrypt then write the encrypted string and read back the original message (decrypted).

The user application has been included to allow for interaction between user and the device driver. This device driver must be ran on a Linux machine to work.