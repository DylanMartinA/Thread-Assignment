#include <iostream>
#include <thread>
#include <semaphore.h>

using namespace std;

sem_t sem;

void runMeFirst() {
    cout << "Run me first" << endl;
    // Signal the semaphore to allow the next function to run
    sem_post(&sem);
}

void runMeSecond() {
    // Wait on the semaphore until it's signaled by runMeFirst
    sem_wait(&sem);
    cout << "I run second" << endl;
}

int main() {
    // Initialize semaphore
    sem_init(&sem, 0, 0);

    thread first(runMeFirst);
    thread second(runMeSecond);

    first.join();
    second.join();

    //Cleanup
    sem_destroy(&sem);

    return 0;
}