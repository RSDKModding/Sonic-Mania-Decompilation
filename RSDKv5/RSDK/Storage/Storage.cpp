#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

DataStorage RSDK::dataStorage[DATASET_MAX];

bool32 RSDK::InitStorage()
{
    // storage limit (in ints)
    dataStorage[DATASET_STG].storageLimit = 24 * 0x100000; // 24MB
    dataStorage[DATASET_MUS].storageLimit = 8 * 0x100000;  // 8MB
    dataStorage[DATASET_SFX].storageLimit = 64 * 0x100000; // 64MB // 32 * 0x100000; // 32 MB
    dataStorage[DATASET_STR].storageLimit = 1 * 0x100000;  // 1MB
    dataStorage[DATASET_TMP].storageLimit = 8 * 0x100000;  // 8MB

    for (int32 s = 0; s < DATASET_MAX; ++s) {
        dataStorage[s].memoryTable = (int32 *)malloc(dataStorage[s].storageLimit);

        dataStorage[s].usedStorage = 0;
        dataStorage[s].entryCount  = 0;
        dataStorage[s].clearCount  = 0;
    }

    return true;
}

void RSDK::ReleaseStorage()
{
    for (int32 s = 0; s < DATASET_MAX; ++s) {
        if (dataStorage[s].memoryTable)
            free(dataStorage[s].memoryTable);

        dataStorage[s].memoryTable = NULL;
        dataStorage[s].usedStorage = 0;
        dataStorage[s].entryCount  = 0;
        dataStorage[s].clearCount  = 0;
    }

    for (int32 p = 0; p < dataPackCount; ++p) {
        if (dataPacks[p].fileBuffer)
            free(dataPacks[p].fileBuffer);

        dataPacks[p].fileBuffer = NULL;
    }
}

void RSDK::AllocateStorage(uint32 size, void **dataPtr, StorageDataSets dataSet, bool32 clear)
{
    int32 **data = (int32 **)dataPtr;
    *data        = NULL;

    if ((uint32)dataSet < DATASET_MAX) {
        DataStorage *storage = &dataStorage[dataSet];

        if ((size & -4) < size)
            size = (size & -4) + sizeof(int32);

        if (storage->entryCount < STORAGE_ENTRY_COUNT) {
            if (size + sizeof(int32) * storage->usedStorage >= storage->storageLimit) {
                ClearUnusedStorage(dataSet);

                if (size + sizeof(int32) * storage->usedStorage >= storage->storageLimit) {
                    if (storage->entryCount >= STORAGE_ENTRY_COUNT)
                        CleanEmptyStorage(dataSet);

                    if (*data && clear)
                        memset(*data, 0, size);
                }
                else {
                    DataStorageHeader *entry = (DataStorageHeader *)&storage->memoryTable[storage->usedStorage];

                    entry->active     = true;
                    entry->setID      = dataSet;
                    entry->dataOffset = storage->usedStorage + STORAGE_HEADER_SIZE;
                    entry->dataSize   = size;

                    storage->usedStorage += STORAGE_HEADER_SIZE;
                    *data = &storage->memoryTable[storage->usedStorage];
                    storage->usedStorage += size / sizeof(int32);

                    storage->dataEntries[storage->entryCount]    = data;
                    storage->storageEntries[storage->entryCount] = *data;
                }
            }
            else {
                DataStorageHeader *entry = (DataStorageHeader *)&storage->memoryTable[storage->usedStorage];

                entry->active     = true;
                entry->setID      = dataSet;
                entry->dataOffset = storage->usedStorage + STORAGE_HEADER_SIZE;
                entry->dataSize   = size;

                storage->usedStorage += STORAGE_HEADER_SIZE;
                *data = &storage->memoryTable[storage->usedStorage];
                storage->usedStorage += size / sizeof(int32);

                storage->dataEntries[storage->entryCount]    = data;
                storage->storageEntries[storage->entryCount] = *data;
            }

            ++storage->entryCount;
            if (storage->entryCount >= STORAGE_ENTRY_COUNT)
                CleanEmptyStorage(dataSet);

            if (*data && clear)
                memset(*data, 0, size);
        }
    }
}

void RSDK::RemoveStorageEntry(void **dataPtr)
{
    if (dataPtr) {
        if (*dataPtr) {
            int32 **data = (int32 **)dataPtr;

            DataStorageHeader *entry = (DataStorageHeader *)(*data - STORAGE_HEADER_SIZE);
            int32 set                = entry->setID;

            for (int32 e = 0; e < dataStorage[set].entryCount; ++e) {
                if (data == dataStorage[set].dataEntries[e])
                    dataStorage[set].dataEntries[e] = NULL;
            }

            int32 newEntryCount = 0;
            for (int32 entryID = 0; entryID < dataStorage[set].entryCount; ++entryID) {
                if (dataStorage[set].dataEntries[entryID]) {
                    if (entryID != newEntryCount) {
                        dataStorage[set].dataEntries[newEntryCount]    = dataStorage[set].dataEntries[entryID];
                        dataStorage[set].storageEntries[newEntryCount] = dataStorage[set].storageEntries[entryID];
                        dataStorage[set].dataEntries[entryID]          = NULL;
                        dataStorage[set].storageEntries[entryID]       = NULL;
                    }

                    newEntryCount++;
                }
            }
            dataStorage[set].entryCount = newEntryCount;

            for (int32 e = dataStorage[set].entryCount; e < STORAGE_ENTRY_COUNT; ++e) {
                dataStorage[set].dataEntries[e]    = NULL;
                dataStorage[set].storageEntries[e] = NULL;
            }

            entry->active = false;
        }
    }
}

