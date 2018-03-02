#ifndef KAPPA_DATASET_SPLIT_H
#define KAPPA_DATASET_SPLIT_H

uint32_t number_of_lines(std::string);
std::vector<uint32_t> dataset_to_batches(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

#endif //KAPPA_DATASET_SPLIT_H
