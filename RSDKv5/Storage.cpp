#include "RetroEngine.hpp"

DataStorage dataStorage[DATASET_MAX];

void InitStorage()
{
    // storage limit (in ints)
    dataStorage[DATASET_STG].storageLimit = 0x1800000;
    dataStorage[DATASET_MUS].storageLimit = 0x800000;
    dataStorage[DATASET_SFX].storageLimit = 0x4000000; // 0x2000000;
    dataStorage[DATASET_STR].storageLimit = 0x200000;
    dataStorage[DATASET_TMP].storageLimit = 0x800000;

    for (int32 s = 0; s < DATASET_MAX; ++s) {
        dataStorage[s].memPtr      = (int32 *)malloc(dataStorage[s].storageLimit);
        dataStorage[s].usedStorage = 0;
        dataStorage[s].rowCount    = 0;
        dataStorage[s].unknown     = 0;
    }
}

void ReleaseStorage()
{
    for (int32 s = 0; s < DATASET_MAX; ++s) {
        if (dataStorage[s].memPtr)
            free(dataStorage[s].memPtr);
        dataStorage[s].memPtr      = NULL;
        dataStorage[s].usedStorage = 0;
        dataStorage[s].rowCount    = 0;
        dataStorage[s].unknown     = 0;
    }

    for (int32 p = 0; p < dataPackCount; ++p) {
        if (dataPacks[p].dataPtr)
            free(dataPacks[p].dataPtr);
        dataPacks[p].dataPtr = NULL;
    }
}

// MemPtr format:
// active
// data set
// start Offset
// size
// [storage bytes]

void AllocateStorage(uint32 size, void **dataPtr, StorageDataSets dataSet, bool32 clear)
{
    int32 **data = (int32 **)dataPtr;
    *data        = NULL;

    if (dataSet < DATASET_MAX) {
        DataStorage *storage = &dataStorage[dataSet];

        if ((size & -4) < size)
            size = (size & -4) + sizeof(int32);

        if (storage->rowCount < STORAGE_ENTRY_COUNT) {
            if (size + sizeof(int32) * storage->usedStorage >= storage->storageLimit) {
                ClearUnusedStorage(dataSet);

                if (size + sizeof(int32) * storage->usedStorage >= storage->storageLimit) {
                    if (storage->rowCount >= STORAGE_ENTRY_COUNT)
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
                    storage->usedStorage += size / sizeof(int32);
                    storage->startPtrs1[storage->rowCount] = data;
                    storage->startPtrs2[storage->rowCount] = *data;
                }
            }
            else {
                storage->memPtr[storage->usedStorage++] = true;
                storage->memPtr[storage->usedStorage++] = dataSet;
                storage->memPtr[storage->usedStorage++] = storage->usedStorage + 2;
                storage->memPtr[storage->usedStorage++] = size;
                *data                                   = &storage->memPtr[storage->usedStorage];
                storage->usedStorage += size / sizeof(int32);
                storage->startPtrs1[storage->rowCount] = data;
                storage->startPtrs2[storage->rowCount] = *data;
            }
            ++storage->rowCount;
            if (storage->rowCount >= STORAGE_ENTRY_COUNT)
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
            int32 **data = (int32 **)dataPtr;
            int32 *ptr   = *data;

            int32 set = *(ptr - 3);

            for (int32 e = 0; e < dataStorage[set].rowCount; ++e) {
                if (data == dataStorage[set].startPtrs1[e]) {
                    dataStorage[set].startPtrs1[e] = NULL;
                }
            }

            int32 c = 0;
            for (int32 e = 0; e < dataStorage[set].rowCount; ++e) {
                if (dataStorage[set].startPtrs1[e]) {
                    if (e != c) {
                        dataStorage[set].startPtrs1[c] = dataStorage[set].startPtrs1[e];
                        dataStorage[set].startPtrs2[c] = dataStorage[set].startPtrs2[e];
                        dataStorage[set].startPtrs1[e] = NULL;
                        dataStorage[set].startPtrs2[e] = NULL;
                    }
                    c++;
                }
            }
            dataStorage[set].rowCount = c;

            for (int32 e = dataStorage[set].rowCount; e < STORAGE_ENTRY_COUNT; ++e) {
                dataStorage[set].startPtrs1[e] = NULL;
                dataStorage[set].startPtrs2[e] = NULL;
            }
            *(ptr - 4) = false;
        }
    }
}

