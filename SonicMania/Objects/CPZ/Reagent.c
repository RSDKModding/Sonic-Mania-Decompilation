// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Reagent Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectReagent *Reagent;

void Reagent_Update(void)
{
    RSDK_THIS(Reagent);

    StateMachine_Run(self->state);
}

void Reagent_LateUpdate(void) {}

void Reagent_StaticUpdate(void) {}

void Reagent_Draw(void)
{
    RSDK_THIS(Reagent);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Reagent_Create(void *data)
{
    RSDK_THIS(Reagent);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x2000000;
        self->inkEffect     = INK_ALPHA;
        self->alpha         = 0xC0;
        self->velocity.x    = ZONE_RAND(-0xC000, 0xC000);
        self->type          = VOID_TO_INT(data);
        self->state         = Reagent_State_CheckPoolCollisions;

        switch (self->type) {
            case CHEMICALPOOL_BLUE:
                self->r = 0x00;
                self->g = 0x08;
                self->b = 0xC0;
                break;

            case CHEMICALPOOL_GREEN:
                self->r = 0x18;
                self->g = 0x90;
                self->b = 0x00;
                break;

            case CHEMICALPOOL_CYAN:
                self->r = 0x00;
                self->g = 0x80;
                self->b = 0xB0;
                break;
        }

        RSDK.SetSpriteAnimation(Reagent->aniFrames, self->type + 1, &self->animator, true, ZONE_RAND(0, 2));
    }
}

void Reagent_StageLoad(void)
{
    Reagent->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE);

    Reagent->hitbox.left   = -2;
    Reagent->hitbox.top    = -2;
    Reagent->hitbox.right  = 2;
    Reagent->hitbox.bottom = 2;

    Reagent->sfxLand = RSDK.GetSfx("Puyo/Land.wav");
}

void Reagent_State_CheckPoolCollisions(void)
{
    RSDK_THIS(Reagent);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_all(ChemicalPool, chemPool)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &Reagent->hitbox, chemPool, &chemPool->hitbox)) {
            self->velocity.y >>= 2;
            self->originPos.x = self->position.x;
            self->state       = Reagent_State_ChangingPoolType;
            chemPool->active  = ACTIVE_NORMAL;
            ChemicalPool_ChangeState(chemPool, self->type, self->r, self->g, self->b);
            RSDK.PlaySfx(Reagent->sfxLand, false, 255);
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Reagent_State_ChangingPoolType(void)
{
    RSDK_THIS(Reagent);

    self->velocity.y -= 0x1800;
    self->alpha -= 4;
    ++self->timer;
    self->position.x = self->originPos.x + (RSDK.Sin256(self->timer) << 10);
    self->position.y += self->velocity.y;

    if (self->timer == 64)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Reagent_EditorDraw(void)
{
    RSDK_THIS(Reagent);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x2000000;
    self->inkEffect     = INK_ALPHA;
    self->alpha         = 0xC0;

    switch (self->type) {
        case CHEMICALPOOL_BLUE:
            self->r = 0x00;
            self->g = 0x08;
            self->b = 192;
            break;

        case CHEMICALPOOL_GREEN:
            self->r = 0x18;
            self->g = 0x90;
            self->b = 0x00;
            break;

        case CHEMICALPOOL_CYAN:
            self->r = 0x00;
            self->g = 0x80;
            self->b = 0xB0;
            break;
    }

    RSDK.SetSpriteAnimation(Reagent->aniFrames, self->type + 1, &self->animator, true, 0);

    Reagent_Draw();
}

void Reagent_EditorLoad(void) { Reagent->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE); }
#endif

void Reagent_Serialize(void) {}
