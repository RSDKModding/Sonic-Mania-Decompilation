#ifndef OBJ_LRZCONVEYOR_H
#define OBJ_LRZCONVEYOR_H

#include "SonicMania.h"

// Object Class
struct ObjectLRZConveyor {
    RSDK_OBJECT
    uint16 aniFrames;
    colour lineColours[64];
};

// Entity Class
struct EntityLRZConveyor {
    RSDK_ENTITY
    int32 slope;
    int32 length;
    int32 speed;
    bool32 globalControl;
    int32 buttonTag;
    uint8 buttonBehavior;
    bool32 off;
    uint8 isOff;
    uint8 startDir;
    Vector2 startPos;
    int32 field_80;
    EntityButton *taggedButton;
    Hitbox hitbox;
    Animator animator1;
    Animator animator2;
};

// Object Struct
extern ObjectLRZConveyor *LRZConveyor;

// Standard Entity Events
void LRZConveyor_Update(void);
void LRZConveyor_LateUpdate(void);
void LRZConveyor_StaticUpdate(void);
void LRZConveyor_Draw(void);
void LRZConveyor_Create(void* data);
void LRZConveyor_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LRZConveyor_EditorDraw(void);
void LRZConveyor_EditorLoad(void);
#endif
void LRZConveyor_Serialize(void);

// Extra Entity Functions
void LRZConveyor_HandleBehaviour(void);
void LRZConveyor_HandlePlayerInteractions(void);
int LRZConveyor_HandlePlayerCollisions(EntityLRZConveyor *conveyor, void *p, Hitbox *hitboxPtr);
void LRZConveyor_Unknown4(void);
void LRZConveyor_Unknown5(colour *colour, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
void LRZConveyor_Unknown6(void);

#endif //!OBJ_LRZCONVEYOR_H
