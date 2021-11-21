#include "SonicMania.h"

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

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->field_84;
    drawPos.y                = drawPos.y - 0x200000 + self->field_88;
    self->animator.frameID = 9;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos = self->position;
    drawPos.x += self->field_7C;
    drawPos.y                = drawPos.y - 0x200000 + self->field_80;
    self->animator.frameID = 10;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);

    switch (self->type) {
        case 0:
            self->animator.frameID = 2;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case 1:
            self->animator.frameID = 3;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case 2:
            self->animator.frameID = 2;
            RSDK.DrawSprite(&self->animator, NULL, false);

            self->animator.frameID = 3;
            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
    }

    drawPos = self->position;
    drawPos.y += -0xD00000 - 0xA00000 * self->height;
    int storeY1              = drawPos.y;
    int storeY2              = self->position.y - 0x300000;
    self->animator.frameID = 4;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 5;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 6;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 7;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    for (int y = 0; y < self->height; ++y) {
        drawPos.y += 0xA00000;
        self->animator.frameID = 6;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        self->animator.frameID = 7;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }

    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    self->inkEffect        = INK_ADD;
    self->animator.frameID = 8;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, (storeY1 >> 16) - screen->position.y, screen->width,
                       (storeY2 >> 16) - screen->position.y + 1);
    drawPos = self->position;
    drawPos.x -= 0x600000;
    drawPos.y = (self->position.y + ((screen->centerY - 160 + screen->position.y) << 16)) >> 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += 0xC00000;
    drawPos.y         = (self->position.y + ((screen->centerY - 384 + screen->position.y) << 16)) >> 1;
    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->direction = FLIP_NONE;
    self->inkEffect = INK_NONE;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
}

void PopcornMachine_Create(void *data)
{
    RSDK_THIS(PopcornMachine);

    if (!SceneInfo->inEditor) {
        if (data) {
            self->active  = ACTIVE_NORMAL;
            self->visible = false;
            self->player  = (EntityPlayer *)data;
            self->state   = PopcornMachine_State2_Unknown1;
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0xB00000;
            self->visible       = true;
            self->updateRange.y = 0xA00000 * self->height + 0x1000000;
            self->drawOrder     = Zone->drawOrderHigh;
            self->drawFX        = FX_FLIP;
            self->alpha         = 0xE0;
            self->field_80      = -0x100000;
            self->field_88      = -0x100000;

            int top                = -0x100 - 0xA0 * self->height;
            self->hitbox1.top    = top;
            self->hitbox1.left   = -128;
            self->hitbox1.right  = -120;
            self->hitbox1.bottom = -32;

            self->hitbox2.left   = 120;
            self->hitbox2.top    = top;
            self->hitbox2.right  = 128;
            self->hitbox2.bottom = -32;

            self->hitbox3.left   = -128;
            self->hitbox3.top    = top;
            self->hitbox3.right  = 128;
            self->hitbox3.bottom = top + 16;
            self->state          = PopcornMachine_State_Unknown1;
            RSDK.SetSpriteAnimation(PopcornMachine->aniFrames, 0, &self->animator, true, 0);
        }
    }
}

void PopcornMachine_StageLoad(void)
{
    PopcornMachine->aniFrames        = RSDK.LoadSpriteAnimation("SPZ1/PopcornMachine.bin", SCOPE_STAGE);
    PopcornMachine->hitbox1.left     = -176;
    PopcornMachine->hitbox1.right    = -128;
    PopcornMachine->hitbox1.top      = -16;
    PopcornMachine->hitbox1.bottom   = 24;
    PopcornMachine->hitbox2.left     = 128;
    PopcornMachine->hitbox2.right    = 176;
    PopcornMachine->hitbox2.top      = -16;
    PopcornMachine->hitbox2.bottom   = 24;
    PopcornMachine->sfxFanStart      = RSDK.GetSfx("Stage/FanStart.wav");
    PopcornMachine->sfxShoot         = RSDK.GetSfx("Stage/Shoot1.wav");
    PopcornMachine->sfxPopcornLaunch = RSDK.GetSfx("Stage/PopcornLaunch.wav");
}

