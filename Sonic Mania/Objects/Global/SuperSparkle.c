// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SuperSparkle Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSuperSparkle *SuperSparkle;

void SuperSparkle_Update(void)
{
    RSDK_THIS(SuperSparkle);

    EntityPlayer *player = self->player;

    if (!player) {
        destroyEntity(self);
    }
    else {
        if (player->groundedStore)
            self->canSpawnSparkle = abs(player->velocity.y) + abs(player->velocity.x) > 0x60000;

        if (self->canSpawnSparkle) {
            if (++self->timer == 12) {
                self->timer          = 0;
                EntityDebris *debris = CREATE_ENTITY(Debris, NULL, player->position.x, player->position.y);
                debris->state        = Debris_State_Move;
                debris->timer        = 16;
                debris->inkEffect    = INK_ADD;
                debris->alpha        = 256;
                debris->drawOrder    = Zone->objectDrawHigh;
                debris->drawOrder    = player->drawOrder;
                RSDK.SetSpriteAnimation(SuperSparkle->aniFrames, 0, &debris->animator, true, 0);
            }
        }
        else {
            self->timer = 0;
        }

        if (player->characterID == ID_SONIC && !(Zone->timer & 7)) {
            int32 x = player->position.x + RSDK.Rand(-0xC0000, 0xC0000);
            int32 y = player->position.y + RSDK.Rand(-0x120000, 0x120000);

            EntityRing *sparkle = CREATE_ENTITY(Ring, NULL, x, y);
            sparkle->state      = Ring_State_Sparkle;
            sparkle->stateDraw  = Ring_Draw_Sparkle;
            sparkle->active     = ACTIVE_NORMAL;
            sparkle->visible    = 0;
            sparkle->velocity.y = -0x10000;
            sparkle->drawOrder  = player->drawOrder;
            RSDK.SetSpriteAnimation(Ring->aniFrames, Zone->timer % 3 + 2, &sparkle->animator, true, 0);
            int32 cnt = sparkle->animator.frameCount;
            if (sparkle->animator.animationID == 2) {
                sparkle->alpha = 0xE0;
                cnt >>= 1;
            }
            sparkle->maxFrameCount  = cnt - 1;
            sparkle->animator.speed = RSDK.Rand(6, 8);
        }

        if (player->superState != SUPERSTATE_SUPER || player->active == ACTIVE_NEVER)
            destroyEntity(self);
    }
}

void SuperSparkle_LateUpdate(void) {}

void SuperSparkle_StaticUpdate(void) {}

void SuperSparkle_Draw(void) {}

void SuperSparkle_Create(void *data)
{
    RSDK_THIS(SuperSparkle);

    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_NORMAL;
        self->player = (EntityPlayer *)data;
    }
}

void SuperSparkle_StageLoad(void) { SuperSparkle->aniFrames = RSDK.LoadSpriteAnimation("Global/SuperSparkle.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void SuperSparkle_EditorDraw(void)
{
    RSDK_THIS(SuperSparkle);
    Animator animator;
    RSDK.SetSpriteAnimation(SuperSparkle->aniFrames, 0, &animator, true, 3);

    RSDK.DrawSprite(&animator, NULL, false);
}

void SuperSparkle_EditorLoad(void) { SuperSparkle->aniFrames = RSDK.LoadSpriteAnimation("Global/SuperSparkle.bin", SCOPE_STAGE); }
#endif

void SuperSparkle_Serialize(void) {}
