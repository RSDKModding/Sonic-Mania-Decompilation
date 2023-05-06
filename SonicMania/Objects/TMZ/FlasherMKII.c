// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FlasherMKII Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFlasherMKII *FlasherMKII;

void FlasherMKII_Update(void)
{
    RSDK_THIS(FlasherMKII);

    RSDK.ProcessAnimation(&self->animator);

    StateMachine_Run(self->state);
}

void FlasherMKII_LateUpdate(void) {}

void FlasherMKII_StaticUpdate(void) {}

void FlasherMKII_Draw(void)
{
    RSDK_THIS(FlasherMKII);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void FlasherMKII_Create(void *data)
{
    RSDK_THIS(FlasherMKII);

    self->drawFX |= FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->startPos      = self->position;

        switch (self->orientation) {
            case FLASHERMKII_ORIENTATION_UP:
                self->type      = 0;
                self->direction = FLIP_NONE;
                break;

            case FLASHERMKII_ORIENTATION_DOWN:
                self->type      = 0;
                self->direction = FLIP_Y;
                break;

            case FLASHERMKII_ORIENTATION_RIGHT:
                self->direction = FLIP_NONE;
                self->type      = 4;
                break;

            case FLASHERMKII_ORIENTATION_LEFT:
                self->direction = FLIP_X;
                self->type      = 4;
                break;

            default: break;
        }

        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type, &self->animator, true, 0);
        self->state = FlasherMKII_State_Idle;
    }
}

void FlasherMKII_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("TMZ1") || RSDK.CheckSceneFolder("TMZ2"))
        FlasherMKII->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/FlasherMKII.bin", SCOPE_STAGE);

    DEBUGMODE_ADD_OBJ(FlasherMKII);

    FlasherMKII->sfxFlop = RSDK.GetSfx("TMZ1/FlasherFlop.wav");
    FlasherMKII->sfxZap  = RSDK.GetSfx("TMZ1/FlasherZap.wav");
}

void FlasherMKII_DebugSpawn(void)
{
    RSDK_THIS(FlasherMKII);

    CREATE_ENTITY(FlasherMKII, NULL, self->position.x, self->position.y);
}

void FlasherMKII_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void FlasherMKII_CheckOffScreen(void)
{
    RSDK_THIS(FlasherMKII);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange))
        FlasherMKII_Create(NULL);
}

void FlasherMKII_HandlePlayerCollisions(void)
{
    RSDK_THIS(FlasherMKII);

    foreach_active(Player, player)
    {
        Hitbox *hitbox = RSDK.GetHitbox(&self->animator, 0);

        if (Player_CheckBadnikTouch(player, self, hitbox)) {
            if ((self->animator.animationID & 3) == 3) {
                if (player->shield == SHIELD_LIGHTNING || player->invincibleTimer || player->blinkTimer) {
                    if (Player_CheckBadnikBreak(player, self, true))
                        RSDK.StopSfx(FlasherMKII->sfxZap);
                }
                else {
                    Player_Hurt(player, self);
                }
            }
            else {
                Player_CheckBadnikBreak(player, self, true);
            }
        }
    }
}

void FlasherMKII_HandleHarmPlayerCollisions(void)
{
    RSDK_THIS(FlasherMKII);

    foreach_active(Player, player)
    {
        if (player->shield == SHIELD_LIGHTNING || player->invincibleTimer || player->blinkTimer) {
            Hitbox *hitbox = RSDK.GetHitbox(&self->animator, 0);
            if (Player_CheckBadnikTouch(player, self, hitbox) && Player_CheckBadnikBreak(player, self, true))
                RSDK.StopSfx(FlasherMKII->sfxZap);
        }
        else {
            Hitbox *hitbox = RSDK.GetHitbox(&self->animator, 1);
            if (Player_CheckCollisionTouch(player, self, hitbox))
                Player_Hurt(player, self);
        }
    }
}

