#include "SonicMania.h"

ObjectPlaneSeeSaw *PlaneSeeSaw;

void PlaneSeeSaw_Update(void)
{
    RSDK_THIS(PlaneSeeSaw);
    StateMachine_Run(entity->state);
}

void PlaneSeeSaw_LateUpdate(void) {}

void PlaneSeeSaw_StaticUpdate(void) {}

void PlaneSeeSaw_Draw(void)
{
    RSDK_THIS(PlaneSeeSaw);
    Vector2 drawPos;

    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y + entity->scale.y * (entity->seeSawPos >> 9);
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void PlaneSeeSaw_Create(void *data)
{
    RSDK_THIS(PlaneSeeSaw);
    RSDK_sceneInfo->entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        entity->state         = PlaneSeeSaw_State_WaitForPlayer;
        RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 2, &entity->animator3, true, 0);
    }
}

void PlaneSeeSaw_StageLoad(void)
{
    PlaneSeeSaw->aniFrames  = RSDK.LoadSpriteAnimation("MMZ/SeeSaw.bin", SCOPE_STAGE);
    PlaneSeeSaw->sfxSpring  = RSDK.GetSFX("Global/Spring.wav");
    PlaneSeeSaw->sfxFlipper = RSDK.GetSFX("Stage/Flipper.wav");
}

void PlaneSeeSaw_State_WaitForPlayer(void)
{
    RSDK_THIS(PlaneSeeSaw);

    Hitbox *hitbox = RSDK.GetHitbox(&entity->animator1, 0);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, hitbox)) {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif
            entity->state = PlaneSeeSaw_State_PlayerPushDown;
        }
    }
}

void PlaneSeeSaw_State_PlayerPushDown(void)
{
    RSDK_THIS(PlaneSeeSaw);

    Hitbox *hitboxOld = RSDK.GetHitbox(&entity->animator1, 0);
    if (entity->animator1.frameID >= entity->animator1.frameCount - 1) {
        entity->velocity.y += 0x3800;
        entity->seeSawPos += entity->velocity.y;
    }
    else {
        ++entity->animator2.frameID;
        ++entity->animator1.frameID;
        if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
            entity->velocity.y = -0x60000;
            RSDK.PlaySfx(PlaneSeeSaw->sfxFlipper, false, 255);
        }
        entity->seeSawPos = RSDK.GetHitbox(&entity->animator2, 0)->top << 16;
    }

    Hitbox *hitboxNew = RSDK.GetHitbox(&entity->animator1, 0);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, hitboxOld))
            player->position.y += (hitboxNew->bottom - hitboxOld->bottom) << 16;
    }

    if (entity->seeSawPos > -0x230000 && entity->velocity.y > 0) {
        entity->velocity.y = 0;
        entity->seeSawPos  = -0x230000;
        entity->state      = PlaneSeeSaw_State_Launch;
        RSDK.PlaySfx(PlaneSeeSaw->sfxSpring, false, 255);
    }
}

void PlaneSeeSaw_State_Launch(void)
{
    RSDK_THIS(PlaneSeeSaw);
    Hitbox *hitboxOld = RSDK.GetHitbox(&entity->animator1, 0);
    if (entity->animator1.frameID <= 0) {
        entity->state = PlaneSeeSaw_State_WaitForPlayer;
    }
    else {
        --entity->animator2.frameID;
        --entity->animator1.frameID;
        entity->seeSawPos = RSDK.GetHitbox(&entity->animator2, 0)->top << 16;
    }

    Hitbox *hitboxNew = RSDK.GetHitbox(&entity->animator1, 0);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, hitboxOld)) {
            player->position.y += (hitboxNew->bottom - hitboxOld->bottom) << 16;
            if (entity->animator1.frameID < 4) {
                if (entity->scale.x == 0x100) {
                    player->position.x -= FarPlane->field_18.x;
                    player->position.y -= FarPlane->field_18.y;
                    player->position.x += FarPlane->field_20.x;
                    player->position.y += FarPlane->field_20.y;
                    if (player->camera) {
                        player->camera->field_6C.x = 0;
                        player->camera->field_6C.y = 0;
                    }
                    player->groundVel  = 0;
                    player->velocity.x = 0;
                    player->drawOrder  = Zone->playerDrawLow;
                    player->state      = PlaneSeeSaw_PlayerState_ToFG;
                    player->velocity.y = -0x60000;
                }
                else {
                    player->groundVel  = 0;
                    player->velocity.x = 0;
                    player->scale.x    = 0x200;
                    player->scale.y    = 0x200;
                    player->state      = PlaneSeeSaw_PlayerState_ToBG;
                    player->velocity.y = -0xA0000;
                }
                player->abilityValues[0]    = entity->position.x;
                player->nextAirState     = StateMachine_None;
                player->nextGroundState  = StateMachine_None;
                player->interaction      = false;
                player->tileCollisions   = false;
                player->jumpAbilityTimer = 0;
                player->drawFX |= FX_SCALE;
                player->jumpAbility  = 0;
                player->onGround     = false;
                EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                if (shield && shield->objectID) {
                    shield->drawFX |= FX_SCALE;
                }
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                player->playerAnimator.animationSpeed = 0x80;
            }
        }
    }
}

