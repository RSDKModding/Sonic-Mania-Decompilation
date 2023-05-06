#ifndef OBJ_TIMETRAVELSETUP_H
#define OBJ_TIMETRAVELSETUP_H

#include "Game.h"

// Object Class
struct ObjectTimeTravelSetup {
    RSDK_OBJECT
    uint16 aniFrames;
    int32 timer;
};

// Entity Class
struct EntityTimeTravelSetup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectTimeTravelSetup *TimeTravelSetup;

// Standard Entity Events
void TimeTravelSetup_Update(void);
void TimeTravelSetup_LateUpdate(void);
void TimeTravelSetup_StaticUpdate(void);
void TimeTravelSetup_Draw(void);
void TimeTravelSetup_Create(void *data);
void TimeTravelSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TimeTravelSetup_EditorDraw(void);
void TimeTravelSetup_EditorLoad(void);
#endif
void TimeTravelSetup_Serialize(void);

// Extra Entity Functions
void TimeTravelSetup_Particle_TimeSparkle(EntityDebris *debris);

#endif //! OBJ_TIMETRAVELSETUP_H
