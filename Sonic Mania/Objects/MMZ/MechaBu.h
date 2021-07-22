#ifndef OBJ_MECHABU_H
#define OBJ_MECHABU_H

#include "SonicMania.h"

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
void MechaBu_Update(void);
void MechaBu_LateUpdate(void);
void MechaBu_StaticUpdate(void);
void MechaBu_Draw(void);
void MechaBu_Create(void* data);
void MechaBu_StageLoad(void);
void MechaBu_EditorDraw(void);
void MechaBu_EditorLoad(void);
void MechaBu_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MECHABU_H
