#include "RSDK/Core/RetroEngine.hpp"

#if RETRO_REV02

// ====================
// API Cores
// ====================

namespace RSDK::SKU
{
// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyStorage.cpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamStorage.cpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSStorage.cpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXStorage.cpp"
#endif

} // namespace RSDK::SKU

using namespace RSDK;

RSDK::SKU::UserStorage *RSDK::SKU::userStorage     = NULL;
RSDK::SKU::UserDBStorage *RSDK::SKU::userDBStorage = NULL;

// =======================
// USER DB VALUE
// =======================
bool32 RSDK::SKU::UserDBValue::CheckMatch(int32 row, int32 column)
{
    UserDB *userDB     = (UserDB *)parent;
    UserDBValue *other = &userDB->rows[row].values[column];

    switch (userDB->columnTypes[column]) {
        case DBVAR_BOOL:
        case DBVAR_UINT8:
        case DBVAR_INT8: return memcmp(data, other->data, sizeof(int8)) == 0;

        case DBVAR_UINT16:
        case DBVAR_INT16: return memcmp(data, other->data, sizeof(int16)) == 0;

        case DBVAR_UINT32:
        case DBVAR_INT32:
        case DBVAR_FLOAT:
        case DBVAR_COLOR: return memcmp(data, other->data, sizeof(int32)) == 0;

        case DBVAR_STRING: {
            char *thisString  = (char *)data;
            char *otherString = (char *)other->data;

            return strcmp(thisString, otherString) == 0;
        }
        default: break;
    }

    return false;
}
void RSDK::SKU::UserDBValue::Set(int32 type, void *data)
{
    size = 0;
    memset(this->data, 0, sizeof(this->data));
    if (data) {
        switch (type) {
            case DBVAR_BOOL:
            case DBVAR_UINT8:
            case DBVAR_INT8:
                size = sizeof(int8);
                memcpy(this->data, data, sizeof(int8));
                break;

            case DBVAR_UINT16:
            case DBVAR_INT16:
                size = sizeof(int16);
                memcpy(this->data, data, sizeof(int16));
                break;

            case DBVAR_UINT32:
            case DBVAR_INT32:
            case DBVAR_FLOAT:
            case DBVAR_COLOR:
                size = sizeof(int32);
                memcpy(this->data, data, sizeof(int32));
                break;

            case DBVAR_STRING: {
                char *string = (char *)data;

                int32 len = (int32)strlen(string);
                for (int32 c = 0; c < len && c < 15; ++c) {
                    this->data[c] = string[c];
                }
                break;
            }

            default: break;
        }
    }
}
void RSDK::SKU::UserDBValue::Get(int32 type, void *data)
{
    int8 *outData = (int8 *)data;
    switch (type) {
        case DBVAR_BOOL:
        case DBVAR_UINT8:
        case DBVAR_INT8: memcpy(outData, this->data, sizeof(int8)); break;

        case DBVAR_UINT16:
        case DBVAR_INT16: memcpy(outData, this->data, sizeof(int16)); break;

        case DBVAR_UINT32:
        case DBVAR_INT32:
        case DBVAR_FLOAT:
        case DBVAR_COLOR: memcpy(outData, this->data, sizeof(int32)); break;

        case DBVAR_STRING: {
            memset(outData, 0, size + 1);
            char *string = (char *)this->data;
            for (int32 c = 0; c < size; ++c) {
                outData[c] = string[c];
            }
            break;
        }

        default: break;
    }
}

// =======================
// USER DB ROW
// =======================
bool32 RSDK::SKU::UserDBRow::AddValue(int32 type, char *name, void *value)
{
    UserDB *userDB = parent;
    uint32 uuid    = 0;
    GenerateHashCRC(&uuid, name);

    for (int32 c = 0; c < userDB->columnCount; ++c) {
        if (userDB->columnUUIDs[c] == uuid) {
            if (c < 0 || type != userDB->columnTypes[c])
                return false;

            values[c].Set(type, value);

            time_t t;
            time(&t);
            changeTime = *localtime(&t);
            return true;
        }
    }

    return false;
}
bool32 RSDK::SKU::UserDBRow::GetValue(int32 type, char *name, void *value)
{
    UserDB *userDB = parent;
    uint32 uuid    = 0;
    GenerateHashCRC(&uuid, name);

    for (int32 c = 0; c < userDB->columnCount; ++c) {
        if (userDB->columnUUIDs[c] == uuid) {
            if (c < 0 || type != userDB->columnTypes[c])
                return false;

            values[c].Get(type, value);
            return true;
        }
    }

    return false;
}

