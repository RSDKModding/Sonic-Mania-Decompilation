#ifndef OBJ_HATCH_H
#define OBJ_HATCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
    uint16 aniFrames;
    uint16 sfxHatchOpen;
    uint16 sfxHatchClose;
    uint16 sfxDescend;
    uint16 sfxSurface;
    uint16 sfxGasPop;
} ObjectHatch;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 go;
    Vector2 subOff1;
    Vector2 subOff2;
    int32 depth;
    int32 dest;
    Vector2 startPos;
    int32 hScrollPos;
    int32 vScrollPos;
    int32 timer;
    bool32 flag;
    Entity *playerPtr;
    StateMachine(stateStore);
    bool32 interactionStore;
    bool32 tileColStore;
    Animator animator1;
    Animator animator2;
    Hitbox hitbox;
    Hitbox warpHitbox;
} EntityHatch;

// Object Struct
extern ObjectHatch *Hatch;

// Standard Entity Events
void Hatch_Update(void);
void Hatch_LateUpdate(void);
void Hatch_StaticUpdate(void);
void Hatch_Draw(void);
void Hatch_Create(void* data);
void Hatch_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Hatch_EditorDraw(void);
void Hatch_EditorLoad(void);
#endif
void Hatch_Serialize(void);

// Extra Entity Functions
void Hatch_Unknown1(void);
void Hatch_Unknown2(void);
void Hatch_Unknown3(void);
void Hatch_Unknown4(void);
void Hatch_Unknown5(void);
void Hatch_Unknown6(void);
void Hatch_Unknown7(void);
void Hatch_Unknown8(void);
void Hatch_Unknown9(void);
void Hatch_Unknown10(void);
void Hatch_Unknown11(void);

#endif //!OBJ_HATCH_H
