#ifndef OBJ_AIZEGGROBO_H
#define OBJ_AIZEGGROBO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectAIZEggRobo;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    bool32 oscillate;
    bool32 forKnux;
    int32 field_64;
    Vector2 unknownPos;
    Vector2 startPos;
    int32 field_78;
    int32 oscillateOffset;
    int32 field_80;
    int32 field_84;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityAIZEggRobo;

// Object Struct
extern ObjectAIZEggRobo *AIZEggRobo;

// Standard Entity Events
void AIZEggRobo_Update(void);
void AIZEggRobo_LateUpdate(void);
void AIZEggRobo_StaticUpdate(void);
void AIZEggRobo_Draw(void);
void AIZEggRobo_Create(void* data);
void AIZEggRobo_StageLoad(void);
void AIZEggRobo_EditorDraw(void);
void AIZEggRobo_EditorLoad(void);
void AIZEggRobo_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_AIZEGGROBO_H
