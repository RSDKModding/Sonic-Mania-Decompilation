#ifndef OBJ_AIZTORNADO_H
#define OBJ_AIZTORNADO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int array[5]; // = { -131072, -65536, 0, 65536, 131072 };
    ushort aniFrames;
} ObjectAIZTornado;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int dword5C;
    int dword60;
    int dword64;
    int dword68;
    int field_6C;
    int field_70;
    Vector2 prevPos;
    int dword7C;
    int dword80;
    int field_84;
    int field_88;
    Vector2 moveVelocity;
    int field_94;
    int dword98;
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
