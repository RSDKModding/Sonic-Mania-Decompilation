#ifndef OBJ_BADNIKHELPERS_H
#define OBJ_BADNIKHELPERS_H

#include "Game.h"

// Object Class
struct ObjectBadnikHelpers {
    RSDK_OBJECT
};

// Entity Class
struct EntityBadnikHelpers {
    RSDK_ENTITY
};

// Object Entity
extern ObjectBadnikHelpers *BadnikHelpers;

// Standard Entity Events
void BadnikHelpers_Update(void);
void BadnikHelpers_LateUpdate(void);
void BadnikHelpers_StaticUpdate(void);
void BadnikHelpers_Draw(void);
void BadnikHelpers_Create(void *data);
void BadnikHelpers_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void BadnikHelpers_EditorDraw(void);
void BadnikHelpers_EditorLoad(void);
#endif
void BadnikHelpers_Serialize(void);

// Extra Entity Functions
// 
// Handles oscillations
int BadnikHelpers_Oscillate(int origin, int speed, int amplitude);

#endif //! OBJ_BADNIKHELPERS_H
