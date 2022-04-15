// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BreakableWall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
        int32 type          = voidToInt(data);
        self->visible       = true;
        self->updateRange.x = 0x100000;
        self->updateRange.y = 0x100000;
        self->active        = ACTIVE_NORMAL;
        self->drawFX        = 2 * (self->velocity.x != 0) + 1;
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
            self->drawOrder     = Zone->drawOrderHigh;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            if (self->priority == BREAKWALL_PRIO_HIGH)
                self->priority = Zone->fgHigh;
            else
                self->priority = Zone->fgLow;
            self->size.y >>= 0x10;
            self->size.x >>= 0x10;
            switch (self->type) {
                case BREAKWALL_TYPE_SIDES:
                    self->state     = BreakableWall_State_BreakableSides;
                    self->stateDraw = BreakableWall_Draw_Outline;
                    if (!self->size.x) {
                        self->size.x = 2;
                        self->size.y = 4;
                    }
                    break;
                case BREAKWALL_TYPE_TOP:
                    if (!self->size.x) {
                        self->size.x = 2;
                        self->size.y = 2;
                    }
                    self->state     = BreakableWall_State_Top;
                    self->stateDraw = BreakableWall_Draw_Outline2;
                    break;
                case BREAKWALL_TYPE_TOPCHUNK:
                case BREAKWALL_TYPE_TOPCHUNK_HIGH:
                    if (!self->size.x)
                        self->size.x = 2;
                    self->state     = BreakableWall_State_TopChunks;
                    self->stateDraw = BreakableWall_Draw_Outline2;
                    break;
                case BREAKWALL_TYPE_BOTTOMCHUNK:
                    if (!self->size.x)
                        self->size.x = 2;
                    self->state     = BreakableWall_State_BottomChunks;
                    self->stateDraw = BreakableWall_Draw_Outline2;
                    break;
                case BREAKWALL_TYPE_BOTTOMFULL:
                    if (!self->size.x) {
                        self->size.x = 2;
                        self->size.y = 2;
                    }
                    self->state     = BreakableWall_State_BottomFull;
                    self->stateDraw = BreakableWall_Draw_Outline2;
                    break;
                default: break;
            }
            int32 x             = 8 * self->size.x;
            int32 y             = 8 * self->size.y;
            self->hitbox.right  = x;
            self->hitbox.left   = -x;
            self->hitbox.bottom = y;
            self->hitbox.top    = -y;
        }
    }
}

void BreakableWall_StageLoad(void)
{
    BreakableWall->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(BreakableWall->aniFrames, 0, &BreakableWall->animator, true, 0);
    if (RSDK.CheckStageFolder("AIZ"))
        BreakableWall->sfxBreak = RSDK.GetSfx("Stage/LedgeBreak3.wav");
    else
        BreakableWall->sfxBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");
    BreakableWall->farPlaneLayer = RSDK.GetSceneLayerID("Far Plane");
}

