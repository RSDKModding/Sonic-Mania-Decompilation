#include "SonicMania.h"

#if RETRO_USE_PLUS
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
    self->position.x = ScreenInfo->centerX << 16;
    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, true);
        RSDK.DrawSprite(&self->animator2, NULL, true);
    }
}

void PBL_HUD_Create(void *data)
{
    RSDK_THIS(PBL_HUD);
    if (!SceneInfo->inEditor) {
        self->visible    = true;
        self->drawOrder  = 12;
        self->active     = ACTIVE_NORMAL;
        self->position.y = 0;
        self->scale.x    = 0x200;
        self->scale.y    = 0x200;
        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 0, &self->animator2, true, 1);
        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 1, &self->animator3, true, 0);
        RSDK.SetText(&self->text, "", 64);

        if (globals->gameMode == MODE_ENCORE)
            PBL_HUD_DisplayMessage(self, "BONUS STAGE! RESCUE YOUR BUDDIES!", 1);
        else
            PBL_HUD_DisplayMessage(self, "BONUS STAGE!", 1);
    }
}

void PBL_HUD_StageLoad(void) { PBL_HUD->aniFrames = RSDK.LoadSpriteAnimation("Pinball/Backglass.bin", SCOPE_STAGE); }

void PBL_HUD_DisplayMessage(EntityPBL_HUD *self, const char *message, int32 type)
{
    if (self->state != PBL_HUD_State_Message7 && self->state != PBL_HUD_Unknown12 && self->state != PBL_HUD_Unknown13) {
        RSDK.PrependText(&self->text, message);
        RSDK.SetSpriteString(PBL_HUD->aniFrames, 1, &self->text);
        self->stateDraw = PBL_HUD_StateDraw_Unknown2;
        switch (type) {
            case 0:
                self->offset.x = 0x410000;
                self->state    = PBL_HUD_State_Message0;
                self->timer    = 12 * self->text.textLength + 128;
                break;
            case 1:
                self->offset.x = 0x410000;
                self->state    = PBL_HUD_State_Message1;
                self->timer    = 6 * self->text.textLength + 64;
                break;
            case 2:
                self->state    = PBL_HUD_State_Message2;
                self->timer    = 12 * self->text.textLength + 128;
                self->offset.x = -0x410000 - 0xC0000 * self->text.textLength;
                break;
            case 3:
                self->state    = PBL_HUD_State_Message3;
                self->timer    = 6 * self->text.textLength + 64;
                self->offset.x = -0x410000 - 0xC0000 * self->text.textLength;
                break;
            case 4:
                self->offset.y   = 0x120000;
                self->timer      = -9;
                self->velocity.y = -0x20000;
                self->state      = PBL_HUD_State_Message4;
                self->offset.x   = -0x60000 * self->text.textLength;
                break;
            case 5:
                self->offset.y   = -0x120000;
                self->timer      = -9;
                self->velocity.y = 0x20000;
                self->state      = PBL_HUD_State_Message4;
                self->offset.x   = -0x60000 * self->text.textLength;
                break;
            case 6:
                self->state    = PBL_HUD_State_Message6;
                self->timer    = 60;
                self->offset.x = -0x60000 * self->text.textLength;
                break;
            case 7:
                self->state    = PBL_HUD_State_Message7;
                self->timer    = 60;
                self->offset.x = -0x60000 * self->text.textLength;
                break;
            default: break;
        }
    }
}

void PBL_HUD_State_Message0(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        self->timer--;
        if (!(self->timer & 1))
            self->offset.x -= 0x20000;
    }
}

void PBL_HUD_State_Message1(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        self->offset.x -= 0x20000;
        self->timer--;
    }
}
void PBL_HUD_State_Message2(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        self->timer--;
        if (!(self->timer & 1))
            self->offset.x += 0x20000;
    }
}

void PBL_HUD_State_Message3(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = 0;
        self->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        self->offset.x += 0x20000;
        self->timer--;
    }
}

void PBL_HUD_State_Message4(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer < 0 || self->timer > 60)
        self->offset.y += self->velocity.y;

    self->timer++;
    if (self->timer > 90) {
        self->timer     = 0;
        self->offset.y  = 0;
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
}

void PBL_HUD_State_Message6(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = StateMachine_None;
        self->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        self->timer--;
        if ((self->timer & 7) < 4)
            self->stateDraw = StateMachine_None;
        else
            self->stateDraw = PBL_HUD_StateDraw_Unknown2;
    }
}

void PBL_HUD_State_Message7(void)
{
    RSDK_THIS(PBL_HUD);

    if (self->timer <= 0) {
        self->state     = PBL_HUD_Unknown12;
        self->stateDraw = PBL_HUD_StateDraw_Unknown1;
        self->drawFX    = FX_SCALE;

        foreach_active(PBL_Crane, crane) { crane->state = PBL_Crane_State_CreatePrizes; }
    }
    else {
        self->timer--;
        if ((self->timer & 7) < 4)
            self->stateDraw = StateMachine_None;
        else
            self->stateDraw = PBL_HUD_StateDraw_Unknown2;
    }
}

void PBL_HUD_StateDraw_Unknown1(void)
{
    RSDK_THIS(PBL_HUD);
    RSDK.DrawSprite(&self->animator1, NULL, true);
}

void PBL_HUD_StateDraw_Unknown2(void)
{
    RSDK_THIS(PBL_HUD);
    RSDK.DrawSprite(&self->animator1, 0, true);
    RSDK.DrawSprite(&self->animator2, 0, true);

    Vector2 position;
    position.x = self->position.x;
    position.y = self->position.y;
    position.x += self->offset.x;
    position.y += self->offset.y;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, ScreenInfo->centerX - 55, 40, ScreenInfo->centerX + 56, 58);
    RSDK.DrawText(&self->animator3, &position, &self->text, 0, 0, 0, 1, 0, 0, true);
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo->width, ScreenInfo->height);
}

void PBL_HUD_StateDraw_Unknown3(void)
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
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += 0x320000;
    RSDK.DrawSprite(&self->animator1, NULL, true);
    RSDK.DrawSprite(&self->animator2, NULL, true);

    int32 div = 10;
    while (count > 0) {
        self->animator3.frameID = PBL_Setup->score / mult % div;
        RSDK.DrawSprite(&self->animator3, &drawPos, true);
        drawPos.x -= 0xC0000;
        mult *= 10;
        --count;
    }
    self->animator3.frameID = 0;
}

void PBL_HUD_Unknown12(void)
{
    RSDK_THIS(PBL_HUD);
    self->scale.x += 4;
    self->position.y -= 0x40000;
    if (self->position.y < -0x480000) {
        self->state   = StateMachine_None;
        self->visible = false;
    }
}

void PBL_HUD_Unknown13(void)
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
        self->stateDraw  = PBL_HUD_StateDraw_Unknown3;

        foreach_all(PBL_Player, player)
        {
            player->active     = ACTIVE_NORMAL;
            player->velocity.y = 0;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void PBL_HUD_EditorDraw(void) {}

void PBL_HUD_EditorLoad(void) {}
#endif

void PBL_HUD_Serialize(void) {}
#endif
