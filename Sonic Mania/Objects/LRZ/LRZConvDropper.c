#include "SonicMania.h"

ObjectLRZConvDropper *LRZConvDropper;

void LRZConvDropper_Update(void)
{
    RSDK_THIS(LRZConvDropper);
    self->visible = DebugMode->debugActive == 1;

    switch (self->triggerMode) {
        case LRZCONVDROP_TRIGGER_BUTTON:
            if (!((Zone->timer + self->intervalOffset) % self->interval))
                LRZConvDropper_HandleButtonDrop(self);
            break;
        case LRZCONVDROP_TRIGGER_PLAYER: {
            bool32 dropFlag  = false;
            bool32 enterFlag = false;

            self->position.x += self->detectOffset.x;
            self->position.y += self->detectOffset.y;
            foreach_active(Player, player)
            {
                int playerID = RSDK.GetEntityID(player);

                bool32 flag = false;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    flag = true;
                }

                if (!((1 << playerID) & self->activePlayers) && flag) {
                    if (!player->sidekick)
                        enterFlag = true;
                    self->activePlayers |= (1 << playerID);
                }

                if ((1 << playerID) & self->activePlayers) {
                    if (!player->sidekick)
                        dropFlag = true;
                    if (!flag)
                        self->activePlayers &= ~(1 << playerID);
                }
            }
            self->position.x -= self->detectOffset.x;
            self->position.y -= self->detectOffset.y;

            if (enterFlag) {
                if (!self->timerStore || Zone->timer - self->timerStore > self->interval)
                    self->timerStore = Zone->timer;
            }

            int32 timer = 0;
            if (dropFlag)
                timer = Zone->timer - self->timerStore;

            if (dropFlag) {
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
        int x = self->detectOffset.x + self->position.x;
        int y = self->detectOffset.y + self->position.y;
        RSDK.DrawLine(self->position.x, self->position.y, x, y, 0xFFFFFF, 0x7F, INK_NONE, false);
        DrawHelpers_DrawHitboxOutline(0xFFFFFF, 0, x, y, &self->hitbox);
    }
}

void LRZConvDropper_Create(void *data)
{
    RSDK_THIS(LRZConvDropper);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
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

    int slot = RSDK.GetEntityID(self) - self->seqCount;
    for (int i = 0; i < self->seqCount; ++i) {
        EntityLRZConvItem *child = RSDK_GET_ENTITY(slot++, LRZConvItem);
        child->active            = ACTIVE_NEVER;
        child->visible           = false;
    }
}

void LRZConvDropper_HandleButtonDrop(EntityLRZConvDropper *entity)
{
    if (entity->seqCount && entity->seqPos < entity->seqCount) {
        int slot                  = RSDK.GetEntityID(entity);
        int32 seqPos              = entity->seqPos - entity->seqCount;
        EntityLRZConvItem *seqEnt = RSDK_GET_ENTITY(slot + seqPos, LRZConvItem);

        if (seqEnt->objectID == LRZConvItem->objectID) {
            EntityLRZConvItem *item = CREATE_ENTITY(LRZConvItem, intToVoid(seqEnt->type), entity->position.x, entity->position.y);
            item->active            = ACTIVE_NORMAL;
            item->velocity.x        = 0;
            item->velocity.y        = 0;
        }
        else if (seqEnt->objectID == Iwamodoki->objectID) {
            EntityIwamodoki *iwamadoki = CREATE_ENTITY(Iwamodoki, NULL, entity->position.x, entity->position.y);
            iwamadoki->active          = ACTIVE_NORMAL;
            iwamadoki->state           = Iwamodoki_State_Unknown1;
            iwamadoki->velocity.x      = 0;
            iwamadoki->velocity.y      = 0;
            iwamadoki->lrzConvPhys     = true;
        }
        else if (seqEnt->objectID == ItemBox->objectID) {
            EntityItemBox *seqBox = (EntityItemBox *)seqEnt;

            EntityItemBox *itembox = CREATE_ENTITY(ItemBox, intToVoid(seqBox->type), entity->position.x, entity->position.y);
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

#if RETRO_INCLUDE_EDITOR
void LRZConvDropper_EditorDraw(void) { LRZConvDropper_Draw(); }

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
