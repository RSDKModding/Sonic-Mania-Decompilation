// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_HUD Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPBL_HUD *PBL_HUD;

void PBL_HUD_Update(void)
{
    RSDK_THIS(PBL_HUD);

    StateMachine_Run(self->state);
}

void PBL_HUD_LateUpdate(void) {}

void PBL_HUD_StaticUpdate(void) {}

void PBL_HUD_Draw(void)
{
    RSDK_THIS(PBL_HUD);

    self->position.x = ScreenInfo->center.x << 16;

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->displayAnimator, NULL, true);
        RSDK.DrawSprite(&self->baseAnimator, NULL, true);
    }
}

void PBL_HUD_Create(void *data)
{
    RSDK_THIS(PBL_HUD);

    if (!SceneInfo->inEditor) {
        self->visible    = true;
        self->drawGroup  = 12;
        self->active     = ACTIVE_NORMAL;
        self->position.y = 0;
        self->scale.x    = 0x200;
        self->scale.y    = 0x200;

        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 0, &self->displayAnimator, true, 0);
        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 0, &self->baseAnimator, true, 1);
        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 1, &self->textAnimator, true, 0);

        RSDK.InitString(&self->text, "", 64);

        if (globals->gameMode == MODE_ENCORE)
            PBL_HUD_DisplayMessage(self, "BONUS STAGE! RESCUE YOUR BUDDIES!", PBL_HUD_MSG_SCROLL_LEFT);
        else
            PBL_HUD_DisplayMessage(self, "BONUS STAGE!", PBL_HUD_MSG_SCROLL_LEFT);
    }
}

void PBL_HUD_StageLoad(void) { PBL_HUD->aniFrames = RSDK.LoadSpriteAnimation("Pinball/Backglass.bin", SCOPE_STAGE); }

void PBL_HUD_DisplayMessage(EntityPBL_HUD *entity, const char *message, int32 type)
{
    if (entity->state != PBL_HUD_StateMessage_FlashThenCrane && entity->state != PBL_HUD_State_RevealCrane
        && entity->state != PBL_HUD_State_HideCrane) {
        RSDK.SetString(&entity->text, message);
        RSDK.SetSpriteString(PBL_HUD->aniFrames, 1, &entity->text);

        entity->stateDraw = PBL_HUD_Draw_Message;

        switch (type) {
            case PBL_HUD_MSG_SCROLL_LEFT_SLOW:
                entity->offset.x = 0x410000;
                entity->state    = PBL_HUD_StateMessage_ScrollLeftSlow;
                entity->timer    = 12 * entity->text.length + 128;
                break;

            case PBL_HUD_MSG_SCROLL_LEFT:
                entity->offset.x = 0x410000;
                entity->state    = PBL_HUD_StateMessage_ScrollLeftFast;
                entity->timer    = 6 * entity->text.length + 64;
                break;

            case PBL_HUD_MSG_SCROLL_RIGHT_SLOW:
                entity->state    = PBL_HUD_StateMessage_ScrollRightSlow;
                entity->timer    = 12 * entity->text.length + 128;
                entity->offset.x = -0x410000 - 0xC0000 * entity->text.length;
                break;

            case PBL_HUD_MSG_SCROLL_RIGHT:
                entity->state    = PBL_HUD_StateMessage_ScrollRightFast;
                entity->timer    = 6 * entity->text.length + 64;
                entity->offset.x = -0x410000 - 0xC0000 * entity->text.length;
                break;

            case PBL_HUD_MSG_SHOW_RISE:
                entity->offset.y   = 0x120000;
                entity->timer      = -9;
                entity->velocity.y = -0x20000;
                entity->state      = PBL_HUD_StateMessage_ShowMove;
                entity->offset.x   = -0x60000 * entity->text.length;
                break;

            case PBL_HUD_MSG_SHOW_FALL:
                entity->offset.y   = -0x120000;
                entity->timer      = -9;
                entity->velocity.y = 0x20000;
                entity->state      = PBL_HUD_StateMessage_ShowMove;
                entity->offset.x   = -0x60000 * entity->text.length;
                break;

            case PBL_HUD_MSG_FLASH:
                entity->state    = PBL_HUD_StateMessage_Flash;
                entity->timer    = 60;
                entity->offset.x = -0x60000 * entity->text.length;
                break;

            case PBL_HUD_MSG_FLASH_CRANE:
                entity->state    = PBL_HUD_StateMessage_FlashThenCrane;
                entity->timer    = 60;
                entity->offset.x = -0x60000 * entity->text.length;
                break;

            default: break;
        }
    }
}

void PBL_HUD_StateMessage_ScrollLeftSlow(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_Draw_Score;
    }
    else {
        self->timer--;
        if (!(self->timer & 1))
            self->offset.x -= 0x20000;
    }
}

