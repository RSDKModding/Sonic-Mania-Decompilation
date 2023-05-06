#ifndef OBJ_CORKSCREWPATH_H
#define OBJ_CORKSCREWPATH_H

#include "Game.h"

// Object Class
struct ObjectCorkscrewPath {
    RSDK_OBJECT
    TABLE(int32 frameTable[24], { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 });
    uint16 aniFrames; // probably editor-only
};

// Entity Class
struct EntityCorkscrewPath {
    RSDK_ENTITY
    int32 period;
    int32 xSize; // length to the (left/right) edges from the center
    int32 amplitude;
    int32 activePlayers;
    Animator animator; // also probably editor-only
};

// Object Struct
extern ObjectCorkscrewPath *CorkscrewPath;

// Standard Entity Events
void CorkscrewPath_Update(void);
void CorkscrewPath_LateUpdate(void);
void CorkscrewPath_StaticUpdate(void);
void CorkscrewPath_Draw(void);
void CorkscrewPath_Create(void *data);
void CorkscrewPath_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CorkscrewPath_EditorDraw(void);
void CorkscrewPath_EditorLoad(void);
#endif
void CorkscrewPath_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_CORKSCREWPATH_H
