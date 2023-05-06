// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ1Intro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
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

    RSDK.SetActivePalette(6, 0, ScreenInfo->size.y);

    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK.SetActivePalette(0, 0, ScreenInfo->size.y);
}

void LRZ1Intro_Create(void *data)
{
    RSDK_THIS(LRZ1Intro);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        RSDK.SetSpriteAnimation(LRZ1Intro->aniFrames, 0, &self->animator, true, 0);
        self->state = LRZ1Intro_State_SetupActors;
    }
}

void LRZ1Intro_StageLoad(void)
{
    LRZ1Intro->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/IntroSub.bin", SCOPE_STAGE);

    LRZ1Intro->sfxSubLand     = RSDK.GetSfx("LRZ/SubLand.wav");
    LRZ1Intro->sfxWalkerLegs2 = RSDK.GetSfx("LRZ/WalkerLegs2.wav");
    LRZ1Intro->sfxLava        = RSDK.GetSfx("Stage/Lava.wav");
}

void LRZ1Intro_HandlePlayerCollisions(void)
{
    RSDK_THIS(LRZ1Intro);

    Hitbox *hitboxTopDeck    = RSDK.GetHitbox(&self->animator, 0);
    Hitbox *hitboxBottomDeck = RSDK.GetHitbox(&self->animator, 1);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, hitboxTopDeck))
            player->position.y += 0x20000;

        if (Player_CheckCollisionPlatform(player, self, hitboxBottomDeck))
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

        player->position.x = self->position.x + self->playerOffset[player->playerID].x;
        player->position.y = self->position.y + self->playerOffset[player->playerID].y;
    }
}

void LRZ1Intro_HandleLavaContact(void)
{
    RSDK_THIS(LRZ1Intro);

    if (!(Zone->timer & 3)) {
        int32 x                                                                = self->position.x + 0xE00000;
        int32 y                                                                = self->spawnPosY + RSDK.Rand(0x400000, 0x580000);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
    }

    if (self->velocity.y <= 0x10000) {
        if (!(Zone->timer % 6)) {
            EntityLavaGeyser *geyser = CREATE_ENTITY(LavaGeyser, NULL, self->position.x, self->spawnPosY + 0x4C0000);
            geyser->visible          = true;
            geyser->active           = ACTIVE_NORMAL;
            geyser->velocity.y       = 0x40000;

            RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &geyser->plumeAnimator, true, 0);
            RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 0, &geyser->flowAnimator, true, 0);
            RSDK.SetSpriteAnimation(LavaGeyser->aniFrames, 1, &geyser->plumeLoopAnimator, true, 16);

            geyser->state = LavaGeyser_State_Erupting;
        }
    }
}

void LRZ1Intro_HandleExplosions(void)
{
    RSDK_THIS(LRZ1Intro);

    if (!(Zone->timer & 3)) {
        int32 x = self->position.x + RSDK.Rand(-0xE00000, 0xE00000);
        int32 y = self->spawnPosY + RSDK.Rand(0x400000, 0x580000);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), x, y);
    }
}

void LRZ1Intro_State_SetupActors(void)
{
    RSDK_THIS(LRZ1Intro);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Hitbox *playerHitbox = Player_GetHitbox(player1);
    player1->state       = Player_State_Static;
    RSDK.SetSpriteAnimation(player1->aniFrames, ANI_BALANCE_2, &player1->animator, false, 0);

    self->playerOffset[0].x = 0x600000;
    self->playerOffset[0].y = -0x10000 * playerHitbox->bottom;

    for (int32 p = 1; p < Player->playerCount; ++p) {
        EntityPlayer *playerPtr = RSDK_GET_ENTITY(p, Player);

        if (playerPtr->classID == Player->classID) {
            Hitbox *hitbox          = Player_GetHitbox(playerPtr);
            self->playerOffset[p].x = 0x400000;
            self->playerOffset[p].y = -0x10000 * hitbox->bottom;
            playerPtr->state        = Player_State_Static;
            RSDK.SetSpriteAnimation(playerPtr->aniFrames, ANI_BALANCE_2, &playerPtr->animator, false, 0);
        }
    }

    foreach_all(Player, player)
    {
        player->position.x = self->position.x;
        player->position.y = self->position.y;

        if (player->camera) {
            player->camera->position.x = self->position.x;
            player->camera->position.y = self->position.y;
        }
    }

    Zone->playerBoundActiveL[0] = false;
    self->state                 = LRZ1Intro_State_IntroDelay;
}

void LRZ1Intro_State_IntroDelay(void)
{
    RSDK_THIS(LRZ1Intro);

    if (++self->timer == 32) {
        self->timer      = 0;
        self->velocity.x = 0x100000;
        self->state      = LRZ1Intro_State_EnterSub;

        RSDK.PlaySfx(LRZ1Intro->sfxSubLand, false, 255);

        Zone->cameraBoundsL[0] = 2732;
        Zone->cameraBoundsL[1] = 2732;
        Zone->cameraBoundsL[2] = 2732;
        Zone->cameraBoundsL[3] = 2732;
    }

    LRZ1Intro_HandlePlayerMovement();
}

void LRZ1Intro_State_EnterSub(void)
{
    RSDK_THIS(LRZ1Intro);

    if (self->velocity.y < 0x100000)
        self->velocity.y += 0x3800;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (self->velocity.y > 0x60000)
        camera->lookPos.y += 3;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x800000, true)) {
        Zone->playerBoundActiveL[0] = true;
        self->spawnPosY             = self->position.y + 0x380000;
        self->state                 = LRZ1Intro_State_RidingSub;

        Camera_ShakeScreen(0, 0, 6);
        camera->lookPos.y &= 0xFFFE;

        RSDK.PlaySfx(LRZ1Intro->sfxWalkerLegs2, false, 255);
    }

    LRZ1Intro_HandlePlayerMovement();
}

void LRZ1Intro_State_RidingSub(void)
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

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x1000000, 0x600000, true)) {
        Camera_ShakeScreen(0, 0, 6);
        self->state = LRZ1Intro_State_CrashedSub;
        RSDK.PlaySfx(LRZ1Intro->sfxWalkerLegs2, false, 255);
    }

    LRZ1Intro_HandleLavaContact();
    LRZ1Intro_HandlePlayerMovement();
}

void LRZ1Intro_State_CrashedSub(void)
{
    RSDK_THIS(LRZ1Intro);

    if (++self->timer == 24) {
        foreach_active(Player, player) { player->state = Player_State_Ground; }
        self->timer   = 0;
        self->originY = self->position.y;
        self->state   = LRZ1Intro_State_SubSinking;
    }
}

void LRZ1Intro_State_SubSinking(void)
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

#if GAME_INCLUDE_EDITOR
void LRZ1Intro_EditorDraw(void) {}

void LRZ1Intro_EditorLoad(void) {}
#endif

void LRZ1Intro_Serialize(void) {}
#endif
