#ifndef OBJ_GHZCUTSCENEK_H
#define OBJ_GHZCUTSCENEK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
} ObjectGHZCutsceneK;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    int32 timer;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Hitbox hitbox;
} EntityGHZCutsceneK;

// Object Struct
extern ObjectGHZCutsceneK *GHZCutsceneK;

// Standard Entity Events
void GHZCutsceneK_Update(void);
void GHZCutsceneK_LateUpdate(void);
void GHZCutsceneK_StaticUpdate(void);
void GHZCutsceneK_Draw(void);
void GHZCutsceneK_Create(void* data);
void GHZCutsceneK_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GHZCutsceneK_EditorDraw(void);
void GHZCutsceneK_EditorLoad(void);
#endif
void GHZCutsceneK_Serialize(void);

// Extra Entity Functions
bool32 GHZCutsceneK_Unknown(void);

#endif //!OBJ_GHZCUTSCENEK_H
