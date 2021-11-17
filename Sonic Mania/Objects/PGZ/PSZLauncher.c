#include "SonicMania.h"

ObjectPSZLauncher *PSZLauncher;

void PSZLauncher_Update(void)
{
    RSDK_THIS(PSZLauncher);
    StateMachine_Run(entity->state);
}

void PSZLauncher_LateUpdate(void) {}

void PSZLauncher_StaticUpdate(void) {}

void PSZLauncher_Draw(void)
{
    RSDK_THIS(PSZLauncher);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PSZLauncher_Create(void *data)
{
    RSDK_THIS(PSZLauncher);
    entity->visible   = 1;
    entity->drawOrder = Zone->drawOrderHigh - 1;
    entity->drawFX    = FX_FLIP;
    if (RSDK_sceneInfo->inEditor && !entity->power)
        entity->power = 10;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = PSZLauncher_State_Setup;
}

void PSZLauncher_StageLoad(void)
{
    PSZLauncher->aniFrames     = RSDK.LoadSpriteAnimation("PSZ1/PSZLauncher.bin", SCOPE_STAGE);
    PSZLauncher->hitbox.left   = -32;
    PSZLauncher->hitbox.top    = 0;
    PSZLauncher->hitbox.right  = 32;
    PSZLauncher->hitbox.bottom = 1;
    DEBUGMODE_ADD_OBJ(PSZLauncher);
}

void PSZLauncher_DebugSpawn(void)
{
    RSDK_THIS(PSZLauncher);
    EntityPSZLauncher *launcher = CREATE_ENTITY(PSZLauncher, NULL, entity->position.x, entity->position.y);
    launcher->direction         = entity->direction;
}

void PSZLauncher_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(PSZLauncher->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void PSZLauncher_State_Setup(void)
{
    RSDK_THIS(PSZLauncher);
    RSDK.SetSpriteAnimation(PSZLauncher->aniFrames, 0, &entity->animator, true, 0);
    entity->activePlayers  = 0;
    entity->activePlayers2 = 0;
    entity->state          = PSZLauncher_Unknown6;
    PSZLauncher_Unknown3();
    PSZLauncher_Unknown4();
}

void PSZLauncher_Unknown3(void)
{
    RSDK_THIS(PSZLauncher);

    Hitbox hitbox;
    hitbox.left   = -32;
    hitbox.right  = 0;
    hitbox.bottom = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        int32 pos = 31 - clampVal(abs(player->position.x - entity->position.x) >> 16, 0, 31);
        if ((entity->direction == FLIP_NONE && player->position.x > entity->position.x)
            || (entity->direction == FLIP_X && player->position.x < entity->position.x))
            pos = 31;

        hitbox.top = -PSZLauncher->heights[pos];
        if ((1 << playerID) & entity->activePlayers)
            player->position.y += 0x10000;

        if (Player_CheckCollisionPlatform(player, entity, &hitbox) == 1) {
            entity->activePlayers |= (1 << playerID);
            player->position.y &= 0xFFFF0000;
        }
        else {
            entity->activePlayers &= ~(1 << playerID);
        }
    }
}

void PSZLauncher_Unknown4(void)
{
    RSDK_THIS(PSZLauncher);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (Player_CheckCollisionTouch(player, entity, &PSZLauncher->hitbox)) {
            if (!((1 << playerID) & entity->activePlayers2) && !((1 << playerID) & entity->activePlayers) && player->velocity.y <= 0) {
                entity->activePlayers2 |= (1 << playerID);
                player->velocity.y = -0x10000 * entity->power;
                player->velocity.x = 0;
                if (entity->direction)
                    player->position.x = entity->position.x - 0x100000;
                else
                    player->position.x = entity->position.x + 0x100000;
                player->jumpAbility      = 0;
                player->jumpAbilityTimer = 0;
                player->collisionMode    = 0;
                player->groundVel        = 0;
                player->state            = Player_State_Air;
                player->onGround         = false;
                RSDK.PlaySfx(Player->sfxRelease, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
            }
        }
        else {
            entity->activePlayers2 &= ~(1 << playerID);
        }
    }
}

void PSZLauncher_Unknown6(void)
{
    PSZLauncher_Unknown3();
    PSZLauncher_Unknown4();
}

#if RETRO_INCLUDE_EDITOR
void PSZLauncher_EditorDraw(void) {}

void PSZLauncher_EditorLoad(void) {}
#endif

void PSZLauncher_Serialize(void)
{
    RSDK_EDITABLE_VAR(PSZLauncher, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PSZLauncher, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PSZLauncher, VAR_UINT8, power);
}
