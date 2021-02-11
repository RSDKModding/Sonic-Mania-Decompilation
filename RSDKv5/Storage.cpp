#include "RetroEngine.hpp"

DataStorage userStorage[DATASET_MAX];

void InitialiseUserStorage()
{
    // storage limit (in bytes)
    userStorage[DATASET_STG].storageLimit = 0x1800000; 
    userStorage[DATASET_MUS].storageLimit = 0x800000; 
    userStorage[DATASET_SFX].storageLimit = 0x2000000;
    userStorage[DATASET_STR].storageLimit = 0x200000; 
    userStorage[DATASET_TMP].storageLimit = 0x800000; 
    
    for (int s = 0; s < DATASET_MAX; ++s) {
        userStorage[s].memPtr = (int*)malloc(userStorage[s].storageLimit);
        userStorage[s].usedStorage = 0;
        userStorage[s].entryCount  = 0;
        userStorage[s].unknown    = 0;
    }
}

void ReleaseUserStorage()
{
    for (int s = 0; s < DATASET_MAX; ++s) {
        if (userStorage[s].memPtr)
            free(userStorage[s].memPtr);
        userStorage[s].usedStorage = 0;
        userStorage[s].entryCount  = 0;
        userStorage[s].unknown    = 0;
    }
}

// MemPtr format:
// active
// data set
// start Offset
// size
// [storage bytes]

void AllocateStorage(uint size, void **dataPtr, StorageDataSets dataSet, bool32 clear) {
    int **data = (int **)dataPtr;
    *data       = NULL;
    if (dataSet < DATASET_MAX) {
        DataStorage *storage = &userStorage[dataSet];

        if ((size & 0xFFFFFFFC) < size)
            size &= 0xFFFFFFFC;

        if (storage->entryCount < 0x1000) {
            if (size + sizeof(int) * storage->usedStorage >= storage->storageLimit) {
                ClearUnusedStorage(dataSet);

                if (size + sizeof(int) * storage->usedStorage >= storage->storageLimit) {
                    if (storage->entryCount >= 0x1000)
                        CleanEmptyStorage(dataSet);

                    if (*data && clear) {
                        memset(*data, 0, size);
                    }
                }
                else {
                    storage->memPtr[storage->usedStorage++] = true;
                    storage->memPtr[storage->usedStorage++] = dataSet;

                    storage->memPtr[storage->usedStorage++] = storage->usedStorage + 2;
                    storage->memPtr[storage->usedStorage++] = size;
                    *data                                   = &storage->memPtr[storage->usedStorage];
                    storage->usedStorage += size >> 2;
                    storage->startPtrs1[storage->entryCount] = data;
                    storage->startPtrs2[storage->entryCount] = *data;
                }
            }
            else {
                storage->memPtr[storage->usedStorage++] = true;
                storage->memPtr[storage->usedStorage++] = dataSet;
                storage->memPtr[storage->usedStorage++] = storage->usedStorage + 2;
                storage->memPtr[storage->usedStorage++] = size;
                *data                                   = &storage->memPtr[storage->usedStorage];
                storage->usedStorage += size >> 2;
                storage->startPtrs1[storage->entryCount] = data;
                storage->startPtrs2[storage->entryCount] = *data;
            }
            ++storage->entryCount;
            if (storage->entryCount >= 0x1000)
                CleanEmptyStorage(dataSet);

            if (*data && clear) {
                memset(*data, 0, size);
            }
        }
    }
}

void RemoveStorageEntry(int* data) {
    if (data) {
        if (data) {
            int set = data[-3];

            for (int e = 0; e < userStorage[set].entryCount; ++e) {
                if (data == *userStorage[set].startPtrs1[e]) {
                    *userStorage[set].startPtrs1[e] = NULL;
                }
            }

            int c = 0;
            for (int e = 0; e < userStorage[set].entryCount; ++e) {
                if (*userStorage[set].startPtrs1[e]) {
                    if (e != c) {
                        userStorage[set].startPtrs1[c] = userStorage[set].startPtrs1[e];
                        userStorage[set].startPtrs2[c] = userStorage[set].startPtrs2[e];
                        userStorage[set].startPtrs1[e] = NULL;
                        userStorage[set].startPtrs2[e] = NULL;
                    }
                    c++;
                }
            }
            userStorage[set].entryCount = c;

            for (int e = userStorage[set].entryCount; e < 0x1000; ++e) {
                userStorage[set].startPtrs1[e] = NULL;
                userStorage[set].startPtrs2[e] = NULL;
            }
            data[-4] = false;
        }
    }
}

