// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpikeCorridor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpikeCorridor *SpikeCorridor;

void SpikeCorridor_Update(void)
{
    RSDK_THIS(SpikeCorridor);

    StateMachine_Run(self->state);
}

void SpikeCorridor_LateUpdate(void) {}

void SpikeCorridor_StaticUpdate(void) {}

void SpikeCorridor_Draw(void)
{
    RSDK_THIS(SpikeCorridor);

    StateMachine_Run(self->stateDraw);
}

void SpikeCorridor_Create(void *data)
{
    RSDK_THIS(SpikeCorridor);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->active    = ACTIVE_BOUNDS;

    if (SceneInfo->inEditor) {
        if (self->colWidth < 20)
            self->colWidth = 20;

        if (self->rowHeight < 1)
            self->rowHeight = 1;

        if (self->yOffset < 64)
            self->yOffset = 64;
    }

    self->updateRange.x = 0x2000000;
    self->updateRange.y = 0x2000000;
    self->state         = SpikeCorridor_StateDropper_Setup;
}

void SpikeCorridor_StageLoad(void)
{
    SpikeCorridor->aniFrames = RSDK.LoadSpriteAnimation("MMZ/SpikeCorridor.bin", SCOPE_STAGE);

    SpikeCorridor->sfxDrop      = RSDK.GetSfx("Stage/Drop.wav");
    SpikeCorridor->sfxImpact    = RSDK.GetSfx("Stage/Impact2.wav");
    SpikeCorridor->sfxIndicator = RSDK.GetSfx("MMZ/Indicator.wav");
}

void SpikeCorridor_SetupHitboxes(void)
{
    RSDK_THIS(SpikeCorridor);

    int32 size = (-8 * self->colWidth) >> 1;

    self->hitboxes[0].left   = size;
    self->hitboxes[0].top    = 0;
    self->hitboxes[0].right  = size + 8 * self->size;
    self->hitboxes[0].bottom = 24;

    self->hitboxes[1].left   = size + 8 * (self->size + 6);
    self->hitboxes[1].top    = 0;
    self->hitboxes[1].bottom = 24;
    self->hitboxes[1].right  = size + 8 * self->colWidth;
}

void SpikeCorridor_HandleDrawing(Animator *animator, int32 offsetY, int8 size, bool32 useWarnAnim)
{
    RSDK_THIS(SpikeCorridor);

    int32 startX = self->position.x - (self->colWidth << 18);

    Vector2 drawPos;
    drawPos.x = 0;
    drawPos.y = self->position.y + offsetY;

    int32 anim = useWarnAnim ? 5 : 0;

    for (int32 x = 0; x < self->colWidth;) {
        drawPos.x = startX;

        int32 w               = 0;
        bool32 useOpeningAnim = false;
        if (x >= size) {
            if (x < size + 6)
                useOpeningAnim = true;
            else
                w = self->colWidth;
        }
        else {
            w = size;
        }

        if (useOpeningAnim) {
            RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim, animator, true, 0);
            x += 2;
            startX += 0x100000;
        }
        else {
            switch (w - x) {
                case 0:
                    RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim, animator, true, 0);
                    x += 2;
                    startX += 0x100000;
                    break;

                case 1: RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim + 1, animator, true, 0); break;

                case 2:
                    RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim + 2, animator, true, 0);
                    ++x;
                    startX += 0x80000;
                    break;

                case 3:
                    RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim + 3, animator, true, 0);
                    x += 2;
                    startX += 0x100000;
                    break;

                default:
                case 4:
                    RSDK.SetSpriteAnimation(SpikeCorridor->aniFrames, anim + 4, animator, true, 0);
                    x += 3;
                    startX += 0x180000;
                    break;
            }
        }

        ++x;
        RSDK.DrawSprite(animator, &drawPos, false);
        startX += 0x80000;
    }
}