bool32 RSDK::SKU::UserDBRow::Compare(UserDBRow *other, int32 type, char *name, bool32 sortAscending)
{
    uint8 thisData[0x10];
    uint8 otherData[0x10];
    memset(thisData, 0, sizeof(thisData));
    memset(otherData, 0, sizeof(otherData));

    this->GetValue(type, name, thisData);
    other->GetValue(type, name, otherData);
    switch (type) {
        case DBVAR_BOOL:
        case DBVAR_UINT8: {
            uint8 thisValue = 0, otherValue = 0;
            memcpy(&thisValue, thisData, sizeof(uint8));
            memcpy(&otherValue, otherData, sizeof(uint8));

            if (!sortAscending)
                return thisValue > otherValue;
            else
                return thisValue < otherValue;
            break;
        }

        case DBVAR_INT8: {
            int8 thisValue = 0, otherValue = 0;
            memcpy(&thisValue, thisData, sizeof(int8));
            memcpy(&otherValue, otherData, sizeof(int8));

            if (!sortAscending)
                return thisValue > otherValue;
            else
                return thisValue < otherValue;
            break;
        }

        case DBVAR_UINT16: {
            uint16 thisValue = 0, otherValue = 0;
            memcpy(&thisValue, thisData, sizeof(uint16));
            memcpy(&otherValue, otherData, sizeof(uint16));

            if (!sortAscending)
                return thisValue > otherValue;
            else
                return thisValue < otherValue;
            break;
        }

        case DBVAR_INT16: {
            int16 thisValue = 0, otherValue = 0;
            memcpy(&thisValue, thisData, sizeof(int16));
            memcpy(&otherValue, otherData, sizeof(int16));

            if (!sortAscending)
                return thisValue > otherValue;
            else
                return thisValue < otherValue;
            break;
        }

        case DBVAR_UINT32:
        case DBVAR_COLOR: {
            uint32 thisValue = 0, otherValue = 0;
            memcpy(&thisValue, thisData, sizeof(uint32));
            memcpy(&otherValue, otherData, sizeof(uint32));

            if (!sortAscending)
                return thisValue > otherValue;
            else
                return thisValue < otherValue;
            break;
        }

        case DBVAR_INT32: {
            int32 thisValue = 0, otherValue = 0;
            memcpy(&thisValue, thisData, sizeof(int32));
            memcpy(&otherValue, otherData, sizeof(int32));

            if (!sortAscending)
                return thisValue > otherValue;
            else
                return thisValue < otherValue;
            break;
        }

        case DBVAR_FLOAT: {
            float thisValue = 0, otherValue = 0;
            memcpy(&thisValue, thisData, sizeof(float));
            memcpy(&otherValue, otherData, sizeof(float));

            if (!sortAscending)
                return thisValue > otherValue;
            else
                return thisValue < otherValue;
            break;
        }

        case DBVAR_STRING: {
            char *thisString  = (char *)thisData;
            char *otherString = (char *)otherData;

            int32 result = strcmp(thisString, otherString);
            if (sortAscending)
                return result > 0 ? true : false;
            else
                return result < 0 ? true : false;
            break;
        }
        default: break;
    }
    return false;
}

