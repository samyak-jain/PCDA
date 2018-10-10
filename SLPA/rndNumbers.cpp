# 8 "rndNumbers.cpp"
#include "rndNumbers.h"

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

double rndDblBtw01(){



 srand (time(NULL));
 return (double)rand()/RAND_MAX;
}
int rndDblBtw0Nminus1(int N){



 srand (time(NULL));
 return rand() % N;
}
