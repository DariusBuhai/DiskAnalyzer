#ifndef DISKANALYZER_TEMP_DATA_H
#define DISKANALYZER_TEMP_DATA_H

    #include "Shared/shared.h"

    struct file_details* new_file_details(char*);
    struct file_details* append_file(struct file_details *, struct file_details *);

#endif //DISKANALYZER_TEMP_DATA_H
