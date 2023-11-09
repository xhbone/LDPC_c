1. All decoding function is in the dec.c
2. main.c is the main function, the decoding function is called in the main.c
3. test_ms_flooding_588 is an example of the executable file. It contains 10 items:
#0:main function
#1:H matrix
#2:the number of parity bits
#3:the file to store the results
#4:channel
#5:SNR
#6: decoding function
#7 maximum number of iteration
#8 maximum number of decoding failures
#9 maximum number of decoding frames