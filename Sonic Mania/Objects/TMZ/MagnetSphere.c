#include "SonicMania.h"

ObjectMagnetSphere *MagnetSphere;

void MagnetSphere_Update(void)
{
    RSDK_THIS(MagnetSphere);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);
    MagnetSphere_CheckPlayerCollision();
}

void MagnetSphere_LateUpdate(void) {}

void MagnetSphere_StaticUpdate(void) {}

void MagnetSphere_Draw(void)
{
    RSDK_THIS(MagnetSphere);

    entity->inkEffect = INK_SUB;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    entity->direction = INK_ADD;
    RSDK.DrawSprite(&entity->animator4, NULL, false);
    entity->direction = INK_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator5, NULL, false);
}

void MagnetSphere_Create(void *data)
{
    RSDK_THIS(MagnetSphere);
    entity->drawFX        = FX_FLIP;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->active        = ACTIVE_BOUNDS;
    entity->alpha         = 255;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 0, &entity->animator2, true, 1);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 1, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 1, &entity->animator4, true, 8);
    RSDK.SetSpriteAnimation(MagnetSphere->aniFrames, 2, &entity->animator5, true, 0);
}

void MagnetSphere_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        MagnetSphere->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MagnetSphere.bin", SCOPE_STAGE);
    MagnetSphere->hitbox.left   = -56;
    MagnetSphere->hitbox.top    = -56;
    MagnetSphere->hitbox.right  = 56;
    MagnetSphere->hitbox.bottom = 56;
    MagnetSphere->sfxBumper     = RSDK.GetSFX("Stage/Bumper3.wav");
    MagnetSphere->sfxPlasmaBall = RSDK.GetSFX("TMZ1/PlasmaBall.wav");
    DEBUGMODE_ADD_OBJ(MagnetSphere);
}

void MagnetSphere_DebugSpawn(void)
{
    RSDK.CreateEntity(MagnetSphere->objectID, 0, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y);
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
        int pid = RSDK.GetEntityID(player);

        if ((1 << pid) & entity->activePlayers) {
            if (player->state != Player_State_None) {
                entity->playerTimers[pid] = 30;
                entity->activePlayers &= ~(1 << pid);
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
                int ang = 12 * RSDK.Sin256(MagnetSphere->angleList[MagnetSphere->sphereAngle[pid] >> 4] | (MagnetSphere->sphereAngle[pid] & 0xF));
                player->velocity.x        = ang * RSDK.Sin256(MagnetSphere->playerAngles[pid]);
                player->velocity.y        = -(ang * RSDK.Cos256(MagnetSphere->playerAngles[pid]));
                player->jumpAbility       = 0;
                player->jumpAbilityTimer  = 1;
                player->tileCollisions    = true;
                player->state             = Player_State_Air;
                entity->playerTimers[pid] = 30;
                entity->activePlayers &= ~(1 << pid);
                RSDK.PlaySFX(Player->sfx_Jump, 0, 255);
            }
        }
        else {
            if (!entity->playerTimers[pid] && player->state != Player_State_None) {
                if (Player_CheckCollisionTouch(player, entity, &MagnetSphere->hitbox)) {
                    int angle = RSDK.ATan2(entity->position.x - player->position.x, entity->position.y - player->position.y);

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
                    RSDK.SetSpriteAnimation(player->spriteIndex, 10, &player->playerAnimator, 0, 0);
                    player->onGround        = 0;
                    player->tileCollisions  = 0;
                    player->state           = Player_State_None;
                    player->nextAirState    = 0;
                    player->nextGroundState = 0;
                    entity->activePlayers |= 1 << pid;
                    MagnetSphere_MovePlayer(player, pid);
                }
            }
            else if (entity->playerTimers[pid]) {
                entity->playerTimers[pid]--;
            }
        }
    }
}

void MagnetSphere_MovePlayer(void *p, int playerID)
{
    RSDK_THIS(MagnetSphere);
    EntityPlayer *player = (EntityPlayer *)p;

    int ang            = 56 * RSDK.Cos256(MagnetSphere->sphereAngle[playerID]) >> 8;
    player->velocity.x = -RSDK.Sin256(MagnetSphere->playerAngles[playerID]);
    player->velocity.y = RSDK.Cos256(MagnetSphere->playerAngles[playerID]);
    player->position.x = entity->position.x - (ang * RSDK.Sin256(MagnetSphere->playerAngles[playerID]) << 8);
    player->position.y = entity->position.y + (ang * RSDK.Cos256(MagnetSphere->playerAngles[playerID]) << 8);

    if (MagnetSphere->sphereAngle[playerID] >= 0x80) {
        player->drawOrder = Zone->playerDrawHigh;
    }
    else {
        player->drawOrder  = Zone->playerDrawLow;
        player->velocity.x = -player->velocity.x;
        player->velocity.y = -player->velocity.y;
    }

    byte angle = MagnetSphere->sphereAngle[playerID];
    MagnetSphere->sphereAngle[playerID] += 4;
    if ((((MagnetSphere->sphereAngle[playerID] + 64) >> 7) & 1) != ((angle + 64) >> 7) && !player->sidekick)
        RSDK.PlaySFX(MagnetSphere->sfxPlasmaBall, 0, 255);
}

void MagnetSphere_EditorDraw(void) {}

void MagnetSphere_EditorLoad(void) {}

void MagnetSphere_Serialize(void) {}
