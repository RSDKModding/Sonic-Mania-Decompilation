#ifndef OBJ_UFO_ITEMBOX_H
#define OBJ_UFO_ITEMBOX_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 breakCount;
    int32 field_8;
    int32 field_C;
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
    int32 type;
    int32 timer;
    int32 height;
    int32 timer2;
    int32 worldX;
    int32 worldY;
    Vector2 drawPos;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
    int32 field_A4;
    int32 field_A8;
    int32 field_AC;
    int32 field_B0;
    int32 field_B4;
    int32 field_B8;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    int32 sfxTimer;
    Animator itemData;
    int32 field_198;
    int32 field_19C;
    int32 field_1A0;
    int32 field_1A4;
    int32 field_1A8;
    int32 field_1AC;
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
