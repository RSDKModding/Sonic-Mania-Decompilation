#include "SonicMania.h"

ObjectMagPlatform *MagPlatform;

void MagPlatform_Update(void) { Platform_Update(); }

void MagPlatform_LateUpdate(void) {}

void MagPlatform_StaticUpdate(void) {}

void MagPlatform_Draw(void)
{
    RSDK_THIS(MagPlatform);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);

    entity->animator.frameID                            = 1;
    RSDK.GetFrame(Platform->spriteIndex, 3, 1)->height = (entity->centerPos.y - entity->drawPos.y) >> 16;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);

    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &entity->centerPos, false);

    entity->animator.frameID = 0;
}

void MagPlatform_Create(void *data)
{
    RSDK_THIS(MagPlatform);
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->spriteIndex, 3, &entity->animator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->length <<= 16;
        entity->stateCollide = MagPlatform_Unknown1;
        entity->state        = MagPlatform_Unknown2;
    }
}

void MagPlatform_StageLoad(void) { MagPlatform->sfxChain = RSDK.GetSFX("Stage/Chain.wav"); }

void MagPlatform_Unknown1(void)
{
    RSDK_THIS(MagPlatform);
    Hitbox *hitbox       = RSDK.GetHitbox(&entity->animator, 1);
    entity->stoodPlayers = 0;
    entity->pushPlayersL = 0;
    entity->pushPlayersR = 0;

    int32 playerID = 0;
    foreach_active(Player, player)
    {
        bool32 groundStore = player->onGround;
        switch (Player_CheckCollisionBox(player, entity, hitbox)) {
            case 1:
                if (!groundStore) {
                    entity->stood = true;
                    if (entity->state == Platform_State_Collapsing && !entity->collapseDelay)
                        entity->collapseDelay = 30;
                    entity->stoodPlayers |= 1 << playerID;
                    player->position.x += entity->collisionOffset.x;
                    player->position.y += entity->collisionOffset.y;
                    player->position.y &= 0xFFFF0000;
                }
                break;
            case 2:
                if (player->onGround && player->right)
                    entity->pushPlayersL |= 1 << playerID;
                break;
            case 3:
                if (player->onGround && player->left)
                    entity->pushPlayersR |= 1 << playerID;
                break;
            case 4:
                if (!player->onGround) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(1024, player, true, &player->uncurlTimer)) {
                        Player_CheckHit(player, entity);
                    }
#else
                    Player_CheckHit(player, entity);
#endif
                }
                else if (player->collisionMode) {
                    if (entity->velocity.y > 0) {
                        Player_CheckHit(player, entity);
                    }
                    else {
#if RETRO_USE_PLUS
                        if (!Player_CheckMightyUnspin(1024, player, true, &player->uncurlTimer)) {
                            Player_CheckHit(player, entity);
                        }
#else
                        Player_CheckHit(player, entity);
#endif
                    }
                }
                else {
                    player->hurtFlag = true;
                }
                break;
            default: break;
        }
        playerID++;
    }
}

void MagPlatform_Unknown2(void) {}

void MagPlatform_Unknown3(void)
{
    RSDK_THIS(MagPlatform);

    entity->drawPos.y += entity->velocity.y;
    int32 posY           = entity->position.y;
    entity->position.y = entity->drawPos.y;
    entity->velocity.y -= 0x3800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x40000, true))
        entity->velocity.y = 0;

    if (entity->drawPos.y <= entity->centerPos.y - entity->length) {
        entity->drawPos.y = entity->centerPos.y - entity->length;
        if (entity->velocity.y < -0x20000)
            RSDK.PlaySfx(MagPlatform->sfxChain, false, 255);
        entity->velocity.y = 0;
    }
    entity->position.y = posY;
    entity->state      = MagPlatform_Unknown4;
}

void MagPlatform_Unknown4(void)
{
    RSDK_THIS(MagPlatform);

    entity->drawPos.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (entity->velocity.y <= 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x40000, true))
        entity->velocity.y = 0;

    if (entity->drawPos.y >= entity->centerPos.y) {
        entity->velocity.y = 0;
        entity->drawPos.y  = entity->centerPos.y;
        entity->active     = ACTIVE_BOUNDS;
        entity->state      = MagPlatform_Unknown2;
    }
}

#if RETRO_INCLUDE_EDITOR
void MagPlatform_EditorDraw(void) {}

void MagPlatform_EditorLoad(void) {}
#endif

void MagPlatform_Serialize(void) { RSDK_EDITABLE_VAR(MagPlatform, VAR_ENUM, length); }
