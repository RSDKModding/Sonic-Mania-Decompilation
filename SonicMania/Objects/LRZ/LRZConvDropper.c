// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZConvDropper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLRZConvDropper *LRZConvDropper;

void LRZConvDropper_Update(void)
{
    RSDK_THIS(LRZConvDropper);

    self->visible = DebugMode->debugActive;

    switch (self->triggerMode) {
        case LRZCONVDROP_TRIGGER_BUTTON:
            if (!((Zone->timer + self->intervalOffset) % self->interval))
                LRZConvDropper_HandleButtonDrop(self);
            break;

        case LRZCONVDROP_TRIGGER_PLAYER: {
            bool32 shouldDrop    = false;
            bool32 playerEntered = false;

            self->position.x += self->detectOffset.x;
            self->position.y += self->detectOffset.y;
            foreach_active(Player, player)
            {
                int32 playerID = RSDK.GetEntitySlot(player);

                bool32 triggered = false;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    triggered = true;
                }

                if (!((1 << playerID) & self->activePlayers) && triggered) {
                    if (!player->sidekick)
                        playerEntered = true;
                    self->activePlayers |= 1 << playerID;
                }

                if ((1 << playerID) & self->activePlayers) {
                    if (!player->sidekick)
                        shouldDrop = true;

                    if (!triggered)
                        self->activePlayers &= ~(1 << playerID);
                }
            }
            self->position.x -= self->detectOffset.x;
            self->position.y -= self->detectOffset.y;

            if (playerEntered) {
                if (!self->timerStore || Zone->timer - self->timerStore > self->interval)
                    self->timerStore = Zone->timer;
            }

            if (shouldDrop) {
                int32 timer = Zone->timer - self->timerStore;
                if (!((timer + self->intervalOffset) % self->interval))
                    LRZConvDropper_HandleButtonDrop(self);
            }
            break;
        }
    }
}

void LRZConvDropper_LateUpdate(void) {}

void LRZConvDropper_StaticUpdate(void) {}

void LRZConvDropper_Draw(void)
{
    RSDK_THIS(LRZConvDropper);

    RSDK.DrawSprite(&self->animator, NULL, false);

    if (self->triggerMode == LRZCONVDROP_TRIGGER_PLAYER) {
        int32 x = self->detectOffset.x + self->position.x;
        int32 y = self->detectOffset.y + self->position.y;
        RSDK.DrawLine(self->position.x, self->position.y, x, y, 0xFFFFFF, 0x7F, INK_NONE, false);
        DrawHelpers_DrawHitboxOutline(x, y, &self->hitbox, 0, 0xFFFFFF);
    }
}

void LRZConvDropper_Create(void *data)
{
    RSDK_THIS(LRZConvDropper);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (self->triggerMode == LRZCONVDROP_TRIGGER_PLAYER) {
        self->updateRange.x = (self->detectSize.x >> 1) + 0x1000000;
        self->updateRange.y = (self->detectSize.y >> 1) + 0x1000000;
    }

    self->hitbox.left   = -(self->detectSize.x >> 17);
    self->hitbox.top    = -(self->detectSize.y >> 17);
    self->hitbox.right  = self->detectSize.x >> 17;
    self->hitbox.bottom = self->detectSize.y >> 17;

    LRZConvDropper_SetupDropperChildren();

    RSDK.SetSpriteAnimation(LRZConvDropper->aniFrames, 0, &self->animator, true, 0);
}

