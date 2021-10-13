#ifndef OBJ_NOSWAP_H
#define OBJ_NOSWAP_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 counter;
    int32 field_8;
} ObjectNoSwap;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 always;
    bool32 noDeathSwap;
    Hitbox hitbox;
    Vector2 posPtr;
    uint8 field_78;
    uint8 field_79;
    uint8 field_7A;
    uint8 field_7B;
    uint8 field_7C;
    uint8 field_7D;
    uint8 field_7E;
    uint8 field_7F;
    uint8 field_80;
    uint8 field_81;
    uint8 field_82;
    uint8 field_83;
    uint8 field_84;
    uint8 field_85;
    uint8 field_86;
    uint8 field_87;
    uint8 field_88;
    uint8 field_89;
    uint8 field_8A;
    uint8 field_8B;
    uint8 field_8C;
    uint8 field_8D;
    uint8 field_8E;
    uint8 field_8F;
    uint8 field_90;
    uint8 field_91;
    uint8 field_92;
    uint8 field_93;
    uint8 field_94;
    uint8 field_95;
    uint8 field_96;
    uint8 field_97;
    uint8 field_98;
    uint8 field_99;
    uint8 field_9A;
    uint8 field_9B;
    uint8 field_9C;
    uint8 field_9D;
    uint8 field_9E;
    uint8 field_9F;
    uint8 field_A0;
    uint8 field_A1;
    uint8 field_A2;
    uint8 field_A3;
    uint8 field_A4;
    uint8 field_A5;
    uint8 field_A6;
    uint8 field_A7;
} EntityNoSwap;

// Object Struct
extern ObjectNoSwap *NoSwap;

// Standard Entity Events
void NoSwap_Update(void);
void NoSwap_LateUpdate(void);
void NoSwap_StaticUpdate(void);
void NoSwap_Draw(void);
void NoSwap_Create(void* data);
void NoSwap_StageLoad(void);
void NoSwap_EditorDraw(void);
void NoSwap_EditorLoad(void);
void NoSwap_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_NOSWAP_H