// =======================
// USER DB
// =======================
void RSDK::SKU::UserDB::Init(va_list list)
{
    int32 cnt = 0;
    while (true) {
        int32 type = va_arg(list, int32);
        if (!type)
            break;

        columnTypes[cnt] = type;
        memset(columnNames[cnt], 0, 0x10);
        sprintf_s(columnNames[cnt], (int32)sizeof(columnNames[cnt]), "%s", va_arg(list, const char *));
        GenerateHashCRC(&columnUUIDs[cnt], columnNames[cnt]);
        ++cnt;
    }

    columnCount = cnt;
    rowCount    = 0;
    memset(rows, 0, sizeof(rows));
    Refresh();
    active = true;
    valid  = true;
}
bool32 RSDK::SKU::UserDB::Load(uint8 *buffer)
{
    uint32 signature = *(uint32 *)buffer;
    if (signature != RETRO_USERDB_SIGNATURE)
        return false;

    buffer += sizeof(int32);
    buffer += sizeof(int32); // used size

    rowCount = *(uint16 *)buffer;
    buffer += sizeof(uint16);

    columnCount = *buffer;
    buffer++;

    for (int32 c = 0; c < columnCount; ++c) {
        columnTypes[c] = *buffer;
        buffer++;

        sprintf_s(columnNames[c], (int32)sizeof(columnNames[c]), "%s", (char *)buffer);
        buffer += 0x10;

        GenerateHashCRC(&columnUUIDs[c], columnNames[c]);
    }

    for (int32 r = 0; r < rowCount; ++r) {
        rows[r].uuid = *(uint32 *)buffer;
        buffer += sizeof(uint32);

        memcpy(&rows[r].createTime, buffer, sizeof(tm));
        buffer += sizeof(tm);
        memcpy(&rows[r].changeTime, buffer, sizeof(tm));
        buffer += sizeof(tm);

        for (int32 c = 0; c < columnCount; ++c) {
            rows[r].values[c].size = *buffer;
            buffer++;

            memcpy(&rows[r].values[c].data, buffer, rows[r].values[c].size);
            buffer += rows[r].values[c].size;
        }
    }

    active = true;
    Refresh();
    return true;
}
void RSDK::SKU::UserDB::Save(int32 totalSize, uint8 *buffer)
{
    int32 size = 0;
    if (sizeof(uint32) < totalSize) {
        size              = sizeof(uint32);
        *(uint32 *)buffer = RETRO_USERDB_SIGNATURE;
        buffer += sizeof(uint32);
    }

    if (size + sizeof(uint32) < totalSize) {
        size += sizeof(uint32);
        *(uint32 *)buffer = (int32)GetSize(); // used size
        buffer += sizeof(uint32);
    }

    if (size + sizeof(uint16) < totalSize) {
        size += sizeof(uint16);
        *(uint16 *)buffer = rowCount;
        buffer += sizeof(uint16);
    }

    if (size + sizeof(uint8) < totalSize) {
        ++size;
        *buffer++ = columnCount;
    }

    for (int32 c = 0; c < columnCount; ++c) {
        if (size + sizeof(uint8) < totalSize) {
            ++size;
            *buffer++ = (uint8)columnTypes[c];
        }
        if (size + 0x10 * sizeof(uint8) < totalSize) {
            memset(buffer, 0, 0x10 * sizeof(uint8));
            sprintf_s((char *)buffer, (int32)sizeof(columnNames[c]), "%s", columnNames[c]);
            size += 0x10;
            buffer += 0x10;
        }
    }

    for (int32 r = 0; r < rowCount; ++r) {
        if (size + sizeof(uint32) < totalSize) {
            size += sizeof(uint32);
            *(uint32 *)buffer = rows[r].uuid;
            buffer += sizeof(uint32);
        }
        if (size + sizeof(tm) < totalSize) {
            memcpy(buffer, &rows[r].createTime, sizeof(tm));
            size += sizeof(tm);
            buffer += sizeof(tm);
        }
        if (size + sizeof(tm) < totalSize) {
            memcpy(buffer, &rows[r].changeTime, sizeof(tm));
            size += sizeof(tm);
            buffer += sizeof(tm);
        }

        for (int32 c = 0; c < columnCount; ++c) {
            if (size + sizeof(uint8) < totalSize) {
                ++size;
                *buffer++ = (uint8)rows[r].values[c].size;
            }
            if (rows[r].values[c].size + size < totalSize) {
                memcpy(buffer, rows[r].values[c].data, rows[r].values[c].size);
                size += rows[r].values[c].size;
                buffer += rows[r].values[c].size;
            }
        }
    }

    if (size < totalSize)
        memset(buffer, 0, totalSize - size);
}
void RSDK::SKU::UserDB::Clear()
{
    sortedRowList.Clear();
    loaded      = false;
    active      = false;
    valid       = false;
    uuid        = 0;
    rowCount    = 0;
    columnCount = 0;
    memset(columnTypes, 0, sizeof(columnTypes));
    memset(columnNames, 0, sizeof(columnNames));
    memset(columnUUIDs, 0, sizeof(columnUUIDs));
    memset(rows, 0, sizeof(rows));
    rowsChanged = true;
}

int32 RSDK::SKU::UserDB::GetColumnID(const char *name)
{
    uint32 uuid = 0;
    GenerateHashCRC(&uuid, (char *)name);

    int32 id = -1;
    for (int32 i = 0; i < columnCount; ++i) {
        if (uuid == columnUUIDs[i]) {
            id = i;
            break;
        }
    }
    return id;
}

