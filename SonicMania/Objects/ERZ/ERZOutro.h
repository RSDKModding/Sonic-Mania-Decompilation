#ifndef OBJ_ERZOUTRO_H
#define OBJ_ERZOUTRO_H

#include "Game.h"

// Object Class
struct ObjectERZOutro {
    RSDK_OBJECT
    int32 unused; // Might be "uint16 aniFrames" and was used inEditor but I do not know
    Vector2 playerPos;
    EntityFXRuby *fxRuby;
    EntityPhantomKing *king;
    EntityKleptoMobile *eggman;
    EntityRubyPortal *rubyPortal;
    EntityPhantomRuby *ruby;
    int32 rubyPortalAcceleration;
    int32 rubyPortalRadius;
    int32 rubyPortalAngle;
    bool32 savedGame;
};

// Entity Class
struct EntityERZOutro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectERZOutro *ERZOutro;

// Standard Entity Events
void ERZOutro_Update(void);
void ERZOutro_LateUpdate(void);
void ERZOutro_StaticUpdate(void);
void ERZOutro_Draw(void);
void ERZOutro_Create(void *data);
void ERZOutro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ERZOutro_EditorDraw(void);
void ERZOutro_EditorLoad(void);
#endif
void ERZOutro_Serialize(void);

// Extra Entity Functions
void ERZOutro_SetEmeraldStates(void);
void ERZOutro_HandleRubyHover(void);

bool32 ERZOutro_Cutscene_AttackEggman(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_AttackRecoil(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_LoseEmeralds(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_OpenPortal(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_EnterPortal(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_FadeOut(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_ShowEnding(EntityCutsceneSeq *host);

#if MANIA_USE_PLUS
void ERZOutro_SaveFileCB(bool32 success);
#else
void ERZOutro_SaveFileCB(void);
#endif

#endif //! OBJ_ERZOUTRO_H
