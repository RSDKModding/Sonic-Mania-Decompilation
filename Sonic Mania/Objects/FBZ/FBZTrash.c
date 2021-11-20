#include "SonicMania.h"

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
            self->type          = voidToInt(data);
            self->drawOrder     = Zone->drawOrderLow;
            self->hitbox.left   = -8;
            self->hitbox.top    = -8;
            self->hitbox.right  = 8;
            self->hitbox.bottom = 8;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->state         = FBZTrash_Unknown4;
        }
        else {
            self->drawOrder     = Zone->drawOrderHigh;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x200000;
            self->onGround      = true;

            self->direction = RSDK.Rand(0, 2);
            RSDK.SetSpriteAnimation(FBZTrash->aniFrames, 0, &self->animator, true, self->frameID);
            self->state = FBZTrash_Unknown3;
        }
    }
}

void FBZTrash_StageLoad(void) { FBZTrash->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Trash.bin", SCOPE_STAGE); }

void FBZTrash_Unknown1(EntityFBZTrash *trashPtr, int32 angle)
{
    EntityFBZTrash *trash = CREATE_ENTITY(FBZTrash, intToVoid(2), trashPtr->position.x, trashPtr->position.y);
    trash->position.x += RSDK.Cos1024(angle) << 10;
    trash->position.y += RSDK.Sin1024(angle) << 10;
    trash->field_70   = trash->position;
    trash->position.x = (RSDK.Rand(0, (BigSqueeze->value4[3] - BigSqueeze->value4[2]) >> 16) << 16) + BigSqueeze->value4[2];
    trash->position.y = BigSqueeze->value7 - 0x80000;
    trash->parent     = (Entity *)trashPtr;
    trash->field_78   = trash->position;
    trash->angle      = angle;
    trash->field_6C   = 10;
    trash->state      = FBZTrash_Unknown4;
    RSDK.SetSpriteAnimation(FBZTrash->aniFrames, 5, &trash->animator, true, RSDK.Rand(0, 2));
}

void FBZTrash_Unknown2(int32 x, int32 y)
{
    EntityFBZTrash *trash = CREATE_ENTITY(FBZTrash, intToVoid(3), x, y);
    FBZTrash_Unknown1(trash, 0);
    FBZTrash_Unknown1(trash, 512);
    trash->field_70   = trash->position;
    trash->position.x = (RSDK.Rand(0, (BigSqueeze->value4[3] - BigSqueeze->value4[2]) >> 16) << 16) + BigSqueeze->value4[2];
    trash->position.y = BigSqueeze->value7 - 0x80000;
    trash->state      = FBZTrash_Unknown4;
    trash->field_78   = trash->position;
    RSDK.SetSpriteAnimation(FBZTrash->aniFrames, 1, &trash->animator, true, (x >> 17) & 1);
}

void FBZTrash_Unknown3(void)
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
                if (rx * rx + ry * ry < 288) {
                    if (BigSqueeze->isRumbling)
                        self->field_64 = (self->field_64 + trash->field_64) >> 1;
                    else
                        self->field_64 >>= 1;

                    if (self->field_64 <= 0) {
                        self->onGround = true;
                    }
                    else {
                        int32 angle          = RSDK.ATan2(rx, ry);
                        self->velocity.x = minVal(self->field_64 << 6, 640) * RSDK.Cos256(angle);
                        self->velocity.y = minVal(self->field_64 << 6, 640) * RSDK.Sin256(angle + RSDK.Rand(-32, 32));
                        self->onGround   = false;
                    }
                }
            }
        }
    }

    if (self->position.y < BigSqueeze->value7 - 0xC0000)
        self->position.y = BigSqueeze->value7 - 0xC0000;

    foreach_active(BigSqueeze, boss)
    {
        switch (boss->type) {
            case 1:
                if (self->position.y >  boss->position.y + 0xC00000) {
                    self->position.y =  boss->position.y + 0xC00000;
                    self->velocity.y = -abs(self->velocity.y >> 1);
                }
                break;
            case 2:
                if (self->position.x < boss->position.x + 0x180000) {
                    self->position.x = boss->position.x + 0x180000;
                    self->velocity.x = 5 * boss->velocity.x;
                    if (self->onGround && BigSqueeze->isRumbling) {
                        self->field_64 = 128;
                        self->onGround = false;
                    }
                }
                break;
            case 3:
                if (self->position.x > boss->position.x - 0x180000) {
                    self->position.x = boss->position.x - 0x180000;
                    self->velocity.x = 5 * boss->velocity.x;
                    if (self->onGround && BigSqueeze->isRumbling) {
                        self->field_64 = 128;
                        self->onGround = false;
                    }
                }
                break;
        }
    }
}

