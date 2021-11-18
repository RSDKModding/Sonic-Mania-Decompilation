#include "SonicMania.h"

ObjectBuzzBomber *BuzzBomber;

void BuzzBomber_Update(void)
{
    RSDK_THIS(BuzzBomber);
    StateMachine_Run(self->state);
}

void BuzzBomber_LateUpdate(void) {}

void BuzzBomber_StaticUpdate(void) {}

void BuzzBomber_Draw(void)
{
    RSDK_THIS(BuzzBomber);

    if (self->inkEffect == INK_ADD) {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
        self->inkEffect = INK_ALPHA;
        RSDK.DrawSprite(&self->animator2, NULL, false);
        self->inkEffect = INK_NONE;
        RSDK.DrawSprite(&self->animator3, NULL, false);
    }
}

void BuzzBomber_Create(void *data)
{
    RSDK_THIS(BuzzBomber);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawOrder  = Zone->drawOrderLow;
    self->startPos.x = self->position.x;
    self->startPos.y = self->position.y;
    self->startDir   = self->direction;
    self->timer      = 128;
    self->field_64   = 0;
    self->projectile = 0;
    if (self->shotRange == 0)
        self->shotRange = 96;

    self->rangeHitbox.right  = self->shotRange;
    self->rangeHitbox.left   = -self->shotRange;
    self->rangeHitbox.top    = -256;
    self->rangeHitbox.bottom = 256;

    if (data) {
        self->inkEffect     = INK_ADD;
        self->alpha         = 0xC0;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x200000;
        self->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 4, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator3, true, 0);
        self->state = BuzzBomber_Unknown6;
    }
    else {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 2, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &self->animator3, true, 0);
        self->state = BuzzBomber_Unknown2;
        self->alpha = 0xC0;
    }
}

void BuzzBomber_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("GHZ/BuzzBomber.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/BuzzBomber.bin", SCOPE_STAGE);

    BuzzBomber->hitbox1.left   = -24;
    BuzzBomber->hitbox1.top    = -12;
    BuzzBomber->hitbox1.right  = 24;
    BuzzBomber->hitbox1.bottom = 12;
    BuzzBomber->hitbox2.left   = -6;
    BuzzBomber->hitbox2.top    = -6;
    BuzzBomber->hitbox2.right  = 6;
    BuzzBomber->hitbox2.bottom = 6;

    DEBUGMODE_ADD_OBJ(BuzzBomber);
}

void BuzzBomber_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void BuzzBomber_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityBuzzBomber *buzzBomber = CREATE_ENTITY(BuzzBomber, NULL, self->position.x, self->position.y);

    buzzBomber->direction = self->direction;
    buzzBomber->startDir  = self->direction;
}

void BuzzBomber_CheckOnScreen(void)
{
    RSDK_THIS(BuzzBomber);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        BuzzBomber_Create(NULL);
    }
}

void BuzzBomber_Unknown1(void)
{
    RSDK_THIS(BuzzBomber);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &BuzzBomber->hitbox1)) {
            if (Player_CheckBadnikBreak(self, player, true)) {
                if (self->projectile)
                    RSDK.ResetEntityPtr(self->projectile, TYPE_BLANK, 0);
            }
        }
        else if (self->state == BuzzBomber_Unknown3 && !self->field_64) {
            if (Player_CheckCollisionTouch(player, self, &self->rangeHitbox)) {
                self->field_64 = 1;
                self->timer    = 90;
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator3, true, 0);
                self->state = BuzzBomber_Unknown5;
            }
        }
    }
}

void BuzzBomber_Unknown2(void)
{
    RSDK_THIS(BuzzBomber);

    self->active = ACTIVE_NORMAL;
    if (!(self->direction & FLIP_X))
        self->velocity.x = -0x40000;
    else
        self->velocity.x = 0x40000;
    self->state = BuzzBomber_Unknown3;
    BuzzBomber_Unknown3();
}

void BuzzBomber_Unknown3(void)
{
    RSDK_THIS(BuzzBomber);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (!self->timer) {
        self->direction ^= FLIP_X;
        self->timer      = 60;
        self->velocity.x = -self->velocity.x;
        self->field_64   = 0;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator3, true, 0);
        self->state = BuzzBomber_Unknown4;
    }

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    BuzzBomber_Unknown1();
    BuzzBomber_CheckOnScreen();
}

void BuzzBomber_Unknown4(void)
{
    RSDK_THIS(BuzzBomber);

    if (!--self->timer) {
        self->timer = 128;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &self->animator3, true, 0);
        self->state = BuzzBomber_Unknown3;
    }
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    BuzzBomber_Unknown1();
    BuzzBomber_CheckOnScreen();
}

void BuzzBomber_Unknown5(void)
{
    RSDK_THIS(BuzzBomber);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    BuzzBomber_Unknown1();
    BuzzBomber_CheckOnScreen();

    self->timer--;
    if (self->timer == 82) {
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 1, &self->animator1, true, 0);
    }
    else if (self->timer == 45) {
        EntityBuzzBomber *projectile = CREATE_ENTITY(BuzzBomber, intToVoid(true), self->position.x, self->position.y);
        if (self->direction) {
            projectile->position.x += 0x180000;
            projectile->velocity.x = 0x20000;
        }
        else {
            projectile->position.x -= 0x180000;
            projectile->velocity.x = -0x20000;
        }
        projectile->position.y += 0x1C0000;
        projectile->velocity.y = 0x20000;
        projectile->groundVel  = 0;
        projectile->projectile = (Entity *)self;
        projectile->direction  = self->direction;
        projectile->active     = ACTIVE_NORMAL;
        self->projectile     = (Entity *)projectile;
    }
    else if (!self->timer) {
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &self->animator1, true, 0);
        self->timer = 128;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &self->animator3, true, 0);
        self->state = BuzzBomber_Unknown3;
    }
}

void BuzzBomber_Unknown6(void)
{
    RSDK_THIS(BuzzBomber);
    RSDK.ProcessAnimation(&self->animator1);

    if (self->animator1.frameID == 6) {
        self->state          = BuzzBomber_Unknown7;
        EntityBuzzBomber *shot = (EntityBuzzBomber *)self->projectile;
        shot->projectile       = NULL;
    }
}

void BuzzBomber_Unknown7(void)
{
    RSDK_THIS(BuzzBomber);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ProcessAnimation(&self->animator1);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &BuzzBomber->hitbox2))
                Player_CheckProjectileHit(player, self);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void BuzzBomber_EditorDraw(void) { BuzzBomber_Draw(); }

void BuzzBomber_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("GHZ/BuzzBomber.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/BuzzBomber.bin", SCOPE_STAGE);
}
#endif

void BuzzBomber_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuzzBomber, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(BuzzBomber, VAR_UINT8, shotRange);
}
