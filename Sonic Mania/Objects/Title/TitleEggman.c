// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TitleEggman Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectTitleEggman *TitleEggman;

void TitleEggman_Update(void)
{
    RSDK_THIS(TitleEggman);
    StateMachine_Run(self->state);
}

void TitleEggman_LateUpdate(void) {}

void TitleEggman_StaticUpdate(void) {}

void TitleEggman_Draw(void)
{
    RSDK_THIS(TitleEggman);
    if (self->state == TitleEggman_State_Dust) {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
    else {
        Vector2 drawPos;
        self->drawFX = FX_NONE;
        int32 angle      = RSDK.Sin256(2 * self->timer) >> 1;

        for (int32 i = 1; i < 5; ++i) {
            drawPos.x = (i << 8) * RSDK.Sin1024(angle) + self->position.x;
            drawPos.y = self->position.y + 0x80000 + (i << 8) * RSDK.Cos1024(angle);
            RSDK.DrawSprite(&self->animator3, &drawPos, false);
        }
        self->drawFX   = FX_ROTATE;
        drawPos.x        = 0x500 * RSDK.Sin1024(angle) + self->position.x;
        drawPos.y        = 0x500 * RSDK.Cos1024(angle) + self->position.y + 0x80000;
        self->rotation = -(angle >> 2);
        RSDK.DrawSprite(&self->animator4, &drawPos, false);

        self->drawFX = FX_FLIP;
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }
}

void TitleEggman_Create(void *data)
{
    RSDK_THIS(TitleEggman);
    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawOrder = 3;
        self->active    = ACTIVE_NORMAL;
        if (data) {
            RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 5, &self->animator1, true, 0);
            self->state = TitleEggman_State_Dust;
        }
        else {
            RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 2, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 3, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 4, &self->animator4, true, 0);
            self->startPos.x = self->position.x;
            self->startPos.y = self->position.y;
            self->velocity.x = 0x10000;
            self->state      = TitleEggman_State_Wait;
        }
    }
}

void TitleEggman_StageLoad(void)
{
    if (API.CheckDLC(DLC_PLUS))
        TitleEggman->aniFrames = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE);
}

void TitleEggman_State_Dust(void)
{
    RSDK_THIS(TitleEggman);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1)
        destroyEntity(self);
}
void TitleEggman_State_Wait(void)
{
    RSDK_THIS(TitleEggman);
    if (++self->timer == 120) {
        self->timer = 0;
        self->state = TitleEggman_State_Move;
    }
}
void TitleEggman_State_Move(void)
{
    RSDK_THIS(TitleEggman);
    self->position.x += self->velocity.x;
    self->startPos.y -= 0x6000;
    self->position.y = self->startPos.y + (RSDK.Sin256(4 * self->timer) << 10);
    ++self->timer;
    if (!(self->timer & 0xF))
        CREATE_ENTITY(TitleEggman, intToVoid(1), self->position.x - 0xE0000, self->position.y + 0x80000);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator4);
}

#if RETRO_INCLUDE_EDITOR
void TitleEggman_EditorDraw(void)
{
    RSDK_THIS(TitleEggman);
    RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 2, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 3, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(TitleEggman->aniFrames, 4, &self->animator4, true, 0);
    self->startPos.x = self->position.x;
    self->startPos.y = self->position.y;

    TitleEggman_Draw();
}

void TitleEggman_EditorLoad(void) { TitleEggman->aniFrames = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE); }
#endif

void TitleEggman_Serialize(void) {}
#endif
