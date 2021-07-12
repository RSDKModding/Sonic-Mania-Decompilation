#include "../SonicMania.h"

ObjectFBZSinkTrash *FBZSinkTrash;

void FBZSinkTrash_Update(void)
{
    RSDK_THIS(FBZSinkTrash);
    if (entity->type != 2) {
        if (entity->type) {
            foreach_active(Player, player) { Player_CheckCollisionPlatform(player, entity, &entity->hitbox1); }

            foreach_active(SignPost, signPost)
            {
                if (signPost->state == SignPost_State_Fall && signPost->position.y + 0x180000 > entity->position.y - (entity->size.y >> 1)) {
                    RSDK.PlaySFX(SignPost->sfx_Slide, 0, 255);
                    signPost->spinCount  = 4;
                    signPost->position.y = entity->position.y - (entity->size.y >> 1) - 0x180000;
                    signPost->velocity.y = 0;
                    Music_FadeOut(0.025);
                    signPost->state = SignPost_State_Land;
                }
            }
        }
        else {
            foreach_active(Player, player)
            {
                if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox2)) {
#if RETRO_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop) {
                            player->velocity.y = player->velocity.y - (player->velocity.y >> 5) - (player->velocity.y >> 4);
                            if (player->velocity.y < 0x30000) {
                                player->state         = Player_State_Air;
                                player->onGround      = true;
                                player->velocity.y    = 0;
                                player->collisionMode = 0;
                                player->angle         = 0;
                                player->position.y += 0x10000;
                            }
                        }
#endif
                        if ((Zone->timer & 7) == 0) {
                            EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Fall,
                                                                                     player->position.x + RSDK.Rand(-0x40000, 0x40000),
                                                                                     player->position.y + RSDK.Rand(-0x100000, 0));
                            RSDK.SetSpriteAnimation(FBZSinkTrash->aniFrames, 0, &debris->data, false, RSDK.Rand(0, 20));
                            debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                            debris->velocity.y    = player->velocity.y;
                            debris->gravity       = 0x3800;
                            debris->updateRange.y = 0x200000;
                            debris->updateRange.x = 0x200000;
                            debris->drawOrder     = Zone->drawOrderLow;
                        }
                    }
                }
                else if (player->velocity.y >= 0) {
#if RETRO_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop) {
                        player->velocity.y = player->velocity.y - (player->velocity.y >> 5) - (player->velocity.y >> 4);
                        if (player->velocity.y >= 0x30000) {
                            int speed = abs(player->velocity.y + player->velocity.x);
                            if (speed > 0x10000 && (Zone->timer & 7) == 0) {
                                EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Fall,
                                                                                         player->position.x + RSDK.Rand(-0x40000, 0x40000),
                                                                                         player->position.y + 0x40000 + RSDK.Rand(-0x40000, 0x40000));
                                RSDK.SetSpriteAnimation(FBZSinkTrash->aniFrames, RSDK.Rand(0, 2) + 9, &debris->data, false, 0);
                                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                                debris->velocity.y    = -0x20000;
                                debris->gravity       = 0x3800;
                                debris->updateRange.y = 0x200000;
                                debris->updateRange.x = 0x200000;
                                debris->drawOrder     = Zone->drawOrderLow;
                            }
                        }
                        else {
                            player->state         = Player_State_Air;
                            player->onGround      = true;
                            player->velocity.y    = 0;
                            player->collisionMode = CMODE_FLOOR;
                            player->angle         = 0;
                            player->position.y += 0x10000;
                            int speed = abs(player->velocity.y + player->velocity.x);
                            if (speed > 0x10000 && (Zone->timer & 7) == 0) {
                                EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Fall,
                                                                                         player->position.x + RSDK.Rand(-0x40000, 0x40000),
                                                                                         player->position.y + 0x40000 + RSDK.Rand(-0x40000, 0x40000));
                                RSDK.SetSpriteAnimation(FBZSinkTrash->aniFrames, RSDK.Rand(0, 2) + 9, &debris->data, false, 0);
                                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                                debris->velocity.y    = -0x20000;
                                debris->gravity       = 0x3800;
                                debris->updateRange.y = 0x200000;
                                debris->updateRange.x = 0x200000;
                                debris->drawOrder     = Zone->drawOrderLow;
                            }
                        }
                    }
                    else {
#endif
                        player->onGround      = true;
                        player->velocity.y    = 0;
                        player->collisionMode = CMODE_FLOOR;
                        player->angle         = 0;
                        player->position.y += 0x10000;
                        int speed = abs(player->velocity.y + player->velocity.x);
                        if (speed > 0x10000 && (Zone->timer & 7) == 0) {
                            EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, Debris_State_Fall,
                                                                                     player->position.x + RSDK.Rand(-0x40000, 0x40000),
                                                                                     player->position.y + 0x40000 + RSDK.Rand(-0x40000, 0x40000));
                            RSDK.SetSpriteAnimation(FBZSinkTrash->aniFrames, RSDK.Rand(0, 2) + 9, &debris->data, false, 0);
                            debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                            debris->velocity.y    = -0x20000;
                            debris->gravity       = 0x3800;
                            debris->updateRange.y = 0x200000;
                            debris->updateRange.x = 0x200000;
                            debris->drawOrder     = Zone->drawOrderLow;
                        }
