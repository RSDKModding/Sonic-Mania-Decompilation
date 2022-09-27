// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Platform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPlatform *Platform;

void Platform_Update(void)
{
    RSDK_THIS(Platform);

#if MANIA_USE_PLUS
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

    if (self->classID) {
        self->stood = false;
        self->collisionOffset.x += self->drawPos.x & 0xFFFF0000;
        self->collisionOffset.y += self->drawPos.y & 0xFFFF0000;
        if (self->state != Platform_State_Falling2 && self->state != Platform_State_Hold) {
            StateMachine_Run(self->stateCollide);
        }

        self->position.x = self->centerPos.x;
        self->position.y = self->centerPos.y;
        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s++);
            if (child->classID == ItemBox->classID) {
                if (!child->scale.y) {
                    EntityItemBox *itemBox = (EntityItemBox *)child;
#if MANIA_USE_PLUS
                    itemBox->parent = (Entity *)self;
#else
                    itemBox->groundVel = RSDK.GetEntitySlot(self);
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

                if (child->classID == Spikes->classID) {
                    EntitySpikes *spikes      = (EntitySpikes *)child;
                    spikes->collisionOffset.x = self->collisionOffset.x;
                    spikes->collisionOffset.y = self->collisionOffset.y;
                }
                else if (child->classID == Platform->classID) {
                    EntityPlatform *platform = (EntityPlatform *)child;
                    platform->centerPos.x += self->collisionOffset.x;
                    platform->centerPos.y += self->collisionOffset.y;
                }
                else if (Ice && child->classID == Ice->classID) {
                    EntityIce *ice        = (EntityIce *)child;
                    ice->playerMoveOffset = self->collisionOffset;
                }
                else if (TurboTurtle && child->classID == TurboTurtle->classID) {
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
    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
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
            || (self->state == Platform_State_Swing || self->state == Platform_State_Clacker || self->type == PLATFORM_SWING_REACT)) {
            int32 ang = self->angle;
            if (self->state == Platform_State_Circular && self->hasTension)
                ang = self->speed * Zone->timer + 4 * self->angle;

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
        }

        if (Platform->aniFrames == (uint16)-1)
            RSDK.DrawRect(self->drawPos.x - TO_FIXED(32), self->drawPos.y - TO_FIXED(16), TO_FIXED(64), TO_FIXED(32), 0x8080A0, 255, INK_NONE, false);
        else
            RSDK.DrawSprite(&self->animator, &self->drawPos, false);
    }
}

void Platform_Create(void *data)
{
    RSDK_THIS(Platform);

    self->amplitude.x >>= 10;
    self->amplitude.y >>= 10;
    self->active    = ACTIVE_BOUNDS;
    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0] + 1;
    self->centerPos = self->position;
    self->drawPos   = self->position;
    RSDK.SetSpriteAnimation(Platform->aniFrames, 0, &self->animator, true, 0);
    self->animator.frameID = self->frameID;

    if (!SceneInfo->inEditor && RSDK.GetFrameID(&self->animator) == 'l')
        self->drawGroup = Zone->objectDrawGroup[0];

    switch (self->type) {
        default:
        case PLATFORM_FIXED:
            self->state         = Platform_State_Fixed;
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);
            break;

        case PLATFORM_FALL:
            self->state         = Platform_State_Fall;
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
            break;

        case PLATFORM_LINEAR:
            self->state         = Platform_State_Linear;
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

        case PLATFORM_PATH:
        case PLATFORM_PATH_REACT:
            self->active = ACTIVE_BOUNDS;
            if (self->type == PLATFORM_PATH)
                self->state = Platform_State_PathStop;
            else
                self->state = Platform_State_PathReact;
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);
            break;

        case PLATFORM_PUSH:
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);
            if (!SceneInfo->inEditor) {
                self->speed <<= 11;
                self->position.x += 0x8000;
            }
            self->state = Platform_State_Push_Init;
            break;

        case PLATFORM_REACT:
            self->amplitude.x <<= 10;
            self->updateRange.x = TO_FIXED(128) + abs(self->amplitude.x);
            self->updateRange.y = TO_FIXED(128) + abs(self->amplitude.x);
            if (self->speed < 0)
                self->direction = FLIP_X;
            self->state = Platform_State_React;
            break;

        case PLATFORM_HOVER_REACT:
            self->amplitude.x <<= 10;
            self->updateRange.x = TO_FIXED(128) + abs(self->amplitude.x);
            self->updateRange.y = TO_FIXED(128) + abs(self->amplitude.x);
            if (self->speed < 0)
                self->direction = FLIP_X;
            self->state = Platform_State_Hover_React;
            break;

        case PLATFORM_DOORSLIDE:
            self->amplitude.x <<= 10;
            self->updateRange.x = TO_FIXED(128) + abs(self->amplitude.x);
            self->updateRange.y = TO_FIXED(128) + abs(self->amplitude.x);

            if (self->speed < 0) {
                self->direction = FLIP_X;
                self->speed     = -self->speed;
            }
            self->state = Platform_State_DoorSlide;
            break;

        case PLATFORM_SWING_REACT:
            self->timer = 88;
            // [Fallthrough]
        case PLATFORM_SWING:
            self->updateRange.x = (abs(self->amplitude.y) + 0x200) << 14;
            self->updateRange.y = (abs(self->amplitude.y) + 0x200) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);

            self->amplitude.y <<= 4;
            self->groundVel = 4 * self->angle;
            self->angle     = self->groundVel + 0x100 + (self->amplitude.x * RSDK.Sin1024(self->speed * self->timer) >> 14);
            self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
            self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
            if (self->type == PLATFORM_SWING) {
                self->state = Platform_State_Swing;
            }
            else {
                if (self->groundVel >= 0)
                    self->drawPos.x -= TO_FIXED(32);
                else
                    self->drawPos.x += TO_FIXED(32);
                self->state = Platform_State_SwingReact;
            }
            break;

        case PLATFORM_TRACK_REACT:
            if (self->direction) {
                self->drawPos.x = self->centerPos.x + (self->amplitude.x << 9);
                self->drawPos.y = self->centerPos.y + (self->amplitude.y << 9);
            }
            else {
                self->drawPos.x = self->centerPos.x - (self->amplitude.x << 9);
                self->drawPos.y = self->centerPos.y - (self->amplitude.y << 9);
            }
            // [Fallthrough]
        case PLATFORM_TRACK:
            self->updateRange.x = (abs(self->amplitude.x) + 0x4000) << 9;
            self->updateRange.y = (abs(self->amplitude.y) + 0x4000) << 9;
            if (self->speed < 0)
                self->direction = FLIP_X;

            if (self->type == PLATFORM_TRACK)
                self->state = Platform_State_Track;
            else
                self->state = Platform_State_TrackReact;
            break;

        case PLATFORM_CLACKER:
            self->updateRange.x = (abs(self->amplitude.y) + 0x200) << 14;
            self->updateRange.y = (abs(self->amplitude.y) + 0x200) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            self->amplitude.y <<= 4;
            self->groundVel = 4 * self->angle;
            self->angle     = self->groundVel + 0x100 + (self->amplitude.x * RSDK.Sin1024(self->speed * self->timer) >> 14);
            self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
            self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
            self->state     = Platform_State_Clacker;
            break;

        case PLATFORM_CHILD:
            self->state         = Platform_State_Child;
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);
            break;

        case PLATFORM_DIPROCK:
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = (abs(self->amplitude.y) + 0x2000) << 10;
            if (!self->speed)
                self->speed = 1;
            self->amplitude.y = self->amplitude.y << 10;
            self->velocity.y  = self->speed << 16;
            self->state       = Platform_State_DipRock;
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
        RSDK.SetSpriteAnimation(-1, 0, &self->animator, true, 0);
    }

    if (!SceneInfo->inEditor) {
        if (self->collision != PLATFORM_C_NONE) {
            Hitbox *hitbox = RSDK.GetHitbox(&self->animator, self->collision != PLATFORM_C_PLATFORM);
            if (Platform->aniFrames != (uint16)-1 && hitbox) {
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
            case PLATFORM_C_PLATFORM:
                self->stateCollide  = Platform_Collision_Platform;
                self->hitbox.bottom = self->hitbox.top + 8;
                break;

            default:
            case PLATFORM_C_SOLID: self->stateCollide = Platform_Collision_Solid; break;

            case PLATFORM_C_TILED: self->stateCollide = Platform_Collision_Tiles; break;
            case PLATFORM_C_HURT: self->stateCollide = Platform_Collision_Hurt; break;
            case PLATFORM_C_NONE: self->stateCollide = Platform_Collision_None; break;
            case PLATFORM_C_SOLID_HURT_SIDES: self->stateCollide = Platform_Collision_Solid_Hurt_Sides; break;
            case PLATFORM_C_SOLID_HURT_BOTTOM: self->stateCollide = Platform_Collision_Solid_Hurt_Bottom; break;
            case PLATFORM_C_SOLID_HURT_TOP: self->stateCollide = Platform_Collision_Solid_Hurt_Top; break;
            case PLATFORM_C_SOLID_HOLD: self->stateCollide = Platform_Collision_Solid_Hold; break;

            case PLATFORM_C_SOLID_STICKY:
            case PLATFORM_C_STICKY_TOP:
            case PLATFORM_C_STICKY_LEFT:
            case PLATFORM_C_STICKY_RIGHT:
            case PLATFORM_C_STICKY_BOTTOM: self->stateCollide = Platform_Collision_Sticky; break;

            case PLATFORM_C_SOLID_BARREL: self->stateCollide = Platform_Collision_Solid_Barrel; break;
            case PLATFORM_C_SOLID_NOCRUSH: self->stateCollide = Platform_Collision_Solid_NoCrush; break;
        }

        for (int32 i = 0; i < self->childCount; ++i) {
            EntityPlatform *child = RSDK_GET_ENTITY((i + RSDK.GetEntitySlot(self) + 1), Platform);
            child->tileCollisions = TILECOLLISION_NONE;
            if (HangPoint && child->classID == HangPoint->classID) {
                EntityHangPoint *hang = (EntityHangPoint *)child;
                if (self->updateRange.y < TO_FIXED(128) + abs(self->position.y - (hang->length << 16) - hang->position.y))
                    self->updateRange.y = TO_FIXED(128) + abs(self->position.y - (hang->length << 16) - hang->position.y);
            }
            else {
                if (self->updateRange.y < TO_FIXED(128) + abs(self->position.y - child->position.y))
                    self->updateRange.y = TO_FIXED(128) + abs(self->position.y - child->position.y);
            }
        }
    }
}

