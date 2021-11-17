#include "SonicMania.h"

ObjectAnimals *Animals;

void Animals_Update(void)
{
    RSDK_THIS(Animals);
    StateMachine_Run(entity->state);
    if (!entity->behaviour && !RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void Animals_LateUpdate(void) {}

void Animals_StaticUpdate(void) {}

void Animals_Draw(void)
{
    RSDK_THIS(Animals);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Animals_Create(void *data)
{
    RSDK_THIS(Animals);
    if (entity->behaviour == ANIMAL_BEHAVE_BOUNCEAROUND_BOUNDS)
        entity->active = ACTIVE_BOUNDS;
    else
        entity->active = ACTIVE_NORMAL;
    entity->drawFX |= FX_FLIP;
    int32 type            = ANIMAL_POCKY;
    entity->visible       = true;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    entity->drawOrder     = Zone->drawOrderLow;
#if RETRO_USE_PLUS
    if (!(globals->secrets & getMod(SECRET_RICKYMODE)))
#endif
        type = voidToInt(data);

    if (!entity->type &&
#if RETRO_USE_PLUS
        RSDK.Random(0, 256, &Zone->randKey) == 21
#else
        RSDK.Rand(0, 256) == 21
#endif
    ) {
        type                  = ANIMAL_POCKY;
        entity->velocity.y    = -0x40000;
        entity->type          = type - 1;
        entity->state         = Animals_State_Freed;
        entity->hitbox.top    = -Animals->hitboxes[entity->type] >> 16;
        entity->hitbox.left   = -4;
        entity->hitbox.right  = 4;
        entity->hitbox.bottom = Animals->hitboxes[entity->type] >> 16;
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * entity->type, &entity->animator, true, 0);
    }
    else if (type) {
        entity->velocity.y    = -0x40000;
        entity->type          = type - 1;
        entity->state         = Animals_State_Freed;
        entity->hitbox.top    = -Animals->hitboxes[entity->type] >> 16;
        entity->hitbox.left   = -4;
        entity->hitbox.right  = 4;
        entity->hitbox.bottom = Animals->hitboxes[entity->type] >> 16;
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * entity->type, &entity->animator, true, 0);
    }
    else if (entity->behaviour == ANIMAL_BEHAVE_FOLLOW) {
        entity->active = ACTIVE_BOUNDS;
        switch (entity->type) {
            case ANIMAL_FLICKY:
            case ANIMAL_CUCKY:
            case ANIMAL_LOCKY: entity->state = Animals_State_FollowPlayer_Bird; break;
            case ANIMAL_RICKY:
            case ANIMAL_POCKY:
            case ANIMAL_PECKY:
            case ANIMAL_PICKY:
            case ANIMAL_ROCKY:
            case ANIMAL_BECKY:
            case ANIMAL_TOCKY:
            case ANIMAL_WOCKY:
            case ANIMAL_MICKY: entity->state = Animals_State_FollowPlayer_Normal; break;
            default: break;
        }
        Animals_CheckPlayerPos();
        entity->velocity.y = Animals->yVelocity[entity->type];
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -Animals->xVelocity[entity->type];
        else
            entity->velocity.x = Animals->xVelocity[entity->type];
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * entity->type + 1, &entity->animator, true, 0);
    }
    else {
        entity->state = Animals_State_BounceAround;
    }
}

void Animals_StageLoad(void)
{
    Animals->aniFrames = RSDK.LoadSpriteAnimation("Global/Animals.bin", SCOPE_STAGE);
    if (RSDK.GetObjectIDByName("Platform"))
        Animals->hasPlatform = true;
    if (RSDK.GetObjectIDByName("Bridge"))
        Animals->hasBridge = true;
}

