#include <iostream>
#include <thread>

using namespace std;
/**/
bool stop = true;
void thread1() { cout << "I am thread 1\n"; stop=false; }
void thread2() { while (stop); cout << "I am thread2\n"; }

int main() {
    thread t1, t2;
    t1 = thread(thread1);
    t2 = thread(thread2);
    t1.join();
    t2.join();
    return 0;
}

/*
void thread1(int & c) { for (int i=0; i<100; i++) c++; }
void thread2(int & c) { for (int i=0; i<100; i++) c++; }
int main() {
    thread t1, t2;
    int counter = 0;
    t1 = thread(thread1, ref(counter));
    t2 = thread(thread2, ref(counter));
    t1.join(); t2.join();
    return 0;
}
*/