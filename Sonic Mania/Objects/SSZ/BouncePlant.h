#ifndef OBJ_BOUNCEPLANT_H
#define OBJ_BOUNCEPLANT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxBouncePlant;
} ObjectBouncePlant;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    Vector2 drawPos[8];
    Vector2 drawPos2[8];
    int unknown[8];
    Vector2 stoodPos;
    bool32 stood;
    bool32 speedFlag;
    int depression;
    int centerX;
    Animator animator;
    Animator animators[8];
} EntityBouncePlant;

// Object Struct
extern ObjectBouncePlant *BouncePlant;

// Standard Entity Events
void BouncePlant_Update(void);
void BouncePlant_LateUpdate(void);
void BouncePlant_StaticUpdate(void);
void BouncePlant_Draw(void);
void BouncePlant_Create(void* data);
void BouncePlant_StageLoad(void);
void BouncePlant_EditorDraw(void);
void BouncePlant_EditorLoad(void);
void BouncePlant_Serialize(void);

// Extra Entity Functions
void BoucePlant_Unknown1(void);
int BoucePlant_Unknown2(int x);
int BoucePlant_Unknown3(int x);

#endif //!OBJ_BOUNCEPLANT_H
