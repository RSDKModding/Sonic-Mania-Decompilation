#include "SonicMania.h"

ObjectLRZConveyor *LRZConveyor;

void LRZConveyor_Update(void)
{
    RSDK_THIS(LRZConveyor);

    if (entity->globalControl) {
        entity->off       = LRZ2Setup->conveyorOff;
        entity->direction = entity->startDir ^ LRZ2Setup->conveyorDir;
    }
    else {
        LRZConveyor_HandleBehaviour();
    }

    LRZConveyor_HandlePlayerInteractions();

    if (!entity->off) {
        RSDK.ProcessAnimation(&entity->animator1);
        RSDK.ProcessAnimation(&entity->animator2);
    }
}

void LRZConveyor_LateUpdate(void) {}

void LRZConveyor_StaticUpdate(void) {}

void LRZConveyor_Draw(void)
{
    LRZConveyor_Unknown6();
    LRZConveyor_Unknown4();
}

void LRZConveyor_Create(void *data)
{
    RSDK_THIS(LRZConveyor);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->updateRange.x = (entity->length + 128) << 16;
    entity->updateRange.y = (entity->length * (entity->slope << 8)) + 0x800000;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->isOff         = entity->off;
    entity->startDir      = entity->direction;
    if (SceneInfo->inEditor) {
        if (!entity->speed)
            entity->speed = 8;
    }
    else {
        entity->speed        = abs(entity->speed);
        entity->taggedButton = LRZ2Setup_HandleTagSetup(entity->buttonTag, (Entity *)entity);
    }

    entity->hitbox.left   = -8;
    entity->hitbox.top    = -8;
    entity->hitbox.right  = 8;
    entity->hitbox.bottom = 8;
    RSDK.SetSpriteAnimation(LRZConveyor->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(LRZConveyor->aniFrames, 1, &entity->animator2, true, 0);
}

void LRZConveyor_StageLoad(void)
{
    LRZConveyor->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConveyor.bin", SCOPE_STAGE);

    colour lineColours[] = { 0x225BDD, 0x429AFD, 0x51DEFF, 0xAFFDF0 };
    for (int i = 0; i < 0x40; ++i) {
        LRZConveyor->lineColours[i] = lineColours[i & 3];
    }
}

void LRZConveyor_HandleBehaviour(void)
{
    RSDK_THIS(LRZConveyor);

    if (entity->buttonTag) {
        if (entity->taggedButton) {
            switch (entity->buttonBehavior) {
                case 0:
                    if (entity->taggedButton->field_70)
                        entity->off = entity->isOff == 0;
                    break;
                case 1: entity->off = entity->taggedButton->field_68 ^ entity->isOff; break;
                case 2:
                    if (entity->taggedButton->field_70)
                        entity->direction = entity->startDir == FLIP_NONE;
                    break;
                case 3: entity->direction = entity->startDir ^ (entity->taggedButton->field_68); break;
                default: break;
            }
        }
    }
}

void LRZConveyor_HandlePlayerInteractions(void)
{
    RSDK_THIS(LRZConveyor);

    int moveX = (entity->speed << 14) * (2 * (entity->direction == FLIP_X) - 1);

    foreach_active(Player, player)
    {
        int side = LRZConveyor_HandlePlayerCollisions(entity, player, NULL);
        if (side == C_TOP && !entity->off) {
            if (player->onGround)
                player->position.y += 0x10000;
            else
                player->velocity.x = moveX;
            player->position.x += moveX;
        }
        else if ((side == C_LEFT || side == C_RIGHT) && !entity->off && !player->onGround) {
            player->velocity.x = moveX;
        }
    }
}

int LRZConveyor_HandlePlayerCollisions(EntityLRZConveyor *conveyor, void *p, Hitbox *hitboxPtr)
{
    EntityPlayer *player = (EntityPlayer *)p;

    Hitbox hitbox;
    memset(&hitbox, 0, sizeof(hitbox));

    int distX = player->position.x - conveyor->position.x;

    hitbox.right = (conveyor->length + 32) >> 1;
    hitbox.left  = -(conveyor->length + 32) >> 1;

    if (abs(distX) >> 16 > conveyor->length >> 1) {
        if (abs(distX) >> 16 <= (conveyor->length >> 1) + 16) {
            int len = conveyor->length >> 1;
            if (distX >= 0) {
                if (distX >> 16 < len)
                    len = distX >> 16;
            }
            else {
                len = -(conveyor->length >> 1);
                if (distX >> 16 > len)
                    len = distX >> 16;
            }

            int slope     = (len * (conveyor->slope << 8)) >> 16;
            int angle     = RSDK.ATan2(distX - (conveyor->length << 15) * (((distX >> 31) & 0xFFFFFFFE) + 1), 0x100000);
            hitbox.top    = slope - (RSDK.Sin256(angle) >> 4);
            hitbox.bottom = slope + (RSDK.Sin256(angle) >> 4);
        }
    }
    else {
        int slope     = ((distX >> 16) * (conveyor->slope << 8)) >> 16;
        hitbox.top    = slope - 16;
        hitbox.bottom = slope + 16;
    }

    int storeDir        = conveyor->direction;
    conveyor->direction = FLIP_NONE;

    int side = C_NONE;
    if (Player_CheckValidState(player)) {
        side                = Player_CheckCollisionBox(player, conveyor, &hitbox);
        conveyor->direction = storeDir;
    }
    else {
        Hitbox playerHitbox;
        if (hitboxPtr) {
            playerHitbox = *hitboxPtr;
        }
        else {
            playerHitbox.left   = -8;
            playerHitbox.top    = -8;
            playerHitbox.right  = 8;
            playerHitbox.bottom = 8;
        }
        side                = RSDK.CheckObjectCollisionBox(conveyor, &hitbox, player, &playerHitbox, false);
        conveyor->direction = storeDir;
    }
    return side;
}

void LRZConveyor_Unknown4(void)
{
    RSDK_THIS(LRZConveyor);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->length << 15;
    drawPos.y -= (entity->slope << 8) * (entity->length >> 1);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    int len = entity->length << 16;
    if (len > 0x300000) {
        int size = (len - 0x300001) / 0x300000 + 1;
        if ((len - 0x300001) / 0x300000 != -1) {
            int incX = len / size;
            for (int i = 0; i < size; ++i) {
                drawPos.x += incX;
                drawPos.y += (incX >> 8) * entity->slope;
                RSDK.DrawSprite(&entity->animator2, &drawPos, false);
            }
        }
    }

    drawPos = entity->position;
    drawPos.x += entity->length << 15;
    drawPos.y += (entity->slope << 8) * (entity->length >> 1);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
}

void LRZConveyor_Unknown5(colour *colour, int a2, int a3, int a4, int a5, int a6, int startX, int startY)
{
    RSDK_THIS(LRZConveyor);

    int len = (entity->length >> 1) + ((entity->length & 1) != 0);

    int moveX = a3;
    int moveY = a4;
    for (int i = 0; i < len; ++i) {
        int id = 0;
        if (entity->direction == FLIP_X)
            id = len - a2 % len - 1;
        else
            id = a2 % len;
        if (!entity->off)
            id += Zone->timer >> 1;

        int x1 = startX + moveX;
        int y1 = startY + moveY;

        moveX += (a5 - a3) / len;
        moveY += (a6 - a4) / len;

        uint32 *lineClr = NULL;
        if (colour) {
            lineClr = colour;
        }
        else {
            lineClr = &LRZConveyor->lineColours[(id % len) & 0x3F];
        }
        RSDK.DrawLine(x1, y1, moveX + startX, moveY + startY, *lineClr, 127, INK_NONE, false);
        ++a2;
    }
}

void LRZConveyor_Unknown6(void)
{
    RSDK_THIS(LRZConveyor);

    colour clr = 0x51DEFF;

    int slopeLen = (entity->length >> 1) * (entity->slope << 8);
    int endX     = (entity->length << 15) + entity->position.x;
    int startX   = entity->position.x - (entity->length << 15);
    int y1       = (entity->length >> 1) + ((entity->length & 1) != 0);
    int y2       = slopeLen + entity->position.y + 0xF0000;
    int y3       = slopeLen + entity->position.y - 0x100000;
    int y4       = entity->position.y - slopeLen;

    LRZConveyor_Unknown5(NULL, 0, startX, y4 - 0x100000, endX, y3, 0, 0);
    LRZConveyor_Unknown5(NULL, y1, endX, y2, startX, y4 + 0xF0000, 0, 0);
    LRZConveyor_Unknown5(NULL, 0, startX, y4 - 0x100000, endX, y3, 0, 0x10000);
    LRZConveyor_Unknown5(NULL, y1, endX, y2, startX, y4 + 0xF0000, 0, -0x10000);
    LRZConveyor_Unknown5(&clr, 0, startX, y4 - 0x100000, endX, y3, 0, 0x20000);
    LRZConveyor_Unknown5(&clr, y1, endX, y2, startX, y4 + 0xF0000, 0, -0x20000);
}

#if RETRO_INCLUDE_EDITOR
void LRZConveyor_EditorDraw(void)
{
    RSDK_THIS(LRZConveyor);

    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->updateRange.x = (entity->length + 128) << 16;
    entity->updateRange.y = (entity->length * (entity->slope << 8)) + 0x800000;
    entity->startDir      = entity->direction;

    int speed     = entity->speed;
    entity->speed = abs(entity->speed);
    entity->speed = speed;

    LRZConveyor_Draw();
}

void LRZConveyor_EditorLoad(void) { LRZConveyor_StageLoad(); }
#endif

void LRZConveyor_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_ENUM, slope);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_UINT32, length);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_BOOL, globalControl);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_UINT8, buttonBehavior);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_BOOL, off);
}