int32 RSDK::SKU::UserDB::AddRow()
{
    if (rowCount >= RETRO_USERDB_ROW_MAX)
        return -1;

    UserDBRow *row = &rows[rowCount];
    row->uuid      = CreateRowUUID();

    time_t timeInfo;
    time(&timeInfo);
    tm *tmA = localtime(&timeInfo);
    tm *tmB = localtime(&timeInfo);

    memcpy(&row->createTime, tmA, sizeof(tm));
    memcpy(&row->changeTime, tmB, sizeof(tm));

    memset(row->values, 0, sizeof(UserDBValue) * RETRO_USERDB_COL_MAX);
    ++rowCount;
    valid = true;

    Refresh();
    rowsChanged = true;
    return rowCount - 1;
}
bool32 RSDK::SKU::UserDB::RemoveRow(uint32 row)
{
    if (row < rowCount) {
        UserDBRow *entry = &rows[row];
        memset(entry, 0, sizeof(UserDBRow));

        int32 id = (int32)(rowCount - row - 1);
        memcpy(entry, &entry[1], id * sizeof(UserDBRow));
        memset(&entry[id + 1], 0, sizeof(UserDBRow));

        --rowCount;
        valid = true;

        Refresh();
        rowsChanged = true;
    }
    return true;
}
bool32 RSDK::SKU::UserDB::RemoveAllRows()
{
    rowCount = 0;
    memset(rows, 0, sizeof(UserDBRow) * RETRO_USERDB_ROW_MAX);
    Refresh();
    return true;
}
uint16 RSDK::SKU::UserDB::GetRowByID(uint32 uuid)
{
    if (!rowCount)
        return -1;

    for (int32 i = 0; i < rowCount; ++i) {
        if (uuid == rows[i].uuid) {
            return i;
        }
    }
    return -1;
}

void RSDK::SKU::UserDB::FilterValues(UserDBValue *value, int32 column)
{
    for (int32 i = sortedRowList.Count() - 1; i >= 0; --i) {
        if (!value->CheckMatch(sortedRowIDs[i], column)) {
            sortedRowList.Remove(i);
        }
    }
}
void RSDK::SKU::UserDB::AddSortFilter(const char *name, void *value)
{
    int32 id = GetColumnID(name);

    if (id >= 0) {
        if (sortedRowCount) {
            UserDBValue dbValue;
            // this is very hacky
            dbValue.parent = (UserDBRow *)this;
            dbValue.Set(columnTypes[id], value);
            FilterValues(&dbValue, id);
            SetupRowSortIDs();
        }
    }
}

void RSDK::SKU::UserDB::SetupRowSortIDs()
{
    sortedRowCount = 0;
    memset(sortedRowIDs, 0, sizeof(sortedRowIDs));

    for (int32 i = 0; i < sortedRowList.Count(); ++i) {
        sortedRowIDs[i] = *sortedRowList.At(i);
        ++sortedRowCount;
    }
}
void RSDK::SKU::UserDB::RefreshSortList()
{
    sortedRowList.Clear();

    for (int32 i = 0; i < rowCount; ++i) {
        int32 *row = sortedRowList.Append();
        if (row)
            *row = i;
    }

    rowsChanged = false;

    SetupRowSortIDs();
}
void RSDK::SKU::UserDB::SortRows(int32 type, char *name, bool32 sortAscending)
{
    if (!rowsChanged && sortedRowCount) {
        if (type || name) { // sort by value
            int32 col = GetColumnID(name);
            if (col >= 0) {
                for (int32 i = 0; i < sortedRowList.Count(); i++) {
                    for (int32 j = i + 1; j < sortedRowList.Count(); j++) {
                        int32 *id1 = sortedRowList.At(i);
                        int32 *id2 = sortedRowList.At(j);

                        if (rows[*id1].Compare(&rows[*id2], type, name, sortAscending)) {
                            int32 temp = *id1;
                            *id1       = *id2;
                            *id2       = temp;
                        }
                    }
                }

                SetupRowSortIDs();
            }
        }
        else { // sort by date
            for (int32 i = 0; i < sortedRowList.Count(); i++) {
                for (int32 j = i + 1; j < sortedRowList.Count(); j++) {
                    int32 *id1 = sortedRowList.At(i);
                    int32 *id2 = sortedRowList.At(j);

                    double d = difftime(mktime(&rows[*id1].createTime), mktime(&rows[*id2].createTime));

                    bool32 swap = false;
                    if (sortAscending)
                        swap = (d < 0) - (d > 0);
                    else
                        swap = (d > 0) - (d < 0);

                    if (swap) {
                        int32 temp = *id1;
                        *id1       = *id2;
                        *id2       = temp;
                    }
                }
            }

            SetupRowSortIDs();
        }
    }
}

