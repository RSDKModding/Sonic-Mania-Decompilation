#ifndef OBJ_TORNADO_H
#define OBJ_TORNADO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int offsets[5], { -0x20000, -0x10000, 0, 0x10000, 0x20000 });
    uint16 aniFrames;
    uint16 knuxFrames;
    uint16 sfxExplosion;
    uint16 sfxImpact;
} ObjectTornado;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    int timer;
    int field_64;
    int field_68;
    int field_6C;
    bool32 field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    Vector2 field_84;
    Vector2 field_8C;
    int field_94;
    int field_98;
    int field_9C;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animatorKnux;
} EntityTornado;

// Object Struct
extern ObjectTornado *Tornado;

// Standard Entity Events
void Tornado_Update(void);
void Tornado_LateUpdate(void);
void Tornado_StaticUpdate(void);
void Tornado_Draw(void);
void Tornado_Create(void* data);
void Tornado_StageLoad(void);
void Tornado_EditorDraw(void);
void Tornado_EditorLoad(void);
void Tornado_Serialize(void);

// Extra Entity Functions
void Tornado_State_Setup(void);
void Tornado_State_SetupPlayers(void);
void Tornado_Unknown3(void);
void Tornado_Unknown4(void);
void Tornado_Unknown5(void);
void Tornado_Unknown6(void);
void Tornado_Unknown7(void);
void Tornado_Unknown8(void);
void Tornado_Unknown9(void);

#endif //!OBJ_TORNADO_H
