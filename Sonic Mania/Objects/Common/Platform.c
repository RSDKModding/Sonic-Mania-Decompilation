// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Platform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPlatform *Platform;

void Platform_Update(void)
{
    RSDK_THIS(Platform);
#if RETRO_USE_PLUS
    if (Zone->teleportActionActive) {
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
        if (self->state != Platform_State_Collapse_Falling && self->state != Platform_State_Collapse_CheckReset) {
            StateMachine_Run(self->stateCollide);
        }

        self->position.x = self->centerPos.x;
        self->position.y = self->centerPos.y;
        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK.GetEntityByID(s++);
            if (child->objectID == ItemBox->objectID) {
                if (!child->scale.y) {
                    EntityItemBox *itemBox = (EntityItemBox *)child;
#if RETRO_USE_PLUS
                    itemBox->parent = (Entity *)self;
#else
                    itemBox->groundVel = RSDK.GetEntityID(self);
#endif
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
                    EntityIce *ice        = (EntityIce *)child;
                    ice->playerMoveOffset = self->collisionOffset;
                }
                else if (TurboTurtle && child->objectID == TurboTurtle->objectID) {
                    EntityTurboTurtle *turboTurtle = (EntityTurboTurtle *)child;
                    turboTurtle->startPos.x += self->collisionOffset.x;
                    turboTurtle->startPos.y += self->collisionOffset.y;
                }
            }
        }
        if (self->animator.frameDuration)
            RSDK.ProcessAnimation(&self->animator);
    }
}

void Platform_LateUpdate(void) {}

void Platform_StaticUpdate(void)
{
    for (int32 i = 0; i < PLAYER_MAX; ++i) {
        Platform->stoodPos[i].x = 0;
        Platform->stoodPos[i].y = 0;
    }
}

