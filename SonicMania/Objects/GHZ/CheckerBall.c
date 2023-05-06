// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CheckerBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCheckerBall *CheckerBall;

void CheckerBall_Update(void)
{
    RSDK_THIS(CheckerBall);
    self->active = ACTIVE_NORMAL;
    CheckerBall_HandlePlayerMovement();
    self->ballAngle += self->angleVel;
    CheckerBall_HandlePhysics();
    RSDK.ProcessObjectMovement(self, &CheckerBall->outerBox, &CheckerBall->innerBox);
    CheckerBall_HandleObjectCollisions();
    CheckerBall_HandlePlayerInteractions();
    self->roundedPos.x = self->position.x & 0xFFFF0000;
    self->roundedPos.y = self->position.y & 0xFFFF0000;
}

void CheckerBall_LateUpdate(void) {}

void CheckerBall_StaticUpdate(void) {}

void CheckerBall_Draw(void)
{
    RSDK_THIS(CheckerBall);
    self->animator.frameID = (((self->ballAngle >> 14) + 8) >> 4) & 7;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void CheckerBall_Create(void *data)
{
    RSDK_THIS(CheckerBall);
    self->visible         = true;
    self->drawGroup       = Zone->objectDrawGroup[0];
    self->active          = ACTIVE_BOUNDS;
    self->updateRange.x   = 0x400000;
    self->updateRange.y   = 0x400000;
    self->onGround        = false;
    self->tileCollisions  = TILECOLLISION_DOWN;
    self->collisionLayers = Zone->collisionLayers;
    self->roundedPos.x    = self->position.x;
    self->roundedPos.y    = self->position.y;
    self->collisionPlane  = 0;
    RSDK.SetSpriteAnimation(CheckerBall->aniFrames, 0, &self->animator, true, 0);
}

void CheckerBall_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        CheckerBall->aniFrames = RSDK.LoadSpriteAnimation("GHZ/CheckerBall.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("Blueprint"))
        CheckerBall->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CheckerBall.bin", SCOPE_STAGE);

    CheckerBall->outerBox.left   = -9;
    CheckerBall->outerBox.top    = -22;
    CheckerBall->outerBox.right  = 9;
    CheckerBall->outerBox.bottom = 22;

    CheckerBall->innerBox.left   = -8;
    CheckerBall->innerBox.top    = -22;
    CheckerBall->innerBox.right  = 8;
    CheckerBall->innerBox.bottom = 22;

    CheckerBall->hitboxBall.left   = -22;
    CheckerBall->hitboxBall.top    = -22;
    CheckerBall->hitboxBall.right  = 22;
    CheckerBall->hitboxBall.bottom = 22;

    DEBUGMODE_ADD_OBJ(CheckerBall);

    CheckerBall->heightArray[0]  = 0;
    CheckerBall->heightArray[1]  = 0;
    CheckerBall->heightArray[2]  = 0;
    CheckerBall->heightArray[3]  = 0;
    CheckerBall->heightArray[4]  = 1;
    CheckerBall->heightArray[5]  = 1;
    CheckerBall->heightArray[6]  = 1;
    CheckerBall->heightArray[7]  = 1;
    CheckerBall->heightArray[8]  = 2;
    CheckerBall->heightArray[9]  = 2;
    CheckerBall->heightArray[10] = 3;
    CheckerBall->heightArray[11] = 3;
    CheckerBall->heightArray[12] = 4;
    CheckerBall->heightArray[13] = 4;
    CheckerBall->heightArray[14] = 5;
    CheckerBall->heightArray[15] = 6;
    CheckerBall->heightArray[16] = 7;
    CheckerBall->heightArray[17] = 8;
    CheckerBall->heightArray[18] = 9;
    CheckerBall->heightArray[19] = 10;
    CheckerBall->heightArray[20] = 12;
    CheckerBall->heightArray[21] = 14;
    CheckerBall->heightArray[22] = 16;
    CheckerBall->heightArray[23] = 20;

    // an unused array, here since the ye olde days of v4
    CheckerBall->unusedArray[0]  = 253;
    CheckerBall->unusedArray[1]  = 251;
    CheckerBall->unusedArray[2]  = 248;
    CheckerBall->unusedArray[3]  = 245;
    CheckerBall->unusedArray[4]  = 243;
    CheckerBall->unusedArray[5]  = 240;
    CheckerBall->unusedArray[6]  = 237;
    CheckerBall->unusedArray[7]  = 235;
    CheckerBall->unusedArray[8]  = 232;
    CheckerBall->unusedArray[9]  = 229;
    CheckerBall->unusedArray[10] = 226;
    CheckerBall->unusedArray[11] = 224;
    CheckerBall->unusedArray[12] = 221;
    CheckerBall->unusedArray[13] = 219;
    CheckerBall->unusedArray[14] = 216;
    CheckerBall->unusedArray[15] = 213;
    CheckerBall->unusedArray[16] = 210;
    CheckerBall->unusedArray[17] = 208;
    CheckerBall->unusedArray[18] = 205;
    CheckerBall->unusedArray[19] = 203;
    CheckerBall->unusedArray[20] = 200;
    CheckerBall->unusedArray[21] = 197;
    CheckerBall->unusedArray[22] = 195;
    CheckerBall->unusedArray[23] = 192;
}

void CheckerBall_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(CheckerBall, NULL, self->position.x, self->position.y);
}

