// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FBZTrash Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFBZTrash *FBZTrash;

void FBZTrash_Update(void)
{
    RSDK_THIS(FBZTrash);

    StateMachine_Run(self->state);
}

void FBZTrash_LateUpdate(void) {}

void FBZTrash_StaticUpdate(void) {}

void FBZTrash_Draw(void)
{
    RSDK_THIS(FBZTrash);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void FBZTrash_Create(void *data)
{
    RSDK_THIS(FBZTrash);

    if (!SceneInfo->inEditor) {
        self->active  = ACTIVE_BOUNDS;
        self->visible = true;
        self->drawFX  = FX_FLIP;

        if (data) {
            self->type      = VOID_TO_INT(data);
            self->drawGroup = Zone->objectDrawGroup[0];

            self->hitbox.left   = -8;
            self->hitbox.top    = -8;
            self->hitbox.right  = 8;
            self->hitbox.bottom = 8;

            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->state         = FBZTrash_State_ReactMagnet;
        }
        else {
            self->drawGroup     = Zone->objectDrawGroup[1];
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x200000;
            self->onGround      = true;

            self->direction = RSDK.Rand(0, 2);
            RSDK.SetSpriteAnimation(FBZTrash->aniFrames, 0, &self->animator, true, self->frameID);
            self->state = FBZTrash_State_LooseTrash;
        }
    }
}

void FBZTrash_StageLoad(void) { FBZTrash->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Trash.bin", SCOPE_STAGE); }

void FBZTrash_SummonOrbinautOrbs(EntityFBZTrash *trashPtr, int32 angle)
{
    EntityFBZTrash *trash = CREATE_ENTITY(FBZTrash, INT_TO_VOID(FBZTRASH_ORB), trashPtr->position.x, trashPtr->position.y);

    trash->position.x += RSDK.Cos1024(angle) << 10;
    trash->position.y += RSDK.Sin1024(angle) << 10;
    trash->targetPos  = trash->position;
    int32 size        = BigSqueeze->crusherX[BIGSQUEEZE_CRUSHER_R] - BigSqueeze->crusherX[BIGSQUEEZE_CRUSHER_L];
    trash->position.x = (RSDK.Rand(0, size >> 16) << 16) + BigSqueeze->crusherX[BIGSQUEEZE_CRUSHER_L];
    trash->position.y = BigSqueeze->boundsB - 0x80000;
    trash->parent     = trashPtr;
    trash->startPos   = trash->position;
    trash->angle      = angle;
    trash->radius     = 10;
    trash->state      = FBZTrash_State_ReactMagnet;

    RSDK.SetSpriteAnimation(FBZTrash->aniFrames, 5, &trash->animator, true, RSDK.Rand(0, 2));
}

void FBZTrash_SummonOrbinaut(int32 x, int32 y)
{
    EntityFBZTrash *trash = CREATE_ENTITY(FBZTrash, INT_TO_VOID(FBZTRASH_ORBINAUT), x, y);
    FBZTrash_SummonOrbinautOrbs(trash, 0);
    FBZTrash_SummonOrbinautOrbs(trash, 512);

    trash->targetPos  = trash->position;
    int32 size        = BigSqueeze->crusherX[BIGSQUEEZE_CRUSHER_R] - BigSqueeze->crusherX[BIGSQUEEZE_CRUSHER_L];
    trash->position.x = (RSDK.Rand(0, size >> 16) << 16) + BigSqueeze->crusherX[BIGSQUEEZE_CRUSHER_L];
    trash->position.y = BigSqueeze->boundsB - 0x80000;
    trash->state      = FBZTrash_State_ReactMagnet;
    trash->startPos   = trash->position;

    RSDK.SetSpriteAnimation(FBZTrash->aniFrames, 1, &trash->animator, true, (x >> 17) & 1);
}

void FBZTrash_State_LooseTrash(void)
{
    RSDK_THIS(FBZTrash);

    if (!self->onGround) {
        self->velocity.y += 0x3800;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
    }

    if (self->velocity.y >= -0x8000) {
        foreach_active(FBZTrash, trash)
        {
            if (trash != self) {
                int32 rx = (self->position.x - trash->position.x) >> 16;
                int32 ry = (self->position.y - trash->position.y) >> 16;

                if (rx * rx + ry * ry < 0x120) {
                    if (BigSqueeze->isCrushing)
                        self->rumbleMove = (self->rumbleMove + trash->rumbleMove) >> 1;
                    else
                        self->rumbleMove >>= 1;

                    if (self->rumbleMove <= 0) {
                        self->onGround = true;
                    }
                    else {
                        int32 angle      = RSDK.ATan2(rx, ry);
                        self->velocity.x = MIN(self->rumbleMove << 6, 640) * RSDK.Cos256(angle);
                        self->velocity.y = MIN(self->rumbleMove << 6, 640) * RSDK.Sin256(angle + RSDK.Rand(-32, 32));
                        self->onGround   = false;
                    }
                }
            }
        }
    }

    if (self->position.y < BigSqueeze->boundsB - 0xC0000)
        self->position.y = BigSqueeze->boundsB - 0xC0000;

    foreach_active(BigSqueeze, boss)
    {
        switch (boss->type) {
            default: break;

            case BIGSQUEEZE_BOSS:
                if (self->position.y > boss->position.y + 0xC00000) {
                    self->position.y = boss->position.y + 0xC00000;
                    self->velocity.y = -abs(self->velocity.y >> 1);
                }
                break;

            case BIGSQUEEZE_CRUSHER_L:
                if (self->position.x < boss->position.x + 0x180000) {
                    self->position.x = boss->position.x + 0x180000;
                    self->velocity.x = 5 * boss->velocity.x;
                    if (self->onGround && BigSqueeze->isCrushing) {
                        self->rumbleMove = 128;
                        self->onGround   = false;
                    }
                }
                break;

            case BIGSQUEEZE_CRUSHER_R:
                if (self->position.x > boss->position.x - 0x180000) {
                    self->position.x = boss->position.x - 0x180000;
                    self->velocity.x = 5 * boss->velocity.x;
                    if (self->onGround && BigSqueeze->isCrushing) {
                        self->rumbleMove = 128;
                        self->onGround   = false;
                    }
                }
                break;
        }
    }
}

void FBZTrash_State_ReactMagnet(void)
{
    RSDK_THIS(FBZTrash);

    self->startPos.y -= 0x8000;
    self->position.x = self->startPos.x + RSDK.Rand(-0x20000, 0x20000);
    self->position.y = self->startPos.y + RSDK.Rand(-0x20000, 0x20000);

    if (++self->timer == 16) {
        self->timer      = 0;
        self->velocity.x = (self->targetPos.x - self->position.x) >> 5;
        self->velocity.y = -0x80000;
        self->state      = FBZTrash_State_MoveToTarget;
    }
}

void FBZTrash_State_MoveToTarget(void)
{
    RSDK_THIS(FBZTrash);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    ++self->timer;
    if (self->velocity.y >= 0) {
        if (self->position.y > self->targetPos.y)
            self->position.y = self->targetPos.y;
    }

    if (self->velocity.x <= 0) {
        if (self->position.x < self->targetPos.x) {
            self->velocity.x = 0;
            self->position.x = self->targetPos.x;
        }
    }
    else if (self->position.x > self->targetPos.x) {
        self->velocity.x = 0;
        self->position.x = self->targetPos.x;
    }

    if (self->timer == 60) {
        self->timer = 0;
        switch (self->type) {
            default: break;
            case FBZTRASH_ORB: self->state = FBZTrash_State_OrbinautOrb; break;
            case FBZTRASH_ORBINAUT: self->state = FBZTrash_State_OrbinautMove; break;
        }
    }
}

void FBZTrash_State_OrbinautOrb(void)
{
    RSDK_THIS(FBZTrash);
    EntityFBZTrash *trash = self->parent;

    int32 angle = 0;
    if (trash->type == FBZTRASH_ORB) {
        self->angle = (self->angle + 8) & 0x3FF;
        angle       = RSDK.Sin256(self->angle) >> 2;
    }
    else if (trash->type == FBZTRASH_ORBINAUT) {
        self->angle = (self->angle + 16) & 0x3FF;
        angle       = self->angle;
    }

    self->position.x = trash->position.x + (RSDK.Cos1024(angle) << self->radius);
    self->position.y = trash->position.y + (RSDK.Sin1024(angle) << self->radius);
}

void FBZTrash_State_OrbinautMove(void)
{
    RSDK_THIS(FBZTrash);

    EntityPlayer *targetPlayer = Player_GetNearestPlayer();
    self->angle                = RSDK.ATan2(self->position.x - targetPlayer->position.x, self->position.y - targetPlayer->position.y);
    self->velocity.x           = RSDK.Cos256(self->angle) << 8;
    self->velocity.y           = RSDK.Sin256(self->angle) << 8;
    self->direction            = self->position.x >= targetPlayer->position.x;
    self->position.x -= self->velocity.x;
    self->position.y -= self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox) && Player_CheckBadnikBreak(player, self, true)) {
            foreach_all(Animals, animals) { destroyEntity(animals); }

#if MANIA_USE_PLUS
            if (player->state != Player_State_MightyHammerDrop)
                player->velocity.y = -0x40000;
#endif
        }
    }
}

#if GAME_INCLUDE_EDITOR
void FBZTrash_EditorDraw(void)
{
    RSDK_THIS(FBZTrash);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    switch (self->type) {
        default: RSDK.SetSpriteAnimation(-1, 0, &self->animator, true, 0); break;

        case FBZTRASH_TRASH:
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x200000;
            RSDK.SetSpriteAnimation(FBZTrash->aniFrames, 0, &self->animator, true, self->frameID);
            break;
    }

    FBZTrash_Draw();
}

void FBZTrash_EditorLoad(void)
{
    FBZTrash->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Trash.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(FBZTrash, type);
    RSDK_ENUM_VAR("Loose Trash", FBZTRASH_TRASH);
    // RSDK_ENUM_VAR("(Unused)", FBZTRASH_UNUSED);
    // RSDK_ENUM_VAR("Orbinaut Orb", FBZTRASH_ORB);
    // RSDK_ENUM_VAR("Orbinaut", FBZTRASH_ORBINAUT);
}
#endif

void FBZTrash_Serialize(void)
{
    RSDK_EDITABLE_VAR(FBZTrash, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(FBZTrash, VAR_ENUM, frameID);
}
