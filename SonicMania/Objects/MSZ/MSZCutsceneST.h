#ifndef OBJ_MSZCUTSCENEST_H
#define OBJ_MSZCUTSCENEST_H

#include "Game.h"

// Object Class
struct ObjectMSZCutsceneST {
    RSDK_OBJECT
    TABLE(int32 signPostOffsets[6], { -0x10000, -0x10000, 0, 0, 0, -0x10000 });
    uint16 unused1;
    uint16 unused2;
    Vector2 tornadoStartPos;
    uint8 rogueID;
    EntitySignPost *signPost;
    EntityCutsceneHBH *mystic;
    EntityTornado *tornado;
    EntityTornadoPath *tornadoPath;
    Entity *projectile;
    EntityCutsceneHBH *rogues[3];
    EntityArmadiloid *roguePlatforms[3];
    bool32 finishedAct;
    bool32 isMayday;
    uint16 sfxExplosion3;
    uint16 sfxMayday;
    uint16 sfxLedgeBreak;
    bool32 playingMaydaySfx;
};

// Entity Class
struct EntityMSZCutsceneST {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    EntityTornadoPath *pathNode;
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    int32 unused6;
    Hitbox hitbox;
};

// Object Struct
extern ObjectMSZCutsceneST *MSZCutsceneST;

// Standard Entity Events
void MSZCutsceneST_Update(void);
void MSZCutsceneST_LateUpdate(void);
void MSZCutsceneST_StaticUpdate(void);
void MSZCutsceneST_Draw(void);
void MSZCutsceneST_Create(void *data);
void MSZCutsceneST_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSZCutsceneST_EditorDraw(void);
void MSZCutsceneST_EditorLoad(void);
#endif
void MSZCutsceneST_Serialize(void);

// Extra Entity Functions
void MSZCutsceneST_SetupCutscene(void);

bool32 MSZCutsceneST_Cutscene_HandleSignPostLand(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_AwaitActFinish(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_EnterMystic(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_PrepareAmbush(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_RoguesAmbush(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_ShowFang(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_ShowBean(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_ShowBark(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_Mayday(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_SetPlayerMSZ2SpawnPos(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_PanCameraToPlayer(EntityCutsceneSeq *host);
bool32 MSZCutsceneST_Cutscene_SetupMSZ2(EntityCutsceneSeq *host);

#endif //! OBJ_MSZCUTSCENEST_H
