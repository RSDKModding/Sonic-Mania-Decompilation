#include "SonicMania.h"

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
    if (SceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        RSDK.DrawSprite(&self->animator2, NULL, false);
        if (!SceneInfo->currentScreenID)
            RSDK.AddDrawListRef(Zone->drawOrderHigh, SceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
        RSDK.DrawSprite(&self->animator3, NULL, false);
    }
}

void EscapeCar_Create(void *data)
{
    RSDK_THIS(EscapeCar);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE) {
            self->state = EscapeCar_StateEncore_Unknown1;
            RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 8, &self->animator2, true, 0);
        }
        else {
#endif
            switch (globals->playerID & 0xFF) {
                case ID_TAILS:
#if RETRO_USE_PLUS
                case ID_MIGHTY:
                case ID_RAY:
#endif
                    self->type = 0;
                    break;
                case ID_KNUCKLES:
                    if (((globals->playerID >> 8) & 0xFF) == ID_KNUCKLES)
                        self->type = 3;
                    else
                        self->type = 0;
                    break;
                default: self->type = 3; break;
            }
            self->state = EscapeCar_StateMania_Unknown1;
            RSDK.SetSpriteAnimation(EscapeCar->aniFrames, (self->type + 2), &self->animator2, true, 0);
#if RETRO_USE_PLUS
        }
#endif
        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 0, &self->animator1, true, 0);
    }
}

void EscapeCar_StageLoad(void)
{
    EscapeCar->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EscapeCar.bin", SCOPE_STAGE);

    EscapeCar->hitbox.left   = -16;
    EscapeCar->hitbox.top    = -16;
    EscapeCar->hitbox.right  = 16;
    EscapeCar->hitbox.bottom = 16;

    EscapeCar->sfxBumper = RSDK.GetSFX("Stage/Bumper3.wav");
    EscapeCar->sfxDrop   = RSDK.GetSFX("Stage/Drop.wav");
}

void EscapeCar_StateMania_Unknown1(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->animator2);

    //Why does this func have encore checks??? Encore has its own states..

    bool32 allAboard = true;
    foreach_active(Player, player)
    {
        if (player->state != Player_State_None && player->position.x > self->position.x - 0x400000 && player->sidekick) {
            player->velocity.x -= 0x1000;
        }
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_ENCORE && player->position.x > self->position.x - 0x400000)
            RSDK.SetSpriteAnimation(EscapeCar->aniFrames, self->type + 3, &self->animator2, true, 0);
#else
        if (player->position.x > self->position.x - 0x400000)
            RSDK.SetSpriteAnimation(EscapeCar->aniFrames, self->type + 3, &self->animator2, true, 0);
#endif

        if (Player_CheckCollisionTouch(player, self, &EscapeCar->hitbox)) {
            player->state      = Player_State_None;
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
            player->animator.animationSpeed = 0;
        }
        else {
            allAboard = false;
        }
    }

    if (self->animator2.animationID == self->type + 3) {
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_ENCORE) {
#endif
            if (self->animator2.frameID == self->animator2.frameCount - 1)
                RSDK.SetSpriteAnimation(EscapeCar->aniFrames, self->type + 4, &self->animator2, true, 0);
#if RETRO_USE_PLUS
        }
#endif
    }
    if (allAboard)
        self->state = EscapeCar_StateMania_CheckPlayerCollisions;
}

void EscapeCar_StateMania_CheckPlayerCollisions(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    if (++self->timer == 60)
        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 1, &self->animator3, true, 0);

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

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->velocity.x < 0x180000)
        self->velocity.x += 0x2800;
    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        player->state      = Player_State_None;
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

#if RETRO_USE_PLUS
void EscapeCar_StateEncore_Unknown1(void)
{
    RSDK_THIS(EscapeCar);

    if (self->position.x - 0x800000 < (ScreenInfo->position.x + ScreenInfo->centerX) << 16) {
        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 1, &self->animator3, true, 0);
        self->velocity.x = 0x6000;
        if (SaveGame->saveRAM->chaosEmeralds == 0x7F)
            self->state = EscapeCar_StateEncore_GoodEnd;
        else
            self->state = EscapeCar_StateEncore_BadEnd;
    }
}

void EscapeCar_StateEncore_BadEnd(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->velocity.x < 0x80000)
        self->velocity.x += 0x2800;
    self->position.x += self->velocity.x;
}

void EscapeCar_StateEncore_GoodEnd(void)
{
    RSDK_THIS(EscapeCar);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->velocity.x < 0x74000)
        self->velocity.x += 0x2800;

    self->position.x += self->velocity.x;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (abs(player1->position.x - self->position.x) < 0x100000) {
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, self->position.x, self->position.y);
        debris->state        = Debris_State_Fall;
        debris->gravity      = 0x3800;
        debris->velocity.x   = 0x30000;
        debris->velocity.y   = -0x78000;
        debris->drawFX       = FX_SCALE | FX_ROTATE;
        debris->scale.x      = 0x200;
        debris->scale.y      = 0x200;
        debris->drawOrder    = Zone->drawOrderLow + 1;
        debris->rotSpeed     = 2;
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 7, &debris->animator, true, 0);
        debris->animator.rotationFlag = 1;
        debris->isPermanent           = true;

        debris                        = CREATE_ENTITY(Debris, NULL, self->position.x, self->position.y);
        debris->state                 = Debris_State_Fall;
        debris->gravity               = 0x3800;
        debris->velocity.x            = 0x40000;
        debris->velocity.y            = -0x48000;
        debris->drawFX                = FX_ROTATE;
        debris->drawOrder             = Zone->drawOrderLow + 1;
        debris->rotSpeed              = -4;
        debris->animator              = self->animator1;
        debris->isPermanent           = true;
        debris->animator.rotationFlag = 1;

        RSDK.PlaySfx(EscapeCar->sfxBumper, false, 255);
        destroyEntity(self);
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void EscapeCar_EditorDraw(void)
{
    RSDK_THIS(EscapeCar);
    self->drawOrder     = Zone->drawOrderLow;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE) {
        self->state = EscapeCar_StateEncore_Unknown1;
        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 8, &self->animator2, true, 0);
    }
    else {
#endif
        switch (globals->playerID & 0xFF) {
            case ID_TAILS:
#if RETRO_USE_PLUS
            case ID_MIGHTY:
            case ID_RAY:
#endif
                self->type = 0;
                break;
            case ID_KNUCKLES:
                if (((globals->playerID >> 8) & 0xFF) == ID_KNUCKLES)
                    self->type = 3;
                else
                    self->type = 0;
                break;
            default: self->type = 3; break;
        }
        RSDK.SetSpriteAnimation(EscapeCar->aniFrames, (self->type + 2), &self->animator2, true, 0);
#if RETRO_USE_PLUS
    }
#endif
    RSDK.SetSpriteAnimation(EscapeCar->aniFrames, 0, &self->animator1, true, 0);

    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void EscapeCar_EditorLoad(void) { EscapeCar->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EscapeCar.bin", SCOPE_STAGE); }
#endif

void EscapeCar_Serialize(void) {}
