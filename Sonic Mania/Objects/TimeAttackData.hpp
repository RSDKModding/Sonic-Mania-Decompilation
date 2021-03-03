#ifndef OBJ_TIMEATTACKDATA_H
#define OBJ_TIMEATTACKDATA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTimeAttackData : Object {
    byte zoneID;
    byte act;
    byte characterID;
    byte field_7;
    bool32 encore;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int dword1C;
    Entity *loadEntityPtr;
    void (*loadCallback)(bool32);
    int saveEntityPtr;
    void (*saveCallback)(bool32);
};

// Object Entity
extern ObjectTimeAttackData *TimeAttackData;


#endif //!OBJ_TIMEATTACKDATA_H
