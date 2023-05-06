#ifndef OBJ_CREDITSSETUP_H
#define OBJ_CREDITSSETUP_H

#include "Game.h"

// Object Class
struct ObjectCreditsSetup {
    RSDK_OBJECT
    int32 unused1;
    int32 creditsTrack;
    EntityFXFade *fxFade;
    bool32 started;
    bool32 skipped;
    int32 scrollPos;
    int32 creditsPos;
    int32 creditsSize;
};

// Entity Class
struct EntityCreditsSetup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectCreditsSetup *CreditsSetup;

// Standard Entity Events
void CreditsSetup_Update(void);
void CreditsSetup_LateUpdate(void);
void CreditsSetup_StaticUpdate(void);
void CreditsSetup_Draw(void);
void CreditsSetup_Create(void *data);
void CreditsSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CreditsSetup_EditorDraw(void);
void CreditsSetup_EditorLoad(void);
#endif
void CreditsSetup_Serialize(void);

// Extra Entity Functions
void CreditsSetup_LoadCreditsStrings(void);

#endif //! OBJ_CREDITSSETUP_H
