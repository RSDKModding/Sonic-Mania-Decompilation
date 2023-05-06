// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PopcornMachine Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPopcornMachine *PopcornMachine;

void PopcornMachine_Update(void)
{
    RSDK_THIS(PopcornMachine);
    StateMachine_Run(self->state);
}

void PopcornMachine_LateUpdate(void) {}

void PopcornMachine_StaticUpdate(void) {}

void PopcornMachine_Draw(void)
{
    RSDK_THIS(PopcornMachine);
    Vector2 drawPos;

    // Draw Dispenser
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->dispenserPos.x;
    drawPos.y              = drawPos.y - 0x200000 + self->dispenserPos.y;
    self->animator.frameID = 9;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Draw Dispenser Holder
    drawPos = self->position;
    drawPos.x += self->dispenserHolderPos.x;
    drawPos.y              = drawPos.y - 0x200000 + self->dispenserHolderPos.y;
    self->animator.frameID = 10;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Draw Sign
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    // Draw Top
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);

    // Draw Entries
    switch (self->type) {
        case POPCORNMACHINE_LEFT:
            self->animator.frameID = 2;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case POPCORNMACHINE_RIGHT:
            self->animator.frameID = 3;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case POPCORNMACHINE_BOTH:
            self->animator.frameID = 2;
            RSDK.DrawSprite(&self->animator, NULL, false);

            self->animator.frameID = 3;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
    }

    // Draw Top
    drawPos = self->position;
    drawPos.y += -0xD00000 - 0xA00000 * self->height;
    int32 storeY1          = drawPos.y;
    int32 storeY2          = self->position.y - 0x300000;
    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 5;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Draw (Initial) Machine Glass Edges
    self->animator.frameID = 6;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 7;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Draw Machine Glass Edges
    for (int32 y = 0; y < self->height; ++y) {
        drawPos.y += 0xA00000;
        self->animator.frameID = 6;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        self->animator.frameID = 7;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }

    // Draw City Reflection FX
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    self->inkEffect        = INK_ADD;
    self->animator.frameID = 8;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, (storeY1 >> 16) - screen->position.y, screen->size.x, (storeY2 >> 16) - screen->position.y + 1);

    drawPos = self->position;
    drawPos.x -= 0x600000;
    drawPos.y = (self->position.y + ((screen->center.y - 160 + screen->position.y) << 16)) >> 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += 0xC00000;
    drawPos.y       = (self->position.y + ((screen->center.y - 384 + screen->position.y) << 16)) >> 1;
    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->direction = FLIP_NONE;
    self->inkEffect = INK_NONE;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->size.x, screen->size.y);
}

void PopcornMachine_Create(void *data)
{
    RSDK_THIS(PopcornMachine);

    if (!SceneInfo->inEditor) {
        if (data) {
            self->active  = ACTIVE_NORMAL;
            self->visible = false;
            self->player  = (EntityPlayer *)data;
            self->state   = PopcornMachine_StateController_ReadyPlayer;
        }
        else {
            self->active               = ACTIVE_BOUNDS;
            self->updateRange.x        = 0xB00000;
            self->visible              = true;
            self->updateRange.y        = 0xA00000 * self->height + 0x1000000;
            self->drawGroup            = Zone->objectDrawGroup[1];
            self->drawFX               = FX_FLIP;
            self->alpha                = 0xE0;
            self->dispenserHolderPos.y = -0x100000;
            self->dispenserPos.y       = -0x100000;

            int32 top                = -0x100 - 0xA0 * self->height;
            self->hitboxSideL.top    = top;
            self->hitboxSideL.left   = -128;
            self->hitboxSideL.right  = -120;
            self->hitboxSideL.bottom = -32;

            self->hitboxSideR.left   = 120;
            self->hitboxSideR.top    = top;
            self->hitboxSideR.right  = 128;
            self->hitboxSideR.bottom = -32;

            self->hitboxTop.left   = -128;
            self->hitboxTop.top    = top;
            self->hitboxTop.right  = 128;
            self->hitboxTop.bottom = top + 16;

            self->state = PopcornMachine_State_Idle;
            RSDK.SetSpriteAnimation(PopcornMachine->aniFrames, 0, &self->animator, true, 0);
        }
    }
}