void Platform_Draw(void)
{
    RSDK_THIS(Platform);
    Vector2 drawPos;

    if (self->frameID >= 0) {

        if ((self->state == Platform_State_Circular && self->hasTension)
            || (self->state == Platform_State_Swing || self->state == Platform_State_Swing_Clack || self->type == PLATFORM_WAIT_ARC)) {
            int32 ang = 0;
            if (self->state == Platform_State_Circular && self->hasTension)
                ang = self->speed * Zone->timer + 4 * self->angle;
            else
                ang = self->angle;
            int32 fxStore = self->drawFX;
            self->drawFX |= FX_FLIP | FX_ROTATE;
            int32 cnt              = (self->amplitude.y >> 10) - 1;
            int32 angle            = 0x400;
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
        else {
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
        self->active      = ACTIVE_BOUNDS;
        self->visible     = true;
        self->drawOrder   = Zone->drawOrderLow + 1;
        self->centerPos.x = self->position.x;
        self->centerPos.y = self->position.y;
        self->drawPos.x   = self->position.x;
        self->drawPos.y   = self->position.y;
        RSDK.SetSpriteAnimation(Platform->aniFrames, 0, &self->animator, true, 0);
        self->animator.frameID = self->frameID;
        if (!SceneInfo->inEditor && RSDK.GetFrameID(&self->animator) == 'l')
            self->drawOrder = Zone->drawOrderLow;

        switch (self->type) {
            case PLATFORM_FIXED:
            default:
                self->state         = Platform_State_Fixed;
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                break;
            case PLATFORM_COLLAPSING:
                self->state         = Platform_State_Collapse;
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
            case PLATFORM_CIRCULAR:
                self->updateRange.x = (abs(self->amplitude.x) + 0x2000) << 10;
                self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
                self->state         = Platform_State_Circular;
                break;
            case PLATFORM_CONTROLLED:
            case PLATFORM_CONT_ACTIVATER:
                self->active = ACTIVE_BOUNDS;
                if (self->type == PLATFORM_CONTROLLED)
                    self->state = Platform_State_WaitForControl;
                else
                    self->state = Platform_State_ActivateControlOnStood;
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                break;
            case PLATFORM_PUSHABLE:
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                if (!SceneInfo->inEditor) {
                    self->speed <<= 11;
                    self->position.x += 0x8000;
                }
                self->state = Platform_State_StartPush;
                break;
            case PLATFORM_WAIT:
                self->amplitude.x <<= 10;
                self->updateRange.x = 0x800000 + abs(self->amplitude.x);
                self->updateRange.y = 0x800000 + abs(self->amplitude.x);
                if (self->speed < 0)
                    self->direction = FLIP_X;
                self->state = Platform_State_Wait;
                break;
            case PLATFORM_WAIT_OSC:
                self->amplitude.x <<= 10;
                self->updateRange.x = 0x800000 + abs(self->amplitude.x);
                self->updateRange.y = 0x800000 + abs(self->amplitude.x);
                if (self->speed < 0)
                    self->direction = FLIP_X;
                self->state = Platform_State_WaitOscillating;
                break;
            case PLATFORM_ACTIVEABOVE:
                self->amplitude.x <<= 10;
                self->updateRange.x = 0x800000 + abs(self->amplitude.x);
                self->updateRange.y = 0x800000 + abs(self->amplitude.x);

                if (self->speed < 0) {
                    self->direction = FLIP_X;
                    self->speed     = -self->speed;
                }
                self->state = Platform_State_ActiveFromAbove;
                break;
            case PLATFORM_WAIT_ARC: self->timer = 88;
            case PLATFORM_SWINGING:
                self->updateRange.x = (abs(self->amplitude.y) + 512) << 14;
                self->updateRange.y = (abs(self->amplitude.y) + 512) << 14;
                RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
                self->amplitude.y <<= 4;
                self->groundVel = 4 * self->angle;
                self->angle     = self->groundVel + 256 + (self->amplitude.x * RSDK.Sin1024(self->speed * self->timer) >> 14);
                self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
                self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                if (self->type == PLATFORM_SWINGING) {
                    self->state = Platform_State_Swing;
                }
                else {
                    if (self->groundVel >= 0)
                        self->drawPos.x -= 0x200000;
                    else
                        self->drawPos.x += 0x200000;
                    self->state = Platform_State_SwingMove_CheckStood;
                }
                break;
            case PLATFORM_STICKY:
                if (self->direction) {
                    self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9);
                    self->drawPos.y = self->centerPos.y + (self->amplitude.y << 9);
                }
                else {
                    self->drawPos.x = self->centerPos.x - (self->amplitude.x << 9);
                    self->drawPos.y = self->centerPos.y - (self->amplitude.y << 9);
                }
            case PLATFORM_MOVING_STATIC:
                self->updateRange.x = (abs(self->amplitude.x) + 0x4000) << 9;
                self->updateRange.y = (abs(self->amplitude.y) + 0x4000) << 9;
                if (self->speed < 0)
                    self->direction = FLIP_X;

                if (self->type == PLATFORM_MOVING_STATIC)
                    self->state = Platform_State_Moving_Static;
                else
                    self->state = Platform_State_Sticky_CheckRide;
                break;
            case PLATFORM_SWING_CLACK:
                self->updateRange.x = (abs(self->amplitude.y) + 0x200) << 14;
                self->updateRange.y = (abs(self->amplitude.y) + 0x200) << 14;
                RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
                self->amplitude.y <<= 4;
                self->groundVel = 4 * self->angle;
                self->angle     = self->groundVel + 0x100 + (self->amplitude.x * RSDK.Sin1024(self->speed * self->timer) >> 14);
                self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
                self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                self->state     = Platform_State_Swing_Clack;
                break;
            case PLATFORM_STATIC:
                self->state         = Platform_State_Static;
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                break;
            case PLATFORM_SINKER:
                self->updateRange.x = 0x800000;
                self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
                if (!self->speed)
                    self->speed = 1;
                self->amplitude.y = self->amplitude.y << 10;
                self->velocity.y  = self->speed << 16;
                self->state       = Platform_State_Sinker;
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
            if (self->collision != PLATFORM_C_SOLID_NONE) {
                Hitbox *hitbox = RSDK.GetHitbox(&self->animator, self->collision != PLATFORM_C_SOLID_TOP);
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
                case PLATFORM_C_SOLID_TOP:
                    self->stateCollide  = Platform_CollisionState_TopSolid;
                    self->hitbox.bottom = self->hitbox.top + 8;
                    break;
                case PLATFORM_C_SOLID_ALL:
                default: self->stateCollide = Platform_CollisionState_AllSolid; break;
                case PLATFORM_C_USE_TILES: self->stateCollide = Platform_CollisionState_Tiles; break;
                case PLATFORM_C_HAZARD_ALL: self->stateCollide = Platform_CollisionState_AllHazard; break;
                case PLATFORM_C_SOLID_NONE: self->stateCollide = Platform_CollisionState_None; break;
                case PLATFORM_C_HAZARD_SIDES: self->stateCollide = Platform_CollisionState_LRHazard; break;
                case PLATFORM_C_HAZARD_BOTTOM: self->stateCollide = Platform_CollisionState_BottomHazard; break;
                case PLATFORM_C_HAZARD_TOP: self->stateCollide = Platform_CollisionState_TopHazard; break;
                case PLATFORM_C_TWISTER: self->stateCollide = Platform_CollisionState_Twister; break;
                case PLATFORM_C_STICKY_ALL:
                case PLATFORM_C_STICKY_TOP:
                case PLATFORM_C_STICKY_LEFT:
                case PLATFORM_C_STICKY_RIGHT:
                case PLATFORM_C_STICKY_BOTTOM: self->stateCollide = Platform_CollisionState_Sticky; break;
                case PLATFORM_C_TURNTABLE: self->stateCollide = Platform_CollisionState_TurnTable; break;
                case PLATFORM_C_SOLID_ALL_NOCRUSH: self->stateCollide = Platform_CollisionState_AllSolid_NoCrush; break;
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
        Platform->sfxClack   = RSDK.GetSfx("Stage/Clack2.wav");
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
        Platform->sfxClack   = RSDK.GetSfx("Stage/Clack2.wav");
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

    Platform->sfxClacker = RSDK.GetSfx("Stage/Clacker.wav");
    Platform->sfxClang   = RSDK.GetSfx("Stage/Clang.wav");
    Platform->sfxPush    = RSDK.GetSfx("Global/Push.wav");
    if (Platform->playingPushSFX) {
        RSDK.StopSfx(Platform->sfxPush);
        Platform->playingPushSFX = 0;
    }
}

void Platform_State_WaitForControl(void)
{
    RSDK_THIS(Platform);
    self->velocity.x = 0;
    self->velocity.y = 0;
}

void Platform_State_Fixed(void)
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
            self->timer = 120;
        self->rotation     = 0;
        self->tileOrigin.x = 0;
        self->tileOrigin.y = 0;
        self->active       = ACTIVE_NORMAL;
        self->state        = Platform_State_PlayerMove_Starting;
    }
    self->velocity.y = 0;
    self->velocity.x = 0;
}

void Platform_State_WaitOscillating(void)
{
    RSDK_THIS(Platform);
    self->rotation += 4;
    self->drawPos.y = self->centerPos.y + (RSDK.Sin1024(self->rotation) << 9);
    if (self->stood) {
        if (self->hasTension)
            self->timer = 120;
        self->tileOrigin.x = self->drawPos.x - self->centerPos.x;
        self->centerPos.x  = self->drawPos.x;
        self->tileOrigin.y = RSDK.Sin1024(self->rotation) << 9;
        self->centerPos.y  = self->drawPos.y;
        self->active       = ACTIVE_NORMAL;
        self->state        = Platform_State_PlayerMove_Starting;
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

void Platform_State_Collapse_StartFall(void)
{
    RSDK_THIS(Platform);
    if (--self->timer <= 0) {
        self->timer = 0;
        self->state         = Platform_State_Collapse_Falling;
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

void Platform_State_ActiveFromAbove(void)
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
            self->amplitude.y -= (self->speed << 16);
            if (self->amplitude.y <= 0) {
                self->amplitude.y = 0;
                if (self->activeScreens && Platform->useClack)
                    RSDK.PlaySfx(Platform->sfxClack, false, 255);
            }
        }
    }
    else {
        if (self->amplitude.y < self->amplitude.x) {
            self->amplitude.y += (self->speed << 16);
            if (self->amplitude.y >= self->amplitude.x) {
                self->amplitude.y = self->amplitude.x;
                if (self->activeScreens && Platform->useClack)
                    RSDK.PlaySfx(Platform->sfxClack, false, 255);
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

void Platform_State_PlayerMove_Starting(void)
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
            self->state       = Platform_State_PlayerMove_Moving;
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
        if (self->timer > 0) {
            self->timer--;
            self->velocity.x = 0;
        }
    }
    else {
        if (Platform->playingPushSFX) {
            RSDK.StopSfx(Platform->sfxPush);
            Platform->playingPushSFX = false;
        }
        if (self->timer < 4) {
            self->timer++;
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

    bool32 collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, (self->hitbox.left + 16) << 16, self->hitbox.bottom << 16, 4);
    int32 y         = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, self->hitbox.bottom << 16, 4);
    if (self->position.y < y)
        y = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, (self->hitbox.right - 16) << 16, self->hitbox.bottom << 16, 4);
    if (self->position.y < y)
        y = self->position.y;

    self->position.y = y;
    self->drawPos.y  = y;
    if (!collided) {
        self->velocity.x <<= 1;
        if (!self->velocity.x) {
            self->state = Platform_State_Pushable_Falling;
        }
        else {
            if (self->velocity.x > 0)
                self->state = Platform_State_Pushable_FallingL;
            else
                self->state = Platform_State_Pushable_FallingR;
        }
    }
    RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, self->hitbox.left << 16, (self->hitbox.bottom - 8) << 16, true);
    RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, self->hitbox.right << 16, (self->hitbox.bottom - 8) << 16, true);
    RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, self->hitbox.left << 16, (self->hitbox.top + 8) << 16, true);
    RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, self->hitbox.right << 16, (self->hitbox.top + 8) << 16, true);

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
            RSDK.StopSfx(Platform->sfxPush);
            Platform->playingPushSFX = false;
        }
        self->state = Platform_State_Collapse_CheckReset;
    }
}

