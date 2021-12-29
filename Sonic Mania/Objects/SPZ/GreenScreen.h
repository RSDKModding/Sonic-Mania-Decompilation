#ifndef OBJ_GREENSCREEN_H
#define OBJ_GREENSCREEN_H

#include "SonicMania.h"

// Object Class
struct ObjectGreenScreen {
    RSDK_OBJECT
    Hitbox hitbox;
    int32 field_C[5];
    int32 field_20[5];
    uint16 aniFrames;
    bool32 greenScreenActive;
};

// Entity Class
struct EntityGreenScreen {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 paraOffset;
    int32 paraYFactor;
    bool32 paraPreview;
    bool32 field_6C;
    int32 field_70;
    int32 field_74;
    Vector2 startPos;
    int32 field_80;
    Animator animator;
};

// Object Struct
extern ObjectGreenScreen *GreenScreen;

// Standard Entity Events
void GreenScreen_Update(void);
void GreenScreen_LateUpdate(void);
void GreenScreen_StaticUpdate(void);
void GreenScreen_Draw(void);
void GreenScreen_Create(void* data);
void GreenScreen_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GreenScreen_EditorDraw(void);
void GreenScreen_EditorLoad(void);
#endif
void GreenScreen_Serialize(void);

// Extra Entity Functions
void GreenScreen_Unknown1(int fieldC, uint8 id, int a3, int a4);
void GreenScreen_Unknown2(void);
void GreenScreen_DrawSprites(void);

#endif //!OBJ_GREENSCREEN_H
