#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectLRZ1Intro *LRZ1Intro;

void LRZ1Intro_Update(void)
{
    RSDK_THIS(LRZ1Intro);
    StateMachine_Run(self->state);
}

void LRZ1Intro_LateUpdate(void) {}

void LRZ1Intro_StaticUpdate(void) {}

void LRZ1Intro_Draw(void)
{
    RSDK_THIS(LRZ1Intro);
    RSDK.SetActivePalette(6, 0, ScreenInfo->height);
    RSDK.DrawSprite(&self->animator, 0, false);
    RSDK.SetActivePalette(0, 0, ScreenInfo->height);
}

void LRZ1Intro_Create(void *data)
{
    RSDK_THIS(LRZ1Intro);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        RSDK.SetSpriteAnimation(LRZ1Intro->aniFrames, 0, &self->animator, true, 0);
        self->state = LRZ1Intro_State_Unknown1;
    }
}

void LRZ1Intro_StageLoad(void)
{
    LRZ1Intro->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/IntroSub.bin", SCOPE_STAGE);
    LRZ1Intro->sfxSubLand     = RSDK.GetSfx("LRZ/SubLand.wav");
    LRZ1Intro->sfxWalkerLegs2 = RSDK.GetSfx("LRZ/WalkerLegs2.wav");
    LRZ1Intro->sfxLava        = RSDK.GetSfx("Stage/Lava.wav");
}

void LRZ1Intro_HandlePlayerCollisions(void)
{
    RSDK_THIS(LRZ1Intro);

    Hitbox *hitbox1 = RSDK.GetHitbox(&self->animator, 0);
    Hitbox *hitbox2 = RSDK.GetHitbox(&self->animator, 1);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, hitbox1))
            player->position.y += 0x20000;
        if (Player_CheckCollisionPlatform(player, self, hitbox2))
            player->position.y += 0x20000;
    }
}

void LRZ1Intro_HandlePlayerMovement(void)
{
    RSDK_THIS(LRZ1Intro);

    foreach_active(Player, player)
    {
        player->velocity.x = 0;
        player->velocity.y = 0;
        player->position.x = self->position.x + self->field_68[player->playerID].x;
        player->position.y = self->position.y + self->field_68[player->playerID].y;
    }
}

void LRZ1Intro_HandleShipExplosions(void)
{
    RSDK_THIS(LRZ1Intro);

    if (!(Zone->timer & 3)) {
        int x                                                                = self->position.x + 0xE00000;
        int y                                                                = self->spawnPosY + RSDK.Rand(0x400000, 0x580000);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
    }

    if (self->velocity.y <= 0x10000) {
        if (!(Zone->timer % 6)) {
            EntityLavaGeyser *geyser = CREATE_ENTITY(LavaGeyser, NULL, self->position.x, self->spawnPosY + 0x4C0000);
            geyser->visible          = true;
            geyser->active           = ACTIVE_NORMAL;
            geyser->velocity.y       = 0x40000;
            RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &geyser->animator1, true, 0);
            RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 0, &geyser->animator2, true, 0);
            RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &geyser->animator3, true, 16);
            geyser->state = LavaGeyser_State_Unknown3;
        }
    }
}

void LRZ1Intro_HandleExplosions(void)
{
    RSDK_THIS(LRZ1Intro);

    if (!(Zone->timer & 3)) {
        int x = self->position.x + RSDK.Rand(-0xE00000, 0xE00000);
        int y = self->spawnPosY + RSDK.Rand(0x400000, 0x580000);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), x, y);
    }
}

