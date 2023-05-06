// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: IceBomba Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    int32 dirStore  = self->direction;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->bombAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    self->direction = dirStore;
    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->wingAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void IceBomba_Create(void *data)
{
    RSDK_THIS(IceBomba);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (!SceneInfo->inEditor) {
        self->dip <<= 7;
        self->drawFX |= FX_FLIP;

        if (data) {
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(IceBomba->aniFrames, 2, &self->bodyAnimator, true, 0);
            self->state = IceBomba_State_Bomb;
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->alpha         = 0x80;
            self->startDist     = self->dist;
            self->updateRange.x = (self->dist + 0x80) << 16;
            self->updateRange.y = (self->dip + 0x8000) << 8;
            self->startPos      = self->position;
            RSDK.SetSpriteAnimation(IceBomba->aniFrames, 0, &self->bodyAnimator, true, 0);
            RSDK.SetSpriteAnimation(IceBomba->aniFrames, 1, &self->wingAnimator, true, 0);
            RSDK.SetSpriteAnimation(IceBomba->aniFrames, 2, &self->bombAnimator, true, 0);

            self->direction = self->dir;
            if (self->dir) {
                self->bodyAnimator.frameID = 4;
                self->bombAnimator.frameID = 4;
            }
            else {
                self->bodyAnimator.frameID = 0;
                self->bombAnimator.frameID = 0;
            }

            self->state = IceBomba_State_Init;
        }
    }
}

void IceBomba_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1")) // PGZ1 doesn't have this badnik anywhere, the anim file doesn't even exist in the files...
        IceBomba->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/IceBomba.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        IceBomba->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/IceBomba.bin", SCOPE_STAGE);

    IceBomba->hitboxBadnik.left   = -12;
    IceBomba->hitboxBadnik.top    = -12;
    IceBomba->hitboxBadnik.right  = 12;
    IceBomba->hitboxBadnik.bottom = 12;

    IceBomba->hitboxBomb.left   = -16;
    IceBomba->hitboxBomb.top    = -16;
    IceBomba->hitboxBomb.right  = 16;
    IceBomba->hitboxBomb.bottom = 16;

    IceBomba->hitboxRange.left   = -12;
    IceBomba->hitboxRange.top    = 64;
    IceBomba->hitboxRange.right  = 12;
    IceBomba->hitboxRange.bottom = 128;

    IceBomba->sfxExplosion = RSDK.GetSfx("Stage/Explosion.wav");
    IceBomba->sfxFreeze    = RSDK.GetSfx("PSZ/Freeze.wav");

    DEBUGMODE_ADD_OBJ(IceBomba);
}

void IceBomba_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(IceBomba->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void IceBomba_DebugSpawn(void)
{
    RSDK_THIS(IceBomba);

    CREATE_ENTITY(IceBomba, NULL, self->position.x, self->position.y);
}

void IceBomba_HandlePlayerCollisions(void)
{
    RSDK_THIS(IceBomba);

    int32 storeDir  = self->direction;
    self->direction = FLIP_NONE;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &IceBomba->hitboxBadnik)) {
            if (Player_CheckBadnikBreak(player, self, false)) {
                if (self->state != IceBomba_State_FlyAway) {
                    EntityIceBomba *bomb = CREATE_ENTITY(IceBomba, INT_TO_VOID(true), self->position.x, self->position.y);
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
        else if (self->state != IceBomba_State_FlyAway) {
            Vector2 storePos = self->position;
            self->position.x += 0x40000 * (self->direction ? -1 : 1);
            self->position.y += 0x1A0000;

            if (Player_CheckCollisionTouch(player, self, &IceBomba->hitboxBomb)) {
                RSDK.PlaySfx(IceBomba->sfxExplosion, false, 0xFF);
                CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
                RSDK.SetSpriteAnimation(-1, 0, &self->bombAnimator, true, 0);
                self->state = IceBomba_State_FlyAway;
                Ice_FreezePlayer(player);
            }

            self->position = storePos;
        }
    }

    self->direction = storeDir;
}

void IceBomba_State_Init(void)
{
    RSDK_THIS(IceBomba);

    self->active     = ACTIVE_NORMAL;
    self->angle      = 0;
    self->direction  = self->dir;
    self->startDir   = self->dir;
    self->velocity.x = 0x8000 * (self->dir ? 1 : -1);

    self->state = IceBomba_State_Flying;
    IceBomba_State_Flying();
}

void IceBomba_CheckOffScreen(void)
{
    RSDK_THIS(IceBomba);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->dip >>= 7;
        self->position = self->startPos;
        IceBomba_Create(NULL);
    }
}

