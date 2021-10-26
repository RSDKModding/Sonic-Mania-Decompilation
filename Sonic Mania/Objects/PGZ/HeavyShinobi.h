#ifndef OBJ_HEAVYSHINOBI_H
#define OBJ_HEAVYSHINOBI_H

#include "SonicMania.h"

typedef enum {
    SHINOBI_MAIN,
    SHINOBI_SLASH,
    SHINOBI_SHURIKEN,
    SHINOBI_3,
    SHINOBI_BOUNDS,
}HeavyShinobiTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 health;
    uint8 invincibilityTimer;
    Animator animator1[4];
    uint8 storedIDs[4];
    uint8 storedAnimIDs[4];
    Vector2 storePos[16];
    int32 field_F0;
    int32 field_F4;
    int32 field_F8;
    int32 field_FC;
    int32 field_100;
    int32 field_104;
    uint8 activeShurikens;
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
    Hitbox hitbox6;
    Hitbox hitbox7;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDefeat;
    uint16 sfxDropIn;
    uint16 sfxExplode;
    uint16 sfxGlitch;
    uint16 sfxJump;
    uint16 sfxParry;
    uint16 sfxSlash;
    uint16 sfxStick;
    uint16 sfxThrow;
} ObjectHeavyShinobi;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 glitchTimer;
    Entity *playerPtr;
    Vector2 playerDistance;
    Animator animator1;
    Animator animator2;
} EntityHeavyShinobi;

// Object Struct
extern ObjectHeavyShinobi *HeavyShinobi;

// Standard Entity Events
void HeavyShinobi_Update(void);
void HeavyShinobi_LateUpdate(void);
void HeavyShinobi_StaticUpdate(void);
void HeavyShinobi_Draw(void);
void HeavyShinobi_Create(void* data);
void HeavyShinobi_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HeavyShinobi_EditorDraw(void);
void HeavyShinobi_EditorLoad(void);
#endif
void HeavyShinobi_Serialize(void);

// Extra Entity Functions
void HeavyShinobi_HandleAfterFX(void);
void HeavyShinobi_HandleSlash(void *p);
void HeavyShinobi_Unknown3(void);
void HeavyShinobi_Explode(void);

void HeavyShinobi_State_Setup(void);
void HeavyShinobi_State_SetupArena(void);
void HeavyShinobi_State_StartFight(void);
void HeavyShinobi_State_Unknown1(void);
void HeavyShinobi_State_Unknown2(void);
void HeavyShinobi_State_Unknown3(void);
void HeavyShinobi_State_Glitched(void);
void HeavyShinobi_State_Destroyed(void);
void HeavyShinobi_State_Finished(void);
void HeavyShinobi_StateDraw_Unknown1(void);

void HeavyShinobi_State1_Unknown1(void);
void HeavyShinobi_StateDraw1_Unknown1(void);

void HeavyShinobi_State2_Unknown1(void);
void HeavyShinobi_State2_Unknown2(void);
void HeavyShinobi_State2_Unknown3(void);
void HeavyShinobi_StateDraw2_Unknown1(void);

void HeavyShinobi_State3_Unknown1(void);
void HeavyShinobi_StateDraw3_Unknown1(void);

void HeavyShinobi_State4_Unknown1(void);
void HeavyShinobi_State4_Unknown2(void);
void HeavyShinobi_StateDraw4_Unknown1(void);

#endif //!OBJ_HEAVYSHINOBI_H
