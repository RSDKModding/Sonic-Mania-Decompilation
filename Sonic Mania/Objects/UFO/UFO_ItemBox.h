#ifndef OBJ_UFO_ITEMBOX_H
#define OBJ_UFO_ITEMBOX_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int breakCount;
    int field_8;
    int field_C;
    uint16 itemBoxSprite;
    uint16 itemBoxModel;
    uint16 sceneIndex;
    uint16 sfx_Destroy;
    uint16 sfx_RockemSockem;
    uint16 sfx_Bumper;
} ObjectUFO_ItemBox;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    int timer;
    int height;
    int timer2;
    int worldX;
    int worldY;
    Vector2 drawPos;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    int sfxTimer;
    Animator itemData;
    int field_198;
    int field_19C;
    int field_1A0;
    int field_1A4;
    int field_1A8;
    int field_1AC;
} EntityUFO_ItemBox;

// Object Struct
extern ObjectUFO_ItemBox *UFO_ItemBox;

// Standard Entity Events
void UFO_ItemBox_Update(void);
void UFO_ItemBox_LateUpdate(void);
void UFO_ItemBox_StaticUpdate(void);
void UFO_ItemBox_Draw(void);
void UFO_ItemBox_Create(void* data);
void UFO_ItemBox_StageLoad(void);
void UFO_ItemBox_EditorDraw(void);
void UFO_ItemBox_EditorLoad(void);
void UFO_ItemBox_Serialize(void);

// Extra Entity Functions
void UFO_ItemBox_Unknown1(void);
void UFO_ItemBox_Unknown2(void);

#endif //!OBJ_UFO_ITEMBOX_H
