#include "SonicMania.h"

ObjectPlatform *Platform;

void Platform_Update(void)
{
    RSDK_THIS(Platform);
#if RETRO_USE_PLUS
    if (Zone->flag) {
        self->pushPlayersL = false;
        self->pushPlayersR = false;
    }
#endif
    self->position.x = self->drawPos.x & 0xFFFF0000;
    self->position.y = self->drawPos.y & 0xFFFF0000;

    if (self->hasTension) {
        if (self->stood && self->stoodAngle < 64)
            self->stoodAngle += 4;
        else if (!self->stood && self->stoodAngle > 0)
            self->stoodAngle -= 4;
    }

    self->collisionOffset.x = -self->position.x;
    self->collisionOffset.y = -self->position.y;

    StateMachine_Run(self->state);

    if (self->objectID) {
        self->stood = false;
        self->collisionOffset.x += self->drawPos.x & 0xFFFF0000;
        self->collisionOffset.y += self->drawPos.y & 0xFFFF0000;
        if (self->state != Platform_State_Falling && self->state != Platform_State_OffScreenReset) {
            StateMachine_Run(self->stateCollide);
        }

        self->position.x = self->centerPos.x;
        self->position.y = self->centerPos.y;
        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK.GetEntityByID(s++);
            if (child->objectID == ItemBox->objectID) {
                if (!child->scale.y) {
                    EntityItemBox *itemBox = (EntityItemBox *)child;
                    itemBox->parent        = (Entity *)self;
                    itemBox->scale.x       = itemBox->position.x - self->centerPos.x;
                    itemBox->scale.y       = itemBox->position.y - self->centerPos.y;
                    itemBox->position.x    = itemBox->scale.x + self->drawPos.x;
                    itemBox->position.y    = itemBox->scale.y + self->drawPos.y;
                    itemBox->updateRange.x = self->updateRange.x;
                    itemBox->updateRange.y = self->updateRange.y;
                }
            }
            else {
                child->position.x += self->collisionOffset.x;
                child->position.y += self->collisionOffset.y;
                if (child->objectID == Spikes->objectID) {
                    EntitySpikes *spikes = (EntitySpikes *)child;
                    spikes->offset.x     = self->collisionOffset.x;
                    spikes->offset.y     = self->collisionOffset.y;
                }
                else if (child->objectID == Platform->objectID) {
                    EntityPlatform *platform = (EntityPlatform *)child;
                    platform->centerPos.x += self->collisionOffset.x;
                    platform->centerPos.y += self->collisionOffset.y;
                }
                else if (Ice && child->objectID == Ice->objectID) {
                    EntityIce *ice = (EntityIce *)child;
                    ice->playerPos = self->collisionOffset;
                }
                else if (TurboTurtle && child->objectID == TurboTurtle->objectID) {
                    EntityTurboTurtle *turboTurtle = (EntityTurboTurtle *)child;
                    turboTurtle->startPos.x += self->collisionOffset.x;
                    turboTurtle->startPos.y += self->collisionOffset.y;
                }
            }
        }
        if (self->animator.frameDelay)
            RSDK.ProcessAnimation(&self->animator);
    }
}

void Platform_LateUpdate(void) {}

void Platform_StaticUpdate(void)
{
    for (int32 i = 0; i < 4; ++i) {
        Platform->stoodPos[i].x = 0;
        Platform->stoodPos[i].y = 0;
    }
}

void Platform_Draw(void)
{
    RSDK_THIS(Platform);
    Vector2 drawPos;

    if (self->frameID >= 0) {
        if ((self->state != Platform_State_Circular || !self->hasTension)
            && (self->state != Platform_State_Swing && self->state != Platform_State_14 && self->type != PLATFORM_12)) {
            if (Platform->aniFrames == 0xFFFF)
                RSDK.DrawRect(self->drawPos.x - 0x200000, self->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, 255, INK_NONE, false);
            else
                RSDK.DrawSprite(&self->animator, &self->drawPos, false);
        }
        else {
            int32 ang = 0;
            if (self->state == Platform_State_Circular && self->hasTension)
                ang = self->speed * Zone->timer + 4 * self->angle;
            else
                ang = self->angle;
            int32 fxStore = self->drawFX;
            self->drawFX |= FX_FLIP | FX_ROTATE;
            int32 cnt                = (self->amplitude.y >> 10) - 1;
            int32 angle              = 0x400;
            self->direction        = FLIP_NONE;
            self->animator.frameID = self->frameID + 1;

            int32 rot = ang >> 1;
            for (int32 i = 0; i < cnt; ++i) {
                drawPos.x = angle * RSDK.Cos1024(ang) + self->centerPos.x;
                drawPos.y = angle * RSDK.Sin1024(ang) + self->centerPos.y;
                RSDK.DrawSprite(&self->animator, &drawPos, false);
                angle += 0x400;
                self->direction ^= FLIP_X;
                self->rotation = rot;
            }

            self->drawFX           = fxStore;
            self->animator.frameID = self->frameID + 2;
            RSDK.DrawSprite(&self->animator, &self->centerPos, false);
            self->animator.frameID = self->frameID;

            if (Platform->aniFrames == 0xFFFF)
                RSDK.DrawRect(self->drawPos.x - 0x200000, self->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, 255, INK_NONE, false);
            else
                RSDK.DrawSprite(&self->animator, &self->drawPos, false);
        }
    }
}

