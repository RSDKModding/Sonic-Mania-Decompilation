// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ChemicalPool Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectChemicalPool *ChemicalPool;

void ChemicalPool_Update(void)
{
    RSDK_THIS(ChemicalPool);

    if (self->timer > 0)
        self->timer--;
    self->offsetY = self->position.y - self->updateRange.y;
    StateMachine_Run(self->state);
}

void ChemicalPool_LateUpdate(void) {}

void ChemicalPool_StaticUpdate(void) {}

void ChemicalPool_Draw(void)
{
    RSDK_THIS(ChemicalPool);
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];
    Vector2 drawPos;
    Vector2 verts[4];

    int32 screenX = self->position.x - (screen->position.x << 16);

    int32 screenY = screen->position.y << 16;
    int32 vy      = self->position.y + self->updateRange.y - screenY;

    Vector2 *vertices = self->vertices;
    for (int32 i = 0; i < self->tileSizeX; ++i) {
        verts[0].x = vertices[0].x + screenX;
        verts[0].y = vertices[0].y - screenY;
        verts[1].x = vertices[2].x + screenX;
        verts[1].y = vertices[2].y - screenY;
        verts[2].x = vertices[3].x + screenX;
        verts[2].y = vertices[3].y - screenY;
        verts[3].x = vertices[1].x + screenX;
        verts[3].y = vertices[1].y - screenY;

        colour *surfaceColours = NULL;
        if (self->timer >= 1)
            surfaceColours = ChemicalPool->surfaceColoursFlash;
        else
            surfaceColours = self->surfaceColours;
        RSDK.DrawBlendedQuad(verts, surfaceColours, 4, 0xC0, INK_NONE);

        verts[0].x = verts[3].x;
        verts[0].y = verts[3].y;
        verts[1].x = verts[2].x;
        verts[1].y = verts[2].y;
        verts[2].y = vy;
        verts[3].y = vy;
        if (self->timer >= 1)
            RSDK.DrawQuad(verts, 4, 0xE0, 0xE0, 0xE0, 0xC0, INK_ALPHA);
        else
            RSDK.DrawQuad(verts, 4, self->r, self->g, self->b, 0xC0, INK_ALPHA);

        vertices += 2;
    }

    if (self->type > CHEMICALPOOL_BLUE) // da reflection
    {
        int32 x = ((self->position.x - self->updateRange.x) >> 16) - screen->position.x;
        int32 y = ((self->position.y - self->updateRange.y) >> 16) - screen->position.y;
        RSDK.SetClipBounds(SceneInfo->currentScreenID, x, y, x + (self->size.x >> 16), y + (self->size.y >> 16));

        drawPos.x = self->position.x - ((self->position.x - (screen->position.x << 16) - (ScreenInfo->centerX << 16)) >> 1);

        int32 offset = 0x800000 - self->size.y;
        offset       = minVal(offset, 0x400000);
        offset       = maxVal(offset, 0);

        drawPos.y = offset + self->position.y + self->maxDeform - ((self->position.y - (screen->centerY << 16) - (screen->position.y << 16)) >> 4);
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, screen->width, screen->height);
    }
}

void ChemicalPool_Create(void *data)
{
    RSDK_THIS(ChemicalPool);
    if (!SceneInfo->inEditor) {
        self->inkEffect     = INK_ADD;
        self->visible       = true;
        self->alpha         = 0xFF;
        self->active        = ACTIVE_BOUNDS;
        self->drawOrder     = Zone->drawOrderHigh - 2;
        self->updateRange.x = self->size.x >> 1;
        self->updateRange.y = self->size.y >> 1;
        self->tileSizeX     = self->size.x >> 20;

        Vector2 *vertices = self->vertices;
        for (int32 i = 0; i < 0x2100000; i += 0x100000) {
            int32 x       = i - (self->size.x >> 1);
            vertices[0].x = x;
            vertices[1].x = x;

            int32 y       = self->position.y - (self->size.y >> 1);
            vertices[0].y = y;
            vertices[1].y = y + 0x80000;

            vertices += 2;
        }

        self->startX           = (self->position.x - (self->size.x >> 1)) >> 20;
        self->endX             = (self->position.x + (self->size.x >> 1)) >> 20;
        self->impactPower      = 0x80;
        self->impactPowerSides = 0x99;
        self->r                = 0x00;
        self->g                = 0x08;
        self->b                = 0xC0;
        self->timer            = 0;
        self->hitbox.left      = -(self->size.x >> 17);
        self->hitbox.top       = (self->type == CHEMICALPOOL_BLUE ? 6 : 0) - (self->size.y >> 17);
        self->hitbox.right     = -self->hitbox.left;
        self->hitbox.bottom    = -self->hitbox.bottom;
        ChemicalPool_SetupColours();
        RSDK.SetSpriteAnimation(ChemicalPool->aniFrames, 0, &self->animator, true, 0);
    }
}

