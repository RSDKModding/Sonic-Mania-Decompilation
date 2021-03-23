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
    int array3[4];
    int array4[4];
    int array5[4];
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
    int waterLevelChannel_L;
    int waterLevelChannel_R;
    int field_C0;
    bool32 playingWaterLevelSFX;
    int field_C8;
    int waterLevelVolume;
    int waterPalette;
    int field_D4;
    int wakePosX[4];
    byte wakeDir[4];
    AnimationData wakeData;
    int field_104;
    bool32 playingSkimSFX;
} ObjectWater;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    void (*stateDraw)();
    int type;
    void *childPtr;
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
    int timer;
    char activePlayers;
    char activePlayers2;
    char field_B2;
    char field_B3;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int taggedObject;
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

//Palette stuff
void Water_SetWaterLevel();
void Water_RemoveWaterEffect();

//Utils
void Water_CheckButtonTag();
void Water_SpawnBubble(EntityPlayer *player, int id);
void Water_SpawnCountDownBubble(EntityPlayer *player, int id, byte bubbleID);
EntityWater *Water_Unknown7(EntityPlayer *entityPtr);

void Water_State_Palette();
void Water_State_Splash();
void Water_Unknown4();
void Water_Unknown5(EntityWater *entity, int a2);
void Water_State_Bubble();
void Water_Unknown6();
void Water_Unknown8();
void Water_Unknown9();
void Water_State_Bubbler();
void Water_State_CountDownBubble();
void Water_State_BubbleMove();
void Water_State_Adjustable();

//Draw States
void Water_State_Draw_Palette();
void Water_State_Draw_Tint();
void Water_State_Draw_Splash();
void Water_State_Draw_CountDownBubble();
void Water_State_Draw_Bubbler();
void Water_State_Draw_Bubble();

#endif //!OBJ_WATER_H
