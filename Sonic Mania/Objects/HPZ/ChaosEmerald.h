#ifndef OBJ_CHAOSEMERALD_H
#define OBJ_CHAOSEMERALD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort value1;
    byte value2;
    int value3[36]; //= { 10485760, 11540512, 12595264, 13650016, 14704768, 4210888, 5789896, 14704864, 7368904, 8947912, 10526920, 12601544, 10498224, 8394904, 6291584, 8960224, 32992, 2658528, 5810400, 12112096, 2129920, 4233216, 6336512, 8439808, 10543104, 14737568, 13682808, 12628048, 11573288, 10518528, 8421504, 9474192, 10526880, 12632256, 11579568, 15263976 };
    int value4;
} ObjectChaosEmerald;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityChaosEmerald;

// Object Struct
extern ObjectChaosEmerald *ChaosEmerald;

// Standard Entity Events
void ChaosEmerald_Update(void);
void ChaosEmerald_LateUpdate(void);
void ChaosEmerald_StaticUpdate(void);
void ChaosEmerald_Draw(void);
void ChaosEmerald_Create(void* data);
void ChaosEmerald_StageLoad(void);
void ChaosEmerald_EditorDraw(void);
void ChaosEmerald_EditorLoad(void);
void ChaosEmerald_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CHAOSEMERALD_H