void ChemicalPool_StageLoad(void)
{
    ChemicalPool->aniFrames     = RSDK.LoadSpriteAnimation("CPZ/ChemPool.bin", SCOPE_STAGE);
    ChemicalPool->sfxChemYellow = RSDK.GetSfx("CPZ/ChemYellow.wav");
    ChemicalPool->sfxChemRed    = RSDK.GetSfx("CPZ/ChemRed.wav");
    ChemicalPool->sfxChemChange = RSDK.GetSfx("CPZ/ChemChange.wav");
}

void ChemicalPool_ChangeState(EntityChemicalPool *chemPool, int32 newType, int32 newR, int32 newG, int32 newB)
{
    chemPool->r                 = chemPool->r + ((newR - chemPool->r) >> 2);
    chemPool->b                 = chemPool->b + ((newB - chemPool->b) >> 2);
    chemPool->g                 = chemPool->g + ((newG - chemPool->g) >> 2);
    chemPool->surfaceColours[2] = (chemPool->r << 16) | (chemPool->g << 8) | chemPool->b;
    chemPool->surfaceColours[3] = (chemPool->r << 16) | (chemPool->g << 8) | chemPool->b;

    uint32 change = abs(newR - chemPool->r) + abs(newG - chemPool->g) + abs(newB - chemPool->b) - 1;
    if (change < 0x1F && chemPool->state != ChemicalPool_State_Change) {
        chemPool->type  = newType;
        chemPool->timer = 4;
        chemPool->state = ChemicalPool_State_Change;
        RSDK.PlaySfx(ChemicalPool->sfxChemChange, false, 255);
    }
}

void ChemicalPool_ProcessDeformations(void)
{
    RSDK_THIS(ChemicalPool);

    for (int32 i = self->startX; i < self->endX; ++i) {
        int32 deform = ChemicalPool->surfaceDeformation[i];
        ChemicalPool->impactTable[i] += (-deform >> 5) - (ChemicalPool->impactTable[i] >> 3);
        ChemicalPool->deformTable[i] = ChemicalPool->impactTable[i] + ChemicalPool->surfaceDeformation[i];
    }

    for (int32 i = self->startX; i < self->endX; ++i) {
        int32 prev = i - 1;
        if (prev < self->startX)
            prev = self->startX;

        int32 next = i + 1;
        if (next >= self->endX)
            next = self->endX;

        ChemicalPool->surfaceDeformation[i] = (self->impactPower * ChemicalPool->deformTable[i] >> 8)
                                              + (self->impactPowerSides * (ChemicalPool->deformTable[prev] + ChemicalPool->deformTable[next]) >> 9);
    }

    self->maxDeform   = 0;
    int32 id          = self->startX;
    Vector2 *vertices = self->vertices;
    for (int32 i = 0; i <= self->tileSizeX; ++i) {
        if (ChemicalPool->surfaceDeformation[id] > self->maxDeform)
            self->maxDeform = ChemicalPool->surfaceDeformation[id];
        vertices[0].y = ChemicalPool->surfaceDeformation[id] + self->offsetY;

        int32 deform = 2 * ChemicalPool->surfaceDeformation[id] + 0x80000;
        if (deform < ChemicalPool->surfaceDeformation[id])
            deform = ChemicalPool->surfaceDeformation[id] + 0x10000;
        vertices[1].y = deform + self->offsetY;

        ++id;
        vertices += 2;
    }
}

