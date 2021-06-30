#include "../SonicMania.h"

ObjectCrate *Crate;

void Crate_Update(void)
{
    RSDK_THIS(Crate);
    if (entity->collision != 1 || !Crate_Unknown3()) {
        if (entity->animator.animationID) {
            StateMachine_Run(entity->state);
        }
        else {
            Platform_Update();
        }
    }
}

void Crate_LateUpdate(void) {}

void Crate_StaticUpdate(void) {}

void Crate_Draw(void)
{
    RSDK_THIS(Crate);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, 0);
}

void Crate_Create(void *data)
{
    RSDK_THIS(Crate);

    int frameID = entity->frameID;
    if (frameID == 3)
        entity->collision = 4;
    else
        entity->collision = 1;
    Platform_Create(NULL);
    entity->frameID = frameID;

    if (!RSDK_sceneInfo->inEditor) {
        entity->hitbox.left   = -0x18;
        entity->hitbox.top    = -0x17;
        entity->hitbox.right  = 0x18;
        entity->hitbox.bottom = 0x18;
        entity->active        = ACTIVE_XBOUNDS;
        entity->drawFX        = FX_SCALE | FX_FLIP;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        if (frameID == 3)
            entity->drawOrder = Zone->drawOrderLow;
        else
            entity->drawOrder = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Crate->aniFrames, 0, &entity->animator, true, frameID);
        entity->state = Crate_Unknown4;
    }
}

void Crate_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Crate->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Crate.bin", SCOPE_STAGE);
    Crate->sfxExplosion2 = RSDK.GetSFX("Stage/Explosion2.wav");
}