uint32 RSDK::SKU::UserDB::CreateRowUUID()
{
    bool32 flag = true;
    uint32 uuid = 0;

    while (flag) {
        uint8 bytes[4];
        bytes[0] = rand();
        bytes[1] = rand();
        bytes[2] = rand();
        bytes[3] = rand();
        uuid     = (bytes[0]) | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);

        if (uuid < 0x10000000)
            uuid |= 0x10000000;

        flag = false;
        for (int32 e = 0; e < rowCount; ++e) {
            if (uuid == rows[e].uuid) {
                flag = true;
            }
        }
    }
    return uuid;
}
void RSDK::SKU::UserDB::Refresh()
{
    parent = this;
    for (int32 r = 0; r < RETRO_USERDB_ROW_MAX; ++r) {
        rows[r].parent = this;
        for (int32 c = 0; c < columnCount; ++c) {
            rows[r].values[c].parent = &rows[r];
        }
    }
}
size_t RSDK::SKU::UserDB::GetSize()
{
    int32 colSize = 1;
    if (columnCount)
        colSize = (0x10 * columnCount) + columnCount + 1;

    size_t size = colSize + 10;

    for (int32 r = 0; r < RETRO_USERDB_ROW_MAX; ++r) {
        rows[r].parent = this;
        int32 rowSize  = (sizeof(tm) * 2) + sizeof(uint32);
        for (int32 c = 0; c < columnCount; ++c) {
            rowSize += sizeof(uint8) + rows[r].values[c].size;
        }
        size += rowSize;
    }

    return size;
}

// =======================
// USER DB STORAGE
// =======================

RSDK::SKU::UserDBStorage::UserDBStorage()
{
    unknown = 0;

    this->loadCallback[0] = UserDBStorage_LoadCB1;
    this->loadCallback[1] = UserDBStorage_LoadCB2;
    this->loadCallback[2] = UserDBStorage_LoadCB3;
    this->loadCallback[3] = UserDBStorage_LoadCB4;
    this->loadCallback[4] = UserDBStorage_LoadCB5;
    this->loadCallback[5] = UserDBStorage_LoadCB6;
    this->loadCallback[6] = UserDBStorage_LoadCB7;
    this->loadCallback[7] = UserDBStorage_LoadCB8;

    this->saveCallback[0] = UserDBStorage_SaveCB1;
    this->saveCallback[1] = UserDBStorage_SaveCB2;
    this->saveCallback[2] = UserDBStorage_SaveCB3;
    this->saveCallback[3] = UserDBStorage_SaveCB4;
    this->saveCallback[4] = UserDBStorage_SaveCB5;
    this->saveCallback[5] = UserDBStorage_SaveCB6;
    this->saveCallback[6] = UserDBStorage_SaveCB7;
    this->saveCallback[7] = UserDBStorage_SaveCB8;
}