void Animals_CheckPlayerPos(void)
{
    RSDK_THIS(Animals);

    switch (entity->behaviour) {
        default:
        case ANIMAL_BEHAVE_BOUNCEAROUND: entity->direction = FLIP_X; break;
        case ANIMAL_BEHAVE_FOLLOW: {
            EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            if (!player) {
                entity->direction = FLIP_NONE;
            }
            else if (entity->position.x < player->position.x) {
                entity->direction = FLIP_NONE;
            }
            else {
                entity->direction = FLIP_X; 
            }
            break;
        }
        case ANIMAL_BEHAVE_BOUNCEAROUND_BOUNDS:
#if RETRO_USE_PLUS
            entity->direction = RSDK.Random(0, 2, &Zone->randKey); 
#else
            entity->direction = RSDK.Rand(0, 2);
#endif
            break;
    }

    if (!entity->direction)
        entity->velocity.x = -Animals->xVelocity[entity->type];
    else
        entity->velocity.x = Animals->xVelocity[entity->type];
}

bool32 Animals_CheckPlatformCollision(void *plat)
{
    RSDK_THIS(Animals);
    EntityPlatform *platform = (EntityPlatform *)plat;
    bool32 flag              = false;
    if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
        if (platform->collision <= 1) {
            flag = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animator, 0), entity, &entity->hitbox, true);
        }
        else if (platform->collision == 2 && RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, entity, &entity->hitbox)) {
            if (entity->collisionLayers & Zone->moveID) {
                TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
                move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
            }
            if (entity->velocity.y >= 0x3800)
                flag = true;
        }
        platform->position.x = platform->centerPos.x;
        platform->position.y = platform->centerPos.y;
    }
    return flag;
}

bool32 Animals_CheckGroundCollision(void)
{
    RSDK_THIS(Animals);
    if (entity->velocity.y <= 0)
        return false;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, Animals->hitboxes[entity->type], false))
        return true;
    if (Animals->hasPlatform) {
        foreach_active(Platform, platform)
        {
            if (Animals_CheckPlatformCollision(platform))
                return true;
        }
    }

    if (Animals->hasBridge) {
        foreach_active(Bridge, bridge)
        {
            if (entity->position.x > bridge->startPos && entity->position.x < bridge->endPos && entity->velocity.y >= 0) {
                Hitbox bridgeHitbox;
                bridgeHitbox.left  = -0x400;
                bridgeHitbox.right = 0x400;

                int32 divisor = 0;
                int32 ang     = 0;
                if (entity->position.x - bridge->startPos <= bridge->stoodPos) {
                    divisor = bridge->stoodPos;
                    ang     = (entity->position.x - bridge->startPos) << 7;
                }
                else {
                    divisor = bridge->endPos - bridge->stoodPos - bridge->startPos;
                    ang     = (bridge->endPos - entity->position.x) << 7;
                }

                int32 hitY = (bridge->field_6C * RSDK.Sin512(ang / divisor) >> 9) - 0x80000;
                if (entity->velocity.y < 0x8000) {
                    bridgeHitbox.bottom = (hitY >> 16);
                    bridgeHitbox.top    = (hitY >> 16) - 8;
                }
                else {
                    bridgeHitbox.top    = (hitY >> 16);
                    bridgeHitbox.bottom = (hitY >> 16) + 8;
                }
                if (RSDK.CheckObjectCollisionTouchBox(bridge, &bridgeHitbox, entity, &entity->hitbox)) {
                    entity->position.y = hitY + bridge->position.y - (entity->hitbox.bottom << 16);
                    return true;
                }
            }
        }
    }
    return false;
}