void Platform_Create(void *data)
{
    RSDK_THIS(Platform);
    if (!SceneInfo->inEditor) {
        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;
    }
    self->active      = ACTIVE_BOUNDS;
    self->visible     = true;
    self->drawOrder   = Zone->drawOrderLow + 1;
    self->centerPos.x = self->position.x;
    self->centerPos.y = self->position.y;
    self->drawPos.x   = self->position.x;
    self->drawPos.y   = self->position.y;
    RSDK.SetSpriteAnimation(Platform->aniFrames, 0, &self->animator, true, 0);
    self->animator.frameID = self->frameID;
    if (!SceneInfo->inEditor && RSDK.GetFrameID(&self->animator) == 108)
        self->drawOrder = Zone->drawOrderLow;

    switch (self->type) {
        case PLATFORM_FIXED:
        default:
            self->state         = Platform_State_Normal;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            break;
        case PLATFORM_COLLAPSING:
            self->state         = Platform_State_Collapsing;
            self->updateRange.x = 0x800000;
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
            break;
        case PLATFORM_MOVING:
            self->state         = Platform_State_Moving;
            self->updateRange.x = (abs(self->amplitude.x) + 0x2000) << 10;
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
            self->rotation      = self->angle;
            self->angle         = 0;
            break;
        case PLATFORM_3:
            self->updateRange.x = (abs(self->amplitude.x) + 0x2000) << 10;
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
            self->state         = Platform_State_Circular;
            break;
        case PLATFORM_5:
        case PLATFORM_11:
            self->active = ACTIVE_BOUNDS;
            if (self->type == PLATFORM_5)
                self->state = Platform_State_Nothing;
            else
                self->state = Platform_Unknown8;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            break;
        case PLATFORM_6:
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            if (!SceneInfo->inEditor) {
                self->speed <<= 11;
                self->position.x += 0x8000;
            }
            self->state = Platform_State_StartPush;
            break;
        case PLATFORM_8:
            self->amplitude.x <<= 10;
            self->updateRange.x = 0x800000 + abs(self->amplitude.x);
            self->updateRange.y = 0x800000 + abs(self->amplitude.x);
            if (self->speed < 0)
                self->direction = FLIP_X;
            self->state = Platform_State_Wait;
            break;
        case PLATFORM_9:
            self->amplitude.x <<= 10;
            self->updateRange.x = 0x800000 + abs(self->amplitude.x);
            self->updateRange.y = 0x800000 + abs(self->amplitude.x);
            if (self->speed < 0)
                self->direction = 1;
            self->state = Platform_State_WaitBobbing;
            break;
        case PLATFORM_10:
            self->amplitude.x <<= 10;
            self->updateRange.x = 0x800000 + abs(self->amplitude.x);
            self->updateRange.y = 0x800000 + abs(self->amplitude.x);

            if (self->speed < 0) {
                self->direction = 1;
                self->speed     = -self->speed;
            }
            self->state = Platform_State_PlayerActivated;
            break;
        case PLATFORM_12: self->collapseDelay = 88;
        case PLATFORM_4:
            self->updateRange.x = (abs(self->amplitude.y) + 512) << 14;
            self->updateRange.y = (abs(self->amplitude.y) + 512) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            self->amplitude.y *= 16;
            self->groundVel = 4 * self->angle;
            self->angle     = self->groundVel + 256 + (self->amplitude.x * RSDK.Sin1024(self->speed * self->collapseDelay) >> 14);
            self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
            self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
            if (self->type == PLATFORM_4) {
                self->state = Platform_State_Swing;
            }
            else {
                if (self->groundVel >= 0)
                    self->drawPos.x -= 0x200000;
                else
                    self->drawPos.x += 0x200000;
                self->state = Platform_State_12;
            }
            break;
        case PLATFORM_13:
            if (self->direction) {
                self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9);
                self->drawPos.y = self->centerPos.y + (self->amplitude.y << 9);
            }
            else {
                self->drawPos.x = self->centerPos.x - (self->amplitude.x << 9);
                self->drawPos.y = self->centerPos.y - (self->amplitude.y << 9);
            }
        case PLATFORM_7:
            self->updateRange.x = (abs(self->amplitude.x) + 0x4000) << 9;
            self->updateRange.y = (abs(self->amplitude.y) + 0x4000) << 9;
            if (self->speed < 0)
                self->direction = FLIP_X;

            if (self->type == PLATFORM_7)
                self->state = Platform_State_MovingSpike;
            else
                self->state = Platform_State_13;
            break;
        case PLATFORM_14:
            self->updateRange.x = (abs(self->amplitude.y) + 512) << 14;
            self->updateRange.y = (abs(self->amplitude.y) + 512) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            self->amplitude.y *= 16;
            self->groundVel = 4 * self->angle;
            self->angle     = self->groundVel + 256 + (self->amplitude.x * RSDK.Sin1024(self->speed * self->collapseDelay) >> 14);
            self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
            self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
            self->state     = Platform_State_14;
            break;
        case PLATFORM_15:
            self->state         = Platform_State_15;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            break;
        case PLATFORM_16:
            self->updateRange.x = 0x800000;
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
            if (!self->speed)
                self->speed = 1;
            self->amplitude.y = self->amplitude.y << 10;
            self->velocity.y  = self->speed << 16;
            self->state       = Platform_State_16;
            break;
    }

    if (self->frameID >= 0) {
        int32 f    = self->frameID;
        int32 anim = 0;
        while (f >= self->animator.frameCount) {
            if (!self->animator.frameCount)
                break;
            f -= self->animator.frameCount;
            RSDK.SetSpriteAnimation(Platform->aniFrames, ++anim, &self->animator, true, 0);
        }
        self->frameID          = f;
        self->animator.frameID = f;
    }
    else {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator, true, 0);
    }

    if (!SceneInfo->inEditor) {
        if (self->collision != PLATFORM_C_4) {
            Hitbox *hitbox = RSDK.GetHitbox(&self->animator, self->collision != PLATFORM_C_0);
            if (Platform->aniFrames != 0xFFFF && hitbox) {
                self->hitbox.left   = hitbox->left;
                self->hitbox.top    = hitbox->top;
                self->hitbox.right  = hitbox->right;
                self->hitbox.bottom = hitbox->bottom;
            }
            else {
                self->hitbox.left   = -32;
                self->hitbox.top    = -16;
                self->hitbox.right  = -8;
                self->hitbox.bottom = 32;
            }
        }
        switch (self->collision) {
            case PLATFORM_C_0:
                self->stateCollide  = Platform_CollisionState_TopSolid;
                self->hitbox.bottom = self->hitbox.top + 8;
                break;
            case PLATFORM_C_1:
            default: self->stateCollide = Platform_CollisionState_AllSolid; break;
            case PLATFORM_C_2: self->stateCollide = Platform_CollisionState_Tiles; break;
            case PLATFORM_C_3: self->stateCollide = Platform_CollisionState_AllHazard; break;
            case PLATFORM_C_4: self->stateCollide = Platform_CollisionState_None; break;
            case PLATFORM_C_5: self->stateCollide = Platform_CollisionState_LRHazard; break;
            case PLATFORM_C_6: self->stateCollide = Platform_CollisionState_BottomHazard; break;
            case PLATFORM_C_7: self->stateCollide = Platform_CollisionState_TopHazard; break;
            case PLATFORM_C_8: self->stateCollide = Platform_CollisionState_Twister; break;
            case PLATFORM_C_9:
            case PLATFORM_C_10:
            case PLATFORM_C_11:
            case PLATFORM_C_12:
            case PLATFORM_C_13: self->stateCollide = Platform_CollisionState_Sticky; break;
            case PLATFORM_C_14: self->stateCollide = Platform_CollisionState_TurnTable; break;
            case PLATFORM_C_15: self->stateCollide = Platform_CollisionState_AllSolid_NoCrush; break;
        }
        for (int32 i = 0; i < self->childCount; ++i) {
            EntityPlatform *child = RSDK_GET_ENTITY((i + RSDK.GetEntityID(self) + 1), Platform);
            child->tileCollisions = false;
            if (HangPoint && child->objectID == HangPoint->objectID) {
                EntityHangPoint *hang = (EntityHangPoint *)child;
                if (self->updateRange.y < 0x800000 + abs(self->position.y - (hang->length << 16) - hang->position.y))
                    self->updateRange.y = 0x800000 + abs(self->position.y - (hang->length << 16) - hang->position.y);
            }
            else {
                if (self->updateRange.y < 0x800000 + abs(self->position.y - child->position.y))
                    self->updateRange.y = 0x800000 + abs(self->position.y - child->position.y);
            }
        }
    }
}

