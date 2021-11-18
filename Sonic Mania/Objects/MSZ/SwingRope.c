#include "SonicMania.h"

ObjectSwingRope *SwingRope;

void SwingRope_Update(void)
{
    RSDK_THIS(SwingRope);

    int32 sine              = 11 * RSDK.Sin512(self->angleOffset + 3 * Zone->timer);
    self->rotatedAngle  = sine;
    self->rotatedOffset = sine >> 3;
    self->rotatePos.x   = self->position.x;
    self->rotatePos.y   = self->position.y;

    for (int32 s = 0; s < self->ropeSize; ++s) {
        self->angle = (sine >> 6) & 0x1FF;
        self->rotatePos.x -= RSDK.Sin512(self->angle) << 11;
        self->rotatePos.y += RSDK.Cos512(self->angle) << 11;
        sine += self->rotatedOffset;
    }

    int32 rotateX   = self->rotatePos.x;
    int32 rotateY   = self->rotatePos.y;
    self->angle = (sine >> 6) & 0x1FF;
    rotateY += 0x700 * RSDK.Cos512(self->angle);
    self->rotatePos.x -= RSDK.Sin512(self->angle) << 11;
    self->rotatePos.y += RSDK.Cos512(self->angle) << 11;

    int32 storeX         = self->position.x;
    int32 storeY         = self->position.y;
    self->position.x = self->rotatePos.x;
    self->position.y = self->rotatePos.y;
    self->velocity.x = self->rotatePos.x - self->ropePos.x;
    self->velocity.y = self->rotatePos.y - self->ropePos.y;
    if (self->ropeGrabDelay > 0)
        self->ropeGrabDelay--;

    foreach_active(Player, player)
    {
        if (player->state == Player_State_None) {
            if (Player_CheckCollisionTouch(player, self, &SwingRope->hitbox2)) {
                if (player->jumpPress) {
                    player->jumpAbilityTimer = 1;
                    player->state            = Player_State_Air;
                    player->drawOrder        = self->playerLayers[player->playerID];
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->velocity.x = self->velocity.x >> 1;
                    if (player->left) {
                        player->velocity.x = -0x20000;
                    }
                    else if (player->right) {
                        player->velocity.x = 0x20000;
                    }
                    player->velocity.y    = (self->velocity.y >> 1) - 0x38000;
                    player->onGround      = false;
                    player->jumpAbility   = 0;
                    self->ropeGrabDelay = 30;
                }
                else {
                    player->position.x = rotateX + (-0x800 * RSDK.Sin512(self->angle));
                    player->position.y = rotateY - ((Player_GetHitbox(player)->top + 2) << 16);
                }
            }
        }
        else if (!self->ropeGrabDelay && !player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            Hitbox otherHitbox;
            otherHitbox.left   = playerHitbox->left;
            otherHitbox.right  = playerHitbox->right;
            otherHitbox.top    = 0;
            otherHitbox.bottom = 0;
            if (RSDK.CheckObjectCollisionTouchBox(self, &SwingRope->hitbox1, player, &otherHitbox)) {
                self->playerLayers[player->playerID] = player->drawOrder;
                player->drawOrder                      = Zone->drawOrderLow;
                player->state                          = Player_State_None;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->playerAnimator, 0, 0);
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->position.x = -0x800 * RSDK.Sin512(self->angle) + rotateX;
                player->position.y = rotateY - ((playerHitbox->top + 2) << 16);
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
            }
        }
    }
    self->position.x = storeX;
    self->position.y = storeY;
}

void SwingRope_LateUpdate(void) {}

void SwingRope_StaticUpdate(void) {}

void SwingRope_Draw(void)
{
    RSDK_THIS(SwingRope);

    self->ropeData.frameID = (self->rotatedAngle >> 10) & 0x1F;
    self->rotation         = (self->rotatedAngle >> 6) & 0xF;
    self->drawFX           = FX_ROTATE;
    RSDK.DrawSprite(&self->ropeData, NULL, false);

    self->ropePos.x = self->position.x;
    self->ropePos.y = self->position.y;

    int32 angle    = self->rotatedAngle >> 6;
    int32 rotAngle = self->rotatedAngle;
    for (int32 s = 0; s < self->ropeSize; ++s) {
        self->angle = angle & 0x1FF;
        self->ropePos.x -= RSDK.Sin512(self->angle) << 11;
        self->ropePos.y += RSDK.Cos512(self->angle) << 11;
        rotAngle += self->rotatedOffset;
        angle                    = rotAngle >> 6;
        self->ropeData.frameID = (rotAngle >> 10) & 0x1F;
        self->rotation         = (rotAngle >> 6) & 0xF;
        RSDK.DrawSprite(&self->ropeData, &self->ropePos, false);
    }

    self->drawFX = FX_NONE;
    self->ropePos.x -= RSDK.Sin512(self->angle) << 11;
    self->ropePos.y += RSDK.Cos512(self->angle) << 11;
    RSDK.DrawSprite(&self->handleData, &self->ropePos, false);
    RSDK.DrawSprite(&self->pivotData, NULL, false);
}

void SwingRope_Create(void *data)
{
    RSDK_THIS(SwingRope);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(SwingRope->aniFrames, 0, &self->ropeData, true, 0);
        RSDK.SetSpriteAnimation(SwingRope->aniFrames, 1, &self->handleData, true, 0);
        RSDK.SetSpriteAnimation(SwingRope->aniFrames, 2, &self->pivotData, true, 0);
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
    EntitySwingRope *rope = CREATE_ENTITY(SwingRope, NULL, self->position.x, self->position.y);
    rope->ropeSize        = 6;
}

void SwingRope_EditorDraw(void)
{
    RSDK_THIS(SwingRope);
    RSDK.SetSpriteAnimation(SwingRope->aniFrames, 0, &self->ropeData, true, 0);
    RSDK.SetSpriteAnimation(SwingRope->aniFrames, 1, &self->handleData, true, 0);
    RSDK.SetSpriteAnimation(SwingRope->aniFrames, 2, &self->pivotData, true, 0);

    self->rotatedAngle  = 11 * RSDK.Sin512(self->angleOffset);
    self->rotatedOffset = self->rotatedAngle >> 3;

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
