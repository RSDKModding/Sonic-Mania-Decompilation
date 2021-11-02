#ifndef OBJ_WATER_H
#define OBJ_WATER_H

#include "SonicMania.h"

typedef enum {
    WATER_PALETTE,
    WATER_RECT,
    WATER_BUBBLER,
    WATER_ADJUST,
    WATER_BUBBLER_2,
    WATER_BUBBLE_SPAWNER,
    WATER_SPLASH,
    WATER_BUBBLE,
    WATER_COUNTDOWNBUBBLE,
}WaterTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 waterLevel;
    int32 newWaterLevel;
    int32 targetWaterLevel;
    int32 waterMoveSpeed;
    int32 array3[4];
    int32 array4[4];
    int32 array5[4];
    TABLE(int32 bubbleSizes[18], { 2, 4, 2, 2, 2, 2, 4, 2, 4, 2, 2, 4, 2, 4, 2, 2, 4, 2 });
    uint16 spriteIndex;
    uint16 bigBubbleSprite;
    uint16 wakeSprite;
    Hitbox hitbox2;
    Hitbox hitbox;
    uint16 sfx_Splash;
    uint16 sfx_Breathe;
    uint16 sfx_Warning;
    uint16 sfx_DrownAlert;
    uint16 sfx_Drown;
    uint16 sfx_Skim;
    uint16 sfx_DNAGrab;
    uint16 sfx_DNABurst;
    uint16 sfx_WaterLevelL;
    uint16 sfx_WaterLevelR;
    int32 waterLevelChannel_L;
    int32 waterLevelChannel_R;
    int32 field_C0;
    bool32 playingWaterLevelSFX;
    int32 field_C8;
    int32 waterLevelVolume;
    int32 waterPalette;
    int32 field_D4;
    int32 wakePosX[4];
    uint8 wakeDir[4];
    Animator wakeData;
    int32 field_104;
    bool32 playingSkimSFX;
} ObjectWater;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    void *childPtr;
    int32 field_68;
    int8 bubbleType1;
    uint8 field_6D;
    uint8 field_6E;
    int8 bubbleType2;
    int32 numDuds;
    int32 countdownID;
    int32 field_78;
    Vector2 size;
    Vector2 height;
    int32 speed;
    int32 buttonTag;
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 priority;
    bool32 destroyOnTrigger;
    Hitbox hitbox;
    int32 gapA4;
    int32 field_A8;
    int32 timer;
    uint8 activePlayers;
    uint8 activePlayers2;
    int32 field_B4;
    int32 field_B8;
    int32 field_BC;
    int32 field_C0;
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
#if RETRO_INCLUDE_EDITOR
void Water_EditorDraw(void);
void Water_EditorLoad(void);
#endif
void Water_Serialize(void);

// Extra Entity Functions

//Palette stuff
void Water_SetWaterLevel(void);
void Water_RemoveWaterEffect(void);

//Utils
void Water_CheckButtonTag(void);
void Water_SpawnBubble(EntityPlayer *player, int32 id);
void Water_SpawnCountDownBubble(EntityPlayer *player, int32 id, uint8 bubbleID);
EntityWater *Water_GetPlayerBubble(EntityPlayer *entityPtr);

void Water_State_Palette(void);
void Water_State_Tint(void);
void Water_State_Splash(void);
void Water_Unknown4(void);
void Water_HCZBubbleBurst(EntityWater *entity, bool32 jumpedOut);
void Water_State_Bubble(void);
void Water_Unknown6(void);
void Water_State_HCZBubble(void);
void Water_HCZBubbleSpawner(void);
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
