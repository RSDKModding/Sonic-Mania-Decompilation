#include "SonicMania.h"

ObjectPopcornMachine *PopcornMachine;

void PopcornMachine_Update(void)
{
    RSDK_THIS(PopcornMachine);
    StateMachine_Run(entity->state);
}

void PopcornMachine_LateUpdate(void) {}

void PopcornMachine_StaticUpdate(void) {}

void PopcornMachine_Draw(void)
{
    RSDK_THIS(PopcornMachine);
    Vector2 drawPos;

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += entity->field_84;
    drawPos.y                = drawPos.y - 0x200000 + entity->field_88;
    entity->animator.frameID = 9;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos = entity->position;
    drawPos.x += entity->field_7C;
    drawPos.y                = drawPos.y - 0x200000 + entity->field_80;
    entity->animator.frameID = 10;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, NULL, false);

    switch (entity->type) {
        case 0:
            entity->animator.frameID = 2;
            RSDK.DrawSprite(&entity->animator, NULL, false);
            break;
        case 1:
            entity->animator.frameID = 3;
            RSDK.DrawSprite(&entity->animator, NULL, false);
            break;
        case 2:
            entity->animator.frameID = 2;
            RSDK.DrawSprite(&entity->animator, NULL, false);

            entity->animator.frameID = 3;
            RSDK.DrawSprite(&entity->animator, NULL, false);
            break;
    }

    drawPos = entity->position;
    drawPos.y += -0xD00000 - 0xA00000 * entity->height;
    int storeY1              = drawPos.y;
    int storeY2              = entity->position.y - 0x300000;
    entity->animator.frameID = 4;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 5;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 6;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 7;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    for (int y = 0; y < entity->height; ++y) {
        drawPos.y += 0xA00000;
        entity->animator.frameID = 6;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        entity->animator.frameID = 7;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }

    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];

    entity->inkEffect        = INK_ADD;
    entity->animator.frameID = 8;
    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, (storeY1 >> 16) - screen->position.y, screen->width,
                       (storeY2 >> 16) - screen->position.y + 1);
    drawPos = entity->position;
    drawPos.x -= 0x600000;
    drawPos.y = (entity->position.y + ((screen->centerY - 160 + screen->position.y) << 16)) >> 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x += 0xC00000;
    drawPos.y         = (entity->position.y + ((screen->centerY - 384 + screen->position.y) << 16)) >> 1;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->direction = FLIP_NONE;
    entity->inkEffect = INK_NONE;
    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
}

