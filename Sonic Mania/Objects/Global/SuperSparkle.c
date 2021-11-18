#include "SonicMania.h"

ObjectSuperSparkle *SuperSparkle;

void SuperSparkle_Update(void)
{
    RSDK_THIS(SuperSparkle);
    EntityPlayer *player = (EntityPlayer *)self->player;
    if (!player) {
        destroyEntity(self);
    }
    else {
        if (player->groundedStore) {
            self->activeFlag = abs(player->velocity.y) + abs(player->velocity.x) > 0x60000;
        }

        if (self->activeFlag) {
            if (++self->timer == 12) {
                self->timer        = 0;
                EntityDebris *debris = CREATE_ENTITY(Debris, NULL, player->position.x, player->position.y);
                debris->state        = Debris_State_Move;
                debris->timer        = 16;
                debris->inkEffect    = INK_ADD;
                debris->alpha        = 256;
                debris->drawOrder    = Zone->drawOrderHigh;
                debris->drawOrder    = player->drawOrder;
                RSDK.SetSpriteAnimation(SuperSparkle->aniFrames, 0, &debris->animator, true, 0);
            }
        }
        else {
            self->timer = 0;
        }

        if (player->characterID == ID_SONIC && !(Zone->timer & 7)) {
            EntityRing *ring =
                CREATE_ENTITY(Ring, NULL, player->position.x + RSDK.Rand(-0xC0000, 0xC0000), player->position.y + RSDK.Rand(-0x120000, 0x120000));
            ring->state      = Ring_State_Sparkle;
            ring->stateDraw  = Ring_Draw_Sparkle;
            ring->active     = ACTIVE_NORMAL;
            ring->visible    = 0;
            ring->velocity.y = -0x10000;
            ring->drawOrder  = player->drawOrder;
            RSDK.SetSpriteAnimation(Ring->aniFrames, Zone->timer % 3 + 2, &ring->animator, true, 0);
            int32 cnt = ring->animator.frameCount;
            if (ring->animator.animationID == 2) {
                ring->alpha = 224;
                cnt >>= 1;
            }
            ring->maxFrameCount           = cnt - 1;
            ring->animator.animationSpeed = RSDK.Rand(6, 8);
        }
        if (player->superState != SUPERSTATE_SUPER || !player->active)
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
void SuperSparkle_EditorDraw(void) {}

void SuperSparkle_EditorLoad(void) {}
#endif

void SuperSparkle_Serialize(void) {}