void Platform_StageLoad(void)
{
    Platform->useClack = false;
    if (RSDK.CheckSceneFolder("GHZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("GHZCutscene")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("GHZCutscene/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("CPZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("SPZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("SPZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("FBZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Platform.bin", SCOPE_STAGE);
        Platform->sfxClack  = RSDK.GetSfx("Stage/Clack2.wav");
        Platform->useClack  = true;
    }
    else if (RSDK.CheckSceneFolder("PSZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("PSZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("SSZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("SSZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("HCZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE);
        Platform->sfxClack  = RSDK.GetSfx("Stage/Clack2.wav");
        Platform->useClack  = true;
    }
    else if (RSDK.CheckSceneFolder("MSZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("LRZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("MMZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("TMZ1") || RSDK.CheckSceneFolder("TMZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("AIZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("SBZ")) { // SBZ once was a planned zone?
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SBZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("Blueprint")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/Platform.bin", SCOPE_STAGE);
    }

    Platform->sfxClacker = RSDK.GetSfx("Stage/Clacker.wav");
    Platform->sfxClang   = RSDK.GetSfx("Stage/Clang.wav");
    Platform->sfxPush    = RSDK.GetSfx("Global/Push.wav");

    if (Platform->playingPushSfx) {
        RSDK.StopSfx(Platform->sfxPush);
        Platform->playingPushSfx = false;
    }
}

void Platform_State_PathStop(void)
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

void Platform_State_Linear(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    self->drawPos.x = self->amplitude.x * RSDK.Sin1024(self->speed * (self->rotation + Zone->timer)) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->speed * (self->rotation + Zone->timer)) + self->centerPos.y;
    self->drawPos.y += RSDK.Sin256(self->stoodAngle) << 10;

    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Swing(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    self->angle     = self->groundVel + 0x100 + ((self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer) + 0x200) >> 14);
    self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;

    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_React(void)
{
    RSDK_THIS(Platform);

    if (self->stood) {
        if (self->hasTension)
            self->timer = 120;
        self->rotation     = 0;
        self->tileOrigin.x = 0;
        self->tileOrigin.y = 0;
        self->active       = ACTIVE_NORMAL;
        self->state        = Platform_State_ReactMove;
    }

    self->velocity.y = 0;
    self->velocity.x = 0;
}

void Platform_State_Hover_React(void)
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
        self->state        = Platform_State_ReactMove;
    }

    self->velocity.y = 0;
    self->velocity.x = 0;
}

void Platform_State_Push_Init(void)
{
    RSDK_THIS(Platform);

    self->active        = ACTIVE_NORMAL;
    self->updateRange.x = TO_FIXED(512);
    self->updateRange.y = TO_FIXED(512);
    self->state         = Platform_State_Push;
    self->velocity.x    = 0;
    self->velocity.y    = 0;
}

void Platform_State_Falling(void)
{
    RSDK_THIS(Platform);

    if (--self->timer <= 0) {
        self->timer = 0;
        self->state = Platform_State_Falling2;
        foreach_active(Player, player)
        {
            if ((1 << RSDK.GetEntitySlot(player)) & self->stoodPlayers)
                player->velocity.y = self->velocity.y - TO_FIXED(1);
        }
    }

    self->drawPos.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->velocity.x = 0;
}

void Platform_State_DoorSlide(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    bool32 activated = false;
    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            Vector2 pivotPos = player->position;
            Zone_RotateOnPivot(&pivotPos, &self->centerPos, ((self->angle & 0xFF) + 0x80) & 0xFF);

            if (abs(player->position.x - self->centerPos.x) <= TO_FIXED(1024)) {
                if (abs(player->position.y - self->centerPos.y) <= TO_FIXED(1024)) {
                    if (pivotPos.y < self->centerPos.y && self->centerPos.y - pivotPos.y < TO_FIXED(256))
                        activated = true;
                }
            }
        }
    }

    if (!activated) {
        if (self->amplitude.y > 0) {
            self->amplitude.y -= (self->speed << 16);
            if (self->amplitude.y <= 0) {
                self->amplitude.y = 0;
                if (self->onScreen && Platform->useClack)
                    RSDK.PlaySfx(Platform->sfxClack, false, 0xFF);
            }
        }
    }
    else {
        if (self->amplitude.y < self->amplitude.x) {
            self->amplitude.y += (self->speed << 16);
            if (self->amplitude.y >= self->amplitude.x) {
                self->amplitude.y = self->amplitude.x;
                if (self->onScreen && Platform->useClack)
                    RSDK.PlaySfx(Platform->sfxClack, false, 0xFF);
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

void Platform_State_ReactMove(void)
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
            self->state       = Platform_State_ReactSlow;
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

void Platform_State_Push(void)
{
    RSDK_THIS(Platform);

    self->velocity.x = 0;
    self->velocity.y = 0;

    foreach_active(Player, playerLoop)
    {
        uint8 id = 1 << RSDK.GetEntitySlot(playerLoop);
        if (id & self->pushPlayersL)
            self->velocity.x += self->speed;
        if (id & self->pushPlayersR)
            self->velocity.x -= self->speed;
    }

    if (self->velocity.x > 0 || self->velocity.x < 0) {
        if (!Platform->playingPushSfx) {
            RSDK.PlaySfx(Platform->sfxPush, true, 255);
            Platform->playingPushSfx = true;
        }
    }

    if (self->velocity.x) {
        if (self->timer > 0) {
            self->timer--;
            self->velocity.x = 0;
        }
    }
    else {
        if (Platform->playingPushSfx) {
            RSDK.StopSfx(Platform->sfxPush);
            Platform->playingPushSfx = false;
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
        int32 bitID          = 1 << RSDK.GetEntitySlot(player);
        if (bitID & self->pushPlayersL)
            player->position.x = self->drawPos.x + ((self->hitbox.left - playerHitbox->right) << 16);
        if (bitID & self->pushPlayersR)
            player->position.x = self->drawPos.x + ((self->hitbox.right - playerHitbox->left - 1) << 16);
    }

    bool32 collided =
        RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, TO_FIXED(self->hitbox.left + 16), TO_FIXED(self->hitbox.bottom), 4);
    int32 y = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, TO_FIXED(0), TO_FIXED(self->hitbox.bottom), 4);
    if (self->position.y < y)
        y = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, TO_FIXED(self->hitbox.right - 16), TO_FIXED(self->hitbox.bottom), 4);
    if (self->position.y < y)
        y = self->position.y;

    self->position.y = y;
    self->drawPos.y  = y;

    if (!collided) {
        self->velocity.x <<= 1;
        if (!self->velocity.x) {
            self->state = Platform_State_Push_Fall;
        }
        else {
            if (self->velocity.x > 0)
                self->state = Platform_State_Push_SlideOffL;
            else
                self->state = Platform_State_Push_SlideOffR;
        }
    }

    RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, TO_FIXED(self->hitbox.left), TO_FIXED(self->hitbox.bottom - 8), true);
    RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, TO_FIXED(self->hitbox.right), TO_FIXED(self->hitbox.bottom - 8), true);
    RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, TO_FIXED(self->hitbox.left), TO_FIXED(self->hitbox.top + 8), true);
    RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, TO_FIXED(self->hitbox.right), TO_FIXED(self->hitbox.top + 8), true);

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;
    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->speed >>= 11;
        self->drawPos.x  = self->centerPos.x;
        self->drawPos.y  = self->centerPos.y;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->visible    = false;
        if (Platform->playingPushSfx) {
            RSDK.StopSfx(Platform->sfxPush);
            Platform->playingPushSfx = false;
        }
        self->state = Platform_State_Hold;
    }
}

void Platform_State_Hold(void)
{
    RSDK_THIS(Platform);

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        Platform_Create(NULL);

    self->velocity.y = 0;
    self->velocity.x = 0;
}

void Platform_State_Track(void)
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
        if (self->onScreen)
            RSDK.PlaySfx(Platform->sfxClang, false, 255);

        self->hasTension = (move >> 16) & 1;
    }

    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_Falling2(void)
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
            destroyEntity(self);
        }
        else {
            self->drawPos.x  = self->centerPos.x;
            self->drawPos.y  = self->centerPos.y;
            self->velocity.y = 0;
            self->visible    = false;
            self->state      = Platform_State_Hold;
        }

        self->velocity.x = 0;
    }
}

