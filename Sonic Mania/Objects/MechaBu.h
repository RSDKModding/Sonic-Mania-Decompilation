#ifndef OBJ_MECHABU_H
#define OBJ_MECHABU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[10]; //= { -1376256, -983040, -1507328, -917504, -1638400, -655360, -1703936, -327680, -1703936, -262144 };
    colour value2;
    colour value3;
    ushort value4;
    ushort value5;
    ushort value6;
} ObjectMechaBu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMechaBu;

// Object Struct
extern ObjectMechaBu *MechaBu;

// Standard Entity Events
void MechaBu_Update();
void MechaBu_LateUpdate();
void MechaBu_StaticUpdate();
void MechaBu_Draw();
void MechaBu_Create(void* data);
void MechaBu_StageLoad();
void MechaBu_EditorDraw();
void MechaBu_EditorLoad();
void MechaBu_Serialize();

// Extra Entity Functions


#endif //!OBJ_MECHABU_H