#if RETRO_USE_PLUS
                    }
#endif
                }
            }
        }
    }
}

void FBZSinkTrash_LateUpdate(void) {}

void FBZSinkTrash_StaticUpdate(void) {}

void FBZSinkTrash_Draw(void)
{
    RSDK_THIS(FBZSinkTrash);
    Vector2 drawPos;

    entity->direction  = FLIP_NONE;
    int drawX          = entity->position.x - (entity->size.x >> 1);
    SpriteFrame *frame = RSDK.GetFrame(FBZSinkTrash->aniFrames, 11, 0);
    drawPos.x          = entity->position.x - (entity->size.x >> 1);
    drawPos.y          = entity->position.y - (entity->size.y >> 1);
    frame->width       = 64;
    for (int i = 0; i < entity->size.x; i += 0x400000) {
        if (entity->size.x - i < 0x400000)
            frame->width = (entity->size.x - i) >> 16;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        drawPos.x += 0x400000;
    }

    frame         = RSDK.GetFrame(FBZSinkTrash->aniFrames, 11, 1);
    frame->height = 64;
    for (int i = 0; i < entity->size.y; i += 0x400000) {
        drawPos.x    = drawX;
        frame->width = 64;
        if (entity->size.y - i < 0x400000)
            frame->height = (entity->size.y - i) >> 16;

        for (int x = 0; x < entity->size.x; x += 0x400000) {
            if (entity->size.x - x < 0x400000)
                frame->width = (entity->size.x - x) >> 16;
            RSDK.DrawSprite(&entity->animator3, &drawPos, false);
            drawPos.x += 0x400000;
        }
        drawPos.y += 0x400000;
    }

    frame             = RSDK.GetFrame(FBZSinkTrash->aniFrames, 11, 0);
    entity->direction = FLIP_Y;
    drawPos.x         = drawX;
    frame->width      = 64;
    for (int i = 0; i < entity->size.x; i += 0x400000) {
        if (entity->size.x - i < 0x400000)
            frame->width = (entity->size.x - i) >> 16;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        drawPos.x += 0x400000;
    }

    if (entity->type < 1) {
        for (int i = 0; i < 64; ++i) {
            drawPos = entity->positions[i];
            drawPos.x += entity->position.x;
            drawPos.y += entity->position.y;
            entity->animator1.frameID = entity->frameIDs[i];
            entity->direction         = entity->directions[i];
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        }

        entity->direction = FLIP_NONE;
    }
}

void FBZSinkTrash_Create(void *data)
{
    RSDK_THIS(FBZSinkTrash);
    if (!RSDK_sceneInfo->inEditor) {
        entity->updateRange.x = entity->size.x >> 1;
        entity->updateRange.y = (entity->size.y >> 1) + 0x400000;
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawFX        = FX_FLIP;
        if (entity->type == 0)
            entity->drawOrder = Zone->drawOrderHigh - 2;
        else
            entity->drawOrder = Zone->drawOrderLow;

        entity->hitbox1.left   = -(entity->size.x >> 17);
        entity->hitbox1.top    = -(entity->size.y >> 17);
        entity->hitbox1.right  = entity->size.x >> 17;
        entity->hitbox1.bottom = 24 - (entity->size.y >> 17);

        entity->hitbox2.left   = -(entity->size.x >> 17);
        entity->hitbox2.top    = (entity->size.y >> 17) - 16;
        entity->hitbox2.right  = entity->size.x >> 17;
        entity->hitbox2.bottom = entity->size.y >> 17;

        RSDK.SetSpriteAnimation(FBZSinkTrash->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(FBZSinkTrash->aniFrames, 11, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(FBZSinkTrash->aniFrames, 11, &entity->animator3, true, 1);

        for (int i = 0; i < 64; ++i) {
            entity->positions[i].x = RSDK.Rand(-(entity->size.x >> 1), entity->size.x >> 1);
            entity->positions[i].y = RSDK.Rand(-(entity->size.y >> 1), entity->size.y >> 1);
            entity->frameIDs[i]    = RSDK.Rand(0, 22);
            entity->directions[i]  = RSDK.Rand(0, 4);
        }
    }
}

void FBZSinkTrash_StageLoad(void) { FBZSinkTrash->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Trash.bin", SCOPE_STAGE); }

void FBZSinkTrash_EditorDraw(void) {}

void FBZSinkTrash_EditorLoad(void) {}

void FBZSinkTrash_Serialize(void)
{
    RSDK_EDITABLE_VAR(FBZSinkTrash, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(FBZSinkTrash, VAR_VECTOR2, size);
}