void ChemicalPool_SetDeform(int32 impactX, int32 impactVelocity)
{
    int32 bounceX = (impactX + 0x80000) >> 20;
    foreach_active(ChemicalPool, chemPool)
    {
        if (bounceX > chemPool->startX && bounceX < chemPool->endX) {
            int32 prev = bounceX - 1;
            if (prev <= chemPool->startX)
                prev = chemPool->startX + 1;

            int32 next = minVal(bounceX + 2, chemPool->endX);

            for (int32 i = 0; i < next - prev; ++i) {
                ChemicalPool->impactTable[prev + i] += impactVelocity;
            }
            foreach_break;
        }
    }
}

void ChemicalPool_SpawnDebris(int32 x, int32 y)
{
    RSDK_THIS(ChemicalPool);

    for (int32 i = 0; i < 6; ++i) {
#if RETRO_USE_PLUS
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x + RSDK.RandSeeded(-0x80000, 0x80000, &Zone->randSeed),
                                             y + RSDK.RandSeeded(0x40000, 0x140000, &Zone->randSeed));
        debris->state        = Debris_State_Fall;
        debris->gravity      = 0x3800;
        debris->velocity.x   = RSDK.RandSeeded(0, 0x20000, &Zone->randSeed);
        if (debris->position.x < x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.RandSeeded(-0x40000, -0x10000, &Zone->randSeed);
        debris->drawOrder  = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Reagent->aniFrames, self->type + 1, &debris->animator, true, RSDK.RandSeeded(0, 2, &Zone->randSeed));
#else
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x + RSDK.Rand(-0x80000, 0x80000), y + RSDK.Rand(0x40000, 0x140000));
        debris->state        = Debris_State_Fall;
        debris->gravity      = 0x3800;
        debris->velocity.x   = RSDK.Rand(0, 0x20000);
        if (debris->position.x < x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
        debris->drawOrder  = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Reagent->aniFrames, self->type + 1, &debris->animator, true, RSDK.Rand(0, 2));
#endif
    }
}

void ChemicalPool_SetupColours(void)
{
    RSDK_THIS(ChemicalPool);

    switch (self->type) {
        case CHEMICALPOOL_BLUE:
        default:
            self->surfaceColours[0] = 0x0060E0;
            self->surfaceColours[1] = 0x0060E0;
            self->surfaceColours[2] = 0x0008C0;
            self->surfaceColours[3] = 0x0008C0;
            self->r                 = 0x00;
            self->g                 = 0x08;
            self->b                 = 0xC0;
            self->state             = ChemicalPool_State_HarmfulBlue;
            self->timer             = 3;
            break;

        case CHEMICALPOOL_GREEN:
            self->surfaceColours[0] = 0x20E020;
            self->surfaceColours[1] = 0x20E020;
            self->surfaceColours[2] = 0x189000;
            self->surfaceColours[3] = 0x189000;
            self->r                 = 0x18;
            self->g                 = 0x90;
            self->b                 = 0x00;
            self->state             = ChemicalPool_State_Green;
            self->timer             = 3;
            break;

        case CHEMICALPOOL_CYAN:
            self->surfaceColours[0] = 0x00B8F0;
            self->surfaceColours[1] = 0x00B8F0;
            self->surfaceColours[2] = 0x0080B0;
            self->surfaceColours[3] = 0x0080B0;
            self->r                 = 0x00;
            self->g                 = 0x80;
            self->b                 = 0xB0;
            self->state             = ChemicalPool_State_Cyan;
            self->timer             = 3;
            break;
    }
}

