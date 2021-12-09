#include "SonicMania.h"

ObjectPhantomRider *PhantomRider;

void PhantomRider_Update(void)
{
    RSDK_THIS(PhantomRider);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);
}

void PhantomRider_LateUpdate(void) {}

void PhantomRider_StaticUpdate(void) {}

void PhantomRider_Draw(void)
{
    RSDK_THIS(PhantomRider);

    RSDK.SetActivePalette(4, 0, ScreenInfo[SceneInfo->currentScreenID].height);

    if (self->invincibilityTimer & 1)
        RSDK.CopyPalette(6, 128, 4, 128, 128);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }

    if (self->invincibilityTimer & 1)
        RSDK.CopyPalette(5, 128, 4, 128, 128);

    RSDK.SetActivePalette(0, 0, ScreenInfo[SceneInfo->currentScreenID].height);
}

void PhantomRider_Create(void *data)
{
    RSDK_THIS(PhantomRider);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->type          = voidToInt(data);

        if (data) {
            if (voidToInt(data) == PHANTOMRIDER_JIMMY) {
                self->active = ACTIVE_NORMAL;
                self->state  = PhantomRider_State2_Unknown;
            }
        }
        else {
            self->active = ACTIVE_BOUNDS;
            self->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 4, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 5, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 6, &self->animator4, true, 0);
            self->direction       = FLIP_X;
            self->collisionPlane  = 1;
            self->collisionLayers = Zone->fgLayers;
            self->startPos        = self->position;
            self->hitbox.left     = -24;
            self->hitbox.top      = 0;
            self->hitbox.right    = 24;
            self->hitbox.bottom   = 20;
            self->outerBox.left   = -12;
            self->outerBox.top    = 0;
            self->outerBox.right  = 12;
            self->outerBox.bottom = 30;
            self->innerBox.left   = -11;
            self->innerBox.top    = 0;
            self->tileCollisions  = true;
            self->innerBox.right  = 11;
            self->innerBox.bottom = 30;
            self->stateDraw       = PhantomRider_StateDraw_Unknown1;
            self->state           = PhantomRider_State_Unknown1;
        }
    }
}

void PhantomRider_StageLoad(void)
{
    PhantomRider->aniFrames      = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE);
    PhantomRider->sfxRiderLaunch = RSDK.GetSfx("LRZ/RiderLaunch.wav");
}

void PhantomRider_ProcessAutoScroll(void)
{
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

    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator4, NULL, false);
}

void PhantomRider_State_Unknown1(void)
{
    RSDK_THIS(PhantomRider);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.x > self->position.x) {
        EntityPhantomRider *child = CREATE_ENTITY(PhantomRider, intToVoid(PHANTOMRIDER_JIMMY), player1->position.x, player1->position.y);

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->targetPtr    = (Entity*)child;
        camera->state        = Camera_State_Follow;

        player1->camera       = NULL;
        Zone->autoScrollSpeed = player1->velocity.x;
        self->groundVel     = 0;
        self->velocity.x    = 0;
        self->child         = (Entity *)child;
        self->active        = ACTIVE_NORMAL;
        self->state         = PhantomRider_State_Unknown2;
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

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    PhantomRider_ProcessAutoScroll();

    if (++self->timer > 48) {
        if (self->groundVel < 0xC0000)
            self->groundVel += 0x40000;
    }

    if (self->position.x > self->child->position.x + 0x200000) {
        self->timer = 0;
        RSDK.PlaySfx(PhantomRider->sfxRiderLaunch, false, 0xFF);
        self->state = PhantomRider_State_Unknown3;
    }

    if (!self->onGround) {
        self->velocity.y += 0x3800;
        if (self->velocity.y > 0xC0000)
            self->velocity.y = 0xC0000;
    }
    self->prevOnGround = self->onGround;
    RSDK.ProcessTileCollisions(self, &self->outerBox, &self->innerBox);
}