void PopcornMachine_LinkPlayer(EntityPlayer *player)
{
    RSDK_THIS(PopcornMachine);

    player->nextAirState    = StateMachine_None;
    player->nextGroundState = StateMachine_None;
    player->state           = Player_State_None;
    player->onGround        = false;
    if (player->position.x < self->position.x)
        player->velocity.x = 0xA0000;
    else
        player->velocity.x = -0xA0000;
    player->tileCollisions = false;
    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

    EntityPopcornMachine *machine = CREATE_ENTITY(PopcornMachine, player, self->position.x, self->position.y);
    machine->isPermanent          = true;
    machine->parent               = (Entity *)self;
    if (!player->sidekick)
        machine->bounds[player->playerID] = Zone->cameraBoundsB[player->playerID];
}
void PopcornMachine_CheckPlayerCollisions(void)
{
    RSDK_THIS(PopcornMachine);

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &self->hitbox1);
        Player_CheckCollisionBox(player, self, &self->hitbox2);
        Player_CheckCollisionPlatform(player, self, &self->hitbox3);
        if (player->state != Player_State_None) {

            switch (self->type) {
                case 0:
                    if (player->onGround) {
                        if (Player_CheckCollisionTouch(player, self, &PopcornMachine->hitbox1)) {
                            PopcornMachine_LinkPlayer(player);
                        }
                    }
                    else {
                        Player_CheckCollisionBox(player, self, &PopcornMachine->hitbox1);
                    }
                    break;
                case 1:
                    if (player->onGround) {
                        if (Player_CheckCollisionTouch(player, self, &PopcornMachine->hitbox1)) {
                            PopcornMachine_LinkPlayer(player);
                        }
                    }
                    else {
                        Player_CheckCollisionBox(player, self, &PopcornMachine->hitbox2);
                    }
                    break;
                case 2:
                    if (player->onGround) {
                        if (Player_CheckCollisionTouch(player, self, &PopcornMachine->hitbox1)) {
                            PopcornMachine_LinkPlayer(player);
                        }
                        else {
                            if (Player_CheckCollisionTouch(player, self, &PopcornMachine->hitbox2))
                                PopcornMachine_LinkPlayer(player);
                        }
                    }
                    else {
                        Player_CheckCollisionBox(player, self, &PopcornMachine->hitbox1);
                        Player_CheckCollisionBox(player, self, &PopcornMachine->hitbox2);
                    }
                    break;
            }
        }
    }
}

void PopcornMachine_State_Unknown1(void) { PopcornMachine_CheckPlayerCollisions(); }

void PopcornMachine_State_Unknown2(void)
{
    RSDK_THIS(PopcornMachine);

    PopcornMachine_CheckPlayerCollisions();
    self->field_80 -= 0x8000;
    if (self->field_80 <= -0x200000)
        self->field_80 = -0x200000;
    self->field_88 -= 0x18000;
    if (self->field_88 <= -0x400000) {
        self->field_88 = -0x400000;
        self->field_60 = 0;
        self->state    = PopcornMachine_State_Unknown3;
    }
}

void PopcornMachine_State_Unknown4(void)
{
    RSDK_THIS(PopcornMachine);

    PopcornMachine_CheckPlayerCollisions();

    self->field_80 += 0x8000;
    if (self->field_80 >= 0)
        self->field_80 = 0;
    self->field_88 += 0x18000;
    if (self->field_88 >= 0) {
        self->field_88 = 0;
        self->state    = PopcornMachine_State_Unknown1;
    }
}

