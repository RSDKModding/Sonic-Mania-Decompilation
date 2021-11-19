#include "SonicMania.h"

ObjectChemBubble *ChemBubble;

void ChemBubble_Update(void)
{
    RSDK_THIS(ChemBubble);
    StateMachine_Run(self->state);
}

void ChemBubble_LateUpdate(void) {}

void ChemBubble_StaticUpdate(void) {}

void ChemBubble_Draw(void)
{
    RSDK_THIS(ChemBubble);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void ChemBubble_Create(void *data)
{
    RSDK_THIS(ChemBubble);
    if (!SceneInfo->inEditor) {
        self->visible    = true;
        self->active     = ACTIVE_NORMAL;
        self->drawOrder  = Zone->drawOrderHigh - 2;
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
#if RETRO_USE_PLUS
        self->velocity.y = RSDK.RandSeeded(-0x20000, 0, &Zone->randSeed);
        self->shiftY     = RSDK.RandSeeded(12, 16, &Zone->randSeed);
        self->shiftY2    = RSDK.RandSeeded(9, 10, &Zone->randSeed);
#else
        self->velocity.y = RSDK.Rand(-0x20000, 0);
        self->shiftY     = RSDK.Rand(12, 16);
        self->shiftY2    = RSDK.Rand(9, 10);
#endif
        RSDK.SetSpriteAnimation(ChemBubble->aniFrames, RSDK.Rand(1, 3), &self->animator, true, 0);
        self->drawFX = FX_SCALE;
        self->state  = ChemBubble_Unknown1;
    }
}

void ChemBubble_StageLoad(void) { ChemBubble->aniFrames = RSDK.LoadSpriteAnimation("CPZ/ChemPool.bin", SCOPE_STAGE); }

void ChemBubble_Unknown1(void)
{
    RSDK_THIS(ChemBubble);
    self->scale.x += 0x20;
    self->scale.y = self->scale.x;
    if (self->scale.x == 0x200) {
        self->drawFX = FX_NONE;
        self->state  = ChemBubble_Unknown2;
    }
}

void ChemBubble_Unknown2(void)
{
    RSDK_THIS(ChemBubble);
    EntityChemicalPool *parent = (EntityChemicalPool *)self->parent;

    self->velocity.y -= 0x2000;
    self->position.y += self->velocity.y;
    int32 deform         = (RSDK.Sin256(self->position.y >> self->shiftY) << self->shiftY2) + self->startPos.x;
    self->position.x = deform;
    int32 val            = ChemicalPool->table1[deform >> 20];
    int32 pos            = parent->offsetY + (((deform >> 12) & 0xFF) * (ChemicalPool->table1[(deform + 0x100000) >> 20] - val) >> 8) + val + 0x20000;
    if (self->position.y <= pos) {
        self->position.y = pos;
        if (self->animator.animationID == 2) {
            RSDK.SetSpriteAnimation(ChemBubble->aniFrames, 3, &self->animator, true, 0);
            self->timer = RSDK.Rand(8, 32);
        }
        else {
            RSDK.SetSpriteAnimation(ChemBubble->aniFrames, 4, &self->animator, true, 5);
        }
        self->state = ChemBubble_Unknown3;
    }
}

void ChemBubble_Unknown3(void)
{
    RSDK_THIS(ChemBubble);
    EntityChemicalPool *parent = (EntityChemicalPool *)self->parent;

    RSDK.ProcessAnimation(&self->animator);
    int32 val = ChemicalPool->table1[self->position.x >> 20];
    int32 pos = parent->offsetY + (((self->position.x >> 12) & 0xFF) * (ChemicalPool->table1[(self->position.x + 0x100000) >> 20] - val) >> 8) + val
              + 0x20000;

    self->position.y = pos;
    if (self->animator.animationID == 4) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.ResetEntityPtr(self, TYPE_BLANK, NULL);
    }
    else if (--self->timer <= 0) {
        RSDK.SetSpriteAnimation(ChemBubble->aniFrames, 4, &self->animator, true, 5);
    }
}

#if RETRO_INCLUDE_EDITOR
void ChemBubble_EditorDraw(void)
{
    RSDK_THIS(ChemBubble);

    RSDK.SetSpriteAnimation(ChemBubble->aniFrames, 1, &self->animator, true, 0);
    self->drawFX = FX_SCALE;

    ChemBubble_Draw();
}

void ChemBubble_EditorLoad(void) { ChemBubble->aniFrames = RSDK.LoadSpriteAnimation("CPZ/ChemPool.bin", SCOPE_STAGE); }
#endif

void ChemBubble_Serialize(void) {}
