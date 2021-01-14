#ifndef DISKANALYZER_SIGNAL_MANAGER_H
#define DISKANALYZER_SIGNAL_MANAGER_H

    #include "Shared/shared.h"

    int initialize_signals(pid_t);
    struct signal_details* get_current_signal();
    void reset_current_signal();

#endif //DISKANALYZER_SIGNAL_MANAGER_H
