// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Caterkiller Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectCaterkiller *Caterkiller = NULL;

void Caterkiller_Update(void)
{
    RSDK_THIS(Caterkiller);
    StateMachine_Run(self->state);
}

void Caterkiller_LateUpdate(void) {}

void Caterkiller_StaticUpdate(void) {}

void Caterkiller_Draw(void)
{
    RSDK_THIS(Caterkiller);
    StateMachine_Run(self->stateDraw);
}

void Caterkiller_Create(void *data)
{
    RSDK_THIS(Caterkiller);
    self->visible = true;
    if (self->planeFilter > 0 && ((uint8)(self->planeFilter - 1) & 2))
        self->drawOrder = Zone->objectDrawHigh;
    else
        self->drawOrder = Zone->objectDrawLow;
    self->drawFX |= FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (data) {
        self->state = (Type_StateMachine)data;
        if (self->state == Caterkiller_StateSplit_Head)
            RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 0, &self->headAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 1, &self->headAnimator, true, 0);
        self->stateDraw = Caterkiller_Draw_Segment;
    }
    else {
        self->startPos = self->position;
        self->startDir = self->direction;

        self->headOffset = 0;
        int32 offset         = 0xC0000;
        if (self->startDir)
            offset = -0xC0000;
        int32 posX = self->position.x;
        for (int32 i = 0; i < Caterkiller_BodyCount; ++i) {
            posX += offset;
            self->bodyPosition[i].x = posX;
            self->bodyPosition[i].y = self->position.y;
            self->bodyDirection[i]  = self->direction;
        }

        self->timer = 0;
        RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 0, &self->headAnimator, true, 0);
        RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 1, &self->bodyAnimator, true, 0);
        self->state     = Caterkiller_State_Setup;
        self->stateDraw = Caterkiller_Draw_Body;
    }
}

void Caterkiller_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        Caterkiller->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Caterkiller.bin", SCOPE_STAGE);

    Caterkiller->hitbox.left   = -8;
    Caterkiller->hitbox.top    = -8;
    Caterkiller->hitbox.right  = 8;
    Caterkiller->hitbox.bottom = 8;

    DEBUGMODE_ADD_OBJ(Caterkiller);
}

void Caterkiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Caterkiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Caterkiller, NULL, self->position.x, self->position.y);
}

void Caterkiller_CheckOffScreen(void)
{
    RSDK_THIS(Caterkiller);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Caterkiller_Create(NULL);
    }
}

void Caterkiller_CheckTileCollisions(void)
{
    RSDK_THIS(Caterkiller);
    int32 storeX = 0;
    int32 storeY = 0;

    if (self->state == Caterkiller_State_LiftHead) {
        storeX = self->position.x;
        storeY = self->position.y;
    }
    else {
        if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 8))
            self->direction ^= FLIP_X;
        storeX = self->position.x;
        storeY = self->position.y;
        if (Caterkiller_CheckTileAngle(self->position.x, self->position.y, self->direction))
            self->direction ^= FLIP_X;
    }

    for (int32 i = 0; i < Caterkiller_BodyCount; ++i) {
        if (self->state != Caterkiller_State_LowerHead || i != (Caterkiller_BodyCount - 1)) {
            self->position.x = self->bodyPosition[i].x;
            self->position.y = self->bodyPosition[i].y;
            if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 8))
                self->bodyDirection[i] = self->direction;
            self->bodyPosition[i].y = self->position.y;
            if (Caterkiller_CheckTileAngle(self->bodyPosition[i].x, self->bodyPosition[i].y, self->bodyDirection[i]))
                self->bodyDirection[i] = self->direction;
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void Caterkiller_Draw_Body(void)
{
    RSDK_THIS(Caterkiller);
    Vector2 drawPos;

    int32 storeDir = self->direction;
    for (int32 i = Caterkiller_BodyCount - 1; i >= 0; --i) {
        drawPos.x = self->bodyPosition[i].x;
        drawPos.y = self->bodyPosition[i].y;
        drawPos.y -= self->bodyOffset[i] << 15;
        self->direction = self->bodyDirection[i];
        RSDK.DrawSprite(&self->bodyAnimator, &drawPos, false);
    }

    drawPos = self->position;
    drawPos.y -= self->headOffset << 15;
    self->direction = storeDir;
    RSDK.DrawSprite(&self->headAnimator, &drawPos, false);
}

