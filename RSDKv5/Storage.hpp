#ifndef STORAGE_H
#define STORAGE_H

#define STORAGE_ENTRY_COUNT (0x1000)

enum StorageDataSets {
    DATASET_STG = 0,
    DATASET_MUS = 1,
    DATASET_SFX = 2,
    DATASET_STR = 3,
    DATASET_TMP = 4,
    DATASET_MAX, //used to signify limits
};

struct DataStorage {
    int32 *memPtr;
    uint32 usedStorage;
    uint32 storageLimit;
    int32 **startPtrs1[STORAGE_ENTRY_COUNT];
    int32 *startPtrs2[STORAGE_ENTRY_COUNT];
    uint32 rowCount;
    uint32 unknown;
};

template <typename T> class List
{
    T *entries = NULL;
    int32 count  = 0;
    int32 length  = 0;

    public:
    List()
    {
        entries = NULL;
        count   = 0;
    }
    ~List() {
        if (entries) {
            free(entries);
            entries = NULL;
        }
    }
    T *Append()
    {
        if (count == length) {
            length += 32;
            size_t len = sizeof(T) * length;
            T *entries_realloc = (T *)realloc(entries, len);

            if (entries_realloc) {
                entries = entries_realloc;
            }
        }

        T *entry = &entries[count];
        memset(entry, 0, sizeof(T));
        count++;
        return entry;
    }
    void Remove(uint index)
    {
        // move every item back one
        for (int i = index; i < count; i++) {
            if (i + 1 < count) {
                entries[i]  = entries[i + 1];
            }
            else { // Last Item, free it
                count--;
            }
        }

        if (count < length - 32) {
            length -= 32;
            size_t len = sizeof(T) * length;
            T *entries_realloc = (T *)realloc(entries, len);

            if (entries_realloc)
                entries = entries_realloc;
        }
    }

    inline T *At(int index) { return &entries[index]; }

    inline void Clear(bool32 dealloc = false)
    {
        for (int i = count - 1; i >= 0; i--) {
            Remove(i);
        }

        if (entries && dealloc) {
            free(entries);
            entries = NULL;
        }
    }

    inline int32 Count() { return count; }
};

extern DataStorage dataStorage[DATASET_MAX];

void InitStorage();
void ReleaseStorage();

void AllocateStorage(uint32 size, void **dataPtr, StorageDataSets dataSet, bool32 clear);
void ClearUnusedStorage(StorageDataSets set);
void RemoveStorageEntry(void **dataPtr);
void CopyStorage(int32 **src, int32 **dst);
void CleanEmptyStorage(StorageDataSets dataSet);

#endif // STORAGE_H
