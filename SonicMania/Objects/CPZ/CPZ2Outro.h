#ifndef OBJ_CPZ2OUTRO_H
#define OBJ_CPZ2OUTRO_H

#include "Game.h"

// Object Class
struct ObjectCPZ2Outro {
    RSDK_OBJECT
    int32 unused;
    EntityEggPrison *eggPrison;
};

// Entity Class
struct EntityCPZ2Outro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectCPZ2Outro *CPZ2Outro;

// Standard Entity Events
void CPZ2Outro_Update(void);
void CPZ2Outro_LateUpdate(void);
void CPZ2Outro_StaticUpdate(void);
void CPZ2Outro_Draw(void);
void CPZ2Outro_Create(void *data);
void CPZ2Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CPZ2Outro_EditorDraw(void);
void CPZ2Outro_EditorLoad(void);
#endif
void CPZ2Outro_Serialize(void);

// Extra Entity Functions
void CPZ2Outro_SetupCutscene(void);
bool32 CPZ2Outro_Cutscene_Outro(EntityCutsceneSeq *host);

#endif //! OBJ_CPZ2OUTRO_H
