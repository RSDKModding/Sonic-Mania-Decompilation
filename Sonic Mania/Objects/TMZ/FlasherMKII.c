#include "SonicMania.h"

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
        self->drawOrder     = Zone->drawOrderLow;
        self->startPos      = self->position;
        switch (self->orientation) {
            case 0:
                self->type      = 0;
                self->direction = FLIP_NONE;
                break;
            case 1:
                self->type      = 0;
                self->direction = FLIP_Y;
                break;
            case 2:
                self->direction = FLIP_NONE;
                self->type      = 4;
                break;
            case 3:
                self->direction = FLIP_X;
                self->type      = 4;
                break;
            default: break;
        }
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type, &self->animator, true, 0);
        self->state = FlasherMKII_State_Unknown1;
    }
}

void FlasherMKII_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        FlasherMKII->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/FlasherMKII.bin", SCOPE_STAGE);

    DEBUGMODE_ADD_OBJ(FlasherMKII);

    FlasherMKII->sfxFlop = RSDK.GetSFX("TMZ1/FlasherFlop.wav");
    FlasherMKII->sfxZap  = RSDK.GetSFX("TMZ1/FlasherZap.wav");
}

void FlasherMKII_DebugSpawn(void)
{
    RSDK_THIS(FlasherMKII);
    CREATE_ENTITY(FlasherMKII, NULL, self->position.x, self->position.y);
}

void FlasherMKII_DebugDraw(void)
{
    RSDK_THIS(FlasherMKII);
    RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void FlasherMKII_CheckOnScreen(void)
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
                    if (Player_CheckBadnikBreak(self, player, true))
                        RSDK.StopSFX(FlasherMKII->sfxZap);
                }
                else {
                    Player_CheckHit(player, self);
                }
            }
            else {
                Player_CheckBadnikBreak(self, player, true);
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
            if (Player_CheckBadnikTouch(player, self, hitbox) && Player_CheckBadnikBreak(self, player, true))
                RSDK.StopSFX(FlasherMKII->sfxZap);
        }
        else {
            Hitbox *hitbox = RSDK.GetHitbox(&self->animator, 1);
            if (Player_CheckCollisionTouch(player, self, hitbox))
                Player_CheckHit(player, self);
        }
    }
}

void FlasherMKII_State_Unknown1(void)
{
    RSDK_THIS(FlasherMKII);

    self->active = ACTIVE_NORMAL;
    if (self->timer > 0)
        self->timer--;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        int rx = (self->position.x - player->position.x) >> 16;
        int ry = (self->position.y - player->position.y) >> 16;
        if (!self->timer && rx * rx + ry * ry < 0x1000) {
            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 2, &self->animator, false, 0);
            RSDK.PlaySfx(FlasherMKII->sfxZap, false, 255);
            self->state = FlasherMKII_State_Unknown3;
        }
        else {
            switch (self->orientation) {
                case 0:
                    if (player->position.x >= self->position.x) {
                        if ((uint32)(rx + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x600000, 0xC0000, 2)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                            self->direction  = FLIP_X;
                            self->velocity.x = 0x220000;
                            self->state      = FlasherMKII_State_Unknown2;
                            break;
                        }
                    }
                    else if ((rx - 65) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x600000, 0xC0000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                        self->direction  = FLIP_NONE;
                        self->velocity.x = -0x220000;
                        self->state      = FlasherMKII_State_Unknown2;
                        break;
                    }
                    break;
                case 1:
                    if (player->position.x >= self->position.x) {
                        if ((uint32)(rx + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_ROOF, 0, 0x600000, -0xC0000, 2)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                            self->velocity.x = 0x220000;
                            self->direction  = FLIP_XY;
                            self->state      = FlasherMKII_State_Unknown2;
                            break;
                        }
                    }
                    else if ((rx - 65) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_ROOF, 0, -0x600000, -0xC0000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                        self->direction  = FLIP_Y;
                        self->velocity.x = -0x220000;
                        self->state      = FlasherMKII_State_Unknown2;
                        break;
                    }
                    break;
                case 2:
                    if (player->position.y >= self->position.y) {
                        if ((uint32)(ry + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_RWALL, 0, -0xC0000, 0x600000, 2u)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                            self->direction  = FLIP_Y;
                            self->velocity.y = 0x220000;
                            self->state      = FlasherMKII_State_Unknown2;
                            break;
                        }
                    }
                    else if ((ry - 65) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_RWALL, 0, -0xC0000, -0x600000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                        self->direction  = FLIP_NONE;
                        self->velocity.y = -0x220000;
                        self->state      = FlasherMKII_State_Unknown2;
                        break;
                    }
                    break;
                case 3:
                    if (player->position.y >= self->position.y) {
                        if ((uint32)(ry + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_LWALL, 0, 0xC0000, 0x600000, 2u)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                            self->velocity.y = 0x220000;
                            self->direction  = FLIP_XY;
                            self->state      = FlasherMKII_State_Unknown2;
                            break;
                        }
                    }
                    else if ((uint32)(ry - 65) <= 0xBE && RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_LWALL, 0, 0xC0000, -0x600000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 1, &self->animator, false, 0);
                        self->direction  = FLIP_X;
                        self->velocity.y = -0x220000;
                        self->state      = FlasherMKII_State_Unknown2;
                        break;
                    }
                    break;
                default: break;
            }
        }
    }

    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

void FlasherMKII_State_Unknown2(void)
{
    RSDK_THIS(FlasherMKII);

    if ((self->animator.frameID == 5 || self->animator.frameID == 12) && self->animator.animationTimer == 1) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        if (self->activeScreens == 1)
            RSDK.PlaySfx(FlasherMKII->sfxFlop, false, 255);
    }
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type, &self->animator, false, 0);
        self->state = FlasherMKII_State_Unknown1;
    }
    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

void FlasherMKII_State_Unknown3(void)
{
    RSDK_THIS(FlasherMKII);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 3, &self->animator, false, 0);
        self->state = FlasherMKII_State_Unknown4;
    }
    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

void FlasherMKII_State_Unknown4(void)
{
    RSDK_THIS(FlasherMKII);

    if (++self->timer == 120) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type + 2, &self->animator, false, 0);
        self->state = FlasherMKII_State_Unknown5;
    }
    FlasherMKII_HandleHarmPlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

void FlasherMKII_State_Unknown5(void)
{
    RSDK_THIS(FlasherMKII);

    if (++self->timer == 30) {
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type, &self->animator, false, 0);
        self->state = FlasherMKII_State_Unknown1;
    }
    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void FlasherMKII_EditorDraw(void)
{
    RSDK_THIS(FlasherMKII);
    self->startPos = self->position;
    switch (self->orientation) {
        case 0:
            self->type      = 0;
            self->direction = FLIP_NONE;
            break;
        case 1:
            self->type      = 0;
            self->direction = FLIP_Y;
            break;
        case 2:
            self->direction = FLIP_NONE;
            self->type      = 4;
            break;
        case 3:
            self->direction = FLIP_X;
            self->type      = 4;
            break;
        default: break;
    }

    RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, self->type, &self->animator, true, 0);

    FlasherMKII_Draw();
}

void FlasherMKII_EditorLoad(void) { FlasherMKII->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/FlasherMKII.bin", SCOPE_STAGE); }
#endif

void FlasherMKII_Serialize(void) { RSDK_EDITABLE_VAR(FlasherMKII, VAR_UINT8, orientation); }
