#include "../SonicMania.h"

ObjectSwingRope *SwingRope;

void SwingRope_Update(void)
{
    RSDK_THIS(SwingRope);
    int sine              = 11 * RSDK.Sin512(entity->angleOffset + 3 * Zone->timer);
    entity->rotatedAngle  = sine;
    entity->rotatedOffset = sine >> 3;
    entity->rotatePos.x   = entity->position.x;
    entity->rotatePos.y   = entity->position.y;

    for (int s = 0; s < entity->ropeSize; ++s) {
        entity->angle = (sine >> 6) & 0x1FF;
        entity->rotatePos.x -= RSDK.Sin512(entity->angle) << 11;
        entity->rotatePos.y += RSDK.Cos512(entity->angle) << 11;
        sine += entity->rotatedOffset;
    }

    int rotateX   = entity->rotatePos.x;
    int rotateY   = entity->rotatePos.y;
    entity->angle = (sine >> 6) & 0x1FF;
    int playerX   = -0x800 * RSDK.Sin512(entity->angle) + rotateX;
    rotateY += 0x700 * RSDK.Cos512(entity->angle);
    entity->rotatePos.x -= RSDK.Sin512(entity->angle) << 11;
    entity->rotatePos.y += RSDK.Cos512(entity->angle) << 11;

    int storeX         = entity->position.x;
    int storeY         = entity->position.y;
    entity->position.x = entity->rotatePos.x;
    entity->position.y = entity->rotatePos.y;
    entity->velocity.x = entity->rotatePos.x - entity->ropePos.x;
    entity->velocity.y = entity->rotatePos.y - entity->ropePos.y;
    if (entity->ropeGrabDelay > 0)
        entity->ropeGrabDelay--;

    EntityPlayer *player = NULL;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        if (player->state == Player_State_None) {
            if (Player_CheckCollisionTouch(player, entity, &SwingRope->hitbox2)) {
                if (player->jumpPress) {
                    player->jumpAbilityTimer = 1;
                    player->state            = Player_State_Air;
                    player->drawOrder        = entity->playerLayers[player->playerID];
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimData, 0, 0);
                    player->velocity.x = entity->velocity.x >> 1;
                    if (player->left) {
                        player->velocity.x = -0x20000;
                    }
                    else if (player->right) {
                        player->velocity.x = 0x20000;
                    }
                    player->velocity.y    = (entity->velocity.y >> 1) - 0x38000;
                    player->onGround      = 0;
                    player->jumpAbility   = 0;
                    entity->ropeGrabDelay = 30;
                }
                else {
                    player->position.x = playerX;
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
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimData, 0, 0);
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->position.x = playerX;
                player->position.y = rotateY - ((playerHitbox->top + 2) << 16);
                RSDK.PlaySFX(Player->sfx_Grab, 0, 255);
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
    entity->rotation         = entity->rotatedAngle & 0xF;
    entity->drawFX           = FX_ROTATE;
    RSDK.DrawSprite(&entity->ropeData, 0, 0);

    entity->ropePos.x = entity->position.x;
    entity->ropePos.y = entity->position.y;

    int angle = entity->rotatedAngle >> 6;
    for (int s = 0; s < entity->ropeSize; ++s) {
        entity->angle = angle & 0x1FF;
        entity->ropePos.x -= RSDK.Sin512(entity->angle) << 11;
        entity->ropePos.y += RSDK.Cos512(entity->angle) << 11;
        int rotAngle             = entity->rotatedOffset + entity->rotatedAngle;
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
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(SwingRope->spriteIndex, 0, &entity->ropeData, true, 0);
        RSDK.SetSpriteAnimation(SwingRope->spriteIndex, 1, &entity->handleData, true, 0);
        RSDK.SetSpriteAnimation(SwingRope->spriteIndex, 2, &entity->pivotData, true, 0);
    }
}

void SwingRope_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ")) {
        SwingRope->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/SwingRope.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("AIZ")) {
        SwingRope->spriteIndex = RSDK.LoadSpriteAnimation("AIZ/SwingRope.bin", SCOPE_STAGE);
    }

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
    RSDK.SetSpriteAnimation(SwingRope->spriteIndex, 2, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
}
void SwingRope_DebugSpawn(void)
{
    RSDK_THIS(SwingRope);
    EntitySwingRope *rope = (EntitySwingRope *)RSDK.CreateEntity(SwingRope->objectID, NULL, entity->position.x, entity->position.y);
    rope->ropeSize        = 6;
}

void SwingRope_EditorDraw(void) {}

void SwingRope_EditorLoad(void) {}

void SwingRope_Serialize(void)
{
    RSDK_EDITABLE_VAR(SwingRope, VAR_UINT8, ropeSize);
    RSDK_EDITABLE_VAR(SwingRope, VAR_ENUM, angleOffset);
}
