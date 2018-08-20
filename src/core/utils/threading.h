#ifndef KAPPA_THREADING_H
#define KAPPA_THREADING_H

#include <thread>

static const unsigned int NON_WORKER_THREADS = 1;

unsigned int get_no_of_cpus(void);

void pin_thread(int, std::thread &);

void burn_cpu(void);

#endif //KAPPA_THREADING_H