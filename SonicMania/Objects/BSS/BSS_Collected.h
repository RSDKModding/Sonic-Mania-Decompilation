#ifndef OBJ_BSS_COLLECTED_H
#define OBJ_BSS_COLLECTED_H

#include "Game.h"

typedef enum {
    BSS_COLLECTED_RING,
    BSS_COLLECTED_BLUE,
    BSS_COLLECTED_BLUE_STOOD,
    BSS_COLLECTED_GREEN,
    BSS_COLLECTED_GREEN_STOOD,
    BSS_COLLECTED_PINK,
} BSS_CollectedTypes;

// Object Class
struct ObjectBSS_Collected {
    RSDK_OBJECT
};

// Entity Class
struct EntityBSS_Collected {
    RSDK_ENTITY
    int32 type;
    int32 timer;
};

// Object Struct
extern ObjectBSS_Collected *BSS_Collected;

// Standard Entity Events
void BSS_Collected_Update(void);
void BSS_Collected_LateUpdate(void);
void BSS_Collected_StaticUpdate(void);
void BSS_Collected_Draw(void);
void BSS_Collected_Create(void *data);
void BSS_Collected_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BSS_Collected_EditorDraw(void);
void BSS_Collected_EditorLoad(void);
#endif
void BSS_Collected_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_BSS_COLLECTED_H
