// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MagnetSphere Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMagnetSphere *MagnetSphere;

void MagnetSphere_Update(void)
{
    RSDK_THIS(MagnetSphere);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator5);
    MagnetSphere_CheckPlayerCollision();
}

void MagnetSphere_LateUpdate(void) {}

void MagnetSphere_StaticUpdate(void) {}

void MagnetSphere_Draw(void)
{
    RSDK_THIS(MagnetSphere);

    self->inkEffect = INK_SUB;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    self->direction = INK_ADD;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    self->direction = INK_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator5, NULL, false);
}

void MagnetSphere_Create(void *data)
{
    RSDK_THIS(MagnetSphere);
    self->drawFX        = FX_FLIP;
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderHigh;
    self->active        = ACTIVE_BOUNDS;
    self->alpha         = 255;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 0, &self->animator2, true, 1);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 1, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 1, &self->animator4, true, 8);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 2, &self->animator5, true, 0);
}

void MagnetSphere_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
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
        int32 pid = RSDK.GetEntityID(player);

        if ((1 << pid) & self->activePlayers) {
            if (player->state != Player_State_None) {
                self->playerTimers[pid] = 30;
                self->activePlayers &= ~(1 << pid);
                if (player->state != Player_State_Die)
                    player->tileCollisions = true;
            }
            else if (!player->jumpPress) {
                if (player->left) {
                    --MagnetSphere->playerAngles[pid];
                }
                else if (player->right) {
                    ++MagnetSphere->playerAngles[pid];
                }
                MagnetSphere_MovePlayer(player, pid);
            }
            else {
                int32 ang = 12 * RSDK.Sin256(MagnetSphere->angleList[MagnetSphere->sphereAngle[pid] >> 4] | (MagnetSphere->sphereAngle[pid] & 0xF));
                player->velocity.x        = ang * RSDK.Sin256(MagnetSphere->playerAngles[pid]);
                player->velocity.y        = -(ang * RSDK.Cos256(MagnetSphere->playerAngles[pid]));
                player->jumpAbility       = 0;
                player->jumpAbilityTimer  = 1;
                player->tileCollisions    = true;
                player->state             = Player_State_Air;
                self->playerTimers[pid] = 30;
                self->activePlayers &= ~(1 << pid);
                RSDK.PlaySfx(Player->sfxJump, false, 255);
            }
        }
        else {
            if (!self->playerTimers[pid] && player->state != Player_State_None) {
                if (Player_CheckCollisionTouch(player, self, &MagnetSphere->hitbox)) {
                    int32 angle = RSDK.ATan2(self->position.x - player->position.x, self->position.y - player->position.y);

                    if (angle >= 128) {
                        MagnetSphere->sphereAngle[pid]  = -128;
                        MagnetSphere->playerAngles[pid] = angle + -128;
                    }
                    else {
                        MagnetSphere->sphereAngle[pid]  = 0;
                        MagnetSphere->playerAngles[pid] = angle;
                    }
                    MagnetSphere->playerAngles[pid] += 64;
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    player->groundVel  = 0;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->onGround        = false;
                    player->tileCollisions  = false;
                    player->state           = Player_State_None;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    self->activePlayers |= 1 << pid;
                    MagnetSphere_MovePlayer(player, pid);
                }
            }
            else if (self->playerTimers[pid]) {
                self->playerTimers[pid]--;
            }
        }
    }
}

void MagnetSphere_MovePlayer(void *p, int32 playerID)
{
    RSDK_THIS(MagnetSphere);
    EntityPlayer *player = (EntityPlayer *)p;

    int32 ang            = 56 * RSDK.Cos256(MagnetSphere->sphereAngle[playerID]) >> 8;
    player->velocity.x = -RSDK.Sin256(MagnetSphere->playerAngles[playerID]);
    player->velocity.y = RSDK.Cos256(MagnetSphere->playerAngles[playerID]);
    player->position.x = self->position.x - (ang * RSDK.Sin256(MagnetSphere->playerAngles[playerID]) << 8);
    player->position.y = self->position.y + (ang * RSDK.Cos256(MagnetSphere->playerAngles[playerID]) << 8);

    if (MagnetSphere->sphereAngle[playerID] >= 0x80) {
        player->drawOrder = Zone->playerDrawHigh;
    }
    else {
        player->drawOrder  = Zone->playerDrawLow;
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

#if RETRO_INCLUDE_EDITOR
void MagnetSphere_EditorDraw(void) { MagnetSphere_Draw(); }

void MagnetSphere_EditorLoad(void) { MagnetSphere->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MagnetSphere.bin", SCOPE_STAGE); }
#endif

void MagnetSphere_Serialize(void) {}
