#ifndef OBJ_MSZCUTSCENEST_H
#define OBJ_MSZCUTSCENEST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 signPostOffsets[6], { -0x10000, -0x10000, 0, 0, 0, -0x10000 });
    uint16 value2;
    uint16 value3;
    Vector2 value4;
    uint8 rougeID;
    Entity *signPost;
    EntityCutsceneHBH *mystic;
    Entity *tornado;
    Entity *nodePtr;
    Entity *projectile;
    EntityCutsceneHBH *rouges[3];
    EntityArmadiloid *rougePlatforms[3];
    bool32 actFinishFlag;
    bool32 isMayday;
    uint16 sfxExplosion3;
    uint16 sfxMayday;
    uint16 sfxLedgeBreak;
    bool32 playingMaydaySfx;
} ObjectMSZCutsceneST;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    Entity* nodePtr;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Hitbox hitbox;
} EntityMSZCutsceneST;

// Object Struct
extern ObjectMSZCutsceneST *MSZCutsceneST;

// Standard Entity Events
void MSZCutsceneST_Update(void);
void MSZCutsceneST_LateUpdate(void);
void MSZCutsceneST_StaticUpdate(void);
void MSZCutsceneST_Draw(void);
void MSZCutsceneST_Create(void* data);
void MSZCutsceneST_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MSZCutsceneST_EditorDraw(void);
void MSZCutsceneST_EditorLoad(void);
#endif
void MSZCutsceneST_Serialize(void);

// Extra Entity Functions
void MSZCutsceneST_SetupCutscene(void);

bool32 MSZCutsceneST_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_Unknown3(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_Unknown4(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_Unknown5(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_ShowRougeA(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_ShowRougeB(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_ShowRougeC(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_Unknown6(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_Unknown7(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_Unknown8(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_CutsceneState_Unknown9(EntityCutsceneSeq *host);

#endif //!OBJ_MSZCUTSCENEST_H