void PopcornMachine_StageLoad(void)
{
    PopcornMachine->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/PopcornMachine.bin", SCOPE_STAGE);

    PopcornMachine->hitboxEntryL.left   = -176;
    PopcornMachine->hitboxEntryL.right  = -128;
    PopcornMachine->hitboxEntryL.top    = -16;
    PopcornMachine->hitboxEntryL.bottom = 24;

    PopcornMachine->hitboxEntryR.left   = 128;
    PopcornMachine->hitboxEntryR.right  = 176;
    PopcornMachine->hitboxEntryR.top    = -16;
    PopcornMachine->hitboxEntryR.bottom = 24;

    PopcornMachine->sfxFanStart      = RSDK.GetSfx("Stage/FanStart.wav");
    PopcornMachine->sfxShoot         = RSDK.GetSfx("Stage/Shoot1.wav");
    PopcornMachine->sfxPopcornLaunch = RSDK.GetSfx("Stage/PopcornLaunch.wav");
}

void PopcornMachine_LinkPlayer(EntityPlayer *player)
{
    RSDK_THIS(PopcornMachine);

    player->nextAirState    = StateMachine_None;
    player->nextGroundState = StateMachine_None;
    player->state           = Player_State_Static;
    player->onGround        = false;
    player->velocity.x      = player->position.x < self->position.x ? 0xA0000 : -0xA0000;
    player->tileCollisions  = TILECOLLISION_NONE;
    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

    EntityPopcornMachine *machine = CREATE_ENTITY(PopcornMachine, player, self->position.x, self->position.y);
    machine->isPermanent          = true;
    machine->parent               = self;
    if (!player->sidekick)
        machine->bounds[player->playerID] = Zone->cameraBoundsB[player->playerID];
}
void PopcornMachine_CheckPlayerCollisions(void)
{
    RSDK_THIS(PopcornMachine);

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &self->hitboxSideL);
        Player_CheckCollisionBox(player, self, &self->hitboxSideR);
        Player_CheckCollisionPlatform(player, self, &self->hitboxTop);

        if (player->state != Player_State_Static) {

            switch (self->type) {
                case POPCORNMACHINE_LEFT:
                    if (player->onGround && Player_CheckCollisionTouch(player, self, &PopcornMachine->hitboxEntryL)) {
                        PopcornMachine_LinkPlayer(player);
                    }
                    else if (!player->onGround) {
                        Player_CheckCollisionBox(player, self, &PopcornMachine->hitboxEntryL);
                    }
                    break;

                case POPCORNMACHINE_RIGHT:
                    if (player->onGround && Player_CheckCollisionTouch(player, self, &PopcornMachine->hitboxEntryL)) {
                        PopcornMachine_LinkPlayer(player);
                    }
                    else if (!player->onGround) {
                        Player_CheckCollisionBox(player, self, &PopcornMachine->hitboxEntryR);
                    }
                    break;

                case POPCORNMACHINE_BOTH:
                    if (player->onGround) {
                        if (Player_CheckCollisionTouch(player, self, &PopcornMachine->hitboxEntryL)) {
                            PopcornMachine_LinkPlayer(player);
                        }
                        else {
                            if (Player_CheckCollisionTouch(player, self, &PopcornMachine->hitboxEntryR))
                                PopcornMachine_LinkPlayer(player);
                        }
                    }
                    else {
                        Player_CheckCollisionBox(player, self, &PopcornMachine->hitboxEntryL);
                        Player_CheckCollisionBox(player, self, &PopcornMachine->hitboxEntryR);
                    }
                    break;
            }
        }
    }
}

void PopcornMachine_State_Idle(void) { PopcornMachine_CheckPlayerCollisions(); }

void PopcornMachine_State_ShowDispenser(void)
{
    RSDK_THIS(PopcornMachine);

    PopcornMachine_CheckPlayerCollisions();

    self->dispenserHolderPos.y -= 0x8000;
    if (self->dispenserHolderPos.y <= -0x200000)
        self->dispenserHolderPos.y = -0x200000;

    self->dispenserPos.y -= 0x18000;
    if (self->dispenserPos.y <= -0x400000) {
        self->dispenserPos.y = -0x400000;
        self->timer          = 0;
        self->state          = PopcornMachine_State_Shaking;
    }
}

