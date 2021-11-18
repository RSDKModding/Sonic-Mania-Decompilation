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

    Vector2 *vertPtr = self->field_A0;
    for (int32 i = 0; i < self->tileSizeX; ++i) {
        verts[0].x = vertPtr[0].x + screenX;
        verts[0].y = vertPtr[0].y - screenY;
        verts[1].x = vertPtr[2].x + screenX;
        verts[1].y = vertPtr[2].y - screenY;
        verts[2].x = vertPtr[3].x + screenX;
        verts[2].y = vertPtr[3].y - screenY;
        verts[3].x = vertPtr[1].x + screenX;
        verts[3].y = vertPtr[1].y - screenY;

        colour *colours = NULL;
        if (self->timer >= 1)
            colours = ChemicalPool->colours;
        else
            colours = self->colours;
        RSDK.DrawBlendedQuad(verts, colours, 4, 0xC0, INK_NONE);

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

        vertPtr += 2;
    }

    if (self->type > 0) // da reflection
    {
        int32 x = ((self->position.x - self->updateRange.x) >> 16) - screen->position.x;
        int32 y = ((self->position.y - self->updateRange.y) >> 16) - screen->position.y;
        RSDK.SetClipBounds(SceneInfo->currentScreenID, x, y, x + (self->size.x >> 16), y + (self->size.y >> 16));

        drawPos.x = self->position.x - ((self->position.x - (screen->position.x << 16) - (ScreenInfo->centerX << 16)) >> 1);

        int32 offset = 0x800000 - self->size.y;
        offset     = minVal(offset, 0x400000);
        offset     = maxVal(offset, 0);

        drawPos.y =
            offset + self->position.y + self->field_78 - ((self->position.y - (screen->centerY << 16) - (screen->position.y << 16)) >> 4);
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

        Vector2 *vertPtr = self->field_A0;
        for (int32 i = 0; i < 0x2100000; i += 0x100000) {
            int32 x        = i - (self->size.x >> 1);
            vertPtr[0].x = x;
            vertPtr[1].x = x;

            int32 y        = self->position.y - (self->size.y >> 1);
            vertPtr[0].y = y;
            vertPtr[1].y = y + 0x80000;

            vertPtr += 2;
        }

        self->dword88       = (self->position.x - (self->size.x >> 1)) >> 20;
        self->dword8C       = (self->position.x + (self->size.x >> 1)) >> 20;
        self->dword90       = 0x80;
        self->dword94       = 0x99;
        self->r             = 0x00;
        self->g             = 0x08;
        self->b             = 0xC0;
        self->timer         = 0;
        self->hitbox.left   = -(self->size.x >> 17);
        self->hitbox.top    = (self->type == 0 ? 6 : 0) - (self->size.y >> 17);
        self->hitbox.right  = -self->hitbox.left;
        self->hitbox.bottom = -self->hitbox.bottom;
        ChemicalPool_SetupColours();
        RSDK.SetSpriteAnimation(ChemicalPool->aniFrames, 0, &self->animator, true, 0);
    }
}

void ChemicalPool_StageLoad(void)
{
    ChemicalPool->aniFrames     = RSDK.LoadSpriteAnimation("CPZ/ChemPool.bin", SCOPE_STAGE);
    ChemicalPool->sfxChemYellow = RSDK.GetSFX("CPZ/ChemYellow.wav");
    ChemicalPool->sfxChemRed    = RSDK.GetSFX("CPZ/ChemRed.wav");
    ChemicalPool->sfxChemChange = RSDK.GetSFX("CPZ/ChemChange.wav");
}

void ChemicalPool_ChangeState(EntityChemicalPool *chemPool, int32 newType, int32 newR, int32 newG, int32 newB)
{
    chemPool->r          = ((newR - chemPool->r) >> 2) + chemPool->r;
    chemPool->b          = chemPool->b + ((newB - chemPool->b) >> 2);
    chemPool->g          = chemPool->g + ((newG - chemPool->g) >> 2);
    chemPool->colours[2] = ((chemPool->g + (chemPool->r << 8)) << 8) + chemPool->b;
    chemPool->colours[3] = ((chemPool->g + (chemPool->r << 8)) << 8) + chemPool->b;

    uint32 val = abs(newR - chemPool->r) + abs(newG - chemPool->g) + abs(newB - chemPool->b) - 1;
    if (val <= 0x1E && chemPool->state != ChemicalPool_State_Change) {
        chemPool->type  = newType;
        chemPool->timer = 4;
        chemPool->state = ChemicalPool_State_Change;
        RSDK.PlaySfx(ChemicalPool->sfxChemChange, 0, 255);
    }
}

void ChemicalPool_ProcessDeformations(void)
{
    RSDK_THIS(ChemicalPool);

    for (int32 i = self->dword88; i < self->dword8C; ++i) {
        int32 val = ChemicalPool->table1[i];
        ChemicalPool->table3[i] += (-val >> 5) - (ChemicalPool->table3[i] >> 3);
        ChemicalPool->table2[i] = ChemicalPool->table3[i] + ChemicalPool->table1[i];
    }

    int32 v = self->dword88 + 1;
    for (int32 i = self->dword88; i < self->dword8C; ++i) {
        int32 id2 = v - 2;
        if (id2 - 2 < self->dword88)
            id2 = self->dword88;
        if (v >= self->dword8C)
            v = self->dword8C;

        ChemicalPool->table1[i] =
            (self->dword90 * ChemicalPool->table2[i] >> 8) + (self->dword94 * (ChemicalPool->table2[id2] + ChemicalPool->table2[v]) >> 9);
        ++v;
    }

    self->field_78 = 0;
    int32 id           = self->dword88;
    Vector2 *vertPtr = self->field_A0;
    for (int32 i = 0; i <= self->tileSizeX; ++i) {
        if (ChemicalPool->table1[id] > self->field_78)
            self->field_78 = ChemicalPool->table1[id];
        vertPtr[0].y = ChemicalPool->table1[id] + self->offsetY;

        int32 val = 2 * ChemicalPool->table1[id] + 0x80000;
        if (val < ChemicalPool->table1[id])
            val = ChemicalPool->table1[id] + 0x10000;
        ++id;
        vertPtr[1].y = val + self->offsetY;
        vertPtr += 2;
    }
}

