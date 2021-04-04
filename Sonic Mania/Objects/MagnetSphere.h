#ifndef OBJ_MAGNETSPHERE_H
#define OBJ_MAGNETSPHERE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    colour value1;
    byte value2;
    byte value3;
    int value4[16]; //= { 32, 32, 32, 48, 64, 80, 96, 96, 96, 160, 160, 176, 192, 208, 224, 224 };
    ushort value5;
    ushort value6;
    ushort value7;
} ObjectMagnetSphere;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMagnetSphere;

// Object Struct
extern ObjectMagnetSphere *MagnetSphere;

// Standard Entity Events
void MagnetSphere_Update(void);
void MagnetSphere_LateUpdate(void);
void MagnetSphere_StaticUpdate(void);
void MagnetSphere_Draw(void);
void MagnetSphere_Create(void* data);
void MagnetSphere_StageLoad(void);
void MagnetSphere_EditorDraw(void);
void MagnetSphere_EditorLoad(void);
void MagnetSphere_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MAGNETSPHERE_H
