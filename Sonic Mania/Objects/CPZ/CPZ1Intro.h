#ifndef OBJ_CPZ1INTRO_H
#define OBJ_CPZ1INTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 playerFrames;
    uint16 particleSpriteIndex;
    uint16 field_8;
    uint16 sfxChemDrop;
    uint16 sfxDNABurst;
    int playerAnimID;
    Entity *fxRuby;
    Entity *debris;
} ObjectCPZ1Intro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    Hitbox hitbox;
} EntityCPZ1Intro;

// Object Struct
extern ObjectCPZ1Intro *CPZ1Intro;

// Standard Entity Events
void CPZ1Intro_Update(void);
void CPZ1Intro_LateUpdate(void);
void CPZ1Intro_StaticUpdate(void);
void CPZ1Intro_Draw(void);
void CPZ1Intro_Create(void* data);
void CPZ1Intro_StageLoad(void);
void CPZ1Intro_EditorDraw(void);
void CPZ1Intro_EditorLoad(void);
void CPZ1Intro_Serialize(void);

// Extra Entity Functions
void CPZ1Intro_Particle_CB(Entity *debris);
void CPZ1Intro_Unknown3(Entity *player1, Entity *cutSeq, Entity *player2, int val);

bool32 CPZ1Intro_CheckSonicAnimFinish(void);
bool32 CPZ1Intro_CheckTailsAnimFinish(int timer);
bool32 CPZ1Intro_CheckKnuxAnimFinish(void);

#if RETRO_USE_PLUS
bool32 CPZ1Intro_CheckMightyAnimFinish(void);
bool32 CPZ1Intro_CheckRayAnimFinish(void);
#endif

bool32 CPZ1Intro_Unknown5(void *h);
bool32 CPZ1Intro_Unknown6(void *h);
bool32 CPZ1Intro_Unknown7(void *h);
bool32 CPZ1Intro_Unknown8(void *h);
bool32 CPZ1Intro_Unknown9(void *h);
bool32 CPZ1Intro_Unknown15(void *h);

#endif //!OBJ_CPZ1INTRO_H
