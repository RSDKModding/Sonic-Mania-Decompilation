#ifndef OBJ_TWISTINGSLIDE_H
#define OBJ_TWISTINGSLIDE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
} ObjectTwistingSlide;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 type;
    int endLen;
    int activePlayers;
    int field_64[4];
    int field_74;
    int field_78;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox1;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
} EntityTwistingSlide;

// Object Struct
extern ObjectTwistingSlide *TwistingSlide;

// Standard Entity Events
void TwistingSlide_Update(void);
void TwistingSlide_LateUpdate(void);
void TwistingSlide_StaticUpdate(void);
void TwistingSlide_Draw(void);
void TwistingSlide_Create(void* data);
void TwistingSlide_StageLoad(void);
void TwistingSlide_EditorDraw(void);
void TwistingSlide_EditorLoad(void);
void TwistingSlide_Serialize(void);

// Extra Entity Functions
void TwistingSlide_Unknown1(void);


#endif //!OBJ_TWISTINGSLIDE_H