void RSDK::ClearUnusedStorage(StorageDataSets set)
{
    ++dataStorage[set].clearCount;

    CleanEmptyStorage(set);

    if (dataStorage[set].usedStorage) {
        int32 curStorageSize = 0;
        int32 newStorageSize = 0;
        int32 usedStorage    = 0;

        int32 *curMemTablePtr = dataStorage[set].memoryTable;
        int32 *newMemTablePtr = dataStorage[set].memoryTable;

        for (int32 memPos = 0; memPos < dataStorage[set].usedStorage;) {
            DataStorageHeader *curEntry = (DataStorageHeader *)curMemTablePtr;

            int32 size       = ((uint32)curEntry->dataSize >> 2) + STORAGE_HEADER_SIZE;
            curEntry->active = false;

            int32 *dataPtr = &dataStorage[set].memoryTable[curEntry->dataOffset];

            bool32 noCopy = true;
            if (dataStorage[set].entryCount) {
                for (int32 e = 0; e < dataStorage[set].entryCount; ++e) {
                    if (dataPtr == dataStorage[set].storageEntries[e])
                        curEntry->active = true;
                }

                if (curEntry->active) {
                    noCopy = false;

                    curStorageSize += size;
                    memPos = curStorageSize;

                    if (curMemTablePtr <= newMemTablePtr) {
                        newMemTablePtr += size;
                        curMemTablePtr += size;
                    }
                    else {
                        for (; size; --size) {
                            *newMemTablePtr++ = *curMemTablePtr++;
                        }
                    }

                    usedStorage = newStorageSize;
                }
            }

            if (noCopy) {
                curMemTablePtr += size;
                newStorageSize += size;
                curStorageSize += size;

                memPos      = curStorageSize;
                usedStorage = newStorageSize;
            }
        }

        if (usedStorage) {
            bool32 noEntriesRemoved = dataStorage[set].usedStorage != usedStorage;
            dataStorage[set].usedStorage -= usedStorage;

            int32 *memory = dataStorage[set].memoryTable;
            if (noEntriesRemoved) {
                for (int32 memPos = 0; memPos < dataStorage[set].usedStorage;) {
                    DataStorageHeader *entry = (DataStorageHeader *)memory;

                    int32 *dataPtr = &dataStorage[set].memoryTable[entry->dataOffset];
                    int32 size     = ((uint32)entry->dataSize >> 2) + STORAGE_HEADER_SIZE; // size (in int32s)

                    for (int32 c = 0; c < dataStorage[set].entryCount; ++c) {
                        if (dataPtr == dataStorage[set].storageEntries[c]) {
                            *dataStorage[set].dataEntries[c]   = memory + STORAGE_HEADER_SIZE;
                            dataStorage[set].storageEntries[c] = memory + STORAGE_HEADER_SIZE;
                        }
                    }

                    entry->dataOffset = memPos + STORAGE_HEADER_SIZE;
                    memPos += size;
                    memory += size;
                }
            }
        }
    }
}

void RSDK::CopyStorage(int32 **src, int32 **dst)
{
    if (src) {
        int32 *dstPtr = *dst;
        *src          = *dst;

        DataStorageHeader *entry = (DataStorageHeader *)(dstPtr - 4);
        int32 setID              = entry->setID;

        if (dataStorage[setID].entryCount < STORAGE_ENTRY_COUNT) {
            dataStorage[setID].dataEntries[dataStorage[setID].entryCount]    = src;
            dataStorage[setID].storageEntries[dataStorage[setID].entryCount] = *src;

            if (dataStorage[setID].entryCount >= STORAGE_ENTRY_COUNT)
                CleanEmptyStorage((StorageDataSets)setID);
        }
    }
}

void RSDK::CleanEmptyStorage(StorageDataSets set)
{
    if ((uint32)set < DATASET_MAX) {
        DataStorage *storage = &dataStorage[set];

        for (int32 e = 0; e < storage->entryCount; ++e) {
            // So what's happening here is the engine is checking to see if the storage entry
            // (which is the pointer to the "memoryTable" offset that is allocated for this entry)
            // matches what the actual variable that allocated the storage is currently pointing to.
            // if they don't match, the storage entry is considered invalid and marked for removal.

            if (storage->dataEntries[e] && *storage->dataEntries[e] != storage->storageEntries[e])
                storage->dataEntries[e] = NULL;
        }

        int32 newEntryCount = 0;
        for (int32 entryID = 0; entryID < storage->entryCount; ++entryID) {
            if (storage->dataEntries[entryID]) {
                if (entryID != newEntryCount) {
                    storage->dataEntries[newEntryCount]    = storage->dataEntries[entryID];
                    storage->storageEntries[newEntryCount] = storage->storageEntries[entryID];
                    storage->dataEntries[entryID]          = NULL;
                    storage->storageEntries[entryID]       = NULL;
                }

                newEntryCount++;
            }
        }
        storage->entryCount = newEntryCount;

        for (int32 e = storage->entryCount; e < STORAGE_ENTRY_COUNT; ++e) {
            storage->dataEntries[e]    = NULL;
            storage->storageEntries[e] = NULL;
        }
    }
}