void LRZ1Intro_State_Unknown1(void)
{
    RSDK_THIS(LRZ1Intro);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Hitbox *playerHitbox = Player_GetHitbox(player1);
    player1->state       = Player_State_None;
    RSDK.SetSpriteAnimation(player1->aniFrames, ANI_BALANCE2, &player1->animator, false, 0);
    self->field_68[0].x = 0x600000;
    self->field_68[0].y = -0x10000 * playerHitbox->bottom;

    for (int p = 1; p < Player->playerCount; ++p) {
        EntityPlayer *playerPtr = RSDK_GET_ENTITY(p, Player);

        if (playerPtr->objectID == Player->objectID) {
            Hitbox *hitbox        = Player_GetHitbox(playerPtr);
            self->field_68[p].x = 0x400000;
            self->field_68[p].y = -0x10000 * hitbox->bottom;
            playerPtr->state      = Player_State_None;
            RSDK.SetSpriteAnimation(playerPtr->aniFrames, ANI_BALANCE2, &playerPtr->animator, false, 0);
        }
    }

    foreach_all(Player, player)
    {
        player1->position.x = self->position.x;
        player1->position.y = self->position.y;

        if (player1->camera) {
            player1->camera->position.x = self->position.x;
            player1->camera->position.y = self->position.y;
        }
    }

    Zone->playerBoundActiveL[0] = false;
    self->state               = LRZ1Intro_State_Unknown2;
}

void LRZ1Intro_State_Unknown2(void)
{
    RSDK_THIS(LRZ1Intro);

    if (++self->timer == 32) {
        self->timer      = 0;
        self->velocity.x = 0x100000;
        self->state      = LRZ1Intro_State_Unknown3;
        RSDK.PlaySfx(LRZ1Intro->sfxSubLand, false, 255);
        Zone->cameraBoundsL[0] = 2732;
        Zone->cameraBoundsL[1] = 2732;
        Zone->cameraBoundsL[2] = 2732;
        Zone->cameraBoundsL[3] = 2732;
    }
    LRZ1Intro_HandlePlayerMovement();
}

void LRZ1Intro_State_Unknown3(void)
{
    RSDK_THIS(LRZ1Intro);

    if (self->velocity.y < 0x100000)
        self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (self->velocity.y > 0x60000)
        camera->lookPos.y += 3;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x800000, true)) {
        Zone->playerBoundActiveL[0] = true;
        self->spawnPosY           = self->position.y + 0x380000;
        self->state               = LRZ1Intro_State_Unknown4;
        Camera_ShakeScreen(0, 0, 6);
        camera->lookPos.y &= 0xFFFE;
        RSDK.PlaySfx(LRZ1Intro->sfxWalkerLegs2, false, 255);
    }
    LRZ1Intro_HandlePlayerMovement();
}

void LRZ1Intro_State_Unknown4(void)
{
    RSDK_THIS(LRZ1Intro);

    self->velocity.x -= 0x1000;
    if (self->velocity.x < 0)
        self->velocity.x = 0;
    self->velocity.y = (200 * self->velocity.y) >> 8;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(LRZ1Intro->sfxLava, false, 255);

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x1000000, 0x600000, true)) {
        Camera_ShakeScreen(0, 0, 6);
        self->state = LRZ1Intro_State_Unknown5;
        RSDK.PlaySfx(LRZ1Intro->sfxWalkerLegs2, false, 255);
    }
    LRZ1Intro_HandleShipExplosions();
    LRZ1Intro_HandlePlayerMovement();
}

void LRZ1Intro_State_Unknown5(void)
{
    RSDK_THIS(LRZ1Intro);

    if (++self->timer == 24) {
        foreach_active(Player, player) { player->state = Player_State_Ground; }
        self->timer   = 0;
        self->originY = self->position.y;
        self->state   = LRZ1Intro_State_Unknown6;
    }
}

void LRZ1Intro_State_Unknown6(void)
{
    RSDK_THIS(LRZ1Intro);

    self->originY += 0x4000;
    self->position.y = (RSDK.Sin256(self->timer) << 10) + self->originY;
    LRZ1Intro_HandleExplosions();
    LRZ1Intro_HandlePlayerCollisions();
    self->timer += 2;
    if (self->timer == 1024 || !RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void LRZ1Intro_EditorDraw(void) {}

void LRZ1Intro_EditorLoad(void) {}

void LRZ1Intro_Serialize(void) {}
#endif
