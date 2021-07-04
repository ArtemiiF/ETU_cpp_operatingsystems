#!/bin/bash

g++ -c Laba_4_Filanovskii_prog2.cpp
g++ -o Laba_4_Filanovskii_prog2 Laba_4_Filanovskii_prog2.o 

g++ -c Laba_4_Filanovskii_prog1.cpp
g++ -o Laba_4_Filanovskii_prog1 Laba_4_Filanovskii_prog1.o 

export PATH=$PATH:

./Laba_4_Filanovskii_prog2
