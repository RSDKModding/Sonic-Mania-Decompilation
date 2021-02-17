#include "RetroEngine.hpp"

DataStorage dataStorage[DATASET_MAX];

void InitStorage()
{
    // storage limit (in ints)
    dataStorage[DATASET_STG].storageLimit = 0x1800000; 
    dataStorage[DATASET_MUS].storageLimit = 0x800000; 
    dataStorage[DATASET_SFX].storageLimit = 0x2000000;
    dataStorage[DATASET_STR].storageLimit = 0x200000; 
    dataStorage[DATASET_TMP].storageLimit = 0x800000; 
    
    for (int s = 0; s < DATASET_MAX; ++s) {
        dataStorage[s].memPtr = (int*)malloc(dataStorage[s].storageLimit);
        dataStorage[s].usedStorage = 0;
        dataStorage[s].entryCount  = 0;
        dataStorage[s].unknown    = 0;
    }
}

void ReleaseStorage()
{
    for (int s = 0; s < DATASET_MAX; ++s) {
        if (dataStorage[s].memPtr)
            free(dataStorage[s].memPtr);
        dataStorage[s].usedStorage = 0;
        dataStorage[s].entryCount  = 0;
        dataStorage[s].unknown    = 0;
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

    if (dataSet == DATASET_STG)
        printf("");

    if (dataSet < DATASET_MAX) {
        DataStorage *storage = &dataStorage[dataSet];

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

void RemoveStorageEntry(void **dataPtr)
{
    if (dataPtr) {
        if (*dataPtr) {
            int **data = (int **)dataPtr;
            *data      = NULL;

            int set = *data[-3];

            for (int e = 0; e < dataStorage[set].entryCount; ++e) {
                if (data == dataStorage[set].startPtrs1[e]) {
                    *dataStorage[set].startPtrs1[e] = NULL;
                }
            }

            int c = 0;
            for (int e = 0; e < dataStorage[set].entryCount; ++e) {
                if (*dataStorage[set].startPtrs1[e]) {
                    if (e != c) {
                        dataStorage[set].startPtrs1[c] = dataStorage[set].startPtrs1[e];
                        dataStorage[set].startPtrs2[c] = dataStorage[set].startPtrs2[e];
                        dataStorage[set].startPtrs1[e] = NULL;
                        dataStorage[set].startPtrs2[e] = NULL;
                    }
                    c++;
                }
            }
            dataStorage[set].entryCount = c;

            for (int e = dataStorage[set].entryCount; e < 0x1000; ++e) {
                dataStorage[set].startPtrs1[e] = NULL;
                dataStorage[set].startPtrs2[e] = NULL;
            }
            *data[-4] = false;
        }
    }
}

void ClearUnusedStorage(StorageDataSets set)
{
    ++dataStorage[set].unknown;
    CleanEmptyStorage(set);

    if (dataStorage[set].usedStorage) {
        int totalSizeA  = 0;
        int totalSizeB  = 0;
        int usedStorage = 0;
        int *memPtr2    = dataStorage[set].memPtr;
        int *memPtr     = dataStorage[set].memPtr;

        for (int c = 0; c < dataStorage[set].usedStorage;) {
            int startOffset = memPtr2[2];
            int size        = ((uint)memPtr2[3] >> 2) + 4;
            *memPtr2         = false;
            int *dataPtr    = &dataStorage[set].memPtr[startOffset];

            if (!dataStorage[set].entryCount) {
                memPtr2 += size;
                c           = size + totalSizeA;
                usedStorage = size + totalSizeA;
                totalSizeB += size;
                totalSizeA += size;
            }
            else {
                for (int e = 0; e < dataStorage[set].entryCount; ++e) {
                    if (dataPtr == dataStorage[set].startPtrs2[e])
                        *memPtr2 = true;
                }

                if (*memPtr2) {
                    c = size + totalSizeA;
                    totalSizeA += size;
                    if (memPtr2 <= memPtr) {
                        memPtr += size;
                        memPtr2 += size;
                    }
                    else {
                        for (; size; --size) {
                            int store = *memPtr2;
                            ++memPtr2;
                            *memPtr = store;
                            ++memPtr;
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
            bool flag = dataStorage[set].usedStorage == usedStorage;
            dataStorage[set].usedStorage -= usedStorage;
            int offset  = 0;
            int *memPtr = dataStorage[set].memPtr;

            if (!flag) {
                for (int offset = 0; offset < dataStorage[set].usedStorage;) {
                    int *ptr = &dataStorage[set].memPtr[memPtr[2]];
                    int size = ((uint)memPtr[3] >> 2) + 4;

                    for (int c = 0; c < dataStorage[set].entryCount; ++c) {
                        if (ptr == dataStorage[set].startPtrs2[c]) {
                            *dataStorage[set].startPtrs1[c] = memPtr + 4;
                            dataStorage[set].startPtrs2[c]  = memPtr + 4;
                        }
                    }

                    memPtr[2] = offset + 4; // offset
                    offset += size;
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

        if (dataStorage[dstSet].entryCount < 0x1000) {
            dataStorage[dstSet].startPtrs1[dataStorage[dstSet].entryCount] = src;
            dataStorage[dstSet].startPtrs2[dataStorage[dstSet].entryCount] = *src;

            dstSet = dstPtr[-3];
            if (dataStorage[dstSet].entryCount >= 0x1000)
                CleanEmptyStorage((StorageDataSets)dstSet);
        }
    }
}

void CleanEmptyStorage(StorageDataSets set)
{
    if (set < DATASET_MAX) {
        DataStorage *storage = &dataStorage[set];

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