void Caterkiller_Draw_Segment(void)
{
    RSDK_THIS(Caterkiller);
    RSDK.DrawSprite(&self->headAnimator, NULL, false);
}

void Caterkiller_HandlePlayerInteractions(void)
{
    RSDK_THIS(Caterkiller);
    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == ((self->planeFilter - 1) & 1)) {
            if (Player_CheckBadnikTouch(player, self, &Caterkiller->hitbox)) {
                Player_CheckBadnikBreak(player, self, true);
            }
            else {
                for (int32 i = 0; i < Caterkiller_BodyCount; ++i) {
                    self->position.x = self->bodyPosition[i].x;
                    self->position.y = self->bodyPosition[i].y;

                    if (Player_CheckCollisionTouch(player, self, &Caterkiller->hitbox)) {
                        Player_CheckHit(player, self);

                        for (int32 d = 0; d < Caterkiller_BodyCount + 1; ++d) {
                            int32 spawnX       = storeX;
                            int32 spawnY       = storeY;
                            int32 spawnDir     = self->direction;
                            void *spawnState = Caterkiller_StateSplit_Head;
                            if (d) {
                                spawnX     = self->bodyPosition[d - 1].x;
                                spawnY     = self->bodyPosition[d - 1].y;
                                spawnDir   = self->bodyDirection[d - 1];
                                spawnState = Caterkiller_StateSplit_Body;
                            }

                            EntityCaterkiller *debris = CREATE_ENTITY(Caterkiller, spawnState, spawnX, spawnY);
                            debris->direction         = spawnDir;
                            if (!debris->direction)
                                debris->velocity.x = (d & 1) ? -0x18000 : -0x20000;
                            else
                                debris->velocity.x = (d & 1) ? 0x18000 : 0x20000;

                            if ((d & 3) >= 2)
                                debris->velocity.x = -debris->velocity.x;

                            debris->velocity.y       = -0x40000;
                            if (!d)
                                debris->headAnimator.frameID = self->headAnimator.frameID;
                            debris->planeFilter      = self->planeFilter;
                            debris->drawOrder        = self->drawOrder;
                        }

                        destroyEntity(self);
                        self->active = ACTIVE_DISABLED;
                        break;
                    }
                }
            }
            self->position.x = storeX;
            self->position.y = storeY;
        }
    }
}

bool32 Caterkiller_CheckTileAngle(int32 x, int32 y, int32 dir)
{
    int32 ty      = (y >> 16) + 8;
    int32 tx      = x >> 16;
    uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, x >> 16, ty);
    if (tile == (uint16)-1)
        tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
    uint8 angle = RSDK.GetTileAngle(tile, 0, CMODE_FLOOR);

    if (dir) {
        if ((uint8)(angle + 127) <= 0x66)
            return true;
    }
    else if ((uint8)(angle - 25) <= 0x66) 
        return true;

    return false;
}

void Caterkiller_State_Setup(void)
{
    RSDK_THIS(Caterkiller);
    self->active = ACTIVE_NORMAL;
    self->state  = Caterkiller_State_Contract;

    Caterkiller_State_Contract();
}

void Caterkiller_State_Contract(void)
{
    RSDK_THIS(Caterkiller);
    if (self->timer) {
        self->timer--;
        Caterkiller_HandlePlayerInteractions();
        Caterkiller_CheckOffScreen();
    }
    else {
        self->timer                = 15;
        self->headAnimator.frameID = 1;
        self->state                = Caterkiller_State_LiftHead;
        Caterkiller_State_LiftHead();
    }
}

