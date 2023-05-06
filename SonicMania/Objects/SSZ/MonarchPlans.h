#ifndef OBJ_MONARCHPLANS_H
#define OBJ_MONARCHPLANS_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectMonarchPlans {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 unused;
    uint16 meshIndex;
    uint16 sceneIndex;
};

// Entity Class
struct EntityMonarchPlans {
    RSDK_ENTITY
    StateMachine(state); // unused
    void *unused1;
    void *unused2;
    void *unused3;
    int32 unused4;
    int32 unused5;
    int32 unused6;
    int32 unused7;
    int32 rotationZ;
    int32 rotationY;
    Matrix matNormalTemp;
    Matrix matWorldTemp;
    Matrix matWorld;
    Matrix matNormal;
};

// Object Struct
extern ObjectMonarchPlans *MonarchPlans;

// Standard Entity Events
void MonarchPlans_Update(void);
void MonarchPlans_LateUpdate(void);
void MonarchPlans_StaticUpdate(void);
void MonarchPlans_Draw(void);
void MonarchPlans_Create(void *data);
void MonarchPlans_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MonarchPlans_EditorDraw(void);
void MonarchPlans_EditorLoad(void);
#endif
void MonarchPlans_Serialize(void);

// Extra Entity Functions
#endif

#endif //! OBJ_MONARCHPLANS_H
