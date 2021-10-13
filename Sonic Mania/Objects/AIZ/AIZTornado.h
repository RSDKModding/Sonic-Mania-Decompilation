#ifndef OBJ_AIZTORNADO_H
#define OBJ_AIZTORNADO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 array[5], { -0x20000, -0x10000, 0x00000, 0x10000, 0x20000 });
    uint16 aniFrames;
} ObjectAIZTornado;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 dword5C;
    int32 dword60;
    int32 dword64;
    int32 dword68;
    int32 field_6C;
    int32 field_70;
    Vector2 prevPos;
    int32 dword7C;
    int32 dword80;
    int32 field_84;
    int32 field_88;
    Vector2 moveVelocity;
    int32 field_94;
    int32 dword98;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityAIZTornado;

// Object Struct
extern ObjectAIZTornado *AIZTornado;

// Standard Entity Events
void AIZTornado_Update(void);
void AIZTornado_LateUpdate(void);
void AIZTornado_StaticUpdate(void);
void AIZTornado_Draw(void);
void AIZTornado_Create(void* data);
void AIZTornado_StageLoad(void);
void AIZTornado_EditorDraw(void);
void AIZTornado_EditorLoad(void);
void AIZTornado_Serialize(void);

// Extra Entity Functions
void AIZTornado_Unknown1(void);
void AIZTornado_Unknown2(void);
void AIZTornado_Unknown3(void);


#endif //!OBJ_AIZTORNADO_H