void PopcornMachine_State_Unknown3(void)
{
    RSDK_THIS(PopcornMachine);

    PopcornMachine_CheckPlayerCollisions();
    self->field_7C = RSDK.Rand(-0x20000, 0x20000);
    self->field_80 = RSDK.Rand(-0x20000, 0x20000) - 0x1E0000;
    self->field_84 = RSDK.Rand(-0x20000, 0x20000);
    self->field_88 = self->field_80 - 0x1E0000 + RSDK.Rand(-0x20000, 0x20000);
    self->field_60++;
    if ((uint32)(self->field_60 - 25) <= 0x86) {
        if (!(self->field_60 & 1)) {
            EntityPopcornKernel *kernel = CREATE_ENTITY(PopcornKernel, NULL, self->position.x, self->position.y - 0x400000);
            kernel->velocity.x          = RSDK.Rand(-0x30000, 0x30000);
            kernel->velocity.y          = RSDK.Rand(-0xC0000, -0x60000);
            kernel->angleVel            = RSDK.Rand(-0x60000, 0x60000);
            kernel->bounds.x            = self->position.x;
            kernel->bounds.y            = self->position.y;
        }
        if (!(self->field_60 & 7))
            RSDK.PlaySfx(PopcornMachine->sfxShoot, false, 255);
    }

    if (self->field_60 == 160) {
        int i = self->position.y - (0xA00000 * self->height) - 0xD00000;
        foreach_active(PopcornKernel, kernel)
        {
            kernel->state      = PopcornKernel_Unknown2;
            kernel->velocity.x = 0;
            kernel->velocity.y = RSDK.Rand(-0xA0000, -0x60000);
            kernel->field_68   = self->position.y - (0xA00000 * self->height) - 0xD00000;
        }
        RSDK.PlaySfx(PopcornMachine->sfxPopcornLaunch, false, 255);
        self->state = PopcornMachine_State_Unknown4;
    }
}

void PopcornMachine_State2_Unknown1(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = (EntityPopcornMachine *)self->parent;
    EntityPlayer *player         = self->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < self->position.y - 0x300000) {
            player->position.y = self->position.y - 0x300000;
            player->velocity.y = 0;
            self->state      = PopcornMachine_State2_Unknown3;
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

        if (parent->state == PopcornMachine_State_Unknown1 || parent->state == PopcornMachine_State_Unknown4) {
            parent->state = PopcornMachine_State_Unknown2;
            RSDK.PlaySfx(PopcornMachine->sfxFanStart, false, 255);
        }
        self->state = PopcornMachine_State2_Unknown2;
    }

    if (!Player_CheckValidState(player)) {
        if (!player->sidekick)
            Zone->cameraBoundsB[player->playerID] = self->bounds[player->playerID];
        destroyEntity(self);
    }
}

void PopcornMachine_State2_Unknown2(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = (EntityPopcornMachine *)self->parent;
    EntityPlayer *player         = self->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < self->position.y - 0x300000) {
            player->position.y = self->position.y - 0x300000;
            player->velocity.y = 0;
            self->state      = PopcornMachine_State2_Unknown3;
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

void PopcornMachine_State2_Unknown3(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = (EntityPopcornMachine *)self->parent;
    EntityPlayer *player         = self->player;

    if (Player_CheckValidState(player)) {
        if (parent->state == PopcornMachine_State_Unknown4) {
            player->state           = Player_State_Air;
            player->gravityStrength = 0x80;
            player->velocity.y      = -0xD8000 - (parent->height << 16);
            player->position.y += player->velocity.y;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
            if (!player->sidekick)
                Zone->cameraBoundsB[player->playerID] = self->bounds[player->playerID];
            self->state = PopcornMachine_State2_Unknown4;
        }
    }
    else {
        destroyEntity(self);
    }
}

void PopcornMachine_State2_Unknown4(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = (EntityPopcornMachine *)self->parent;
    EntityPlayer *player         = self->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < parent->position.y - 0xA00000 * parent->height + 0xD00000) {
            player->tileCollisions = true;
            Player_ChangePhysicsState(player);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void PopcornMachine_EditorDraw(void)
{
    RSDK_THIS(PopcornMachine);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0xB00000;
    self->visible       = true;
    self->updateRange.y = 0xA00000 * self->height + 0x1000000;
    self->drawOrder     = Zone->drawOrderHigh;
    self->drawFX        = FX_FLIP;
    self->alpha         = 0xE0;
    self->field_80      = -0x100000;
    self->field_88      = -0x100000;
    RSDK.SetSpriteAnimation(PopcornMachine->aniFrames, 0, &self->animator, true, 0);

    PopcornMachine_Draw();
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
