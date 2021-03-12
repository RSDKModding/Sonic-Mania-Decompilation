#ifndef OBJ_SPECIALRING_H
#define OBJ_SPECIALRING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpecialRing : Object {
    ushort spriteIndex;
    Hitbox hitbox;
    ushort sfx_SpecialRing;
    ushort sfx_SpecialWarp;
    ushort modelIndex;
    ushort sceneIndex;
};

// Entity Class
struct EntitySpecialRing : Entity {
    void (*state)();
    int id;
    int planeFilter;
    int warpTimer;
    int dword68;
    EntityAnimationData warpData;
    int angleZ;
    int angleY;
    bool32 enabled;
    Matrix matrix4;
    Matrix matrix;
    Matrix matrix2;
    Matrix matrix3;
};

// Object Struct
extern ObjectSpecialRing *SpecialRing;

// Standard Entity Events
void SpecialRing_Update();
void SpecialRing_LateUpdate();
void SpecialRing_StaticUpdate();
void SpecialRing_Draw();
void SpecialRing_Create(void* data);
void SpecialRing_StageLoad();
void SpecialRing_EditorDraw();
void SpecialRing_EditorLoad();
void SpecialRing_Serialize();

// Extra Entity Functions
void SpecialRing_DebugDraw();
void SpecialRing_DebugSpawn();

void SpecialRing_StartWarp();
void SpecialRing_State_Warp();
void SpecialRing_State_Normal();

#endif //!OBJ_SPECIALRING_H
