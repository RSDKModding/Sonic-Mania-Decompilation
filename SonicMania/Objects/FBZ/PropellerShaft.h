#ifndef OBJ_PROPELLERSHAFT_H
#define OBJ_PROPELLERSHAFT_H

#include "Game.h"

// Object Class
struct ObjectPropellerShaft {
    RSDK_OBJECT
};

// Entity Class
struct EntityPropellerShaft {
    RSDK_ENTITY
    int32 size;
    int32 activePlayers;
    int32 playerTimers[PLAYER_COUNT];
    int32 unused1[PLAYER_COUNT];
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectPropellerShaft *PropellerShaft;

// Standard Entity Events
void PropellerShaft_Update(void);
void PropellerShaft_LateUpdate(void);
void PropellerShaft_StaticUpdate(void);
void PropellerShaft_Draw(void);
void PropellerShaft_Create(void *data);
void PropellerShaft_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PropellerShaft_EditorDraw(void);
void PropellerShaft_EditorLoad(void);
#endif
void PropellerShaft_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_PROPELLERSHAFT_H