void ClearUnusedStorage(StorageDataSets set)
{
    ++dataStorage[set].unknown;
    CleanEmptyStorage(set);

    if (dataStorage[set].usedStorage) {
        int32 totalSizeA  = 0;
        int32 totalSizeB  = 0;
        int32 usedStorage = 0;
        int32 *memPtr2    = dataStorage[set].memPtr;
        int32 *memPtr     = dataStorage[set].memPtr;

        for (int32 c = 0; c < dataStorage[set].usedStorage;) {
            int32 startOffset = memPtr2[2];
            int32 size        = ((uint32)memPtr2[3] >> 2) + 4;
            *memPtr2          = false;
            int32 *dataPtr    = &dataStorage[set].memPtr[startOffset];

            if (!dataStorage[set].rowCount) {
                memPtr2 += size;
                c           = size + totalSizeA;
                usedStorage = size + totalSizeA;
                totalSizeB += size;
                totalSizeA += size;
            }
            else {
                for (int32 e = 0; e < dataStorage[set].rowCount; ++e) {
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
                            int32 store = *memPtr2;
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
                    usedStorage = size + totalSizeB;
                    totalSizeB += size;
                    totalSizeA += size;
                }
            }
        }

        if (usedStorage) {
            bool32 flag = dataStorage[set].usedStorage == usedStorage;
            dataStorage[set].usedStorage -= usedStorage;
            int32 *memPtr = dataStorage[set].memPtr;

            if (!flag) {
                for (int32 offset = 0; offset < dataStorage[set].usedStorage;) {
                    int32 *ptr = &dataStorage[set].memPtr[memPtr[2]];
                    int32 size = ((uint32)memPtr[3] >> 2) + 4;

                    for (int32 c = 0; c < dataStorage[set].rowCount; ++c) {
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

void CopyStorage(int32 **src, int32 **dst)
{
    if (src) {
        int32 *dstPtr = *dst;
        *src          = *dst;
        int32 dstSet  = dstPtr[-3];

        if (dataStorage[dstSet].rowCount < STORAGE_ENTRY_COUNT) {
            dataStorage[dstSet].startPtrs1[dataStorage[dstSet].rowCount] = src;
            dataStorage[dstSet].startPtrs2[dataStorage[dstSet].rowCount] = *src;

            dstSet = dstPtr[-3];
            if (dataStorage[dstSet].rowCount >= STORAGE_ENTRY_COUNT)
                CleanEmptyStorage((StorageDataSets)dstSet);
        }
    }
}

void CleanEmptyStorage(StorageDataSets set)
{
    if (set < DATASET_MAX) {
        DataStorage *storage = &dataStorage[set];

        for (int32 e = 0; e < storage->rowCount; ++e) {
            if (storage->startPtrs1[e] && *storage->startPtrs1[e] != storage->startPtrs2[e])
                storage->startPtrs1[e] = NULL;
        }

        int32 c = 0;
        for (int32 e = 0; e < storage->rowCount; ++e) {
            if (storage->startPtrs1[e]) {
                if (e != c) {
                    storage->startPtrs1[c] = storage->startPtrs1[e];
                    storage->startPtrs2[c] = storage->startPtrs2[e];
                    storage->startPtrs1[e] = NULL;
                    storage->startPtrs2[e] = NULL;
                }
                c++;
            }
        }
        storage->rowCount = c;

        for (int32 e = storage->rowCount; e < STORAGE_ENTRY_COUNT; ++e) {
            storage->startPtrs1[e] = NULL;
            storage->startPtrs2[e] = NULL;
        }
    }
}