void ClearUnusedStorage(StorageDataSets set)
{
    ++userStorage[set].unknown;
    CleanEmptyStorage(set);

    if (userStorage[set].usedStorage) {
        int totalSizeA  = 0;
        int totalSizeB  = 0;
        int usedStorage = 0;
        int *memPtr2    = userStorage[set].memPtr;
        int *memPtr     = userStorage[set].memPtr;

        for (int c = 0; c < userStorage[set].usedStorage;) {
            int startOffset = memPtr2[2];
            int size        = ((uint)memPtr[3] >> 2) + 4;
            *memPtr         = false;
            int *dataPtr    = &memPtr[startOffset];

            if (!userStorage[set].entryCount) {
                memPtr2 += size;
                c           = size + totalSizeA;
                usedStorage = size + totalSizeA;
                totalSizeB += size;
                totalSizeA += size;
            }
            else {
                for (int e = 0; e < userStorage[set].entryCount; ++e) {
                    if (dataPtr == userStorage[set].startPtrs2[e])
                        *memPtr = true;
                }

                if (*memPtr) {
                    c = size + totalSizeA;
                    totalSizeA += size;
                    if (memPtr <= memPtr2) {
                        memPtr += size;
                        memPtr2 += size;
                    }
                    else {
                        for (; size; --size) {
                            int store = *memPtr;
                            ++memPtr;
                            *memPtr2 = store;
                            ++memPtr2;
                        }
                    }
                    usedStorage = totalSizeB;
                }
                else {
                    memPtr2 += size;
                    c           = size + totalSizeA;
                    usedStorage = size + totalSizeA;
                    totalSizeB += size;
                    totalSizeA += size;
                }
            }
        }

        if (usedStorage) {
            bool flag = userStorage[set].usedStorage == usedStorage;
            userStorage[set].usedStorage -= usedStorage;
            int offset  = 0;
            int *memPtr = userStorage[set].memPtr;

            if (!flag) {
                for (int offset = 0; offset < userStorage[set].usedStorage; ++offset) {
                    int *ptr = &userStorage[set].memPtr[memPtr[2]];
                    int size = ((uint)memPtr[3] >> 2) + 4;

                    for (int c = 0; c < userStorage[set].entryCount; ++c) {
                        if (ptr == userStorage[set].startPtrs2[c]) {
                            *userStorage[set].startPtrs1[c] = memPtr + 4;
                            userStorage[set].startPtrs2[c]  = memPtr + 4;
                        }
                    }

                    offset += size;
                    memPtr[2] = offset + 4; // offset
                    memPtr += size;
                }
            }
        }
    }
}

void CopyStorage(int **src, int **dst)
{
    if (src) {
        int *dstPtr = *dst;
        *src        = *dst;
        int dstSet  = dstPtr[-3];

        if (userStorage[dstSet].entryCount < 0x1000) {
            userStorage[dstSet].startPtrs1[userStorage[dstSet].entryCount] = src;
            userStorage[dstSet].startPtrs2[userStorage[dstSet].entryCount] = *src;

            dstSet = dstPtr[-3];
            if (userStorage[dstSet].entryCount >= 0x1000)
                CleanEmptyStorage((StorageDataSets)dstSet);
        }
    }
}

void CleanEmptyStorage(StorageDataSets set)
{
    if (set < DATASET_MAX) {
        DataStorage *storage = &userStorage[set];

        for (int e = 0; e < storage->entryCount; ++e) {
            if (*storage->startPtrs1[e] && *storage->startPtrs1[e] != storage->startPtrs2[e])
                *storage->startPtrs1[e] = NULL;
        }

        int c = 0;
        for (int e = 0; e < storage->entryCount; ++e) {
            if (*storage->startPtrs1[e]) {
                if (e != c) {
                    storage->startPtrs1[c] = storage->startPtrs1[e];
                    storage->startPtrs2[c] = storage->startPtrs2[e];
                    storage->startPtrs1[e] = NULL;
                    storage->startPtrs2[e] = NULL;
                }
                c++;
            }
        }
        storage->entryCount = c;

        for (int e = storage->entryCount; e < 0x1000; ++e) {
            storage->startPtrs1[e] = NULL;
            storage->startPtrs2[e] = NULL;
        }
    }
}