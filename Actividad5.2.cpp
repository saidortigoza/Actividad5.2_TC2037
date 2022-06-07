// =================================================================
//
// File: Actividad5.2.cpp
// Author: Said Ortigoza Trujillo
// Description: This file contains the code that adds all the
//				prime numbers under 5000000 using pthreads.
//              To compile: g++ Actividad5.2.cpp -lpthread
//
// Copyright (c) 2022 by Tecnologico de Monterrey.
// All Rights Reserved. May be reproduced for any non-commercial
// purpose.
//
// =================================================================

#include <iostream>
#include <iomanip>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

using namespace std;

const int SIZEN = 5000000; // 5e^9
const int THREADS = 8;

// Ejecución secuencial

bool isPrime (int num){
    if (num < 2) {
        return false;
    } else {
        bool result = true;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) {
                result = false;
            }
        } return result;
    }
}

double addPrime (int size) {
    double acum = 0;
    for (int i = 0; i <= size; i++) {
        if (isPrime(i)) {
            acum = acum + i;
        }
    } return acum;
}

// Ejecución paralela

typedef struct {
  int start, end;
  int *arr;
} Block;

void* addPrimeParalell (void* param) {
    double *acum;
    Block *block;
    
    block = (Block *) param;
    acum = new double;
    (*acum) = 0;

    for (int i = block->start; i < block->end; i++) {
        if (isPrime(block->arr[i])) {
            (*acum) += block->arr[i];
        }
    } return ((void**) acum);
}

// Main program

int main (int argc, char* argv[]) {
    int *a, block_size;
	double ms, result, *acum;
    
    Block blocks[THREADS];
    pthread_t tids[THREADS];

	a = new int[SIZEN];

    for (int i = 0; i <= SIZEN; i++) {
        a[i] = i;
    }

    block_size = SIZEN / THREADS;
    
    for (int i = 0; i < THREADS; i++) {
        blocks[i].arr = a;
        // Asignacion de start
        blocks[i].start = i * block_size;

        // Chequeo
        if (i != (THREADS - 1)) {
            blocks[i].end = (i + 1) * block_size;
        } else {
            blocks[i].end = SIZEN;
        }
    }

	cout << "Starting...\n";

    start_timer();
    double resultSec = addPrime(SIZEN);
    double tiempoSec = stop_timer();

    cout << "Resultado en secuencial: " << setprecision(5) << resultSec << "\n";
    cout << "Avg execution time:  " << setprecision(5) << (tiempoSec / N) << "\n";

	ms = 0;
    for (int j = 0; j < N; j++) {
        start_timer();
        result = 0;

        for (int i = 0; i < THREADS; i++) {
            // Parametros: hilos, forma en que se manejan los hilos, funcion a ejecutar, resultado
            pthread_create(&tids[i], NULL, addPrimeParalell, (void*)&blocks[i]);
        }

        for (int i = 0; i < THREADS; i++) {
            pthread_join(tids[i], (void**) &acum);
            result += (*acum);
            delete acum;
        }

        ms += stop_timer();
    }


	cout << "Resultado en paralelo: " << setprecision(5) << result << "\n";
	cout << "Avg execution time:  " << setprecision(5) << (ms / N) << "\n";

	delete [] a;
    return 0;
}
