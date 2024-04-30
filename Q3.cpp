#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <vector>
#include <atomic>

using namespace std;

struct requestStructure {
    int request_id;
    string ip_address;
    string page_requested;
};

queue<requestStructure> msg_queue;
mutex queue_mutex;
condition_variable queue_cond;
atomic<int> request_counter(0);
string webPages[10] = {"google.com", "yahoo.com", "bing.com", "duckduckgo.com", "youtube.com", "amazon.com", "wikipedia.com", "twitch.tv", "libraryofbabel.info", "galactanet.com"};

void listen() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));
        requestStructure new_request;
        new_request.request_id = request_counter++;
        new_request.ip_address = "";
        new_request.page_requested = webPages[rand() % 10];

        {
            lock_guard<mutex> lock(queue_mutex);
            msg_queue.push(new_request);
        }

        queue_cond.notify_one();
    }
}

void do_request(int thread_id) {
    while (true) {
        unique_lock<mutex> lock(queue_mutex);
        queue_cond.wait(lock, []{ return !msg_queue.empty(); });

        requestStructure request = msg_queue.front();
        msg_queue.pop();

        lock.unlock();

        // Simulate finding and returning the requested page
        cout << "thread " << thread_id << " completed request " << request.request_id 
             << " requesting webpage " << request.page_requested << endl;
    }
}

int main() {
    srand(time(nullptr));  // Seed the number generator

    // Start listener thread here
    thread listener(listen);

    // Start consumer threads here
    const int num_consumer_threads = 5;
    vector<thread> consumer_threads;
    for (int i = 0; i < num_consumer_threads; ++i) {
        consumer_threads.emplace_back(do_request, i);
    }

    listener.join();  // I dont think this will ever join since the listener is in an infinite loop...
    for (auto& t : consumer_threads) {
        t.join();  // Same here...
    }

    return 0;
}