void PopcornMachine_Create(void *data)
{
    RSDK_THIS(PopcornMachine);

    if (!RSDK_sceneInfo->inEditor) {
        if (data) {
            entity->active  = ACTIVE_NORMAL;
            entity->visible = false;
            entity->player  = (EntityPlayer *)data;
            entity->state   = PopcornMachine_State2_Unknown1;
        }
        else {
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0xB00000;
            entity->visible       = true;
            entity->updateRange.y = 0xA00000 * entity->height + 0x1000000;
            entity->drawOrder     = Zone->drawOrderHigh;
            entity->drawFX        = FX_FLIP;
            entity->alpha         = 0xE0;
            entity->field_80      = -0x100000;
            entity->field_88      = -0x100000;

            int top                = -0x100 - 0xA0 * entity->height;
            entity->hitbox1.top    = top;
            entity->hitbox1.left   = -128;
            entity->hitbox1.right  = -120;
            entity->hitbox1.bottom = -32;

            entity->hitbox2.left   = 120;
            entity->hitbox2.top    = top;
            entity->hitbox2.right  = 128;
            entity->hitbox2.bottom = -32;

            entity->hitbox3.left   = -128;
            entity->hitbox3.top    = top;
            entity->hitbox3.right  = 128;
            entity->hitbox3.bottom = top + 16;
            entity->state          = PopcornMachine_State_Unknown1;
            RSDK.SetSpriteAnimation(PopcornMachine->aniFrames, 0, &entity->animator, true, 0);
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
    PopcornMachine->sfxFanStart      = RSDK.GetSFX("Stage/FanStart.wav");
    PopcornMachine->sfxShoot         = RSDK.GetSFX("Stage/Shoot1.wav");
    PopcornMachine->sfxPopcornLaunch = RSDK.GetSFX("Stage/PopcornLaunch.wav");
}

void PopcornMachine_LinkPlayer(EntityPlayer *player)
{
    RSDK_THIS(PopcornMachine);

    player->nextAirState    = StateMachine_None;
    player->nextGroundState = StateMachine_None;
    player->state           = Player_State_None;
    player->onGround        = false;
    if (player->position.x < entity->position.x)
        player->velocity.x = 0xA0000;
    else
        player->velocity.x = -0xA0000;
    player->tileCollisions = false;
    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);

    EntityPopcornMachine *machine = CREATE_ENTITY(PopcornMachine, player, entity->position.x, entity->position.y);
    machine->isPermanent          = true;
    machine->parent               = (Entity *)entity;
    if (!player->sidekick)
        machine->bounds[player->playerID] = Zone->screenBoundsB1[player->playerID];
}
void PopcornMachine_CheckPlayerCollisions(void)
{
    RSDK_THIS(PopcornMachine);

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, entity, &entity->hitbox1);
        Player_CheckCollisionBox(player, entity, &entity->hitbox2);
        Player_CheckCollisionPlatform(player, entity, &entity->hitbox3);
        if (player->state != Player_State_None) {

            switch (entity->type) {
                case 0:
                    if (player->onGround) {
                        if (Player_CheckCollisionTouch(player, entity, &PopcornMachine->hitbox1)) {
                            PopcornMachine_LinkPlayer(player);
                        }
                    }
                    else {
                        Player_CheckCollisionBox(player, entity, &PopcornMachine->hitbox1);
                    }
                    break;
                case 1:
                    if (player->onGround) {
                        if (Player_CheckCollisionTouch(player, entity, &PopcornMachine->hitbox1)) {
                            PopcornMachine_LinkPlayer(player);
                        }
                    }
                    else {
                        Player_CheckCollisionBox(player, entity, &PopcornMachine->hitbox2);
                    }
                    break;
                case 2:
                    if (player->onGround) {
                        if (Player_CheckCollisionTouch(player, entity, &PopcornMachine->hitbox1)) {
                            PopcornMachine_LinkPlayer(player);
                        }
                        else {
                            if (Player_CheckCollisionTouch(player, entity, &PopcornMachine->hitbox2))
                                PopcornMachine_LinkPlayer(player);
                        }
                    }
                    else {
                        Player_CheckCollisionBox(player, entity, &PopcornMachine->hitbox1);
                        Player_CheckCollisionBox(player, entity, &PopcornMachine->hitbox2);
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
    entity->field_80 -= 0x8000;
    if (entity->field_80 <= -0x200000)
        entity->field_80 = -0x200000;
    entity->field_88 -= 0x18000;
    if (entity->field_88 <= -0x400000) {
        entity->field_88 = -0x400000;
        entity->field_60 = 0;
        entity->state    = PopcornMachine_State_Unknown3;
    }
}

void PopcornMachine_State_Unknown4(void)
{
    RSDK_THIS(PopcornMachine);

    PopcornMachine_CheckPlayerCollisions();

    entity->field_80 += 0x8000;
    if (entity->field_80 >= 0)
        entity->field_80 = 0;
    entity->field_88 += 0x18000;
    if (entity->field_88 >= 0) {
        entity->field_88 = 0;
        entity->state    = PopcornMachine_State_Unknown1;
    }
}

void PopcornMachine_State_Unknown3(void)
{
    RSDK_THIS(PopcornMachine);

    PopcornMachine_CheckPlayerCollisions();
    entity->field_7C = RSDK.Rand(-0x20000, 0x20000);
    entity->field_80 = RSDK.Rand(-0x20000, 0x20000) - 0x1E0000;
    entity->field_84 = RSDK.Rand(-0x20000, 0x20000);
    entity->field_88 = entity->field_80 - 0x1E0000 + RSDK.Rand(-0x20000, 0x20000);
    entity->field_60++;
    if ((uint32)(entity->field_60 - 25) <= 0x86) {
        if (!(entity->field_60 & 1)) {
            EntityPopcornKernel *kernel = CREATE_ENTITY(PopcornKernel, NULL, entity->position.x, entity->position.y - 0x400000);
            kernel->velocity.x          = RSDK.Rand(-0x30000, 0x30000);
            kernel->velocity.y          = RSDK.Rand(-0xC0000, -0x60000);
            kernel->angleVel            = RSDK.Rand(-0x60000, 0x60000);
            kernel->bounds.x            = entity->position.x;
            kernel->bounds.y            = entity->position.y;
        }
        if (!(entity->field_60 & 7))
            RSDK.PlaySfx(PopcornMachine->sfxShoot, false, 255);
    }

    if (entity->field_60 == 160) {
        int i = entity->position.y - (0xA00000 * entity->height) - 0xD00000;
        foreach_active(PopcornKernel, kernel)
        {
            kernel->state      = PopcornKernel_Unknown2;
            kernel->velocity.x = 0;
            kernel->velocity.y = RSDK.Rand(-0xA0000, -0x60000);
            kernel->field_68   = entity->position.y - (0xA00000 * entity->height) - 0xD00000;
        }
        RSDK.PlaySfx(PopcornMachine->sfxPopcornLaunch, false, 255);
        entity->state = PopcornMachine_State_Unknown4;
    }
}

void PopcornMachine_State2_Unknown1(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = (EntityPopcornMachine *)entity->parent;
    EntityPlayer *player         = entity->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < entity->position.y - 0x300000) {
            player->position.y = entity->position.y - 0x300000;
            player->velocity.y = 0;
            entity->state      = PopcornMachine_State2_Unknown3;
        }

        if (!Player_CheckValidState(player)) {
            if (!player->sidekick)
                Zone->screenBoundsB1[player->playerID] = entity->bounds[player->playerID];
            destroyEntity(entity);
        }
    }
    else {
        destroyEntity(entity);
    }

    if (abs(entity->position.x - player->position.x) < 0x100000) {
        player->position.x = entity->position.x;
        player->groundVel  = 0;
        player->velocity.x = 0;
        player->velocity.y = -0x80000;
        if (!player->sidekick)
            Zone->screenBoundsB1[player->playerID] = (entity->position.y >> 16) + 32;

        if (parent->state == PopcornMachine_State_Unknown1 || parent->state == PopcornMachine_State_Unknown4) {
            parent->state = PopcornMachine_State_Unknown2;
            RSDK.PlaySfx(PopcornMachine->sfxFanStart, false, 255);
        }
        entity->state = PopcornMachine_State2_Unknown2;
    }

    if (!Player_CheckValidState(player)) {
        if (!player->sidekick)
            Zone->screenBoundsB1[player->playerID] = entity->bounds[player->playerID];
        destroyEntity(entity);
    }
}

void PopcornMachine_State2_Unknown2(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = (EntityPopcornMachine *)entity->parent;
    EntityPlayer *player         = entity->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < entity->position.y - 0x300000) {
            player->position.y = entity->position.y - 0x300000;
            player->velocity.y = 0;
            entity->state      = PopcornMachine_State2_Unknown3;
        }

        if (!Player_CheckValidState(player)) {
            if (!player->sidekick)
                Zone->screenBoundsB1[player->playerID] = entity->bounds[player->playerID];
            destroyEntity(entity);
        }
    }
    else {
        destroyEntity(entity);
    }
}

