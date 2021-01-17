#ifndef DISKANALYZER_PROCESS_MANAGER_H
#define DISKANALYZER_PROCESS_MANAGER_H

#include "../Shared/shared.h"

void update_ids();
void take_new_task();
void check_processes();
int process_signal(struct signal_details);

#endif //DISKANALYZER_PROCESS_MANAGER_H
