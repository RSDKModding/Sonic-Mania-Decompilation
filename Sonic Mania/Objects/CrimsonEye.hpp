#ifndef OBJ_CRIMSONEYE_H
#define OBJ_CRIMSONEYE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCrimsonEye : Object {
    int value1[97]; //= { 24, 0, 0, -163840, -163840, 1, 0, -81920, -163840, 2, 0, 81920, -155648, 3, 0, 163840, -155648, 4, 0, -155648, -147456, 5, 0, 155648, -147456, 6, 0, -155648, -139264, 7, 0, 155648, -139264, 8, 0, -147456, -131072, 9, 0, -73728, -131072, 10, 0, 73728, -131072, 11, 0, 147456, -131072, 12, 0, -139264, -122880, 13, 0, -69632, -122880, 14, 0, 69632, -122880, 15, 0, 139264, -122880, 16, 0, -131072, -114688, 17, 0, -65536, -114688, 18, 0, 65536, -114688, 19, 0, 131072, -114688, 20, 0, -106496, -106496, 21, 0, -53248, -106496, 22, 0, 53248, -106496, 23, 0, 106496, -106496 };
    byte value2;
    byte value3;
    byte value4;
    byte value5;
    int value6;
    int value7;
    TextInfo value8;
    TextInfo value9;
    int value10[8]; //= { 1, 5, 2, 6, 3, 7, 4, 8 };
    int value11;
    int value12;
    int value13;
    int value14;
    int value15;
    int value16;
    byte value17;
    int value18;
    ushort value19;
    colour value20;
    colour value21;
    colour value22;
    colour value23;
    colour value24;
    colour value25;
    colour value26;
    ushort value27;
    ushort value28;
    ushort value29;
    ushort value30;
    ushort value31;
    ushort value32;
    ushort value33;
    ushort value34;
    ushort value35;
    ushort value36;
    bool32 value37;
    bool32 value38;
};

// Entity Class
struct EntityCrimsonEye : Entity {

};

// Object Struct
extern ObjectCrimsonEye *CrimsonEye;

// Standard Entity Events
void CrimsonEye_Update();
void CrimsonEye_LateUpdate();
void CrimsonEye_StaticUpdate();
void CrimsonEye_Draw();
void CrimsonEye_Create(void* data);
void CrimsonEye_StageLoad();
void CrimsonEye_EditorDraw();
void CrimsonEye_EditorLoad();
void CrimsonEye_Serialize();

// Extra Entity Functions


#endif //!OBJ_CRIMSONEYE_H