void CheckerBall_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(CheckerBall->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void CheckerBall_HandlePhysics(void)
{
    RSDK_THIS(CheckerBall);
    if (self->onGround) {
        int32 spd = RSDK.Sin256(self->angle) << 13 >> 8;
        self->groundVel += spd;
        self->angleVel = self->groundVel;
        if (!spd && !self->playerControlled) {
            if (self->groundVel <= 0) {
                self->groundVel += 0x400;
                if (self->groundVel > 0)
                    self->groundVel = 0;
            }
            else {
                self->groundVel -= 0x400;
                if (self->groundVel < 0)
                    self->groundVel = 0;
            }
        }
        self->velocity.x = self->groundVel * RSDK.Cos256(self->angle) >> 8;
        self->velocity.y = self->groundVel * RSDK.Sin256(self->angle) >> 8;
    }
    else {
        self->velocity.y += 0x3800;
        self->groundVel = self->velocity.x;
    }
    self->playerControlled = false;
}

void CheckerBall_HandlePlayerMovement(void)
{
    RSDK_THIS(CheckerBall);

    Hitbox hitbox;
    hitbox.top    = -22;
    hitbox.bottom = 22;

    int32 playerID      = 0;
    self->activePlayers = 0;
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 acc            = player->acceleration >> 1;
        if (player->onGround) {
            if (player->position.x <= self->position.x) {
                hitbox.left  = -23;
                hitbox.right = 0;
                if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                    if (!player->right) {
                        if (self->velocity.x > 0 && player->groundVel > self->groundVel) {
                            player->velocity.x -= 0x10000;
                            player->groundVel -= 0x10000;
                        }
                    }
                    else {
                        self->activePlayers |= 1 << playerID;
                        if (self->groundVel < player->topSpeed)
                            self->groundVel += acc;
                        self->playerControlled = true;
                        player->position.x     = self->roundedPos.x + ((playerHitbox->left - 22) << 16);
                        player->groundVel      = 0;
                    }
                }
            }
            else {
                hitbox.left  = 0;
                hitbox.right = 23;
                if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                    if (!player->left) {
                        if (self->velocity.x < 0 && player->groundVel < self->groundVel) {
                            player->velocity.x += 0x10000;
                            player->groundVel += 0x10000;
                        }
                    }
                    else {
                        self->activePlayers |= 1 << playerID;
                        if (self->groundVel > -player->topSpeed)
                            self->groundVel -= acc;
                        self->playerControlled = true;
                        player->position.x     = self->roundedPos.x + ((playerHitbox->right + 22) << 16);
                        player->groundVel      = 0;
                    }
                }
            }
        }
        ++playerID;
    }
}