// UserDB Management
uint16 RSDK::SKU::UserDBStorage::InitUserDB(const char *name, va_list list)
{
    int32 tableID = -1;
    uint32 uuid   = 0;
    GenerateHashCRC(&uuid, (char *)name);

    for (int32 i = 0; i < RETRO_USERDB_MAX; ++i) {
        if (!userDBStorage->userDB[i].loaded) {
            tableID = i;
            break;
        }
    }

    if (tableID == -1)
        return -1;

    UserDB *userDB = &userDBStorage->userDB[tableID];

    userDB->loaded = true;
    userDB->name   = name;
    GenerateHashCRC(&userDB->uuid, (char *)name);
    userDB->Init(list);
    userDB->parent->RefreshSortList();

    return tableID;
}
uint16 RSDK::SKU::UserDBStorage::LoadUserDB(const char *filename, void (*callback)(int32 status))
{
    int32 tableID = -1;
    uint32 uuid   = 0;
    GenerateHashCRC(&uuid, (char *)filename);
    for (int32 i = 0; i < RETRO_USERDB_MAX; ++i) {
        if (uuid == userDBStorage->userDB[i].uuid && userDBStorage->userDB[i].loaded)
            return i;
    }

    for (int32 i = 0; i < RETRO_USERDB_MAX; ++i) {
        if (!userDBStorage->userDB[i].loaded) {
            tableID = i;
            break;
        }
    }

    if (tableID == -1) {
        if (callback)
            callback(STATUS_ERROR);
        return -1;
    }

    AllocateStorage(sizeof(UserDB), (void **)&userDBStorage->readBuffer[tableID], DATASET_TMP, true);
    userDBStorage->dbLoadCB[tableID]  = UserDBStorage::LoadCB;
    userDBStorage->callbacks[tableID] = callback;
    TryLoadUserFile(filename, userDBStorage->readBuffer[tableID], sizeof(UserDB), userDBStorage->loadCallback[tableID]);

    UserDB *userDB = &userDBStorage->userDB[tableID];
    userDB->loaded = true;
    userDB->name   = filename;
    userDB->uuid   = uuid;

    return tableID;
}
bool32 RSDK::SKU::UserDBStorage::SaveUserDB(uint16 tableID, void (*callback)(int32 status))
{
    UserDB *userDB = &userDBStorage->userDB[tableID];

    bool32 success = false;
    if (userDB->active) {
        int32 totalSize = (int32)userDB->GetSize();
        AllocateStorage(totalSize, (void **)&userDBStorage->writeBuffer[tableID], DATASET_TMP, true);
        userDB->Save(totalSize, (uint8 *)userDBStorage->writeBuffer[tableID]);
        userDBStorage->dbSaveCB[tableID]  = UserDBStorage::SaveCB;
        userDBStorage->callbacks[tableID] = callback;
        success = TrySaveUserFile(userDB->name, userDBStorage->writeBuffer[tableID], totalSize, userDBStorage->saveCallback[tableID], true);
    }
    else {
        if (callback)
            callback(STATUS_ERROR);
    }

    return success;
}
void RSDK::SKU::UserDBStorage::ClearUserDB(uint16 tableID)
{
    if (tableID == (uint16)-1)
        return;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (userDB->loaded) {
        userDB->Clear();
    }
}
void RSDK::SKU::UserDBStorage::ClearAllUserDBs()
{
    for (int32 i = 0; i < RETRO_USERDB_MAX; ++i) ClearUserDB(i);
}

bool32 RSDK::SKU::UserDBStorage::LoadCB(uint16 tableID, int32 status)
{
    bool32 succeeded = false;
    if (status == STATUS_OK) {
        UserDB *userDB = &userDBStorage->userDB[tableID];

        succeeded = userDB->Load((uint8 *)userDBStorage->readBuffer[tableID]);
        if (succeeded) {
            userDB->parent->RefreshSortList();
        }
    }
    else {
        RSDK::SKU::ClearUserDB(tableID);
    }
    RemoveStorageEntry((void **)&userDBStorage->readBuffer[tableID]);

    if (userDBStorage->callbacks[tableID]) {
        userDBStorage->callbacks[tableID](succeeded ? status : STATUS_ERROR);
        userDBStorage->callbacks[tableID] = NULL;
    }

    return succeeded;
}
bool32 RSDK::SKU::UserDBStorage::SaveCB(uint16 tableID, int32 status)
{
    RemoveStorageEntry((void **)&userDBStorage->writeBuffer[tableID]);
    if (status != STATUS_OK)
        userDBStorage->userDB[tableID].valid = false;

    if (userDBStorage->callbacks[tableID]) {
        userDBStorage->callbacks[tableID](status);
        userDBStorage->callbacks[tableID] = NULL;
        return true;
    }
    return false;
}

// =======================
// USER DB API
// =======================

// UserDB Row Sorting
uint16 RSDK::SKU::SetupUserDBRowSorting(uint16 tableID)
{
    if (tableID == (uint16)-1)
        return 0;
    userDBStorage->userDB[tableID].parent->RefreshSortList();

    return userDBStorage->userDB[tableID].sortedRowCount;
}
int32 RSDK::SKU::AddUserDBRowSortFilter(uint16 tableID, int32 type, const char *name, void *value)
{
    if (tableID == (uint16)-1)
        return 0;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;

    userDB->parent->AddSortFilter(name, value);
    return userDB->sortedRowCount;
}
int32 RSDK::SKU::SortUserDBRows(uint16 tableID, int32 type, const char *name, bool32 sortAscending)
{
    if (tableID == (uint16)-1)
        return 0;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;

    userDB->parent->SortRows(type, (char *)name, sortAscending);
    return userDB->sortedRowCount;
}
int32 RSDK::SKU::GetSortedUserDBRowCount(uint16 tableID)
{
    if (tableID == (uint16)-1)
        return 0;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;

    return userDB->sortedRowCount;
}
int32 RSDK::SKU::GetSortedUserDBRowID(uint16 tableID, uint16 sortedRowID)
{
    if (tableID == (uint16)-1)
        return -1;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active || userDB->rowsChanged || sortedRowID >= userDB->sortedRowCount)
        return -1;

    return userDB->sortedRowIDs[sortedRowID];
}

