#ifndef KAPPA_THREADING_H
#define KAPPA_THREADING_H

#include <thread>

static const unsigned int NON_WORKER_THREADS = 2;

unsigned int get_no_of_cpus(void);

void pin_thread(int, std::thread &);

#endif //KAPPA_THREADING_H