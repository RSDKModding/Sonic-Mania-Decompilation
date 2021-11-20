#include "SonicMania.h"

ObjectBuzzSaw *BuzzSaw;

void BuzzSaw_Update(void)
{
    RSDK_THIS(BuzzSaw);
    StateMachine_Run(self->state);
}

void BuzzSaw_LateUpdate(void) {}

void BuzzSaw_StaticUpdate(void) {}

void BuzzSaw_Draw(void)
{
    RSDK_THIS(BuzzSaw);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void BuzzSaw_Create(void *data)
{
    RSDK_THIS(BuzzSaw);

    self->drawFX = FX_ROTATE;
    if (!self->type)
        self->rotation = self->angle;
    else
        self->inkEffect = INK_ADD;

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(BuzzSaw->aniFrames, self->type, &self->animator, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = (abs(self->speed * self->amplitude.x) + 64) << 17;
        self->updateRange.y = (abs(self->speed * self->amplitude.y) + 64) << 17;
        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;
        self->visible   = true;
        self->drawOrder = Zone->drawOrderLow;
        self->startPos  = self->position;

        if (self->type == 0)
            self->state = BuzzSaw_State_OnArm;
        else
            self->state = BuzzSaw_State_FreeMove_Waiting;
    }
}

void BuzzSaw_StageLoad(void)
{
    BuzzSaw->aniFrames     = RSDK.LoadSpriteAnimation("MMZ/BuzzSaw.bin", SCOPE_STAGE);
    BuzzSaw->field_10      = 0x200000;
    BuzzSaw->field_14      = 0x200000;
    BuzzSaw->hitbox.left   = -24;
    BuzzSaw->hitbox.top    = -24;
    BuzzSaw->hitbox.right  = 24;
    BuzzSaw->hitbox.bottom = 24;

    Soundboard_LoadSFX("MMZ/SawSus.wav", true, BuzzSaw_CheckCB, BuzzSaw_UpdateCB);
}

bool32 BuzzSaw_CheckCB(void)
{
    int32 worldX = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    int32 worldY = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;

    int32 count = 0;

    foreach_all(BuzzSaw, saw)
    {
        int32 x    = abs(worldX - saw->position.x);
        int32 y    = abs(worldY - saw->position.y);
        int32 dist = MathHelpers_SquareRoot((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) << 16;
        if (dist > 0x2800000)
            count++;
    }

    return count > 0;
}

void BuzzSaw_UpdateCB(int32 sfx)
{
    int32 dist   = 0x7FFF0000;
    int32 worldX = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    int32 worldY = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;

    foreach_all(BuzzSaw, saw)
    {
        int32 x = abs(worldX - saw->position.x);
        int32 y = abs(worldY - saw->position.y);
        dist  = minVal(MathHelpers_SquareRoot((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) << 16, dist);
    }

    RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfx], 1.0 - (minVal(dist >> 16, 640) / 640.0), 0.0, 1.0);
}

void BuzzSaw_CheckPlayerCollisions(void)
{
    RSDK_THIS(BuzzSaw);
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &BuzzSaw->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x600, player, false, &player->uncurlTimer))
#endif
            Player_CheckHit(player, self);
        }
    }
    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

void BuzzSaw_State_OnArm(void)
{
    RSDK_THIS(BuzzSaw);
    self->drawPos.x = self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer) + self->startPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer) + self->startPos.y;
    RSDK.ProcessAnimation(&self->animator);
    BuzzSaw_CheckPlayerCollisions();
}

void BuzzSaw_State_FreeMove_Waiting(void)
{
    RSDK_THIS(BuzzSaw);

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;

    foreach_active(Player, player)
    {
        int32 angle = RSDK.ATan2((player->position.x - self->position.x) >> 16, (player->position.y - self->position.y) >> 16);

        int32 rx = (abs(player->position.x - self->position.x) >> 16) * (abs(player->position.x - self->position.x) >> 16);
        int32 ry = (abs(player->position.y - self->position.y) >> 16) * (abs(player->position.y - self->position.y) >> 16);
        if (angle + 32 - (self->angle & 0xFF) < 0x40 && (rx + ry) - 0x4000 < 0x5000) {
            self->active     = ACTIVE_NORMAL;
            self->velocity.x = 0x600 * RSDK.Cos256(self->angle);
            self->velocity.y = 0x600 * RSDK.Sin256(self->angle);
            self->state      = BuzzSaw_State_FreeMove_Released;
        }
    }
}

void BuzzSaw_State_FreeMove_Released(void)
{
    RSDK_THIS(BuzzSaw);

    if (self->alpha < 256)
        self->alpha += 16;
    self->drawPos.x += self->velocity.x;
    self->drawPos.y += self->velocity.y;
    RSDK.ProcessAnimation(&self->animator);
    BuzzSaw_CheckPlayerCollisions();
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->drawPos.x  = self->startPos.x;
        self->drawPos.y  = self->startPos.y;
        self->alpha      = 0;
        self->state      = BuzzSaw_State_FreeMove_Reset;
    }
    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

void BuzzSaw_State_FreeMove_Reset(void)
{
    RSDK_THIS(BuzzSaw);
    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->state = BuzzSaw_State_FreeMove_Waiting;
        BuzzSaw_Create(NULL);
    }
}

#if RETRO_INCLUDE_EDITOR
void BuzzSaw_EditorDraw(void)
{
    RSDK_THIS(BuzzSaw);

    self->drawFX    = FX_ROTATE;
    self->inkEffect = INK_NONE;
    self->rotation  = 0;
    if (!self->type)
        self->rotation = self->angle;
    else
        self->inkEffect = INK_ADD;

    RSDK.SetSpriteAnimation(BuzzSaw->aniFrames, self->type, &self->animator, true, 0);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = (abs(self->speed * self->amplitude.x) + 64) << 17;
    self->updateRange.y = (abs(self->speed * self->amplitude.y) + 64) << 17;
    self->drawPos       = self->position;

    BuzzSaw_Draw();
}

void BuzzSaw_EditorLoad(void) { BuzzSaw->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BuzzSaw.bin", SCOPE_STAGE); }
#endif

void BuzzSaw_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_ENUM, speed);
}
