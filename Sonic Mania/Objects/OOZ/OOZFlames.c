#include "SonicMania.h"

ObjectOOZFlames *OOZFlames;

void OOZFlames_Update(void)
{
    RSDK_THIS(OOZFlames);
    RSDK.ProcessAnimation(&self->animator);
    StateMachine_Run(self->state);
}

void OOZFlames_LateUpdate(void) {}

void OOZFlames_StaticUpdate(void) {}

void OOZFlames_Draw(void)
{
    RSDK_THIS(OOZFlames);
    Vector2 drawPos;

    drawPos.x = (ScreenInfo[SceneInfo->currentScreenID].centerX - 240) << 16;
    for (int32 angle = 0; angle < 0x100; angle += 0x10) {
        drawPos.y = (RSDK.Sin256(4 * (angle + Zone->timer)) << 11) + self->field_60;
        RSDK.DrawSprite(&self->animator, &drawPos, true);
        drawPos.x += 0x200000;
    }
}

void OOZFlames_Create(void *data)
{
    RSDK_THIS(OOZFlames);
    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawOrder = Zone->drawOrderHigh;
        self->active    = ACTIVE_NORMAL;
        self->drawFX    = FX_SCALE;
        self->inkEffect = INK_ADD;
        self->alpha     = 256;
        self->scale.x   = 512;
        self->scale.y   = 512;
        self->field_60  = 0x1400000;
        self->state     = OOZFlames_Unknown1;
        RSDK.SetSpriteAnimation(OOZFlames->aniFrames, 0, &self->animator, true, 0);
    }
}

void OOZFlames_StageLoad(void) { OOZFlames->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Flames.bin", SCOPE_STAGE); }

void OOZFlames_Unknown1(void)
{
    RSDK_THIS(OOZFlames);

    if (self->field_60 > 0xE00000) {
        self->field_60 -= 0x20000;
    }
    if (++self->timer == 120) {
        self->timer = 0;
        self->state = OOZFlames_Unknown2;
    }
}

void OOZFlames_Unknown2(void)
{
    RSDK_THIS(OOZFlames);

    if (self->scale.y < 4096) {
        self->scale.y += 16;
    }
}

#if RETRO_INCLUDE_EDITOR
void OOZFlames_EditorDraw(void) {}

void OOZFlames_EditorLoad(void) {}
#endif

void OOZFlames_Serialize(void) {}