void ChemicalPool_State_HarmfulBlue(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();
    int32 topStore = self->hitbox.top;

    foreach_active(Player, player)
    {
        if (player->velocity.y >= 0 && Player_CheckCollisionBox(player, self, &self->hitbox)) {
            ChemicalPool_SetDeform(player->position.x, (player->velocity.y >> 4) + 0x1000);
            if (player->shield != SHIELD_BUBBLE) {
                if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown
                    && !player->invincibleTimer && player->blinkTimer <= 0) {
                    if (player->position.x > self->position.x)
                        player->velocity.x = 0x20000;
                    else
                        player->velocity.x = -0x20000;
                    Player_Hit(player);
                }
            }
            if (player->velocity.y < 0 && player->velocity.y > -0x40000)
                player->velocity.y = -0x40000;
        }
    }

    self->hitbox.top = topStore;
    if (!(Zone->timer & 3)) {
        EntityChemBubble *bubble = CREATE_ENTITY(ChemBubble, NULL, self->position.x + RSDK.Rand(-self->updateRange.x, self->updateRange.x),
                                                 self->position.y + self->updateRange.y);
        bubble->parent           = (Entity *)self;
    }
}

void ChemicalPool_State_Green(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            ChemicalPool_SetDeform(player->position.x, player->velocity.y >> 1);
            ChemicalPool_SpawnDebris(player->position.x, player->position.y);
            if (player->state != Player_State_DropDash)
                player->state = Player_State_Air;

            int32 anim = player->animator.animationID;
            if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                player->animator.animationID = ANI_WALK;
            player->onGround    = false;
            player->jumpAbility = 0;
            player->velocity.y  = -0x140000;
            RSDK.PlaySfx(ChemicalPool->sfxChemRed, false, 255);
        }
    }
}

void ChemicalPool_State_Cyan(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();

    foreach_active(Player, player)
    {
        if (player->velocity.y >= 0) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                ChemicalPool_SetDeform(player->position.x, player->velocity.y >> 1);
                ChemicalPool_SpawnDebris(player->position.x, player->position.y);
                if (player->state != Player_State_DropDash)
                    player->state = Player_State_Air;

                int32 anim = player->animator.animationID;
                if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                    player->animator.animationID = ANI_WALK;
                if ((-0x10000 - player->velocity.y) > -0x80000)
                    player->velocity.y = -0x10000 - player->velocity.y;
                else
                    player->velocity.y = -0x80000;
                player->onGround    = false;
                player->jumpAbility = 0;
                RSDK.PlaySfx(ChemicalPool->sfxChemYellow, false, 255);
            }
        }
    }
}

void ChemicalPool_State_Change(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();
    if (++self->changeTimer == 16) {
        foreach_active(ChemBubble, bubble)
        {
            if (abs(bubble->position.x - self->position.x) <= self->updateRange.x) {
                if (abs(bubble->position.y - self->position.y) <= self->updateRange.y) {
                    destroyEntity(bubble);
                }
            }
        }
        self->changeTimer = 0;
        ChemicalPool_SetupColours();
        self->active = ACTIVE_BOUNDS;
    }
}

#if RETRO_INCLUDE_EDITOR
void ChemicalPool_EditorDraw(void)
{
    RSDK_THIS(ChemicalPool);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->size.x >> 1;
    drawPos.y -= self->size.y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + self->size.x, drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, self->size.y + drawPos.y, drawPos.x + self->size.x, self->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + self->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + self->size.x, drawPos.y, drawPos.x + self->size.x, drawPos.y + self->size.y, 0xFFFF00, 0, INK_NONE, false);

    RSDK.DrawRect(drawPos.x, drawPos.y, self->size.x, self->size.y, (self->r << 16) | (self->g << 8) | (self->b << 0), 0xC0, INK_ALPHA, true);
}

void ChemicalPool_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(ChemicalPool, type);
    RSDK_ENUM_VAR("Blue", CHEMICALPOOL_BLUE);
    RSDK_ENUM_VAR("Green", CHEMICALPOOL_GREEN);
    RSDK_ENUM_VAR("Cyan", CHEMICALPOOL_CYAN);
}
#endif

void ChemicalPool_Serialize(void)
{
    RSDK_EDITABLE_VAR(ChemicalPool, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ChemicalPool, VAR_VECTOR2, size);
}