void PopcornMachine_State_Shaking(void)
{
    RSDK_THIS(PopcornMachine);

    PopcornMachine_CheckPlayerCollisions();

    self->dispenserHolderPos.x = RSDK.Rand(-0x20000, 0x20000);
    self->dispenserHolderPos.y = RSDK.Rand(-0x20000, 0x20000) - 0x1E0000;

    self->dispenserPos.x = RSDK.Rand(-0x20000, 0x20000);
    self->dispenserPos.y = self->dispenserHolderPos.y - 0x1E0000 + RSDK.Rand(-0x20000, 0x20000);

    self->timer++;
    if (self->timer >= 25 && self->timer < 160) {
        if (!(self->timer & 1)) {
            EntityPopcornKernel *kernel = CREATE_ENTITY(PopcornKernel, NULL, self->position.x, self->position.y - 0x400000);
            kernel->velocity.x          = RSDK.Rand(-0x30000, 0x30000);
            kernel->velocity.y          = RSDK.Rand(-0xC0000, -0x60000);
            kernel->angleVel            = RSDK.Rand(-0x60000, 0x60000);
            kernel->bounds.x            = self->position.x;
            kernel->bounds.y            = self->position.y;
        }

        if (!(self->timer & 7))
            RSDK.PlaySfx(PopcornMachine->sfxShoot, false, 255);
    }

    if (self->timer == 160) {
        foreach_active(PopcornKernel, kernel)
        {
            kernel->state      = PopcornKernel_State_RiseUp;
            kernel->velocity.x = 0;
            kernel->velocity.y = RSDK.Rand(-0xA0000, -0x60000);
            kernel->maxY       = self->position.y - (0xA00000 * self->height) - 0xD00000;
        }

        RSDK.PlaySfx(PopcornMachine->sfxPopcornLaunch, false, 255);
        self->state = PopcornMachine_State_HideDispenser;
    }
}

void PopcornMachine_State_HideDispenser(void)
{
    RSDK_THIS(PopcornMachine);

    PopcornMachine_CheckPlayerCollisions();

    self->dispenserHolderPos.y += 0x8000;
    if (self->dispenserHolderPos.y >= 0)
        self->dispenserHolderPos.y = 0;

    self->dispenserPos.y += 0x18000;
    if (self->dispenserPos.y >= 0) {
        self->dispenserPos.y = 0;
        self->state          = PopcornMachine_State_Idle;
    }
}

void PopcornMachine_StateController_ReadyPlayer(void)
{
    RSDK_THIS(PopcornMachine);

    EntityPopcornMachine *parent = self->parent;
    EntityPlayer *player         = self->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < self->position.y - 0x300000) {
            player->position.y = self->position.y - 0x300000;
            player->velocity.y = 0;
            self->state        = PopcornMachine_StateController_FirePlayer;
        }

        if (!Player_CheckValidState(player)) {
            if (!player->sidekick)
                Zone->cameraBoundsB[player->playerID] = self->bounds[player->playerID];

            destroyEntity(self);
        }
    }
    else {
        destroyEntity(self);
    }

    if (abs(self->position.x - player->position.x) < 0x100000) {
        player->position.x = self->position.x;
        player->groundVel  = 0;
        player->velocity.x = 0;
        player->velocity.y = -0x80000;

        if (!player->sidekick)
            Zone->cameraBoundsB[player->playerID] = (self->position.y >> 16) + 32;

        if (parent->state == PopcornMachine_State_Idle || parent->state == PopcornMachine_State_HideDispenser) {
            parent->state = PopcornMachine_State_ShowDispenser;
            RSDK.PlaySfx(PopcornMachine->sfxFanStart, false, 255);
        }

        self->state = PopcornMachine_StateController_RisePlayer;
    }

    if (!Player_CheckValidState(player)) {
        if (!player->sidekick)
            Zone->cameraBoundsB[player->playerID] = self->bounds[player->playerID];

        destroyEntity(self);
    }
}

