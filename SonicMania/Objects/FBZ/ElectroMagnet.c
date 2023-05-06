// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ElectroMagnet Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectElectroMagnet *ElectroMagnet;

void ElectroMagnet_Update(void)
{
    RSDK_THIS(ElectroMagnet);

    StateMachine_Run(self->state);
}

void ElectroMagnet_LateUpdate(void) {}

void ElectroMagnet_StaticUpdate(void)
{
    if (ElectroMagnet->magnetSfxTimer) {
        if (!ElectroMagnet->playingMagnetSfx) {
            RSDK.PlaySfx(ElectroMagnet->sfxMagnet, true, 255);
            ElectroMagnet->playingMagnetSfx = true;
        }
        ElectroMagnet->magnetSfxTimer = 0;
    }
    else if (ElectroMagnet->playingMagnetSfx) {
        RSDK.StopSfx(ElectroMagnet->sfxMagnet);
        ElectroMagnet->playingMagnetSfx = false;
    }
}

void ElectroMagnet_Draw(void)
{
    RSDK_THIS(ElectroMagnet);

    if (self->state == ElectroMagnet_State_MagnetActive) {
        self->animator.frameID = Zone->timer & 3;
        RSDK.DrawSprite(&self->animator, NULL, false);

        self->animator.frameID = (Zone->timer & 1) + 4;
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

void ElectroMagnet_Create(void *data)
{
    RSDK_THIS(ElectroMagnet);

    if (!self->interval)
        self->interval = 600;

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->visible       = !self->invisible;
    self->updateRange.y = (self->height + 64) << 16;
    self->drawGroup     = Zone->objectDrawGroup[1];

    self->hitboxMagnetRange.left   = -64;
    self->hitboxMagnetRange.top    = 0;
    self->hitboxMagnetRange.right  = 64;
    self->hitboxMagnetRange.bottom = self->height;

    self->hitboxShieldRange.left   = -64;
    self->hitboxShieldRange.top    = 0;
    self->hitboxShieldRange.right  = 64;
    self->hitboxShieldRange.bottom = self->shieldRange;

    RSDK.SetSpriteAnimation(ElectroMagnet->aniFrames, 0, &self->animator, true, 0);
    self->state = ElectroMagnet_State_AwaitInterval;
}

void ElectroMagnet_StageLoad(void)
{
    ElectroMagnet->active = ACTIVE_ALWAYS;

    if (RSDK.CheckSceneFolder("FBZ"))
        ElectroMagnet->aniFrames = RSDK.LoadSpriteAnimation("FBZ/ElectroMagnet.bin", SCOPE_STAGE);

    ElectroMagnet->hitboxPlayer.left   = -1;
    ElectroMagnet->hitboxPlayer.top    = -1;
    ElectroMagnet->hitboxPlayer.right  = 1;
    ElectroMagnet->hitboxPlayer.bottom = 1;

    ElectroMagnet->sfxRange.x = 0x800000;
    ElectroMagnet->sfxRange.y = 0x400000;

    ElectroMagnet->sfxMagnet = RSDK.GetSfx("Stage/Magnet.wav");
}

void ElectroMagnet_State_AwaitInterval(void)
{
    RSDK_THIS(ElectroMagnet);
    int32 time = (Zone->timer + self->intervalOffset) % self->interval;

    if (time <= self->duration) {
        self->active = ACTIVE_NORMAL;
        self->timer  = self->duration - time;
        self->state  = ElectroMagnet_State_MagnetActive;
    }
}

void ElectroMagnet_State_MagnetActive(void)
{
    RSDK_THIS(ElectroMagnet);

    if (self->timer <= 0) {
        if (self->hitboxShieldRange.top != self->hitboxShieldRange.bottom) {
            foreach_active(Player, player)
            {
                if (RSDK.CheckObjectCollisionTouchBox(player, &ElectroMagnet->hitboxPlayer, self, &self->hitboxShieldRange)
                    && player->invertGravity) {
                    player->invertGravity = false;
                    player->onGround      = false;
                }
            }
        }

        self->active = ACTIVE_BOUNDS;
        self->state  = ElectroMagnet_State_AwaitInterval;
    }
    else {
        self->timer--;

        foreach_all(Blaster, blaster)
        {
            if (RSDK.CheckObjectCollisionTouchBox(blaster, &ElectroMagnet->hitboxPlayer, self, &self->hitboxMagnetRange)
                && blaster->animator.animationID < 2 && blaster->state != Blaster_State_Init) {
                blaster->state  = Blaster_State_MagnetAttract;
                blaster->active = ACTIVE_NORMAL;
            }
        }

        foreach_all(MagSpikeBall, spikeBall)
        {
            if (RSDK.CheckObjectCollisionTouchBox(spikeBall, &ElectroMagnet->hitboxPlayer, self, &self->hitboxMagnetRange)) {
                spikeBall->direction = FLIP_X;
                spikeBall->active    = ACTIVE_NORMAL;
            }
        }

        foreach_all(MagPlatform, platform)
        {
            if (RSDK.CheckObjectCollisionTouchBox(platform, &ElectroMagnet->hitboxPlayer, self, &self->hitboxMagnetRange)) {
                platform->state  = MagPlatform_State_Rise;
                platform->active = ACTIVE_NORMAL;
            }
        }

        if (RSDK.CheckOnScreen(self, &ElectroMagnet->sfxRange))
            ++ElectroMagnet->magnetSfxTimer;

        if (self->hitboxShieldRange.top != self->hitboxShieldRange.bottom) {
            foreach_active(Player, player)
            {
                if (RSDK.CheckObjectCollisionTouchBox(player, &ElectroMagnet->hitboxPlayer, self, &self->hitboxShieldRange)) {
                    if (player->state == Player_State_FlyCarried
                        || (player->characterID == ID_KNUCKLES
                            && (player->state == Player_State_KnuxWallClimb || player->state == Player_State_KnuxLedgePullUp))) {
                        player->invertGravity = false;
                    }
                    else if (player->shield == SHIELD_LIGHTNING) {
                        if (player->collisionMode != CMODE_ROOF)
                            player->onGround = false;

                        player->invertGravity = true;

                        if (player->state != Player_State_Transform) {
                            if (player->velocity.y > -0x68000)
                                player->velocity.y -= 2 * player->gravityStrength;

                            if (RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_ROOF, player->collisionPlane, 0, -0x160000, false)) {
                                player->collisionMode = CMODE_ROOF;
                                player->angle         = 128;
                                player->onGround      = true;
                                player->groundVel     = -player->velocity.x;
                            }
                            else {
                                player->state = Player_State_Air;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            }
                        }
                    }
                }
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void ElectroMagnet_EditorDraw(void)
{
    RSDK_THIS(ElectroMagnet);
    self->alpha = 0xC0;

    self->inkEffect = self->invisible ? INK_ALPHA : INK_NONE;

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->inkEffect = INK_NONE;

    self->hitboxMagnetRange.left   = -64;
    self->hitboxMagnetRange.top    = 0;
    self->hitboxMagnetRange.right  = 64;
    self->hitboxMagnetRange.bottom = self->height;

    self->hitboxShieldRange.left   = -64;
    self->hitboxShieldRange.top    = 0;
    self->hitboxShieldRange.right  = 64;
    self->hitboxShieldRange.bottom = self->shieldRange;

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxMagnetRange, FLIP_NONE, 0xFFFF00);
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxShieldRange, FLIP_NONE, 0xFF0000);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void ElectroMagnet_EditorLoad(void) { ElectroMagnet->aniFrames = RSDK.LoadSpriteAnimation("FBZ/ElectroMagnet.bin", SCOPE_STAGE); }
#endif

void ElectroMagnet_Serialize(void)
{
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_UINT16, duration);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_ENUM, shieldRange);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_BOOL, invisible);
}
