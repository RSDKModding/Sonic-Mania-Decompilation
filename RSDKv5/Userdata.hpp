#ifndef USERDATA_H
#define USERDATA_H

enum StorageDataSets {
    DATASET_STG = 0,
    DATASET_MUS = 1,
    DATASET_SFX = 2,
    DATASET_STR = 3,
    DATASET_TMP = 4,
};

void AllocateStorage(uint size, void *data, StorageDataSets dataSet, bool clearStruct);

#endif
