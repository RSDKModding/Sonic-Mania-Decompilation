#include "SonicMania.h"

ObjectHiLoSign *HiLoSign;

void HiLoSign_Update(void)
{
    RSDK_THIS(HiLoSign);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &HiLoSign->hitboxes[self->type])) {
            int velocity = 0;
            if (self->type & 1)
                velocity = player->velocity.x;
            else
                velocity = player->velocity.y;

            int speed = abs((velocity >> 7) + (velocity >> 6));
            if (speed > self->spinSpeed && speed >= 0xC00) {
                self->spinSpeed = speed & 0xFFFFFF80;
                self->active    = ACTIVE_NORMAL;
                self->state     = HiLoSign_State_Spinning;
                self->direction = velocity < 0;
            }
        }
    }
    StateMachine_Run(self->state);
}

void HiLoSign_LateUpdate(void) {}

void HiLoSign_StaticUpdate(void) {}

void HiLoSign_Draw(void)
{
    RSDK_THIS(HiLoSign);
    StateMachine_Run(self->stateDraw);
}

void HiLoSign_Create(void *data)
{
    RSDK_THIS(HiLoSign);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(HiLoSign->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(HiLoSign->aniFrames, 1, &self->animator2, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawFX        = FX_SCALE;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;
        self->state         = HiLoSign_State_Spinning;

        switch (self->type) {
            case 0:
                self->animator2.frameID = 1;
                self->stateDraw         = HiLoSign_StateDraw_Horizontal;
                break;
            case 1: self->stateDraw = HiLoSign_StateDraw_Vertical; break;
        }
    }
}

void HiLoSign_StageLoad(void)
{
    HiLoSign->aniFrames          = RSDK.LoadSpriteAnimation("SSZ/HiLoSign.bin", SCOPE_STAGE);
    HiLoSign->hitboxes[0].left   = -24;
    HiLoSign->hitboxes[0].top    = -6;
    HiLoSign->hitboxes[0].right  = 24;
    HiLoSign->hitboxes[0].bottom = 6;
    HiLoSign->hitboxes[1].left   = -6;
    HiLoSign->hitboxes[1].top    = -24;
    HiLoSign->hitboxes[1].right  = 6;
    HiLoSign->hitboxes[1].bottom = 24;
    HiLoSign->sfxSignPost = RSDK.GetSFX("Global/SignPost.wav");
    DEBUGMODE_ADD_OBJ(HiLoSign);
}

void HiLoSign_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(HiLoSign, NULL, self->position.x, self->position.y);
}

void HiLoSign_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(HiLoSign->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void HiLoSign_State_Spinning(void)
{
    RSDK_THIS(HiLoSign);

    if (self->spinSpeed) {
        if (self->direction)
            self->angle -= self->spinSpeed;
        else
            self->angle += self->spinSpeed;

        self->spinSpeed -= 64;
        if (self->spinSpeed < 256) {
            if (self->direction) {
                if ((self->angle & 0xFFFF) < 0x4000)
                    self->direction ^= 1;
            }
            else if ((self->angle & 0xFFFF) > 0xC000) {
                self->direction = FLIP_X;
            }
            self->spinSpeed = 0;
            self->state    = HiLoSign_State_Spinning2;
        }
    }

    self->rotation = (self->angle >> 8) & 0x1FF;
}

void HiLoSign_State_Spinning2(void)
{
    RSDK_THIS(HiLoSign);

    int prevAng = self->angle & 0xFFFF0000;
    if (self->direction)
        self->angle -= self->spinSpeed;
    else
        self->angle += self->spinSpeed;

    self->spinSpeed -= 32;
    if ((self->angle & 0xFFFF0000) != prevAng) {
        self->active   = ACTIVE_BOUNDS;
        self->spinSpeed = 0;
        self->state    = HiLoSign_State_Spinning;
    }
    self->rotation = (self->angle >> 8) & 0x1FF;
}

void HiLoSign_StateDraw_Horizontal(void)
{
    RSDK_THIS(HiLoSign);
    Vector2 drawPos;

    drawPos.x       = self->position.x;
    self->drawFX  = FX_SCALE;
    self->scale.y = abs(RSDK.Cos512(self->rotation)) + 1;
    int scaleY      = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.y = self->position.y + (scaleY << 9);
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
            drawPos.y += -2048 * self->scale.y - (scaleY << 9);
            break;
        case 1:
        case 3:
            drawPos.y = self->position.y - (scaleY << 9);
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
            drawPos.y += (scaleY + 4 * (self->scale.y - 16)) << 9;
            break;
        default: break;
    }
    self->scale.y = scaleY;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
}

void HiLoSign_StateDraw_Vertical(void)
{
    RSDK_THIS(HiLoSign);
    Vector2 drawPos;

    drawPos.y       = self->position.y;
    self->drawFX  = FX_SCALE;
    self->scale.x = abs(RSDK.Cos512(self->rotation)) + 1;
    int scaleX      = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = self->position.x + (scaleX << 9);
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
            drawPos.x += -0xF00 * self->scale.x - (scaleX << 9);
            break;
        case 1:
        case 3:
            drawPos.x = self->position.x - (scaleX << 9);
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
            drawPos.x += ((scaleX - 64) << 9) + 0xF00 * self->scale.x;
            break;
        default: break;
    }
    self->scale.x = scaleX;

    RSDK.DrawSprite(&self->animator2, &drawPos, false);
}

void HiLoSign_EditorDraw(void)
{
    RSDK_THIS(HiLoSign);
    RSDK.SetSpriteAnimation(HiLoSign->aniFrames, 0, &self->animator1, true, 0);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void HiLoSign_EditorLoad(void) { HiLoSign->aniFrames = RSDK.LoadSpriteAnimation("SSZ/HiLoSign.bin", SCOPE_STAGE); }

void HiLoSign_Serialize(void) { RSDK_EDITABLE_VAR(HiLoSign, VAR_UINT8, type); }