void Platform_StageLoad(void)
{
    Platform->useClack = false;
    if (RSDK.CheckStageFolder("GHZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("GHZCutscene")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("GHZCutscene/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Platform.bin", SCOPE_STAGE);
        Platform->sfxClack   = RSDK.GetSFX("Stage/Clack2.wav");
        Platform->useClack    = true;
    }
    else if (RSDK.CheckStageFolder("PSZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("PSZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE);
        Platform->sfxClack   = RSDK.GetSFX("Stage/Clack2.wav");
        Platform->useClack    = true;
    }
    else if (RSDK.CheckStageFolder("MSZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MMZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("AIZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Blueprint")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/Platform.bin", SCOPE_STAGE);
    }

    Platform->sfxClacker = RSDK.GetSFX("Stage/Clacker.wav");
    Platform->sfxClang   = RSDK.GetSFX("Stage/Clang.wav");
    Platform->sfxPush    = RSDK.GetSFX("Global/Push.wav");
    if (Platform->playingPushSFX) {
        RSDK.StopSFX(Platform->sfxPush);
        Platform->playingPushSFX = 0;
    }
}

void Platform_State_Nothing(void)
{
    RSDK_THIS(Platform);
    self->velocity.x = 0;
    self->velocity.y = 0;
}

void Platform_State_Normal(void)
{
    RSDK_THIS(Platform);
    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y;
    self->drawPos.y += RSDK.Sin256(self->stoodAngle) << 10;
    self->velocity.x = 0;
    self->velocity.y = 0;
}

void Platform_State_Moving(void)
{
    RSDK_THIS(Platform);
    int32 drawX       = -self->drawPos.x;
    int32 drawY       = -self->drawPos.y;
    self->drawPos.x = self->amplitude.x * RSDK.Sin1024(self->speed * (self->rotation + Zone->timer)) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->speed * (self->rotation + Zone->timer)) + self->centerPos.y;
    self->drawPos.y += RSDK.Sin256(self->stoodAngle) << 10;
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Swing(void)
{
    RSDK_THIS(Platform);
    int32 drawX        = -self->drawPos.x;
    int32 drawY        = -self->drawPos.y;
    self->angle      = self->groundVel + 0x100 + ((self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer) + 0x200) >> 14);
    self->drawPos.x  = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y  = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Wait(void)
{
    RSDK_THIS(Platform);
    if (self->stood) {
        if (self->hasTension)
            self->collapseDelay = 120;
        self->rotation     = 0;
        self->tileOrigin.x = 0;
        self->tileOrigin.y = 0;
        self->active       = ACTIVE_NORMAL;
        self->state        = Platform_State_PlayerMove;
    }
    self->velocity.y = 0;
    self->velocity.x = 0;
}

void Platform_State_WaitBobbing(void)
{
    RSDK_THIS(Platform);
    self->rotation += 4;
    self->drawPos.y = self->centerPos.y + (RSDK.Sin1024(self->rotation) << 9);
    if (self->stood) {
        if (self->hasTension)
            self->collapseDelay = 120;
        self->tileOrigin.x = self->drawPos.x - self->centerPos.x;
        self->centerPos.x  = self->drawPos.x;
        self->tileOrigin.y = RSDK.Sin1024(self->rotation) << 9;
        self->centerPos.y  = self->drawPos.y;
        self->active       = ACTIVE_NORMAL;
        self->state        = Platform_State_PlayerMove;
    }
    self->velocity.y = 0;
    self->velocity.x = 0;
}

void Platform_State_StartPush(void)
{
    RSDK_THIS(Platform);
    self->active        = ACTIVE_NORMAL;
    self->updateRange.x = 0x2000000;
    self->updateRange.y = 0x2000000;
    self->state         = Platform_State_Pushable;
    self->velocity.x    = 0;
    self->velocity.y    = 0;
}

void Platform_State_StartFalling(void)
{
    RSDK_THIS(Platform);
    if (--self->collapseDelay <= 0) {
        self->collapseDelay = 0;
        self->state         = Platform_State_Falling;
        foreach_active(Player, player)
        {
            if ((1 << RSDK.GetEntityID(player)) & self->stoodPlayers)
                player->velocity.y = self->velocity.y - 0x10000;
        }
    }

    self->drawPos.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->velocity.x = 0;
}

void Platform_State_PlayerActivated(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            int32 angle = (uint8)((uint8)(self->angle) + -0x80);
            int32 xOff  = (player->position.x - self->centerPos.x) >> 8;
            int32 yOff  = (player->position.y - self->centerPos.y) >> 8;
            RSDK.Sin256(angle);
            RSDK.Cos256(angle);

            int32 cy = (yOff * RSDK.Cos256(angle)) - xOff * RSDK.Sin256(angle);

            // int32 center = cy + self->centerPos.y;
            if (abs(player->position.x - self->centerPos.x) <= 0x4000000) {
                if (abs(player->position.y - self->centerPos.y) <= 0x4000000 && (cy + self->centerPos.y) < self->centerPos.y) {
                    if (self->centerPos.y - (cy + self->centerPos.y) < 0x1000000)
                        flag = true;
                }
            }
        }
    }

    if (!flag) {
        if (self->amplitude.y > 0) {
            self->amplitude.y = self->amplitude.y - (self->speed << 16);
            if (self->amplitude.y <= 0) {
                self->amplitude.y = 0;
                if (self->activeScreens && Platform->useClack)
                    RSDK.PlaySfx(Platform->sfxClack, 0, 255);
            }
        }
    }
    else {
        if (self->amplitude.y < self->amplitude.x) {
            self->amplitude.y = self->amplitude.y + (self->speed << 16);
            if (self->amplitude.y >= self->amplitude.x) {
                self->amplitude.y = self->amplitude.x;
                if (self->activeScreens && Platform->useClack)
                    RSDK.PlaySfx(Platform->sfxClack, 0, 255);
            }
        }
    }

    if (self->direction) {
        self->drawPos.x = (-self->amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
        self->drawPos.y = (-self->amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;
    }
    else {
        self->drawPos.x = (self->amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
        self->drawPos.y = (self->amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;
    }

    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_PlayerMove(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    self->amplitude.y += self->groundVel;

    self->drawPos.x = (self->amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
    self->drawPos.y = (self->amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;

    int32 speed16 = self->speed << 16;
    if (self->groundVel == speed16) {
        if (self->amplitude.y >= self->amplitude.x) {
            self->amplitude.y = self->amplitude.x;
            self->groundVel   = self->groundVel - (self->speed << 11);
            self->state       = Platform_Unknown5;
        }
    }
    else {
        self->groundVel += (self->speed << 10);
        if (self->groundVel >= speed16) {
            self->groundVel = speed16;
            self->centerPos.x += ((self->amplitude.y + self->groundVel) >> 8) * RSDK.Cos256(self->angle);
            self->centerPos.y += ((self->groundVel + self->amplitude.y) >> 8) * RSDK.Sin256(self->angle);
            self->amplitude.y = -self->groundVel;
        }
    }
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Pushable(void)
{
    RSDK_THIS(Platform);
    self->velocity.x = 0;
    self->velocity.y = 0;
    foreach_active(Player, playerLoop)
    {
        uint8 id = 1 << RSDK.GetEntityID(playerLoop);
        if (id & self->pushPlayersL)
            self->velocity.x += self->speed;
        if (id & self->pushPlayersR)
            self->velocity.x -= self->speed;
    }

    if (self->velocity.x > 0 || self->velocity.x < 0) {
        if (!Platform->playingPushSFX) {
            RSDK.PlaySfx(Platform->sfxPush, true, 255);
            Platform->playingPushSFX = true;
        }
    }

    if (self->velocity.x) {
        if (self->collapseDelay > 0) {
            self->collapseDelay--;
            self->velocity.x = 0;
        }
    }
    else {
        if (Platform->playingPushSFX) {
            RSDK.StopSFX(Platform->sfxPush);
            Platform->playingPushSFX = false;
        }
        if (self->collapseDelay < 4) {
            self->collapseDelay++;
        }
    }

    self->drawPos.x += self->velocity.x;
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 bitID          = 1 << RSDK.GetEntityID(&player);
        if (bitID & self->pushPlayersL)
            player->position.x = self->drawPos.x + ((self->hitbox.left - playerHitbox->right) << 16);
        if (bitID & self->pushPlayersR)
            player->position.x = self->drawPos.x + ((self->hitbox.right - playerHitbox->left - 1) << 16);
    }

    bool32 collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, 0, 0, (self->hitbox.left + 16) << 16, self->hitbox.bottom << 16, 4);
    int32 y         = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, 0, 0, 0, self->hitbox.bottom << 16, 4);
    if (self->position.y < y)
        y = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, 0, 0, (self->hitbox.right - 16) << 16, self->hitbox.bottom << 16, 4);
    if (self->position.y < y)
        y = self->position.y;

    self->position.y = y;
    self->drawPos.y  = y;
    if (!collided) {
        int32 vel          = 2 * self->velocity.x;
        self->velocity.x = vel;
        if (!vel) {
            self->state = Platform_Unknown3;
        }
        else {
            if (vel > 0)
                self->state = Platform_Unknown1;
            else
                self->state = Platform_Unknown2;
        }
    }
    RSDK.ObjectTileCollision(self, Zone->fgLayers, 3, 0, self->hitbox.left << 16, (self->hitbox.bottom - 8) << 16, 1);
    RSDK.ObjectTileCollision(self, Zone->fgLayers, 1, 0, self->hitbox.right << 16, (self->hitbox.bottom - 8) << 16, 1);
    RSDK.ObjectTileCollision(self, Zone->fgLayers, 3, 0, self->hitbox.left << 16, (self->hitbox.top + 8) << 16, 1);
    RSDK.ObjectTileCollision(self, Zone->fgLayers, 1, 0, self->hitbox.right << 16, (self->hitbox.top + 8) << 16, 1);

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;
    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->speed >>= 11;
        self->drawPos.x  = self->centerPos.x;
        self->drawPos.y  = self->centerPos.y;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->visible    = false;
        if (Platform->playingPushSFX) {
            RSDK.StopSFX(Platform->sfxPush);
            Platform->playingPushSFX = false;
        }
        self->state = Platform_State_OffScreenReset;
    }
}

void Platform_State_OffScreenReset(void)
{
    RSDK_THIS(Platform);
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        Platform_Create(NULL);
    self->velocity.y = 0;
    self->velocity.x = 0;
}

void Platform_State_MovingSpike(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 move  = Zone->timer * (self->speed << 7);

    int32 y1, y2;
    if (((move >> 16) & 1) == self->direction) {
        self->drawPos.x = self->centerPos.x + ((uint16)move * self->amplitude.x >> 6) - (self->amplitude.x << 9);
        y1                = (uint16)move * self->amplitude.y >> 6;
        y2                = self->amplitude.y << 9;
    }
    else {
        self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9) - ((uint16)move * self->amplitude.x >> 6);
        y2                = (uint16)move * self->amplitude.y >> 6;
        y1                = self->amplitude.y << 9;
    }
    self->drawPos.y = self->centerPos.y + y1 - y2;
    if (((move >> 16) & 1) != self->hasTension) {
        if (self->activeScreens)
            RSDK.PlaySfx(Platform->sfxClang, 0, 255);
        self->hasTension = (move >> 16) & 1;
    }
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Falling(void)
{
    RSDK_THIS(Platform);
    self->drawPos.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->position.x = self->centerPos.x;
        self->position.y = self->centerPos.y;
        self->velocity.x = 0;
    }
    else {
        if (self->amplitude.x) {
            RSDK.ResetEntityPtr(self, 0, 0);
        }
        else {
            self->drawPos.x  = self->centerPos.x;
            self->drawPos.y  = self->centerPos.y;
            self->velocity.y = 0;
            self->visible    = 0;
            self->state      = Platform_State_OffScreenReset;
        }
        self->velocity.x = 0;
    }
}

void Platform_State_Collapsing(void)
{
    RSDK_THIS(Platform);

    if (self->collapseDelay) {
        if (!--self->collapseDelay) {
            self->active        = ACTIVE_NORMAL;
            self->state         = Platform_State_StartFalling;
            self->collapseDelay = 30;
        }
    }

    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y;
    self->drawPos.y += RSDK.Sin256(self->stoodAngle) << 10;
    self->velocity.x = 0;
}

void Platform_State_Circular(void)
{
    RSDK_THIS(Platform);
    int32 drawX        = -self->drawPos.x;
    int32 drawY        = -self->drawPos.y;
    self->drawPos.x  = self->amplitude.x * RSDK.Cos1024(self->speed * Zone->timer + 4 * self->angle) + self->centerPos.x;
    self->drawPos.y  = self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer + 4 * self->angle) + self->centerPos.y;
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_12(void)
{
    RSDK_THIS(Platform);
    self->velocity.x = 0;
    self->velocity.y = 0;
    if (self->stood)
        self->state = Platform_Unknown9;
}

void Platform_State_13(void)
{
    RSDK_THIS(Platform);
    self->velocity.x = 0;
    self->velocity.y = 0;
    if (self->stood)
        self->state = Platform_Unknown12;
}

void Platform_State_14(void)
{
    RSDK_THIS(Platform);
    int32 oldAngle = self->angle;
    int32 drawX    = -self->drawPos.x;
    int32 drawY    = -self->drawPos.y;
    int32 angle    = self->groundVel + 0x100;
    self->angle  = self->groundVel + ((self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer) + 512) >> 14) + 0x100;
    if (!self->hasTension) {
        if (self->angle >= angle) {
            if (!self->activeScreens || oldAngle >= angle) {
                self->angle = self->groundVel + 0x100;
            }
            else {
                RSDK.PlaySfx(Platform->sfxClacker, 0, 255);
                self->angle = self->groundVel + 0x100;
            }
        }
    }
    else if (self->angle <= angle) {
        if (!self->activeScreens || oldAngle <= angle) {
            self->angle = self->groundVel + 0x100;
        }
        else {
            RSDK.PlaySfx(Platform->sfxClacker, 0, 255);
            self->angle = self->groundVel + 0x100;
        }
    }

    self->drawPos.x  = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y  = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_15(void)
{
    RSDK_THIS(Platform);
    int32 drawX        = -self->drawPos.x;
    int32 drawY        = -self->drawPos.y;
    self->drawPos.x  = self->centerPos.x;
    self->drawPos.y  = self->centerPos.y;
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_16(void)
{
    RSDK_THIS(Platform);

    if (self->stood) {
        self->drawPos.y += self->velocity.y;
        if (self->drawPos.y > self->centerPos.y + self->amplitude.y)
            self->drawPos.y = self->centerPos.y + self->amplitude.y;
    }
    else {
        self->drawPos.y -= self->velocity.y;
        if (self->drawPos.y < self->centerPos.y)
            self->drawPos.y = self->centerPos.y;
    }
}

void Platform_Unknown1(void)
{
    RSDK_THIS(Platform);
    self->drawPos.x += self->velocity.x;
    int32 x = self->position.x;
    int32 y = self->position.y;

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    if (!RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, self->hitbox.left << 16, self->hitbox.bottom << 16, 0))
        self->state = Platform_Unknown3;
    self->position.x = x;
    self->position.y = y;
}

void Platform_Unknown2(void)
{
    RSDK_THIS(Platform);
    self->drawPos.x += self->velocity.x;
    int32 x = self->position.x;
    int32 y = self->position.y;

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    if (!RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, self->hitbox.right << 16, self->hitbox.bottom << 16, 0))
        self->state = Platform_Unknown3;
    self->position.x = x;
    self->position.y = y;
}