void Platform_State_Fall(void)
{
    RSDK_THIS(Platform);

    if (self->timer) {
        if (!--self->timer) {
            self->active = ACTIVE_NORMAL;
            self->state  = Platform_State_Falling;
            self->timer  = 30;
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

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    self->drawPos.x = self->amplitude.x * RSDK.Cos1024(self->speed * Zone->timer + 4 * self->angle) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer + 4 * self->angle) + self->centerPos.y;

    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_SwingReact(void)
{
    RSDK_THIS(Platform);

    self->velocity.x = 0;
    self->velocity.y = 0;

    if (self->stood)
        self->state = Platform_State_Swing2;
}

void Platform_State_TrackReact(void)
{
    RSDK_THIS(Platform);

    self->velocity.x = 0;
    self->velocity.y = 0;

    if (self->stood)
        self->state = Platform_State_Track2;
}

void Platform_State_Clacker(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    int32 prevAngle = self->angle;
    int32 maxAngle  = self->groundVel + 0x100;
    self->angle     = self->groundVel + ((self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer) + 0x200) >> 14) + 0x100;

    if (!self->hasTension) {
        if (self->angle >= maxAngle) {
            if (!self->onScreen || prevAngle >= maxAngle) {
                self->angle = self->groundVel + 0x100;
            }
            else {
                RSDK.PlaySfx(Platform->sfxClacker, false, 255);
                self->angle = self->groundVel + 0x100;
            }
        }
    }
    else if (self->angle <= maxAngle) {
        if (!self->onScreen || prevAngle <= maxAngle) {
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

void Platform_State_Child(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    self->drawPos.x = self->centerPos.x;
    self->drawPos.y = self->centerPos.y;

    self->velocity.x = self->drawPos.x + drawX;
    self->velocity.y = self->drawPos.y + drawY;
}

void Platform_State_DipRock(void)
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

void Platform_State_Push_SlideOffL(void)
{
    RSDK_THIS(Platform);

    self->drawPos.x += self->velocity.x;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    if (!RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, TO_FIXED(self->hitbox.left), TO_FIXED(self->hitbox.bottom), false))
        self->state = Platform_State_Push_Fall;

    self->position.x = storeX;
    self->position.y = storeY;
}

void Platform_State_Push_SlideOffR(void)
{
    RSDK_THIS(Platform);

    self->drawPos.x += self->velocity.x;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;

    if (!RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, TO_FIXED(self->hitbox.right), TO_FIXED(self->hitbox.bottom), false))
        self->state = Platform_State_Push_Fall;

    self->position.x = storeX;
    self->position.y = storeY;
}

void Platform_State_Push_Fall(void)
{
    RSDK_THIS(Platform);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->drawPos.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;

    bool32 collided =
        RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, TO_FIXED(self->hitbox.left + 16), TO_FIXED(self->hitbox.bottom), 4);
    int32 y = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, TO_FIXED(0), TO_FIXED(self->hitbox.bottom), 4);
    if (self->position.y < y)
        y = self->position.y;

    collided |= RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, TO_FIXED(self->hitbox.right - 16), TO_FIXED(self->hitbox.bottom), 4);
    if (self->position.y < y)
        y = self->position.y;

    if (collided) {
        self->velocity.y = 0;
        self->state      = Platform_State_Push;
    }
    else {
        self->position.y = y;
        self->drawPos.y  = y;
    }

    self->position.x = storeX;
    self->position.y = storeY;

    self->velocity.x = 0;
}

