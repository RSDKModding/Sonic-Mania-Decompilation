#ifndef OBJ_UNCURLPLANT_H
#define OBJ_UNCURLPLANT_H

#include "Game.h"

#define UNCURLPLANT_NODE_COUNT (8)

// Object Class
struct ObjectUncurlPlant {
    RSDK_OBJECT
    Hitbox hitboxNode;
    TABLE(int32 startingNodeAngles[8], { 0x00, 0x10, 0x40, 0x60, 0x70, 0x90, 0xA0, 0xC0 });
    TABLE(int32 targetNodeAnglesReleased[8], { 0x00, -0x08, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40 });
    TABLE(int32 targetNodeAnglesStood[8], { 0x00, -0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 });
    uint16 aniFrames;
};

// Entity Class
struct EntityUncurlPlant {
    RSDK_ENTITY
    StateMachine(state); // unused
    int32 unused;
    Vector2 drawPositions[UNCURLPLANT_NODE_COUNT];
    Vector2 nodePositions[UNCURLPLANT_NODE_COUNT];
    int32 nodeAngles[UNCURLPLANT_NODE_COUNT];
    int32 targetNodeAngles[UNCURLPLANT_NODE_COUNT];
    int32 stood;
    int32 uncurlPercent;
    int32 stoodNodeID;
    int32 uncurlMode;
    Animator nodeAnimator;
    Animator decorAnimators[UNCURLPLANT_NODE_COUNT];
};

// Object Struct
extern ObjectUncurlPlant *UncurlPlant;

// Standard Entity Events
void UncurlPlant_Update(void);
void UncurlPlant_LateUpdate(void);
void UncurlPlant_StaticUpdate(void);
void UncurlPlant_Draw(void);
void UncurlPlant_Create(void *data);
void UncurlPlant_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UncurlPlant_EditorDraw(void);
void UncurlPlant_EditorLoad(void);
#endif
void UncurlPlant_Serialize(void);

// Extra Entity Functions
void UncurlPlant_CalculateDrawPositions(void);
void UncurlPlant_CalculatePositions(void);

#endif //! OBJ_UNCURLPLANT_H
