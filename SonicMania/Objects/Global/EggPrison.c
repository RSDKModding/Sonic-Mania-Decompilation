// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EggPrison Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectEggPrison *EggPrison;

void EggPrison_Update(void)
{
    RSDK_THIS(EggPrison);

    StateMachine_Run(self->state);

    if (self->type == EGGPRISON_FLYING) {
        RSDK.ProcessAnimation(&self->propellerAnimator);

        EggPrison_HandleMovement();

        if (!self->notSolid) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionBox(player, self, &self->hitboxSolid) == C_TOP) {
                    player->position.x += self->velocity.x;
                    player->position.y += 0x10000;
                }

                if (self->state == EggPrison_State_HandleBounds) {
                    if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_BOTTOM) {
                        self->velocity.x = 0;
                        self->active     = ACTIVE_NORMAL;
                        self->state      = EggPrison_State_Activated;
                        self->buttonPos  = -0x80000;
                    }
                    else {
                        if (!Player_CheckCollisionTouch(player, self, &self->hitboxButtonTrigger)) {
                            if (self->buttonPos < 0)
                                self->buttonPos += 0x10000;
                        }
                        else {
                            Hitbox *playerHitbox = Player_GetHitbox(player);
                            self->buttonPos      = ((playerHitbox->top - 48) << 16) - self->position.y + player->position.y;
                            self->buttonPos      = clampVal(self->buttonPos, -0x80000, 0);
                            self->buttonPos &= 0xFFFF0000;
                        }
                    }
                }
                else {
                    Player_CheckCollisionBox(player, self, &self->hitboxButton);
                }
            }
        }
    }
    else {
        if (!self->notSolid) {
            foreach_active(Player, player)
            {
                Player_CheckCollisionBox(player, self, &self->hitboxSolid);

                if (self->state == EggPrison_State_HandleBounds) {
                    if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_TOP) {
                        self->buttonPos = 0x80000;

                        if (self->type < EGGPRISON_DUD)
                            SceneInfo->timeEnabled = false;

                        if (self->type == EGGPRISON_SPRING) {
                            int32 anim = player->animator.animationID;
                            if (anim == ANI_WALK || (anim > ANI_AIR_WALK && anim <= ANI_DASH))
                                player->animationReserve = player->animator.animationID;
                            else
                                player->animationReserve = ANI_WALK;
                            player->state      = Player_State_Air;
                            player->onGround   = false;
                            player->velocity.y = -0xA0000;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_TWIRL, &player->animator, true, 0);
                            RSDK.PlaySfx(EggPrison->sfxSpring, false, 255);
                        }
                        else {
                            self->active = ACTIVE_NORMAL;
                            self->state  = EggPrison_State_Activated;
                        }
                    }
                    else {
                        if (Player_CheckCollisionTouch(player, self, &self->hitboxButtonTrigger)) {
                            Hitbox *playerHitbox = Player_GetHitbox(player);

                            self->buttonPos = ((playerHitbox->bottom + 48) << 16) - self->position.y + player->position.y;
                            self->buttonPos = clampVal(self->buttonPos, 0, 0x80000);
                            self->buttonPos &= 0xFFFF0000;
                        }
                        else if (self->buttonPos > 0)
                            self->buttonPos -= 0x10000;
                    }
                }
                else {
                    Player_CheckCollisionBox(player, self, &self->hitboxButton);
                }
            }
        }
    }
}

void EggPrison_LateUpdate(void) {}

void EggPrison_StaticUpdate(void) {}

void EggPrison_Draw(void)
{
    RSDK_THIS(EggPrison);

    Vector2 drawPos;
    if (self->type == EGGPRISON_FLYING) {
        self->direction = FLIP_Y;
        drawPos.x       = self->position.x;
        drawPos.y       = self->position.y + self->buttonPos;
        RSDK.DrawSprite(&self->buttonAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->capsuleAnimator, NULL, false);
        RSDK.DrawSprite(&self->panelAnimator, NULL, false);
        RSDK.DrawSprite(&self->propellerAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->propellerAnimator, NULL, false);

        self->direction = FLIP_NONE;
    }
    else {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + self->buttonPos;
        RSDK.DrawSprite(&self->buttonAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->capsuleAnimator, NULL, false);
        RSDK.DrawSprite(&self->panelAnimator, NULL, false);
    }
}