void Platform_State_Path(void)
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

void Platform_State_ReactSlow(void)
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
            if (self->type == PLATFORM_HOVER_REACT)
                self->state = Platform_State_Hover_React;
            else
                self->state = Platform_State_React;
        }
        else {
            if (self->hasTension) {
                self->state = Platform_State_ReactWait;
            }
            else {
                self->active = ACTIVE_BOUNDS;
                if (self->type == PLATFORM_HOVER_REACT)
                    self->state = Platform_State_Hover;
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

void Platform_State_Hover(void)
{
    RSDK_THIS(Platform);

    self->rotation += 4;
    self->drawPos.y  = (RSDK.Sin1024(self->rotation) << 9) + self->centerPos.y;
    self->velocity.x = 0;
    self->velocity.y = 0;
}

void Platform_State_ReactWait(void)
{
    RSDK_THIS(Platform);

    if (self->type == PLATFORM_HOVER_REACT) {
        self->rotation += 4;
        self->drawPos.y = (RSDK.Sin1024(self->rotation) << 9) + self->centerPos.y;
    }

    if (self->stood) {
        self->timer      = 120;
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
    else {
        if (--self->timer) {
            self->tileOrigin.x = self->drawPos.x - self->centerPos.x;
            self->tileOrigin.y = self->drawPos.y - self->centerPos.y;
            self->centerPos.x  = self->drawPos.x;
            self->centerPos.y  = self->drawPos.y;
            self->timer        = -1;
            self->active       = ACTIVE_NORMAL;
            self->state        = Platform_State_ReactMove;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_State_PathReact(void)
{
    RSDK_THIS(Platform);

    if (!self->stood) {
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
    else {
        int32 slot                        = SceneInfo->entitySlot - 1;
        EntityPlatformControl *controller = RSDK_GET_ENTITY(slot, PlatformControl);
        if (controller->classID == PlatformControl->classID) {
            controller->setActive = true;
            self->state           = Platform_State_Path;
            self->velocity.x      = 0;
            self->velocity.y      = 0;
        }
        else {
            while (controller->classID == Platform->classID || controller->classID == PlatformNode->classID) {
                controller = RSDK_GET_ENTITY(slot--, PlatformControl);
                if (controller->classID == PlatformControl->classID) {
                    controller->setActive = true;
                    self->state           = Platform_State_Path;
                    self->velocity.x      = 0;
                    self->velocity.y      = 0;
                    break;
                }
            }
        }
    }
}

void Platform_State_Swing2(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    self->angle = self->groundVel + 0x100 + ((self->amplitude.x * RSDK.Sin1024(self->timer * self->speed) + 0x200) >> 14);

    if (self->timer++ == 256 && self->hasTension) {
        self->timer = 119;
        self->state = Platform_State_SwingWait;
    }
    self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;

    if (self->groundVel < 0)
        self->drawPos.x += TO_FIXED(32);
    else
        self->drawPos.x -= TO_FIXED(32);

    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_State_SwingWait(void)
{
    RSDK_THIS(Platform);

    if (self->stood) {
        self->timer      = 120;
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
    else {
        if (!--self->timer) {
            self->timer  = 257;
            self->active = ACTIVE_NORMAL;
            self->state  = Platform_State_SwingReturn;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_State_SwingReturn(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 amp   = self->amplitude.x * RSDK.Sin1024(self->timer-- * self->speed);
    self->angle = self->groundVel + 0x100 + ((amp + 0x200) >> 14);

    if (self->timer == 88)
        self->state = Platform_State_SwingReact;

    self->drawPos.x = self->amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
    self->drawPos.y = self->amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
    if (self->groundVel < 0)
        self->drawPos.x += TO_FIXED(32);
    else
        self->drawPos.x -= TO_FIXED(32);
    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void Platform_State_Track2(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 move  = self->angle++ * (self->speed << 7);

    if (self->hasTension) {
        if (move < TO_FIXED(1)) {
            if (move <= 0)
                move = 0;
        }
        else {
            move        = TO_FIXED(1);
            self->timer = 120;
            self->state = Platform_State_TrackWait;
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

void Platform_State_TrackWait(void)
{
    RSDK_THIS(Platform);

    if (self->stoodPlayers) {
        self->timer      = 120;
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
    else {
        if (!--self->timer) {
            self->active = ACTIVE_NORMAL;
            self->state  = Platform_State_TrackReturn;
        }
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

void Platform_State_TrackReturn(void)
{
    RSDK_THIS(Platform);

    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;
    int32 move  = self->angle-- * (self->speed << 7);

    if (move > 0) {
        if (move >= TO_FIXED(1))
            move = TO_FIXED(1);
    }
    else {
        move        = 0;
        self->state = Platform_State_TrackReact;
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
void Platform_Collision_Solid(void)
{
    RSDK_THIS(Platform);

    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;

    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);

        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP:
                Platform_HandleStood(self, player, playerID, stoodPlayers);

                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;

            case C_LEFT:
                if (player->onGround && player->right)
                    self->pushPlayersL |= 1 << playerID;

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
                    self->pushPlayersR |= 1 << playerID;

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
void Platform_Collision_Hurt(void)
{
    RSDK_THIS(Platform);

    if (self->timer)
        self->timer--;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x400, self->type == PLATFORM_CIRCULAR, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }
}
void Platform_Collision_Solid_Hurt_Bottom(void)
{
    RSDK_THIS(Platform);

    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);

    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);

        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP:
                Platform_HandleStood(self, player, playerID, 0xFF);

                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;

            case C_LEFT:
                if (player->onGround && player->right)
                    self->pushPlayersL |= 1 << playerID;

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
                    self->pushPlayersR |= 1 << playerID;

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

#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(player, 0x400, 0, &player->uncurlTimer))
#endif
                    Player_Hurt(player, self);
                break;

            default: break;
        }
    }
}
void Platform_Collision_Solid_Hurt_Sides(void)
{
    RSDK_THIS(Platform);

    Hitbox *solidHitbox = RSDK.GetHitbox(&self->animator, 1);

    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 playerID = RSDK.GetEntitySlot(player);

        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP:
                Platform_HandleStood(self, player, playerID, 0xFF);

                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;

            case C_LEFT:
                if (
#if MANIA_USE_PLUS
                    Player_CheckMightyUnspin(player, 1024, 0, &player->uncurlTimer) ||
#endif
                    Player_Hurt(player, self)) {
                    player->velocity.x += self->velocity.x;
                }

                if (!player->blinkTimer || player->velocity.x >= self->velocity.x) {
                    if (player->onGround && player->right)
                        self->pushPlayersL |= 1 << playerID;

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
#if MANIA_USE_PLUS
                    Player_CheckMightyUnspin(player, 0x400, 0, &player->uncurlTimer) ||
#endif
                    Player_Hurt(player, self)) {
                    player->velocity.x += self->velocity.x;
                }

                if (!player->blinkTimer || player->velocity.x <= self->velocity.x) {
                    if (player->onGround && player->left)
                        self->pushPlayersR |= 1 << playerID;

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

            default: break;
        }
    }
}
void Platform_Collision_Tiles(void)
{
    RSDK_THIS(Platform);

    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        Hitbox hitbox;
        hitbox.left   = self->hitbox.left - 16;
        hitbox.top    = self->hitbox.top - 16;
        hitbox.right  = self->hitbox.right + 16;
        hitbox.bottom = self->hitbox.bottom + 16;

        if (Player_CheckCollisionTouch(player, self, &hitbox)) {
            player->collisionLayers |= Zone->moveLayerMask;
            player->moveLayerPosition.x = self->tileOrigin.x - self->drawPos.x;
            player->moveLayerPosition.y = self->tileOrigin.y - self->drawPos.y;
            if (player->animator.animationID == ANI_PUSH && player->onGround) {
                if (player->right)
                    self->pushPlayersL |= 1 << playerID;

                if (player->onGround && player->left)
                    self->pushPlayersR |= 1 << playerID;
            }

            bool32 isClimbing = false;
            if (player->state == Player_State_KnuxWallClimb || player->state == Player_State_KnuxLedgePullUp) {
                isClimbing = true;

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

            if (Player_CheckCollisionTouch(player, self, &hitbox) && (player->onGround || isClimbing)) {
                Platform_HandleStood_Tiles(self, player, playerID);
            }
        }
    }
}
void Platform_Collision_Sticky(void)
{
    RSDK_THIS(Platform);

    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);

    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 playerID = RSDK.GetEntitySlot(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);

        int32 side = Player_CheckCollisionBox(player, self, solidHitbox);

        Platform_HandleStood_Sticky(self, player, playerID, side);
    }
}
void Platform_Collision_Solid_Hurt_Top(void)
{
    RSDK_THIS(Platform);

    Hitbox *solidHitbox = RSDK.GetHitbox(&self->animator, 1);

    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 playerID = RSDK.GetEntitySlot(player);

        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP: Platform_HandleStood(self, player, playerID, 0xFF);

#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(player, 0x400, 0, &player->uncurlTimer))
#endif
                    Player_Hurt(player, self);

                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;

            case C_LEFT:
                if (player->onGround && player->right)
                    self->pushPlayersL |= 1 << playerID;

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
                    self->pushPlayersR |= 1 << playerID;

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
void Platform_Collision_Platform(void)
{
    RSDK_THIS(Platform);

    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;

    self->stoodPlayers = 0;
    foreach_active(Player, player)
    {
        uint16 playerID = RSDK.GetEntitySlot(player);
        int32 yVel      = player->velocity.y;

        if (self->collisionOffset.y < 0)
            player->velocity.y -= self->collisionOffset.y;

        if (Player_CheckCollisionPlatform(player, self, platformHitbox)) {
            Platform_HandleStood(self, player, playerID, stoodPlayers);
        }
        else {
            player->velocity.y = yVel;
        }
    }
}
void Platform_Collision_Solid_Barrel(void)
{
    RSDK_THIS(Platform);

    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;

    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 playerID = RSDK.GetEntitySlot(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);

        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_TOP: self->stood = true;
#if MANIA_USE_PLUS
                if (player->characterID != ID_MIGHTY || player->state != Player_State_MightyHammerDrop) {
#endif

                    Platform_HandleStood_Barrel(self, player, playerID, stoodPlayers);

                    if (self->velocity.y <= 0)
                        player->collisionFlagV |= 1;
#if MANIA_USE_PLUS
                }
#endif
                break;

            case C_LEFT:
                if (player->onGround && player->right)
                    self->pushPlayersL |= 1 << playerID;

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
                    self->pushPlayersR |= 1 << playerID;

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
void Platform_Collision_Solid_Hold(void)
{
    RSDK_THIS(Platform);

    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;

    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 playerID = RSDK.GetEntitySlot(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);

        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            case C_NONE:
                if ((1 << playerID) & stoodPlayers) {
                    if (player->state == Player_State_Static) {
                        player->state = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_AIR_WALK, &player->animator, false, 0);
                    }
                }
                break;

            case C_TOP:
#if MANIA_USE_PLUS
                if (player->characterID != ID_MIGHTY || player->state != Player_State_MightyHammerDrop) {
#endif

                    Platform_HandleStood_Hold(self, player, playerID, stoodPlayers);

                    if (self->velocity.y <= 0)
                        player->collisionFlagV |= 1;
#if MANIA_USE_PLUS
                }
#endif
                break;

            case C_LEFT:
                if (player->onGround && player->right)
                    self->pushPlayersL |= 1 << playerID;

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
                    self->pushPlayersR |= 1 << playerID;

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
void Platform_Collision_None(void)
{
    // hehe
}
void Platform_Collision_Solid_NoCrush(void)
{
    RSDK_THIS(Platform);

    Hitbox *solidHitbox    = RSDK.GetHitbox(&self->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&self->animator, 0);
    int32 stoodPlayers     = self->stoodPlayers;

    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 playerID = RSDK.GetEntitySlot(player);
        Player_CheckCollisionPlatform(player, self, platformHitbox);

        switch (Player_CheckCollisionBox(player, self, solidHitbox)) {
            default:
            case C_NONE: break;

            case C_TOP: Platform_HandleStood(self, player, playerID, stoodPlayers); break;

            case C_LEFT:
                if (player->onGround && player->right)
                    self->pushPlayersL |= 1 << playerID;

                if (self->velocity.x <= 0 && player->left) {
                    player->groundVel  = self->velocity.x;
                    player->velocity.x = player->groundVel;
                    player->velocity.x -= player->acceleration;
                }
                break;

            case C_RIGHT:
                if (player->onGround && player->left)
                    self->pushPlayersR |= 1 << playerID;

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

void Platform_HandleStood(EntityPlatform *self, EntityPlayer *player, int32 playerID, int32 stoodPlayers)
{
    self->stood = true;

    // if stoodPlayers is 0xFF thats the "ignore stoodPlayers" flag
    bool32 isStood = ((1 << playerID) & stoodPlayers) != 0;
    isStood &= stoodPlayers != 0xFF;

    if (!isStood && !player->sidekick && self->state == Platform_State_Fall && !self->timer) {
#if MANIA_USE_PLUS
        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
            self->timer = 1;
        else
#endif
            self->timer = 30;
    }

    self->stoodPlayers |= 1 << playerID;

    if (Platform->stoodPos[playerID].x) {
        player->position.x = Platform->stoodPos[playerID].x;
        player->position.y = Platform->stoodPos[playerID].y;
    }
    else {
        Platform->stoodPos[playerID].x = player->position.x;
        Platform->stoodPos[playerID].y = player->position.y;
    }

    player->position.x += self->collisionOffset.x;
    player->position.y += self->collisionOffset.y;
    player->position.y &= 0xFFFF0000;
}
void Platform_HandleStood_Tiles(EntityPlatform *self, EntityPlayer *player, int32 playerID)
{
    if (!player->sidekick) {
        self->stood = true;
        if (self->state == Platform_State_Fall && !self->timer) {
#if MANIA_USE_PLUS
            if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                self->timer = 1;
            else
#endif
                self->timer = 30;
        }
    }

    self->stoodPlayers |= 1 << playerID;

    player->position.x += self->collisionOffset.x;
    player->position.y += self->collisionOffset.y;
    player->position.y &= 0xFFFF0000;
}
void Platform_HandleStood_Hold(EntityPlatform *self, EntityPlayer *player, int32 playerID, int32 stoodPlayers)
{
    if (!((1 << playerID) & stoodPlayers)) {
        player->state           = Player_State_Static;
        player->nextGroundState = StateMachine_None;
        player->nextAirState    = StateMachine_None;
        player->velocity.x      = 0;
        player->velocity.y      = 0;
        player->groundVel       = 0;

        if (self->classID == Platform->classID)
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTER, &player->animator, false, 0);

        player->animator.speed = 64;
        player->direction      = FLIP_X;
        if (!player->sidekick) {
            self->stood = true;
        }
    }

    bool32 stoodStore = self->stood;
    Platform_HandleStood(self, player, playerID, stoodPlayers);
    self->stood = stoodStore;

    if ((player->position.x ^ self->position.x) & 0xFFFF0000) {
        if (player->position.x >= self->position.x)
            player->position.x -= TO_FIXED(1);
        else
            player->position.x += TO_FIXED(1);
    }

    if (player->jumpPress)
        Player_Action_Jump(player);
}
void Platform_HandleStood_Barrel(EntityPlatform *self, EntityPlayer *player, int32 playerID, int32 stoodPlayers)
{
    if (!((1 << playerID) & stoodPlayers)) {
        player->state           = Player_State_Static;
        player->nextGroundState = StateMachine_None;
        player->nextAirState    = StateMachine_None;
        player->velocity.x      = 0;
        player->velocity.y      = 0;
        player->groundVel       = 0;

        if (self->classID == Platform->classID)
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TURNTABLE, &player->animator, false, 0);

        player->animator.speed = 64;
        player->direction      = FLIP_NONE;
    }

    Platform_HandleStood(self, player, playerID, stoodPlayers);

    if ((player->position.x ^ self->position.x) & 0xFFFF0000) {
        if (player->position.x >= self->position.x)
            player->position.x -= TO_FIXED(1);
        else
            player->position.x += TO_FIXED(1);
    }

    if (player->jumpPress)
        Player_Action_Jump(player);
}
void Platform_HandleStood_Sticky(EntityPlatform *self, EntityPlayer *player, int32 playerID, uint8 cSide)
{

    bool32 isStuck = false;
    if ((self->collision == PLATFORM_C_SOLID_STICKY || self->collision == PLATFORM_C_SOLID_STICKY + cSide) && cSide)
        isStuck = true;

    if (player->state != Player_State_Static && isStuck) {
        player->state           = Player_State_Static;
        player->nextGroundState = StateMachine_None;
        player->nextAirState    = StateMachine_None;
        player->velocity.x      = 0;
        player->velocity.y      = 0;
        player->groundVel       = 0;
        player->animator.speed  = 0;

        switch (cSide) {
            case C_TOP: player->angle = 0x00; break;

            case C_LEFT:
                player->angle    = 0xC0;
                player->onGround = false;
                break;

            case C_RIGHT: player->angle = 0x40; break;

            case C_BOTTOM: player->angle = 0x80; break;

            default: break;
        }

        player->tileCollisions = TILECOLLISION_NONE;
        if (!player->sidekick) {
            self->stood = true;
            if (self->state == Platform_State_Fall && !self->timer) {
#if MANIA_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                    self->timer = 1;
                else
#endif
                    self->timer = 30;
            }
        }

        self->stoodPlayers |= 1 << playerID;
    }
    else if ((1 << playerID) & self->stoodPlayers) {
        if (player->state == Player_State_Static) {
            if (Platform->stoodPos[playerID].x) {
                player->position.x = Platform->stoodPos[playerID].x;
                player->position.y = Platform->stoodPos[playerID].y;
            }
            else {
                Platform->stoodPos[playerID].x = player->position.x;
                Platform->stoodPos[playerID].y = player->position.y;
            }

            player->position.x += self->collisionOffset.x;
            player->position.y += self->collisionOffset.y;
            player->position.y &= 0xFFFF0000;

            if (player->jumpPress) {
                player->tileCollisions = TILECOLLISION_DOWN;
                Player_Action_Jump(player);
            }
        }
        else {
            self->stoodPlayers &= ~(1 << playerID);
        }
    }
    else if (cSide == C_TOP) {
        if (Platform->stoodPos[playerID].x) {
            player->position.x = Platform->stoodPos[playerID].x;
            player->position.y = Platform->stoodPos[playerID].y;
        }
        else {
            Platform->stoodPos[playerID].x = player->position.x;
            Platform->stoodPos[playerID].y = player->position.y;
        }

        player->position.x += self->collisionOffset.x;
        player->position.y += self->collisionOffset.y;
        player->position.y &= 0xFFFF0000;
    }
}

#if RETRO_INCLUDE_EDITOR
void Platform_EditorDraw_Normal(void)
{
    RSDK_THIS(Platform);

    if (Platform->aniFrames == (uint16)-1 || self->frameID == -1)
        RSDK.DrawRect(self->drawPos.x - TO_FIXED(32), self->drawPos.y - TO_FIXED(16), TO_FIXED(64), TO_FIXED(32), 0x8080A0, self->alpha,
                      self->inkEffect, false);
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
        if (Platform->aniFrames == (uint16)-1 || self->frameID == -1)
            RSDK.DrawRect(drawPos.x - TO_FIXED(32), drawPos.y - TO_FIXED(16), TO_FIXED(64), TO_FIXED(32), 0x8080A0, self->alpha,
                          self->inkEffect, false);
        else
            RSDK.DrawSprite(&self->animator, &drawPos, false);
        angle += 0x400;
        self->direction ^= FLIP_X;
        self->rotation = rot;
    }

    self->drawFX           = fxStore;
    self->animator.frameID = frame + 2;
    if (Platform->aniFrames == (uint16)-1 || self->frameID == -1)
        RSDK.DrawRect(self->centerPos.x - TO_FIXED(32), self->centerPos.y - TO_FIXED(16), TO_FIXED(64), TO_FIXED(32), 0x8080A0, self->alpha,
                      self->inkEffect, false);
    else
        RSDK.DrawSprite(&self->animator, &self->centerPos, false);

    self->animator.frameID = frame;
    if (Platform->aniFrames == (uint16)-1 || self->frameID == -1)
        RSDK.DrawRect(self->drawPos.x - TO_FIXED(32), self->drawPos.y - TO_FIXED(16), TO_FIXED(64), TO_FIXED(32), 0x8080A0, self->alpha,
                      self->inkEffect, false);
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
        RSDK.SetSpriteAnimation(-1, 0, &self->animator, true, 0);
    }

    Vector2 amplitude;
    Vector2 drawPos;

    amplitude.x = self->amplitude.x >> 10;
    amplitude.y = self->amplitude.y >> 10;

    switch (self->type) {
        case PLATFORM_FIXED:
        default:
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);

            Platform_EditorDraw_Normal();
            break;

        case PLATFORM_FALL:
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = (abs(amplitude.y) + 0x2000) << 10;

            Platform_EditorDraw_Normal();
            break;

        case PLATFORM_LINEAR:
            self->updateRange.x = (abs(amplitude.x) + 0x2000) << 10;
            self->updateRange.y = (abs(amplitude.y) + 0x2000) << 10;

            Platform_EditorDraw_Normal();

            if (showGizmos() && (amplitude.x || amplitude.y)) {
                RSDK_DRAWING_OVERLAY(true);

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

                DrawHelpers_DrawArrow(drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, INK_NONE, 0xFF);
                DrawHelpers_DrawArrow(self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y, 0x00FF00, INK_NONE, 0xFF);

                self->inkEffect = INK_NONE;

                RSDK_DRAWING_OVERLAY(false);
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

            if (showGizmos() && !self->hasTension && (amplitude.x || amplitude.y)) {
                RSDK_DRAWING_OVERLAY(true);

                self->inkEffect = INK_BLEND;

                // start pos
                self->drawPos.x = amplitude.x * RSDK.Cos1024(4 * self->angle) + self->centerPos.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(4 * self->angle) + self->centerPos.y;
                Platform_EditorDraw_Normal();

                self->inkEffect = INK_NONE;

                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        case PLATFORM_PATH:
        case PLATFORM_PATH_REACT:
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);

            // PLATFORM_PATH_REACT activates control in prev slot
            // Note: will go back through platforms if not found
            // e.g. if entity is slot 3, and platform control is slot 0, with slot 1 and 2 being platform/platformNode, it'll work
            // if slot 1 or 2 ISNT a platform/platformNode, it will not work

            // self->speed = starting node

            Platform_EditorDraw_Normal();
            break;

        case PLATFORM_PUSH:
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);

            Platform_EditorDraw_Normal();
            break;

        case PLATFORM_REACT:
        case PLATFORM_HOVER_REACT:
            amplitude.x <<= 10;
            self->updateRange.x = TO_FIXED(128) + abs(amplitude.x);
            self->updateRange.y = TO_FIXED(128) + abs(amplitude.x);
            if (self->speed < 0)
                self->direction = FLIP_X;

            Platform_EditorDraw_Normal();

            if (showGizmos()) {
                RSDK_DRAWING_OVERLAY(true);

                drawPos           = self->drawPos;
                self->groundVel   = 0;
                bool32 reachedEnd = false;
                uint8 type        = 0;

                while (self->speed) {
                    if (!reachedEnd) {
                        amplitude.y += self->groundVel;

                        self->drawPos.x = (amplitude.y >> 8) * RSDK.Cos256(self->angle) + self->centerPos.x;
                        self->drawPos.y = (amplitude.y >> 8) * RSDK.Sin256(self->angle) + self->centerPos.y;

                        int32 speed16 = self->speed << 16;
                        if (self->groundVel == speed16) {
                            if (amplitude.y >= amplitude.x) {
                                amplitude.y = amplitude.x;
                                self->groundVel -= (self->speed << 11);
                                reachedEnd = true;
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
                DrawHelpers_DrawArrow(drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, INK_NONE, 0xFF);
                if (type)
                    DrawHelpers_DrawArrow(self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y, 0x00FF00, INK_NONE, 0xFF);

                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        case PLATFORM_DOORSLIDE:
            amplitude.x <<= 10;
            self->updateRange.x = TO_FIXED(128) + abs(amplitude.x);
            self->updateRange.y = TO_FIXED(128) + abs(amplitude.x);

            if (self->speed < 0)
                self->direction = FLIP_X;

            Platform_EditorDraw_Normal();

            if (showGizmos()) {
                RSDK_DRAWING_OVERLAY(true);

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

                DrawHelpers_DrawArrow(drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, INK_NONE, 0xFF);
                DrawHelpers_DrawArrow(self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y, 0x00FF00, INK_NONE, 0xFF);

                RSDK_DRAWING_OVERLAY(false);
            }

            break;

        case PLATFORM_SWING_REACT:
        case PLATFORM_SWING: {
            self->timer = 0;
            if (self->type == PLATFORM_SWING_REACT)
                self->timer = 88;

            self->updateRange.x = (abs(amplitude.y) + 0x200) << 14;
            self->updateRange.y = (abs(amplitude.y) + 0x200) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            amplitude.y <<= 4;
            if (self->type != PLATFORM_SWING) {
                if (self->groundVel >= 0)
                    self->drawPos.x -= TO_FIXED(32);
                else
                    self->drawPos.x += TO_FIXED(32);
            }

            int32 angle     = self->angle;
            self->groundVel = 4 * self->angle;
            self->angle     = self->groundVel + 0x100 + (self->amplitude.x * RSDK.Sin1024(0x000 + self->timer) >> 14);
            self->drawPos.x = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
            self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
            Platform_EditorDraw_Swinging(amplitude);

            if (showGizmos() && (amplitude.x || amplitude.y)) {
                RSDK_DRAWING_OVERLAY(true);

                if (self->type == PLATFORM_SWING) {
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

                RSDK_DRAWING_OVERLAY(false);
            }
            self->angle = angle;
            break;
        }

        case PLATFORM_TRACK_REACT:
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
                RSDK_DRAWING_OVERLAY(true);

                drawPos       = self->drawPos;
                self->drawPos = self->centerPos;

                self->inkEffect = INK_BLEND;

                Platform_EditorDraw_Normal();

                self->inkEffect = INK_NONE;

                DrawHelpers_DrawArrow(drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, INK_NONE, 0xFF);
                DrawHelpers_DrawArrow(self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y, 0x00FF00, INK_NONE, 0xFF);

                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        case PLATFORM_TRACK:
            self->updateRange.x = (abs(amplitude.x) + 0x4000) << 9;
            self->updateRange.y = (abs(amplitude.y) + 0x4000) << 9;
            if (self->speed < 0)
                self->direction = FLIP_X;

            Platform_EditorDraw_Normal();

            if (showGizmos() && (amplitude.x || amplitude.y)) {
                RSDK_DRAWING_OVERLAY(true);

                Vector2 storePos = self->drawPos;
                drawPos          = self->drawPos;

                uint8 finishDirs = 0;
                int8 last        = -1;
                int32 timer      = 0;
                while (finishDirs != 2) {
                    int32 move = timer++ * (self->speed << 7);

                    if (((move >> 16) & 1) == self->direction) {
                        if (last > 0)
                            finishDirs++;

                        if (finishDirs != 2) {
                            self->drawPos.x = self->centerPos.x + ((move & 0xFFFF) * amplitude.x >> 6) - (amplitude.x << 9);
                            self->drawPos.y = self->centerPos.y + ((move & 0xFFFF) * amplitude.y >> 6) - (amplitude.y << 9);
                            storePos        = self->drawPos;
                        }
                        last = 0;
                    }
                    else {
                        if (!last)
                            finishDirs++;

                        if (finishDirs != 2) {
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

                DrawHelpers_DrawArrow(drawPos.x, drawPos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, INK_NONE, 0xFF);
                DrawHelpers_DrawArrow(self->drawPos.x, self->drawPos.y, drawPos.x, drawPos.y, 0x00FF00, INK_NONE, 0xFF);

                RSDK_DRAWING_OVERLAY(false);
            }

            break;

        case PLATFORM_CLACKER: {
            self->updateRange.x = (abs(amplitude.y) + 0x200) << 14;
            self->updateRange.y = (abs(amplitude.y) + 0x200) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
            amplitude.y <<= 4;
            self->groundVel = 4 * self->angle;
            self->angle     = self->groundVel + 0x100 + (amplitude.x * RSDK.Sin1024(self->hasTension ? 0x100 : 0) >> 14);
            self->drawPos.x = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
            self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;

            Platform_EditorDraw_Swinging(amplitude);

            if (showGizmos() && (amplitude.x || amplitude.y)) {
                RSDK_DRAWING_OVERLAY(true);

                self->inkEffect = INK_BLEND;

                // max
                self->angle     = self->groundVel + 0x100 + (amplitude.x * RSDK.Sin1024(self->hasTension ? 0 : -0x100) >> 14);
                self->drawPos.x = amplitude.y * RSDK.Cos1024(self->angle) + self->centerPos.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(self->angle) + self->centerPos.y;
                Platform_EditorDraw_Swinging(amplitude);

                self->inkEffect = INK_NONE;
                self->angle     = self->groundVel / 4;

                RSDK_DRAWING_OVERLAY(false);
            }
            break;
        }

        case PLATFORM_CHILD:
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);

            Platform_EditorDraw_Normal();
            break;

        case PLATFORM_DIPROCK:
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = (abs(amplitude.y) + 0x2000) << 10;

            Platform_EditorDraw_Normal();

            if (showGizmos() && (amplitude.x || amplitude.y)) {
                RSDK_DRAWING_OVERLAY(true);

                self->inkEffect = INK_BLEND;

                // max
                self->drawPos.y += amplitude.y;
                Platform_EditorDraw_Normal();

                DrawHelpers_DrawArrow(self->drawPos.x, self->drawPos.y - amplitude.y, self->drawPos.x, self->drawPos.y, 0x00FF00, INK_NONE, 0xFF);

                self->inkEffect = INK_NONE;

                RSDK_DRAWING_OVERLAY(false);
            }
            break;
    }

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Platform_EditorLoad(void)
{
    Platform_StageLoad();

    RSDK_ACTIVE_VAR(Platform, type);
    RSDK_ENUM_VAR("Fixed", PLATFORM_FIXED);
    RSDK_ENUM_VAR("Fall", PLATFORM_FALL);
    RSDK_ENUM_VAR("Linear", PLATFORM_LINEAR);
    RSDK_ENUM_VAR("Circlular", PLATFORM_CIRCULAR);
    RSDK_ENUM_VAR("Swing", PLATFORM_SWING);
    RSDK_ENUM_VAR("Path", PLATFORM_PATH);
    RSDK_ENUM_VAR("Push", PLATFORM_PUSH);
    RSDK_ENUM_VAR("Track", PLATFORM_TRACK);
    RSDK_ENUM_VAR("React", PLATFORM_REACT);
    RSDK_ENUM_VAR("Hover-React", PLATFORM_HOVER_REACT);
    RSDK_ENUM_VAR("Door Slide", PLATFORM_DOORSLIDE);
    RSDK_ENUM_VAR("Path-React", PLATFORM_PATH_REACT);
    RSDK_ENUM_VAR("Swing-React", PLATFORM_SWING_REACT);
    RSDK_ENUM_VAR("Track-React", PLATFORM_TRACK_REACT);
    RSDK_ENUM_VAR("Clacker", PLATFORM_CLACKER);
    RSDK_ENUM_VAR("Child", PLATFORM_CHILD);
    RSDK_ENUM_VAR("Dip Rock", PLATFORM_DIPROCK);

    RSDK_ACTIVE_VAR(Platform, collision);
    RSDK_ENUM_VAR("Platform", PLATFORM_C_PLATFORM);
    RSDK_ENUM_VAR("Solid", PLATFORM_C_SOLID);
    RSDK_ENUM_VAR("Tiled", PLATFORM_C_TILED);
    RSDK_ENUM_VAR("Hurt", PLATFORM_C_HURT);
    RSDK_ENUM_VAR("None", PLATFORM_C_NONE);
    RSDK_ENUM_VAR("Solid Hurt Sides", PLATFORM_C_SOLID_HURT_SIDES);
    RSDK_ENUM_VAR("Solid Hurt Bottom", PLATFORM_C_SOLID_HURT_BOTTOM);
    RSDK_ENUM_VAR("Solid Hurt Top", PLATFORM_C_SOLID_HURT_TOP);
    RSDK_ENUM_VAR("Solid Hold", PLATFORM_C_SOLID_HOLD);
    RSDK_ENUM_VAR("Solid Sticky", PLATFORM_C_SOLID_STICKY);
    RSDK_ENUM_VAR("Sticky Top", PLATFORM_C_STICKY_TOP);
    RSDK_ENUM_VAR("Sticky Left", PLATFORM_C_STICKY_LEFT);
    RSDK_ENUM_VAR("Sticky Right", PLATFORM_C_STICKY_RIGHT);
    RSDK_ENUM_VAR("Sticky Bottom", PLATFORM_C_STICKY_BOTTOM);
    RSDK_ENUM_VAR("Solid Barrel", PLATFORM_C_SOLID_BARREL);
    RSDK_ENUM_VAR("Solid No Crush", PLATFORM_C_SOLID_NOCRUSH);
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
