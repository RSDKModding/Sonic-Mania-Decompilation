#ifndef OBJ_COVERLAY_H
#define OBJ_COVERLAY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCOverlay : Object {
    ushort spriteIndex;
};

// Entity Class
struct EntityCOverlay : Entity {

};

// Object Struct
extern ObjectCOverlay *COverlay;

// Standard Entity Events
void COverlay_Update();
void COverlay_LateUpdate();
void COverlay_StaticUpdate();
void COverlay_Draw();
void COverlay_Create(void* data);
void COverlay_StageLoad();
void COverlay_EditorDraw();
void COverlay_EditorLoad();
void COverlay_Serialize();

// Extra Entity Functions
void COverlay_DebugDraw();
void COverlay_DebugSpawn();
void COverlay_DrawTile();

#endif //!OBJ_COVERLAY_H