void PBL_HUD_StateMessage_ScrollLeftFast(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_Draw_Score;
    }
    else {
        self->offset.x -= 0x20000;
        self->timer--;
    }
}
void PBL_HUD_StateMessage_ScrollRightSlow(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_Draw_Score;
    }
    else {
        self->timer--;

        if (!(self->timer & 1))
            self->offset.x += 0x20000;
    }
}

void PBL_HUD_StateMessage_ScrollRightFast(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_Draw_Score;
    }
    else {
        self->offset.x += 0x20000;
        self->timer--;
    }
}

void PBL_HUD_StateMessage_ShowMove(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer < 0 || self->timer > 60)
        self->offset.y += self->velocity.y;

    if (++self->timer > 90) {
        self->timer     = 0;
        self->offset.y  = 0;
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_Draw_Score;
    }
}

void PBL_HUD_StateMessage_Flash(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_Draw_Score;
    }
    else {
        self->timer--;

        if ((self->timer & 7) < 4)
            self->stateDraw = StateMachine_None;
        else
            self->stateDraw = PBL_HUD_Draw_Message;
    }
}

void PBL_HUD_StateMessage_FlashThenCrane(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = PBL_HUD_State_RevealCrane;
        self->stateDraw = PBL_HUD_Draw_Basic;
        self->drawFX    = FX_SCALE;

        foreach_active(PBL_Crane, crane) { crane->state = PBL_Crane_State_CreatePrizes; }
    }
    else {
        self->timer--;

        if ((self->timer & 7) < 4)
            self->stateDraw = StateMachine_None;
        else
            self->stateDraw = PBL_HUD_Draw_Message;
    }
}

void PBL_HUD_Draw_Basic(void)
{
    RSDK_THIS(PBL_HUD);

    RSDK.DrawSprite(&self->displayAnimator, NULL, true);
}

void PBL_HUD_Draw_Message(void)
{
    RSDK_THIS(PBL_HUD);

    RSDK.DrawSprite(&self->displayAnimator, NULL, true);
    RSDK.DrawSprite(&self->baseAnimator, NULL, true);

    Vector2 position;
    position.x = self->position.x + self->offset.x;
    position.y = self->position.y + self->offset.y;

    RSDK.SetClipBounds(SceneInfo->currentScreenID, ScreenInfo->center.x - 55, 40, ScreenInfo->center.x + 56, 58);

    RSDK.DrawText(&self->textAnimator, &position, &self->text, 0, 0, 0, 1, 0, 0, true);

    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
}

void PBL_HUD_Draw_Score(void)
{
    RSDK_THIS(PBL_HUD);

    int32 score = PBL_Setup->score;
    int32 count = 0;
    int32 mult  = 1;

    while (score > 0) {
        ++count;
        score /= 10;
    }

    Vector2 drawPos;
    drawPos.x = self->position.x + 0x320000;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->displayAnimator, NULL, true);
    RSDK.DrawSprite(&self->baseAnimator, NULL, true);

    int32 div = 10;
    while (count > 0) {
        self->textAnimator.frameID = PBL_Setup->score / mult % div;
        RSDK.DrawSprite(&self->textAnimator, &drawPos, true);

        drawPos.x -= 0xC0000;
        mult *= 10;
        --count;
    }

    self->textAnimator.frameID = 0;
}

void PBL_HUD_State_RevealCrane(void)
{
    RSDK_THIS(PBL_HUD);

    self->scale.x += 4;
    self->position.y -= 0x40000;

    if (self->position.y < -0x480000) {
        self->state   = StateMachine_None;
        self->visible = false;
    }
}

void PBL_HUD_State_HideCrane(void)
{
    RSDK_THIS(PBL_HUD);

    self->scale.x -= 4;
    self->position.y += 0x40000;

    if (self->position.y >= 0) {
        self->position.y = 0;
        self->scale.x    = 0x200;
        self->scale.y    = 0x200;
        self->drawFX     = FX_NONE;
        self->state      = StateMachine_None;
        self->stateDraw  = PBL_HUD_Draw_Score;

        foreach_all(PBL_Player, player)
        {
            player->active     = ACTIVE_NORMAL;
            player->velocity.y = 0;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void PBL_HUD_EditorDraw(void)
{
    RSDK_THIS(PBL_HUD);

    RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 0, &self->displayAnimator, true, 0);
    RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 0, &self->baseAnimator, true, 1);

    RSDK.DrawSprite(&self->displayAnimator, NULL, false);
}

void PBL_HUD_EditorLoad(void) { PBL_HUD->aniFrames = RSDK.LoadSpriteAnimation("Pinball/Backglass.bin", SCOPE_STAGE); }
#endif

void PBL_HUD_Serialize(void) {}
#endif
