#include "SonicMania.h"

ObjectFBZTrash *FBZTrash;

void FBZTrash_Update(void)
{
    RSDK_THIS(FBZTrash);
    StateMachine_Run(entity->state);
}

void FBZTrash_LateUpdate(void) {}

void FBZTrash_StaticUpdate(void) {}

void FBZTrash_Draw(void)
{
    RSDK_THIS(FBZTrash);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void FBZTrash_Create(void *data)
{
    RSDK_THIS(FBZTrash);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active  = ACTIVE_BOUNDS;
        entity->visible = true;
        entity->drawFX  = FX_FLIP;
        if (data) {
            entity->type          = voidToInt(data);
            entity->drawOrder     = Zone->drawOrderLow;
            entity->hitbox.left   = -8;
            entity->hitbox.top    = -8;
            entity->hitbox.right  = 8;
            entity->hitbox.bottom = 8;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            entity->state         = FBZTrash_Unknown4;
        }
        else {
            entity->drawOrder     = Zone->drawOrderHigh;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x200000;
            entity->onGround      = true;

            entity->direction = RSDK.Rand(0, 2);
            RSDK.SetSpriteAnimation(FBZTrash->aniFrames, 0, &entity->animator, true, entity->frameID);
            entity->state = FBZTrash_Unknown3;
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
    if (!entity->onGround) {
        entity->velocity.y += 0x3800;
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
    }

    if (entity->velocity.y >= -0x8000) {
        foreach_active(FBZTrash, trash)
        {
            if (trash != entity) {
                int32 rx = (entity->position.x - trash->position.x) >> 16;
                int32 ry = (entity->position.y - trash->position.y) >> 16;
                if (rx * rx + ry * ry < 288) {
                    if (BigSqueeze->isRumbling)
                        entity->field_64 = (entity->field_64 + trash->field_64) >> 1;
                    else
                        entity->field_64 >>= 1;

                    if (entity->field_64 <= 0) {
                        entity->onGround = true;
                    }
                    else {
                        int32 angle          = RSDK.ATan2(rx, ry);
                        entity->velocity.x = minVal(entity->field_64 << 6, 640) * RSDK.Cos256(angle);
                        entity->velocity.y = minVal(entity->field_64 << 6, 640) * RSDK.Sin256(angle + RSDK.Rand(-32, 32));
                        entity->onGround   = false;
                    }
                }
            }
        }
    }

    if (entity->position.y < BigSqueeze->value7 - 0xC0000)
        entity->position.y = BigSqueeze->value7 - 0xC0000;

    foreach_active(BigSqueeze, boss)
    {
        switch (boss->type) {
            case 1:
                if (entity->position.y >  boss->position.y + 0xC00000) {
                    entity->position.y =  boss->position.y + 0xC00000;
                    entity->velocity.y = -abs(entity->velocity.y >> 1);
                }
                break;
            case 2:
                if (entity->position.x < boss->position.x + 0x180000) {
                    entity->position.x = boss->position.x + 0x180000;
                    entity->velocity.x = 5 * boss->velocity.x;
                    if (entity->onGround && BigSqueeze->isRumbling) {
                        entity->field_64 = 128;
                        entity->onGround = false;
                    }
                }
                break;
            case 3:
                if (entity->position.x > boss->position.x - 0x180000) {
                    entity->position.x = boss->position.x - 0x180000;
                    entity->velocity.x = 5 * boss->velocity.x;
                    if (entity->onGround && BigSqueeze->isRumbling) {
                        entity->field_64 = 128;
                        entity->onGround = false;
                    }
                }
                break;
        }
    }
}

void FBZTrash_Unknown4(void)
{
    RSDK_THIS(FBZTrash);
    entity->field_78.y -= 0x8000;
    entity->position.x = entity->field_78.x + RSDK.Rand(-0x20000, 0x20000);
    entity->position.y = entity->field_78.y + RSDK.Rand(-0x20000, 0x20000);
    if (++entity->timer == 16) {
        entity->timer      = 0;
        entity->velocity.x = (entity->field_70.x - entity->position.x) >> 5;
        entity->velocity.y = -0x80000;
        entity->state      = FBZTrash_Unknown5;
    }
}

void FBZTrash_Unknown5(void)
{
    RSDK_THIS(FBZTrash);

    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    ++entity->timer;
    if (entity->velocity.y >= 0) {
        if (entity->position.y > entity->field_70.y)
            entity->position.y = entity->field_70.y;
    }

    if (entity->velocity.x <= 0) {
        if (entity->position.x < entity->field_70.x) {
            entity->velocity.x = 0;
            entity->position.x = entity->field_70.x;
        }
    }
    else if (entity->position.x > entity->field_70.x) {
        entity->velocity.x = 0;
        entity->position.x = entity->field_70.x;
    }

    if (entity->timer == 60) {
        entity->timer = 0;
        switch (entity->type) {
            default: break;
            case 2: entity->state = FBZTrash_Unknown6; break;
            case 3: entity->state = FBZTrash_Unknown7; break;
        }
    }
}

void FBZTrash_Unknown6(void)
{
    RSDK_THIS(FBZTrash);
    EntityFBZTrash *trash = (EntityFBZTrash *)entity->parent;

    int32 angle = 0;
    if (trash->type == 2) {
        entity->angle = (entity->angle + 8) & 0x3FF;
        angle         = RSDK.Sin256(entity->angle) >> 2;
    }
    else if (trash->type == 3) {
        entity->angle = (entity->angle + 16) & 0x3FF;
        angle         = entity->angle;
    }
    entity->position.x = trash->position.x;
    entity->position.y = trash->position.y;
    entity->position.x += RSDK.Cos1024(angle) << entity->field_6C;
    entity->position.y += RSDK.Sin1024(angle) << entity->field_6C;
}

void FBZTrash_Unknown7(void)
{
    RSDK_THIS(FBZTrash);
    EntityPlayer *playerPtr = Player_GetNearestPlayer();
    entity->angle           = RSDK.ATan2(entity->position.x - playerPtr->position.x, entity->position.y - playerPtr->position.y);
    entity->velocity.x      = RSDK.Cos256(entity->angle) << 8;
    entity->velocity.y      = RSDK.Sin256(entity->angle) << 8;
    entity->direction       = entity->position.x >= playerPtr->position.x;
    entity->position.x -= entity->velocity.x;
    entity->position.y -= entity->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && Player_CheckBadnikBreak(entity, player, true)) {
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
