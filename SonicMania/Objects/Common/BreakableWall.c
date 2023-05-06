// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BreakableWall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBreakableWall *BreakableWall;

void BreakableWall_Update(void)
{
    RSDK_THIS(BreakableWall);

    StateMachine_Run(self->state);
}

void BreakableWall_LateUpdate(void) {}

void BreakableWall_StaticUpdate(void) {}

void BreakableWall_Draw(void)
{
    RSDK_THIS(BreakableWall);

    StateMachine_Run(self->stateDraw);
}

void BreakableWall_Create(void *data)
{
    RSDK_THIS(BreakableWall);

    self->gravityStrength = 0x3800;

    if (data) {
        int32 type          = VOID_TO_INT(data);
        self->visible       = true;
        self->updateRange.x = TO_FIXED(16);
        self->updateRange.y = TO_FIXED(16);
        self->active        = ACTIVE_NORMAL;
        self->drawFX        = FX_FLIP | (self->velocity.x != 0 ? FX_ROTATE : FX_NONE);
        self->tileRotation  = RSDK.Rand(-8, 8);

        switch (type) {
            case BREAKWALL_TILE_FIXED:
                self->state     = BreakableWall_State_Tile;
                self->stateDraw = BreakableWall_Draw_Tile;
                break;

            default:
            case BREAKWALL_TILE_DYNAMIC:
                self->state     = BreakableWall_State_FallingTile;
                self->stateDraw = StateMachine_None;
                break;
        }
    }
    else {
        self->drawFX |= FX_FLIP;

        if (!SceneInfo->inEditor) {
            self->visible       = false;
            self->drawGroup     = Zone->objectDrawGroup[1];
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = TO_FIXED(128);
            self->updateRange.y = TO_FIXED(128);
            self->priority      = self->priority == BREAKWALL_PRIO_HIGH ? Zone->fgLayer[1] : Zone->fgLayer[0];

            self->size.x >>= 0x10;
            self->size.y >>= 0x10;

            switch (self->type) {
                case BREAKWALL_TYPE_WALL:
                    if (!self->size.x) {
                        self->size.x = 2;
                        self->size.y = 4;
                    }

                    self->state     = BreakableWall_State_Wall;
                    self->stateDraw = BreakableWall_Draw_Wall;
                    break;

                case BREAKWALL_TYPE_FLOOR:
                    if (!self->size.x) {
                        self->size.x = 2;
                        self->size.y = 2;
                    }

                    self->state     = BreakableWall_State_Floor;
                    self->stateDraw = BreakableWall_Draw_Floor;
                    break;

                case BREAKWALL_TYPE_BURROWFLOOR:
                case BREAKWALL_TYPE_BURROWFLOOR_B:
                    if (!self->size.x)
                        self->size.x = 2;

                    self->state     = BreakableWall_State_BurrowFloor;
                    self->stateDraw = BreakableWall_Draw_Floor;
                    break;

                case BREAKWALL_TYPE_BURROWFLOORUP:
                    if (!self->size.x)
                        self->size.x = 2;

                    self->state     = BreakableWall_State_BurrowFloorUp;
                    self->stateDraw = BreakableWall_Draw_Floor;
                    break;

                case BREAKWALL_TYPE_CEILING:
                    if (!self->size.x) {
                        self->size.x = 2;
                        self->size.y = 2;
                    }

                    self->state     = BreakableWall_State_Ceiling;
                    self->stateDraw = BreakableWall_Draw_Floor;
                    break;

                default: break;
            }

            self->hitbox.left   = -(8 * self->size.x);
            self->hitbox.top    = -(8 * self->size.y);
            self->hitbox.right  = 8 * self->size.x;
            self->hitbox.bottom = 8 * self->size.y;
        }
    }
}

