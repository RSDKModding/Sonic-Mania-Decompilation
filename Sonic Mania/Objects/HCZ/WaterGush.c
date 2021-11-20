#include "SonicMania.h"

ObjectWaterGush *WaterGush;

void WaterGush_Update(void)
{
    RSDK_THIS(WaterGush);
    WaterGush_SetupHitboxes();
    int32 flag          = self->flag;
    self->direction = FLIP_NONE;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (!((1 << playerID) & self->activePlayers)) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox2)) {
                self->active = ACTIVE_NORMAL;
                if (!player->sidekick)
                    self->flag = true;
                RSDK.PlaySfx(WaterGush->sfxGush, false, 255);
                self->activePlayers |= 1 << playerID;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, true, 6);
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->tileCollisions  = false;
                player->onGround        = false;
                player->state           = Player_State_None;
            }
        }

        if (((1 << playerID) & self->activePlayers)) {
            int32 xMult = 0;
            int32 yMult = 0;

            int32 posY = 0;
            switch (self->orientation) {
                default: break;
                case 0:
                    flag  = self->position.x;
                    posY  = self->position.y - self->field_78 - 0x140000;
                    yMult = -1;
                    break;
                case 1:
                    xMult = 1;
                    flag  = self->field_78 + 0x140000 + self->position.x;
                    posY  = self->position.y;
                    break;
                case 2:
                    xMult = -1;
                    flag  = self->position.x - self->field_78 - 0x140000;
                    posY  = self->position.y;
                    break;
            }

            player->position.x += (flag - player->position.x) >> 2;
            player->position.y += (posY - player->position.y) >> 2;
            player->state = Player_State_None;

            if ((!Player_CheckCollisionTouch(player, self, &self->hitbox1) && !Player_CheckCollisionTouch(player, self, &self->hitbox2))
                || self->field_84) {
                self->activePlayers &= ~(1 << playerID);
                player->state          = Player_State_Air;
                player->tileCollisions = true;
                player->onGround       = false;
                player->velocity.x     = xMult * (abs(self->speed) << 15);
                player->velocity.y     = yMult * (abs(self->speed) << 15);
            }
        }
    }

    if (self->flag) {
        if (!flag) {
            RSDK.PlaySfx(Water->sfxSplash, false, 255);
            WaterGush_Unknown3();
        }

        if (self->field_84) {
            if (!self->orientation) {
                self->field_7C += 0x3800;
                if (self->field_78 > 0)
                    self->field_78 -= self->field_7C;

                self->field_78 = maxVal(self->field_78, 0);
                if (!self->field_78) {
                    self->flag     = false;
                    self->field_84 = 0;
                    self->field_7C = 0;
                }
            }
        }
        else {
            if (self->field_78 < self->length << 22) {
                self->field_78 += (abs(self->speed) << 15);
            }
            self->field_78 = minVal(self->length << 22, self->field_78);
            if (self->field_78 == self->length << 22)
                self->field_84 = 1;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->field_78 = 0;
        self->flag     = false;
        self->field_84 = 0;
        self->field_7C = 0;
        self->active   = ACTIVE_BOUNDS;
    }
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
}

void WaterGush_LateUpdate(void) {}

void WaterGush_StaticUpdate(void) {}

void WaterGush_Draw(void)
{
    RSDK_THIS(WaterGush);
    if (self->field_78 > 0)
        WaterGush_DrawSprites();
}

void WaterGush_Create(void *data)
{
    RSDK_THIS(WaterGush);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->posUnknown1.x = self->position.x;
    self->posUnknown1.y = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    if (self->orientation == 2 || self->orientation == 1)
        self->updateRange.x = (self->length + 2) << 22;
    self->updateRange.y = 0x800000;
    if (!self->orientation)
        self->updateRange.y = (self->length + 2) << 22;
    if (!self->speed)
        self->speed = 16;
    WaterGush_SetupHitboxes();

    if (self->orientation) {
        if (self->orientation == 1) {
            self->direction = FLIP_NONE;
        }
        else {
            if (self->orientation == 2)
                self->direction = FLIP_X;
        }
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 3, &self->animator2, true, 0);
    }
    else {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 2, &self->animator2, true, 0);
    }
}

void WaterGush_StageLoad(void)
{
    WaterGush->aniFrames = RSDK.LoadSpriteAnimation("HCZ/WaterGush.bin", SCOPE_STAGE);
    WaterGush->sfxGush   = RSDK.GetSfx("HCZ/WaterGush.wav");
}