void IceBomba_State_Flying(void)
{
    RSDK_THIS(IceBomba);

    RSDK.ProcessAnimation(&self->wingAnimator);

    self->angle += 4;

    self->position.x += self->velocity.x;
    self->position.y = self->startPos.y + self->dip * RSDK.Sin1024(self->angle);

    if (--self->startDist <= 0) {
        RSDK.SetSpriteAnimation(-1, 1, &self->wingAnimator, true, 0);
        self->state = IceBomba_State_Turning;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &IceBomba->hitboxRange)) {
            EntityIceBomba *bomb = CREATE_ENTITY(IceBomba, INT_TO_VOID(true), self->position.x, self->position.y);
            bomb->position.x += 0x40000 * (self->direction ? -1 : 1);
            bomb->position.y += 0x1A0000;
            bomb->direction = self->direction;
            RSDK.SetSpriteAnimation(-1, 0, &self->bombAnimator, true, 0);
            self->state = IceBomba_State_FlyAway;
            foreach_break;
        }
    }

    IceBomba_HandlePlayerCollisions();
    IceBomba_CheckOffScreen();
}

void IceBomba_State_Turning(void)
{
    RSDK_THIS(IceBomba);

    self->angle += 4;

    self->position.x += self->velocity.x;
    self->position.y = self->dip * RSDK.Sin1024(self->angle) + self->startPos.y;
    self->velocity.x += 0x1000 * (self->direction ? -1 : 1);

    int32 animTimer = ++self->bodyAnimator.timer;
    if (self->direction) {
        if (animTimer >= 3) {
            --self->bombAnimator.frameID;
            --self->bodyAnimator.frameID;
            self->bodyAnimator.timer = 0;

            if (self->bodyAnimator.frameID <= 0) {
                self->direction ^= FLIP_X;
                self->startDist  = 2 * self->dist;
                self->velocity.x = 0x8000 * (self->direction ? 1 : -1);
                RSDK.SetSpriteAnimation(IceBomba->aniFrames, 1, &self->wingAnimator, true, 0);
                self->state = IceBomba_State_Flying;
            }
        }
    }
    else if (animTimer >= 3) {
        ++self->bombAnimator.frameID;
        ++self->bodyAnimator.frameID;
        self->bodyAnimator.timer = 0;
        if (self->bodyAnimator.frameID >= 4) {
            self->direction ^= FLIP_X;
            self->startDist  = 2 * self->dist;
            self->velocity.x = 0x8000;
            RSDK.SetSpriteAnimation(IceBomba->aniFrames, 1, &self->wingAnimator, true, 0);
            self->state = IceBomba_State_Flying;
        }
    }

    IceBomba_HandlePlayerCollisions();
    IceBomba_CheckOffScreen();
}

void IceBomba_State_FlyAway(void)
{
    RSDK_THIS(IceBomba);

    RSDK.ProcessAnimation(&self->wingAnimator);

    if (self->direction) {
        if (self->bodyAnimator.frameID < 4) {
            ++self->bodyAnimator.frameID;
            ++self->bombAnimator.frameID;
        }
    }
    else {
        if (self->bodyAnimator.frameID > 0) {
            --self->bodyAnimator.frameID;
            --self->bombAnimator.frameID;
        }
    }

    self->position.x += self->velocity.x;
    self->position.y -= 0x20000;

    IceBomba_HandlePlayerCollisions();
    IceBomba_CheckOffScreen();
}

void IceBomba_State_Bomb(void)
{
    RSDK_THIS(IceBomba);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.CheckOnScreen(self, NULL)) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            RSDK.PlaySfx(IceBomba->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];

            CREATE_ENTITY(Ice, INT_TO_VOID(ICE_CHILD_PILLAR), self->position.x, self->position.y + 0x100000);
            RSDK.PlaySfx(IceBomba->sfxFreeze, false, 0xFF);
            destroyEntity(self);
        }

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &IceBomba->hitboxBomb)) {
                RSDK.PlaySfx(IceBomba->sfxExplosion, false, 0xFF);
                CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];

                Ice_FreezePlayer(player);
                destroyEntity(self);
                foreach_break;
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void IceBomba_EditorDraw(void)
{
    RSDK_THIS(IceBomba);

    self->drawFX |= FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->alpha         = 0x80;
    self->startDist     = self->dist;
    self->updateRange.x = (self->dist + 0x80) << 16;
    self->updateRange.y = ((self->dip << 7) + 0x8000) << 8;

    RSDK.SetSpriteAnimation(IceBomba->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(IceBomba->aniFrames, 1, &self->wingAnimator, true, 0);
    RSDK.SetSpriteAnimation(IceBomba->aniFrames, 2, &self->bombAnimator, true, 0);

    self->direction = self->dir;
    if (self->dir) {
        self->bodyAnimator.frameID = 4;
        self->bombAnimator.frameID = 4;
    }
    else {
        self->bodyAnimator.frameID = 0;
        self->bombAnimator.frameID = 0;
    }

    IceBomba_Draw();
}

void IceBomba_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        IceBomba->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/IceBomba.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        IceBomba->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/IceBomba.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(IceBomba, dir);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void IceBomba_Serialize(void)
{
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT8, dir);
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT16, dist);
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT16, dip);
}