void BreakableWall_StageLoad(void)
{
    BreakableWall->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(BreakableWall->aniFrames, 0, &BreakableWall->animator, true, 0);

    if (RSDK.CheckSceneFolder("AIZ"))
        BreakableWall->sfxBreak = RSDK.GetSfx("Stage/LedgeBreak3.wav");
    else
        BreakableWall->sfxBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");

    BreakableWall->farPlaneLayer = RSDK.GetTileLayerID("Far Plane");
}

// States
void BreakableWall_State_BurrowFloorUp(void)
{
    RSDK_THIS(BreakableWall);

    self->visible = DebugMode->debugActive;

    BreakableWall_CheckBreak_BurrowFloorUp();
}
void BreakableWall_State_Ceiling(void)
{
    RSDK_THIS(BreakableWall);

    self->visible = DebugMode->debugActive;

    BreakableWall_CheckBreak_Ceiling();
}
void BreakableWall_State_FallingTile(void)
{
    RSDK_THIS(BreakableWall);

    if (--self->timer <= 0) {
        RSDK.SetTile(self->targetLayer, self->tilePos.x, self->tilePos.y, -1);

        if (self->drawGroup < Zone->objectDrawGroup[0] && BreakableWall->farPlaneLayer != (uint16)-1) {
            RSDK.SetTile(BreakableWall->farPlaneLayer, self->tilePos.x, self->tilePos.y, -1);
        }

        self->state     = BreakableWall_State_Tile;
        self->stateDraw = BreakableWall_Draw_Tile;
    }
}
void BreakableWall_State_Wall(void)
{
    RSDK_THIS(BreakableWall);

    self->visible = DebugMode->debugActive;

    BreakableWall_CheckBreak_Wall();
}
void BreakableWall_State_Tile(void)
{
    RSDK_THIS(BreakableWall);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += self->gravityStrength;

    if (self->velocity.x)
        self->rotation += self->tileRotation;

    if (self->drawGroup >= Zone->objectDrawGroup[0]) {
        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
    else {
        if (++self->timer == 120)
            destroyEntity(self);
    }
}
void BreakableWall_State_Floor(void)
{
    RSDK_THIS(BreakableWall);

    self->visible = DebugMode->debugActive;

    BreakableWall_CheckBreak_Floor();
}
void BreakableWall_State_BurrowFloor(void)
{
    RSDK_THIS(BreakableWall);

    self->visible = DebugMode->debugActive;

    BreakableWall_CheckBreak_BurrowFloor();
}

// Draw States
void BreakableWall_Draw_Wall(void)
{
    RSDK_THIS(BreakableWall);

    Vector2 drawPos;
    drawPos.x = self->position.x - (self->size.x << 19);
    drawPos.y = self->position.y - (self->size.y << 19);

    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + (self->size.x << 20), drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y + (self->size.y << 20), drawPos.x + (self->size.x << 20), drawPos.y + (self->size.y << 20), 0xE0E0E0,
                  0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + (self->size.y << 20), 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + (self->size.x << 20), drawPos.y - 0x10000, drawPos.x + (self->size.x << 20), drawPos.y + (self->size.y << 20), 0xE0E0E0,
                  0, INK_NONE, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x += self->size.x << 20;
    self->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.y += self->size.y << 20;
    self->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x -= self->size.x << 20;
    self->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);
}

