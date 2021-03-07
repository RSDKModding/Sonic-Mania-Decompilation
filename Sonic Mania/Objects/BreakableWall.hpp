#ifndef OBJ_BREAKABLEWALL_H
#define OBJ_BREAKABLEWALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBreakableWall : Object {
    unknown value1;
    ushort value2;
    int value3[16]; //= { -524288, -1572864, 524288, -1572864, -524288, -524288, 524288, -524288, -524288, 524288, 524288, 524288, -524288, 1572864, 524288, 1572864 };
    int value4[16]; //= { -393216, -393216, -262144, -327680, -1179648, -131072, -393216, -65536, -524288, 131072, -393216, 65536, -393216, 393216, -262144, 327680 };
    int value5[16]; //= { 262144, -327680, 393216, -393216, 393216, -65536, 524288, -131072, 393216, 65536, 524288, 131072, 262144, 327680, 393216, 393216 };
    int value6[8]; //= { -524288, -524288, 524288, -524288, -524288, 524288, 524288, 524288 };
    int value7[8]; //= { -131072, -131072, 131072, -131072, -65536, -65536, 65536, -65536 };
    ushort value8;
    ushort value9;
};

// Entity Class
struct EntityBreakableWall : Entity {

};

// Object Struct
extern ObjectBreakableWall *BreakableWall;

// Standard Entity Events
void BreakableWall_Update();
void BreakableWall_LateUpdate();
void BreakableWall_StaticUpdate();
void BreakableWall_Draw();
void BreakableWall_Create(void* data);
void BreakableWall_StageLoad();
void BreakableWall_EditorDraw();
void BreakableWall_EditorLoad();
void BreakableWall_Serialize();

// Extra Entity Functions


#endif //!OBJ_BREAKABLEWALL_H
