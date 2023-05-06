// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MagnetSphere Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMagnetSphere *MagnetSphere;

void MagnetSphere_Update(void)
{
    RSDK_THIS(MagnetSphere);

    RSDK.ProcessAnimation(&self->plasma1Animator);
    RSDK.ProcessAnimation(&self->plasma2Animator);
    RSDK.ProcessAnimation(&self->centerBallAnimator);

    MagnetSphere_CheckPlayerCollision();
}

void MagnetSphere_LateUpdate(void) {}

void MagnetSphere_StaticUpdate(void) {}

void MagnetSphere_Draw(void)
{
    RSDK_THIS(MagnetSphere);

    self->inkEffect = INK_SUB;
    RSDK.DrawSprite(&self->glassInnerAnimator, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->plasma1Animator, NULL, false);

    self->direction = INK_ADD;
    RSDK.DrawSprite(&self->plasma2Animator, NULL, false);

    self->direction = INK_NONE;
    RSDK.DrawSprite(&self->glassOuterAnimator, NULL, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->centerBallAnimator, NULL, false);
}

void MagnetSphere_Create(void *data)
{
    RSDK_THIS(MagnetSphere);

    self->drawFX        = FX_FLIP;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[1];
    self->active        = ACTIVE_BOUNDS;
    self->alpha         = 0xFF;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 0, &self->glassInnerAnimator, true, 0);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 0, &self->glassOuterAnimator, true, 1);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 1, &self->plasma1Animator, true, 0);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 1, &self->plasma2Animator, true, 8);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 2, &self->centerBallAnimator, true, 0);
}

void MagnetSphere_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("TMZ1") || RSDK.CheckSceneFolder("TMZ2"))
        MagnetSphere->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MagnetSphere.bin", SCOPE_STAGE);

    MagnetSphere->hitbox.left   = -56;
    MagnetSphere->hitbox.top    = -56;
    MagnetSphere->hitbox.right  = 56;
    MagnetSphere->hitbox.bottom = 56;

    MagnetSphere->sfxBumper     = RSDK.GetSfx("Stage/Bumper3.wav");
    MagnetSphere->sfxPlasmaBall = RSDK.GetSfx("TMZ1/PlasmaBall.wav");

    DEBUGMODE_ADD_OBJ(MagnetSphere);
}

void MagnetSphere_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(MagnetSphere, NULL, self->position.x, self->position.y);
}

void MagnetSphere_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 0, &DebugMode->animator, true, 1);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void MagnetSphere_CheckPlayerCollision(void)
{
    RSDK_THIS(MagnetSphere);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((1 << playerID) & self->activePlayers) {
            if (player->state != Player_State_Static) {
                self->playerTimers[playerID] = 30;
                self->activePlayers &= ~(1 << playerID);

                if (player->state != Player_State_Death)
                    player->tileCollisions = TILECOLLISION_DOWN;
            }
            else if (!player->jumpPress) {
                if (player->left)
                    --MagnetSphere->playerAngles[playerID];
                else if (player->right)
                    ++MagnetSphere->playerAngles[playerID];

                MagnetSphere_MovePlayer(player, playerID);
            }
            else {
                int32 ang =
                    12
                    * RSDK.Sin256(MagnetSphere->angleTable[MagnetSphere->sphereAngle[playerID] >> 4] | (MagnetSphere->sphereAngle[playerID] & 0xF));

                player->velocity.x           = ang * RSDK.Sin256(MagnetSphere->playerAngles[playerID]);
                player->velocity.y           = -(ang * RSDK.Cos256(MagnetSphere->playerAngles[playerID]));
                player->applyJumpCap         = false;
                player->jumpAbilityState     = 1;
                player->tileCollisions       = TILECOLLISION_DOWN;
                player->state                = Player_State_Air;
                self->playerTimers[playerID] = 30;
                self->activePlayers &= ~(1 << playerID);

                RSDK.PlaySfx(Player->sfxJump, false, 255);
            }
        }
        else {
            if (!self->playerTimers[playerID] && player->state != Player_State_Static) {
                if (Player_CheckCollisionTouch(player, self, &MagnetSphere->hitbox)) {
                    int32 angle = RSDK.ATan2(self->position.x - player->position.x, self->position.y - player->position.y);

                    if (angle >= 0x80) {
                        MagnetSphere->sphereAngle[playerID]  = -0x80;
                        MagnetSphere->playerAngles[playerID] = angle + -0x80;
                    }
                    else {
                        MagnetSphere->sphereAngle[playerID]  = 0;
                        MagnetSphere->playerAngles[playerID] = angle;
                    }
                    MagnetSphere->playerAngles[playerID] += 0x40;

                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    player->groundVel  = 0;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

                    player->onGround        = false;
                    player->tileCollisions  = TILECOLLISION_NONE;
                    player->state           = Player_State_Static;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;

                    self->activePlayers |= 1 << playerID;
                    MagnetSphere_MovePlayer(player, playerID);
                }
            }
            else if (self->playerTimers[playerID]) {
                self->playerTimers[playerID]--;
            }
        }
    }
}

void MagnetSphere_MovePlayer(EntityPlayer *player, int32 playerID)
{
    RSDK_THIS(MagnetSphere);

    int32 ang          = 56 * RSDK.Cos256(MagnetSphere->sphereAngle[playerID]) >> 8;
    player->velocity.x = -RSDK.Sin256(MagnetSphere->playerAngles[playerID]);
    player->velocity.y = RSDK.Cos256(MagnetSphere->playerAngles[playerID]);
    player->position.x = self->position.x - (ang * RSDK.Sin256(MagnetSphere->playerAngles[playerID]) << 8);
    player->position.y = self->position.y + (ang * RSDK.Cos256(MagnetSphere->playerAngles[playerID]) << 8);

    if (MagnetSphere->sphereAngle[playerID] >= 0x80) {
        player->drawGroup = Zone->playerDrawGroup[1];
    }
    else {
        player->drawGroup  = Zone->playerDrawGroup[0];
        player->velocity.x = -player->velocity.x;
        player->velocity.y = -player->velocity.y;
    }

    uint8 angle = MagnetSphere->sphereAngle[playerID];
    MagnetSphere->sphereAngle[playerID] += 4;

    uint32 angVal    = (((uint32)MagnetSphere->sphereAngle[playerID] + 64) >> 7) & 1;
    uint8 angValPrev = ((angle + 64) >> 7) & 1;
    if (angVal != angValPrev && !player->sidekick)
        RSDK.PlaySfx(MagnetSphere->sfxPlasmaBall, false, 255);
}

#if GAME_INCLUDE_EDITOR
void MagnetSphere_EditorDraw(void) { MagnetSphere_Draw(); }

void MagnetSphere_EditorLoad(void) { MagnetSphere->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MagnetSphere.bin", SCOPE_STAGE); }
#endif

void MagnetSphere_Serialize(void) {}