// The... same function as above?
void BreakableWall_Draw_Floor(void)
{
    RSDK_THIS(BreakableWall);

    Vector2 drawPos;
    drawPos.x = self->position.x - (self->size.x << 19);
    drawPos.y = self->position.y - (self->size.y << 19);

    RSDK.DrawLine(drawPos.x - TO_FIXED(1), drawPos.y - TO_FIXED(1), drawPos.x + (self->size.x << 20), drawPos.y - TO_FIXED(1), 0xE0E0E0, 0x00,
                  INK_NONE, false);
    RSDK.DrawLine(drawPos.x - TO_FIXED(1), drawPos.y + (self->size.y << 20), drawPos.x + (self->size.x << 20), drawPos.y + (self->size.y << 20),
                  0xE0E0E0, 0x00, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - TO_FIXED(1), drawPos.y - TO_FIXED(1), drawPos.x - TO_FIXED(1), drawPos.y + (self->size.y << 20), 0xE0E0E0, 0x00,
                  INK_NONE, false);
    RSDK.DrawLine(drawPos.x + (self->size.x << 20), drawPos.y - TO_FIXED(1), drawPos.x + (self->size.x << 20), drawPos.y + (self->size.y << 20),
                  0xE0E0E0, 0x00, INK_NONE, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x += self->size.x << 20;
    self->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.y += self->size.y << 20;
    self->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x -= self->size.x << 20;
    self->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);
}
void BreakableWall_Draw_Tile(void)
{
    RSDK_THIS(BreakableWall);

    self->angle = self->rotation;
    RSDK.DrawTile(&self->tileInfo, 1, 1, NULL, NULL, false);
}

// Breaking
void BreakableWall_CheckBreak_Wall(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
#if MANIA_USE_PLUS
        if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_HAMMERDROP)) {
#endif
            if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                bool32 canBreak = abs(player->groundVel) >= 0x48000 && player->onGround && player->animator.animationID == ANI_JUMP;

                if (player->shield == SHIELD_FIRE) {
                    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield);
                    canBreak |= shield->shieldAnimator.animationID == SHIELDANI_FIREATTACK;
                }

                switch (player->characterID) {
                    default: break;

                    case ID_SONIC: canBreak |= player->superState == SUPERSTATE_SUPER; break;

                    case ID_KNUCKLES: canBreak = true; break;
                }

                if (player->state == Ice_PlayerState_Frozen)
                    canBreak |= abs(player->groundVel) >= 0x48000;

                if (canBreak && !player->sidekick) {
                    if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                        BreakableWall_Break(self, player->position.x > self->position.x);

                        if (player->characterID == ID_KNUCKLES) {
                            if (player->animator.animationID == ANI_GLIDE) {
                                player->abilitySpeed -= player->abilitySpeed >> 2;
                                player->velocity.x -= player->velocity.x >> 2;
                                if (abs(player->velocity.x) <= 0x30000) {
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_GLIDE_DROP, &player->animator, false, 0);
                                    player->state = Player_State_KnuxGlideDrop;
                                }
                            }
                            else if (player->animator.animationID == ANI_GLIDE_SLIDE) {
                                player->abilitySpeed -= player->abilitySpeed >> 2;
                                player->velocity.x -= player->velocity.x >> 2;
                            }
                        }

                        RSDK.PlaySfx(BreakableWall->sfxBreak, false, 255);
                        destroyEntity(self);
                    }

                    continue; // skip to next loop, so we dont do the box collision
                }
            }
#if MANIA_USE_PLUS
        }
#endif
        Player_CheckCollisionBox(player, self, &self->hitbox);
    }
}
void BreakableWall_CheckBreak_Floor(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
#if MANIA_USE_PLUS
        int32 velY = player->velocity.y;
#endif

        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
#if MANIA_USE_PLUS
            if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_HAMMERDROP)) {
#endif
                if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                    bool32 canBreak = player->animator.animationID == ANI_JUMP;

                    switch (player->characterID) {
                        default: break;

                        case ID_SONIC:
                            if (!canBreak)
                                canBreak = player->animator.animationID == ANI_DROPDASH;
                            break;

                        case ID_KNUCKLES: canBreak = true; break;

#if MANIA_USE_PLUS
                        case ID_MIGHTY:
                            if (!canBreak)
                                canBreak = player->state == Player_State_MightyHammerDrop;
                            break;
#endif
                    }

                    if (player->groundedStore && player->collisionMode != CMODE_LWALL && player->collisionMode != CMODE_RWALL)
                        canBreak = false;

                    if (canBreak && !player->sidekick) {
                        player->onGround = false;

                        BreakableWall_Break(self, FLIP_Y);

                        RSDK.PlaySfx(BreakableWall->sfxBreak, false, 255);
                        BreakableWall_GiveScoreBonus(player);

#if MANIA_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = velY - TO_FIXED(1);
                        else
#endif
                            player->velocity.y = -TO_FIXED(3);

                        destroyEntity(self);

                        continue; // skip to next loop, so we dont do the box collision
                    }
                }