void CheckerBall_HandlePlayerInteractions(void)
{
    RSDK_THIS(CheckerBall);
    int32 x          = ((self->position.x - self->collisionOffset.x) & 0xFFFF0000) - self->roundedPos.x;
    int32 y          = ((self->position.y - self->collisionOffset.y) & 0xFFFF0000) - self->roundedPos.y;
    int32 startX     = self->position.x;
    int32 startY     = self->position.y;
    self->position.x = self->roundedPos.x;
    self->position.y = self->roundedPos.y;

    int32 playerID = 0;
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        if ((1 << playerID) & self->activePlayers) {
            if (player->position.x >= self->position.x)
                player->position.x = (22 - playerHitbox->left) << 16;
            else
                player->position.x = (-22 - playerHitbox->right) << 16;
            player->position.x += startX;
        }
        else {
            int32 side = Player_CheckCollisionBox(player, self, &CheckerBall->hitboxBall);
            if (side == C_BOTTOM && self->velocity.y >= 0 && player->onGround) {
                player->deathType = PLAYER_DEATH_DIE_USESFX;
            }
            else if (side == C_TOP) {
                player->position.x += x + (x >> 1) + self->collisionOffset.x;
                player->position.y += y + self->collisionOffset.y;

                if (player->velocity.x) {
                    if (player->velocity.x <= 0)
                        self->groundVel += player->acceleration >> 2;
                    else
                        self->groundVel -= player->acceleration >> 2;
                    self->playerControlled = true;
                }
                self->position.x = startX;
                self->position.y = startY;
                self->position.x = self->roundedPos.x;
                self->position.y = self->roundedPos.y;
                if (!Player_CheckCollisionBox(player, self, &CheckerBall->hitboxBall)) {
                    player->groundVel += x + (x >> 1);
                    player->velocity.x  = player->groundVel;
                    player->onGround    = false;
                    player->controlLock = 15;
                }
            }
        }
        ++playerID;
    }
    self->position.x        = startX;
    self->position.y        = startY;
    self->collisionOffset.x = 0;
    self->collisionOffset.y = 0;
}

void CheckerBall_BadnikBreak(void *b, Hitbox *hitbox)
{
    RSDK_THIS(CheckerBall);
    Entity *badnik = (Entity *)b;

    if (RSDK.CheckObjectCollisionTouchBox(badnik, hitbox, self, &CheckerBall->hitboxBall)) {
        BadnikHelpers_BadnikBreakUnseeded(badnik, false, true);

        EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityScoreBonus *bonus = CREATE_ENTITY(ScoreBonus, NULL, badnik->position.x, badnik->position.y);
        bonus->drawGroup        = Zone->objectDrawGroup[1];
        bonus->animator.frameID = player1->scoreBonus;
        switch (player1->scoreBonus) {
            case 0: Player_GiveScore(player1, 100); break;
            case 1: Player_GiveScore(player1, 200); break;
            case 2: Player_GiveScore(player1, 500); break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14: Player_GiveScore(player1, 1000); break;
            case 15: Player_GiveScore(player1, 10000); break;
            default: break;
        }

        if (player1->scoreBonus < 15)
            player1->scoreBonus++;

        destroyEntity(badnik);
    }
}

