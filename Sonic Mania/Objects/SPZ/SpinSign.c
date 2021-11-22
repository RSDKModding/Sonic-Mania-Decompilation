#include "SonicMania.h"

ObjectSpinSign *SpinSign;

void SpinSign_Update(void)
{
    RSDK_THIS(SpinSign);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &SpinSign->hitboxes[self->type])) {
            int32 vel = 0;
            if (self->type & 1)
                vel = player->velocity.x;
            else
                vel = player->velocity.y;

            int32 val = (vel >> 7) + (vel >> 6);
            if (abs(val) > self->timer && val >= 0xC00) {
                self->timer     = val & -0x80;
                self->active    = ACTIVE_NORMAL;
                self->state     = SpinSign_Unknown1;
                self->direction = vel < 0;
            }
        }
    }
    StateMachine_Run(self->state);
}

void SpinSign_LateUpdate(void) {}

void SpinSign_StaticUpdate(void) {}

void SpinSign_Draw(void)
{
    RSDK_THIS(SpinSign);
    StateMachine_Run(self->stateDraw);
}

void SpinSign_Create(void *data)
{
    RSDK_THIS(SpinSign);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SpinSign->aniFrames, 3, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &self->animator3, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawFX        = FX_SCALE;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;
        self->state         = SpinSign_Unknown1;
        switch (self->type) {
            case 0:
                self->animator3.frameID = 1;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &self->animator2, true, 0);
                self->stateDraw = SpinSign_Unknown3;
                break;
            case 1:
                self->animator3.frameID = 0;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &self->animator2, true, 0);
                self->stateDraw = SpinSign_Unknown4;
                break;
            case 2:
                self->animator3.frameID = 1;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &self->animator2, true, 0);
                self->stateDraw = SpinSign_Unknown5;
                break;
            case 3:
                self->animator3.frameID = 2;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &self->animator2, true, 0);
                self->stateDraw = SpinSign_Unknown6;
                break;
            default: break;
        }
    }
}

void SpinSign_StageLoad(void)
{
    SpinSign->aniFrames          = RSDK.LoadSpriteAnimation("SPZ1/SpinSign.bin", SCOPE_STAGE);
    SpinSign->hitboxes[0].left   = -24;
    SpinSign->hitboxes[0].top    = -6;
    SpinSign->hitboxes[0].right  = 24;
    SpinSign->hitboxes[0].bottom = 6;
    SpinSign->hitboxes[1].left   = -6;
    SpinSign->hitboxes[1].top    = -24;
    SpinSign->hitboxes[1].right  = 6;
    SpinSign->hitboxes[1].bottom = 24;
    SpinSign->hitboxes[2].left   = -24;
    SpinSign->hitboxes[2].top    = -6;
    SpinSign->hitboxes[2].right  = 24;
    SpinSign->hitboxes[2].bottom = 6;
    SpinSign->hitboxes[3].left   = -6;
    SpinSign->hitboxes[3].top    = -40;
    SpinSign->hitboxes[3].right  = 6;
    SpinSign->hitboxes[3].bottom = 40;
    DEBUGMODE_ADD_OBJ(SpinSign);
    SpinSign->sfxSignPost = RSDK.GetSfx("Global/SignPost.wav");
}

void SpinSign_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(SpinSign, NULL, self->position.x, self->position.y);
}
void SpinSign_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 3, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &DebugMode->animator, true, 2);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void SpinSign_Unknown1(void)
{
    RSDK_THIS(SpinSign);

    if (self->timer) {
        if (self->direction)
            self->angle -= self->timer;
        else
            self->angle += self->timer;

        self->timer -= 0x40;
        if (self->timer < 256) {
            if (self->direction) {
                if (self->angle < 0x4000)
                    self->direction ^= FLIP_X;
            }
            else if (self->angle > 0xC000) {
                self->direction = FLIP_X;
            }
            self->timer = 0;
            self->state = SpinSign_Unknown2;
        }
    }
    self->rotation = (self->angle >> 8) & 0x1FF;
}
void SpinSign_Unknown2(void)
{
    RSDK_THIS(SpinSign);

    int32 ang = self->angle & 0xFFFF0000;
    if (self->direction)
        self->angle -= self->timer;
    else
        self->angle += self->timer;

    self->timer -= 0x20;
    if ((self->angle & 0xFFFF0000) != ang) {
        self->active = ACTIVE_BOUNDS;
        self->timer  = 0;
        self->state  = SpinSign_Unknown1;
    }
    self->rotation = (self->angle >> 8) & 0x1FF;
}