#if MANIA_USE_PLUS
            }
#endif
        }

        Player_CheckCollisionBox(player, self, &self->hitbox);
    }
}
void BreakableWall_CheckBreak_BurrowFloor(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
#if MANIA_USE_PLUS
        int32 velY = player->velocity.y;
#endif
        bool32 onGround = player->onGround;

        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP && !player->sidekick
            && ((player->collisionPlane == 1 && self->type == BREAKWALL_TYPE_BURROWFLOOR_B) || self->type == BREAKWALL_TYPE_BURROWFLOOR)) {
#if MANIA_USE_PLUS
            if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_HAMMERDROP)) {
#endif
                if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                    bool32 canBreak = player->animator.animationID == ANI_JUMP;

                    switch (player->characterID) {
                        default: break;

                        case ID_SONIC:
                            if (!canBreak)
                                canBreak = player->animator.animationID == ANI_DROPDASH;
                            break;

                        case ID_KNUCKLES: canBreak = true; break;

#if MANIA_USE_PLUS
                        case ID_MIGHTY:
                            if (!canBreak)
                                canBreak = player->state == Player_State_MightyHammerDrop;
                            break;
#endif
                    }

                    if (onGround && player->collisionMode != CMODE_LWALL && player->collisionMode != CMODE_RWALL)
                        canBreak = false;

                    if (canBreak && !player->sidekick) {
                        player->onGround = false;

                        int32 sizeX = self->size.x;
                        int32 sizeY = self->size.y;
                        int32 posX  = self->position.x;
                        int32 posY  = self->position.y;

                        self->size.y = 1;
                        self->position.y += 0x80000 - (sizeY << 19);

                        BreakableWall_Break(self, FLIP_Y);

                        self->size.x     = sizeX;
                        self->size.y     = sizeY;
                        self->position.x = posX;
                        self->position.y = posY;
                        RSDK.PlaySfx(BreakableWall->sfxBreak, false, 255);

                        BreakableWall_GiveScoreBonus(player);

#if MANIA_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = velY - TO_FIXED(1);
                        else
#endif
                            player->velocity.y = 0;

                        self->hitbox.top += 8;
                        self->hitbox.bottom -= 8;
                        --self->size.y;
                        self->position.y += TO_FIXED(8);

                        if (self->size.y <= 0)
                            destroyEntity(self);
                    }
                }
#if MANIA_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_CheckBreak_BurrowFloorUp(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_BOTTOM) {
#if MANIA_USE_PLUS
            if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_HAMMERDROP)) {
#endif
                if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                    if (!player->sidekick) {
                        int32 sizeX = self->size.x;
                        int32 sizeY = self->size.y;
                        int32 posX  = self->position.x;
                        int32 posY  = self->position.y;

                        if (sizeY > 2)
                            self->size.y = 2;
                        self->position.y += (sizeY - self->size.y) << 19;

                        BreakableWall_Break(self, FLIP_Y);

                        self->size.x     = sizeX;
                        self->size.y     = sizeY;
                        self->position.x = posX;
                        self->position.y = posY;
                        RSDK.PlaySfx(BreakableWall->sfxBreak, false, 255);

                        player->velocity.y = 0;

                        if (self->size.y < 2) {
                            self->hitbox.top += 8;
                            self->size.y -= 1;
                            self->hitbox.bottom -= 8;
                            self->position.y -= TO_FIXED(8);
                        }
                        else {
                            self->hitbox.top += 16;
                            self->size.y -= 2;
                            self->hitbox.bottom -= 16;
                            self->position.y -= TO_FIXED(16);
                        }

                        if (self->size.y <= 0)
                            destroyEntity(self);

                        player->velocity.y = velY;
                    }
                }
