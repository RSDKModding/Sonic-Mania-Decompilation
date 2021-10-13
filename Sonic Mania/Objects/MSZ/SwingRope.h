#ifndef OBJ_SWINGROPE_H
#define OBJ_SWINGROPE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 spriteIndex;
} ObjectSwingRope;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 ropeSize;
    int ropeGrabDelay;
    int rotatedAngle;
    int rotatedOffset;
    int playerLayers[4];
    int angleOffset;
    Vector2 ropePos;
    Vector2 rotatePos;
    Animator ropeData;
    Animator handleData;
    Animator pivotData;
} EntitySwingRope;

// Object Struct
extern ObjectSwingRope *SwingRope;

// Standard Entity Events
void SwingRope_Update(void);
void SwingRope_LateUpdate(void);
void SwingRope_StaticUpdate(void);
void SwingRope_Draw(void);
void SwingRope_Create(void* data);
void SwingRope_StageLoad(void);
void SwingRope_EditorDraw(void);
void SwingRope_EditorLoad(void);
void SwingRope_Serialize(void);

// Extra Entity Functions
void SwingRope_DebugDraw(void);
void SwingRope_DebugSpawn(void);

#endif //!OBJ_SWINGROPE_H
