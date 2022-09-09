// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TetherBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTetherBall *TetherBall;

void TetherBall_Update(void)
{
    RSDK_THIS(TetherBall);

    StateMachine_Run(self->state);
}

void TetherBall_LateUpdate(void) {}

void TetherBall_StaticUpdate(void) {}

void TetherBall_Draw(void)
{
    RSDK_THIS(TetherBall);
    Vector2 drawPos;

    int32 i = 0x200;
    for (; i < self->size; i += 0x400) {
        TetherBall->animator.frameID = 2;
        drawPos                      = self->position;
        drawPos.x += i * RSDK.Cos1024(self->angle);
        drawPos.y += i * RSDK.Sin1024(self->angle);
        RSDK.DrawSprite(&TetherBall->animator, &drawPos, false);
    }

    TetherBall->animator.frameID = 3;
    drawPos                      = self->position;
    drawPos.x += (i + 0x200) * RSDK.Cos1024(self->angle);
    drawPos.y += (i + 0x200) * RSDK.Sin1024(self->angle);
    RSDK.DrawSprite(&TetherBall->animator, &drawPos, false);

    self->ballPos = drawPos;

    TetherBall->animator.frameID = self->type >> 1;
    self->drawFX                 = FX_FLIP;
    RSDK.DrawSprite(&TetherBall->animator, NULL, false);

    self->drawFX = FX_NONE;
}

void TetherBall_Create(void *data)
{
    RSDK_THIS(TetherBall);

    switch (self->type) {
        case 0:
        case 2: self->direction = FLIP_NONE; break;

        case 1: self->direction = FLIP_Y; break;
        case 3: self->direction = FLIP_X; break;

        default: break;
    }

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->size          = (self->chainCount << 10) + 0x200;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];

        // Another static hitbox being initialized in Create...
        TetherBall->hitbox.top    = -40;
        TetherBall->hitbox.left   = -40;
        TetherBall->hitbox.right  = 40;
        TetherBall->hitbox.bottom = 40;

        self->angle = self->angleStart;
        if (self->angleStart > self->angleEnd) {
            self->angleStart = self->angleEnd;
            self->angleEnd   = self->angle;
        }

        self->state = TetherBall_State_CheckPlayerAttach;
    }
}

void TetherBall_StageLoad(void)
{
    TetherBall->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TetherBall.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(TetherBall->aniFrames, 0, &TetherBall->animator, true, 0);

    TetherBall->sfxWhack = RSDK.GetSfx("Stage/Whack.wav");
}

