#ifndef OBJ_ENCOREROUTE_H
#define OBJ_ENCOREROUTE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    char field_4;
    char field_5;
    char field_6;
    char field_7;
    char field_8;
    char field_9;
    char field_A;
    char field_B;
    char field_C;
    char field_D;
    char field_E;
    char field_F;
    char field_10;
    char field_11;
    char field_12;
    char field_13;
    char field_14;
    char field_15;
    char field_16;
    char field_17;
    char field_18;
    char field_19;
    char field_1A;
    char field_1B;
    char field_1C;
    char field_1D;
    char field_1E;
    char field_1F;
} ObjectEncoreRoute;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 offset;
    Vector2 size;
    char layerSrc;
    char layerDest;
    int frameID;
} EntityEncoreRoute;

// Object Struct
extern ObjectEncoreRoute *EncoreRoute;

// Standard Entity Events
void EncoreRoute_Update();
void EncoreRoute_LateUpdate();
void EncoreRoute_StaticUpdate();
void EncoreRoute_Draw();
void EncoreRoute_Create(void* data);
void EncoreRoute_StageLoad();
void EncoreRoute_EditorDraw();
void EncoreRoute_EditorLoad();
void EncoreRoute_Serialize();

// Extra Entity Functions


#endif //!OBJ_ENCOREROUTE_H
