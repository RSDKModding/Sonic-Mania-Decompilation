// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: JunctionWheel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        if (player->state == Player_State_Static) {
            if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitboxWheelRange)) {
                player->position.x = player->groundVel * RSDK.Cos512(self->rotation) + self->position.x;
                player->position.y = player->groundVel * RSDK.Sin512(self->rotation) + self->position.y;

                if (player->groundVel < -0x1000)
                    player->groundVel += 0x200;

                if (player->groundVel == -0x1000) {
                    if (self->rotation == 0x180) {
                        player->state          = Player_State_Air;
                        player->tileCollisions = TILECOLLISION_DOWN;
                        player->groundVel      = 0;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0x80000;
                        player->onGround       = false;
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }

                    if (self->spinDir == 1 && self->rotation == 0x11C) {
                        player->state          = Player_State_Air;
                        player->tileCollisions = TILECOLLISION_DOWN;
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
                    Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolidR);

                    if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitboxEntryR)) {
                        player->state = Player_State_Static;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                        player->onGround  = false;
                        player->groundVel = -0x1C00;

                        player->tileCollisions  = TILECOLLISION_NONE;
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
                        Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolidL);

                    Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolidR);
                    break;

                case 5:
                    if (player->velocity.x <= 0) {
                        Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolidL);

                        if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitboxEntryL)) {
                            player->state = Player_State_Static;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

                            player->onGround  = false;
                            player->groundVel = -0x1800;

                            player->tileCollisions  = TILECOLLISION_NONE;
                            player->nextAirState    = StateMachine_None;
                            player->nextGroundState = StateMachine_None;
                            player->velocity.x      = 0;
                            player->velocity.y      = 0;
                            RSDK.PlaySfx(Player->sfxRoll, false, 255);
                        }
                    }
                    break;
            }

            if (player->velocity.y < 0 || (player->onGround && player->collisionMode))
                Player_CheckCollisionBox(player, self, &JunctionWheel->hitboxSolidB);
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
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_ROTATE;

    RSDK.SetSpriteAnimation(JunctionWheel->aniFrames, 0, &self->slotAnimator, true, 0);
    RSDK.SetSpriteAnimation(JunctionWheel->aniFrames, 1, &self->maskAnimator, true, 0);
}

void JunctionWheel_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        JunctionWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/JunctionWheel.bin", SCOPE_STAGE);

    JunctionWheel->hitboxWheelRange.left   = -192;
    JunctionWheel->hitboxWheelRange.top    = -192;
    JunctionWheel->hitboxWheelRange.right  = 192;
    JunctionWheel->hitboxWheelRange.bottom = 192;

    JunctionWheel->hitboxSolidL.left   = -40;
    JunctionWheel->hitboxSolidL.top    = -56;
    JunctionWheel->hitboxSolidL.right  = -16;
    JunctionWheel->hitboxSolidL.bottom = 32;

    JunctionWheel->hitboxSolidR.left   = 16;
    JunctionWheel->hitboxSolidR.top    = -56;
    JunctionWheel->hitboxSolidR.right  = 40;
    JunctionWheel->hitboxSolidR.bottom = 32;

    JunctionWheel->hitboxSolidB.left   = -40;
    JunctionWheel->hitboxSolidB.top    = 32;
    JunctionWheel->hitboxSolidB.right  = 40;
    JunctionWheel->hitboxSolidB.bottom = 56;

    JunctionWheel->hitboxEntryR.left   = -40;
    JunctionWheel->hitboxEntryR.top    = -40;
    JunctionWheel->hitboxEntryR.right  = -16;
    JunctionWheel->hitboxEntryR.bottom = -8;

    JunctionWheel->hitboxEntryL.left   = 16;
    JunctionWheel->hitboxEntryL.top    = 8;
    JunctionWheel->hitboxEntryL.right  = 38;
    JunctionWheel->hitboxEntryL.bottom = 40;
}

#if GAME_INCLUDE_EDITOR
void JunctionWheel_EditorDraw(void)
{
    RSDK_THIS(JunctionWheel);

    JunctionWheel_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        // May also be something like a SSZ/SDashWheel since Common/Button doesn't exist in SSZ1 object list
        EntityButton *button = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, Button);

        if (button) {
            DrawHelpers_DrawArrow(button->position.x, button->position.y, self->position.x, self->position.y, 0xFFFF00, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void JunctionWheel_EditorLoad(void) { JunctionWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/JunctionWheel.bin", SCOPE_STAGE); }
#endif

void JunctionWheel_Serialize(void) {}
