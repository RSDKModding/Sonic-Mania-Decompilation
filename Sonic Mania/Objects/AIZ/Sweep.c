#include "SonicMania.h"

ObjectSweep *Sweep;

void Sweep_Update(void)
{
    RSDK_THIS(Sweep);
    StateMachine_Run(self->state);
    if (self->state != Sweep_State_Projectile) {
        EntityWater *water = (EntityWater *)self->waterPtr;
        if (water)
            self->position.y = water->position.y - (water->size.y >> 1) - 0x90000;
        else
            self->position.y = Water->waterLevel - 0x90000;
    }
}

void Sweep_LateUpdate(void) {}

void Sweep_StaticUpdate(void) {}

void Sweep_Draw(void)
{
    RSDK_THIS(Sweep);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Sweep_Create(void *data)
{
    RSDK_THIS(Sweep);
    if (!SceneInfo->inEditor) {
        self->visible = true;
        self->drawFX |= FX_FLIP;
        self->drawOrder = Zone->drawOrderLow;
        self->startPos  = self->position;
        self->startDir  = self->direction;
        self->timer     = 128;
        self->hasShot   = false;

        if (data) {
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x200000;
            self->updateRange.y = 0x200000;
            RSDK.SetSpriteAnimation(Sweep->aniFrames, 4, &self->animator, true, 0);
            self->state = Sweep_State_Projectile;
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &self->animator, true, 0);

            foreach_all(Water, water)
            {
                Hitbox waterHitbox;
                Hitbox hitbox;

                waterHitbox.right  = water->size.x >> 17;
                waterHitbox.left   = -(water->size.x >> 17);
                waterHitbox.bottom = water->size.y >> 17;
                waterHitbox.top    = -(water->size.y >> 17);

                hitbox.left   = 1;
                hitbox.top    = 64;
                hitbox.right  = 1;
                hitbox.bottom = 64;
                if (water->type == WATER_RECT && RSDK.CheckObjectCollisionTouchBox(water, &waterHitbox, self, &hitbox)) {
                    self->waterPtr = (Entity *)water;
                }
            }
            self->state = Sweep_State_Setup;
        }
    }
}

void Sweep_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        Sweep->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Sweep.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    else if (RSDK.CheckStageFolder("AIZ"))
        Sweep->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Sweep.bin", SCOPE_STAGE);
#endif
    Sweep->hitboxBadnik.left       = -10;
    Sweep->hitboxBadnik.top        = -7;
    Sweep->hitboxBadnik.right      = 10;
    Sweep->hitboxBadnik.bottom     = 4;
    Sweep->hitboxRange.left        = -256;
    Sweep->hitboxRange.top         = -16;
    Sweep->hitboxRange.right       = 0;
    Sweep->hitboxRange.bottom      = 16;
    Sweep->hitboxProjectile.left   = -13;
    Sweep->hitboxProjectile.top    = -3;
    Sweep->hitboxProjectile.right  = -8;
    Sweep->hitboxProjectile.bottom = 3;
    Sweep->sfxPon                  = RSDK.GetSFX("Stage/Pon.wav");
    DEBUGMODE_ADD_OBJ(Sweep);
}

void Sweep_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntitySweep *sweep = CREATE_ENTITY(Sweep, NULL, self->position.x, self->position.y);
    sweep->direction   = self->direction;
    sweep->startDir    = self->direction;
}

void Sweep_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Sweep_CheckOnScreen(void)
{
    RSDK_THIS(Sweep);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Sweep_Create(NULL);
    }
}

void Sweep_HandleInteractions(void)
{
    RSDK_THIS(Sweep);

    foreach_active(Player, player)
    {
        Hitbox hitbox;
        Hitbox *playerHitbox = Player_GetHitbox(player);
        RSDK.GetHitbox(&player->animator, 0);

        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
        if (shield->objectID == Shield->objectID && shield->state == Shield_State_Insta) {
            hitbox.left   = 2 * playerHitbox->left - (playerHitbox->left >> 1);
            hitbox.top    = 2 * playerHitbox->top - (playerHitbox->top >> 1);
            hitbox.right  = 2 * playerHitbox->right - (playerHitbox->right >> 1);
            hitbox.bottom = 2 * playerHitbox->bottom - (playerHitbox->bottom >> 1);
            playerHitbox  = &hitbox;
        }

        int32 side = RSDK.CheckObjectCollisionBox(self, &Sweep->hitboxBadnik, player, playerHitbox, false);
        if (side) {
            if (self->state != Sweep_State_Turn
                && ((self->direction == FLIP_NONE && side == C_LEFT) || (self->direction == FLIP_X && side == C_RIGHT)))
                Player_CheckHit(player, self);
            else
                Player_CheckBadnikBreak(self, player, true);
        }
    }
}

