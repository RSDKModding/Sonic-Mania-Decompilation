#include "SonicMania.h"

ObjectBallCannon *BallCannon;

void BallCannon_Update(void)
{
    RSDK_THIS(BallCannon);
    StateMachine_Run(self->state);
}

void BallCannon_LateUpdate(void) {}

void BallCannon_StaticUpdate(void) {}

void BallCannon_Draw(void)
{
    RSDK_THIS(BallCannon);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void BallCannon_Create(void *data)
{
    RSDK_THIS(BallCannon);
    self->drawOrder = Zone->playerDrawLow;
    self->visible   = true;
    if (!SceneInfo->inEditor) {
        if (data) {
            self->visible       = true;
            self->drawFX        = FX_ROTATE;
            self->active        = ACTIVE_NORMAL;
            self->rotationSpeed = RSDK.Rand(-8, 8);
            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &self->animator, true, voidToInt(data));
            self->state = BallCannon_Unknown8;
        }
        else {
            self->drawFX        = FX_ROTATE | FX_FLIP;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;

            switch (self->type) {
                case 0:
                    if (self->angle >= 4)
                        self->direction = FLIP_X;
                    self->rotation = (self->angle + self->direction + 1) << 7;
                    switch (self->angle) {
                        case 0:
                        case 5: self->velocity.y = 0x100000; break;
                        case 1:
                        case 6: self->velocity.x = -0x100000; break;
                        case 2:
                        case 7: self->velocity.y = -0x100000; break;
                        case 3:
                        case 4: self->velocity.x = 0x100000; break;
                        default: break;
                    }
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &self->animator, true, 0);
                    self->state = BallCannon_Unknown2;
                    break;
                case 1:
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &self->animator, true, 0);
                    self->velocity.y = -0x80000;
                    self->state      = BallCannon_StateCheckPlayerCollisions;
                    break;
                case 2:
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 4, &self->animator, true, 0);
                    self->velocity.x = 0x80000;
                    self->state      = BallCannon_StateCheckPlayerCollisions;
                    break;
            }
        }
    }
}

void BallCannon_StageLoad(void)
{
    BallCannon->aniFrames      = RSDK.LoadSpriteAnimation("OOZ/BallCannon.bin", SCOPE_STAGE);
    BallCannon->hitbox1.top    = -4;
    BallCannon->hitbox1.left   = -4;
    BallCannon->hitbox1.right  = 4;
    BallCannon->hitbox1.bottom = 4;
    BallCannon->hitbox2.top    = -16;
    BallCannon->hitbox2.left   = -16;
    BallCannon->hitbox2.right  = 16;
    BallCannon->hitbox2.bottom = 16;
    BallCannon->hitbox3.top    = -4;
    BallCannon->hitbox3.left   = -8;
    BallCannon->hitbox3.right  = 8;
    BallCannon->hitbox3.bottom = 4;
    BallCannon->sfxLedgeBreak  = RSDK.GetSFX("Stage/LedgeBreak.wav");
    BallCannon->sfxFire        = RSDK.GetSFX("Stage/CannonFire.wav");
}

void BallCannon_Unknown1(void)
{
    RSDK_THIS(BallCannon);
    if (RSDK.CheckOnScreen(self, NULL)) {
        foreach_all(Player, player)
        {
            if (Player_CheckValidState(player)) {
                int32 playerID = RSDK.GetEntityID(player);

                if (self->playerTimers[playerID]) {
                    self->playerTimers[playerID]--;
                }
                else {
                    if ((1 << playerID) & self->activePlayers) {
                        if (player->state != Player_State_None)
                            self->activePlayers &= ~(1 << playerID);
                    }
                    else {
                        if (Player_CheckCollisionTouch(player, self, &BallCannon->hitbox1)) {
                            RSDK.PlaySfx(Player->sfxRoll, false, 255);
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                            player->position.x     = self->position.x;
                            player->position.y     = self->position.y;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            player->tileCollisions = false;
                            player->interaction    = false;
                            player->blinkTimer     = 0;
                            player->visible        = false;
                            player->state          = Player_State_None;
                            self->activePlayers |= 1 << playerID;
                            self->active   = ACTIVE_NORMAL;
                            self->rotation = (self->direction + 1 + self->angle) << 7;
                            self->drawFX   = FX_ROTATE;
                            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &self->animator, true, 0);
                            self->state = BallCannon_Unknown3;
                        }
                    }
                }
            }
        }
    }
    else {
        self->active   = ACTIVE_BOUNDS;
        self->rotation = (self->direction + self->angle + 1) << 7;
    }
}

void BallCannon_Unknown2(void) { BallCannon_Unknown1(); }

void BallCannon_Unknown3(void)
{
    RSDK_THIS(BallCannon);
    BallCannon_Unknown1();
    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(BallCannon->aniFrames, 1, &self->animator, true, 0);
        self->drawFX = FX_FLIP;
        self->state  = BallCannon_Unknown4;
    }
}

void BallCannon_Unknown4(void)
{
    RSDK_THIS(BallCannon);
    BallCannon_Unknown1();
    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(BallCannon->aniFrames, 2, &self->animator, true, 0);
        self->drawFX   = FX_ROTATE;
        self->state    = BallCannon_Unknown5;
        self->rotation = (self->angle - self->direction + 2) << 7;
    }
}

