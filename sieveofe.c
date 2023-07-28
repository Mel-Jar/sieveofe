//
// Created by Mason Landis on 3/30/22.
//$time a.out
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#define LENGTH 10000
#define NUM_THREADS 4
struct Content{
    int lower, upper, current;
    pthread_mutex_t mutex;
    char numbers[LENGTH];
};
void *worker(void *param);

int main() {
    int largest_prime = 0;
    int start = 0;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_t tid[NUM_THREADS];
    struct Content num = {.lower = start, .upper = LENGTH, .current = start, .mutex = mutex};
    for (int p = start; p < LENGTH; p++){
        num.numbers[p] = 1;
    }
    for(int p = 0; p < NUM_THREADS; p++){
        pthread_create(&tid[p], NULL, worker, &num);
    }
    for(int p = 0; p < NUM_THREADS; p++){
        pthread_join(tid[p], NULL);
    }
    for (int p = 0; p < LENGTH; p++){
        //printf("%d ", num.numbers[p]);
        //printf("%d ", p+2);
        if(num.numbers[p] == -1 || num.numbers[p] == 1 ){
            //printf("%d ", p+2);
            if(p+2 > largest_prime){
                largest_prime = p+2;
            }
        }
    }
    printf("%d ", largest_prime);
    return 0;
}


void *worker(void *param) {
    static struct Content *num;
    pthread_mutex_t *mutex;
    num = (struct Content *) param;
    int upper = num->upper;
    int lower = num->lower;
    int n, i;
    double s = sqrt(upper);
    int done = 0;
    mutex = (pthread_mutex_t *) &num->mutex;
    static char numbers[LENGTH];
    for (int p = lower; p < upper; p++){
        numbers[p] = num->numbers[p];
        //printf(" %d ", numbers[p]);
    }
    //printf(" %d ", num->numbers[i]);
    do{
        pthread_mutex_lock(mutex);
        i = num->current;
        if (i <= s) {
            while(numbers[i] != 1 && i <= s) {
                //printf(" %d ", i);
                i++;
            }
            if (numbers[i] == 1) {
                //numbers[i] = -1;
                //num->numbers[i] = -1;
            }
            num->current = i+1;
            //printf(" %d ", i);
        } else {
            done = 1;
        }
        pthread_mutex_unlock(mutex);
        if (i <= s){
            for (int k = lower+2; k < upper; k++) {
                n = (i + 2) * k;
                //printf("%d ", n);
                if (n - 2 <= upper) {
                    numbers[n - 2] = 0;
                    num->numbers[n - 2] = 0;
                } else {
                    k = upper * 2;
                }
            }
        }
    }while(!done);
    pthread_exit(0);
}

