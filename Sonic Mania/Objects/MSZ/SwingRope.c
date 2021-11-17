#include "SonicMania.h"

ObjectSwingRope *SwingRope;

void SwingRope_Update(void)
{
    RSDK_THIS(SwingRope);

    int32 sine              = 11 * RSDK.Sin512(entity->angleOffset + 3 * Zone->timer);
    entity->rotatedAngle  = sine;
    entity->rotatedOffset = sine >> 3;
    entity->rotatePos.x   = entity->position.x;
    entity->rotatePos.y   = entity->position.y;

    for (int32 s = 0; s < entity->ropeSize; ++s) {
        entity->angle = (sine >> 6) & 0x1FF;
        entity->rotatePos.x -= RSDK.Sin512(entity->angle) << 11;
        entity->rotatePos.y += RSDK.Cos512(entity->angle) << 11;
        sine += entity->rotatedOffset;
    }

    int32 rotateX   = entity->rotatePos.x;
    int32 rotateY   = entity->rotatePos.y;
    entity->angle = (sine >> 6) & 0x1FF;
    rotateY += 0x700 * RSDK.Cos512(entity->angle);
    entity->rotatePos.x -= RSDK.Sin512(entity->angle) << 11;
    entity->rotatePos.y += RSDK.Cos512(entity->angle) << 11;

    int32 storeX         = entity->position.x;
    int32 storeY         = entity->position.y;
    entity->position.x = entity->rotatePos.x;
    entity->position.y = entity->rotatePos.y;
    entity->velocity.x = entity->rotatePos.x - entity->ropePos.x;
    entity->velocity.y = entity->rotatePos.y - entity->ropePos.y;
    if (entity->ropeGrabDelay > 0)
        entity->ropeGrabDelay--;

    foreach_active(Player, player)
    {
        if (player->state == Player_State_None) {
            if (Player_CheckCollisionTouch(player, entity, &SwingRope->hitbox2)) {
                if (player->jumpPress) {
                    player->jumpAbilityTimer = 1;
                    player->state            = Player_State_Air;
                    player->drawOrder        = entity->playerLayers[player->playerID];
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->velocity.x = entity->velocity.x >> 1;
                    if (player->left) {
                        player->velocity.x = -0x20000;
                    }
                    else if (player->right) {
                        player->velocity.x = 0x20000;
                    }
                    player->velocity.y    = (entity->velocity.y >> 1) - 0x38000;
                    player->onGround      = false;
                    player->jumpAbility   = 0;
                    entity->ropeGrabDelay = 30;
                }
                else {
                    player->position.x = rotateX + (-0x800 * RSDK.Sin512(entity->angle));
                    player->position.y = rotateY - ((Player_GetHitbox(player)->top + 2) << 16);
                }
            }
        }
        else if (!entity->ropeGrabDelay && !player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            Hitbox otherHitbox;
            otherHitbox.left   = playerHitbox->left;
            otherHitbox.right  = playerHitbox->right;
            otherHitbox.top    = 0;
            otherHitbox.bottom = 0;
            if (RSDK.CheckObjectCollisionTouchBox(entity, &SwingRope->hitbox1, player, &otherHitbox)) {
                entity->playerLayers[player->playerID] = player->drawOrder;
                player->drawOrder                      = Zone->drawOrderLow;
                player->state                          = Player_State_None;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->playerAnimator, 0, 0);
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->position.x = -0x800 * RSDK.Sin512(entity->angle) + rotateX;
                player->position.y = rotateY - ((playerHitbox->top + 2) << 16);
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
            }
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void SwingRope_LateUpdate(void) {}

void SwingRope_StaticUpdate(void) {}

void SwingRope_Draw(void)
{
    RSDK_THIS(SwingRope);

    entity->ropeData.frameID = (entity->rotatedAngle >> 10) & 0x1F;
    entity->rotation         = (entity->rotatedAngle >> 6) & 0xF;
    entity->drawFX           = FX_ROTATE;
    RSDK.DrawSprite(&entity->ropeData, NULL, false);

    entity->ropePos.x = entity->position.x;
    entity->ropePos.y = entity->position.y;

    int32 angle    = entity->rotatedAngle >> 6;
    int32 rotAngle = entity->rotatedAngle;
    for (int32 s = 0; s < entity->ropeSize; ++s) {
        entity->angle = angle & 0x1FF;
        entity->ropePos.x -= RSDK.Sin512(entity->angle) << 11;
        entity->ropePos.y += RSDK.Cos512(entity->angle) << 11;
        rotAngle += entity->rotatedOffset;
        angle                    = rotAngle >> 6;
        entity->ropeData.frameID = (rotAngle >> 10) & 0x1F;
        entity->rotation         = (rotAngle >> 6) & 0xF;
        RSDK.DrawSprite(&entity->ropeData, &entity->ropePos, false);
    }

    entity->drawFX = FX_NONE;
    entity->ropePos.x -= RSDK.Sin512(entity->angle) << 11;
    entity->ropePos.y += RSDK.Cos512(entity->angle) << 11;
    RSDK.DrawSprite(&entity->handleData, &entity->ropePos, false);
    RSDK.DrawSprite(&entity->pivotData, NULL, false);
}

void SwingRope_Create(void *data)
{
    RSDK_THIS(SwingRope);
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(SwingRope->aniFrames, 0, &entity->ropeData, true, 0);
        RSDK.SetSpriteAnimation(SwingRope->aniFrames, 1, &entity->handleData, true, 0);
        RSDK.SetSpriteAnimation(SwingRope->aniFrames, 2, &entity->pivotData, true, 0);
    }
}

void SwingRope_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        SwingRope->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SwingRope.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("AIZ"))
        SwingRope->aniFrames = RSDK.LoadSpriteAnimation("AIZ/SwingRope.bin", SCOPE_STAGE);

    SwingRope->hitbox1.left   = -10;
    SwingRope->hitbox1.top    = -8;
    SwingRope->hitbox1.right  = 10;
    SwingRope->hitbox1.bottom = 24;
    SwingRope->hitbox2.left   = -8;
    SwingRope->hitbox2.top    = -4;
    SwingRope->hitbox2.right  = 8;
    SwingRope->hitbox2.bottom = 20;

    DEBUGMODE_ADD_OBJ(SwingRope);
}

