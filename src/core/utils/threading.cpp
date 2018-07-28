#include <iostream>
#include <thread>

#include "threading.h"

using namespace std;

unsigned int get_no_of_cpus(void) {
    return thread::hardware_concurrency();
}

void pin_thread(int cpu_no, thread &thread) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_no, &cpuset);

    int err_no = pthread_setaffinity_np(thread.native_handle(), sizeof(cpu_set_t), &cpuset);

    if (err_no != 0) {
        cerr << "pthread_setaffinity_np returned error number " << err_no << endl;
    }
}