void Platform_State_Collapse_CheckReset(void)
{
    RSDK_THIS(Platform);
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        Platform_Create(NULL);
    self->velocity.y = 0;
    self->velocity.x = 0;
}

void Platform_State_Moving_Static(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 move  = Zone->timer * (self->speed << 7);

    if (((move >> 16) & 1) == self->direction) {
        self->drawPos.x = self->centerPos.x + ((move & 0xFFFF) * self->amplitude.x >> 6) - (self->amplitude.x << 9);
        self->drawPos.y = self->centerPos.y + ((move & 0xFFFF) * self->amplitude.y >> 6) - (self->amplitude.y << 9);
    }
    else {
        self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9) - ((move & 0xFFFF) * self->amplitude.x >> 6);
        self->drawPos.y = self->centerPos.y + (self->amplitude.y << 9) - ((move & 0xFFFF) * self->amplitude.y >> 6);
    }

    if (((move >> 16) & 1) != self->hasTension) {
        if (self->activeScreens)
            RSDK.PlaySfx(Platform->sfxClang, false, 255);
        self->hasTension = (move >> 16) & 1;
    }
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Collapse_Falling(void)
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
            self->state      = Platform_State_Collapse_CheckReset;
        }
        self->velocity.x = 0;
    }
}

void Platform_State_Collapse(void)
{
    RSDK_THIS(Platform);

    if (self->timer) {
        if (!--self->timer) {
            self->active        = ACTIVE_NORMAL;
            self->state         = Platform_State_Collapse_StartFall;
            self->timer = 30;
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

void Platform_State_SwingMove_CheckStood(void)
{
    RSDK_THIS(Platform);
    self->velocity.x = 0;
    self->velocity.y = 0;
    if (self->stood)
        self->state = Platform_State_SwingMove_MoveToDest;
}

void Platform_State_Sticky_CheckRide(void)
{
    RSDK_THIS(Platform);
    self->velocity.x = 0;
    self->velocity.y = 0;
    if (self->stood)
        self->state = Platform_State_Sticky_MoveToDest;
}

void Platform_State_Swing_Clack(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    int32 prevAngle = self->angle;
    int32 maxAngle  = self->groundVel + 0x100;
    self->angle     = self->groundVel + ((self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer) + 0x200) >> 14) + 0x100;
    if (!self->hasTension) {
        if (self->angle >= maxAngle) {
            if (!self->activeScreens || prevAngle >= maxAngle) {
                self->angle = self->groundVel + 0x100;
            }
            else {
                RSDK.PlaySfx(Platform->sfxClacker, false, 255);
                self->angle = self->groundVel + 0x100;
            }
        }
    }
    else if (self->angle <= maxAngle) {
        if (!self->activeScreens || prevAngle <= maxAngle) {
            self->angle = self->groundVel + 0x100;
        }
        else {
            RSDK.PlaySfx(Platform->sfxClacker, false, 255);
            self->angle = self->groundVel + 0x100;
        }
    }

    self->drawPos.x  = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y  = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Static(void)
{
    RSDK_THIS(Platform);
    int32 drawX        = -self->drawPos.x;
    int32 drawY        = -self->drawPos.y;
    self->drawPos.x  = self->centerPos.x;
    self->drawPos.y  = self->centerPos.y;
    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Sinker(void)
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

void Platform_State_Pushable_FallingL(void)
{
    RSDK_THIS(Platform);
    self->drawPos.x += self->velocity.x;
    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    if (!RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, self->hitbox.left << 16, self->hitbox.bottom << 16, false))
        self->state = Platform_State_Pushable_Falling;
    self->position.x = storeX;
    self->position.y = storeY;
}

void Platform_State_Pushable_FallingR(void)
{
    RSDK_THIS(Platform);
    self->drawPos.x += self->velocity.x;
    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    if (!RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, self->hitbox.right << 16, self->hitbox.bottom << 16, false))
        self->state = Platform_State_Pushable_Falling;
    self->position.x = storeX;
    self->position.y = storeY;
}

void Platform_State_Pushable_Falling(void)
{
    RSDK_THIS(Platform);
    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->drawPos.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    bool32 collided  = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, (self->hitbox.left + 16) << 16, self->hitbox.bottom << 16, 4);
    int32 ypos         = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, self->hitbox.bottom << 16, 4);
    if (self->position.y < ypos)
        ypos = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, (self->hitbox.right - 16) << 16, self->hitbox.bottom << 16, 4);
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
    self->position.x = storeX;
    self->position.y = storeY;
    self->velocity.x = 0;
}

