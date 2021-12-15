// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: IceBomba Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectIceBomba *IceBomba;

void IceBomba_Update(void)
{
    RSDK_THIS(IceBomba);
    StateMachine_Run(self->state);
}

void IceBomba_LateUpdate(void) {}

void IceBomba_StaticUpdate(void) {}

void IceBomba_Draw(void)
{
    RSDK_THIS(IceBomba);
    Vector2 drawPos = self->position;
    drawPos.x += 0x40000;
    drawPos.y += 0x1A0000;

    int32 dirStore      = self->direction;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->bombAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->direction = dirStore;
    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->wingAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void IceBomba_Create(void *data)
{
    RSDK_THIS(IceBomba);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    if (!SceneInfo->inEditor) {
        self->dip <<= 7;
        self->drawFX |= FX_FLIP;
        if (data) {
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(IceBomba->animID, 2, &self->animator, true, 0);
            self->state = IceBomba_Bomb_Handle;
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->alpha         = 0x80;
            self->spawnDist     = self->dist;
            self->updateRange.x = (self->dist + 0x80) << 16;
            self->updateRange.y = (self->dip + 0x8000) << 8;
            self->spawnPos.x    = self->position.x;
            self->spawnPos.y    = self->position.y;
            RSDK.SetSpriteAnimation(IceBomba->animID, 0, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(IceBomba->animID, 1, &self->wingAnimator, true, 0);
            RSDK.SetSpriteAnimation(IceBomba->animID, 2, &self->bombAnimator, true, 0);
            self->direction = self->dir;
            if (self->dir) {
                self->animator.frameID     = 4;
                self->bombAnimator.frameID = 4;
            }
            else {
                self->animator.frameID     = 0;
                self->bombAnimator.frameID = 0;
            }
            self->state = IceBomba_Fly_Create;
        }
    }
}

void IceBomba_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        IceBomba->animID = RSDK.LoadSpriteAnimation("PSZ1/IceBomba.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        IceBomba->animID = RSDK.LoadSpriteAnimation("PSZ2/IceBomba.bin", SCOPE_STAGE);

    IceBomba->flyHitbox.left    = -12;
    IceBomba->flyHitbox.top     = -12;
    IceBomba->flyHitbox.right   = 12;
    IceBomba->flyHitbox.bottom  = 12;
    IceBomba->bombHitbox.left   = -16;
    IceBomba->bombHitbox.top    = -16;
    IceBomba->bombHitbox.right  = 16;
    IceBomba->bombHitbox.bottom = 16;
    IceBomba->checkbox.left     = -12;
    IceBomba->checkbox.top      = 64;
    IceBomba->checkbox.right    = 12;
    IceBomba->checkbox.bottom   = 128;
    IceBomba->explosionSFX = RSDK.GetSfx("Stage/Explosion.wav");
    IceBomba->freezeSFX    = RSDK.GetSfx("PSZ/Freeze.wav");
    DEBUGMODE_ADD_OBJ(IceBomba);
}

void IceBomba_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(IceBomba->animID, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void IceBomba_DebugSpawn(void)
{
    RSDK_THIS(IceBomba);
    CREATE_ENTITY(IceBomba, NULL, self->position.x, self->position.y);
}

void IceBomba_Fly_Collide(void)
{
    RSDK_THIS(IceBomba);
    int32 oldDir        = self->direction;
    self->direction = 0;
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &IceBomba->flyHitbox)) {
            if (Player_CheckBadnikBreak(self, player, false)) {
                if (self->state != IceBomba_Fly_FlyAway) {
                    EntityIceBomba *bomb = CREATE_ENTITY(IceBomba, intToVoid(1), self->position.x, self->position.y);
                    if (self->direction)
                        bomb->position.x -= 0x40000;
                    else
                        bomb->position.x += 0x40000;
                    bomb->position.y += 0x1A0000;
                    bomb->direction = self->direction;
                }
                destroyEntity(self);
            }
        }
        else if (self->state != IceBomba_Fly_FlyAway) {
            Vector2 prePos = self->position;
            self->position.x += 0x40000 * (self->direction ? -1 : 1);
            self->position.y += 0x1A0000;
            if (Player_CheckCollisionTouch(player, self, &IceBomba->bombHitbox)) {
                RSDK.PlaySfx(IceBomba->explosionSFX, 0, 255);
                CREATE_ENTITY(Explosion, intToVoid(1), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->bombAnimator, true, 0);
                self->state = IceBomba_Fly_FlyAway;
                Ice_FreezePlayer((Entity*)player);
            }
            self->position = prePos;
        }
    }
    self->direction = oldDir;
}