void EggPrison_Create(void *data)
{
    RSDK_THIS(EggPrison);

    if (globals->gameMode < MODE_TIMEATTACK || self->type >= EGGPRISON_FLYING) {
        self->drawFX = FX_FLIP;

        if (!SceneInfo->inEditor) {
            if (data)
                self->type = voidToInt(data);

            if (self->type == EGGPRISON_FLYING) {
                RSDK.SetSpriteAnimation(EggPrison->aniFrames, 0, &self->capsuleAnimator, true, 1);
                RSDK.SetSpriteAnimation(EggPrison->aniFrames, 1, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->aniFrames, 2, &self->panelAnimator, true, 1);
                RSDK.SetSpriteAnimation(EggPrison->aniFrames, 3, &self->propellerAnimator, true, 0);

                self->hitboxButton.left   = -16;
                self->hitboxButton.top    = 24;
                self->hitboxButton.right  = 16;
                self->hitboxButton.bottom = 38;

                self->hitboxButtonTrigger.left   = -15;
                self->hitboxButtonTrigger.top    = 24;
                self->hitboxButtonTrigger.right  = 15;
                self->hitboxButtonTrigger.bottom = 48;

                self->velocity.x = 0x10000;
                self->velocity.y = 0x4000;
            }
            else {
                RSDK.SetSpriteAnimation(EggPrison->aniFrames, 0, &self->capsuleAnimator, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->aniFrames, 1, &self->buttonAnimator, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->aniFrames, 2, &self->panelAnimator, true, 0);

                self->hitboxButton.left   = -16;
                self->hitboxButton.top    = -38;
                self->hitboxButton.right  = 16;
                self->hitboxButton.bottom = -24;

                self->hitboxButtonTrigger.left  = -15;
                self->hitboxButtonTrigger.top   = -47;
                self->hitboxButtonTrigger.right = 15;
                self->hitboxButtonTrigger.top   = -24;
            }
            self->hitboxSolid.left   = -32;
            self->hitboxSolid.top    = -24;
            self->hitboxSolid.right  = 32;
            self->hitboxSolid.bottom = 32;

            self->state         = EggPrison_State_Setup;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->visible       = true;
            self->drawOrder     = Zone->objectDrawLow;
        }
    }
    else {
        destroyEntity(self);
    }
}

void EggPrison_StageLoad(void)
{
    EggPrison->aniFrames = RSDK.LoadSpriteAnimation("Global/EggPrison.bin", SCOPE_STAGE);

    EggPrison->sfxDestroy = RSDK.GetSfx("Global/Destroy.wav");
    EggPrison->sfxSpring  = RSDK.GetSfx("Global/Spring.wav");
}

void EggPrison_HandleMovement(void)
{
    RSDK_THIS(EggPrison);

    bool32 hitFloor = false;
    if (self->checkTileCollisions) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x300000, 0x900000, false)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x300000, 0x900000, false)) {
            self->originY -= self->velocity.y;
            hitFloor = true;
        }
    }

    if (!hitFloor) {
        if (self->originY < (ScreenInfo->position.y + 64) << 16)
            self->originY += self->velocity.y;
    }

    if (self->state != EggPrison_State_FlyOffScreen) {
        if (self->velocity.x > 0) {
            if (self->position.x <= (ScreenInfo->position.x + ScreenInfo->width - 48) << 16) {
                if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x400000, 0, true))
                    self->velocity.x = -self->velocity.x;
            }
            else {
                self->velocity.x = -self->velocity.x;
            }
        }
        else if (self->position.x < (ScreenInfo->position.x + 48) << 16) {
            self->velocity.x = -self->velocity.x;
        }
        else {
            if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x400000, 0, true))
                self->velocity.x = -self->velocity.x;
        }
    }

    self->position.x += self->velocity.x;
    self->position.y = BadnikHelpers_Oscillate(self->originY, 4, 10);
}