void Crate_Unknown1(void *e)
{
    EntityPlatform *entity = (EntityPlatform *)e;
    for (int i = 0; i < 64; ++i) {
        EntityIce *ice  = (EntityIce *)RSDK.CreateEntity(Ice->objectID, intToVoid(3), (RSDK.Rand(-24, 25) << 16) + entity->position.x,
                                                        (RSDK.Rand(-24, 25) << 16) + entity->position.y);
        ice->velocity.x = RSDK.Rand(-6, 8) << 15;
        ice->velocity.y = RSDK.Rand(-10, 2) << 15;
        ice->direction  = RSDK.Rand(0, 4);
        // LOWORD(ice[1].alpha) = RSDK.Rand(1, 4);
        ice->drawOrder = Zone->drawOrderLow + 1;
        switch (entity->animator.frameID) {
            case 0:
            case 3: /*RSDK.SetSpriteAnimation(Crate->aniFrames, 1, &ice[1].updateRange, true, 0);*/ break;
            case 1:
            case 2:
                // if (RSDK.Rand(0, 6) >= 2)
                //    RSDK.SetSpriteAnimation(Crate->aniFrames, 2, &ice[1].updateRange, true, 0);
                // else
                //    RSDK.SetSpriteAnimation(Crate->aniFrames, 3, &ice[1].updateRange, true, 0);
                break;
            default: break;
        }
    }
    RSDK.PlaySFX(Crate->sfxExplosion2, 0, 255);
    entity->position.y -= 0x10000;

    foreach_active(Crate, crate)
    {
        if ((void *)crate != (void *)entity && crate->state == Crate_Unknown4
            && RSDK.CheckObjectCollisionTouchBox(entity, &entity->hitbox, crate, &crate->hitbox) == 1) {
            crate->state = Crate_Unknown5;
        }
    }
    RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
}
void Crate_Unknown2(void *e, int offset)
{
    EntityPlatform *entity = (EntityPlatform *)e;
    entity->drawPos.y += offset;
    entity->centerPos.x = entity->drawPos.x;
    entity->centerPos.y = entity->drawPos.y;
    entity->position.x  = entity->drawPos.x;
    entity->position.y  = entity->drawPos.y;
    int start           = (entity->drawPos.y - 0x300000) & 0xFFFF0000;
    foreach_active(Crate, crate)
    {
        if ((void *)crate != (void *)entity && RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, crate, &crate->hitbox, true) == 1)
            Crate_Unknown2(crate, start - crate->drawPos.y);
    }
}
bool32 Crate_Unknown3(void)
{
    RSDK_THIS(Crate);

    foreach_active(Player, player)
    {
        int storeX           = player->position.x;
        int storeY           = player->position.y;
        int storeXVel        = player->velocity.x;
        int storeYVel        = player->velocity.y;
        int storeVel         = player->groundVel;
        int storeGrounded    = player->onGround;
        Hitbox *playerHitbox = Player_GetHitbox(player);
        switch (RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, player, playerHitbox, false)) {
            default:
            case 0:
                player->velocity.x = storeXVel;
                player->velocity.y = storeYVel;
                player->position.x = storeX;
                player->position.y = storeY;
                Player_CheckCollisionBox(player, entity, &entity->hitbox);
                break;
            case 1:
                if (entity->frameID != 1) {
                    player->velocity.x = storeXVel;
                    player->velocity.y = storeYVel;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    Player_CheckCollisionBox(player, entity, &entity->hitbox);
                    break;
                }
                else if (player->state == Player_State_MightyHammerDrop) {
                    player->velocity.y -= 0x10000;
                    Crate_Unknown1(entity);
                    player->velocity.x = storeXVel;
                    player->velocity.y = storeYVel;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    player->groundVel  = storeVel;
                    player->onGround   = storeGrounded;
                    foreach_return true;
                }
                else if (player->shield == SHIELD_BUBBLE && player->invincibleTimer <= 0) {
                    if (RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield)->data.animationID == 8
                        && player->velocity.y >= 0x80000) {
                        Crate_Unknown1(entity);
                        player->velocity.x = storeXVel;
                        player->velocity.y = storeYVel;
                        player->position.x = storeX;
                        player->position.y = storeY;
                        player->groundVel  = storeVel;
                        player->onGround   = storeGrounded;
                        foreach_return true;
                    }
                }
                player->velocity.x = storeXVel;
                player->velocity.y = storeYVel;
                player->position.x = storeX;
                player->position.y = storeY;
                Player_CheckCollisionBox(player, entity, &entity->hitbox);
                break;
            case 2:
            case 3:
                if (entity->frameID != 1 || player->shield != SHIELD_FIRE || player->invincibleTimer > 0
                    || RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield)->data.animationID != 2) {
                    player->velocity.x = storeXVel;
                    player->velocity.y = storeYVel;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    Player_CheckCollisionBox(player, entity, &entity->hitbox);
                }
                else if (player->position.x < entity->position.x) {
                    if (player->velocity.x >= 0x78000) {
                        Crate_Unknown1(entity);
                        player->velocity.x = storeXVel;
                        player->velocity.y = storeYVel;
                        player->position.x = storeX;
                        player->position.y = storeY;
                        player->groundVel  = storeVel;
                        player->onGround   = storeGrounded;
                        foreach_return true;
                    }
                    else {
                        player->velocity.x = storeXVel;
                        player->velocity.y = storeYVel;
                        player->position.x = storeX;
                        player->position.y = storeY;
                        Player_CheckCollisionBox(player, entity, &entity->hitbox);
                    }
                }
                else {
                    if (player->velocity.x <= -0x78000) {
                        Crate_Unknown1(entity);
                        player->velocity.x = storeXVel;
                        player->velocity.y = storeYVel;
                        player->position.x = storeX;
                        player->position.y = storeY;
                        player->groundVel  = storeVel;
                        player->onGround   = storeGrounded;
                        foreach_return true;
                    }
                    else {
                        player->velocity.x = storeXVel;
                        player->velocity.y = storeYVel;
                        player->position.x = storeX;
                        player->position.y = storeY;
                        Player_CheckCollisionBox(player, entity, &entity->hitbox);
                    }
                }
                break;
            case 4:
                if (entity->collisionOffset.y >= 0)
                    player->collisionFlagV |= 2;
                player->velocity.x = storeXVel;
                player->velocity.y = storeYVel;
                player->position.x = storeX;
                player->position.y = storeY;
                Player_CheckCollisionBox(player, entity, &entity->hitbox);
                break;
        }
    }
    return false;
}
void Crate_Unknown4(void) {}
void Crate_Unknown5(void) {}
void Crate_Unknown6(void)
{
    RSDK_THIS(Crate);
    if (--entity->collapseDelay <= 0)
        entity->state = Crate_Unknown7;
}
void Crate_Unknown7(void)
{
    RSDK_THIS(Crate);
    entity->drawPos.y += entity->velocity.y;
    int x = entity->position.x;
    int y = entity->position.y;
    entity->velocity.y += 0x3800;
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, CMODE_FLOOR, 0, 0x180000, true)) {
        entity->velocity.y = 0;
        entity->state      = Crate_Unknown4;
    }
    else {
        foreach_active(Crate, crate)
        {
            if (crate != entity && !crate->velocity.y && RSDK.CheckObjectCollisionBox(crate, &crate->hitbox, entity, &entity->hitbox, true) == 1) {
                entity->velocity.y = 0;
                entity->state      = Crate_Unknown4;
            }
        }
    }
    entity->drawPos.x   = entity->position.x;
    entity->drawPos.y   = entity->position.y;
    entity->centerPos.x = entity->position.x;
    entity->centerPos.y = entity->position.y;
    entity->position.x  = x;
    entity->position.y  = y;
}

void Crate_EditorDraw(void) {}

void Crate_EditorLoad(void) {}

void Crate_Serialize(void)
{
    RSDK_EDITABLE_VAR(Crate, VAR_UINT8, frameID);
    RSDK_EDITABLE_VAR(Crate, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(Crate, VAR_BOOL, ignoreItemBox);
}