void CheckerBall_HandleObjectCollisions(void)
{
    RSDK_THIS(CheckerBall);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    foreach_active(PlaneSwitch, planeSwitch)
    {
        PlaneSwitch_CheckCollisions(planeSwitch, self, planeSwitch->flags, planeSwitch->size, true, Zone->playerDrawGroup[0],
                                    Zone->playerDrawGroup[1]);
    }

    foreach_active(CheckerBall, checkerBall)
    {
        if (checkerBall != self && self->collisionPlane == checkerBall->collisionPlane) {
            int32 pos = MIN(abs(self->position.x - checkerBall->position.x) >> 17, 23);

            Hitbox hitboxBall;
            hitboxBall.top    = CheckerBall->heightArray[pos] - 22 + (CheckerBall->heightArray[pos] >> 1);
            hitboxBall.bottom = -hitboxBall.top;
            hitboxBall.left   = -22;
            hitboxBall.right  = 22;

            switch (RSDK.CheckObjectCollisionBox(checkerBall, &hitboxBall, self, &hitboxBall, true)) {
                case C_TOP:
                    if (self->position.x >= checkerBall->position.x) {
                        self->groundVel += 0xC00;
                        self->velocity.x += 0xC00;
                        checkerBall->groundVel -= 0xC00;
                        checkerBall->velocity.x -= 0xC00;
                        checkerBall->angleVel -= 0xC00;
                    }
                    else {
                        self->groundVel -= 0xC00;
                        self->velocity.x -= 0xC00;
                        checkerBall->groundVel += 0xC00;
                        checkerBall->velocity.x += 0xC00;
                        checkerBall->angleVel += 0xC00;
                    }
                    break;

                case C_LEFT:
                case C_RIGHT:
                    if (self->groundVel <= 0) {
                        if (self->groundVel < 0 && self->position.x > checkerBall->position.x) {
                            self->velocity.x = checkerBall->velocity.x;
                            self->groundVel  = checkerBall->groundVel;
                        }
                    }
                    else if (self->position.x < checkerBall->position.x) {
                        self->velocity.x        = checkerBall->velocity.x;
                        self->groundVel         = checkerBall->groundVel;
                        checkerBall->groundVel  = self->groundVel;
                        checkerBall->velocity.x = self->velocity.x;
                    }
                    else if (self->groundVel < 0 && self->position.x > checkerBall->position.x) {
                        self->velocity.x = checkerBall->velocity.x;
                        self->groundVel  = checkerBall->groundVel;
                    }
                    break;

                case C_BOTTOM:
                    if (self->velocity.y <= 0) {
                        if (self->velocity.y < 0 && self->position.y > checkerBall->position.y) {
                            self->velocity.y        = checkerBall->velocity.y;
                            checkerBall->velocity.y = self->velocity.y;

                            if (self->onGround) {
                                self->velocity.x = checkerBall->velocity.x;
                                self->groundVel  = checkerBall->groundVel;
                            }

                            if (checkerBall->onGround) {
                                checkerBall->groundVel  = self->groundVel;
                                checkerBall->velocity.x = self->velocity.x;
                            }
                        }
                    }

                    if (self->position.y < checkerBall->position.y) {
                        self->velocity.y        = checkerBall->velocity.y;
                        checkerBall->velocity.y = self->velocity.y;

                        if (self->onGround) {
                            self->velocity.x = checkerBall->velocity.x;
                            self->groundVel  = checkerBall->groundVel;
                        }

                        if (checkerBall->onGround) {
                            checkerBall->groundVel  = self->groundVel;
                            checkerBall->velocity.x = self->velocity.x;
                        }
                    }
                    else {
                        if (self->velocity.y < 0 && self->position.y > checkerBall->position.y) {
                            self->velocity.y        = checkerBall->velocity.y;
                            checkerBall->velocity.y = self->velocity.y;

                            if (self->onGround) {
                                self->velocity.x = checkerBall->velocity.x;
                                self->groundVel  = checkerBall->groundVel;
                            }

                            if (checkerBall->onGround) {
                                checkerBall->groundVel  = self->groundVel;
                                checkerBall->velocity.x = self->velocity.x;
                            }
                        }
                    }
                    break;

                default: break;
            }

            if (RSDK.CheckObjectCollisionTouchBox(checkerBall, &hitboxBall, self, &hitboxBall)) {
                if (self->position.x >= checkerBall->position.x)
                    self->position.x += 0x10000;
                else
                    self->position.x -= 0x10000;

                if (self->position.y >= checkerBall->position.y)
                    self->position.y += 0x10000;
                else
                    self->position.y -= 0x10000;
            }
        }
    }

    foreach_active(Platform, platform)
    {
        if (platform->state != Platform_State_Falling2 && platform->state != Platform_State_Hold) {
            platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
            platform->position.y = platform->drawPos.y - platform->collisionOffset.y;

            int32 side = C_NONE;
            if (platform->collision)
                side = RSDK.CheckObjectCollisionBox(platform, &platform->hitbox, self, &CheckerBall->hitboxBall, true);
            else
                side = RSDK.CheckObjectCollisionPlatform(platform, &platform->hitbox, self, &CheckerBall->hitboxBall, true);

            if (side == C_TOP) {
                if (platform->state == Platform_State_Fall && !platform->timer)
                    platform->timer = 30;
                platform->stood = true;
                self->position.x += platform->collisionOffset.x;
                self->position.y += platform->collisionOffset.y;
                self->position.y &= 0xFFFF0000;
                self->onGround          = true;
                self->collisionOffset.x = platform->collisionOffset.x;
                self->collisionOffset.y = platform->collisionOffset.y;
            }
            platform->position.x = platform->centerPos.x;
            platform->position.y = platform->centerPos.y;
        }
    }

    foreach_active(Bridge, bridge) { Bridge_HandleCollisions(self, bridge, &CheckerBall->hitboxBall, true, false); }

    foreach_active(Batbrain, batBrain) { CheckerBall_BadnikBreak(batBrain, &Batbrain->hitboxBadnik); }

    foreach_active(BuzzBomber, buzzBomber)
    {
        if (buzzBomber->state == BuzzBomber_State_ProjectileCharge || buzzBomber->state == BuzzBomber_State_ProjectileShot) {
            if (RSDK.CheckObjectCollisionTouchBox(buzzBomber, &BuzzBomber->hitboxProjectile, self, &CheckerBall->hitboxBall)) {
                int32 angle            = RSDK.ATan2(self->position.x - buzzBomber->position.x, self->position.y - buzzBomber->position.y);
                buzzBomber->velocity.x = -0x800 * RSDK.Cos256(angle);
                buzzBomber->velocity.y = -0x800 * RSDK.Sin256(angle);
            }
        }
        else {
            CheckerBall_BadnikBreak(buzzBomber, &BuzzBomber->hitboxBadnik);
        }
    }

    foreach_active(Chopper, chopper) { CheckerBall_BadnikBreak(chopper, &Chopper->hitboxJump); }

    foreach_active(Crabmeat, crabmeat)
    {
        if (crabmeat->state == Crabmeat_State_Projectile) {
            if (RSDK.CheckObjectCollisionTouchBox(crabmeat, &Crabmeat->hitboxProjectile, self, &CheckerBall->hitboxBall)) {
                int32 angle          = RSDK.ATan2(self->position.x - crabmeat->position.x, self->position.y - crabmeat->position.y);
                crabmeat->velocity.x = -0x800 * RSDK.Cos256(angle);
                crabmeat->velocity.y = -0x800 * RSDK.Sin256(angle);
            }
        }
        else {
            CheckerBall_BadnikBreak(crabmeat, &Crabmeat->hitboxBadnik);
        }
    }

    foreach_active(Motobug, motobug)
    {
        if (motobug->state != Motobug_State_Smoke)
            CheckerBall_BadnikBreak(motobug, &Motobug->hitboxBadnik);
    }

    foreach_active(Newtron, newtron)
    {
        if (newtron->state == Newtron_State_Projectile) {
            if (RSDK.CheckObjectCollisionTouchBox(newtron, &Newtron->hitboxProjectile, self, &CheckerBall->hitboxBall)) {
                int32 angle         = RSDK.ATan2(self->position.x - newtron->position.x, self->position.y - newtron->position.y);
                newtron->velocity.x = -0x800 * RSDK.Cos256(angle);
                newtron->velocity.y = -0x800 * RSDK.Sin256(angle);
            }
        }
        else {
            if (newtron->type == NEWTRON_FLY)
                CheckerBall_BadnikBreak(newtron, &Newtron->hitboxFly);
            else
                CheckerBall_BadnikBreak(newtron, &Newtron->hitboxShoot);
        }
    }

    foreach_active(Splats, splats) { CheckerBall_BadnikBreak(splats, &Splats->hitboxBadnikGHZ); }

    foreach_active(ItemBox, itemBox)
    {
        if ((itemBox->state == ItemBox_State_Idle || itemBox->state == ItemBox_State_Falling)
            && RSDK.CheckObjectCollisionTouchBox(itemBox, &ItemBox->hitboxItemBox, self, &CheckerBall->hitboxBall)) {

            // This code is basically "ItemBox_Break"
            // "ItemBox_Break" was updated for plus stuff (mighty, swap monitors, etc)
            // But this is cant be in comp or effect mighty's velocity this was left unchanged

            RSDK.CreateEntity(TYPE_BLANK, NULL, itemBox->position.x, itemBox->position.y);
            itemBox->storedEntity  = (Entity *)player1;
            itemBox->alpha         = 0x100;
            itemBox->contentsSpeed = -0x38000;
            itemBox->active        = ACTIVE_ALWAYS;
            itemBox->velocity.y    = -0x20000;
            itemBox->state         = ItemBox_State_Break;
            RSDK.SetSpriteAnimation(ItemBox->aniFrames, 1, &itemBox->boxAnimator, true, 0);
            itemBox->boxAnimator.frameID = ItemBox->brokenFrame++;
            ItemBox->brokenFrame %= 3;
            RSDK.SetSpriteAnimation(-1, 0, &itemBox->overlayAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &itemBox->debrisAnimator, true, 0);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ITEMBOX), itemBox->position.x, itemBox->position.y - 0x100000);

            for (int32 i = 0; i < 6; ++i) {
                EntityDebris *debris    = CREATE_ENTITY(Debris, NULL, itemBox->position.x + RSDK.Rand(-0x80000, 0x80000),
                                                     itemBox->position.y + RSDK.Rand(-0x80000, 0x80000));
                debris->state           = Debris_State_Fall;
                debris->gravityStrength = 0x4000;
                debris->velocity.x      = RSDK.Rand(0, 0x20000);
                if (debris->position.x < itemBox->position.x)
                    debris->velocity.x = -debris->velocity.x;
                debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                debris->drawFX     = FX_FLIP;
                debris->direction  = i & 3;
                debris->drawGroup  = Zone->objectDrawGroup[1];
                RSDK.SetSpriteAnimation(ItemBox->aniFrames, 6, &debris->animator, true, RSDK.Rand(0, 4));
            }

            RSDK.PlaySfx(ItemBox->sfxDestroy, false, 0xFF);
            itemBox->active = ACTIVE_NORMAL;
            if (itemBox->type == ITEMBOX_RANDOM) {
                if (itemBox->type == ITEMBOX_1UP_SONIC) {
                    switch (player1->characterID) {
                        case ID_SONIC: itemBox->type = ITEMBOX_1UP_SONIC; break;
                        case ID_TAILS: itemBox->type = ITEMBOX_1UP_TAILS; break;
                        case ID_KNUCKLES: itemBox->type = ITEMBOX_1UP_KNUX; break;
#if MANIA_USE_PLUS
                        case ID_MIGHTY: itemBox->type = ITEMBOX_1UP_MIGHTY; break;
                        case ID_RAY: itemBox->type = ITEMBOX_1UP_RAY; break;
#endif
                        default: break;
                    }
                }
                itemBox->contentsAnimator.frameID = itemBox->type;
            }
        }
    }

    foreach_active(BreakableWall, breakableWall)
    {
        if (breakableWall->state == BreakableWall_State_Wall
            && RSDK.CheckObjectCollisionTouchBox(breakableWall, &breakableWall->hitbox, self, &CheckerBall->hitboxBall)) {
            int32 *offsets = BreakableWall->breakOffsets;

            int32 *velocities = NULL;
            if (self->position.x >= breakableWall->position.x)
                velocities = BreakableWall->breakVelocitiesR;
            else
                velocities = BreakableWall->breakVelocitiesL;

            for (int32 y = 0; y < 4; ++y) {
                for (int32 x = 0; x < 2; ++x) {
                    int32 tx                  = breakableWall->position.x + offsets[0];
                    int32 ty                  = breakableWall->position.y + offsets[1];
                    EntityBreakableWall *tile = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_FIXED), tx, ty);
                    tile->tileInfo            = RSDK.GetTile(Zone->fgLayer[1], tx >> 20, ty >> 20);
                    tile->velocity.x          = velocities[0];
                    tile->velocity.y          = velocities[1];
                    RSDK.SetTile(Zone->fgLayer[1], tx >> 20, ty >> 20, -1);
                    offsets += 2;
                    velocities += 2;
                }
            }

            RSDK.PlaySfx(BreakableWall->sfxBreak, false, 0xFF);
            destroyEntity(breakableWall);
        }
    }

    foreach_active(CollapsingPlatform, collapsingPlatform)
    {
        if (RSDK.CheckObjectCollisionTouchBox(collapsingPlatform, &collapsingPlatform->hitboxTrigger, self, &CheckerBall->hitboxBall)
            && self->onGround)
            collapsingPlatform->stoodPos.x = self->position.x;
    }

    foreach_active(Spikes, spikes)
    {
        if (RSDK.CheckObjectCollisionBox(spikes, &spikes->hitbox, self, &CheckerBall->hitboxBall, true)) {
            self->onGround = true;
            self->position.x += spikes->collisionOffset.x;
            self->position.y += spikes->collisionOffset.y;
        }
    }

    foreach_active(Spring, spring)
    {
        int32 side = RSDK.CheckObjectCollisionBox(spring, &spring->hitbox, self, &CheckerBall->hitboxBall, true);
        if (side) {
            bool32 bounced = false;

            if (spring->state == Spring_State_Vertical) {
                if ((spring->direction && side == C_BOTTOM) || (!spring->direction && side == C_TOP))
                    bounced = true;
            }
            else {
                if (spring->state == Spring_State_Diagonal) {
                    if (self->onGround || self->velocity.y >= 0 || abs(self->velocity.x) > -self->velocity.y)
                        bounced = true;
                }
                else if (spring->state == Spring_State_Horizontal && (!spring->onGround || self->onGround)) {
                    if ((!spring->direction && side == C_RIGHT) || (spring->direction && side == C_LEFT))
                        bounced = true;
                }
            }

            if (bounced) {
                if (spring->state != Spring_State_Vertical) {
                    self->groundVel  = spring->velocity.x;
                    self->velocity.x = spring->velocity.x;
                }

                if (spring->state != Spring_State_Horizontal) {
                    self->onGround   = false;
                    self->velocity.y = spring->velocity.y;
                }

                RSDK.SetSpriteAnimation(Spring->aniFrames, spring->type, &spring->animator, true, 0);
                RSDK.PlaySfx(Spring->sfxSpring, false, 0xFF);
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void CheckerBall_EditorDraw(void) { CheckerBall_Draw(); }

void CheckerBall_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        CheckerBall->aniFrames = RSDK.LoadSpriteAnimation("GHZ/CheckerBall.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("Blueprint"))
        CheckerBall->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CheckerBall.bin", SCOPE_STAGE);
}
#endif

void CheckerBall_Serialize(void) {}