void Platform_Unknown3(void)
{
    RSDK_THIS(Platform);
    int32 x = self->position.x;
    int32 y = self->position.y;

    self->drawPos.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    bool32 collided    = RSDK.ObjectTileGrip(self, Zone->fgLayers, 0, 0, (self->hitbox.left + 16) << 16, self->hitbox.bottom << 16, 4);
    int32 ypos         = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, 0, 0, 0, self->hitbox.bottom << 16, 4);
    if (self->position.y < ypos)
        ypos = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, 0, 0, (self->hitbox.right - 16) << 16, self->hitbox.bottom << 16, 4);
    if (self->position.y < ypos)
        ypos = self->position.y;
    if (collided) {
        self->velocity.y = 0;
        self->state      = Platform_State_Pushable;
    }
    else {
        self->position.y = ypos;
        self->drawPos.y  = ypos;
    }
    self->position.x = x;
    self->position.y = y;
    self->velocity.x = 0;
}

void Platform_Unknown4(void)
{
    RSDK_THIS(Platform);
    self->drawPos.x += self->velocity.x;
    self->drawPos.y += self->velocity.y;
    EntityPlatformNode *node = RSDK_GET_ENTITY(self->speed, PlatformNode);

    if (self->velocity.y <= 0) {
        if (self->drawPos.y < node->position.y)
            self->drawPos.y = node->position.y;
    }
    else {
        if (self->drawPos.y > node->position.y)
            self->drawPos.y = node->position.y;
    }

    if (self->velocity.x <= 0) {
        if (self->drawPos.x < node->position.x)
            self->drawPos.x = node->position.x;
    }
    else {
        if (self->drawPos.x > node->position.x)
            self->drawPos.x = node->position.x;
    }
}