void Animals_State_Freed(void)
{
    RSDK_THIS(Animals);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    RSDK.ProcessAnimation(&entity->animator);
    if (Animals_CheckGroundCollision()) {
        RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * entity->type + 1, &entity->animator, true, 0);
        switch (entity->type) {
            case ANIMAL_FLICKY:
            case ANIMAL_CUCKY:
            case ANIMAL_LOCKY: entity->state = Animals_State_FollowPlayer_Bird; break;
            case ANIMAL_RICKY:
            case ANIMAL_POCKY:
            case ANIMAL_PECKY:
            case ANIMAL_PICKY:
            case ANIMAL_ROCKY:
            case ANIMAL_BECKY:
            case ANIMAL_TOCKY:
            case ANIMAL_WOCKY:
            case ANIMAL_MICKY: entity->state = Animals_State_FollowPlayer_Normal; break;
            default: break;
        }
        Animals_CheckPlayerPos();
        entity->velocity.y = Animals->yVelocity[entity->type];
    }
}

void Animals_State_FollowPlayer_Normal(void)
{
    RSDK_THIS(Animals);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += Animals->gravityStrength[entity->type];
    if (Animals_CheckGroundCollision()) {
        Animals_CheckPlayerPos();
        entity->velocity.y = Animals->yVelocity[entity->type];
    }
    entity->animator.frameID = entity->velocity.y < 0;
}

void Animals_State_FollowPlayer_Bird(void)
{
    RSDK_THIS(Animals);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += Animals->gravityStrength[entity->type];
    if (Animals_CheckGroundCollision()) {
        Animals_CheckPlayerPos();
        entity->velocity.y = Animals->yVelocity[entity->type];
    }
    RSDK.ProcessAnimation(&entity->animator);
}

void Animals_State_BounceAround(void)
{
    RSDK_THIS(Animals);
    if (entity->delay <= 0)
        entity->state = Animals_State_Freed;
    else
        entity->delay--;
}

#if RETRO_INCLUDE_EDITOR
void Animals_EditorDraw(void)
{
    RSDK_THIS(Animals);

    switch (entity->behaviour) {
        default: break;
        case ANIMAL_BEHAVE_BOUNCEAROUND: RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * entity->type, &entity->animator, true, 0); break;
        case ANIMAL_BEHAVE_FOLLOW:
        case ANIMAL_BEHAVE_BOUNCEAROUND_BOUNDS: RSDK.SetSpriteAnimation(Animals->aniFrames, 2 * entity->type + 1, &entity->animator, true, 0); break;
    }

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Animals_EditorLoad(void)
{
    Animals->aniFrames = RSDK.LoadSpriteAnimation("Global/Animals.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Animals, type);
    RSDK_ENUM_VAR("Flicky", ANIMAL_FLICKY);
    RSDK_ENUM_VAR("Ricky", ANIMAL_RICKY);
    RSDK_ENUM_VAR("Pocky", ANIMAL_POCKY);
    RSDK_ENUM_VAR("Pecky", ANIMAL_PECKY);
    RSDK_ENUM_VAR("Picky", ANIMAL_PICKY);
    RSDK_ENUM_VAR("Cucky", ANIMAL_CUCKY);
    RSDK_ENUM_VAR("Rocky", ANIMAL_ROCKY);
    RSDK_ENUM_VAR("Becky", ANIMAL_BECKY);
    RSDK_ENUM_VAR("Locky", ANIMAL_LOCKY);
    RSDK_ENUM_VAR("Tocky", ANIMAL_TOCKY);
    RSDK_ENUM_VAR("Wocky", ANIMAL_WOCKY);
    RSDK_ENUM_VAR("Mickey", ANIMAL_MICKY);

    RSDK_ACTIVE_VAR(Animals, behaviour);
    RSDK_ENUM_VAR("Bounce Around (No Bounds)", ANIMAL_BEHAVE_BOUNCEAROUND);
    RSDK_ENUM_VAR("Follow Player", ANIMAL_BEHAVE_FOLLOW);
    RSDK_ENUM_VAR("Bounce Around (Bounds)", ANIMAL_BEHAVE_BOUNCEAROUND_BOUNDS);
}
#endif

void Animals_Serialize(void)
{
    RSDK_EDITABLE_VAR(Animals, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Animals, VAR_ENUM, behaviour);
}
