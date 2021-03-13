#ifndef OBJ_CORKSCREWPATH_H
#define OBJ_CORKSCREWPATH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int frameTable[24]; //= { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 };
    ushort spriteIndex; // probably editor-only
} ObjectCorkscrewPath;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int period;
    int periodShifted;
    int amplitude;
    int activePlayers;
} EntityCorkscrewPath;

// Object Struct
extern ObjectCorkscrewPath *CorkscrewPath;

// Standard Entity Events
void CorkscrewPath_Update();
void CorkscrewPath_LateUpdate();
void CorkscrewPath_StaticUpdate();
void CorkscrewPath_Draw();
void CorkscrewPath_Create(void* data);
void CorkscrewPath_StageLoad();
void CorkscrewPath_EditorDraw();
void CorkscrewPath_EditorLoad();
void CorkscrewPath_Serialize();

// Extra Entity Functions


#endif //!OBJ_CORKSCREWPATH_H
