#ifndef OBJ_SWINGROPE_H
#define OBJ_SWINGROPE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    ushort spriteIndex;
} ObjectSwingRope;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte ropeSize;
    int ropeGrabDelay;
    int rotatedAngle;
    int rotatedOffset;
    int playerLayers[4];
    int angleOffset;
    Vector2 ropePos;
    Vector2 rotatePos;
    AnimationData ropeData;
    AnimationData handleData;
    AnimationData pivotData;
} EntitySwingRope;

// Object Struct
extern ObjectSwingRope *SwingRope;

// Standard Entity Events
void SwingRope_Update();
void SwingRope_LateUpdate();
void SwingRope_StaticUpdate();
void SwingRope_Draw();
void SwingRope_Create(void* data);
void SwingRope_StageLoad();
void SwingRope_EditorDraw();
void SwingRope_EditorLoad();
void SwingRope_Serialize();

// Extra Entity Functions
void SwingRope_DebugDraw();
void SwingRope_DebugSpawn();

#endif //!OBJ_SWINGROPE_H