void PlaneSeeSaw_PlayerState_ToBG(void)
{
    EntityPlayer *entity = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot, Player);
    entity->left         = false;
    entity->right        = false;
    if (((entity->abilityValues[0] ^ entity->position.x) & 0xFFFF0000)) {
        if (entity->position.x <= entity->abilityValues[0])
            entity->position.x += 0x8000;
        else
            entity->position.x -= 0x8000;
    }

    RSDK_controller[entity->controllerID].keyY.press = false;
    Player_State_Air();

    entity->tileCollisions = false;
    if (entity->scale.x <= 0x100) {
        entity->interaction    = true;
        entity->tileCollisions = true;
        entity->drawOrder      = 2;
        entity->position.x -= FarPlane->field_20.x;
        entity->position.y -= FarPlane->field_20.y;
        entity->position.x += FarPlane->field_18.x;
        entity->position.y += FarPlane->field_18.y;
        Zone->deathBoundary[0] += 0x8000000;
        entity->state = Player_State_Air;
        if (entity->camera) {
            entity->camera->field_6C.x = FarPlane->field_20.x - FarPlane->field_18.x;
            entity->camera->field_6C.y = FarPlane->field_20.y - FarPlane->field_18.y;
        }
        entity->scale.y = entity->scale.x;
    }
    else {
        entity->scale.x -= 4;
        entity->scale.y = entity->scale.x;
    }
}

void PlaneSeeSaw_PlayerState_ToFG(void)
{
    EntityPlayer *entity = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot, Player);

    entity->left  = false;
    entity->right = false;
    if (((entity->abilityValues[0] ^ entity->position.x) & 0xFFFF0000)) {
        if (entity->position.x <= entity->abilityValues[0])
            entity->position.x += 0x8000;
        else
            entity->position.x -= 0x8000;
    }
    RSDK_controller[entity->controllerID].keyY.press = false;
    Player_State_Air();

    entity->tileCollisions = false;
    if (entity->scale.x >= 0x200) {
        entity->interaction    = true;
        entity->tileCollisions = true;
        Zone->deathBoundary[0] -= 0x8000000;
        entity->drawFX &= ~FX_SCALE;
        entity->state = Player_State_Air;

        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(entity), Shield);
        if (shield && shield->objectID) {
            shield->drawFX &= ~FX_SCALE;
        }
        entity->scale.y = entity->scale.x;
    }
    else {
        entity->scale.x += 4;
        entity->scale.y = entity->scale.x;
    }
}

#if RETRO_INCLUDE_EDITOR
void PlaneSeeSaw_EditorDraw(void)
{
    RSDK_THIS(PlaneSeeSaw);
    RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 0, &entity->animator2, false, 0);
    RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 1, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(PlaneSeeSaw->aniFrames, 2, &entity->animator3, false, 0);

    PlaneSeeSaw_Draw();
}

void PlaneSeeSaw_EditorLoad(void) { PlaneSeeSaw->aniFrames = RSDK.LoadSpriteAnimation("MMZ/SeeSaw.bin", SCOPE_STAGE); }
#endif

void PlaneSeeSaw_Serialize(void) {}
