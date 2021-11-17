#include "SonicMania.h"

ObjectPhantomRider *PhantomRider;

void PhantomRider_Update(void)
{
    RSDK_THIS(PhantomRider);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    StateMachine_Run(entity->state);
}

void PhantomRider_LateUpdate(void) {}

void PhantomRider_StaticUpdate(void) {}

void PhantomRider_Draw(void)
{
    RSDK_THIS(PhantomRider);

    RSDK.SetActivePalette(4, 0, ScreenInfo[SceneInfo->currentScreenID].height);

    if (entity->invincibilityTimer & 1)
        RSDK.CopyPalette(6, 128, 4, 128, 128);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }

    if (entity->invincibilityTimer & 1)
        RSDK.CopyPalette(5, 128, 4, 128, 128);

    RSDK.SetActivePalette(0, 0, ScreenInfo[SceneInfo->currentScreenID].height);
}

void PhantomRider_Create(void *data)
{
    RSDK_THIS(PhantomRider);

    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->type          = voidToInt(data);

        if (data) {
            if (voidToInt(data) == PHANTOMRIDER_JIMMY) {
                entity->active = ACTIVE_NORMAL;
                entity->state  = PhantomRider_State2_Unknown;
            }
        }
        else {
            entity->active = ACTIVE_BOUNDS;
            entity->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 4, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 5, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 6, &entity->animator4, true, 0);
            entity->direction       = FLIP_X;
            entity->collisionPlane  = 1;
            entity->collisionLayers = Zone->fgLayers;
            entity->startPos        = entity->position;
            entity->hitbox.left     = -24;
            entity->hitbox.top      = 0;
            entity->hitbox.right    = 24;
            entity->hitbox.bottom   = 20;
            entity->outerBox.left   = -12;
            entity->outerBox.top    = 0;
            entity->outerBox.right  = 12;
            entity->outerBox.bottom = 30;
            entity->innerBox.left   = -11;
            entity->innerBox.top    = 0;
            entity->tileCollisions  = true;
            entity->innerBox.right  = 11;
            entity->innerBox.bottom = 30;
            entity->stateDraw       = PhantomRider_StateDraw_Unknown1;
            entity->state           = PhantomRider_State_Unknown1;
        }
    }
}

void PhantomRider_StageLoad(void)
{
    PhantomRider->aniFrames      = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE);
    PhantomRider->sfxRiderLaunch = RSDK.GetSFX("LRZ/RiderLaunch.wav");
}

void PhantomRider_ProcessAutoScroll(void)
{
    RSDK_THIS(PhantomRider);

    if (Zone->autoScrollSpeed > 0x80000) {
        Zone->autoScrollSpeed -= 0x2000;
        if (Zone->autoScrollSpeed < 0x80000)
            Zone->autoScrollSpeed = 0x80000;
    }
    if (Zone->autoScrollSpeed < 0x80000) {
        Zone->autoScrollSpeed += 0x2000;

        if (Zone->autoScrollSpeed > 0x80000)
            Zone->autoScrollSpeed = 0x80000;
    }

    foreach_active(Player, player)
    {
        if (player->groundVel < Zone->autoScrollSpeed - 0x20000)
            player->groundVel = Zone->autoScrollSpeed - 0x20000;
        player->direction = FLIP_NONE;
        player->topSpeed  = Zone->autoScrollSpeed + 0x20000;
        if (player->groundVel > player->topSpeed)
            player->groundVel = player->topSpeed;
    }
}

void PhantomRider_StateDraw_Unknown1(void)
{
    RSDK_THIS(PhantomRider);

    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator4, NULL, false);
}

void PhantomRider_State_Unknown1(void)
{
    RSDK_THIS(PhantomRider);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.x > entity->position.x) {
        EntityPhantomRider *child = CREATE_ENTITY(PhantomRider, intToVoid(PHANTOMRIDER_JIMMY), player1->position.x, player1->position.y);

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->targetPtr    = (Entity*)child;
        camera->state        = Camera_State_Follow;

        player1->camera       = NULL;
        Zone->autoScrollSpeed = player1->velocity.x;
        entity->groundVel     = 0;
        entity->velocity.x    = 0;
        entity->child         = (Entity *)child;
        entity->active        = ACTIVE_NORMAL;
        entity->state         = PhantomRider_State_Unknown2;
        foreach_active(PopOut, popOut)
        {
            popOut->active    = ACTIVE_NORMAL;
            popOut->drawOrder = 2;
        }
        foreach_active(Button, button) { button->active = ACTIVE_NORMAL; }
        RSDK.PlaySfx(PhantomRider->sfxRiderLaunch, false, 255);
    }
}