void EggPrison_State_Activated(void)
{
    RSDK_THIS(EggPrison);

    RSDK.SetSpriteAnimation(-1, 0, &self->panelAnimator, true, 0);
    self->state = EggPrison_State_Explode;

    switch (self->type) {
        default:
        case EGGPRISON_NORMAL:
        case EGGPRISON_FLYING:
        case EGGPRISON_DUD:
        case EGGPRISON_SPRING:
            for (int32 a = 0; a < 10; ++a) {
                int32 x                = self->position.x + (((RSDK.Rand(0, 48) & -4) - 24) << 16);
                int32 y                = self->position.y + 0x40000;
                EntityAnimals *animals = CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[a & 1] + 1), x, y);

                animals->timer     = 4 * a;
                animals->state     = Animals_State_BounceAround;
                animals->behaviour = ANIMAL_BEHAVE_FOLLOW;
                animals->direction = (a ^ (a >> 1)) & 1;
            }
            break;

        case EGGPRISON_RINGS: {
            int32 angle = 0x90;
            for (int32 r = 0; r < 6; ++r) {
                int32 x          = self->position.x + (((RSDK.Rand(0, 48) & -4) - 24) << 16);
                int32 y          = self->position.y;
                EntityRing *ring = CREATE_ENTITY(Ring, self, x, y);

                ring->velocity.x     = RSDK.Cos256(angle) << 9;
                ring->velocity.y     = RSDK.Sin256(angle) << 9;
                ring->animator.speed = 512;
                ring->collisionPlane = 0;
                ring->inkEffect      = INK_ALPHA;
                if (angle != 176)
                    angle += 8;
                else
                    angle = 0xD0;
                ring->alpha     = 0x100;
                ring->state     = Ring_State_Bounce;
                ring->stateDraw = Ring_Draw_Normal;
            }
            break;
        }

        case EGGPRISON_TRAP: {
            EntityTechnosqueek *technosqueek = CREATE_ENTITY(Technosqueek, NULL, self->position.x - 0x80000, self->position.y);
            technosqueek->velocity.x         = -0x30000;
            technosqueek->velocity.y         = -0x40000;
            technosqueek->active             = ACTIVE_NORMAL;
            technosqueek->state              = Technosqueek_State_Fall;

            technosqueek             = CREATE_ENTITY(Technosqueek, NULL, self->position.x + 0x80000, self->position.y);
            technosqueek->velocity.x = 0x30000;
            technosqueek->velocity.y = -0x40000;
            technosqueek->active     = ACTIVE_NORMAL;
            technosqueek->direction  = FLIP_X;
            technosqueek->state      = Technosqueek_State_Fall;

            EntityBlaster *blaster = CREATE_ENTITY(Blaster, NULL, self->position.x - 0x180000, self->position.y);
            blaster->velocity.x    = -0x30000;
            blaster->velocity.y    = -0x30000;
            blaster->active        = ACTIVE_NORMAL;
            blaster->state         = Blaster_State_Fall;

            blaster             = CREATE_ENTITY(Blaster, NULL, self->position.x + 0x180000, self->position.y);
            blaster->velocity.x = 0x30000;
            blaster->velocity.y = -0x30000;
            blaster->active     = ACTIVE_NORMAL;
            blaster->direction  = 1;
            blaster->state      = Blaster_State_Fall;
            break;
        }
    }

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x160000, self->position.y);
    RSDK.SetSpriteAnimation(EggPrison->aniFrames, 2, &debris->animator, true, 2);
    debris->velocity.x      = -0x20000;
    debris->velocity.y      = -0x20000;
    debris->gravityStrength = 0x3800;
    debris->rotSpeed        = -4;
    debris->drawFX          = FX_ROTATE;
    debris->drawOrder       = Zone->objectDrawHigh;
    debris->updateRange.x   = 0x800000;
    debris->updateRange.y   = 0x800000;

    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
    RSDK.SetSpriteAnimation(EggPrison->aniFrames, 2, &debris->animator, true, 3);
    debris->velocity.x      = 0;
    debris->velocity.y      = -0x30000;
    debris->gravityStrength = 0x3800;
    debris->rotSpeed        = 8;
    debris->drawFX          = FX_ROTATE;
    debris->drawOrder       = Zone->objectDrawHigh;
    debris->updateRange.x   = 0x800000;
    debris->updateRange.y   = 0x800000;

    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x160000, self->position.y);
    RSDK.SetSpriteAnimation(EggPrison->aniFrames, 2, &debris->animator, true, 4);
    debris->velocity.x      = 0x20000;
    debris->velocity.y      = -0x20000;
    debris->gravityStrength = 0x3800;
    debris->rotSpeed        = 4;
    debris->drawFX          = FX_ROTATE;
    debris->drawOrder       = Zone->objectDrawHigh;
    debris->updateRange.x   = 0x800000;
    debris->updateRange.y   = 0x800000;
}

void EggPrison_State_Setup(void)
{
    RSDK_THIS(EggPrison);

    self->originY = self->position.y;
    self->state   = EggPrison_State_HandleBounds;
}