void ChemicalPool_SetDeform(int32 x, int32 y)
{
    int32 val = (x + 0x80000) >> 20;
    foreach_active(ChemicalPool, chemPool)
    {
        if (val > chemPool->dword88 && val < chemPool->dword8C) {
            int32 val1 = val - 1;
            if (val1 <= chemPool->dword88)
                val1 = chemPool->dword88 + 1;

            int32 val2 = minVal(val + 2, chemPool->dword8C);

            for (int32 i = 0; i < val2 - val1; ++i) {
                ChemicalPool->table3[val1++] += y;
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
        EntityDebris *debris =
            CREATE_ENTITY(Debris, NULL, x + RSDK.Random(-0x80000, 0x80000, &Zone->randKey), y + RSDK.Random(0x40000, 0x140000, &Zone->randKey));
        debris->state        = Debris_State_Fall;
        debris->gravity      = 0x3800;
        debris->velocity.x   = RSDK.Random(0, 0x20000, &Zone->randKey);
        if (debris->position.x < x)
            debris->velocity.x = -debris->velocity.x;
        debris->velocity.y = RSDK.Random(-0x40000, -0x10000, &Zone->randKey);
        debris->drawOrder  = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Reagent->aniFrames, self->type + 1, &debris->animator, true, RSDK.Random(0, 2, &Zone->randKey));
#else
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x + RSDK.Rand(-0x80000, 0x80000), y + RSDK.Rand(0x40000, 0x140000));
        debris->state      = Debris_State_Fall;
        debris->gravity    = 0x3800;
        debris->velocity.x = RSDK.Rand(0, 0x20000);
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
        case 0:
        default:
            self->colours[0] = 0x0060E0;
            self->colours[1] = 0x0060E0;
            self->colours[2] = 0x0008C0;
            self->colours[3] = 0x0008C0;
            self->g          = 8;
            self->b          = 0xC0;
            self->r          = 0;
            self->state      = ChemicalPool_State_HarmfulBlue;
            self->timer      = 3;
            break;
        case 1:
            self->colours[0] = 0x20E020;
            self->colours[1] = 0x20E020;
            self->colours[2] = 0x189000;
            self->colours[3] = 0x189000;
            self->r          = 0x18;
            self->g          = 0x90;
            self->b          = 0;
            self->state      = ChemicalPool_State_Green;
            self->timer      = 3;
            break;
        case 2:
            self->colours[0] = 0x00B8F0;
            self->colours[1] = 0x00B8F0;
            self->colours[2] = 0x0080B0;
            self->colours[3] = 0x0080B0;
            self->g          = 0x80;
            self->b          = 0xB0;
            self->r          = 0;
            self->state      = ChemicalPool_State_Blue;
            self->timer      = 3;
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
        EntityChemBubble *bubble = (EntityChemBubble *)RSDK.CreateEntity(
            ChemBubble->objectID, NULL, self->position.x + RSDK.Rand(-self->updateRange.x, self->updateRange.x),
            self->position.y + self->updateRange.y);
        bubble->parent = (Entity *)self;
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

            int32 anim = player->playerAnimator.animationID;
            if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                player->playerAnimator.animationID = ANI_WALK;
            player->onGround    = false;
            player->jumpAbility = 0;
            player->velocity.y  = -0x140000;
            RSDK.PlaySfx(ChemicalPool->sfxChemRed, 0, 255);
        }
    }
}

void ChemicalPool_State_Blue(void)
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

                int32 anim = player->playerAnimator.animationID;
                if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                    player->playerAnimator.animationID = ANI_WALK;
                if ((-0x10000 - player->velocity.y) > -0x80000)
                    player->velocity.y = -0x10000 - player->velocity.y;
                else
                    player->velocity.y = -0x80000;
                player->onGround    = false;
                player->jumpAbility = 0;
                RSDK.PlaySfx(ChemicalPool->sfxChemYellow, 0, 255);
            }
        }
    }
}

void ChemicalPool_State_Change(void)
{
    RSDK_THIS(ChemicalPool);
    ChemicalPool_ProcessDeformations();
    if (++self->timer2 == 16) {
        foreach_active(ChemBubble, bubble)
        {
            if (abs(bubble->position.x - self->position.x) <= self->updateRange.x) {
                if (abs(bubble->position.y - self->position.y) <= self->updateRange.y) {
                    RSDK.ResetEntityPtr(bubble, TYPE_BLANK, NULL);
                }
            }
        }
        self->timer2 = 0;
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

    RSDK.DrawRect(drawPos.x, drawPos.y, self->size.x, self->size.y, (self->r << 16) | (self->g << 8) | (self->b << 0), 0xC0, INK_ALPHA,
                  true);
}

void ChemicalPool_EditorLoad(void) {}
#endif

void ChemicalPool_Serialize(void)
{
    RSDK_EDITABLE_VAR(ChemicalPool, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ChemicalPool, VAR_VECTOR2, size);
}
