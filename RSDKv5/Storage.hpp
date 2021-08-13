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
    uint rowCount;
    uint unknown;
};

template <typename T> class List
{
    T *entries = NULL;
    int count  = 0;
    int length  = 0;

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

    inline int Count() { return count; }
};

extern DataStorage dataStorage[DATASET_MAX];

void InitStorage();
void ReleaseStorage();

void AllocateStorage(uint size, void **dataPtr, StorageDataSets dataSet, bool32 clear);
void ClearUnusedStorage(StorageDataSets set);
void RemoveStorageEntry(void **dataPtr);
void CopyStorage(int **src, int **dst);
void CleanEmptyStorage(StorageDataSets dataSet);

#endif // STORAGE_H
