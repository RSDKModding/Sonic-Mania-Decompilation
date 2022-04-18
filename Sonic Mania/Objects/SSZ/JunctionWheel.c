// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: JunctionWheel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectJunctionWheel *JunctionWheel;

void JunctionWheel_Update(void)
{
    RSDK_THIS(JunctionWheel);
    if (RSDK_GET_ENTITY((SceneInfo->entitySlot - 1), Button)->currentlyActive)
        self->spinDir ^= 1;

    if (self->spinDir)
        self->rotation += 4;
    else
        self->rotation -= 4;
    self->rotation &= 0x1FF;

    int32 direction = ((self->rotation + 48) >> 6) & 7;

    foreach_active(Player, player)
    {
        if (player->state == Player_State_None) {
            if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitboxEnteredWheel)) {
                player->position.x += player->groundVel * RSDK.Cos512(self->rotation) + self->position.x;
                player->position.y += player->groundVel * RSDK.Sin512(self->rotation) + self->position.y;

                if (player->groundVel < -0x1000)
                    player->groundVel += 0x200;

                if (player->groundVel == -0x1000) {
                    if (self->rotation == 0x180) {
                        player->state          = Player_State_Air;
                        player->tileCollisions = true;
                        player->groundVel      = 0;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0x80000;
                        player->onGround       = false;
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }

                    if (self->spinDir == 1 && self->rotation == 0x11C) {
                        player->state          = Player_State_Air;
                        player->tileCollisions = true;
                        player->groundVel      = 0x80000;
                        player->velocity.x     = 0x80000;
                        player->velocity.y     = 0x40000;
                        player->onGround       = false;
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }
                }
            }
        }
        else {
            switch (direction) {
                case 1:
                    Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolid);

                    if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitboxEntry1)) {
                        player->state = Player_State_None;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                        player->onGround  = false;
                        player->groundVel = -0x1C00;

                        player->tileCollisions  = false;
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        RSDK.PlaySfx(Player->sfxRoll, false, 255);
                    }
                    break;

                case 0:
                case 2:
                case 3:
                case 4:
                case 6:
                case 7:
                    if (player->position.y < self->position.y)
                        Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolid2);

                    Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolid);
                    break;

                case 5:
                    if (player->velocity.x <= 0) {
                        Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolid2);

                        if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitboxEntry5)) {
                            player->state = Player_State_None;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            player->onGround  = false;
                            player->groundVel = -0x1800;
                        }
                        player->tileCollisions  = false;
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        RSDK.PlaySfx(Player->sfxRoll, false, 255);
                    }
                    break;
            }

            if (player->velocity.y < 0 || (player->onGround && player->collisionMode))
                Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolid3);
        }
    }
}

void JunctionWheel_LateUpdate(void) {}

void JunctionWheel_StaticUpdate(void) {}

void JunctionWheel_Draw(void)
{
    RSDK_THIS(JunctionWheel);
    RSDK.DrawSprite(&self->slotAnimator, NULL, false);
    RSDK.DrawSprite(&self->maskAnimator, NULL, false);
}

void JunctionWheel_Create(void *data)
{
    RSDK_THIS(JunctionWheel);
    self->visible       = true;
    self->drawOrder     = Zone->objectDrawLow;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_ROTATE;

    RSDK.SetSpriteAnimation(JunctionWheel->aniFrames, 0, &self->slotAnimator, true, 0);
    RSDK.SetSpriteAnimation(JunctionWheel->aniFrames, 1, &self->maskAnimator, true, 0);
}

void JunctionWheel_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        JunctionWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/JunctionWheel.bin", SCOPE_STAGE);

    JunctionWheel->hitboxEnteredWheel.left   = -192;
    JunctionWheel->hitboxEnteredWheel.top    = -192;
    JunctionWheel->hitboxEnteredWheel.right  = 192;
    JunctionWheel->hitboxEnteredWheel.bottom = 192;

    JunctionWheel->hitboxSolid2.left   = -40;
    JunctionWheel->hitboxSolid2.top    = -56;
    JunctionWheel->hitboxSolid2.right  = -16;
    JunctionWheel->hitboxSolid2.bottom = 32;

    JunctionWheel->hitboxSolid.left   = 16;
    JunctionWheel->hitboxSolid.top    = -56;
    JunctionWheel->hitboxSolid.right  = 40;
    JunctionWheel->hitboxSolid.bottom = 32;

    JunctionWheel->hitboxSolid3.left   = -40;
    JunctionWheel->hitboxSolid3.top    = 32;
    JunctionWheel->hitboxSolid3.right  = 40;
    JunctionWheel->hitboxSolid3.bottom = 56;

    JunctionWheel->hitboxEntry1.left   = -40;
    JunctionWheel->hitboxEntry1.top    = -40;
    JunctionWheel->hitboxEntry1.right  = -16;
    JunctionWheel->hitboxEntry1.bottom = -8;

    JunctionWheel->hitboxEntry5.left   = 16;
    JunctionWheel->hitboxEntry5.top    = 8;
    JunctionWheel->hitboxEntry5.right  = 38;
    JunctionWheel->hitboxEntry5.bottom = 40;
}

#if RETRO_INCLUDE_EDITOR
void JunctionWheel_EditorDraw(void) { JunctionWheel_Draw(); }

void JunctionWheel_EditorLoad(void) { JunctionWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/JunctionWheel.bin", SCOPE_STAGE); }
#endif

void JunctionWheel_Serialize(void) {}
