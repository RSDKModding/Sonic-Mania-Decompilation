#include "SonicMania.h"

ObjectSideBarrel *SideBarrel;

void SideBarrel_Update(void)
{
    RSDK_THIS(SideBarrel);
    if (entity->activePlayers <= 0)
        entity->animator.animationSpeed = 0;
    else
        entity->animator.animationSpeed = 128;
    RSDK.ProcessAnimation(&entity->animator);

    int playerID = 0;
    foreach_active(Player, player)
    {
        if (globals->gameMode >= MODE_TIMEATTACK && entity->timeAttackFreeze) {
            entity->hitbox.bottom = -8;
            entity->hitbox.top    = -24;
            Player_CheckCollisionPlatform(player, entity, &entity->hitbox);
        }
        else {
            if (player->state == Player_State_KnuxGlideDrop) {
                entity->field_74[playerID] = 0;
                entity->field_64[playerID] = 128;
            }

            if (entity->field_74[playerID]) {
                entity->field_74[playerID]--;
                if (entity->field_74[playerID] <= 0) {
                    entity->activePlayers &= ~(1 << playerID);
                }
            }
            else {
                if ((1 << playerID) & entity->activePlayers)
                    entity->field_64[playerID] += 2;
                else
                    entity->field_64[playerID] = 128;

                entity->hitbox.top    = (RSDK.Cos256(entity->field_64[playerID]) >> 3) - 4;
                entity->hitbox.bottom = entity->hitbox.top + 16;
                if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox)) {
                    player->position.y += 0x40000;
                    if (player->state != Player_State_KnuxGlideDrop) {
                        if (entity->field_64[playerID] > 176) {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->playerAnimator, false, 1);
                            player->onGround           = false;
                            player->state              = Player_State_Air;
                            entity->field_74[playerID] = 48;
                            RSDK.PlaySfx(SideBarrel->sfxDrop, false, 255);
                        }
                        entity->activePlayers |= (1 << playerID);
                    }
                }
                else {
                    entity->field_64[playerID] = 128;
                }
            }
        }
        playerID++;
    }
}

void SideBarrel_LateUpdate(void) {}

void SideBarrel_StaticUpdate(void) {}

void SideBarrel_Draw(void)
{
    RSDK_THIS(SideBarrel);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void SideBarrel_Create(void *data)
{
    RSDK_THIS(SideBarrel);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SideBarrel->aniFrames, 0, &entity->animator, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->hitbox.left   = -32;
        entity->hitbox.top    = -32;
        entity->hitbox.right  = 32;
        entity->hitbox.bottom = 32;
    }
}

void SideBarrel_StageLoad(void)
{
    SideBarrel->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SideBarrel.bin", SCOPE_STAGE);
    SideBarrel->sfxDrop   = RSDK.GetSFX("Stage/Drop.wav");
}

#if RETRO_INCLUDE_EDITOR
void SideBarrel_EditorDraw(void) { SideBarrel_Draw(); }

void SideBarrel_EditorLoad(void) { SideBarrel->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SideBarrel.bin", SCOPE_STAGE); }
#endif

void SideBarrel_Serialize(void) { RSDK_EDITABLE_VAR(SideBarrel, VAR_BOOL, timeAttackFreeze); }