// UserDB Values
bool32 RSDK::SKU::GetUserDBValue(uint16 tableID, uint32 rowID, int32 type, char *name, void *value)
{
    if (tableID == (uint16)-1 || rowID == (uint16)-1 || !userDBStorage->userDB[tableID].active)
        return false;
    return userDBStorage->userDB[tableID].rows[rowID].GetValue(type, name, value);
}
bool32 RSDK::SKU::SetUserDBValue(uint16 tableID, uint32 rowID, int32 type, char *name, void *value)
{
    if (tableID == (uint16)-1 || rowID == (uint16)-1 || !userDBStorage->userDB[tableID].active)
        return false;

    return userDBStorage->userDB[tableID].rows[rowID].AddValue(type, name, value);
}

// UserDB Misc
int32 RSDK::SKU::GetUserDBRowsChanged(uint16 tableID) { return userDBStorage->userDB[tableID].rowsChanged; }
void RSDK::SKU::GetUserDBRowCreationTime(uint16 tableID, int32 rowID, char *buf, size_t size, char *format)
{
    if (tableID != (uint16)-1 && rowID != (uint16)-1) {
        UserDB *userDB = &userDBStorage->userDB[tableID];
        if (userDB->active)
            strftime(buf, size, format, &userDB->rows[rowID].createTime);
    }
}

// =======================
// USER DB CALLBACKS
// =======================

void RSDK::SKU::UserDBStorage_LoadCB1(int32 status)
{
    if (userDBStorage->dbLoadCB[0])
        userDBStorage->dbLoadCB[0](0, status);
}
void RSDK::SKU::UserDBStorage_LoadCB2(int32 status)
{
    if (userDBStorage->dbLoadCB[1])
        userDBStorage->dbLoadCB[1](1, status);
}
void RSDK::SKU::UserDBStorage_LoadCB3(int32 status)
{
    if (userDBStorage->dbLoadCB[2])
        userDBStorage->dbLoadCB[2](2, status);
}
void RSDK::SKU::UserDBStorage_LoadCB4(int32 status)
{
    if (userDBStorage->dbLoadCB[3])
        userDBStorage->dbLoadCB[3](3, status);
}
void RSDK::SKU::UserDBStorage_LoadCB5(int32 status)
{
    if (userDBStorage->dbLoadCB[4])
        userDBStorage->dbLoadCB[4](4, status);
}
void RSDK::SKU::UserDBStorage_LoadCB6(int32 status)
{
    if (userDBStorage->dbLoadCB[5])
        userDBStorage->dbLoadCB[5](5, status);
}
void RSDK::SKU::UserDBStorage_LoadCB7(int32 status)
{
    if (userDBStorage->dbLoadCB[6])
        userDBStorage->dbLoadCB[6](6, status);
}
void RSDK::SKU::UserDBStorage_LoadCB8(int32 status)
{
    if (userDBStorage->dbLoadCB[7])
        userDBStorage->dbLoadCB[7](7, status);
}

void RSDK::SKU::UserDBStorage_SaveCB1(int32 status)
{
    if (userDBStorage->dbSaveCB[0])
        userDBStorage->dbSaveCB[0](0, status);
}
void RSDK::SKU::UserDBStorage_SaveCB2(int32 status)
{
    if (userDBStorage->dbSaveCB[1])
        userDBStorage->dbSaveCB[1](1, status);
}
void RSDK::SKU::UserDBStorage_SaveCB3(int32 status)
{
    if (userDBStorage->dbSaveCB[2])
        userDBStorage->dbSaveCB[2](2, status);
}
void RSDK::SKU::UserDBStorage_SaveCB4(int32 status)
{
    if (userDBStorage->dbSaveCB[3])
        userDBStorage->dbSaveCB[3](3, status);
}
void RSDK::SKU::UserDBStorage_SaveCB5(int32 status)
{
    if (userDBStorage->dbSaveCB[4])
        userDBStorage->dbSaveCB[4](4, status);
}
void RSDK::SKU::UserDBStorage_SaveCB6(int32 status)
{
    if (userDBStorage->dbSaveCB[5])
        userDBStorage->dbSaveCB[5](5, status);
}
void RSDK::SKU::UserDBStorage_SaveCB7(int32 status)
{
    if (userDBStorage->dbSaveCB[6])
        userDBStorage->dbSaveCB[6](6, status);
}
void RSDK::SKU::UserDBStorage_SaveCB8(int32 status)
{
    if (userDBStorage->dbSaveCB[7])
        userDBStorage->dbSaveCB[7](7, status);
}

