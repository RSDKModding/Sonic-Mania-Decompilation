#ifndef OBJ_NOSWAP_H
#define OBJ_NOSWAP_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int counter;
    int field_8;
} ObjectNoSwap;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 always;
    bool32 noDeathSwap;
    Hitbox hitbox;
    Vector2 posPtr;
    byte field_78;
    byte field_79;
    byte field_7A;
    byte field_7B;
    byte field_7C;
    byte field_7D;
    byte field_7E;
    byte field_7F;
    byte field_80;
    byte field_81;
    byte field_82;
    byte field_83;
    byte field_84;
    byte field_85;
    byte field_86;
    byte field_87;
    byte field_88;
    byte field_89;
    byte field_8A;
    byte field_8B;
    byte field_8C;
    byte field_8D;
    byte field_8E;
    byte field_8F;
    byte field_90;
    byte field_91;
    byte field_92;
    byte field_93;
    byte field_94;
    byte field_95;
    byte field_96;
    byte field_97;
    byte field_98;
    byte field_99;
    byte field_9A;
    byte field_9B;
    byte field_9C;
    byte field_9D;
    byte field_9E;
    byte field_9F;
    byte field_A0;
    byte field_A1;
    byte field_A2;
    byte field_A3;
    byte field_A4;
    byte field_A5;
    byte field_A6;
    byte field_A7;
} EntityNoSwap;

// Object Struct
extern ObjectNoSwap *NoSwap;

// Standard Entity Events
void NoSwap_Update();
void NoSwap_LateUpdate();
void NoSwap_StaticUpdate();
void NoSwap_Draw();
void NoSwap_Create(void* data);
void NoSwap_StageLoad();
void NoSwap_EditorDraw();
void NoSwap_EditorLoad();
void NoSwap_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_NOSWAP_H
