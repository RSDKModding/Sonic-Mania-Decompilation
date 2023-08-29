// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BuzzSaw Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    if (self->type == BUZZSAW_ATTACHED)
        self->rotation = self->angle;
    else
        self->inkEffect = INK_ADD;

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(BuzzSaw->aniFrames, self->type, &self->animator, true, 0);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = (abs(self->speed * self->amplitude.x) + 0x40) << 17;
        self->updateRange.y = (abs(self->speed * self->amplitude.y) + 0x40) << 17;
        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;
        self->visible   = true;
        self->drawGroup = Zone->objectDrawGroup[0];
        self->startPos  = self->position;

        if (self->type == BUZZSAW_ATTACHED)
            self->state = BuzzSaw_State_Attached;
        else
            self->state = BuzzSaw_State_Stray_Waiting;
    }
}

void BuzzSaw_StageLoad(void)
{
    BuzzSaw->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BuzzSaw.bin", SCOPE_STAGE);

    BuzzSaw->unused1.x = 0x200000;
    BuzzSaw->unused1.y = 0x200000;

    BuzzSaw->hitbox.left   = -24;
    BuzzSaw->hitbox.top    = -24;
    BuzzSaw->hitbox.right  = 24;
    BuzzSaw->hitbox.bottom = 24;

    // ඞ
    Soundboard_LoadSfx("MMZ/SawSus.wav", true, BuzzSaw_SfxCheck_SawSus, BuzzSaw_SfxUpdate_SawSus);
}

bool32 BuzzSaw_SfxCheck_SawSus(void)
{
    int32 worldX = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    int32 worldY = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

    int32 count = 0;

    foreach_all(BuzzSaw, saw)
    {
        if (MathHelpers_Distance(saw->position.x, saw->position.y, worldX, worldY) <= (640 << 16))
            count++;
    }

    return count > 0;
}

void BuzzSaw_SfxUpdate_SawSus(int32 sfx)
{
    int32 dist   = 0x7FFF0000;
    int32 worldX = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    int32 worldY = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

    foreach_all(BuzzSaw, saw) { dist = MIN(MathHelpers_Distance(saw->position.x, saw->position.y, worldX, worldY), dist); }

    RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfx], 1.0 - (MIN(dist >> 16, 640) / 640.0), 0.0, 1.0);
}

void BuzzSaw_CheckPlayerCollisions(void)
{
    RSDK_THIS(BuzzSaw);

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &BuzzSaw->hitbox)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x600, false, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

void BuzzSaw_State_Attached(void)
{
    RSDK_THIS(BuzzSaw);

    self->drawPos.x = self->startPos.x + self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer);
    self->drawPos.y = self->startPos.y + self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer);

    RSDK.ProcessAnimation(&self->animator);

    BuzzSaw_CheckPlayerCollisions();
}

void BuzzSaw_State_Stray_Waiting(void)
{
    RSDK_THIS(BuzzSaw);

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;

    foreach_active(Player, player)
    {
        int32 angle = RSDK.ATan2((player->position.x - self->position.x) >> 16, (player->position.y - self->position.y) >> 16);

        int32 rx = (abs(player->position.x - self->position.x) >> 16) * (abs(player->position.x - self->position.x) >> 16);
        int32 ry = (abs(player->position.y - self->position.y) >> 16) * (abs(player->position.y - self->position.y) >> 16);
        if (((angle + 0x20 - (self->angle & 0xFF)) & 0xFF) < 0x40 && (uint32)((rx + ry) - 0x4000) < 0x5000) {
            self->active     = ACTIVE_NORMAL;
            self->velocity.x = 0x600 * RSDK.Cos256(self->angle);
            self->velocity.y = 0x600 * RSDK.Sin256(self->angle);
            self->state      = BuzzSaw_State_Stray_Released;
        }
    }
}

void BuzzSaw_State_Stray_Released(void)
{
    RSDK_THIS(BuzzSaw);

    if (self->alpha < 0x100)
        self->alpha += 0x10;

    self->drawPos.x += self->velocity.x;
    self->drawPos.y += self->velocity.y;

    RSDK.ProcessAnimation(&self->animator);

    BuzzSaw_CheckPlayerCollisions();

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->position = self->startPos;
        self->drawPos  = self->startPos;
        self->alpha    = 0x00;
        self->state    = BuzzSaw_State_FreeMove_Reset;
    }

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

void BuzzSaw_State_FreeMove_Reset(void)
{
    RSDK_THIS(BuzzSaw);

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->state = BuzzSaw_State_Stray_Waiting;
        BuzzSaw_Create(NULL);
    }
}

#if GAME_INCLUDE_EDITOR
void BuzzSaw_EditorDraw(void)
{
    RSDK_THIS(BuzzSaw);

    self->drawFX    = FX_ROTATE;
    self->inkEffect = INK_NONE;
    self->rotation  = 0;

    if (self->type == BUZZSAW_ATTACHED)
        self->rotation = self->angle;
    else
        self->inkEffect = INK_ADD;

    RSDK.SetSpriteAnimation(BuzzSaw->aniFrames, self->type, &self->animator, true, 0);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = (abs(self->speed * self->amplitude.x) + 0x40) << 17;
    self->updateRange.y = (abs(self->speed * self->amplitude.y) + 0x40) << 17;
    self->drawPos       = self->position;

    BuzzSaw_Draw();

    if (showGizmos() && self->type == BUZZSAW_ATTACHED) {
        RSDK_DRAWING_OVERLAY(true);

        Vector2 pos;
        Vector2 amplitude;
        amplitude.x = self->amplitude.x >> 10;
        amplitude.y = self->amplitude.y >> 10;

        self->updateRange.x = (abs(self->speed * self->amplitude.x) + 0x40) << 17;
        self->updateRange.y = (abs(self->speed * self->amplitude.y) + 0x40) << 17;

        // draw distance previews
        self->inkEffect = INK_BLEND;
        self->drawPos.x = amplitude.x * RSDK.Sin1024(0x100) + self->position.x;
        self->drawPos.y = amplitude.y * RSDK.Sin1024(0x100) + self->position.y;
        pos             = self->drawPos;
        BuzzSaw_Draw();

        self->drawPos.x = amplitude.x * RSDK.Sin1024(0x300) + self->position.x;
        self->drawPos.y = amplitude.y * RSDK.Sin1024(0x300) + self->position.y;
        BuzzSaw_Draw();

        RSDK.DrawLine(pos.x, pos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, 0, INK_NONE, false);

        self->inkEffect = INK_NONE;
        RSDK_DRAWING_OVERLAY(false);
    }
}

void BuzzSaw_EditorLoad(void)
{
    BuzzSaw->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BuzzSaw.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BuzzSaw, type);
    RSDK_ENUM_VAR("Attached", BUZZSAW_ATTACHED);
    RSDK_ENUM_VAR("Stray", BUZZSAW_STRAY);
}
#endif

void BuzzSaw_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(BuzzSaw, VAR_ENUM, speed);
}