void PopcornMachine_StateController_RisePlayer(void)
{
    RSDK_THIS(PopcornMachine);

    EntityPlayer *player = self->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < self->position.y - 0x300000) {
            player->position.y = self->position.y - 0x300000;
            player->velocity.y = 0;
            self->state        = PopcornMachine_StateController_FirePlayer;
        }

        if (!Player_CheckValidState(player)) {
            if (!player->sidekick)
                Zone->cameraBoundsB[player->playerID] = self->bounds[player->playerID];

            destroyEntity(self);
        }
    }
    else {
        destroyEntity(self);
    }
}

void PopcornMachine_StateController_FirePlayer(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = self->parent;
    EntityPlayer *player         = self->player;

    if (Player_CheckValidState(player)) {
        if (parent->state == PopcornMachine_State_HideDispenser) {
            player->state           = Player_State_Air;
            player->gravityStrength = 0x80;
            player->velocity.y      = -0xD8000 - (parent->height << 16);
            player->position.y += player->velocity.y;

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
            if (!player->sidekick)
                Zone->cameraBoundsB[player->playerID] = self->bounds[player->playerID];

            self->state = PopcornMachine_StateController_HandleFinish;
        }
    }
    else {
        destroyEntity(self);
    }
}

void PopcornMachine_StateController_HandleFinish(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = self->parent;
    EntityPlayer *player         = self->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < parent->position.y - 0xA00000 * parent->height + 0xD00000) {
            player->tileCollisions = TILECOLLISION_DOWN;
            Player_UpdatePhysicsState(player);
            destroyEntity(self);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void PopcornMachine_EditorDraw(void)
{
    RSDK_THIS(PopcornMachine);
    self->active               = ACTIVE_BOUNDS;
    self->updateRange.x        = 0xB00000;
    self->visible              = true;
    self->updateRange.y        = 0xA00000 * self->height + 0x1000000;
    self->drawGroup            = Zone->objectDrawGroup[1];
    self->drawFX               = FX_FLIP;
    self->alpha                = 0xE0;
    self->dispenserHolderPos.y = -0x100000;
    self->dispenserPos.y       = -0x100000;
    RSDK.SetSpriteAnimation(PopcornMachine->aniFrames, 0, &self->animator, true, 0);

    // Draw !!
    Vector2 drawPos;

    // Draw Dispenser
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->dispenserPos.x;
    drawPos.y              = drawPos.y - 0x200000 + self->dispenserPos.y;
    self->animator.frameID = 9;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Draw Dispenser Holder
    drawPos = self->position;
    drawPos.x += self->dispenserHolderPos.x;
    drawPos.y              = drawPos.y - 0x200000 + self->dispenserHolderPos.y;
    self->animator.frameID = 10;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Draw Sign
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    // Draw Top
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);

    // Draw Entries
    switch (self->type) {
        case POPCORNMACHINE_LEFT:
            self->animator.frameID = 2;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case POPCORNMACHINE_RIGHT:
            self->animator.frameID = 3;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case POPCORNMACHINE_BOTH:
            self->animator.frameID = 2;
            RSDK.DrawSprite(&self->animator, NULL, false);

            self->animator.frameID = 3;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
    }

    // Draw Top
    drawPos = self->position;
    drawPos.y += -0xD00000 - 0xA00000 * self->height;
    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 5;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Draw (Initial) Machine Glass Edges
    self->animator.frameID = 6;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 7;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Draw Machine Glass Edges
    for (int32 y = 0; y < self->height; ++y) {
        drawPos.y += 0xA00000;
        self->animator.frameID = 6;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        self->animator.frameID = 7;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }
}

void PopcornMachine_EditorLoad(void)
{
    PopcornMachine->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/PopcornMachine.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PopcornMachine, type);
    RSDK_ENUM_VAR("Left Entry", POPCORNMACHINE_LEFT);
    RSDK_ENUM_VAR("Right Entry", POPCORNMACHINE_RIGHT);
    RSDK_ENUM_VAR("Double Entry", POPCORNMACHINE_BOTH);
}
#endif

void PopcornMachine_Serialize(void)
{
    RSDK_EDITABLE_VAR(PopcornMachine, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(PopcornMachine, VAR_UINT8, height);
}