void Caterkiller_State_LiftHead(void)
{
    RSDK_THIS(Caterkiller);
    if (self->timer) {
        self->timer--;
        for (int32 b = 0; b < Caterkiller_BodyCount; ++b) {
            if (self->bodyDirection[b])
                self->bodyPosition[b].x += 0x4000 * (b + 1);
            else
                self->bodyPosition[b].x -= 0x4000 * (b + 1);
        }
        ++self->headOffset;
        ++self->bodyOffset[Caterkiller_BodyCount / 2];
    }
    else {
        self->timer = 7;
        self->state = Caterkiller_State_Uncontract;
    }
    Caterkiller_CheckTileCollisions();
    Caterkiller_HandlePlayerInteractions();
    Caterkiller_CheckOffScreen();
}

void Caterkiller_State_Uncontract(void)
{
    RSDK_THIS(Caterkiller);
    if (self->timer) {
        self->timer--;
        Caterkiller_HandlePlayerInteractions();
        Caterkiller_CheckOffScreen();
    }
    else {
        self->timer            = 15;
        self->headAnimator.frameID = 0;
        self->state            = Caterkiller_State_LowerHead;
        Caterkiller_State_LowerHead();
    }
}

void Caterkiller_State_LowerHead(void)
{
    RSDK_THIS(Caterkiller);
    if (self->timer) {
        self->timer--;

        int32 mult = 1;
        for (int32 b = Caterkiller_BodyCount - 2; b >= 0; --b) {
            if (self->bodyDirection[b])
                self->bodyPosition[b].x += 0x4000 * mult;
            else
                self->bodyPosition[b].x -= 0x4000 * mult;
            ++mult;
        }

        if (self->direction)
            self->position.x += 0x4000 * mult;
        else
            self->position.x -= 0x4000 * mult;

        --self->headOffset;
        --self->bodyOffset[Caterkiller_BodyCount / 2];
    }
    else {
        self->timer = 7;
        self->state = Caterkiller_State_Contract;
    }
    Caterkiller_CheckTileCollisions();
    Caterkiller_HandlePlayerInteractions();
    Caterkiller_CheckOffScreen();
}

void Caterkiller_StateSplit_Head(void)
{
    RSDK_THIS(Caterkiller);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        if (self->velocity.y > 0 && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 4)) {
            self->velocity.y = -0x40000;
        }

        foreach_active(Player, player)
        {
            if ((self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
                && Player_CheckBadnikTouch(player, self, &Caterkiller->hitbox)) {
                Player_CheckBadnikBreak(player, self, true);
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void Caterkiller_StateSplit_Body(void)
{
    RSDK_THIS(Caterkiller);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        if (self->velocity.y > 0 && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 4)) {
            self->velocity.y = -0x40000;
        }

        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, self, &Caterkiller->hitbox)) 
                    Player_CheckHit(player, self);
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Caterkiller_EditorDraw(void)
{
    RSDK_THIS(Caterkiller);
    
    self->startPos = self->position;

    self->headOffset = 0;
    int32 offset       = 0xC0000;
    if (self->startDir)
        offset = -0xC0000;
    int32 posX = self->position.x;
    for (int32 i = 0; i < Caterkiller_BodyCount; ++i) {
        posX += offset;
        self->bodyPosition[i].x = posX;
        self->bodyPosition[i].y = self->position.y;
        self->bodyDirection[i]  = self->direction;
    }

    RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 0, &self->headAnimator, true, 0);
    RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 1, &self->bodyAnimator, true, 0);

    Caterkiller_Draw_Body();
}

void Caterkiller_EditorLoad(void)
{
    Caterkiller->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Caterkiller.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Caterkiller, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}
#endif

void Caterkiller_Serialize(void) { RSDK_EDITABLE_VAR(Caterkiller, VAR_ENUM, planeFilter); }