void WaterGush_SetupHitboxes(void)
{
    RSDK_THIS(WaterGush);

    switch (self->orientation) {
        default: break;
        case 0:
            self->hitbox1.left   = -32;
            self->hitbox1.top    = -20 - (self->field_78 >> 16);
            self->hitbox1.right  = 32;
            self->hitbox1.bottom = 0;

            self->hitbox2.left   = -32;
            self->hitbox2.top    = 0;
            self->hitbox2.right  = 32;
            self->hitbox2.bottom = 16;
            break;
        case 1:
            self->hitbox1.left   = 0;
            self->hitbox1.top    = -32;
            self->hitbox1.right  = (self->field_78 >> 16) + 20;
            self->hitbox1.bottom = 32;

            if (Music->activeTrack == TRACK_EGGMAN1) {
                self->hitbox2.left   = -192;
                self->hitbox2.top    = -16;
                self->hitbox2.bottom = 16;
            }
            else {
                self->hitbox2.left   = -16;
                self->hitbox2.top    = -32;
                self->hitbox2.bottom = 32;
            }
            self->hitbox2.right = 0;
            break;
        case 2:
            self->hitbox1.left   = -20 - (self->field_78 >> 16);
            self->hitbox1.top    = -32;
            self->hitbox1.right  = 0;
            self->hitbox1.bottom = 32;

            self->hitbox2.left   = 0;
            self->hitbox2.top    = -32;
            self->hitbox2.right  = 16;
            self->hitbox2.bottom = 32;
            break;
    }
}

void WaterGush_DrawSprites(void)
{
    RSDK_THIS(WaterGush);
    Vector2 drawPos, drawPos2;

    uint8 storeDir = self->direction;
    drawPos2.x    = self->position.x;
    drawPos2.y    = self->position.y;

    int32 offsetX = 0;
    int32 offsetY = 0;
    switch (self->orientation) {
        default: break;
        case 0:
            self->direction = FLIP_NONE;
            drawPos2.y -= self->field_78;
            offsetX = 0;
            offsetY = 0x400000;
            break;
        case 1:
            self->direction = FLIP_NONE;
            offsetX           = -0x400000;
            offsetY           = 0;
            drawPos2.x += self->field_78;
            break;
        case 2:
            self->direction = FLIP_X;
            offsetX           = 0x400000;
            offsetY           = 0;
            drawPos2.x -= self->field_78;
            break;
    }

    drawPos.x = drawPos2.x;
    drawPos.y = drawPos2.y;
    if (self->field_78 > 0) {
        int32 count = ((self->field_78 - 1) >> 22) + 1;
        for (int32 i = 0; i < count; ++i) {
            drawPos.x += offsetX;
            drawPos.y += offsetY;
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
        }
    }
    RSDK.DrawSprite(&self->animator2, &drawPos2, false);

    self->direction = storeDir;
}