void PhantomRider_State_Unknown3(void)
{
    RSDK_THIS(PhantomRider);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    PhantomRider_ProcessAutoScroll();

    if (self->groundVel > Zone->autoScrollSpeed - 0x10000) {
        self->groundVel -= 0x2000;
        if (self->groundVel < Zone->autoScrollSpeed - 0x4000)
            self->groundVel = Zone->autoScrollSpeed - 0x4000;
    }

    if (!self->onGround) {
        self->velocity.y += 0x3800;
        if (self->velocity.y > 0xC0000)
            self->velocity.y = 0xC0000;
    }
    self->prevOnGround = self->onGround;
    RSDK.ProcessTileCollisions(self, &self->outerBox, &self->innerBox);

    EntityPlatformNode *marker = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, PlatformNode);
    if (self->position.x > marker->position.x)
        self->state = PhantomRider_State_Unknown4;
}

void PhantomRider_State_Unknown4(void)
{
    RSDK_THIS(PhantomRider);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    PhantomRider_ProcessAutoScroll();

    if (!self->onGround) {
        self->velocity.y += 0x3800;
        if (self->velocity.y > 0xC0000)
            self->velocity.y = 0xC0000;
    }
    self->prevOnGround = self->onGround;
    RSDK.ProcessTileCollisions(self, &self->outerBox, &self->innerBox);

    if (++self->timer == 560) {
        self->timer         = 0;
        Zone->autoScrollSpeed = 0;
        PhantomEgg_SetupScanlineCB();
        self->state = PhantomRider_State_Unknown5;
        foreach_active(PopOut, popOut) { popOut->flag = false; }
        foreach_active(Player, player) { Player_ChangePhysicsState(player); }
    }

    foreach_active(Spikes, spikes)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox, spikes, &spikes->hitbox) == true) {
            int slot             = RSDK.GetEntityID(spikes);
            EntityPopOut *popOut = RSDK_GET_ENTITY(slot - 1, PopOut);
            popOut->flag         = false;
            foreach_active(Button, button)
            {
                if (button->tag == popOut->tag)
                    button->activated = false;
            }
            self->invincibilityTimer = 48;
            RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
        }
    }
}

void PhantomRider_State_Unknown5(void)
{
    RSDK_THIS(PhantomRider);

    if (++self->timer == 60) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->timer      = 0;
        self->active     = ACTIVE_BOUNDS;
        destroyEntity(self->child);

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        Camera_SetTargetEntity(0, player1);
        player1->camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

        foreach_active(PopOut, popOut) { popOut->active = ACTIVE_BOUNDS; }

        foreach_active(Button, button)
        {
            button->active   = ACTIVE_BOUNDS;
            button->activated = false;
        }

        self->state = PhantomRider_State_Unknown1;
    }
}

void PhantomRider_State2_Unknown(void)
{
    RSDK_THIS(PhantomRider);

    self->position.x += Zone->autoScrollSpeed;

    if (self->position.y < 0x8A80000)
        self->position.y += 0x20000;

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    if (camera->offset.y > 0)
        camera->offset.y -= (camera->offset.y >> 4);

    foreach_active(Player, player)
    {
        int x = self->position.x - (ScreenInfo->centerX << 16) + 0x100000;
        if (player->position.x < x)
            player->position.x = x;

        int y = ((ScreenInfo->centerX - 16) << 16) + self->position.x;
        if (player->position.x > y)
            player->position.x = y;
    }
}

#if RETRO_INCLUDE_EDITOR
void PhantomRider_EditorDraw(void)
{
    RSDK_THIS(PhantomRider);
    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 4, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 5, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(PhantomRider->aniFrames, 6, &self->animator4, true, 0);
    self->direction = FLIP_X;

    PhantomRider_StateDraw_Unknown1();
}

void PhantomRider_EditorLoad(void) { PhantomRider->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE); }
#endif

void PhantomRider_Serialize(void) {}