#if MANIA_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_CheckBreak_Ceiling(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_BOTTOM) {
#if MANIA_USE_PLUS
            if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_HAMMERDROP)) {
#endif
                if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                    player->onGround = false;

                    BreakableWall_Break(self, FLIP_Y);

                    RSDK.PlaySfx(BreakableWall->sfxBreak, false, 0xFF);
                    player->velocity.y = velY;
                    destroyEntity(self);
                }
#if MANIA_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_Break(EntityBreakableWall *self, uint8 direction)
{
    int32 startX = self->position.x;
    int32 startY = self->position.y;
    int32 endX   = self->position.x - (self->size.x << 19) + TO_FIXED(8);
    int32 endY   = self->position.y - (self->size.y << 19) + TO_FIXED(8);

    switch (direction) {
        case FLIP_NONE: startX += self->size.x << 19; break;
        case FLIP_X: startX -= self->size.x << 19; break;
        case FLIP_Y: startY += self->size.y << 19; break;
        case FLIP_XY: startY -= self->size.y << 19; break;
        default: break;
    }

    int32 curY = endY - startY;
    for (int32 y = 0; y < self->size.y; ++y) {
        int32 curX   = endX - startX;
        int32 tileY  = (curY + startY) >> 20;
        int32 angleX = 2 * (endX - startX);

        for (int32 x = 0; x < self->size.x; ++x) {
            int32 tileX               = (curX + startX) >> 20;
            EntityBreakableWall *tile = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_FIXED), curX + startX, curY + startY);
            tile->tileInfo            = RSDK.GetTile(self->priority, tileX, tileY);
            tile->drawGroup           = self->drawGroup;

            switch (direction) {
                case FLIP_NONE:
                case FLIP_X: {
                    int32 angle  = RSDK.ATan2(angleX, curY);
                    int32 angle2 = 0;
                    if (abs(curX) > 0x80000) {
                        if (curX + startX >= startX)
                            angle2 = RSDK.ATan2(TO_FIXED(8), curY);
                        else
                            angle2 = RSDK.ATan2(-TO_FIXED(8), curY);
                    }
                    else {
                        angle2 = RSDK.ATan2(curX, curY);
                    }

                    tile->velocity.x = direction == FLIP_NONE ? -TO_FIXED(1) : TO_FIXED(1);
                    tile->velocity.y = TO_FIXED(1);
                    tile->velocity.x += 40 * (((self->size.y << 19) + 3 * abs(curX) - abs(curY)) >> 18) * RSDK.Cos256(angle);
                    tile->velocity.y += 32 * ((abs(curY) + abs(curX) + 2 * abs(curY)) >> 18) * RSDK.Sin256(angle2);
                    break;
                }

                case FLIP_Y: {
                    int32 angle    = RSDK.ATan2(angleX, curY);
                    int32 velocity = (abs(curX) + 3 * abs(curY)) >> 18;

                    tile->velocity.x += 40 * velocity * RSDK.Cos256(angle);
                    tile->velocity.y += 40 * velocity * RSDK.Sin256(angle);
                    break;
                }
            }

            RSDK.SetTile(self->priority, tileX, tileY, -1);
            if (self->drawGroup < Zone->objectDrawGroup[0]) {
                if (BreakableWall->farPlaneLayer != (uint16)-1)
                    RSDK.SetTile(BreakableWall->farPlaneLayer, tileX, tileY, -1);
            }

            curX += TO_FIXED(16);
            angleX += TO_FIXED(32);
        }

        curY += TO_FIXED(16);
    }
}