void WaterGush_Unknown3(void)
{
    RSDK_THIS(WaterGush);

    int32 velX1   = 0;
    int32 velY1   = 0;
    int32 dir1    = 0;
    int32 spawnX1 = 0;
    int32 spawnY1 = 0;

    int32 velX2   = 0;
    int32 velY2   = 0;
    int32 dir2    = 0;
    int32 spawnX2 = 0;
    int32 spawnY2 = 0;

    int32 velX3   = 0;
    int32 velY3   = 0;
    int32 dir3    = 0;
    int32 spawnY3 = 0;
    int32 spawnX3 = 0;

    int32 velX4   = 0;
    int32 velY4   = 0;
    int32 dir4    = 0;
    int32 spawnX4 = 0;
    int32 spawnY4 = 0;

    switch (self->orientation) {
        default: break;
        case 0:
            velX1   = -0x30000;
            velY1   = -0x4000 * abs(self->speed);
            dir1    = FLIP_X;
            spawnY1 = self->position.y;
            spawnX1 = self->position.x - 0x200000;

            velX2   = -0x28000;
            velY2   = -0x8000 * abs(self->speed);
            dir2    = FLIP_X;
            spawnX2 = self->position.x - 0x100000;
            spawnY2 = self->position.y;

            velX3   = 0x28000;
            velY3   = -0x8000 * abs(self->speed);
            dir4    = FLIP_NONE;
            spawnX3 = self->position.x + 0x100000;
            spawnY3 = spawnY1;

            velX4   = 0x30000;
            velY4   = -0x4000 * abs(self->speed);
            dir3    = FLIP_NONE;
            spawnX4 = self->position.x + 0x200000;
            spawnY4 = self->position.y;
            break;
        case 1:
            velX1   = abs(self->speed) << 14;
            velY1   = -0x30000;
            dir1    = FLIP_NONE;
            spawnX1 = self->position.x;
            spawnY1 = self->position.y - 0x200000;

            velX2   = abs(self->speed) << 15;
            velY2   = -0x28000;
            dir2    = FLIP_NONE;
            spawnX2 = self->position.x;
            spawnY2 = self->position.y - 0x100000;

            velX3   = velX2;
            velY3   = 0x28000;
            dir3    = FLIP_NONE;
            spawnX3 = self->position.x;
            spawnY3 = self->position.y + 0x100000;

            velX4   = velX1;
            velY4   = 0x30000;
            dir4    = FLIP_NONE;
            spawnX4 = self->position.x;
            spawnY4 = self->position.y + 0x200000;
            break;
        case 2:
            velX1   = -0x4000 * abs(self->speed);
            velY1   = -0x30000;
            dir1    = FLIP_X;
            spawnX1 = self->position.x;
            spawnY1 = self->position.y - 0x200000;

            velX2   = -0x8000 * abs(self->speed);
            velY2   = -0x28000;
            dir2    = FLIP_X;
            spawnX2 = self->position.x;
            spawnY2 = self->position.y - 0x100000;

            velX3   = velX2;
            velY3   = 0x28000;
            dir3    = FLIP_X;
            spawnY3 = self->position.y + 0x100000;
            spawnY4 = self->position.y + 0x200000;

            velX4   = velX1;
            velY4   = 0x30000;
            dir4    = FLIP_X;
            spawnX4 = self->position.x;
            spawnX3 = self->position.x;
            break;
    }

    for (int32 i = 0; i < 2; ++i) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX1, spawnY1);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
        debris->position.x += RSDK.Rand(-0x80000, 0x80000);
        debris->position.y += RSDK.Rand(-0x80000, 0x80000);
        debris->velocity.y = velY1;
        debris->velocity.y >>= i;
        debris->velocity.x = velX1;
        debris->direction  = dir1;
        debris->drawFX     = FX_FLIP;
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->gravity    = 0x3800;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX2, spawnY2);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
        debris->position.x += RSDK.Rand(-0x80000, 0x80000);
        debris->position.y += RSDK.Rand(-0x80000, 0x80000);
        debris->velocity.y = velY2;
        debris->velocity.y >>= i;
        debris->velocity.x = velX2;
        debris->direction  = dir2;
        debris->drawFX     = FX_SCALE | FX_FLIP;
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->gravity    = 0x3800;
        debris->scale.x    = 512;
        debris->scale.y    = 512;
        debris->scaleInc.x = 8 >> i;
        debris->scaleInc.y = 8 >> i;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX3, spawnY3);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
        debris->position.x += RSDK.Rand(-0x80000, 0x80000);
        debris->position.y += RSDK.Rand(-0x80000, 0x80000);
        debris->velocity.x = velX3;
        debris->velocity.y = velY3;
        debris->velocity.y >>= i;
        debris->direction  = dir3;
        debris->drawFX     = FX_SCALE | FX_FLIP;
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->gravity    = 0x3800;
        debris->scale.x    = 512;
        debris->scale.y    = 512;
        debris->scaleInc.x = 8 >> i;
        debris->scaleInc.y = 8 >> i;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX4, spawnY4);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
        debris->position.x += RSDK.Rand(-0x80000, 0x80000);
        debris->position.y += RSDK.Rand(-0x80000, 0x80000);
        debris->velocity.x = velX4;
        debris->velocity.y = velY4;
        debris->velocity.y >>= i;
        debris->direction = dir4;
        debris->drawFX    = FX_FLIP;
        debris->drawOrder = Zone->drawOrderHigh;
        debris->gravity   = 0x3800;
    }
}

#if RETRO_INCLUDE_EDITOR
void WaterGush_EditorDraw(void)
{
    RSDK_THIS(WaterGush);

    self->updateRange.x = 0x800000;
    if (self->orientation == 2 || self->orientation == 1)
        self->updateRange.x = (self->length + 2) << 22;
    self->updateRange.y = 0x800000;
    if (!self->orientation)
        self->updateRange.y = (self->length + 2) << 22;

    if (self->orientation) {
        if (self->orientation == 1) {
            self->direction = FLIP_NONE;
        }
        else {
            if (self->orientation == 2)
                self->direction = FLIP_X;
        }
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 3, &self->animator2, true, 0);
    }
    else {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 2, &self->animator2, true, 0);
    }

    self->field_78 = self->length << 22;

    WaterGush_DrawSprites();
}

void WaterGush_EditorLoad(void) { WaterGush->aniFrames = RSDK.LoadSpriteAnimation("HCZ/WaterGush.bin", SCOPE_STAGE); }
#endif

void WaterGush_Serialize(void)
{
    RSDK_EDITABLE_VAR(WaterGush, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(WaterGush, VAR_UINT32, length);
    RSDK_EDITABLE_VAR(WaterGush, VAR_INT32, speed);
}