void LRZConvDropper_StageLoad(void) { LRZConvDropper->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvDropper.bin", SCOPE_STAGE); }

void LRZConvDropper_SetupDropperChildren(void)
{
    RSDK_THIS(LRZConvDropper);

    int32 slot = RSDK.GetEntitySlot(self) - self->seqCount;
    for (int32 i = 0; i < self->seqCount; ++i) {
        EntityLRZConvItem *child = RSDK_GET_ENTITY(slot++, LRZConvItem);
        child->active            = ACTIVE_NEVER;
        child->visible           = false;
    }
}

void LRZConvDropper_HandleButtonDrop(EntityLRZConvDropper *entity)
{
    if (entity->seqCount && entity->seqPos < entity->seqCount) {
        int32 slot                = RSDK.GetEntitySlot(entity);
        int32 seqPos              = entity->seqPos - entity->seqCount;
        EntityLRZConvItem *seqEnt = RSDK_GET_ENTITY(slot + seqPos, LRZConvItem);

        if (seqEnt->classID == LRZConvItem->classID) {
            EntityLRZConvItem *item = CREATE_ENTITY(LRZConvItem, INT_TO_VOID(seqEnt->type), entity->position.x, entity->position.y);
            item->active            = ACTIVE_NORMAL;
            item->velocity.x        = 0;
            item->velocity.y        = 0;
        }
        else if (seqEnt->classID == Iwamodoki->classID) {
            EntityIwamodoki *iwamadoki = CREATE_ENTITY(Iwamodoki, NULL, entity->position.x, entity->position.y);
            iwamadoki->active          = ACTIVE_NORMAL;
            iwamadoki->state           = Iwamodoki_State_AwaitPlayer;
            iwamadoki->velocity.x      = 0;
            iwamadoki->velocity.y      = 0;
            iwamadoki->lrzConvPhys     = true;
        }
        else if (seqEnt->classID == ItemBox->classID) {
            EntityItemBox *seqBox = (EntityItemBox *)seqEnt;

            EntityItemBox *itembox = CREATE_ENTITY(ItemBox, INT_TO_VOID(seqBox->type), entity->position.x, entity->position.y);
            itembox->active        = ACTIVE_NORMAL;
            itembox->velocity.x    = 0;
            itembox->velocity.y    = 0;
            itembox->lrzConvPhys   = true;
            itembox->direction     = seqEnt->direction;
            itembox->state         = ItemBox_State_Conveyor;
        }

        if (++entity->seqPos >= entity->seqCount) {
            if (entity->seqLoop)
                entity->seqPos %= entity->seqCount;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void LRZConvDropper_EditorDraw(void)
{
    RSDK_THIS(LRZConvDropper);

    RSDK.DrawSprite(&self->animator, NULL, false);

    if (self->triggerMode == LRZCONVDROP_TRIGGER_PLAYER && showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 x = self->detectOffset.x + self->position.x;
        int32 y = self->detectOffset.y + self->position.y;

        RSDK.DrawLine(self->position.x, self->position.y, x, y, 0xFFFFFF, 0x7F, INK_NONE, false);
        DrawHelpers_DrawHitboxOutline(x, y, &self->hitbox, 0, 0xFFFFFF);

        int32 slot = RSDK.GetEntitySlot(self) - self->seqCount;
        for (int32 i = 0; i < self->seqCount; ++i) {
            EntityLRZConvItem *child = RSDK_GET_ENTITY(slot++, LRZConvItem);

            if (child)
                DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xFFFF00, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void LRZConvDropper_EditorLoad(void)
{
    LRZConvDropper->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConvDropper.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LRZConvDropper, triggerMode);
    RSDK_ENUM_VAR("Button", LRZCONVDROP_TRIGGER_BUTTON);
    RSDK_ENUM_VAR("Player", LRZCONVDROP_TRIGGER_PLAYER);
}
#endif

void LRZConvDropper_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZConvDropper, VAR_UINT8, triggerMode);
    RSDK_EDITABLE_VAR(LRZConvDropper, VAR_ENUM, interval);
    RSDK_EDITABLE_VAR(LRZConvDropper, VAR_ENUM, intervalOffset);
    RSDK_EDITABLE_VAR(LRZConvDropper, VAR_VECTOR2, detectOffset);
    RSDK_EDITABLE_VAR(LRZConvDropper, VAR_VECTOR2, detectSize);
    RSDK_EDITABLE_VAR(LRZConvDropper, VAR_UINT8, seqCount);
    RSDK_EDITABLE_VAR(LRZConvDropper, VAR_BOOL, seqLoop);
}
