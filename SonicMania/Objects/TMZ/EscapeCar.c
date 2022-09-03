// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EscapeCar Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectEscapeCar *EscapeCar;

void EscapeCar_Update(void)
{
    RSDK_THIS(EscapeCar);

    StateMachine_Run(self->state);
}

void EscapeCar_LateUpdate(void) {}

void EscapeCar_StaticUpdate(void) {}

void EscapeCar_Draw(void)
{
    RSDK_THIS(EscapeCar);

    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0]) {
        RSDK.DrawSprite(&self->driverAnimator, NULL, false);

        if (!SceneInfo->currentScreenID)
            RSDK.AddDrawListRef(Zone->objectDrawGroup[1], SceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&self->carAnimator, NULL, false);
        RSDK.DrawSprite(&self->thrustAnimator, NULL, false);
    }
}

void EscapeCar_Create(void *data)
{
    RSDK_THIS(EscapeCar);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_ENCORE) {
            self->state = EscapeCar_StateEncore_Setup;
            RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 8, &self->driverAnimator, true, 0);
        }
        else {
#endif
            switch (GET_CHARACTER_ID(1)) {
                default:
                case ID_SONIC: self->driver = ESCAPECAR_DRIVER_KNUX; break;

                case ID_TAILS:
#if MANIA_USE_PLUS
                case ID_MIGHTY:
                case ID_RAY:
#endif
                    self->driver = ESCAPECAR_DRIVER_SONIC;
                    break;

                case ID_KNUCKLES: self->driver = CHECK_CHARACTER_ID(ID_KNUCKLES, 2) ? ESCAPECAR_DRIVER_KNUX : ESCAPECAR_DRIVER_SONIC; break;
            }
            self->state = EscapeCar_StateMania_AwaitPlayer;
            RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 2 + self->driver, &self->driverAnimator, true, 0);
#if MANIA_USE_PLUS
        }
#endif

        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 0, &self->carAnimator, true, 0);
    }
}

void EscapeCar_StageLoad(void)
{
    EscapeCar->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EscapeCar.bin", SCOPE_STAGE);

    EscapeCar->hitbox.left   = -16;
    EscapeCar->hitbox.top    = -16;
    EscapeCar->hitbox.right  = 16;
    EscapeCar->hitbox.bottom = 16;

    EscapeCar->sfxBumper = RSDK.GetSfx("Stage/Bumper3.wav");
    EscapeCar->sfxDrop   = RSDK.GetSfx("Stage/Drop.wav");
}

void EscapeCar_StateMania_AwaitPlayer(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->driverAnimator);

    // Why does this func have encore checks??? Encore has its own states..

    bool32 allAboard = true;
    foreach_active(Player, player)
    {
        if (player->state != Player_State_Static && player->position.x > self->position.x - 0x400000 && player->sidekick) {
            player->velocity.x -= 0x1000;
        }
#if MANIA_USE_PLUS
        if (globals->gameMode != MODE_ENCORE && player->position.x > self->position.x - 0x400000)
#else
        if (player->position.x > self->position.x - 0x400000)
#endif
            RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 3 + self->driver, &self->driverAnimator, true, 0);

        if (Player_CheckCollisionTouch(player, self, &EscapeCar->hitbox)) {
            player->state      = Player_State_Static;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->position.x = self->position.x;
            player->position.y = self->position.y;

            if (player->sidekick) {
                player->position.x -= 0x140000;
                player->position.y -= 0x140000;
            }
            else {
                player->position.x -= 0xC0000;
                player->position.y -= 0x80000;
            }

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_RIDE, &player->animator, false, 0);
            player->animator.speed = 0;
        }
        else {
            allAboard = false;
        }
    }

    if (self->driverAnimator.animationID == 3 + self->driver) {
#if MANIA_USE_PLUS
        if (globals->gameMode != MODE_ENCORE) {
#endif
            if (self->driverAnimator.frameID == self->driverAnimator.frameCount - 1)
                RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 4 + self->driver, &self->driverAnimator, true, 0);
#if MANIA_USE_PLUS
        }
#endif
    }

    if (allAboard)
        self->state = EscapeCar_StateMania_EnteredCar;
}

void EscapeCar_StateMania_EnteredCar(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->driverAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);

    if (++self->timer == 60)
        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 1, &self->thrustAnimator, true, 0);

    if (self->timer == 160) {
        foreach_active(Player, player) { RSDK.SetSpriteAnimation(player->aniFrames, ANI_RIDE, &player->animator, true, 0); }

        self->timer      = 0;
        self->velocity.x = 0x10000;
        self->state      = EscapeCar_StateMania_Ride;
    }
}