void Platform_Unknown5(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    self->amplitude.y += self->groundVel;
    self->drawPos.x = (self->amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
    self->drawPos.y = (self->amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;

    if (self->groundVel <= 0) {
        self->centerPos.x = self->drawPos.x - self->tileOrigin.x;
        self->centerPos.y = self->drawPos.y - self->tileOrigin.y;
        self->groundVel   = 0;
        self->amplitude.y = 0;
        self->angle       = -self->angle;

        if (self->collapseDelay == -1) {
            self->collapseDelay = 0;
            if (self->type == PLATFORM_9)
                self->state = Platform_State_WaitBobbing;
            else
                self->state = Platform_State_Wait;
        }
        else {
            if (self->hasTension) {
                self->state = Platform_Unknown7;
            }
            else {
                self->active = ACTIVE_BOUNDS;
                if (self->type == PLATFORM_9)
                    self->state = Platform_Unknown6;
                else
                    self->state = Platform_State_Normal;
            }
        }
    }
    else {
        self->groundVel -= self->speed << 10;
    }

    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_Unknown6(void)
{
    RSDK_THIS(Platform);
    self->rotation += 4;
    self->velocity.x = 0;
    self->drawPos.y  = (RSDK.Sin1024(self->rotation) << 9) + self->centerPos.y;
    self->velocity.y = 0;
}

void Platform_Unknown7(void)
{
    RSDK_THIS(Platform);
    if (self->type == PLATFORM_9) {
        self->rotation += 4;
        self->drawPos.y = (RSDK.Sin1024(self->rotation) << 9) + self->centerPos.y;
    }
    if (self->stood) {
        self->collapseDelay = 120;
        self->velocity.x    = 0;
        self->velocity.y    = 0;
    }
    else {
        if (--self->collapseDelay) {
            self->tileOrigin.x  = self->drawPos.x - self->centerPos.x;
            self->tileOrigin.y  = self->drawPos.y - self->centerPos.y;
            self->centerPos.x   = self->drawPos.x;
            self->collapseDelay = -1;
            self->centerPos.y   = self->drawPos.y;
            self->active        = ACTIVE_NORMAL;
            self->state         = Platform_State_PlayerMove;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_Unknown8(void)
{
    RSDK_THIS(Platform);
    if (!self->stood) {
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
    else {
        int32 slot                        = SceneInfo->entitySlot - 1;
        EntityPlatformControl *ControllerInfo = RSDK_GET_ENTITY(slot, PlatformControl);
        if (ControllerInfo->objectID == PlatformControl->objectID) {
            ControllerInfo->setActive = true;
            self->state         = Platform_Unknown4;
            self->velocity.x    = 0;
            self->velocity.y    = 0;
        }
        else {
            while (ControllerInfo->objectID == Platform->objectID || ControllerInfo->objectID == PlatformNode->objectID) {
                ControllerInfo = RSDK_GET_ENTITY(slot--, PlatformControl);
                if (ControllerInfo->objectID == PlatformControl->objectID) {
                    ControllerInfo->setActive = true;
                    self->state         = Platform_Unknown4;
                    self->velocity.x    = 0;
                    self->velocity.y    = 0;
                    break;
                }
            }
        }
    }
}

void Platform_Unknown9(void)
{
    RSDK_THIS(Platform);
    int32 drawX   = -self->drawPos.x;
    int32 drawY   = -self->drawPos.y;
    self->angle = self->groundVel + 0x100 + ((self->amplitude.x * RSDK.Sin1024(self->collapseDelay * self->speed) + 0x200) >> 14);

    if (self->collapseDelay == 256 && self->hasTension) {
        self->collapseDelay = 119;
        self->state         = Platform_Unknown10;
    }
    ++self->collapseDelay;
    self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;

    if (self->groundVel < 0)
        self->drawPos.x += 0x200000;
    else
        self->drawPos.x -= 0x200000;
    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_Unknown10(void)
{
    RSDK_THIS(Platform);
    if (self->stood) {
        self->collapseDelay = 120;
        self->velocity.x    = 0;
        self->velocity.y    = 0;
    }
    else {
        if (!--self->collapseDelay) {
            self->collapseDelay = 257;
            self->active        = ACTIVE_NORMAL;
            self->state         = Platform_Unknown11;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_Unknown11(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 amp   = self->amplitude.x * RSDK.Sin1024(self->collapseDelay * self->speed);
    --self->collapseDelay;
    self->angle = self->groundVel + 0x100 + ((amp + 0x200) >> 14);

    if (self->collapseDelay == 88)
        self->state = Platform_State_12;

    self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
    if (self->groundVel < 0)
        self->drawPos.x += 0x200000;
    else
        self->drawPos.x -= 0x200000;
    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_Unknown12(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 speed = self->angle * (self->speed << 7);
    self->angle++;

    if (self->hasTension) {
        if (speed < 0x10000) {
            if (speed <= 0)
                speed = 0;
        }
        else {
            speed                 = 0x10000;
            self->collapseDelay = 120;
            self->state         = Platform_Unknown13;
        }
    }

    int32 y1, y2;
    if (((speed >> 0x10) & 1) == self->direction) {
        self->drawPos.x = self->centerPos.x + ((uint16)speed * self->amplitude.x >> 6) - (self->amplitude.x << 9);
        y1                = (uint16)speed * self->amplitude.y >> 6;
        y2                = self->amplitude.y << 9;
    }
    else {
        self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9) - ((uint16)speed * self->amplitude.x >> 6);
        y2                = (uint16)speed * self->amplitude.y >> 6;
        y1                = self->amplitude.y << 9;
    }
    self->velocity.x = drawX + self->drawPos.x;
    self->drawPos.y  = self->centerPos.y + y1 - y2;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_Unknown13(void)
{
    RSDK_THIS(Platform);
    if (self->stoodPlayers) {
        self->collapseDelay = 120;
        self->velocity.x    = 0;
        self->velocity.y    = 0;
    }
    else {
        if (!--self->collapseDelay) {
            self->active = ACTIVE_NORMAL;
            self->state  = Platform_Unknown14;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_Unknown14(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 speed = self->angle * (self->speed << 7);
    self->angle--;

    if (speed > 0) {
        if (speed >= 0x10000)
            speed = 0x10000;
    }
    else {
        speed         = 0;
        self->state = Platform_State_13;
    }

    int32 y1, y2;
    if (((speed >> 0x10) & 1) == self->direction) {
        self->drawPos.x = self->centerPos.x + ((uint16)speed * self->amplitude.x >> 6) - (self->amplitude.x << 9);
        y1                = (uint16)speed * self->amplitude.y >> 6;
        y2                = self->amplitude.y << 9;
    }
    else {
        self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9) - ((uint16)speed * self->amplitude.x >> 6);
        y2                = (uint16)speed * self->amplitude.y >> 6;
        y1                = self->amplitude.y << 9;
    }
    self->velocity.x = drawX + self->drawPos.x;
    self->drawPos.y  = self->centerPos.y + y1 - y2;
    self->velocity.y = drawY + self->drawPos.y;
}

// Collision States
void Platform_CollisionState_AllSolid(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;
    self->stoodPlayers   = 0;
    self->pushPlayersL   = 0;
    self->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        int32 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);
        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP:
                self->stood = true;
                if (!((1 << pid) & stoodPlayers) && !player->sidekick && self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->collapseDelay = 1;
                    else
#endif
                        self->collapseDelay = 30;
                }
                self->stoodPlayers |= 1 << pid;

                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    self->pushPlayersL |= 1 << pid;
                }

                if (self->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left) {
                    self->pushPlayersR |= 1 << pid;
                }

                if (self->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (self->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_AllHazard(void)
{
    RSDK_THIS(Platform);
    if (self->collapseDelay)
        self->collapseDelay--;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox)
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(1024, player, self->type == PLATFORM_3, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, self);
        }
    }
}
void Platform_CollisionState_BottomHazard(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    self->stoodPlayers   = 0;
    self->pushPlayersL   = 0;
    self->pushPlayersR   = 0;

    foreach_active(Player, player)
    {
        int32 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);
        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP:
                self->stood = true;
                self->stoodPlayers |= (1 << pid);
                if (!player->sidekick && self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->collapseDelay = 1;
                    else
#endif
                        self->collapseDelay = 30;
                }
                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    self->pushPlayersL |= 1 << pid;
                }

                if (self->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left) {
                    self->pushPlayersR |= 1 << pid;
                }

                if (self->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (self->velocity.y >= 0)
                    player->collisionFlagV |= 2;
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(1024, player, 0, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, self);
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_LRHazard(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox  = RSDK.GetHitbox(&self->animator, 1);
    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP:
                self->stood = true;
                self->stoodPlayers |= (1 << pid);
                if (!player->sidekick && self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->collapseDelay = 1;
                    else
#endif
                        self->collapseDelay = 30;
                }
                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT:
                if (
#if RETRO_USE_PLUS
                    Player_CheckMightyUnspin(1024, player, 0, &player->uncurlTimer) ||
#endif
                    Player_CheckHit(player, self)) {
                    player->velocity.x += self->velocity.x;
                }

                if (!player->blinkTimer || player->velocity.x >= self->velocity.x) {
                    if (player->onGround && player->right) {
                        self->pushPlayersL |= 1 << pid;
                    }

                    if (self->velocity.x < 0 && player->velocity.x >= self->velocity.x && player->left) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                }
                if (self->velocity.x <= 0)
                    player->collisionFlagH |= 1;
                break;
            case C_RIGHT:
                if (
#if RETRO_USE_PLUS
                    Player_CheckMightyUnspin(1024, player, 0, &player->uncurlTimer) ||
#endif
                    Player_CheckHit(player, self)) {
                    player->velocity.x += self->velocity.x;
                }

                if (!player->blinkTimer || player->velocity.x <= self->velocity.x) {
                    if (player->onGround && player->left) {
                        self->pushPlayersR |= 1 << pid;
                    }

                    if (self->velocity.x > 0 && player->velocity.x <= self->velocity.x && player->right) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                }
                if (self->velocity.x >= 0)
                    player->collisionFlagH |= 2;
                break;
            case C_BOTTOM:
                if (self->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: continue;
        }
    }
}
void Platform_CollisionState_Tiles(void)
{
    RSDK_THIS(Platform);
    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        int32 pid = RSDK.GetEntityID(player);

        Hitbox hitbox;
        hitbox.left   = self->hitbox.left;
        hitbox.top    = self->hitbox.top;
        hitbox.right  = self->hitbox.right;
        hitbox.bottom = self->hitbox.bottom;
        hitbox.left -= 16;
        hitbox.right += 16;
        hitbox.bottom += 16;
        hitbox.top -= 16;

        if (Player_CheckCollisionTouch(player, self, &hitbox)) {
            player->collisionLayers |= Zone->moveID;
            player->moveOffset.x = self->tileOrigin.x - self->drawPos.x;
            player->moveOffset.y = self->tileOrigin.y - self->drawPos.y;
            if (player->animator.animationID == ANI_PUSH && player->onGround) {
                if (player->right)
                    self->pushPlayersL |= 1 << pid;
                if (player->onGround && player->left)
                    self->pushPlayersR |= 1 << pid;
            }

            bool32 climbFlag = false;
            if (player->state == Player_State_KnuxWallClimb || player->state == Player_State_KnuxLedgePullUp) {
                climbFlag = true;
                if (player->state == Player_State_KnuxLedgePullUp)
                    hitbox.top -= 16;
                if (player->position.x >= self->position.x) {
                    hitbox.top += 16;
                    hitbox.left += 16;
                }
                else {
                    hitbox.top += 16;
                    hitbox.right -= 16;
                }
                hitbox.bottom -= 16;
            }
            else {
                switch (player->collisionMode) {
                    case CMODE_FLOOR:
                        hitbox.right -= 16;
                        hitbox.left += 16;
                        hitbox.bottom -= 16;
                        break;
                    case CMODE_LWALL:
                        hitbox.top += 16;
                        hitbox.right -= 16;
                        hitbox.bottom -= 16;
                        break;
                    case CMODE_ROOF:
                        hitbox.top += 16;
                        hitbox.left += 16;
                        hitbox.right -= 16;
                        break;
                    case CMODE_RWALL:
                        hitbox.top += 16;
                        hitbox.left += 16;
                        hitbox.bottom -= 16;
                        break;
                    default: break;
                }
            }

            if (Player_CheckCollisionTouch(player, self, &hitbox) && (player->onGround || climbFlag)) {
                self->stoodPlayers |= 1 << pid;
                if (!player->sidekick) {
                    self->stood = true;
                    if (self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            self->collapseDelay = 1;
                        else
#endif
                            self->collapseDelay = 30;
                    }
                }
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }
    }
}
void Platform_CollisionState_Sticky(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    self->pushPlayersL   = 0;
    self->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);
        int32 side = Player_CheckCollisionBox(player, self, solidHitbox);

        bool32 flag = false;
        if ((self->collision != PLATFORM_C_9 && self->collision != side + PLATFORM_C_9) || !side)
            flag = true;
        if (player->state != Player_State_None && !flag) {
            player->state                         = Player_State_None;
            player->nextGroundState               = StateMachine_None;
            player->nextAirState                  = StateMachine_None;
            player->velocity.x                    = 0;
            player->velocity.y                    = 0;
            player->groundVel                     = 0;
            player->animator.animationSpeed = 0;
            switch (side) {
                case C_TOP: player->angle = 0x00; break;
                case C_LEFT:
                    player->angle    = 0xC0;
                    player->onGround = false;
                    break;
                case C_RIGHT: player->angle = 0x40; break;
                case C_BOTTOM: player->angle = 0x80; break;
                default: break;
            }

            player->tileCollisions = false;
            if (!player->sidekick) {
                self->stood = true;
                if (self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->collapseDelay = 1;
                    else
#endif
                        self->collapseDelay = 30;
                }
            }
            self->stoodPlayers |= 1 << pid;
        }
        else {
            if ((1 << pid) & self->stoodPlayers) {
                if (player->state == Player_State_None) {
                    if (Platform->stoodPos[pid].x) {
                        player->position.x = Platform->stoodPos[pid].x;
                        player->position.y = Platform->stoodPos[pid].y;
                    }
                    else {
                        Platform->stoodPos[pid].x = player->position.x;
                        Platform->stoodPos[pid].y = player->position.y;
                    }
                    player->position.x += self->collisionOffset.x;
                    player->position.y += self->collisionOffset.y;
                    player->position.y &= 0xFFFF0000;
                    if (player->jumpPress) {
                        player->tileCollisions = true;
                        Player_StartJump(player);
                    }
                }
                else {
                    self->stoodPlayers = self->stoodPlayers & ~(1 << pid);
                }
            }
            else if (side == C_TOP) {
                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }
    }
}
void Platform_CollisionState_TopHazard(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox  = RSDK.GetHitbox(&self->animator, 1);
    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP:
                self->stood = true;
                self->stoodPlayers |= (1 << pid);
                if (!player->sidekick && self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->collapseDelay = 1;
                    else
#endif
                        self->collapseDelay = 30;
                }

                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;

#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(1024, player, 0, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, self);
                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT:
                if (player->onGround && player->right)
                    self->pushPlayersL |= 1 << pid;

                if (self->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left)
                    self->pushPlayersR |= 1 << pid;

                if (self->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (self->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_TopSolid(void)
{
    RSDK_THIS(Platform);
    int32 stoodPlayers     = self->stoodPlayers;
    self->stoodPlayers   = 0;
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        int32 yVel = player->velocity.y;
        if (self->collisionOffset.y < 0)
            player->velocity.y -= self->collisionOffset.y;
        if (Player_CheckCollisionPlatform(player, self, platformHitbox)) {
            self->stood = true;
            if (!((1 << pid) & stoodPlayers) && !player->sidekick && self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                    self->collapseDelay = 1;
                else
#endif
                    self->collapseDelay = 30;
            }
            self->stoodPlayers |= 1 << pid;

            if (Platform->stoodPos[pid].x) {
                player->position.x = Platform->stoodPos[pid].x;
                player->position.y = Platform->stoodPos[pid].y;
            }
            else {
                Platform->stoodPos[pid].x = player->position.x;
                Platform->stoodPos[pid].y = player->position.y;
            }
            player->position.x += self->collisionOffset.x;
            player->position.y += self->collisionOffset.y;
            player->position.y &= 0xFFFF0000;
        }
        else {
            player->velocity.y = yVel;
        }
    }
}
void Platform_CollisionState_TurnTable(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;
    self->stoodPlayers   = 0;
    self->pushPlayersL   = 0;
    self->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);
        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP: self->stood = true;
#if RETRO_USE_PLUS
                if (player->characterID != ID_MIGHTY || player->state != Player_State_MightyHammerDrop) {
#endif
                    if (!((1 << pid) & stoodPlayers)) {
                        player->state           = Player_State_None;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        player->groundVel       = 0;
                        if (self->objectID == Platform->objectID)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TURNTABLE, &player->animator, false, 0);
                        player->animator.animationSpeed = 64;
                        player->direction                     = FLIP_NONE;
                        if (!player->sidekick && self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                            if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                                self->collapseDelay = 1;
                            else
#endif
                                self->collapseDelay = 30;
                        }
                    }
                    self->stoodPlayers |= 1 << pid;
                    if (Platform->stoodPos[pid].x) {
                        player->position.x = Platform->stoodPos[pid].x;
                        player->position.y = Platform->stoodPos[pid].y;
                    }
                    else {
                        Platform->stoodPos[pid].x = player->position.x;
                        Platform->stoodPos[pid].y = player->position.y;
                    }

                    if ((player->position.x ^ self->position.x) & 0xFFFF0000) {
                        if (player->position.x >= self->position.x)
                            player->position.x -= 0x10000;
                        else
                            player->position.x += 0x10000;
                    }
                    player->position.x += self->collisionOffset.x;
                    player->position.y += self->collisionOffset.y;
                    player->position.y &= 0xFFFF0000;
                    if (player->jumpPress) {
                        Player_StartJump(player);
                    }
                    else if (self->velocity.y <= 0) {
                        player->collisionFlagV |= 1;
                    }
#if RETRO_USE_PLUS
                }
#endif
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    self->pushPlayersL |= 1 << pid;
                }

                if (self->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left) {
                    self->pushPlayersR |= 1 << pid;
                }

                if (self->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (self->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_Twister(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;
    self->stoodPlayers   = 0;
    self->pushPlayersL   = 0;
    self->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);
        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_NONE:
                if ((1 << pid) & stoodPlayers) {
                    if (player->state == Player_State_None) {
                        player->state = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_AIRWALK, &player->animator, false, 0);
                    }
                }
                break;
            case C_TOP:
#if RETRO_USE_PLUS
                if (player->characterID != ID_MIGHTY || player->state != Player_State_MightyHammerDrop) {
#endif
                    if (!((1 << pid) & stoodPlayers)) {
                        player->state           = Player_State_None;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        player->groundVel       = 0;
                        if (self->objectID == Platform->objectID)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTER, &player->animator, false, 0);
                        player->animator.animationSpeed = 64;
                        player->direction                     = FLIP_X;
                        if (!player->sidekick) {
                            self->stood = true;
                            if (self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                                    self->collapseDelay = 1;
                                else
#endif
                                    self->collapseDelay = 30;
                            }
                        }
                    }
                    self->stoodPlayers |= 1 << pid;
                    if (Platform->stoodPos[pid].x) {
                        player->position.x = Platform->stoodPos[pid].x;
                        player->position.y = Platform->stoodPos[pid].y;
                    }
                    else {
                        Platform->stoodPos[pid].x = player->position.x;
                        Platform->stoodPos[pid].y = player->position.y;
                    }

                    if ((player->position.x ^ self->position.x) & 0xFFFF0000) {
                        if (player->position.x >= self->position.x)
                            player->position.x -= 0x10000;
                        else
                            player->position.x += 0x10000;
                    }
                    player->position.x += self->collisionOffset.x;
                    player->position.y += self->collisionOffset.y;
                    player->position.y &= 0xFFFF0000;
                    if (player->jumpPress) {
                        Player_StartJump(player);
                    }
                    else if (self->velocity.y <= 0) {
                        player->collisionFlagV |= 1;
                    }
#if RETRO_USE_PLUS
                }
#endif
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    self->pushPlayersL |= 1 << pid;
                }

                if (self->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left) {
                    self->pushPlayersR |= 1 << pid;
                }

                if (self->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (self->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_None(void) {}
void Platform_CollisionState_AllSolid_NoCrush(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;
    self->stoodPlayers   = 0;
    self->pushPlayersL   = 0;
    self->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);

        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            default: break;
            case C_TOP:
                self->stood = true;
                if (!((1 << pid) & stoodPlayers) && !player->sidekick && self->state == Platform_State_Collapsing && !self->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->collapseDelay = 1;
                    else
#endif
                        self->collapseDelay = 30;
                }
                self->stoodPlayers |= 1 << pid;
                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    self->pushPlayersL |= 1 << pid;
                }

                if (self->velocity.x <= 0 && player->left) {
                    player->groundVel  = self->velocity.x;
                    player->velocity.x = player->groundVel;
                    player->velocity.x -= player->acceleration;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left)
                    self->pushPlayersR |= 1 << pid;

                if (self->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = self->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                }
                break;
            case C_BOTTOM: break;
        }
    }
}

void Platform_EditorDraw(void)
{
    RSDK_THIS(Platform);
    self->drawPos = self->position;
    self->centerPos = self->position;

    if (self->frameID >= 0) {
        int32 f    = self->frameID;
        int32 anim = 0;
        while (f >= self->animator.frameCount) {
            if (!self->animator.frameCount)
                break;
            f -= self->animator.frameCount;
            RSDK.SetSpriteAnimation(Platform->aniFrames, ++anim, &self->animator, true, 0);
        }
        self->frameID          = f;
        self->animator.frameID = f;
    }
    else {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator, true, 0);
    }

    if (self->frameID >= 0) {
        switch (self->type) {
            case PLATFORM_FIXED:
            default:
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;

                RSDK.DrawSprite(&self->animator, &self->drawPos, false);
                break;
            case PLATFORM_COLLAPSING:
                self->updateRange.x = 0x800000;
                self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;

                RSDK.DrawSprite(&self->animator, &self->drawPos, false);
                break;
            case PLATFORM_MOVING:
                self->updateRange.x = (abs(self->amplitude.x) + 0x2000) << 10;
                self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;

                RSDK.DrawSprite(&self->animator, &self->drawPos, false);

                if (showGizmos()) {
                    self->inkEffect = INK_BLEND;

                    // start pos
                    self->drawPos.x = self->amplitude.x * RSDK.Sin1024(self->angle) + self->centerPos.x;
                    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

                    Vector2 drawPos;
                    // right max
                    drawPos.x = self->amplitude.x * RSDK.Sin1024(0x100) + self->centerPos.x;
                    drawPos.y = self->amplitude.y * RSDK.Sin1024(0x100) + self->centerPos.y;
                    RSDK.DrawSprite(&self->animator, &drawPos, false);

                    // left max
                    self->drawPos.x = self->amplitude.x * RSDK.Sin1024(0x300) + self->centerPos.x;
                    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(0x300) + self->centerPos.y;
                    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

                    RSDK.DrawLine(drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, 0, INK_NONE, false);

                    self->inkEffect = INK_NONE;
                }
                break;
            case PLATFORM_3:
                self->updateRange.x = (abs(self->amplitude.x) + 0x2000) << 10;
                self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;

                if (showGizmos()) {
                    self->inkEffect = INK_BLEND;

                    // start pos
                    self->drawPos.x = self->amplitude.x * RSDK.Cos1024(4 * self->angle) + self->centerPos.x;
                    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(4 * self->angle) + self->centerPos.y;
                    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

                    self->inkEffect = INK_NONE;
                }
                break;
            case PLATFORM_5:
            case PLATFORM_11:
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                break;
            case PLATFORM_6:
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                break;
            case PLATFORM_8:
                // self->amplitude.x <<= 10;
                self->updateRange.x = 0x800000 + abs(self->amplitude.x);
                self->updateRange.y = 0x800000 + abs(self->amplitude.x);
                if (self->speed < 0)
                    self->direction = FLIP_X;
                break;
            case PLATFORM_9:
                // self->amplitude.x <<= 10;
                self->updateRange.x = 0x800000 + abs(self->amplitude.x);
                self->updateRange.y = 0x800000 + abs(self->amplitude.x);
                if (self->speed < 0)
                    self->direction = FLIP_X;
                break;
            case PLATFORM_10:
                // self->amplitude.x <<= 10;
                self->updateRange.x = 0x800000 + abs(self->amplitude.x);
                self->updateRange.y = 0x800000 + abs(self->amplitude.x);

                if (self->speed < 0) {
                    // self->direction = FLIP_X;
                    // self->speed     = -self->speed;
                }
                break;
            case PLATFORM_12:
            case PLATFORM_4:
                self->updateRange.x = (abs(self->amplitude.y) + 0x200) << 14;
                self->updateRange.y = (abs(self->amplitude.y) + 0x200) << 14;
                RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
                // self->amplitude.y *= 16;
                // self->groundVel = 4 * self->angle;
                // self->angle     = self->groundVel + 256 + (self->amplitude.x * RSDK.Sin1024(self->speed * self->collapseDelay) >> 14);
                self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
                self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                if (self->type != PLATFORM_4) {
                    if (self->groundVel >= 0)
                        self->drawPos.x -= 0x200000;
                    else
                        self->drawPos.x += 0x200000;
                }
                break;
            case PLATFORM_13:
                if (self->direction) {
                    self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9);
                    self->drawPos.y = self->centerPos.y + (self->amplitude.y << 9);
                }
                else {
                    self->drawPos.x = self->centerPos.x - (self->amplitude.x << 9);
                    self->drawPos.y = self->centerPos.y - (self->amplitude.y << 9);
                }
            case PLATFORM_7:
                self->updateRange.x = (abs(self->amplitude.x) + 0x4000) << 9;
                self->updateRange.y = (abs(self->amplitude.y) + 0x4000) << 9;
                if (self->speed < 0)
                    self->direction = FLIP_X;
                break;
            case PLATFORM_14:
                self->updateRange.x = (abs(self->amplitude.y) + 0x200) << 14;
                self->updateRange.y = (abs(self->amplitude.y) + 0x200) << 14;
                RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
                // self->amplitude.y *= 16;
                // self->groundVel = 4 * self->angle;
                // self->angle     = self->groundVel + 256 + (self->amplitude.x * RSDK.Sin1024(self->speed * self->collapseDelay) >> 14);
                self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
                self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                break;
            case PLATFORM_15:
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                break;
            case PLATFORM_16:
                self->updateRange.x = 0x800000;
                self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
                break;
        }
    }
}

void Platform_EditorLoad(void)
{
    Platform_StageLoad();

    RSDK_ACTIVE_VAR(Platform, type);
    RSDK_ENUM_VAR("Type 0", PLATFORM_FIXED);
    RSDK_ENUM_VAR("Type 1", PLATFORM_COLLAPSING);
    RSDK_ENUM_VAR("Type 2", PLATFORM_MOVING);
    RSDK_ENUM_VAR("Type 3", PLATFORM_3);
    RSDK_ENUM_VAR("Type 4", PLATFORM_4);
    RSDK_ENUM_VAR("Type 5", PLATFORM_5);
    RSDK_ENUM_VAR("Type 6", PLATFORM_6);
    RSDK_ENUM_VAR("Type 7", PLATFORM_7);
    RSDK_ENUM_VAR("Type 8", PLATFORM_8);
    RSDK_ENUM_VAR("Type 9", PLATFORM_9);
    RSDK_ENUM_VAR("Type 10", PLATFORM_10);
    RSDK_ENUM_VAR("Type 11", PLATFORM_11);
    RSDK_ENUM_VAR("Type 12", PLATFORM_12);
    RSDK_ENUM_VAR("Type 13", PLATFORM_13);
    RSDK_ENUM_VAR("Type 14", PLATFORM_14);
    RSDK_ENUM_VAR("Type 15", PLATFORM_15);
    RSDK_ENUM_VAR("Type 16", PLATFORM_16);

    RSDK_ACTIVE_VAR(Platform, collision);
    RSDK_ENUM_VAR("Type 0", PLATFORM_C_0);
    RSDK_ENUM_VAR("Type 1", PLATFORM_C_1);
    RSDK_ENUM_VAR("Type 2", PLATFORM_C_2);
    RSDK_ENUM_VAR("Type 3", PLATFORM_C_3);
    RSDK_ENUM_VAR("Type 4", PLATFORM_C_4);
    RSDK_ENUM_VAR("Type 5", PLATFORM_C_5);
    RSDK_ENUM_VAR("Type 6", PLATFORM_C_6);
    RSDK_ENUM_VAR("Type 7", PLATFORM_C_7);
    RSDK_ENUM_VAR("Type 8", PLATFORM_C_8);
    RSDK_ENUM_VAR("Type 9", PLATFORM_C_9);
    RSDK_ENUM_VAR("Type 10", PLATFORM_C_10);
    RSDK_ENUM_VAR("Type 11", PLATFORM_C_11);
    RSDK_ENUM_VAR("Type 12", PLATFORM_C_12);
    RSDK_ENUM_VAR("Type 13", PLATFORM_C_13);
    RSDK_ENUM_VAR("Type 14", PLATFORM_C_14);
    RSDK_ENUM_VAR("Type 15", PLATFORM_C_15);
}

void Platform_Serialize(void)
{
    RSDK_EDITABLE_VAR(Platform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Platform, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(Platform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Platform, VAR_BOOL, hasTension);
    RSDK_EDITABLE_VAR(Platform, VAR_INT8, frameID);
    RSDK_EDITABLE_VAR(Platform, VAR_UINT8, collision);
    RSDK_EDITABLE_VAR(Platform, VAR_VECTOR2, tileOrigin);
    RSDK_EDITABLE_VAR(Platform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(Platform, VAR_INT32, angle);
}
