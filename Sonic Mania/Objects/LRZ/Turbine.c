#include "SonicMania.h"

ObjectTurbine *Turbine;

void Turbine_Update(void)
{
    RSDK_THIS(Turbine);

    entity->angle = (6 * Zone->timer) & 0x1FF;
    RSDK.SetSpriteAnimation(Turbine->aniFrames, entity->type, &entity->animator, true, (entity->angle >> 3) & 7);
    StateMachine_Run(entity->state);
}

void Turbine_LateUpdate(void) {}

void Turbine_StaticUpdate(void)
{
    bool32 flag = false;
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {

        foreach_active(Turbine, turbine)
        {
            if (!Turbine->playingTurbineSfx) {
                RSDK.PlaySfx(Turbine->sfxTurbine, 34161, 255);
                Turbine->playingTurbineSfx = true;
            }
            flag = true;
            foreach_break;
        }
    }

    if (!flag) {
        if (Turbine->playingTurbineSfx) {
            RSDK.StopSFX(Turbine->sfxTurbine);
            Turbine->playingTurbineSfx = false;
        }
    }
}

void Turbine_Draw(void)
{
    RSDK_THIS(Turbine);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Turbine_Create(void *data)
{
    RSDK_THIS(Turbine);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Turbine->aniFrames, entity->type, &entity->animator, true, 0);
        if (entity->type) {
            if (entity->type == 1)
                entity->state = Turbine_CheckPlayerCollisions_Hurt;
            else
                entity->state = StateMachine_None;
            entity->drawOrder = Zone->drawOrderHigh;
        }
        else {
            entity->state     = Turbine_CheckPlayerCollisions;
            entity->drawOrder = Zone->drawOrderHigh;
        }
    }
}

void Turbine_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ2"))
        Turbine->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Turbine.bin", SCOPE_STAGE);
    Turbine->hitbox1.left      = -16;
    Turbine->hitbox1.right     = 16;
    Turbine->hitbox1.top       = -48;
    Turbine->hitbox1.bottom    = 48;
    Turbine->hitboxHurt.left   = -6;
    Turbine->hitboxHurt.right  = 6;
    Turbine->hitboxHurt.top    = -44;
    Turbine->hitboxHurt.bottom = 44;
    Turbine->sfxTurbine        = RSDK.GetSFX("LRZ/Turbine.wav");
}

void Turbine_CheckPlayerCollisions(void)
{
    RSDK_THIS(Turbine);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);

        if (entity->playerTimers[playerID] > 0)
            entity->playerTimers[playerID]--;
        if (!((1 << playerID) & entity->activePlayers) && !entity->playerTimers[playerID] && player->playerAnimator.animationID != ANI_HURT
            && player->state != Player_State_None) {
            if (player->playerAnimator.animationID != ANI_HURT && player->state != Player_State_None) {
                if (Player_CheckCollisionTouch(player, entity, &Turbine->hitbox1)) {
                    entity->activePlayers |= 1 << playerID;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->onGround        = false;
                    player->state           = Player_State_None;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGH, &player->playerAnimator, true, 0);
                    player->playerAnimator.animationSpeed = 0;

                    if (player->position.y >= entity->position.y)
                        entity->playerAngles[playerID] = 0x80;
                    else
                        entity->playerAngles[playerID] = 0x180;
                    entity->playerAngles[playerID] += entity->angle & 0x3F;
                    RSDK.PlaySfx(Player->sfxGrab, false, 255);
                }
            }
        }
        else if ((1 << playerID) & entity->activePlayers) {
            player->velocity.x             = 0;
            player->velocity.y             = 0;
            player->groundVel              = 0;
            entity->playerAngles[playerID] = (entity->playerAngles[playerID] + 6) % 512;
            player->position.x             = entity->position.x;
            player->position.y             = entity->position.y;

            player->position.y += 0x1700 * RSDK.Sin512(entity->playerAngles[playerID]) + 0x20000;
            if ((uint32)(entity->playerAngles[playerID] - 0x81) > 0xFF)
                player->drawOrder = Zone->playerDrawLow;
            else
                player->drawOrder = Zone->playerDrawHigh;

            player->playerAnimator.frameID = (((entity->playerAngles[playerID] >> 1) / 21) + 9) % 12;

            if (player->jumpPress) {
                player->velocity.y = 0x600 * RSDK.Cos512(entity->playerAngles[playerID]);
                if (player->velocity.y < 0)
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                else
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_WALK, &player->playerAnimator, true, 0);

                entity->activePlayers &= ~(1 << playerID);
                player->state                  = Player_State_Air;
                entity->playerTimers[playerID] = 30;
            }
            else if (player->playerAnimator.animationID != ANI_POLESWINGH || player->state != Player_State_None) {
                entity->activePlayers &= ~(1 << playerID);
                player->state                  = Player_State_Air;
                entity->playerTimers[playerID] = 30;
            }
        }
    }
}

void Turbine_CheckPlayerCollisions_Hurt(void)
{
    RSDK_THIS(Turbine);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Turbine->hitboxHurt)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(768, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, entity);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Turbine_EditorDraw(void)
{
    RSDK_THIS(Turbine);
    RSDK.SetSpriteAnimation(Turbine->aniFrames, entity->type, &entity->animator, true, 0);
    Turbine_Draw();
}

void Turbine_EditorLoad(void) { Turbine->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Turbine.bin", SCOPE_STAGE); }
#endif

void Turbine_Serialize(void) { RSDK_EDITABLE_VAR(Turbine, VAR_UINT8, type); }
