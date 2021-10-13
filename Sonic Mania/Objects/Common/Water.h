#ifndef OBJ_WATER_H
#define OBJ_WATER_H

#include "SonicMania.h"

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
    TABLE(int bubbleSizes[18], { 2, 4, 2, 2, 2, 2, 4, 2, 4, 2, 2, 4, 2, 4, 2, 2, 4, 2 });
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
    Animator wakeData;
    int field_104;
    bool32 playingSkimSFX;
} ObjectWater;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int type;
    void *childPtr;
    int field_68;
    sbyte bubbleType1;
    byte field_6D;
    byte field_6E;
    sbyte bubbleType2;
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
    byte activePlayers;
    byte activePlayers2;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    EntityButton* taggedObject;
    Animator animator;
} EntityWater;

// Object Struct
extern ObjectWater *Water;

// Standard Entity Events
void Water_Update(void);
void Water_LateUpdate(void);
void Water_StaticUpdate(void);
void Water_Draw(void);
void Water_Create(void* data);
void Water_StageLoad(void);
void Water_EditorDraw(void);
void Water_EditorLoad(void);
void Water_Serialize(void);

// Extra Entity Functions

//Palette stuff
void Water_SetWaterLevel(void);
void Water_RemoveWaterEffect(void);

//Utils
void Water_CheckButtonTag(void);
void Water_SpawnBubble(EntityPlayer *player, int id);
void Water_SpawnCountDownBubble(EntityPlayer *player, int id, byte bubbleID);
EntityWater *Water_GetPlayerBubble(EntityPlayer *entityPtr);

void Water_State_Palette(void);
void Water_State_Tint(void);
void Water_State_Splash(void);
void Water_Unknown4(void);
void Water_HCZBubbleBurst(EntityWater *entity, bool32 jumpedOut);
void Water_State_Bubble(void);
void Water_Unknown6(void);
void Water_State_HCZBubble(void);
void Water_Unknown9(void);
void Water_State_Bubbler(void);
void Water_State_CountDownBubble(void);
void Water_State_BubbleMove(void);
void Water_State_Adjustable(void);

//Draw States
void Water_State_Draw_Palette(void);
void Water_State_Draw_Tint(void);
void Water_State_Draw_Splash(void);
void Water_State_Draw_CountDownBubble(void);
void Water_State_Draw_Bubbler(void);
void Water_State_Draw_Bubble(void);

#endif //!OBJ_WATER_H