void IceBomba_Fly_Create(void)
{
    RSDK_THIS(IceBomba);
    self->active     = ACTIVE_NORMAL;
    self->angle      = 0;
    self->direction  = self->dir;
    self->spawnDir   = self->dir;
    self->velocity.x = 0x8000 * (self->dir ? 1 : -1);
    self->state      = IceBomba_Fly_Idle;
    IceBomba_Fly_Idle();
}

void IceBomba_CheckOnScreen(void)
{
    RSDK_THIS(IceBomba);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->spawnPos, &self->updateRange)) {
        self->dip >>= 7;
        self->position.x = self->spawnPos.x;
        self->position.y = self->spawnPos.y;
        IceBomba_Create(NULL);
    }
}

void IceBomba_Fly_Idle(void)
{
    RSDK_THIS(IceBomba);
    RSDK.ProcessAnimation(&self->wingAnimator);
    self->angle += 4;
    self->position.x += self->velocity.x;
    self->position.y = self->dip * RSDK.Sin1024(self->angle) + self->spawnPos.y;
    if (--self->spawnDist <= 0) {
        RSDK.SetSpriteAnimation(0xFFFF, 1, &self->wingAnimator, true, 0);
        self->state = IceBomba_Fly_Turn;
    }
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &IceBomba->checkbox)) {
            EntityIceBomba *bomb = CREATE_ENTITY(IceBomba, intToVoid(1), self->position.x, self->position.y);
            bomb->position.x += 0x40000 * (self->direction ? -1 : 1);
            bomb->position.y += 0x1A0000;
            bomb->direction = self->direction;
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->bombAnimator, true, 0);
            self->state = IceBomba_Fly_FlyAway;
            foreach_break;
        }
    }
    IceBomba_Fly_Collide();
    IceBomba_CheckOnScreen();
}

void IceBomba_Fly_Turn(void)
{
    RSDK_THIS(IceBomba);
    self->angle += 4;
    self->position.x += self->velocity.x;
    self->position.y = self->dip * RSDK.Sin1024(self->angle) + self->spawnPos.y;
    self->velocity.x += 0x1000 * (self->direction ? -1 : 1);
    int32 animTimer = ++self->animator.timer;
    if (self->direction) {
        if (animTimer >= 3) {
            --self->bombAnimator.frameID;
            --self->animator.frameID;
            self->animator.timer = 0;
            if (self->animator.frameID <= 0) {
                self->direction ^= 1;
                self->spawnDist = 2 * self->dist;
                self->velocity.x = 0x8000 * (self->direction ? 1 : -1);
                RSDK.SetSpriteAnimation(IceBomba->animID, 1, &self->wingAnimator, true, 0);
                self->state = IceBomba_Fly_Idle;
            }
        }
    }
    else if (animTimer >= 3) {
        ++self->bombAnimator.frameID;
        ++self->animator.frameID;
        self->animator.timer = 0;
        if (self->animator.frameID >= 4) {
            self->direction  ^= 1; //ida says its a direct =. this does not make any sense
            self->spawnDist  = 2 * self->dist;
            self->velocity.x = 0x8000;
            RSDK.SetSpriteAnimation(IceBomba->animID, 1, &self->wingAnimator, true, 0);
            self->state = IceBomba_Fly_Idle;
        }
    }
    IceBomba_Fly_Collide();
    IceBomba_CheckOnScreen();
}

void IceBomba_Fly_FlyAway(void)
{
    RSDK_THIS(IceBomba);
    RSDK.ProcessAnimation(&self->wingAnimator);
    if (self->direction) {
        if (self->animator.frameID < 4) {
            ++self->animator.frameID;
            ++self->bombAnimator.frameID;
        }
    }
    else {
        if (self->animator.frameID > 0) {
            --self->animator.frameID;
            --self->bombAnimator.frameID;
        }
    }
    self->position.x += self->velocity.x;
    self->position.y -= 0x20000;
    IceBomba_Fly_Collide();
    IceBomba_CheckOnScreen();
}

void IceBomba_Bomb_Handle(void)
{
    RSDK_THIS(IceBomba);
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(self, NULL)) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            RSDK.PlaySfx(IceBomba->explosionSFX, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
            CREATE_ENTITY(Ice, intToVoid(ICE_CHILD_PILLAR), self->position.x, self->position.y + 0x100000);
            RSDK.PlaySfx(IceBomba->freezeSFX, 0, 255);
            destroyEntity(self);
        }
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &IceBomba->bombHitbox)) {
                RSDK.PlaySfx(IceBomba->explosionSFX, false, 255);
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
                Ice_FreezePlayer((Entity*)player);
                destroyEntity(self);
                foreach_return;
            }
        }
    }
    else
        destroyEntity(self);
}

void IceBomba_EditorDraw(void) { IceBomba_DebugDraw(); }

void IceBomba_EditorLoad(void) { IceBomba_StageLoad(); }

void IceBomba_Serialize(void)
{
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT8, dir);
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT16, dist);
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT16, dip);
}