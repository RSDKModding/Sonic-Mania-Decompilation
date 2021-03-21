#ifndef OBJ_WATER_H
#define OBJ_WATER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int waterLevel;
    int newWaterLevel;
    int targetWaterLevel;
    int waterMoveSpeed;
    int field_14[4];
    int field_24[4];
    int field_34[4];
    int bubbleSizes[18];
    ushort spriteIndex;
    ushort bigBubbleSprite;
    ushort wakeSprite;
    Hitbox hitbox2;
    Hitbox hitbox;
    ushort sfx_Splash;
    ushort sfx_Breathe;
    ushort sfx_Warning;
    ushort sfx_DrownAlert;
    ushort sfx_Drown;
    ushort sfx_Skim;
    ushort sfx_DNAGrab;
    ushort sfx_DNABurst;
    ushort sfx_WaterLevelL;
    ushort sfx_WaterLevelR;
    int field_B8;
    int field_BC;
    int field_C0;
    char field_C4;
    char field_C5;
    int field_C8;
    byte field_CC;
    char field_CD;
    int waterPalette;
    byte field_D4;
    byte field_D5;
    int field_D8[4];
    byte array2[4];
    AnimationData wakeData;
    int field_104;
    int field_108;
} ObjectWater;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    void (*stateDraw)();
    int type;
    EntityPlayer *playerPtr;
    int field_68;
    char bubbleType1;
    char field_6D;
    char field_6E;
    byte bubbleType2;
    int numDuds;
    int countdownID;
    int field_78;
    Vector2 size;
    Vector2 height;
    int speed;
    int buttonTag;
    byte r;
    byte g;
    byte b;
    byte priority;
    bool32 destroyOnTrigger;
    Hitbox hitbox;
    int gapA4;
    int field_A8;
    int field_AC;
    char field_B0;
    char field_B1;
    char field_B2;
    char field_B3;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    AnimationData waterData;
} EntityWater;

// Object Struct
extern ObjectWater *Water;

// Standard Entity Events
void Water_Update();
void Water_LateUpdate();
void Water_StaticUpdate();
void Water_Draw();
void Water_Create(void* data);
void Water_StageLoad();
void Water_EditorDraw();
void Water_EditorLoad();
void Water_Serialize();

// Extra Entity Functions


#endif //!OBJ_WATER_H