void SpikeCorridor_SetupNextSpikeRow(void)
{
    RSDK_THIS(SpikeCorridor);

    int32 max  = (int8)(self->colWidth - 9);
    int32 size = RSDK.Rand(3, max);

    if (self->lastSize <= -1) {
        self->size     = size;
        self->lastSize = size;
    }
    else {
        while (true) {
            if (abs(size - self->lastSize) >= 6 || abs(size - self->lastSize) == 3)
                break;

            size = RSDK.Rand(3, max);
        }

        self->size     = size;
        self->lastSize = size;
    }
}

void SpikeCorridor_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpikeCorridor);

    foreach_active(Player, player)
    {
        for (int32 i = 0; i < 2; ++i) {
            int32 side = Player_CheckCollisionBox(player, self, &self->hitboxes[i]);

            if (side == C_BOTTOM) {
                Player_Hurt(player, self);
                player->collisionFlagV |= 2;
            }
            else if (side == C_TOP) {
                player->collisionFlagV |= 1;
            }
        }
    }
}

void SpikeCorridor_StateDropper_Setup(void)
{
    RSDK_THIS(SpikeCorridor);

    self->hitbox.left   = -((8 * self->colWidth) >> 1);
    self->hitbox.top    = self->yOffset;
    self->hitbox.right  = (8 * self->colWidth) >> 1;
    self->hitbox.bottom = self->yOffset + 24 * self->rowHeight;

    self->startPos = self->position;
    self->active   = ACTIVE_BOUNDS;
    self->visible  = true;
    self->rowID    = 0;
    self->lastSize = -1;
    self->timer    = 0;

    SpikeCorridor_StateDropper_CheckForPlayer();
    self->state = SpikeCorridor_StateDropper_CheckForPlayer;

    self->stateDraw = StateMachine_None;
}

void SpikeCorridor_StateDropper_CheckForPlayer(void)
{
    RSDK_THIS(SpikeCorridor);

    foreach_active(Player, player)
    {
        if (player->sidekick)
            continue;

        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            if (player->position.x <= self->position.x) {
                if (player->position.x + (playerHitbox->left << 16) >= self->position.x + (self->hitbox.left << 16)) {
                    SpikeCorridor_SetupNextSpikeRow();

                    self->state     = SpikeCorridor_StateDropper_DropWarn;
                    self->stateDraw = SpikeCorridor_Draw_DropWarn;
                }
            }
            else if (player->position.x + (playerHitbox->right << 16) <= self->position.x + (self->hitbox.right << 16)) {
                SpikeCorridor_SetupNextSpikeRow();

                self->state     = SpikeCorridor_StateDropper_DropWarn;
                self->stateDraw = SpikeCorridor_Draw_DropWarn;
            }
        }
    }
}

void SpikeCorridor_StateDropper_DropWarn(void)
{
    RSDK_THIS(SpikeCorridor);

    if (self->timer >= 120) {
        self->state     = SpikeCorridor_StateDropper_SpawnSpikes;
        self->stateDraw = 0;
    }
    else {
        if (!(self->timer % 40))
            RSDK.PlaySfx(SpikeCorridor->sfxIndicator, false, 255);

        ++self->timer;
    }
}

void SpikeCorridor_StateDropper_SpawnSpikes(void)
{
    RSDK_THIS(SpikeCorridor);

    RSDK.PlaySfx(SpikeCorridor->sfxDrop, false, 255);

    EntitySpikeCorridor *fallRow = CREATE_ENTITY(SpikeCorridor, self, self->startPos.x, self->startPos.y);
    fallRow->isPermanent         = true;
    fallRow->state               = SpikeCorridor_StateSpikes_Setup;
    fallRow->active              = ACTIVE_NORMAL;
    fallRow->storedRowID         = self->rowID;
    fallRow->size                = self->size;
    fallRow->parent              = self;
    fallRow->drawGroup           = self->drawGroup;
    fallRow->colWidth            = self->colWidth;
    fallRow->yOffset             = 0;
    fallRow->fallOffset          = (self->yOffset + 24 * (self->rowHeight - self->rowID++ - 1)) << 16;

    self->timer = 0;

    if (self->rowID >= self->rowHeight) {
        self->interaction = false;
        self->state       = StateMachine_None;
    }
    else {
        self->state = SpikeCorridor_StateDropper_DropWait;
    }
}

