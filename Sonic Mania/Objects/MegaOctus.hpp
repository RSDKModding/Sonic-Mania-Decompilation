#ifndef OBJ_MEGAOCTUS_H
#define OBJ_MEGAOCTUS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMegaOctus : Object {
    bool32 value1;
    unknown value2;
    unknown value3;
    unknown value4;
    unknown value5;
    unknown value6;
    int value7;
    int value8;
    byte value9;
    int value10;
    int value11[11]; //= { -2097152, -1572864, -1048576, -786432, -524288, 0, 524288, 786432, 1048576, 1572864, 2097152 };
    int value12[4]; //= { -7864320, -8388608, -7340032, -5308416 };
    int value13;
    byte value14;
    ushort value15;
    ushort value16;
    ushort value17;
    colour value18;
    ushort value19;
    ushort value20;
    ushort value21;
    ushort value22;
    ushort value23;
    ushort value24;
    ushort value25;
};

// Entity Class
struct EntityMegaOctus : Entity {

};

// Object Struct
extern ObjectMegaOctus *MegaOctus;

// Standard Entity Events
void MegaOctus_Update();
void MegaOctus_LateUpdate();
void MegaOctus_StaticUpdate();
void MegaOctus_Draw();
void MegaOctus_Create(void* data);
void MegaOctus_StageLoad();
void MegaOctus_EditorDraw();
void MegaOctus_EditorLoad();
void MegaOctus_Serialize();

// Extra Entity Functions


#endif //!OBJ_MEGAOCTUS_H
