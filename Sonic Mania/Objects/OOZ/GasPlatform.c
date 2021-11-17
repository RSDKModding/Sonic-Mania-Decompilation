#include "SonicMania.h"

ObjectGasPlatform *GasPlatform;

void GasPlatform_Update(void)
{
    RSDK_THIS(GasPlatform);
    if (entity->state == Platform_State_Normal) {
        switch (entity->type) {
            case 0:
                if (!((Zone->timer + entity->intervalOffset) % entity->interval))
                    GasPlatform_Unknown1();
                break;
            case 1: {
                int32 id = 0;
                foreach_active(Player, player)
                {
                    if ((1 << id) & entity->stoodPlayers && !player->sidekick) {
                        if (abs(player->position.x - entity->position.x) < 0x40000) {
                            player->position.x = entity->position.x;
                            player->state      = Player_State_None;
                            player->velocity.x = 0;
                            player->groundVel  = 0;
                            GasPlatform_Unknown1();
                        }
                    }
                    ++id;
                }
                break;
            }
        }
    }
    Platform_Update();
}

void GasPlatform_LateUpdate(void) {}

void GasPlatform_StaticUpdate(void) {}

void GasPlatform_Draw(void)
{
    RSDK_THIS(GasPlatform);
    if (entity->centerPos.y - entity->drawPos.y > 0x180000)
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void GasPlatform_Create(void *data)
{
    RSDK_THIS(GasPlatform);
    if (!entity->interval)
        entity->interval = 1;
    entity->frameID   = 2;
    entity->collision = PLATFORM_C_0;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &entity->animator2, true, 0);
    entity->stateCollide = Platform_CollisionState_AllSolid;
    entity->state        = Platform_State_Normal;
}

void GasPlatform_StageLoad(void)
{
    GasPlatform->hitbox.top    = -16;
    GasPlatform->hitbox.left   = -16;
    GasPlatform->hitbox.right  = 16;
    GasPlatform->hitbox.bottom = 0;
    GasPlatform->range.x       = 0x800000;
    GasPlatform->range.y       = 0x180000;
    GasPlatform->sfxGasPop     = RSDK.GetSFX("OOZ/GasPop.wav");
    GasPlatform->sfxSpring     = RSDK.GetSFX("Global/Spring.wav");
}

void GasPlatform_Unknown1(void)
{
    RSDK_THIS(GasPlatform);

    if (RSDK.CheckOnScreen(SceneInfo->entity, &GasPlatform->range))
        RSDK.PlaySfx(GasPlatform->sfxGasPop, false, 255);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.y = -0x96800;
    entity->state      = GasPlatform_Unknown2;
}

void GasPlatform_Unknown2(void)
{
    RSDK_THIS(GasPlatform);

    entity->drawPos.y += entity->velocity.y;
    if (entity->drawPos.y >= entity->centerPos.y) {
        entity->drawPos.y = entity->centerPos.y;
        if (entity->velocity.y <= 0x10000) {
            entity->active = ACTIVE_BOUNDS;
            entity->state  = Platform_State_Normal;
            return;
        }
        entity->velocity.y = -(entity->velocity.y >> 2);
    }
    else {
        entity->velocity.y += 0x3800;
    }

    int32 dist = entity->centerPos.y - entity->drawPos.y;
    if (dist > 0x180000) {
        RSDK.ProcessAnimation(&entity->animator2);
        int32 storeX         = entity->position.x;
        int32 storeY         = entity->position.y;
        entity->position.x = entity->centerPos.x;
        entity->position.y = entity->centerPos.y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &GasPlatform->hitbox)) {
                Player_CheckElementalHit(player, entity, SHIELD_FIRE);
            }
        }

        entity->position.x = storeX;
        entity->position.y = storeY;
    }

    if (entity->type == 1) {
        int32 id = 0;
        foreach_active(Player, player)
        {
            int32 posY = entity->centerPos.y - 0x780000;
            if (entity->position.y <= posY) {
                entity->position.y = posY;
                if ((1 << id) & entity->stoodPlayers) {
                    player->velocity.y = -0x100000;
                    player->state      = Player_State_Air;
                    player->onGround   = false;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                    RSDK.PlaySfx(GasPlatform->sfxSpring, false, 255);
                    entity->collapseDelay = 240;
                    entity->state         = GasPlatform_Unknown3;
                }
            }
            else if ((1 << id) & entity->stoodPlayers) {
                if (abs(player->position.x - entity->position.x) < 0x40000) {
                    player->position.x = entity->position.x;
                    player->state      = Player_State_None;
                    player->velocity.x = 0;
                    player->groundVel  = 0;
                }
            }
            ++id;
        }
    }
}

void GasPlatform_Unknown3(void)
{
    RSDK_THIS(GasPlatform);
    RSDK.ProcessAnimation(&entity->animator2);
    int32 storeX         = entity->position.x;
    int32 storeY         = entity->position.y;
    entity->position.x = entity->centerPos.x;
    entity->position.y = entity->centerPos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &GasPlatform->hitbox)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
    if (!--entity->collapseDelay) {
        entity->velocity.y = 0;
        entity->state      = GasPlatform_Unknown2;
    }
}

void GasPlatform_Unknown4(void)
{
    RSDK_THIS(GasPlatform);
    entity->drawPos.x = (RSDK.Rand(-1, 1) << 16) + entity->centerPos.x;
    entity->drawPos.y = (RSDK.Rand(-2, 2) << 16) + entity->centerPos.y;
    if (entity->collapseDelay <= 0) {
        RSDK.PlaySfx(GasPlatform->sfxGasPop, false, 255);
        entity->active     = ACTIVE_NORMAL;
        entity->velocity.y = -0x8C000;
        entity->state      = GasPlatform_Unknown2;
    }
    else {
        entity->collapseDelay--;
    }
}

#if RETRO_INCLUDE_EDITOR
void GasPlatform_EditorDraw(void)
{
    RSDK_THIS(GasPlatform);
    entity->drawPos = entity->position;

    GasPlatform_Draw();
}

void GasPlatform_EditorLoad(void) {}
#endif

void GasPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(GasPlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(GasPlatform, VAR_UINT16, intervalOffset);
}
