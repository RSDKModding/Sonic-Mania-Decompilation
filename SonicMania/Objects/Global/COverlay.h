#ifndef OBJ_COVERLAY_H
#define OBJ_COVERLAY_H

#include "Game.h"

// Object Class
struct ObjectCOverlay {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityCOverlay {
    RSDK_ENTITY
};

// Object Struct
extern ObjectCOverlay *COverlay;

// Standard Entity Events
void COverlay_Update(void);
void COverlay_LateUpdate(void);
void COverlay_StaticUpdate(void);
void COverlay_Draw(void);
void COverlay_Create(void *data);
void COverlay_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void COverlay_EditorDraw(void);
void COverlay_EditorLoad(void);
#endif
void COverlay_Serialize(void);

// Extra Entity Functions
void COverlay_DebugDraw(void);
void COverlay_DebugSpawn(void);
void COverlay_DrawTile(void);

#endif //! OBJ_COVERLAY_H