void EggPrison_State_HandleBounds(void)
{
    RSDK_THIS(EggPrison);

    for (int32 p = 0; p < Player->playerCount && self->type < EGGPRISON_DUD; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (!player->sidekick) {
            if (abs(self->position.x - player->position.x) < 0x1000000) {
                if (abs(self->position.y - player->position.y) < 0x1000000 && self->position.x - (Zone->cameraBoundsR[p] << 16) < 0x1000000) {
                    Zone->playerBoundActiveL[p] = true;
                    Zone->playerBoundActiveR[p] = true;

                    if (self->type == EGGPRISON_NORMAL) {
                        Zone->cameraBoundsL[p] = (self->position.x >> 0x10) - ScreenInfo[p].centerX;
                        Zone->cameraBoundsR[p] = (self->position.x >> 0x10) + ScreenInfo[p].centerX;
                    }
                }
            }
        }
    }
}

void EggPrison_State_Explode(void)
{
    RSDK_THIS(EggPrison);

    if (!(self->timer % 3)) {
        int32 x                    = self->position.x + (RSDK.Rand(-24, 24) << 16);
        int32 y                    = self->position.y + (RSDK.Rand(-24, 24) << 16);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(2 * (RSDK.Rand(0, 256) > 192) + EXPLOSION_ENEMY), x, y);
        explosion->drawOrder       = Zone->objectDrawHigh;
        RSDK.PlaySfx(EggPrison->sfxDestroy, false, 255);
    }

    if (++self->timer == 20) {
        self->timer = 0;
        if (self->type < EGGPRISON_DUD) {
            self->state = EggPrison_State_SetupActClear;
            Music_FadeOut(0.025);
        }
        else {
            self->state = StateMachine_None;
        }
    }
}

void EggPrison_State_SetupActClear(void)
{
    RSDK_THIS(EggPrison);

    if (++self->timer == 60) {
        self->timer               = 0;
        self->state               = StateMachine_None;
        Zone->forcePlayerOnScreen = false;
        Music_PlayTrack(TRACK_ACTCLEAR);
        RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->classID, NULL);
    }
}

void EggPrison_State_FlyOffScreen(void)
{
    RSDK_THIS(EggPrison);

    if (self->velocity.x > -0x30000)
        self->velocity.x -= 0x1000;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void EggPrison_EditorDraw(void)
{
    RSDK_THIS(EggPrison);

    Vector2 drawPos;
    if (self->type == EGGPRISON_FLYING) {
        RSDK.SetSpriteAnimation(EggPrison->aniFrames, 0, &self->capsuleAnimator, true, 1);
        RSDK.SetSpriteAnimation(EggPrison->aniFrames, 1, &self->buttonAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggPrison->aniFrames, 2, &self->panelAnimator, true, 1);
        RSDK.SetSpriteAnimation(EggPrison->aniFrames, 3, &self->propellerAnimator, true, 0);

        self->direction = FLIP_Y;
        drawPos.x       = self->position.x;
        drawPos.y       = self->position.y + self->buttonPos;
        RSDK.DrawSprite(&self->buttonAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->capsuleAnimator, NULL, false);
        RSDK.DrawSprite(&self->panelAnimator, NULL, false);
        RSDK.DrawSprite(&self->propellerAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->propellerAnimator, NULL, false);

        self->direction = FLIP_NONE;
    }
    else {
        RSDK.SetSpriteAnimation(EggPrison->aniFrames, 0, &self->capsuleAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggPrison->aniFrames, 1, &self->buttonAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggPrison->aniFrames, 2, &self->panelAnimator, true, 0);

        drawPos.x = self->position.x;
        drawPos.y = self->position.y + self->buttonPos;
        RSDK.DrawSprite(&self->buttonAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->capsuleAnimator, NULL, false);
        RSDK.DrawSprite(&self->panelAnimator, NULL, false);
    }
}

void EggPrison_EditorLoad(void)
{
    EggPrison->aniFrames = RSDK.LoadSpriteAnimation("Global/EggPrison.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(EggPrison, type);
    RSDK_ENUM_VAR("Normal", EGGPRISON_NORMAL);
    RSDK_ENUM_VAR("Flying", EGGPRISON_FLYING);
    RSDK_ENUM_VAR("Dud", EGGPRISON_DUD);
    RSDK_ENUM_VAR("Rings", EGGPRISON_RINGS);
    RSDK_ENUM_VAR("Trap", EGGPRISON_TRAP);
    RSDK_ENUM_VAR("Spring", EGGPRISON_SPRING);
}
#endif

void EggPrison_Serialize(void) { RSDK_EDITABLE_VAR(EggPrison, VAR_ENUM, type); }