void SpinSign_Unknown3(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    self->drawFX = FX_SCALE;
    drawPos.x      = self->position.x;

    Animator *animator = NULL;
    if (self->rotation <= 128 || self->rotation >= 384) {
        animator = &self->animator1;
    }
    else {
        animator = &self->animator2;
    }
    animator->frameID = (Zone->timer >> 4) & 1;

    self->scale.y = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scale       = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.y = self->position.y + (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.y += -0xF00 * self->scale.y - (scale << 9);
            break;
        case 1:
        case 3:
            drawPos.y = self->position.y - (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.y += ((scale - 64) << 9) + 0xF00 * self->scale.y;
            break;
        default: break;
    }
    self->scale.y           = scale;
    self->animator3.frameID = (animator->frameID + 2) ^ (self->rotation < 256);
    RSDK.DrawSprite(&self->animator3, &drawPos, false);
}
void SpinSign_Unknown4(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.y      = self->position.y;
    self->drawFX = FX_SCALE;

    Animator *animator = NULL;
    if (self->rotation <= 128 || self->rotation >= 384)
        animator = &self->animator1;
    else
        animator = &self->animator2;

    animator->frameID = (Zone->timer >> 4) & 1;

    self->scale.x   = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scale = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = self->position.x + (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.x += -0xF00 * self->scale.x - (scale << 9);
            break;
        case 1:
        case 3:
            drawPos.x = self->position.x - (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.x += ((scale - 64) << 9) + 0xF00 * self->scale.x;
            break;
        default: break;
    }
    self->scale.x           = scale;
    self->animator3.frameID = animator->frameID ^ (self->rotation < 256);
    RSDK.DrawSprite(&self->animator3, &drawPos, false);
}
void SpinSign_Unknown5(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.x                 = self->position.x;
    self->drawFX            = FX_SCALE;
    self->animator2.frameID = self->rotation <= 128 || self->rotation >= 384;

    self->scale.y = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scale       = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.y = self->position.y + (scale << 9);
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
            drawPos.y += -0x1400 * self->scale.y - (scale << 9);
            break;
        case 1:
        case 3:
            drawPos.y = self->position.y - (scale << 9);
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
            drawPos.y += (scale + 2 * (5 * self->scale.y - 32)) << 9;
            break;
        default: break;
    }
    self->scale.y           = scale;
    self->animator3.frameID = self->animator2.frameID + 6;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);
}
void SpinSign_Unknown6(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.y                 = self->position.y;
    self->drawFX            = FX_SCALE;
    self->animator2.frameID = self->rotation <= 128 || self->rotation >= 384;

    self->scale.x = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scale       = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = self->position.x + (scale << 9);
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
            drawPos.x += -0xC00 * self->scale.x - (scale << 9);
            break;
        case 1:
        case 3:
            drawPos.x = self->position.x - (scale << 9);
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
            drawPos.x += (scale + 2 * (3 * self->scale.x - 32)) << 9;
            break;
        default: break;
    }
    self->scale.x           = scale;
    self->animator3.frameID = self->animator2.frameID + 4;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);
}

#if RETRO_INCLUDE_EDITOR
void SpinSign_EditorDraw(void)
{
    RSDK_THIS(SpinSign);

    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 3, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &self->animator3, true, 0);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->scale.x       = 0x200;
    self->scale.y       = 0x200;

    switch (self->type) {
        case 0:
            self->animator3.frameID = 1;
            RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &self->animator2, true, 0);
            self->stateDraw = SpinSign_Unknown3;
            break;
        case 1:
            self->animator3.frameID = 0;
            RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &self->animator2, true, 0);
            self->stateDraw = SpinSign_Unknown4;
            break;
        case 2:
            self->animator3.frameID = 1;
            RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &self->animator2, true, 0);
            self->stateDraw = SpinSign_Unknown5;
            break;
        case 3:
            self->animator3.frameID = 2;
            RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &self->animator2, true, 0);
            self->stateDraw = SpinSign_Unknown6;
            break;
        default: break;
    }

    StateMachine_Run(self->stateDraw);
}

void SpinSign_EditorLoad(void) { SpinSign->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/SpinSign.bin", SCOPE_STAGE); }
#endif

void SpinSign_Serialize(void) { RSDK_EDITABLE_VAR(SpinSign, VAR_UINT8, type); }