void FlasherMKII_State_Idle(void)
{
    RSDK_THIS(FlasherMKII);

    self->active = ACTIVE_NORMAL;
    if (self->timer > 0)
        self->timer--;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        int32 rx = (self->position.x - player->position.x) >> 16;
        int32 ry = (self->position.y - player->position.y) >> 16;

        if (!self->timer && rx * rx + ry * ry < 0x1000) {
            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 2, &self->animator, false, 0);
            RSDK.PlaySfx(FlasherMKII->sfxZap, false, 255);
            self->state = FlasherMKII_State_WeakFlash;
        }
        else {
            switch (self->orientation) {
                case FLASHERMKII_ORIENTATION_UP:
                    if (player->position.x >= self->position.x) {
                        if ((uint32)(rx + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x600000, 0xC0000, 2)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                            self->direction  = FLIP_X;
                            self->velocity.x = 0x220000;
                            self->state      = FlasherMKII_State_Moving;
                            break;
                        }
                    }
                    else if ((uint32)(rx - 65) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x600000, 0xC0000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                        self->direction  = FLIP_NONE;
                        self->velocity.x = -0x220000;
                        self->state      = FlasherMKII_State_Moving;
                        break;
                    }
                    break;

                case FLASHERMKII_ORIENTATION_DOWN:
                    if (player->position.x >= self->position.x) {
                        if ((uint32)(rx + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_ROOF, 0, 0x600000, -0xC0000, 2)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                            self->velocity.x = 0x220000;
                            self->direction  = FLIP_XY;
                            self->state      = FlasherMKII_State_Moving;
                            break;
                        }
                    }
                    else if ((uint32)(rx - 65) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_ROOF, 0, -0x600000, -0xC0000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                        self->direction  = FLIP_Y;
                        self->velocity.x = -0x220000;
                        self->state      = FlasherMKII_State_Moving;
                        break;
                    }
                    break;

                case FLASHERMKII_ORIENTATION_RIGHT:
                    if (player->position.y >= self->position.y) {
                        if ((uint32)(ry + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_RWALL, 0, -0xC0000, 0x600000, 2)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                            self->direction  = FLIP_Y;
                            self->velocity.y = 0x220000;
                            self->state      = FlasherMKII_State_Moving;
                            break;
                        }
                    }
                    else if ((uint32)(ry - 65) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_RWALL, 0, -0xC0000, -0x600000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                        self->direction  = FLIP_NONE;
                        self->velocity.y = -0x220000;
                        self->state      = FlasherMKII_State_Moving;
                        break;
                    }
                    break;

                case FLASHERMKII_ORIENTATION_LEFT:
                    if (player->position.y >= self->position.y) {
                        if ((uint32)(ry + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_LWALL, 0, 0xC0000, 0x600000, 2)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                            self->velocity.y = 0x220000;
                            self->direction  = FLIP_XY;
                            self->state      = FlasherMKII_State_Moving;
                            break;
                        }
                    }
                    else if ((uint32)(ry - 65) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_LWALL, 0, 0xC0000, -0x600000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                        self->direction  = FLIP_X;
                        self->velocity.y = -0x220000;
                        self->state      = FlasherMKII_State_Moving;
                        break;
                    }
                    break;

                default: break;
            }
        }
    }

    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOffScreen();
}

void FlasherMKII_State_Moving(void)
{
    RSDK_THIS(FlasherMKII);

    if ((self->animator.frameID == 5 || self->animator.frameID == 12) && self->animator.timer == 1) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        if (self->onScreen == 1)
            RSDK.PlaySfx(FlasherMKII->sfxFlop, false, 255);
    }

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type, &self->animator, false, 0);
        self->state = FlasherMKII_State_Idle;
    }

    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOffScreen();
}

void FlasherMKII_State_WeakFlash(void)
{
    RSDK_THIS(FlasherMKII);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 3, &self->animator, false, 0);
        self->state = FlasherMKII_State_StrongFlash;
    }

    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOffScreen();
}

void FlasherMKII_State_StrongFlash(void)
{
    RSDK_THIS(FlasherMKII);

    if (++self->timer == 120) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 2, &self->animator, false, 0);
        self->state = FlasherMKII_State_FinishedFlashing;
    }

    FlasherMKII_HandleHarmPlayerCollisions();
    FlasherMKII_CheckOffScreen();
}

void FlasherMKII_State_FinishedFlashing(void)
{
    RSDK_THIS(FlasherMKII);

    if (++self->timer == 30) {
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type, &self->animator, false, 0);
        self->state = FlasherMKII_State_Idle;
    }

    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void FlasherMKII_EditorDraw(void)
{
    RSDK_THIS(FlasherMKII);

    self->startPos = self->position;

    switch (self->orientation) {
        case FLASHERMKII_ORIENTATION_UP:
            self->type      = 0;
            self->direction = FLIP_NONE;
            break;

        case FLASHERMKII_ORIENTATION_DOWN:
            self->type      = 0;
            self->direction = FLIP_Y;
            break;

        case FLASHERMKII_ORIENTATION_RIGHT:
            self->direction = FLIP_NONE;
            self->type      = 4;
            break;

        case FLASHERMKII_ORIENTATION_LEFT:
            self->direction = FLIP_X;
            self->type      = 4;
            break;

        default: break;
    }

    RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type, &self->animator, true, 0);

    FlasherMKII_Draw();
}

void FlasherMKII_EditorLoad(void)
{
    FlasherMKII->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/FlasherMKII.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(FlasherMKII, orientation);
    RSDK_ENUM_VAR("Up", FLASHERMKII_ORIENTATION_UP);
    RSDK_ENUM_VAR("Down", FLASHERMKII_ORIENTATION_DOWN);
    RSDK_ENUM_VAR("Right", FLASHERMKII_ORIENTATION_RIGHT);
    RSDK_ENUM_VAR("Left", FLASHERMKII_ORIENTATION_LEFT);
}
#endif

void FlasherMKII_Serialize(void) { RSDK_EDITABLE_VAR(FlasherMKII, VAR_UINT8, orientation); }
