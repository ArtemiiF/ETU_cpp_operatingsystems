#!/bin/bash

g++ -c Laba_8_Filanovskii_prog1.cpp
g++ -o Laba_8_Filanovskii_prog1 Laba_8_Filanovskii_prog1.o -lpthread -lrt 

sudo setcap cap_sys_resource=eip ./Laba_8_Filanovskii_prog1
./Laba_8_Filanovskii_prog1