#endif

void (*RSDK::SKU::preLoadSaveFileCB)();
void (*RSDK::SKU::postLoadSaveFileCB)();
char RSDK::SKU::userFileDir[0x100];

bool32 RSDK::SKU::LoadUserFile(const char *filename, void *buffer, uint32 bufSize)
{
    if (preLoadSaveFileCB)
        preLoadSaveFileCB();

    char fullFilePath[0x400];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "%s%s", userFileDir, filename);
    PrintLog(PRINT_NORMAL, "Attempting to load user file: %s", fullFilePath);

    FileIO *file = fOpen(fullFilePath, "rb");
    if (file) {
        fSeek(file, 0, SEEK_END);
        int32 fSize = (int32)fTell(file);
        fSeek(file, 0, SEEK_SET);
        int32 size = bufSize;
        if (bufSize > fSize)
            size = fSize;
        fRead(buffer, 1, size, file);
        fClose(file);

        if (postLoadSaveFileCB)
            postLoadSaveFileCB();

        return true;
    }
    else {
        if (postLoadSaveFileCB)
            postLoadSaveFileCB();

        PrintLog(PRINT_NORMAL, "Nope!");
    }

    return false;
}
bool32 RSDK::SKU::SaveUserFile(const char *filename, void *buffer, uint32 bufSize)
{
    if (preLoadSaveFileCB)
        preLoadSaveFileCB();

    char fullFilePath[0x400];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "%s%s", userFileDir, filename);
    PrintLog(PRINT_NORMAL, "Attempting to save user file: %s", fullFilePath);

    FileIO *file = fOpen(fullFilePath, "wb");
    if (file) {
        fWrite(buffer, 1, bufSize, file);
        fClose(file);

        if (postLoadSaveFileCB)
            postLoadSaveFileCB();

        return true;
    }
    else {
        if (postLoadSaveFileCB)
            postLoadSaveFileCB();

        PrintLog(PRINT_NORMAL, "Nope!");
    }
    return false;
}
bool32 RSDK::SKU::DeleteUserFile(const char *filename)
{
    if (preLoadSaveFileCB)
        preLoadSaveFileCB();

    char fullFilePath[0x400];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "%s%s", userFileDir, filename);
    PrintLog(PRINT_NORMAL, "Attempting to delete user file: %s", fullFilePath);
    int32 status = remove(fullFilePath);

    if (postLoadSaveFileCB)
        postLoadSaveFileCB();

    return status == 0;
}

#if !RETRO_REV02
bool32 RSDK::SKU::TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status))
{
    bool32 success = false;

    success = LoadUserFile(filename, buffer, size);

    if (callback)
        callback(success ? STATUS_OK : STATUS_ERROR);

    return success;
}
bool32 RSDK::SKU::TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status))
{
    bool32 success = false;

    success = SaveUserFile(filename, buffer, size);

    if (callback)
        callback(success ? STATUS_OK : STATUS_ERROR);

    return success;
}
#endif

void RSDK::SKU::InitUserDirectory()
{
#if RETRO_PLATFORM == RETRO_OSX

    char buffer[0x100];
    sprintf_s(buffer, (int32)sizeof(buffer), "%s/RSDKv5/", getResourcesPath());
    SKU::SetUserFileCallbacks(buffer, NULL, NULL);

#elif RETRO_PLATFORM == RETRO_ANDROID

    char buffer[0x200];

    JNIEnv *env      = (JNIEnv *)SDL_AndroidGetJNIEnv();
    jobject activity = (jobject)SDL_AndroidGetActivity();
    jclass cls(env->GetObjectClass(activity));
    jmethodID method = env->GetMethodID(cls, "getBasePath", "()Ljava/lang/String;");
    auto ret         = env->CallObjectMethod(activity, method);

    strcpy(buffer, env->GetStringUTFChars((jstring)ret, NULL));

    SKU::SetUserFileCallbacks(buffer, NULL, NULL);

    env->DeleteLocalRef(activity);
    env->DeleteLocalRef(cls);

#elif RETRO_PLATFORM == RETRO_LINUX

    SKU::SetUserFileCallbacks("./", NULL, NULL);

#else

    SKU::SetUserFileCallbacks("", NULL, NULL);

#endif
}