#ifndef STORAGE_H
#define STORAGE_H

enum StorageDataSets {
    DATASET_STG = 0,
    DATASET_MUS = 1,
    DATASET_SFX = 2,
    DATASET_STR = 3,
    DATASET_TMP = 4,
    DATASET_MAX, //used to signify limits
};

struct DataStorage {
    int *memPtr;
    uint usedStorage;
    uint storageLimit;
    int **startPtrs1[0x1000];
    int *startPtrs2[0x1000];
    uint entryCount;
    uint unknown;
};

extern DataStorage userStorage[DATASET_MAX];

void InitialiseUserStorage();
void ReleaseUserStorage();

void AllocateStorage(uint size, void **dataPtr, StorageDataSets dataSet, bool32 clear);
void ClearUnusedStorage(StorageDataSets set);
void RemoveStorageEntry(int *data);
void CopyStorage(int **src, int **dst);
void CleanEmptyStorage(StorageDataSets dataSet);

#endif // STORAGE_H