// States
void BreakableWall_State_BottomChunks(void)
{
    RSDK_THIS(BreakableWall);
    self->visible = DebugMode->debugActive;
    BreakableWall_HandleBottomBreak_Chunks();
}
void BreakableWall_State_BottomFull(void)
{
    RSDK_THIS(BreakableWall);
    self->visible = DebugMode->debugActive;
    BreakableWall_HandleBottomBreak_All();
}
void BreakableWall_State_FallingTile(void)
{
    RSDK_THIS(BreakableWall);
    if (--self->timer <= 0) {
        RSDK.SetTileInfo(self->layerID, self->tilePos.x, self->tilePos.y, -1);
        if (self->drawOrder < Zone->drawOrderLow) {
            if (BreakableWall->farPlaneLayer != (uint16)-1)
                RSDK.SetTileInfo(BreakableWall->farPlaneLayer, self->tilePos.x, self->tilePos.y, -1);
        }
        self->state     = BreakableWall_State_Tile;
        self->stateDraw = BreakableWall_Draw_Tile;
    }
}
void BreakableWall_State_BreakableSides(void)
{
    RSDK_THIS(BreakableWall);
    self->visible = DebugMode->debugActive;
    BreakableWall_HandleSidesBreak();
}
void BreakableWall_State_Tile(void)
{
    RSDK_THIS(BreakableWall);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += self->gravityStrength;
    if (self->velocity.x)
        self->rotation += self->tileRotation;
    if (self->drawOrder >= Zone->drawOrderLow) {
        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
    else {
        if (++self->timer == 120)
            destroyEntity(self);
    }
}
void BreakableWall_State_Top(void)
{
    RSDK_THIS(BreakableWall);
    self->visible = DebugMode->debugActive;
    BreakableWall_HandleTopBreak_All();
}
void BreakableWall_State_TopChunks(void)
{
    RSDK_THIS(BreakableWall);
    self->visible = DebugMode->debugActive;
    BreakableWall_HandleTopBreak_Chunks();
}

// Draw States
void BreakableWall_Draw_Outline(void)
{
    RSDK_THIS(BreakableWall);
    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->size.x << 19;
    drawPos.y -= self->size.y << 19;

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
void BreakableWall_Draw_Outline2(void)
{
    RSDK_THIS(BreakableWall);
    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->size.x << 19;
    drawPos.y -= self->size.y << 19;

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
void BreakableWall_Draw_Tile(void)
{
    RSDK_THIS(BreakableWall);
    self->angle = self->rotation;
    RSDK.DrawTile(&self->tileInfo, 1, 1, NULL, NULL, false);
}

// Breaking
void BreakableWall_HandleTopBreak_All(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
#if RETRO_USE_PLUS
            if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_DROPDASH)) {
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
#if RETRO_USE_PLUS
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
                        int32 ty         = self->position.y - (self->size.y << 19) + 0x80000;
                        int32 th         = self->position.y - (self->size.y << 19) + 0x80000 - ((self->size.y << 19) + self->position.y);
                        for (int32 y = 0; y < self->size.y; ++y) {
                            int32 tx          = self->position.x - (self->size.x << 19) + 0x80000;
                            int32 posY        = ty >> 20;
                            int32 speed       = 3 * abs(th);
                            int32 offsetX     = tx - self->position.x;
                            int32 blockSpeedX = 2 * (tx - self->position.x);
                            for (int32 x = 0; x < self->size.x; ++x) {
                                int32 posX                     = tx >> 20;
                                EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), tx, ty);
                                tileChunk->tileInfo            = RSDK.GetTileInfo(self->priority, posX, posY);
                                tileChunk->drawOrder           = self->drawOrder;
                                int32 angle                    = RSDK.ATan2(blockSpeedX, th);
                                int32 spd                      = (speed + abs(offsetX)) >> 18;
                                tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
                                tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
                                RSDK.SetTileInfo(self->priority, posX, posY, -1);

                                if (self->drawOrder < Zone->drawOrderLow) {
                                    if (BreakableWall->farPlaneLayer != (uint16)-1)
                                        RSDK.SetTileInfo(BreakableWall->farPlaneLayer, posX, posY, -1);
                                }

                                blockSpeedX += 0x200000;
                                tx += 0x100000;
                                offsetX += 0x100000;
                            }
                            ty += 0x100000;
                            th += 0x100000;
                        }

                        RSDK.PlaySfx(BreakableWall->sfxBreak, false, 255);
                        BreakableWall_GiveScoreBonus(player);
#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = velY - 0x10000;
                        else
#endif
                            player->velocity.y = -0x30000;
                        destroyEntity(self);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_HandleTopBreak_Chunks(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY      = player->velocity.y;
        bool32 onGround = player->onGround;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP && !player->sidekick
            && ((player->collisionPlane == 1 && self->type != 2) || self->type == C_LEFT)) {
#if RETRO_USE_PLUS
            if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_DROPDASH)) {
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
#if RETRO_USE_PLUS
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
                        BreakableWall_HandleBlockBreak_V();
                        BreakableWall_GiveScoreBonus(player);

#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = velY - 0x10000;
                        else
#endif
                            player->velocity.y = 0;
                        self->hitbox.top += 8;
                        self->hitbox.bottom -= 8;
                        --self->size.y;
                        self->position.y += 0x80000;
                        if (self->size.y <= 0)
                            destroyEntity(self);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_HandleSidesBreak(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
#if RETRO_USE_PLUS
        if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_DROPDASH)) {
#endif
            if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                bool32 canBreak = abs(player->groundVel) >= 0x48000 && player->onGround && player->animator.animationID == ANI_JUMP;

                if (player->shield == SHIELD_FIRE) {
                    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                    canBreak |= shield->shieldAnimator.animationID == 2;
                }

                switch (player->characterID) {
                    default: break;
                    case ID_SONIC:
                        canBreak |= player->animator.animationID == ANI_DROPDASH;
                        canBreak |= player->superState == SUPERSTATE_SUPER;
                        break;
                    case ID_KNUCKLES: canBreak = true; break;
                }

                if (player->state == Ice_State_FrozenPlayer) {
                    canBreak |= abs(player->groundVel) >= 0x48000;
                }

                if (canBreak && !player->sidekick) {
                    if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                        BreakableWall_HandleBlockBreak_H(self, player->position.x > self->position.x);
                        if (player->characterID == ID_KNUCKLES) {
                            if (player->animator.animationID == ANI_FLY) {
                                player->abilitySpeed -= player->abilitySpeed >> 2;
                                player->velocity.x -= player->velocity.x >> 2;
                                if (abs(player->velocity.x) <= 0x30000) {
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLYTIRED, &player->animator, false, 0);
                                    player->state = Player_State_KnuxGlideDrop;
                                }
                            }
                            else if (player->animator.animationID == ANI_FLYLIFTTIRED) {
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
#if RETRO_USE_PLUS
        }
#endif
        Player_CheckCollisionBox(player, self, &self->hitbox);
    }
}
void BreakableWall_HandleBottomBreak_Chunks(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_BOTTOM) {
#if RETRO_USE_PLUS
            if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_DROPDASH)) {
#endif
                if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                    if (!player->sidekick) {
                        BreakableWall_HandleBlockBreak_V();
                        player->velocity.y = 0;

                        if (self->size.y < 2) {
                            self->hitbox.top += 8;
                            self->size.y -= 1;
                            self->hitbox.bottom -= 8;
                            self->position.y -= 0x80000;
                        }
                        else {
                            self->hitbox.top += 16;
                            self->size.y -= 2;
                            self->hitbox.bottom -= 16;
                            self->position.y -= 0x100000;
                        }

                        if (self->size.y <= 0)
                            destroyEntity(self);
                        player->velocity.y = velY;
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_HandleBottomBreak_All(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_BOTTOM) {
#if RETRO_USE_PLUS
            if (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_DROPDASH)) {
#endif
                if (!self->onlyKnux || player->characterID == ID_KNUCKLES) {
                    player->onGround = false;
                    int32 tx         = self->position.x - (self->size.x << 19) + 0x80000;
                    int32 tw         = self->position.x - (self->size.x << 19) + 0x80000;
                    int32 ty         = self->position.y - (self->size.y << 19) + 0x80000;
                    int32 th         = (self->size.y << 19) + self->position.y;
                    int32 offsetY    = ty - th;
                    for (int32 y = 0; y < self->size.y; ++y) {
                        int32 posY        = ty >> 20;
                        int32 speed       = 3 * abs(offsetY);
                        int32 offsetX     = tx - self->position.x;
                        int32 blockSpeedX = 2 * (tx - self->position.x);
                        for (int32 x = 0; x < self->size.x; ++x) {
                            int32 posX                     = tx >> 20;
                            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), tx, ty);
                            tileChunk->tileInfo            = RSDK.GetTileInfo(self->priority, posX, posY);
                            tileChunk->drawOrder           = self->drawOrder;
                            int32 angle                    = RSDK.ATan2(blockSpeedX, offsetY);
                            int32 spd                      = (speed + abs(offsetX)) >> 18;
                            tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
                            tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
                            RSDK.SetTileInfo(self->priority, posX, posY, -1);

                            if (self->drawOrder < Zone->drawOrderLow) {
                                if (BreakableWall->farPlaneLayer != (uint16)-1)
                                    RSDK.SetTileInfo(BreakableWall->farPlaneLayer, posX, posY, -1);
                            }

                            blockSpeedX += 0x200000;
                            tx += 0x100000;
                            offsetX += 0x100000;
                        }
                        ty += 0x100000;
                        tx = tw;
                        offsetY += 0x100000;
                    }

                    RSDK.PlaySfx(BreakableWall->sfxBreak, false, 0xFF);
                    player->velocity.y = velY;
                    destroyEntity(self);
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_HandleBlockBreak_V(void)
{
    RSDK_THIS(BreakableWall);

    int32 sizeX = self->size.x;
    int32 sizeY = self->size.y;
    int32 posX  = self->position.x;
    int32 posY  = self->position.y;
    if (self->state == BreakableWall_State_BottomChunks) {
        if (sizeY > 2)
            self->size.y = 2;
        self->position.y += (sizeY - self->size.y) << 19;
    }
    else {
        self->size.y = 1;
        self->position.y += 0x80000 - (sizeY << 19);
    }

    int32 startX = self->position.x;
    int32 endX   = self->position.x - (self->size.x << 19) + 0x80000;
    int32 curY   = self->position.y - (self->size.y << 19) + 0x80000;
    int32 startY = (self->size.y << 19) + self->position.y;

    int32 distY = curY - startY;
    for (int32 y = 0; y < self->size.y; ++y) {
        int32 tx     = endX;
        int32 distX  = endX - startX;
        int32 tileY  = curY >> 20;
        int32 angleX = 2 * distX;
        int32 speed  = 3 * abs(distY);
        for (int32 x = 0; x < self->size.x; ++x) {
            int32 tileX                    = tx >> 20;
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), tx, curY);
            tileChunk->tileInfo            = RSDK.GetTileInfo(self->priority, tileX, tileY);
            tileChunk->drawOrder           = self->drawOrder;

            int32 angle = RSDK.ATan2(angleX, distY);
            int32 spd   = (speed + abs(distX)) >> 18;
            tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
            tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
            RSDK.SetTileInfo(self->priority, tileX, tileY, -1);
            if (self->drawOrder < Zone->drawOrderLow) {
                if (BreakableWall->farPlaneLayer != (uint16)-1)
                    RSDK.SetTileInfo(BreakableWall->farPlaneLayer, tileX, tileY, -1);
            }

            tx += 0x100000;
            distX += 0x100000;
            angleX += 0x200000;
        }
        curY += 0x100000;
        distY += 0x100000;
    }

    self->size.x     = sizeX;
    self->size.y     = sizeY;
    self->position.x = posX;
    self->position.y = posY;
    RSDK.PlaySfx(BreakableWall->sfxBreak, false, 255);
}
void BreakableWall_HandleBlockBreak_H(EntityBreakableWall *self, uint8 flip)
{
    int32 startX = self->position.x;
    int32 startY = self->position.y;
    int32 endX   = self->position.x - (self->size.x << 19) + 0x80000;
    int32 endY   = self->position.y - (self->size.y << 19) + 0x80000;
    switch (flip) {
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
            int32 tileX                    = (curX + startX) >> 20;
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), curX + startX, curY + startY);
            tileChunk->tileInfo            = RSDK.GetTileInfo(self->priority, tileX, tileY);
            tileChunk->drawOrder           = self->drawOrder;

            switch (flip) {
                case FLIP_NONE:
                case FLIP_X: {
                    int32 angle  = RSDK.ATan2(angleX, curY);
                    int32 angle2 = 0;
                    if (abs(curX) > 0x80000) {
                        if (curX + startX >= startX)
                            angle2 = RSDK.ATan2(0x80000, curY);
                        else
                            angle2 = RSDK.ATan2(-0x80000, curY);
                    }
                    else {
                        angle2 = RSDK.ATan2(curX, curY);
                    }
                    tileChunk->velocity.x = flip == FLIP_NONE ? -0x10000 : 0x10000;
                    tileChunk->velocity.y = 0x10000;
                    tileChunk->velocity.x += 40 * (((self->size.y << 19) + 3 * abs(curX) - abs(curY)) >> 18) * RSDK.Cos256(angle);
                    tileChunk->velocity.y += 32 * ((abs(curY) + abs(curX) + 2 * abs(curY)) >> 18) * RSDK.Sin256(angle2);
                    break;
                }
                case FLIP_Y: {
                    int32 angle = RSDK.ATan2(angleX, curY);
                    tileChunk->velocity.x += 40 * ((abs(curX) + 3 * abs(curY)) >> 18) * RSDK.Cos256(angle);
                    tileChunk->velocity.y += 40 * ((abs(curX) + 3 * abs(curY)) >> 18) * RSDK.Sin256(angle);
                }
            }

            RSDK.SetTileInfo(self->priority, tileX, tileY, -1);
            if (self->drawOrder < Zone->drawOrderLow) {
                if (BreakableWall->farPlaneLayer != (uint16)-1)
                    RSDK.SetTileInfo(BreakableWall->farPlaneLayer, tileX, tileY, -1);
            }
            curX += 0x10 << 16;
            angleX += 0x200000;
        }
        curY += 0x10 << 16;
    }
}

