#include <iostream>
#include <thread>
#include <semaphore.h>

using namespace std;

sem_t sem1, sem2;

void runMeFirst() {
    cout << "Run me first" << endl;
    // Signal the first semaphore to allow the second function to run
    sem_post(&sem1);
}

void runMeSecond() {
    // Wait on the first semaphore until it's signaled by runMeFirst
    sem_wait(&sem1);
    cout << "I run second" << endl;
    // Signal the second semaphore to allow the third function to run
    sem_post(&sem2);
}

void runMeThird() {
    // Wait on the second semaphore until it's signaled by runMeSecond
    sem_wait(&sem2);
    cout << "I run third" << endl;
}

int main() {
    // Initialize semaphores
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    thread first(runMeFirst);
    thread second(runMeSecond);
    thread third(runMeThird);

    first.join();
    second.join();
    third.join();

    // Cleanup
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}