// Misc
void BreakableWall_GiveScoreBonus(EntityPlayer *player)
{
    RSDK_THIS(BreakableWall);

    EntityScoreBonus *scoreBonus = CREATE_ENTITY(ScoreBonus, NULL, self->position.x, self->position.y);
    scoreBonus->drawGroup        = Zone->objectDrawGroup[1];
    scoreBonus->animator.frameID = player->scoreBonus;

    switch (player->scoreBonus) {
        case 0: Player_GiveScore(player, 100); break;
        case 1: Player_GiveScore(player, 200); break;
        case 2: Player_GiveScore(player, 500); break;

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
        case 14: Player_GiveScore(player, 1000); break;

        case 15: Player_GiveScore(player, 10000); break;

        default: break;
    }

    if (player->scoreBonus < 15)
        player->scoreBonus++;
}

#if GAME_INCLUDE_EDITOR
void BreakableWall_EditorDraw(void)
{
    RSDK_THIS(BreakableWall);

    int32 sizeX = self->size.x;
    int32 sizeY = self->size.y;

    switch (self->type) {
        case BREAKWALL_TYPE_WALL:
            if (!sizeX) {
                sizeX = TO_FIXED(2);
                sizeY = TO_FIXED(4);
            }
            break;

        case BREAKWALL_TYPE_FLOOR:
            if (!sizeX) {
                sizeX = TO_FIXED(2);
                sizeY = TO_FIXED(2);
            }
            break;

        case BREAKWALL_TYPE_BURROWFLOOR:
        case BREAKWALL_TYPE_BURROWFLOOR_B:
            if (!sizeX)
                sizeX = TO_FIXED(2);
            break;

        case BREAKWALL_TYPE_BURROWFLOORUP:
            if (!sizeX)
                sizeX = TO_FIXED(2);
            break;

        case BREAKWALL_TYPE_CEILING:
            if (!sizeX) {
                sizeX = TO_FIXED(2);
                sizeY = TO_FIXED(2);
            }
            break;

        default: break;
    }

    sizeX <<= 4;
    sizeY <<= 4;

    Vector2 drawPos;
    drawPos.x = self->position.x - sizeX;
    drawPos.y = self->position.y - sizeY;

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, sizeX, sizeY, 0xFFFF00);

    drawPos.x += sizeX >> 1;
    drawPos.y += sizeY >> 1;

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x += sizeX;
    self->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.y += sizeY;
    self->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x -= sizeX;
    self->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);
}

void BreakableWall_EditorLoad(void)
{
    BreakableWall->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(BreakableWall->aniFrames, 0, &BreakableWall->animator, true, 0);

    RSDK_ACTIVE_VAR(BreakableWall, type);
    RSDK_ENUM_VAR("Wall", BREAKWALL_TYPE_WALL);
    RSDK_ENUM_VAR("Floor", BREAKWALL_TYPE_FLOOR);
    RSDK_ENUM_VAR("Burrow Floor", BREAKWALL_TYPE_BURROWFLOOR);
    RSDK_ENUM_VAR("Burrow Floor (Plane B Only)", BREAKWALL_TYPE_BURROWFLOOR_B);
    RSDK_ENUM_VAR("Burrow Floor Up", BREAKWALL_TYPE_BURROWFLOORUP);
    RSDK_ENUM_VAR("Ceiling", BREAKWALL_TYPE_CEILING);

    RSDK_ACTIVE_VAR(BreakableWall, priority);
    RSDK_ENUM_VAR("FG High", BREAKWALL_PRIO_HIGH);
    RSDK_ENUM_VAR("FG Low", BREAKWALL_PRIO_LOW);
}
#endif

void BreakableWall_Serialize(void)
{
    RSDK_EDITABLE_VAR(BreakableWall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_BOOL, onlyKnux);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_BOOL, onlyMighty);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_ENUM, priority);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_VECTOR2, size);
}