// Misc
void BreakableWall_GiveScoreBonus(void *plr)
{
    EntityPlayer *player = (EntityPlayer *)plr;
    RSDK_THIS(BreakableWall);
    EntityScoreBonus *scoreBonus = CREATE_ENTITY(ScoreBonus, NULL, self->position.x, self->position.y);
    scoreBonus->drawOrder        = Zone->drawOrderHigh;
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
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE: Player_GiveScore(player, 1000); break;
        case 0xF: Player_GiveScore(player, 10000); break;
        default: break;
    }
    if (player->scoreBonus < 15)
        player->scoreBonus++;
}

#if RETRO_INCLUDE_EDITOR
void BreakableWall_EditorDraw(void)
{
    RSDK_THIS(BreakableWall);

    int sizeX = self->size.x;
    int sizeY = self->size.y;

    switch (self->type) {
        case BREAKWALL_TYPE_SIDES:
            if (!sizeX) {
                sizeX = 2 << 16;
                sizeY = 4 << 16;
            }
            break;
        case BREAKWALL_TYPE_TOP:
            if (!sizeX) {
                sizeX = 2 << 16;
                sizeY = 2 << 16;
            }
            break;
        case BREAKWALL_TYPE_TOPCHUNK:
        case BREAKWALL_TYPE_TOPCHUNK_HIGH:
            if (!sizeX)
                sizeX = 2 << 16;
            break;
        case BREAKWALL_TYPE_BOTTOMCHUNK:
            if (!sizeX)
                sizeX = 2 << 16;
            break;
        case BREAKWALL_TYPE_BOTTOMFULL:
            if (!sizeX) {
                sizeX = 2 << 16;
                sizeY = 2 << 16;
            }
            break;
        default: break;
    }
    sizeX <<= 4;
    sizeY <<= 4;

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= sizeX;
    drawPos.y -= sizeY;

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
    RSDK_ENUM_VAR("Sides", BREAKWALL_TYPE_SIDES);
    RSDK_ENUM_VAR("Top", BREAKWALL_TYPE_TOP);
    RSDK_ENUM_VAR("Top Chunks", BREAKWALL_TYPE_TOPCHUNK);
    RSDK_ENUM_VAR("Top Chunks (High Collision Only)", BREAKWALL_TYPE_TOPCHUNK_HIGH);
    RSDK_ENUM_VAR("Bottom Chunks", BREAKWALL_TYPE_BOTTOMCHUNK);
    RSDK_ENUM_VAR("Bottom", BREAKWALL_TYPE_BOTTOMFULL);

    RSDK_ACTIVE_VAR(BreakableWall, priority);
    RSDK_ENUM_VAR("FG High", 0);
    RSDK_ENUM_VAR("FG Low", 1);
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