void FBZTrash_Unknown4(void)
{
    RSDK_THIS(FBZTrash);
    self->field_78.y -= 0x8000;
    self->position.x = self->field_78.x + RSDK.Rand(-0x20000, 0x20000);
    self->position.y = self->field_78.y + RSDK.Rand(-0x20000, 0x20000);
    if (++self->timer == 16) {
        self->timer      = 0;
        self->velocity.x = (self->field_70.x - self->position.x) >> 5;
        self->velocity.y = -0x80000;
        self->state      = FBZTrash_Unknown5;
    }
}

void FBZTrash_Unknown5(void)
{
    RSDK_THIS(FBZTrash);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    ++self->timer;
    if (self->velocity.y >= 0) {
        if (self->position.y > self->field_70.y)
            self->position.y = self->field_70.y;
    }

    if (self->velocity.x <= 0) {
        if (self->position.x < self->field_70.x) {
            self->velocity.x = 0;
            self->position.x = self->field_70.x;
        }
    }
    else if (self->position.x > self->field_70.x) {
        self->velocity.x = 0;
        self->position.x = self->field_70.x;
    }

    if (self->timer == 60) {
        self->timer = 0;
        switch (self->type) {
            default: break;
            case 2: self->state = FBZTrash_Unknown6; break;
            case 3: self->state = FBZTrash_Unknown7; break;
        }
    }
}

void FBZTrash_Unknown6(void)
{
    RSDK_THIS(FBZTrash);
    EntityFBZTrash *trash = (EntityFBZTrash *)self->parent;

    int32 angle = 0;
    if (trash->type == 2) {
        self->angle = (self->angle + 8) & 0x3FF;
        angle         = RSDK.Sin256(self->angle) >> 2;
    }
    else if (trash->type == 3) {
        self->angle = (self->angle + 16) & 0x3FF;
        angle         = self->angle;
    }
    self->position.x = trash->position.x;
    self->position.y = trash->position.y;
    self->position.x += RSDK.Cos1024(angle) << self->field_6C;
    self->position.y += RSDK.Sin1024(angle) << self->field_6C;
}

void FBZTrash_Unknown7(void)
{
    RSDK_THIS(FBZTrash);
    EntityPlayer *playerPtr = Player_GetNearestPlayer();
    self->angle           = RSDK.ATan2(self->position.x - playerPtr->position.x, self->position.y - playerPtr->position.y);
    self->velocity.x      = RSDK.Cos256(self->angle) << 8;
    self->velocity.y      = RSDK.Sin256(self->angle) << 8;
    self->direction       = self->position.x >= playerPtr->position.x;
    self->position.x -= self->velocity.x;
    self->position.y -= self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox) && Player_CheckBadnikBreak(self, player, true)) {
            foreach_all(Animals, animals) { destroyEntity(animals); }
#if RETRO_USE_PLUS
            if (player->state != Player_State_MightyHammerDrop)
                player->velocity.y = -0x40000;
#endif
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void FBZTrash_EditorDraw(void) {}

void FBZTrash_EditorLoad(void) {}
#endif

void FBZTrash_Serialize(void)
{
    RSDK_EDITABLE_VAR(FBZTrash, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(FBZTrash, VAR_ENUM, frameID);
}