void Platform_State_Controlled(void)
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

void Platform_State_PlayerMove_Moving(void)
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

        if (self->timer == -1) {
            self->timer = 0;
            if (self->type == PLATFORM_WAIT_OSC)
                self->state = Platform_State_WaitOscillating;
            else
                self->state = Platform_State_Wait;
        }
        else {
            if (self->hasTension) {
                self->state = Platform_State_PlayerMove_HandleRestart;
            }
            else {
                self->active = ACTIVE_BOUNDS;
                if (self->type == PLATFORM_WAIT_OSC)
                    self->state = Platform_State_PlayerMove_Rotate;
                else
                    self->state = Platform_State_Fixed;
            }
        }
    }
    else {
        self->groundVel -= self->speed << 10;
    }

    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_State_PlayerMove_Rotate(void)
{
    RSDK_THIS(Platform);
    self->rotation += 4;
    self->drawPos.y  = (RSDK.Sin1024(self->rotation) << 9) + self->centerPos.y;
    self->velocity.x = 0;
    self->velocity.y = 0;
}

void Platform_State_PlayerMove_HandleRestart(void)
{
    RSDK_THIS(Platform);
    if (self->type == PLATFORM_WAIT_OSC) {
        self->rotation += 4;
        self->drawPos.y = (RSDK.Sin1024(self->rotation) << 9) + self->centerPos.y;
    }

    if (self->stood) {
        self->timer = 120;
        self->velocity.x    = 0;
        self->velocity.y    = 0;
    }
    else {
        if (--self->timer) {
            self->tileOrigin.x  = self->drawPos.x - self->centerPos.x;
            self->tileOrigin.y  = self->drawPos.y - self->centerPos.y;
            self->centerPos.x   = self->drawPos.x;
            self->timer = -1;
            self->centerPos.y   = self->drawPos.y;
            self->active        = ACTIVE_NORMAL;
            self->state         = Platform_State_PlayerMove_Starting;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_State_ActivateControlOnStood(void)
{
    RSDK_THIS(Platform);

    if (!self->stood) {
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
    else {
        int32 slot                        = SceneInfo->entitySlot - 1;
        EntityPlatformControl *controller = RSDK_GET_ENTITY(slot, PlatformControl);
        if (controller->objectID == PlatformControl->objectID) {
            controller->setActive = true;
            self->state         = Platform_State_Controlled;
            self->velocity.x    = 0;
            self->velocity.y    = 0;
        }
        else {
            while (controller->objectID == Platform->objectID || controller->objectID == PlatformNode->objectID) {
                controller = RSDK_GET_ENTITY(slot--, PlatformControl);
                if (controller->objectID == PlatformControl->objectID) {
                    controller->setActive = true;
                    self->state         = Platform_State_Controlled;
                    self->velocity.x    = 0;
                    self->velocity.y    = 0;
                    break;
                }
            }
        }
    }
}

void Platform_State_SwingMove_MoveToDest(void)
{
    RSDK_THIS(Platform);
    int32 drawX   = -self->drawPos.x;
    int32 drawY   = -self->drawPos.y;
    self->angle = self->groundVel + 0x100 + ((self->amplitude.x * RSDK.Sin1024(self->timer * self->speed) + 0x200) >> 14);

    if (self->timer++ == 256 && self->hasTension) {
        self->timer = 119;
        self->state = Platform_State_SwingMove_WaitNotStood;
    }
    self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;

    if (self->groundVel < 0)
        self->drawPos.x += 0x200000;
    else
        self->drawPos.x -= 0x200000;
    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_State_SwingMove_WaitNotStood(void)
{
    RSDK_THIS(Platform);
    if (self->stood) {
        self->timer = 120;
        self->velocity.x    = 0;
        self->velocity.y    = 0;
    }
    else {
        if (!--self->timer) {
            self->timer = 257;
            self->active        = ACTIVE_NORMAL;
            self->state         = Platform_State_SwingMove_MoveToStart;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_State_SwingMove_MoveToStart(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 amp   = self->amplitude.x * RSDK.Sin1024(self->timer-- * self->speed);
    self->angle = self->groundVel + 0x100 + ((amp + 0x200) >> 14);

    if (self->timer == 88)
        self->state = Platform_State_SwingMove_CheckStood;

    self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
    if (self->groundVel < 0)
        self->drawPos.x += 0x200000;
    else
        self->drawPos.x -= 0x200000;
    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_State_Sticky_MoveToDest(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 move  = self->angle++ * (self->speed << 7);

    if (self->hasTension) {
        if (move < 0x10000) {
            if (move <= 0)
                move = 0;
        }
        else {
            move        = 0x10000;
            self->timer = 120;
            self->state = Platform_State_Sticky_WaitNotStood;
        }
    }

    if (((move >> 16) & 1) == self->direction) {
        self->drawPos.x = self->centerPos.x + ((move & 0xFFFF) * self->amplitude.x >> 6) - (self->amplitude.x << 9);
        self->drawPos.y = self->centerPos.y + ((move & 0xFFFF) * self->amplitude.y >> 6) - (self->amplitude.y << 9);
    }
    else {
        self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9) - ((move & 0xFFFF) * self->amplitude.x >> 6);
        self->drawPos.y = self->centerPos.y + (self->amplitude.y << 9) - ((move & 0xFFFF) * self->amplitude.y >> 6);
    }
    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_State_Sticky_WaitNotStood(void)
{
    RSDK_THIS(Platform);
    if (self->stoodPlayers) {
        self->timer = 120;
        self->velocity.x    = 0;
        self->velocity.y    = 0;
    }
    else {
        if (!--self->timer) {
            self->active = ACTIVE_NORMAL;
            self->state  = Platform_State_Sticky_MoveToStart;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_State_Sticky_MoveToStart(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 move = self->angle-- * (self->speed << 7);

    if (move > 0) {
        if (move >= 0x10000)
            move = 0x10000;
    }
    else {
        move         = 0;
        self->state = Platform_State_Sticky_CheckRide;
    }

    if (((move >> 16) & 1) == self->direction) {
        self->drawPos.x = self->centerPos.x + ((move & 0xFFFF) * self->amplitude.x >> 6) - (self->amplitude.x << 9);
        self->drawPos.y = self->centerPos.y + ((move & 0xFFFF) * self->amplitude.y >> 6) - (self->amplitude.y << 9);
    }
    else {
        self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9) - ((move & 0xFFFF) * self->amplitude.x >> 6);
        self->drawPos.y = self->centerPos.y + (self->amplitude.y << 9) - ((move & 0xFFFF) * self->amplitude.y >> 6);
    }
    self->velocity.x = drawX + self->drawPos.x;
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
                if (!((1 << pid) & stoodPlayers) && !player->sidekick && self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->timer = 1;
                    else
#endif
                        self->timer = 30;
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
    if (self->timer)
        self->timer--;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, self->type == PLATFORM_CIRCULAR, &player->uncurlTimer))
#endif
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
                if (!player->sidekick && self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->timer = 1;
                    else
#endif
                        self->timer = 30;
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
                if (!player->sidekick && self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->timer = 1;
                    else
#endif
                        self->timer = 30;
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
                    if (self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            self->timer = 1;
                        else
#endif
                            self->timer = 30;
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
        if ((self->collision != PLATFORM_C_STICKY_ALL && self->collision != side + PLATFORM_C_STICKY_ALL) || !side)
            flag = true;
        if (player->state != Player_State_None && !flag) {
            player->state                         = Player_State_None;
            player->nextGroundState               = StateMachine_None;
            player->nextAirState                  = StateMachine_None;
            player->velocity.x                    = 0;
            player->velocity.y                    = 0;
            player->groundVel                     = 0;
            player->animator.speed = 0;
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
                if (self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->timer = 1;
                    else
#endif
                        self->timer = 30;
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
                if (!player->sidekick && self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->timer = 1;
                    else
#endif
                        self->timer = 30;
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
            if (!((1 << pid) & stoodPlayers) && !player->sidekick && self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                    self->timer = 1;
                else
#endif
                    self->timer = 30;
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
                        player->animator.speed = 64;
                        player->direction                     = FLIP_NONE;
                        if (!player->sidekick && self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                            if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                                self->timer = 1;
                            else
#endif
                                self->timer = 30;
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
                        player->animator.speed = 64;
                        player->direction                     = FLIP_X;
                        if (!player->sidekick) {
                            self->stood = true;
                            if (self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                                    self->timer = 1;
                                else
#endif
                                    self->timer = 30;
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
                if (!((1 << pid) & stoodPlayers) && !player->sidekick && self->state == Platform_State_Collapse && !self->timer) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        self->timer = 1;
                    else
#endif
                        self->timer = 30;
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

#if RETRO_INCLUDE_EDITOR
void Platform_EditorDraw_Normal(void)
{
    RSDK_THIS(Platform);

    if (Platform->aniFrames == 0xFFFF || self->frameID == -1)
        RSDK.DrawRect(self->drawPos.x - 0x200000, self->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, self->alpha, self->inkEffect, false);
    else
        RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void Platform_EditorDraw_Swinging(Vector2 amplitude)
{
    RSDK_THIS(Platform);

    int32 frame = self->animator.frameID;

    int32 ang = self->angle;
    if (self->type == PLATFORM_CIRCULAR && self->hasTension)
        ang = 4 * self->angle;

    int32 fxStore = self->drawFX;
    self->drawFX |= FX_FLIP | FX_ROTATE;
    int32 cnt              = (amplitude.y >> 10) - 1;
    int32 angle            = 0x400;
    self->direction        = FLIP_NONE;
    self->animator.frameID = frame + 1;

    int32 rot = ang >> 1;
    Vector2 drawPos;
    for (int32 i = 0; i < cnt; ++i) {
        drawPos.x = angle * RSDK.Cos1024(ang) + self->centerPos.x;
        drawPos.y = angle * RSDK.Sin1024(ang) + self->centerPos.y;
        if (Platform->aniFrames == 0xFFFF || self->frameID == -1)
            RSDK.DrawRect(self->drawPos.x - 0x200000, self->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, self->alpha, self->inkEffect, false);
        else
            RSDK.DrawSprite(&self->animator, &drawPos, false);
        angle += 0x400;
        self->direction ^= FLIP_X;
        self->rotation = rot;
    }

    self->drawFX           = fxStore;
    self->animator.frameID = frame + 2;
    if (Platform->aniFrames == 0xFFFF || self->frameID == -1)
        RSDK.DrawRect(self->drawPos.x - 0x200000, self->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, self->alpha, self->inkEffect, false);
    else
        RSDK.DrawSprite(&self->animator, &self->centerPos, false);
    self->animator.frameID = frame;

    if (Platform->aniFrames == 0xFFFF || self->frameID == -1)
        RSDK.DrawRect(self->drawPos.x - 0x200000, self->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, self->alpha, self->inkEffect, false);
    else
        RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void Platform_EditorDraw(void)
{
    RSDK_THIS(Platform);
    self->drawPos   = self->position;
    self->centerPos = self->position;
    self->drawFX    = FX_NONE;
    self->inkEffect = INK_NONE;
    self->alpha     = 0xFF;
    self->direction = FLIP_NONE;

    RSDK.SetSpriteAnimation(Platform->aniFrames, 0, &self->animator, true, 0);
    self->animator.frameID = self->frameID;
    if (self->frameID >= 0) {
        int32 f    = self->frameID;
        int32 anim = 0;
        while (f >= self->animator.frameCount) {
            if (!self->animator.frameCount)
                break;
            f -= self->animator.frameCount;
            RSDK.SetSpriteAnimation(Platform->aniFrames, ++anim, &self->animator, true, 0);
        }
        self->animator.frameID = f;
    }
    else {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator, true, 0);
    }

    Vector2 amplitude;
    Vector2 drawPos;

    amplitude.x = self->amplitude.x >> 10;
    amplitude.y = self->amplitude.y >> 10;

    switch (self->type) {
        case PLATFORM_FIXED:
        default:
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            Platform_EditorDraw_Normal();
            break;
        case PLATFORM_COLLAPSING:
            self->updateRange.x = 0x800000;
            self->updateRange.y = (abs(amplitude.y) + 0x2000) << 10;

            Platform_EditorDraw_Normal();
            break;
        case PLATFORM_MOVING:
            self->updateRange.x = (abs(amplitude.x) + 0x2000) << 10;
            self->updateRange.y = (abs(amplitude.y) + 0x2000) << 10;

            Platform_EditorDraw_Normal();

            if (showGizmos()) {
                self->inkEffect = INK_BLEND;

                // start pos
                self->drawPos.x = amplitude.x * RSDK.Sin1024(self->angle) + self->centerPos.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                Platform_EditorDraw_Normal();

                // right max
                self->drawPos.x = amplitude.x * RSDK.Sin1024(0x100) + self->centerPos.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(0x100) + self->centerPos.y;
                drawPos         = self->drawPos;
                Platform_EditorDraw_Normal();

                // left max
                self->drawPos.x = amplitude.x * RSDK.Sin1024(0x300) + self->centerPos.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(0x300) + self->centerPos.y;
                Platform_EditorDraw_Normal();

                DrawHelpers_DrawArrow(0x00FF00, drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y);
                DrawHelpers_DrawArrow(0x00FF00, self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y);

                self->inkEffect = INK_NONE;
            }
            break;
        case PLATFORM_CIRCULAR:
            self->updateRange.x = (abs(amplitude.x) + 0x2000) << 10;
            self->updateRange.y = (abs(amplitude.y) + 0x2000) << 10;

            if (self->hasTension) {
                self->drawPos.x = amplitude.x * RSDK.Cos1024(4 * self->angle) + self->centerPos.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(4 * self->angle) + self->centerPos.y;
                Platform_EditorDraw_Swinging(amplitude);
            }
            else {
                Platform_EditorDraw_Normal();
            }

            if (showGizmos() && !self->hasTension) {
                self->inkEffect = INK_BLEND;

                // start pos
                self->drawPos.x = amplitude.x * RSDK.Cos1024(4 * self->angle) + self->centerPos.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(4 * self->angle) + self->centerPos.y;
                Platform_EditorDraw_Normal();

                self->inkEffect = INK_NONE;
            }
            break;
        case PLATFORM_CONTROLLED:
        case PLATFORM_CONT_ACTIVATER:
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            // PLATFORM_CONT_ACTIVATER activates control in prev slot
            // Note: will go back through platforms if not found
            // e.g. if entity is slot 3, and platform control is slot 0, with slot 1 and 2 being platform/platformNode, it'll work
            // if slot 1 or 2 ISNT a platform/platformNode, it will not work

            // self->speed = starting node

            Platform_EditorDraw_Normal();
            break;
        case PLATFORM_PUSHABLE:
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            Platform_EditorDraw_Normal();
            break;
        case PLATFORM_WAIT:
        case PLATFORM_WAIT_OSC:
            amplitude.x <<= 10;
            self->updateRange.x = 0x800000 + abs(amplitude.x);
            self->updateRange.y = 0x800000 + abs(amplitude.x);
            if (self->speed < 0)
                self->direction = FLIP_X;

            Platform_EditorDraw_Normal();

            if (showGizmos()) {
                drawPos         = self->drawPos;
                self->groundVel = 0;
                bool32 flag     = false;
                uint8 type      = 0;

                while (self->speed) {
                    if (!flag) {
                        amplitude.y += self->groundVel;

                        self->drawPos.x = (amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
                        self->drawPos.y = (amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;

                        int32 speed16 = self->speed << 16;
                        if (self->groundVel == speed16) {
                            if (amplitude.y >= amplitude.x) {
                                amplitude.y = amplitude.x;
                                self->groundVel -= (self->speed << 11);
                                flag = true;
                            }
                        }
                        else {
                            self->groundVel += (self->speed << 10);
                            if (self->groundVel >= speed16) {
                                self->groundVel = speed16;
                                self->centerPos.x += ((amplitude.y + self->groundVel) >> 8) * RSDK.Cos256(self->angle);
                                self->centerPos.y += ((self->groundVel + amplitude.y) >> 8) * RSDK.Sin256(self->angle);
                                amplitude.y = -self->groundVel;
                            }
                        }
                    }
                    else {
                        amplitude.y += self->groundVel;
                        self->drawPos.x = (amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
                        self->drawPos.y = (amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;

                        if (self->groundVel <= 0) {
                            if (self->hasTension) {
                                // draw double arrow (auto returns)
                                type = 1;
                            }
                            else {
                                // draw one arrow (one way)
                                type = 0;
                            }
                            break;
                        }
                        else {
                            self->groundVel -= self->speed << 10;
                        }
                    }
                }

                self->inkEffect = INK_BLEND;
                Platform_EditorDraw_Normal();

                self->inkEffect = INK_NONE;
                DrawHelpers_DrawArrow(0x00FF00, drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y);
                if (type)
                    DrawHelpers_DrawArrow(0x00FF00, self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y);
            }
            break;
        case PLATFORM_ACTIVEABOVE:
            amplitude.x <<= 10;
            self->updateRange.x = 0x800000 + abs(amplitude.x);
            self->updateRange.y = 0x800000 + abs(amplitude.x);

            if (self->speed < 0)
                self->direction = FLIP_X;

            Platform_EditorDraw_Normal();

            if (showGizmos()) {
                self->inkEffect = INK_BLEND;

                drawPos     = self->drawPos;
                amplitude.y = 0;
                while (self->speed && amplitude.x) {
                    if (amplitude.y < amplitude.x) {
                        amplitude.y += (abs(self->speed) << 16);
                        if (amplitude.y >= amplitude.x) {
                            amplitude.y = amplitude.x;
                            break;
                        }
                    }

                    if (self->direction) {
                        self->drawPos.x = (-amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
                        self->drawPos.y = (-amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;
                    }
                    else {
                        self->drawPos.x = (amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
                        self->drawPos.y = (amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;
                    }
                }

                Platform_EditorDraw_Normal();

                self->inkEffect = INK_NONE;

                DrawHelpers_DrawArrow(0x00FF00, drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y);
                DrawHelpers_DrawArrow(0x00FF00, self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y);
            }

            break;
        case PLATFORM_WAIT_ARC:
        case PLATFORM_SWINGING: {
            self->timer = 0;
            if (self->type == PLATFORM_WAIT_ARC)
                self->timer = 88;

            self->updateRange.x = (abs(amplitude.y) + 0x200) << 14;
            self->updateRange.y = (abs(amplitude.y) + 0x200) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            amplitude.y <<= 4;
            if (self->type != PLATFORM_SWINGING) {
                if (self->groundVel >= 0)
                    self->drawPos.x -= 0x200000;
                else
                    self->drawPos.x += 0x200000;
            }

            int32 angle     = self->angle;
            self->groundVel = 4 * self->angle;
            self->angle     = self->groundVel + 0x100 + (self->amplitude.x * RSDK.Sin1024(0x000 + self->timer) >> 14);
            self->drawPos.x = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
            self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
            Platform_EditorDraw_Swinging(amplitude);

            if (showGizmos()) {
                if (self->type == PLATFORM_SWINGING) {
                    self->inkEffect = INK_BLEND;

                    // right max
                    self->angle = self->groundVel + 0x100 + ((amplitude.x * RSDK.Sin1024(0x100) + 0x200) >> 14);

                    self->drawPos.x = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
                    self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                    drawPos         = self->drawPos;
                    Platform_EditorDraw_Swinging(amplitude);

                    // left max
                    self->angle     = self->groundVel + 0x100 + ((amplitude.x * RSDK.Sin1024(0x300) + 0x200) >> 14);
                    self->drawPos.x = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
                    self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                    Platform_EditorDraw_Swinging(amplitude);

                    self->inkEffect = INK_NONE;
                }
            }
            self->angle = angle;
            break;
        }
        case PLATFORM_STICKY:
            if (self->direction) {
                self->drawPos.x = self->centerPos.x + (amplitude.x << 9);
                self->drawPos.y = self->centerPos.y + (amplitude.y << 9);
            }
            else {
                self->drawPos.x = self->centerPos.x - (amplitude.x << 9);
                self->drawPos.y = self->centerPos.y - (amplitude.y << 9);
            }
            self->updateRange.x = (abs(amplitude.x) + 0x4000) << 9;
            self->updateRange.y = (abs(amplitude.y) + 0x4000) << 9;
            if (self->speed < 0)
                self->direction = FLIP_X;

            Platform_EditorDraw_Normal();
            if (showGizmos()) {
                drawPos       = self->drawPos;
                self->drawPos = self->centerPos;

                self->inkEffect = INK_BLEND;

                Platform_EditorDraw_Normal();

                self->inkEffect = INK_NONE;

                DrawHelpers_DrawArrow(0x00FF00, drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y);
                DrawHelpers_DrawArrow(0x00FF00, self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y);
            }
            break;
        case PLATFORM_MOVING_STATIC:
            self->updateRange.x = (abs(amplitude.x) + 0x4000) << 9;
            self->updateRange.y = (abs(amplitude.y) + 0x4000) << 9;
            if (self->speed < 0)
                self->direction = FLIP_X;

            Platform_EditorDraw_Normal();

            if (showGizmos()) {
                Vector2 storePos = self->drawPos;
                drawPos          = self->drawPos;

                uint8 flags = 0;
                int8 last   = -1;
                int32 timer = 0;
                while (flags != 2) {
                    int32 move = timer++ * (self->speed << 7);

                    if (((move >> 16) & 1) == self->direction) {
                        if (last > 0)
                            flags++;
                        if (flags != 2) {
                            self->drawPos.x = self->centerPos.x + ((move & 0xFFFF) * amplitude.x >> 6) - (amplitude.x << 9);
                            self->drawPos.y = self->centerPos.y + ((move & 0xFFFF) * amplitude.y >> 6) - (amplitude.y << 9);
                            storePos        = self->drawPos;
                        }
                        last = 0;
                    }
                    else {
                        if (!last)
                            flags++;
                        if (flags != 2) {
                            self->drawPos.x = self->centerPos.x + (amplitude.x << 9) - ((move & 0xFFFF) * amplitude.x >> 6);
                            self->drawPos.y = self->centerPos.y + (amplitude.y << 9) - ((move & 0xFFFF) * amplitude.y >> 6);
                            drawPos         = self->drawPos;
                        }
                        last = 1;
                    }
                }

                self->inkEffect = INK_BLEND;

                self->drawPos = drawPos;
                Platform_EditorDraw_Normal();

                self->drawPos = storePos;
                Platform_EditorDraw_Normal();

                self->inkEffect = INK_NONE;

                DrawHelpers_DrawArrow(0x00FF00, drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y);
                DrawHelpers_DrawArrow(0x00FF00, self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y);
            }

            break;
        case PLATFORM_SWING_CLACK: {
            self->updateRange.x = (abs(amplitude.y) + 0x200) << 14;
            self->updateRange.y = (abs(amplitude.y) + 0x200) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            amplitude.y <<= 4;
            self->groundVel = 4 * self->angle;
            self->angle     = self->groundVel + 0x100 + (amplitude.x * RSDK.Sin1024(self->hasTension ? 0x100 : 0) >> 14);
            self->drawPos.x = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
            self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;

            Platform_EditorDraw_Swinging(amplitude);

            if (showGizmos()) {
                self->inkEffect = INK_BLEND;

                // max
                self->angle     = self->groundVel + 0x100 + (amplitude.x * RSDK.Sin1024(self->hasTension ? 0 : -0x100) >> 14);
                self->drawPos.x = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                Platform_EditorDraw_Swinging(amplitude);

                self->inkEffect = INK_NONE;
                self->angle     = self->groundVel / 4;
            }
            break;
        }
        case PLATFORM_STATIC:
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            Platform_EditorDraw_Normal();
            break;
        case PLATFORM_SINKER:
            self->updateRange.x = 0x800000;
            self->updateRange.y = (abs(amplitude.y) + 0x2000) << 10;

            Platform_EditorDraw_Normal();

            if (showGizmos()) {
                self->inkEffect = INK_BLEND;

                // max
                self->drawPos.y += amplitude.y;
                Platform_EditorDraw_Normal();

                DrawHelpers_DrawArrow(0x00FF00, self->drawPos.x, self->drawPos.y - amplitude.y, self->drawPos.x, self->drawPos.y);

                self->inkEffect = INK_NONE;
            }
            break;
    }
}

void Platform_EditorLoad(void)
{
    Platform_StageLoad();

    RSDK_ACTIVE_VAR(Platform, type);
    RSDK_ENUM_VAR("Fixed", PLATFORM_FIXED);
    RSDK_ENUM_VAR("Collapsing", PLATFORM_COLLAPSING);
    RSDK_ENUM_VAR("Moving (Smooth)", PLATFORM_MOVING);
    RSDK_ENUM_VAR("Circlular", PLATFORM_CIRCULAR);
    RSDK_ENUM_VAR("Swinging", PLATFORM_SWINGING);
    RSDK_ENUM_VAR("Controlled", PLATFORM_CONTROLLED);
    RSDK_ENUM_VAR("Pushable", PLATFORM_PUSHABLE);
    RSDK_ENUM_VAR("Moving (Static)", PLATFORM_MOVING_STATIC);
    RSDK_ENUM_VAR("Wait for Player", PLATFORM_WAIT);
    RSDK_ENUM_VAR("Wait for Player (Oscillate)", PLATFORM_WAIT_OSC);
    RSDK_ENUM_VAR("Activate when Above", PLATFORM_ACTIVEABOVE);
    RSDK_ENUM_VAR("Controlled (Activates PlatformControl)", PLATFORM_CONT_ACTIVATER);
    RSDK_ENUM_VAR("Wait for Player, then Move with Arc", PLATFORM_WAIT_ARC);
    RSDK_ENUM_VAR("Sticky", PLATFORM_STICKY);
    RSDK_ENUM_VAR("Swinging (Clackers)", PLATFORM_SWING_CLACK);
    RSDK_ENUM_VAR("Static", PLATFORM_STATIC);
    RSDK_ENUM_VAR("Wait for Player, then Sink", PLATFORM_SINKER);

    RSDK_ACTIVE_VAR(Platform, collision);
    RSDK_ENUM_VAR("Solid (Top)", PLATFORM_C_SOLID_TOP);
    RSDK_ENUM_VAR("Solid (All)", PLATFORM_C_SOLID_ALL);
    RSDK_ENUM_VAR("Use Tiles", PLATFORM_C_USE_TILES);
    RSDK_ENUM_VAR("Hazard (All)", PLATFORM_C_HAZARD_ALL);
    RSDK_ENUM_VAR("Not Solid", PLATFORM_C_SOLID_NONE);
    RSDK_ENUM_VAR("Hazard (Sides)", PLATFORM_C_HAZARD_SIDES);
    RSDK_ENUM_VAR("Hazard (Bottom)", PLATFORM_C_HAZARD_BOTTOM);
    RSDK_ENUM_VAR("Hazard (Top)", PLATFORM_C_HAZARD_TOP);
    RSDK_ENUM_VAR("Twister", PLATFORM_C_TWISTER);
    RSDK_ENUM_VAR("Sticky (All)", PLATFORM_C_STICKY_ALL);
    RSDK_ENUM_VAR("Sticky (Top)", PLATFORM_C_STICKY_TOP);
    RSDK_ENUM_VAR("Sticky (Left)", PLATFORM_C_STICKY_LEFT);
    RSDK_ENUM_VAR("Sticky (Right)", PLATFORM_C_STICKY_RIGHT);
    RSDK_ENUM_VAR("Sticky (Bottom)", PLATFORM_C_STICKY_BOTTOM);
    RSDK_ENUM_VAR("Turntable", PLATFORM_C_TURNTABLE);
    RSDK_ENUM_VAR("Solid (All, No Crush)", PLATFORM_C_SOLID_ALL_NOCRUSH);
}
#endif

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