void EscapeCar_StateMania_Ride(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->driverAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);

    if (self->velocity.x < 0x180000)
        self->velocity.x += 0x2800;

    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        player->state      = Player_State_Static;
        player->velocity.x = 0;
        player->velocity.y = 0;
        player->position.x = self->position.x;
        player->position.y = self->position.y;

        if (player->sidekick) {
            player->position.x -= 0x140000;
            player->position.y -= 0x140000;
        }
        else {
            player->position.x -= 0xC0000;
            player->position.y -= 0x80000;
        }
    }
}

#if MANIA_USE_PLUS
void EscapeCar_StateEncore_Setup(void)
{
    RSDK_THIS(EscapeCar);

    if (self->position.x - 0x800000 < (ScreenInfo->position.x + ScreenInfo->center.x) << 16) {
        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 1, &self->thrustAnimator, true, 0);

        self->velocity.x = 0x6000;
        if (SaveGame_AllChaosEmeralds())
            self->state = EscapeCar_StateEncore_GoodEnd;
        else
            self->state = EscapeCar_StateEncore_BadEnd;
    }
}

void EscapeCar_StateEncore_BadEnd(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->driverAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);

    if (self->velocity.x < 0x80000)
        self->velocity.x += 0x2800;

    self->position.x += self->velocity.x;
}

void EscapeCar_StateEncore_GoodEnd(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->driverAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);

    if (self->velocity.x < 0x74000)
        self->velocity.x += 0x2800;

    self->position.x += self->velocity.x;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (abs(player1->position.x - self->position.x) < 0x100000) {
        EntityDebris *debris    = CREATE_ENTITY(Debris, NULL, self->position.x, self->position.y);
        debris->state           = Debris_State_Fall;
        debris->gravityStrength = 0x3800;
        debris->velocity.x      = 0x30000;
        debris->velocity.y      = -0x78000;
        debris->drawFX          = FX_SCALE | FX_ROTATE;
        debris->scale.x         = 0x200;
        debris->scale.y         = 0x200;
        debris->drawGroup       = Zone->objectDrawGroup[0] + 1;
        debris->rotSpeed        = 2;
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 7, &debris->animator, true, 0);
        debris->animator.rotationStyle = ROTSTYLE_FULL;
        debris->isPermanent            = true;

        debris                         = CREATE_ENTITY(Debris, NULL, self->position.x, self->position.y);
        debris->state                  = Debris_State_Fall;
        debris->gravityStrength        = 0x3800;
        debris->velocity.x             = 0x40000;
        debris->velocity.y             = -0x48000;
        debris->drawFX                 = FX_ROTATE;
        debris->drawGroup              = Zone->objectDrawGroup[0] + 1;
        debris->rotSpeed               = -4;
        debris->animator               = self->carAnimator;
        debris->isPermanent            = true;
        debris->animator.rotationStyle = ROTSTYLE_FULL;

        RSDK.PlaySfx(EscapeCar->sfxBumper, false, 255);
        destroyEntity(self);
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void EscapeCar_EditorDraw(void)
{
    RSDK_THIS(EscapeCar);

    self->drawGroup     = Zone->objectDrawGroup[0];
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_ENCORE) {
        self->state = EscapeCar_StateEncore_Setup;
        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 8, &self->driverAnimator, true, 0);
    }
    else {
#endif
        switch (GET_CHARACTER_ID(1)) {
            default:
            case ID_SONIC: self->driver = ESCAPECAR_DRIVER_KNUX; break;

            case ID_TAILS:
#if MANIA_USE_PLUS
            case ID_MIGHTY:
            case ID_RAY:
#endif
                self->driver = ESCAPECAR_DRIVER_SONIC;
                break;

            case ID_KNUCKLES: self->driver = CHECK_CHARACTER_ID(ID_KNUCKLES, 2) ? ESCAPECAR_DRIVER_KNUX : ESCAPECAR_DRIVER_SONIC; break;
        }

        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 2 + self->driver, &self->driverAnimator, true, 0);
#if MANIA_USE_PLUS
    }
#endif

    RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 0, &self->carAnimator, true, 0);

    RSDK.DrawSprite(&self->driverAnimator, NULL, false);
    RSDK.DrawSprite(&self->carAnimator, NULL, false);
    RSDK.DrawSprite(&self->thrustAnimator, NULL, false);
}

void EscapeCar_EditorLoad(void) { EscapeCar->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EscapeCar.bin", SCOPE_STAGE); }
#endif

void EscapeCar_Serialize(void) {}