void PopcornMachine_State2_Unknown3(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = (EntityPopcornMachine *)entity->parent;
    EntityPlayer *player         = entity->player;

    if (Player_CheckValidState(player)) {
        if (parent->state == PopcornMachine_State_Unknown4) {
            player->state           = Player_State_Air;
            player->gravityStrength = 0x80;
            player->velocity.y      = -0xD8000 - (parent->height << 16);
            player->position.y += player->velocity.y;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
            if (!player->sidekick)
                Zone->screenBoundsB1[player->playerID] = entity->bounds[player->playerID];
            entity->state = PopcornMachine_State2_Unknown4;
        }
    }
    else {
        destroyEntity(entity);
    }
}

void PopcornMachine_State2_Unknown4(void)
{
    RSDK_THIS(PopcornMachine);
    EntityPopcornMachine *parent = (EntityPopcornMachine *)entity->parent;
    EntityPlayer *player         = entity->player;

    if (Player_CheckValidState(player)) {
        if (player->position.y < parent->position.y - 0xA00000 * parent->height + 0xD00000) {
            player->tileCollisions = true;
            Player_ChangePhysicsState(player);
        }
    }
    else {
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void PopcornMachine_EditorDraw(void)
{
    RSDK_THIS(PopcornMachine);
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0xB00000;
    entity->visible       = true;
    entity->updateRange.y = 0xA00000 * entity->height + 0x1000000;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->drawFX        = FX_FLIP;
    entity->alpha         = 0xE0;
    entity->field_80      = -0x100000;
    entity->field_88      = -0x100000;
    RSDK.SetSpriteAnimation(PopcornMachine->aniFrames, 0, &entity->animator, true, 0);

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
