

/* 
 * File:   Misc.h
 * Author: Fred
 *
 * Created on 2 agosto 2012, 20.16
 */

#include <stdlib.h>
//#include <math.h>

using namespace std;

#define BIAS false

#define LEARNING_RATE 0.80      // 0.75
#define MOMENTUM 0.03            // 0.2

#define MAX_ERROR 0.00011
#define MAX_ITER 1000
#define UPDATE 150


inline double dRand() { // Genera un numero random compreso tra 0 e 1    
    return ((double)(rand())/(double)RAND_MAX);
}

inline double wRand() { // Genera un numero random compreso tra -1 e 1
	return sin( (double)rand() );
}