void SpikeCorridor_StateDropper_DropWait(void)
{
    RSDK_THIS(SpikeCorridor);

    if (self->timer >= 60) {
        SpikeCorridor_SetupNextSpikeRow();
        self->timer     = 0;
        self->state     = SpikeCorridor_StateDropper_DropWarn;
        self->stateDraw = SpikeCorridor_Draw_DropWarn;
    }
    else {
        self->timer++;
    }
}

void SpikeCorridor_StateSpikes_Setup(void)
{
    RSDK_THIS(SpikeCorridor);

    SpikeCorridor_SetupHitboxes();

    self->active    = ACTIVE_NORMAL;
    self->visible   = true;
    self->stateDraw = SpikeCorridor_Draw_Spikes;

    self->state = SpikeCorridor_StateSpikes_Fall;
    SpikeCorridor_StateSpikes_Fall();
}

void SpikeCorridor_StateSpikes_Fall(void)
{
    RSDK_THIS(SpikeCorridor);

    if (self->yOffset >= self->fallOffset) {
        RSDK.StopSfx(SpikeCorridor->sfxDrop);
        RSDK.PlaySfx(SpikeCorridor->sfxImpact, false, 255);

        Camera_ShakeScreen(0, 0, 5);
        self->active = ACTIVE_ALWAYS;
        self->state  = SpikeCorridor_StateSpikes_Land;
    }
    else {
        self->velocity.y += 0x3800;
        self->yOffset += self->velocity.y;

        if (self->yOffset > self->fallOffset)
            self->yOffset = self->fallOffset;

        self->position.y = self->yOffset + self->parent->startPos.y;
    }

    SpikeCorridor_CheckPlayerCollisions();
}

void SpikeCorridor_StateSpikes_Land(void) { SpikeCorridor_CheckPlayerCollisions(); }

void SpikeCorridor_Draw_DropWarn(void)
{
    RSDK_THIS(SpikeCorridor);

    Animator animator;
    memset(&animator, 0, sizeof(Animator));

    int32 yOff = 0;
    if ((ScreenInfo->position.y << 16) - self->startPos.y > 0)
        yOff = (ScreenInfo->position.y << 16) - self->startPos.y;

    SpikeCorridor_HandleDrawing(&animator, yOff, self->size, true);
}

void SpikeCorridor_Draw_Spikes(void)
{
    RSDK_THIS(SpikeCorridor);

    Animator animator;
    memset(&animator, 0, sizeof(Animator));

    SpikeCorridor_HandleDrawing(&animator, 0, self->size, false);
}

#if GAME_INCLUDE_EDITOR
void SpikeCorridor_EditorDraw(void)
{
    RSDK_THIS(SpikeCorridor);

    Animator animator;
    memset(&animator, 0, sizeof(Animator));

    self->inkEffect = INK_NONE;
    SpikeCorridor_HandleDrawing(&animator, 0, self->colWidth, true);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 yOffset    = 0;
        self->fallOffset = (self->yOffset + 24 * (self->rowHeight - 1)) << 16;

        while (yOffset < self->fallOffset) {
            self->velocity.y += 0x3800;
            yOffset += self->velocity.y;
            if (yOffset > self->fallOffset)
                yOffset = self->fallOffset;
        }

        self->inkEffect = INK_BLEND;
        SpikeCorridor_HandleDrawing(&animator, yOffset, self->colWidth, false);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SpikeCorridor_EditorLoad(void) { SpikeCorridor->aniFrames = RSDK.LoadSpriteAnimation("MMZ/SpikeCorridor.bin", SCOPE_STAGE); }
#endif

void SpikeCorridor_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpikeCorridor, VAR_INT8, colWidth);
    RSDK_EDITABLE_VAR(SpikeCorridor, VAR_INT8, rowHeight);
    RSDK_EDITABLE_VAR(SpikeCorridor, VAR_INT32, yOffset);
}