void BallCannon_Unknown5(void)
{
    RSDK_THIS(BallCannon);
    BallCannon_Unknown1();
    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        foreach_all(Player, player)
        {
            if (Player_CheckValidState(player)) {
                int32 playerID = RSDK.GetEntityID(player);

                if (((1 << playerID) & self->activePlayers)) {
                    RSDK.PlaySfx(BallCannon->sfxFire, false, 255);
                    player->velocity = self->velocity;
                    player->visible  = true;
                    if (self->exit) {
                        player->onGround       = false;
                        player->jumpAbility    = 0;
                        player->state          = Player_State_Air;
                        player->tileCollisions = true;
                        player->interaction    = true;
                    }
                    self->activePlayers &= ~(1 << playerID);
                    self->playerTimers[playerID] = 15;
                }
            }
        }
        self->state = BallCannon_Unknown2;
    }
}

void BallCannon_StateCheckPlayerCollisions(void)
{
    RSDK_THIS(BallCannon);

    foreach_active(Player, player)
    {
        Animator *animator = &player->playerAnimator;
        int32 storeVelY      = player->velocity.y;
        int32 storeVelX      = player->velocity.x;
        int32 storeX         = player->position.x;
        int32 storeY         = player->position.y;

        if (Player_CheckCollisionBox(player, self, &BallCannon->hitbox2) == 1) {
            if (player->playerAnimator.animationID == ANI_JUMP || player->state == Player_State_DropDash
#if RETRO_USE_PLUS
                || player->state == Player_State_MightyHammerDrop
#endif
                ) {
                if (storeVelY >= 0 && !player->groundedStore) {
                    for (int32 i = 0; i < 16; ++i) {
                        EntityBallCannon *debris =
                            CREATE_ENTITY(BallCannon, intToVoid((i & 3) + 1), self->position.x + BallCannon->array1[(i * 2) + 0],
                                          self->position.y + BallCannon->array2[(i * 2) + 1]);
                        debris->velocity.x = BallCannon->array2[(i * 2) + 0];
                        debris->velocity.y = BallCannon->array2[(i * 2) + 1];
                    }

                    RSDK.PlaySfx(BallCannon->sfxLedgeBreak, false, 255);
                    memcpy(&player->playerAnimator, animator, sizeof(Animator));
                    player->velocity.x = storeVelX;
                    player->velocity.y = storeVelY;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    player->onGround   = false;
                    self->active     = ACTIVE_NORMAL;
                    self->visible    = false;
                    self->state      = BallCannon_Unknown7;
                    foreach_break;
                }
            }
        }
    }
}

void BallCannon_Unknown7(void)
{
    RSDK_THIS(BallCannon);

    if (RSDK.CheckOnScreen(self, NULL)) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);

            if (self->playerTimers[playerID]) {
                self->playerTimers[playerID]--;
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &BallCannon->hitbox3)) {
                    RSDK.SetSpriteAnimation(player->aniFrames, 10, &player->playerAnimator, false, 0);
                    RSDK.PlaySfx(BallCannon->sfxFire, false, 255);
                    player->state                  = Player_State_None;
                    player->nextGroundState        = 0;
                    player->nextAirState           = 0;
                    player->position.x             = self->position.x;
                    player->position.y             = self->position.y;
                    player->velocity               = self->velocity;
                    player->tileCollisions         = false;
                    player->interaction            = false;
                    player->onGround               = false;
                    self->playerTimers[playerID] = 15;
                }
            }
        }
    }
    else {
        self->visible = true;
        for (int32 i = 0; i < Player->playerCount; ++i) {
            self->playerTimers[i] = 0;
        }
        self->state = BallCannon_StateCheckPlayerCollisions;
    }
}

void BallCannon_Unknown8(void)
{
    RSDK_THIS(BallCannon);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->rotation += self->rotationSpeed;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void BallCannon_EditorDraw(void)
{
    RSDK_THIS(BallCannon);

    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    switch (self->type) {
        case 0:
            if (self->angle >= 4)
                self->direction = FLIP_X;
            self->rotation = (self->angle + self->direction + 1) << 7;
            switch (self->angle) {
                case 0:
                case 5: self->velocity.y = 0x100000; break;
                case 1:
                case 6: self->velocity.x = -0x100000; break;
                case 2:
                case 7: self->velocity.y = -0x100000; break;
                case 3:
                case 4: self->velocity.x = 0x100000; break;
                default: break;
            }
            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &self->animator, true, 0);
            break;
        case 1:
            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &self->animator, true, 0);
            break;
        case 2:
            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 4, &self->animator, true, 0);
            break;
    }

    BallCannon_Draw();
}

void BallCannon_EditorLoad(void) { BallCannon->aniFrames = RSDK.LoadSpriteAnimation("OOZ/BallCannon.bin", SCOPE_STAGE); }
#endif

void BallCannon_Serialize(void)
{
    RSDK_EDITABLE_VAR(BallCannon, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BallCannon, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(BallCannon, VAR_BOOL, exit);
}