void Sweep_CheckShoot(void)
{
    RSDK_THIS(Sweep);

    if (!self->hasShot && self->timer <= 16) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Sweep->hitboxRange)) {
                RSDK.SetSpriteAnimation(Sweep->aniFrames, 3, &self->animator, true, 0);
                EntitySweep *projectile = CREATE_ENTITY(Sweep, intToVoid(true), self->position.x, self->position.y);
                if (!self->direction)
                    projectile->velocity.x = -0x30000;
                else
                    projectile->velocity.x = 0x30000;
                projectile->direction = self->direction;
                projectile->active    = ACTIVE_NORMAL;
                RSDK.PlaySfx(Sweep->sfxPon, false, 255);
                self->hasShot    = true;
                self->stateStore = self->state;
                self->state      = Sweep_State_FiredShot;
                foreach_break;
            }
        }
    }
}

void Sweep_State_Setup(void)
{
    RSDK_THIS(Sweep);

    self->active  = ACTIVE_NORMAL;
    self->timer   = 32;
    self->hasShot = false;
    self->state   = Sweep_State_Idle;
    Sweep_State_Idle();
}

void Sweep_State_Idle(void)
{
    RSDK_THIS(Sweep);

    self->timer--;
    if (self->timer) {
        Sweep_CheckShoot();
    }
    else {
        self->hasShot = false;
        self->timer   = 64;
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &self->animator, true, 1);
        self->state = Sweep_State_Dash;
    }
    Sweep_HandleInteractions();
    Sweep_CheckOnScreen();
}

void Sweep_State_Dash(void)
{
    RSDK_THIS(Sweep);

    RSDK.ProcessAnimation(&self->animator);
    self->timer--;
    if (self->timer <= 0) {
        self->timer   = 32;
        self->hasShot = false;
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 1, &self->animator, true, 0);
        self->state = Sweep_State_Stop;
    }
    else {
        if (self->direction && self->velocity.x < 0x20000) {
            self->velocity.x += 0x3800;
        }
        else if (!self->direction && self->velocity.x > -0x20000) {
            self->velocity.x -= 0x3800;
        }
        self->position.x += self->velocity.x;
    }

    Sweep_HandleInteractions();
    Sweep_CheckOnScreen();
}

void Sweep_State_Stop(void)
{
    RSDK_THIS(Sweep);

    RSDK.ProcessAnimation(&self->animator);
    self->timer--;
    if (self->timer <= 0) {
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 2, &self->animator, true, 0);
        self->state = Sweep_State_Turn;
    }
    else if (!self->velocity.x) {
        Sweep_CheckShoot();
    }
    else {
        if (self->direction) {
            if (self->velocity.x > 0)
                self->velocity.x -= 0x3800;
            else
                self->velocity.x = 0;
        }
        else {
            if (self->velocity.x > 0)
                self->velocity.x += 0x3800;
            else
                self->velocity.x = 0;
        }
        self->position.x += self->velocity.x;
    }

    Sweep_HandleInteractions();
    Sweep_CheckOnScreen();
}

void Sweep_State_FiredShot(void)
{
    RSDK_THIS(Sweep);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 1, &self->animator, true, 4);
        self->state = self->stateStore;
        StateMachine_Run(self->state);
    }
    else {
        Sweep_HandleInteractions();
        Sweep_CheckOnScreen();
    }
}

void Sweep_State_Turn(void)
{
    RSDK_THIS(Sweep);

    self->position.x += self->velocity.x;
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &self->animator, true, 0);
        self->timer = 32;
        self->direction ^= FLIP_X;
        self->state = Sweep_State_Idle;
        Sweep_State_Idle();
    }
    else {
        Sweep_HandleInteractions();
        Sweep_CheckOnScreen();
    }
}

void Sweep_State_Projectile(void)
{
    RSDK_THIS(Sweep);
    self->position.x += self->velocity.x;
    if (RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Sweep->hitboxProjectile)) {
                Player_CheckProjectileHit(player, self);
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Sweep_EditorDraw(void)
{
    RSDK_THIS(Sweep);
    RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &self->animator, true, 0);
    Sweep_Draw();
}

void Sweep_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        Sweep->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Sweep.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    else if (RSDK.CheckStageFolder("AIZ"))
        Sweep->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Sweep.bin", SCOPE_STAGE);
#endif
}
#endif

void Sweep_Serialize(void) {}
