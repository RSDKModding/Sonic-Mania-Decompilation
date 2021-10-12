#ifndef OBJ_MECHABU_H
#define OBJ_MECHABU_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int sawOffsets[10]; //= { -1376256, -983040, -1507328, -917504, -1638400, -655360, -1703936, -327680, -1703936, -262144 };
    Hitbox hitboxSaw;
    Hitbox hitbox2;
    ushort aniFrames;
    ushort sfxSawUp;
    ushort sfxSawDown;
} ObjectMechaBu;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    PlaneFilterTypes planeFilter;
    byte timer;
    byte timer2;
    Vector2 startPos;
    byte startDir;
    Vector2 sawPos;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityMechaBu;

// Object Struct
extern ObjectMechaBu *MechaBu;

// Standard Entity Events
void MechaBu_Update(void);
void MechaBu_LateUpdate(void);
void MechaBu_StaticUpdate(void);
void MechaBu_Draw(void);
void MechaBu_Create(void* data);
void MechaBu_StageLoad(void);
void MechaBu_EditorDraw(void);
void MechaBu_EditorLoad(void);
void MechaBu_Serialize(void);

// Extra Entity Functions
void MechaBu_DebugSpawn(void);
void MechaBu_DebugDraw(void);

void MechaBu_CheckPlayerCollisions(void);
void MechaBu_CheckOnScreen(void);
Vector2 MechaBu_GetSawOffset(void);

void MechaBu_State_Setup(void);
void MechaBu_Unknown6(void);
void MechaBu_Unknown7(void);
void MechaBu_Unknown8(void);

#endif //!OBJ_MECHABU_H