void SwingRope_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SwingRope->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, 0);
}
void SwingRope_DebugSpawn(void)
{
    RSDK_THIS(SwingRope);
    EntitySwingRope *rope = CREATE_ENTITY(SwingRope, NULL, entity->position.x, entity->position.y);
    rope->ropeSize        = 6;
}

void SwingRope_EditorDraw(void)
{
    RSDK_THIS(SwingRope);
    RSDK.SetSpriteAnimation(SwingRope->aniFrames, 0, &entity->ropeData, true, 0);
    RSDK.SetSpriteAnimation(SwingRope->aniFrames, 1, &entity->handleData, true, 0);
    RSDK.SetSpriteAnimation(SwingRope->aniFrames, 2, &entity->pivotData, true, 0);

    entity->rotatedAngle  = 11 * RSDK.Sin512(entity->angleOffset);
    entity->rotatedOffset = entity->rotatedAngle >> 3;

    SwingRope_Draw();
}

void SwingRope_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        SwingRope->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SwingRope.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("AIZ"))
        SwingRope->aniFrames = RSDK.LoadSpriteAnimation("AIZ/SwingRope.bin", SCOPE_STAGE);
}

void SwingRope_Serialize(void)
{
    RSDK_EDITABLE_VAR(SwingRope, VAR_UINT8, ropeSize);
    RSDK_EDITABLE_VAR(SwingRope, VAR_ENUM, angleOffset);
}