void PhantomRider_State_Unknown2(void)
{
    RSDK_THIS(PhantomRider);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    PhantomRider_ProcessAutoScroll();

    if (++entity->timer > 48) {
        if (entity->groundVel < 0xC0000)
            entity->groundVel += 0x40000;
    }

    if (entity->position.x > entity->child->position.x + 0x200000) {
        entity->timer = 0;
        RSDK.PlaySfx(PhantomRider->sfxRiderLaunch, false, 0xFF);
        entity->state = PhantomRider_State_Unknown3;
    }

    if (!entity->onGround) {
        entity->velocity.y += 0x3800;
        if (entity->velocity.y > 0xC0000)
            entity->velocity.y = 0xC0000;
    }
    entity->prevOnGround = entity->onGround;
    RSDK.ProcessTileCollisions(entity, &entity->outerBox, &entity->innerBox);
}

void PhantomRider_State_Unknown3(void)
{
    RSDK_THIS(PhantomRider);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    PhantomRider_ProcessAutoScroll();

    if (entity->groundVel > Zone->autoScrollSpeed - 0x10000) {
        entity->groundVel -= 0x2000;
        if (entity->groundVel < Zone->autoScrollSpeed - 0x4000)
            entity->groundVel = Zone->autoScrollSpeed - 0x4000;
    }

    if (!entity->onGround) {
        entity->velocity.y += 0x3800;
        if (entity->velocity.y > 0xC0000)
            entity->velocity.y = 0xC0000;
    }
    entity->prevOnGround = entity->onGround;
    RSDK.ProcessTileCollisions(entity, &entity->outerBox, &entity->innerBox);

    EntityPlatformNode *marker = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, PlatformNode);
    if (entity->position.x > marker->position.x)
        entity->state = PhantomRider_State_Unknown4;
}

void PhantomRider_State_Unknown4(void)
{
    RSDK_THIS(PhantomRider);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    PhantomRider_ProcessAutoScroll();

    if (!entity->onGround) {
        entity->velocity.y += 0x3800;
        if (entity->velocity.y > 0xC0000)
            entity->velocity.y = 0xC0000;
    }
    entity->prevOnGround = entity->onGround;
    RSDK.ProcessTileCollisions(entity, &entity->outerBox, &entity->innerBox);

    if (++entity->timer == 560) {
        entity->timer         = 0;
        Zone->autoScrollSpeed = 0;
        PhantomEgg_SetupScanlineCB();
        entity->state = PhantomRider_State_Unknown5;
        foreach_active(PopOut, popOut) { popOut->flag = false; }
        foreach_active(Player, player) { Player_ChangePhysicsState(player); }
    }

    foreach_active(Spikes, spikes)
    {
        if (RSDK.CheckObjectCollisionTouchBox(entity, &entity->hitbox, spikes, &spikes->hitbox) == true) {
            int slot             = RSDK.GetEntityID(spikes);
            EntityPopOut *popOut = RSDK_GET_ENTITY(slot - 1, PopOut);
            popOut->flag         = false;
            foreach_active(Button, button)
            {
                if (button->tag == popOut->tag)
                    button->field_70 = false;
            }
            entity->invincibilityTimer = 48;
            RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
        }
    }
}

void PhantomRider_State_Unknown5(void)
{
    RSDK_THIS(PhantomRider);

    if (++entity->timer == 60) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->timer      = 0;
        entity->active     = ACTIVE_BOUNDS;
        destroyEntity(entity->child);

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        Camera_SetTargetEntity(0, player1);
        player1->camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

        foreach_active(PopOut, popOut) { popOut->active = ACTIVE_BOUNDS; }

        foreach_active(Button, button)
        {
            button->active   = ACTIVE_BOUNDS;
            button->field_70 = false;
        }

        entity->state = PhantomRider_State_Unknown1;
    }
}

void PhantomRider_State2_Unknown(void)
{
    RSDK_THIS(PhantomRider);

    entity->position.x += Zone->autoScrollSpeed;

    if (entity->position.y < 0x8A80000)
        entity->position.y += 0x20000;

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    if (camera->offset.y > 0)
        camera->offset.y -= (camera->offset.y >> 4);

    foreach_active(Player, player)
    {
        int x = entity->position.x - (ScreenInfo->centerX << 16) + 0x100000;
        if (player->position.x < x)
            player->position.x = x;

        int y = ((ScreenInfo->centerX - 16) << 16) + entity->position.x;
        if (player->position.x > y)
            player->position.x = y;
    }
}

#if RETRO_INCLUDE_EDITOR
void PhantomRider_EditorDraw(void)
{
    RSDK_THIS(PhantomRider);
    entity->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 4, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 5, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 6, &entity->animator4, true, 0);
    entity->direction = FLIP_X;

    PhantomRider_StateDraw_Unknown1();
}

void PhantomRider_EditorLoad(void) { PhantomRider->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE); }
#endif

void PhantomRider_Serialize(void) {}