void TetherBall_State_CheckPlayerAttach(void)
{
    RSDK_THIS(TetherBall);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position = self->ballPos;
    self->rotation = self->angle << 16;
    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &TetherBall->hitbox)) {
                switch ((self->angle >> 8) & 3) {
                    case 0:
                        if (!player->onGround && player->groundedStore) {
                            if (player->rotation > 0x70 && player->rotation < 0x90) {
                                self->swingSpeed        = 8 * player->velocity.y / self->chainCount;
                                self->player            = player;
                                player->state           = Player_State_Static;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                self->state             = TetherBall_State_SwingBall;
                            }
                        }
                        break;

                    case 1:
                        if (!player->onGround && player->groundedStore) {
                            if (player->rotation > 0xF0 && player->rotation < 0x110) {
                                self->swingSpeed        = 8 * -player->velocity.x / self->chainCount;
                                self->player            = player;
                                player->state           = Player_State_Static;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                self->state             = TetherBall_State_SwingBall;
                            }
                        }
                        break;

                    case 2:
                        if (!player->onGround && player->groundedStore) {
                            if (player->rotation > 0x170 && player->rotation < 0x190) {
                                self->swingSpeed        = 8 * -player->velocity.y / self->chainCount;
                                self->player            = player;
                                player->state           = Player_State_Static;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                self->state             = TetherBall_State_SwingBall;
                            }
                        }
                        break;

                    case 3:
                        if (!player->onGround && player->groundedStore) {
                            if (player->rotation < 0x10 || player->rotation > 0x1F0) {
                                self->swingSpeed        = 8 * player->velocity.x / self->chainCount;
                                self->player            = player;
                                player->state           = Player_State_Static;
                                player->nextGroundState = StateMachine_None;
                                player->nextAirState    = StateMachine_None;
                                self->state             = TetherBall_State_SwingBall;
                            }
                        }
                        break;
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}
void TetherBall_State_SwingBall(void)
{
    RSDK_THIS(TetherBall);

    self->rotation += self->swingSpeed;
    self->angle = self->rotation >> 16;

    if ((uint32)(self->angle - 0x101) > 0x1FE)
        self->swingSpeed += 0x1800;
    else
        self->swingSpeed -= 0x1800;

    if (self->swingSpeed >= 0) {
        if (self->angle >= self->angleEnd) {
            self->angle    = self->angleEnd;
            self->rotation = self->angleEnd << 16;
            self->state    = TetherBall_State_FinishedSwing;
            RSDK.PlaySfx(TetherBall->sfxWhack, false, 255);
        }
    }
    else {
        if (self->angle <= self->angleStart) {
            self->angle    = self->angleStart;
            self->rotation = self->angleStart << 16;
            self->state    = TetherBall_State_FinishedSwing;
            RSDK.PlaySfx(TetherBall->sfxWhack, false, 255);
        }
    }

    EntityPlayer *player = self->player;
    if (player) {
        if (Player_CheckValidState(player)) {
            Hitbox *playerHitbox    = Player_GetHitbox(player);
            player->angle           = ((self->angle >> 2) + 64) & 0xFF;
            player->rotation        = player->angle << 1;
            player->position.x      = self->position.x + ((self->size + ((playerHitbox->bottom + 24) << 6)) * RSDK.Cos1024(self->angle));
            player->position.y      = self->position.y + ((self->size + ((playerHitbox->bottom + 24) << 6)) * RSDK.Sin1024(self->angle));
            player->onGround        = true;
            player->nextGroundState = StateMachine_None;
            player->nextAirState    = StateMachine_None;
            player->groundVel       = (self->swingSpeed * self->chainCount) >> 3;

            if (self->state == TetherBall_State_SwingBall) {
                if (player->jumpPress) {
                    self->player = NULL;
                    Player_Action_Jump(player);
                }
            }
            else {
                int32 vel = player->groundVel <= 0 ? -0x60000 : 0x60000;

                switch ((self->angle >> 8) & 3) {
                    case 0:
                        player->collisionMode = CMODE_RWALL;
                        player->position.y += vel;
                        break;

                    case 1:
                        player->collisionMode = CMODE_ROOF;
                        player->position.x -= vel;
                        break;

                    case 2:
                        player->collisionMode = CMODE_LWALL;
                        player->position.y -= vel;
                        break;

                    case 3:
                        player->collisionMode = CMODE_FLOOR;
                        player->position.x += vel;
                        break;
                }

                self->player = NULL;
                self->timer  = 0;
                if (player->animator.animationID == ANI_JUMP)
                    player->state = Player_State_Roll;
                else
                    player->state = Player_State_Ground;
            }
        }
        else {
            self->player = NULL;
        }
    }
}
void TetherBall_State_FinishedSwing(void)
{
    RSDK_THIS(TetherBall);

    if (++self->timer == 8) {
        self->timer = 0;
        self->state = TetherBall_State_CheckPlayerAttach;
    }
}

#if RETRO_INCLUDE_EDITOR
void TetherBall_EditorDraw(void)
{
    RSDK_THIS(TetherBall);

    switch (self->type) {
        case TETHERBALL_DOWN:
        case TETHERBALL_LEFT: self->direction = FLIP_NONE; break;
        case TETHERBALL_UP: self->direction = FLIP_Y; break;
        case TETHERBALL_RIGHT: self->direction = FLIP_X; break;
        default: break;
    }

    self->size          = (self->chainCount << 10) + 0x200;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;

    self->angle = self->angleStart;

    TetherBall_Draw();

    if (showGizmos()) {
        self->inkEffect = INK_BLEND;
        self->angle     = self->angleEnd;

        TetherBall_Draw();

        self->inkEffect = INK_NONE;
    }
}

void TetherBall_EditorLoad(void)
{
    TetherBall->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TetherBall.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(TetherBall->aniFrames, 0, &TetherBall->animator, true, 0);

    RSDK_ACTIVE_VAR(TetherBall, type);
    RSDK_ENUM_VAR("Down", TETHERBALL_DOWN);
    RSDK_ENUM_VAR("Up", TETHERBALL_UP);
    RSDK_ENUM_VAR("Left", TETHERBALL_LEFT);
    RSDK_ENUM_VAR("Right", TETHERBALL_RIGHT);
}
#endif

void TetherBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(TetherBall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TetherBall, VAR_UINT8, chainCount);
    RSDK_EDITABLE_VAR(TetherBall, VAR_ENUM, angleStart);
    RSDK_EDITABLE_VAR(TetherBall, VAR_ENUM, angleEnd);
}
