#include "RetroEngine.hpp"

int collisionTolerance = 0;
int collisionOffset    = 0;
int collisionMaskAir   = 0;

int collisionLeft_Outer   = 0;
int collisionTop_Outer    = 0;
int collisionRight_Outer  = 0;
int collisionBottom_Outer = 0;

int collisionLeft_Inner   = 0;
int collisionTop_Inner    = 0;
int collisionRight_Inner  = 0;
int collisionBottom_Inner = 0;

Entity *collisionEntity = NULL;

CollisionSensor sensors[6];

bool32 CheckObjectCollisionTouch(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox)
{
    int store, store2, store3;
    if (!thisEntity || !otherEntity || !thisHitbox || !otherHitbox)
        return 0;
    switch (thisEntity->direction) {
        case FLIP_X:
            store             = -thisHitbox->left;
            thisHitbox->left  = -thisHitbox->right;
            thisHitbox->right = store;
            break;
        case FLIP_Y:
            store              = -thisHitbox->top;
            thisHitbox->top    = -thisHitbox->bottom;
            thisHitbox->bottom = store;
            break;
        case FLIP_XY:
            store              = -thisHitbox->left;
            thisHitbox->left   = -thisHitbox->right;
            store2             = thisHitbox->bottom;
            thisHitbox->right  = store;
            store3             = -thisHitbox->top;
            thisHitbox->top    = -store2;
            thisHitbox->bottom = store3;
            break;
    }
    switch (thisEntity->direction) {
        case FLIP_X:
            store              = -otherHitbox->left;
            otherHitbox->left  = -otherHitbox->right;
            otherHitbox->right = store;
            break;
        case FLIP_Y:
            store               = -otherHitbox->top;
            otherHitbox->top    = -otherHitbox->bottom;
            otherHitbox->bottom = store;
            break;
        case FLIP_XY:
            store               = -otherHitbox->left;
            otherHitbox->left   = -otherHitbox->right;
            store2              = otherHitbox->bottom;
            otherHitbox->right  = store;
            store3              = -otherHitbox->top;
            otherHitbox->top    = -store2;
            otherHitbox->bottom = store3;
            break;
    }

    int thisIX  = thisEntity->position.x >> 0x10;
    int thisIY  = thisEntity->position.y >> 0x10;
    int otherIX = otherEntity->position.x >> 0x10;
    int otherIY = otherEntity->position.y >> 0x10;

    bool32 result = false;
    if (thisIX + thisHitbox->left >= otherIX + otherHitbox->right || thisIX + thisHitbox->right <= otherIX + otherHitbox->left
        || thisIY + thisHitbox->top >= otherIY + otherHitbox->bottom || thisIY + thisHitbox->bottom <= otherIY + otherHitbox->top) {
        result = 0;
    }
    else {
        result = 1;
    }

    switch (thisEntity->direction) {
        case FLIP_X:
            store             = -thisHitbox->left;
            thisHitbox->left  = -thisHitbox->right;
            thisHitbox->right = store;
            break;
        case FLIP_Y:
            store              = -thisHitbox->top;
            thisHitbox->top    = -thisHitbox->bottom;
            thisHitbox->bottom = store;
            break;
        case FLIP_XY:
            store              = -thisHitbox->left;
            thisHitbox->left   = -thisHitbox->right;
            store2             = thisHitbox->bottom;
            thisHitbox->right  = store;
            store3             = -thisHitbox->top;
            thisHitbox->top    = -store2;
            thisHitbox->bottom = store3;
            break;
    }
    switch (thisEntity->direction) {
        case FLIP_X:
            store              = -otherHitbox->left;
            otherHitbox->left  = -otherHitbox->right;
            otherHitbox->right = store;
            break;
        case FLIP_Y:
            store               = -otherHitbox->top;
            otherHitbox->top    = -otherHitbox->bottom;
            otherHitbox->bottom = store;
            return result;
        case FLIP_XY:
            store2              = otherHitbox->left;
            otherHitbox->left   = -otherHitbox->right;
            store2              = otherHitbox->bottom;
            otherHitbox->right  = -store2;
            store3              = -otherHitbox->top;
            otherHitbox->top    = -store2;
            otherHitbox->bottom = store3;
            return result;
    }
    return result;
}

byte CheckObjectCollisionBox(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setValues)
{
    if (thisEntity && otherEntity && thisHitbox && otherHitbox) {
        int store, store2, store3;
        int collisionResultX = 0;
        int collisionResultY = 0;

        int otherX      = otherEntity->position.x;
        int otherY      = otherEntity->position.y;
        int otherX2     = otherEntity->position.x;
        int otherY2     = otherEntity->position.y;
        switch (thisEntity->direction) {
            case FLIP_X:
                store             = -thisHitbox->left;
                thisHitbox->left  = -thisHitbox->right;
                thisHitbox->right = store;
                break;
            case FLIP_Y:
                store              = -thisHitbox->top;
                thisHitbox->top    = -thisHitbox->bottom;
                thisHitbox->bottom = store;
                break;
            case FLIP_XY:
                store              = -thisHitbox->left;
                thisHitbox->left   = -thisHitbox->right;
                store2             = thisHitbox->bottom;
                thisHitbox->right  = store;
                store3             = -thisHitbox->top;
                thisHitbox->top    = -store2;
                thisHitbox->bottom = store3;
                break;
        }
        switch (thisEntity->direction) {
            case FLIP_X:
                store              = -otherHitbox->left;
                otherHitbox->left  = -otherHitbox->right;
                otherHitbox->right = store;
                break;
            case FLIP_Y:
                store               = -otherHitbox->top;
                otherHitbox->top    = -otherHitbox->bottom;
                otherHitbox->bottom = store;
                break;
            case FLIP_XY:
                store               = -otherHitbox->left;
                otherHitbox->left   = -otherHitbox->right;
                store2              = otherHitbox->bottom;
                otherHitbox->right  = store;
                store3              = -otherHitbox->top;
                otherHitbox->top    = -store2;
                otherHitbox->bottom = store3;
                break;
        }
        int thisIX  = thisEntity->position.x >> 0x10;
        int thisIY  = thisEntity->position.y >> 0x10;
        int otherIX = otherEntity->position.x >> 0x10;
        int otherIY = otherEntity->position.y >> 0x10;

        otherHitbox->top++;
        otherHitbox->bottom--;

        if (otherIX <= (thisHitbox->right + thisHitbox->left + 2 * thisIX) >> 1) {
            if (otherHitbox->right + otherIX >= thisHitbox->left + thisIX && thisIY + thisHitbox->top < otherIY + otherHitbox->bottom
                && thisIY + thisHitbox->bottom > otherIY + otherHitbox->top) {
                collisionResultX = 2;
                otherX          = thisEntity->position.x + ((thisHitbox->left - otherHitbox->right) << 16);
            }
        }
        else if (otherHitbox->left + otherIX < thisHitbox->right + thisIX && thisIY + thisHitbox->top < otherIY + otherHitbox->bottom
                 && thisIY + thisHitbox->bottom > otherIY + otherHitbox->top) {
            collisionResultX = 3;
            otherX          = thisEntity->position.x + ((thisHitbox->right - otherHitbox->left) << 16);
        }
        otherHitbox->bottom++;
        otherHitbox->top--;
        otherHitbox->left++;
        otherHitbox->right--;

        if (otherIY >= (thisHitbox->top + thisHitbox->bottom + 2 * thisIY) >> 1) {
            if (otherHitbox->top + otherIY < thisHitbox->bottom + thisIY && thisIX + thisHitbox->left < otherIX + otherHitbox->right) {
                if (otherIX + otherHitbox->left < thisIX + thisHitbox->right) {
                    collisionResultY = 4;
                    otherY2      = thisEntity->position.y + ((thisHitbox->bottom - otherHitbox->top) << 16);
                }
            }
        }
        else if (otherHitbox->bottom + otherIY >= thisHitbox->top + thisIY && thisIX + thisHitbox->left < otherIX + otherHitbox->right
                 && thisIX + thisHitbox->right > otherIX + otherHitbox->left) {
            collisionResultY = 1;
            otherY2      = thisEntity->position.y + ((thisHitbox->top - otherHitbox->bottom) << 16);
        }

        otherHitbox->left--;
        otherHitbox->right++;
        switch (thisEntity->direction) {
            case FLIP_X:
                store             = -thisHitbox->left;
                thisHitbox->left  = -thisHitbox->right;
                thisHitbox->right = store;
                break;
            case FLIP_Y:
                store              = -thisHitbox->top;
                thisHitbox->top    = -thisHitbox->bottom;
                thisHitbox->bottom = store;
                break;
            case FLIP_XY:
                store              = -thisHitbox->left;
                thisHitbox->left   = -thisHitbox->right;
                store2             = thisHitbox->bottom;
                thisHitbox->right  = store;
                store3             = -thisHitbox->top;
                thisHitbox->top    = -store2;
                thisHitbox->bottom = store3;
                break;
        }
        switch (thisEntity->direction) {
            case FLIP_X:
                store              = -otherHitbox->left;
                otherHitbox->left  = -otherHitbox->right;
                otherHitbox->right = store;
                break;
            case FLIP_Y:
                store               = -otherHitbox->top;
                otherHitbox->top    = -otherHitbox->bottom;
                otherHitbox->bottom = store;
                break;
            case FLIP_XY:
                store               = -otherHitbox->left;
                otherHitbox->left   = -otherHitbox->right;
                store2              = otherHitbox->bottom;
                otherHitbox->right  = store;
                store3              = -otherHitbox->top;
                otherHitbox->top    = -store2;
                otherHitbox->bottom = store3;
                break;
        }

        int ox  = ((otherX - otherEntity->position.x) >> 0x10);
        int oy  = ((otherY - otherEntity->position.y) >> 0x10);
        int ox2 = ((otherX2 - otherEntity->position.x) >> 0x10);
        int oy2 = ((otherY2 - otherEntity->position.y) >> 0x10);
        if (ox * ox + oy * oy >= ox2 * ox2 + oy2 * oy2) {
            if (collisionResultY || !collisionResultX) {
                otherEntity->position.x = otherX2;
                otherEntity->position.y = otherY2;
                if (setValues) {
                    if (collisionResultY == 1) {
                        if (otherEntity->velocity.y > 0)
                            otherEntity->velocity.y = 0;
                        if (!otherEntity->onGround && otherEntity->velocity.y >= 0) {
                            otherEntity->groundVel = otherEntity->velocity.x;
                            otherEntity->angle     = 0;
                            otherEntity->onGround  = true;
                        }
                    }
                    else if (collisionResultY == 4 && otherEntity->velocity.y < 0) {
                        otherEntity->velocity.y = 0;
                    }
                }
                return collisionResultY;
            }
        }
        else if (!collisionResultX && collisionResultY) {
            otherEntity->position.x = otherX2;
            otherEntity->position.y = otherY2;
            if (setValues) {
                if (collisionResultY == 1) {
                    if (otherEntity->velocity.y > 0)
                        otherEntity->velocity.y = 0;
                    if (!otherEntity->onGround && otherEntity->velocity.y >= 0) {
                        otherEntity->groundVel = otherEntity->velocity.x;
                        otherEntity->angle     = 0;
                        otherEntity->onGround  = true;
                    }
                }
                else if (collisionResultY == 4 && otherEntity->velocity.y < 0) {
                    otherEntity->velocity.y = 0;
                }
            }
            return collisionResultY;
        }

        otherEntity->position.x = otherX;
        otherEntity->position.y = otherY;
        if (!setValues)
            return collisionResultX;

        int vel = 0;
        if (otherEntity->onGround) {
            vel = otherEntity->groundVel;
            if (otherEntity->collisionMode == CMODE_ROOF)
                vel = -vel;
        }
        else {
            vel = otherEntity->velocity.x;
        }

        if (collisionResultX == 2) {
            if (vel <= 0)
                return collisionResultX;
        }
        else if (collisionResultX != 3 || vel >= 0) {
            return collisionResultX;
        }
        otherEntity->groundVel  = 0;
        otherEntity->velocity.x = 0;
        return collisionResultX;
    }
    return 0;
}

bool32 CheckObjectCollisionPlatform(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setValues)
{
    int store;
    int store2;
    int store3;
    bool32 status;

    if (!thisEntity || !otherEntity || !thisHitbox || !otherHitbox)
        return 0;
    switch (thisEntity->direction) {
        case FLIP_X:
            store             = -thisHitbox->left;
            thisHitbox->left  = -thisHitbox->right;
            thisHitbox->right = store;
            break;
        case FLIP_Y:
            store              = -thisHitbox->top;
            thisHitbox->top    = -thisHitbox->bottom;
            thisHitbox->bottom = store;
            break;
        case FLIP_XY:
            store              = -thisHitbox->left;
            thisHitbox->left   = -thisHitbox->right;
            store2             = thisHitbox->bottom;
            thisHitbox->right  = store;
            store3             = -thisHitbox->top;
            thisHitbox->top    = -store2;
            thisHitbox->bottom = store3;
            break;
    }
    switch (thisEntity->direction) {
        case FLIP_X:
            store              = -otherHitbox->left;
            otherHitbox->left  = -otherHitbox->right;
            otherHitbox->right = store;
            break;
        case FLIP_Y:
            store               = -otherHitbox->top;
            otherHitbox->top    = -otherHitbox->bottom;
            otherHitbox->bottom = store;
            break;
        case FLIP_XY:
            store               = -otherHitbox->left;
            otherHitbox->left   = -otherHitbox->right;
            store2              = otherHitbox->bottom;
            otherHitbox->right  = store;
            store3              = -otherHitbox->top;
            otherHitbox->top    = -store2;
            otherHitbox->bottom = store3;
            break;
    }

    int thisIX  = (thisEntity->position.x >> 16);
    int thisIY  = (thisEntity->position.y >> 16);
    int otherIX = (otherEntity->position.x >> 16);
    int otherIY = (otherEntity->position.y >> 16);

    if (otherHitbox->bottom + otherIY < thisIY + thisHitbox->top
        || otherHitbox->bottom + ((otherEntity->position.y - otherEntity->velocity.y) >> 16) > thisIY + thisHitbox->bottom
        || (thisIX + thisHitbox->left >= otherIX + otherHitbox->right) || thisIX + thisHitbox->right <= otherIX + otherHitbox->left
        || otherEntity->velocity.y < 0) {
        status = false;
    }
    else {
        otherEntity->position.y = thisEntity->position.y + ((thisHitbox->top - otherHitbox->bottom) << 16);
        if (setValues) {
            otherEntity->velocity.y = 0;
            if (!otherEntity->onGround) {
                otherEntity->groundVel = otherEntity->velocity.x;
                otherEntity->angle     = 0;
                otherEntity->onGround  = true;
            }
        }
        status = true;
    }

    switch (thisEntity->direction) {
        case FLIP_X:
            store             = -thisHitbox->left;
            thisHitbox->left  = -thisHitbox->right;
            thisHitbox->right = store;
            break;
        case FLIP_Y:
            store              = -thisHitbox->top;
            thisHitbox->top    = -thisHitbox->bottom;
            thisHitbox->bottom = store;
            break;
        case FLIP_XY:
            store              = -thisHitbox->left;
            thisHitbox->left   = -thisHitbox->right;
            store2             = thisHitbox->bottom;
            thisHitbox->right  = store;
            store3             = -thisHitbox->top;
            thisHitbox->top    = -store2;
            thisHitbox->bottom = store3;
            break;
    }
    switch (thisEntity->direction) {
        case FLIP_X:
            store              = -otherHitbox->left;
            otherHitbox->left  = -otherHitbox->right;
            otherHitbox->right = store;
            break;
        case FLIP_Y:
            store2              = otherHitbox->top;
            otherHitbox->top    = -otherHitbox->bottom;
            otherHitbox->bottom = -store2;
            break;
        case FLIP_XY:
            store               = otherHitbox->left;
            otherHitbox->left   = -otherHitbox->right;
            store2              = otherHitbox->bottom;
            otherHitbox->right  = -store;
            store3              = otherHitbox->top;
            otherHitbox->top    = -store2;
            otherHitbox->bottom = -store3;
            break;
    }
    return status;
}

bool32 ObjectTileCollision(Entity *entity, ushort cLayers, char cMode, char cPlane, int xOffset, int yOffset, bool32 setPos)
{
    int layerID     = 1;
    bool32 collided = 0;
    int posX        = (xOffset + entity->position.x) >> 16;
    int posY        = (yOffset + entity->position.y) >> 16;
    switch (cMode) {
        default: return false;
        case CMODE_FLOOR: {
            int solid = 1 << 12;
            if (cPlane)
                solid = 1 << 14;
            for (int l = 0; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cy           = (colY & 0xFFFFFFF0) - 16;
                    if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                        ushort *layout = &layer->layout[(colX / TILE_SIZE) + ((cy / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int ty = cy + collisionMasks[cPlane][tile & 0xFFF].floorMasks[colX & 0xF];
                                    if (colY >= ty && abs(colY - ty) <= 14) {
                                        collided = true;
                                        colY     = ty;
                                        i        = 3;
                                    }
                                }
                            }
                            layout += layer->width;
                            cy += 16;
                        }
                    }
                    posX = layer->position.x + colX;
                    posY = layer->position.y + colY;
                }
            }

            if (setPos && collided)
                entity->position.y = (posY << 16) - yOffset;
            return collided;
        }
        case CMODE_LWALL: {
            int solid = 1 << 13;
            if (cPlane)
                solid = 1 << 15;
            for (int l = 0; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cx           = (colX & 0xFFFFFFF0) - 16;
                    if (colY >= 0 && colY < TILE_SIZE * layer->height) {
                        ushort *layout = &layer->layout[(cx >> 4) + ((colY / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cx >= 0 && cx < TILE_SIZE * layer->width) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int tx = cx + collisionMasks[cPlane][tile & 0xFFF].lWallMasks[colY & 0xF];
                                    if (colX >= tx && abs(colX - tx) <= 14) {
                                        collided = true;
                                        colX     = tx;
                                        i        = 3;
                                    }
                                }
                            }
                            layout++;
                            cx += 16;
                        }
                    }
                    posX = layer->position.x + colX;
                    posY = layer->position.y + colY;
                }
            }

            if (setPos && collided)
                entity->position.x = (posX << 16) - xOffset;
            return collided;
        }
        case CMODE_ROOF: {
            int solid = 1 << 13;
            if (cPlane)
                solid = 1 << 15;
            for (int l = 0; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cy           = (colY & 0xFFFFFFF0) + 16;
                    if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                        ushort *layout = &layer->layout[(colX >> 4) + ((cy / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int ty = cy + collisionMasks[cPlane][tile & 0xFFF].roofMasks[colX & 0xF];
                                    if (colY <= ty && abs(colY - ty) <= 14) {
                                        collided = true;
                                        colY     = ty;
                                        i        = 3;
                                    }
                                }
                            }
                            layout -= layer->width;
                            cy -= 16;
                        }
                    }
                    posX = layer->position.x + colX;
                    posY = layer->position.y + colY;
                }
            }

            if (setPos && collided)
                entity->position.y = (posY << 16) - yOffset;
            return collided;
        }
        case CMODE_RWALL: {
            int solid = 1 << 13;
            if (cPlane)
                solid = 1 << 15;
            for (int l = 0; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cx           = (colX & 0xFFFFFFF0) + 16;
                    if (colY >= 0 && colY < TILE_SIZE * layer->height) {
                        ushort *layout = &layer->layout[(cx >> 4) + ((colY / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cx >= 0 && cx < TILE_SIZE * layer->width) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int tx = cx + collisionMasks[cPlane][tile & 0xFFF].rWallMasks[colY & 0xF];
                                    if (colX <= tx && abs(colX - tx) <= 14) {
                                        collided = true;
                                        colX     = tx;
                                        i        = 3;
                                    }
                                }
                            }
                            layout--;
                            cx -= 16;
                        }
                    }
                    posX = layer->position.x + colX;
                    posY = layer->position.y + colY;
                }
            }

            if (setPos && collided)
                entity->position.x = (posX << 16) - xOffset;
            return collided;
        }
    }
}
bool32 ObjectTileGrip(Entity *entity, ushort cLayers, char cMode, char cPlane, int xOffset, int yOffset, int tolerance)
{
    int layerID     = 1;
    bool32 collided = false;
    int posX        = (xOffset + entity->position.x) >> 16;
    int posY        = (yOffset + entity->position.y) >> 16;
    switch (cMode) {
        default: return false;
        case CMODE_FLOOR: {
            int solid = 1 << 12;
            if (cPlane)
                solid = 1 << 14;
            for (int l = 0; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cy           = (colY & 0xFFFFFFF0) - 16;
                    if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                        ushort *layout = &layer->layout[(colX / TILE_SIZE) + ((cy / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int mask = collisionMasks[cPlane][tile & 0xFFF].floorMasks[colX & 0xF];
                                    int ty   = cy + mask;
                                    if (mask < 0xFF) {
                                        if (abs(colY - ty) <= tolerance) {
                                            collided = true;
                                            colY     = ty;
                                        }
                                        i = 3;
                                    }
                                }
                            }
                            layout += layer->width;
                            cy += 16;
                        }
                    }
                    posX = layer->position.x + colX;
                    posY = layer->position.y + colY;
                }
            }

            if (collided)
                entity->position.y = (posY << 16) - yOffset;
            return collided;
        }
        case CMODE_LWALL: {
            int solid = 0x2000;
            if (cPlane)
                solid = 0x8000;
            for (int l = 0; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cx           = (colX & 0xFFFFFFF0) - 16;
                    if (colY >= 0 && colY < TILE_SIZE * layer->height) {
                        ushort *layout = &layer->layout[(cx >> 4) + ((colY / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cx >= 0 && cx < TILE_SIZE * layer->width) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int mask = collisionMasks[cPlane][tile & 0xFFF].lWallMasks[colY & 0xF];
                                    int tx   = cx + mask;
                                    if (mask < 0xFF) {
                                        if (abs(colX - tx) <= tolerance) {
                                            collided = true;
                                            colX     = tx;
                                        }
                                        i = 3;
                                    }
                                }
                            }
                            layout++;
                            cx += 16;
                        }
                    }
                    posX = layer->position.x + colX;
                    posY = layer->position.y + colY;
                }
            }

            if (collided)
                entity->position.x = (posX << 16) - xOffset;
            return collided;
        }
        case CMODE_ROOF: {
            int solid = 0x1000;
            if (cPlane)
                solid = 0x4000;
            for (int l = 0; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cy           = (colY & 0xFFFFFFF0) + 16;
                    if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                        ushort *layout = &layer->layout[(colX >> 4) + ((cy / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int mask = collisionMasks[cPlane][tile & 0xFFF].roofMasks[colX & 0xF];
                                    int ty = cy + mask;
                                    if (mask < 0xFF) {
                                        if (colY <= ty && abs(colY - ty) <= tolerance) {
                                            collided = true;
                                            colY     = ty;
                                        }
                                        i = 3;
                                    }
                                }
                            }
                            layout -= layer->width;
                            cy -= 16;
                        }
                    }
                    posX = layer->position.x + colX;
                    posY = layer->position.y + colY;
                }
            }

            if (collided)
                entity->position.y = (posY << 16) - yOffset;
            return collided;
        }
        case CMODE_RWALL: {
            int solid = 0x2000;
            if (cPlane)
                solid = 0x8000;
            for (int l = 0; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cx           = (colX & 0xFFFFFFF0) + 16;
                    if (colY >= 0 && colY < TILE_SIZE * layer->height) {
                        ushort *layout = &layer->layout[(cx >> 4) + ((colY / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cx >= 0 && cx < TILE_SIZE * layer->width) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int mask = collisionMasks[cPlane][tile & 0xFFF].rWallMasks[colY & 0xF];
                                    int tx = cx + mask;
                                    if (mask < 0xFF) {
                                        if (colX >= tx && abs(colX - tx) <= tolerance) {
                                            collided = true;
                                            colX     = tx;
                                        }
                                        i = 3;
                                    }
                                }
                            }
                            layout--;
                            cx -= 16;
                        }
                    }
                    posX = layer->position.x + colX;
                    posY = layer->position.y + colY;
                }
            }

            if (collided)
                entity->position.x = (posX << 16) - xOffset;
            return collided;
        }
    }
}

void ProcessTileCollisions(Entity *entity, Hitbox *outerBox, Hitbox *innerBox)
{
    if (entity && outerBox && innerBox) {
        if (entity->tileCollisions) {
            entity->angle &= 0xFF;
            int vel = abs(entity->groundVel);
            if (vel < 0x60000)
                collisionTolerance = entity->angle == 0 ? 8 : 15;

            collisionLeft_Outer   = outerBox->left;
            collisionTop_Outer    = outerBox->top;
            collisionRight_Outer  = outerBox->right;
            collisionBottom_Outer = outerBox->bottom;

            collisionLeft_Inner   = innerBox->left;
            collisionTop_Inner    = innerBox->top;
            collisionRight_Inner  = innerBox->right;
            collisionBottom_Inner = innerBox->bottom;

            collisionEntity = entity;
            if (collisionBottom_Outer >= 14) {
                collisionOffset  = 0x40000;
                collisionMaskAir = 19;
            }
            else {
                collisionOffset    = 0;
                collisionTolerance = 15;
                collisionMaskAir   = 17;
            }

            if (entity->onGround)
                ProcessPathGrip();
            else
                ProcessAirCollision();

            if (entity->onGround) {
                entity->velocity.x = entity->groundVel * cosVal256[entity->angle & 0xFF] >> 8;
                entity->velocity.y = entity->groundVel * cosVal256[entity->angle & 0xFF] >> 8;
            }
            else {
                entity->groundVel = entity->velocity.x;
            }
        }
        else {
            entity->position.x += entity->velocity.x;
            entity->position.y += entity->velocity.y;
        }
    }
}

void ProcessAirCollision()
{
    byte movingDown  = 0;
    byte movingUp    = 0;
    byte movingLeft  = 0;
    byte movingRight = 0;

    if (collisionEntity->velocity.x >= 0) {
        movingRight         = 1;
        sensors[0].pos.y    = collisionEntity->position.y + collisionOffset;
        sensors[0].pos.x    = collisionEntity->position.x + (collisionRight_Outer << 16);
    }

    if (collisionEntity->velocity.x <= 0) {
        movingLeft          = 1;
        sensors[1].pos.y    = collisionEntity->position.y + collisionOffset;
        sensors[1].pos.x    = collisionEntity->position.x + ((collisionLeft_Outer - 1) << 16);
    }
    sensors[2].pos.x    = collisionEntity->position.x + ((collisionRight_Outer - 2) << 16);
    sensors[3].pos.x    = collisionEntity->position.x + ((collisionLeft_Outer + 1) << 16);
    sensors[4].pos.x    = sensors[2].pos.x;
    sensors[5].pos.x    = sensors[3].pos.x;

    sensors[0].collided = false;
    sensors[1].collided = false;
    sensors[2].collided = false;
    sensors[3].collided = false;
    sensors[4].collided = false;
    sensors[5].collided = false;
    if (collisionEntity->velocity.y >= 0) {
        movingDown       = 1;
        sensors[2].pos.y = collisionEntity->position.y + (collisionBottom_Outer << 16);
        sensors[3].pos.y = collisionEntity->position.y + (collisionBottom_Outer << 16);
    }

    if (abs(collisionEntity->velocity.x) > 0x10000 || collisionEntity->velocity.y < 0) {
        movingUp         = 1;
        sensors[4].pos.y = collisionEntity->position.y + ((collisionTop_Outer - 1) << 16);
        sensors[5].pos.y = collisionEntity->position.y + ((collisionTop_Outer - 1) << 16);
    }

    int cnt   = (abs(collisionEntity->velocity.x) <= abs(collisionEntity->velocity.y) ? ((abs(collisionEntity->velocity.y) >> collisionMaskAir) + 1)
                                                                                    : (abs(collisionEntity->velocity.x) >> collisionMaskAir) + 1);
    int XVel  = collisionEntity->velocity.x / cnt;
    int YVel  = collisionEntity->velocity.y / cnt;
    int XVel2 = collisionEntity->velocity.x - XVel * (cnt - 1);
    int YVel2 = collisionEntity->velocity.y - YVel * (cnt - 1);
    while (cnt > 0) {
        if (cnt < 2) {
            XVel = XVel2;
            YVel = YVel2;
        }
        cnt--;

        if (movingRight == 1) {
            sensors[0].pos.x += XVel;
            sensors[0].pos.y += YVel;
            LWallCollision(&sensors[0]);
            if (sensors[0].collided) {
                movingRight = 2;
            }
            else if (collisionEntity->velocity.x < 0x20000 && collisionOffset > 0) {
                sensors[0].pos.y -= collisionOffset << 1;
                LWallCollision(&sensors[0]);
                movingRight = 1;
                if (sensors[0].collided)
                    movingRight = 2;
                sensors[0].pos.y += collisionOffset << 1;
            }
        }

        if (movingLeft == 1) {
            sensors[1].pos.x += XVel;
            sensors[1].pos.y += YVel;
            RWallCollision(&sensors[1]);
            if (sensors[1].collided) {
                movingLeft = 2;
            }
            else if (collisionEntity->velocity.x > -0x20000 && collisionOffset > 0) {
                sensors[1].pos.y -= collisionOffset << 1;
                RWallCollision(&sensors[1]);
                movingLeft = 1;
                if (sensors[1].collided)
                    movingLeft = 2;
                sensors[1].pos.y += collisionOffset << 1;
            }
        }

        if (movingRight == 2) {
            collisionEntity->velocity.x = 0;
            collisionEntity->groundVel  = 0;
            collisionEntity->position.x = sensors[0].pos.x - (collisionRight_Outer << 16);
            sensors[2].pos.x            = collisionEntity->position.x + ((collisionLeft_Outer + 1) << 16);
            sensors[3].pos.x            = collisionEntity->position.x + ((collisionRight_Outer - 2) << 16);
            sensors[4].pos.x            = sensors[2].pos.x;
            sensors[5].pos.x            = sensors[3].pos.x;
            XVel                        = 0;
            XVel2                       = 0;
            movingRight                 = 3;
        }

        if (movingLeft == 2) {
            collisionEntity->velocity.x = 0;
            collisionEntity->groundVel  = 0;
            collisionEntity->position.x = sensors[1].pos.x - ((collisionLeft_Outer + 1) << 16);
            sensors[2].pos.x            = collisionEntity->position.x + ((collisionLeft_Outer + 1) << 16);
            sensors[3].pos.x            = collisionEntity->position.x + ((collisionRight_Outer - 2) << 16);
            sensors[4].pos.x            = sensors[2].pos.x;
            sensors[5].pos.x            = sensors[3].pos.x;
            XVel                        = 0;
            XVel2                       = 0;
            movingLeft                  = 3;
        }

        if (movingDown == 1) {
            for (int i = 2; i < 4; i++) {
                if (!sensors[i].collided) {
                    sensors[i].pos.x += XVel;
                    sensors[i].pos.y += YVel;
                    FloorCollision(&sensors[i]);
                }
            }
            if (sensors[2].collided || sensors[3].collided) {
                movingDown = 2;
                cnt        = 0;
            }
        }

        if (movingUp == 1) {
            for (int i = 4; i < 6; i++) {
                if (!sensors[i].collided) {
                    sensors[i].pos.x += XVel;
                    sensors[i].pos.y += YVel;
                    RoofCollision(&sensors[i]);
                }
            }
            if (sensors[4].collided || sensors[5].collided) {
                movingUp = 2;
                cnt      = 0;
            }
        }
    }

    if (movingRight < 2 && movingLeft < 2)
        collisionEntity->position.x += collisionEntity->velocity.x;

    if (movingUp < 2 && movingDown < 2) {
        collisionEntity->position.y += collisionEntity->velocity.y;
        return;
    }

    if (movingDown == 2) {
        collisionEntity->onGround = true;
        if (sensors[2].collided && sensors[3].collided) {
            if (sensors[2].pos.y >= sensors[3].pos.y) {
                collisionEntity->position.y = sensors[3].pos.y - (collisionBottom_Outer << 16);
                collisionEntity->angle      = sensors[3].angle;
            }
            else {
                collisionEntity->position.y = sensors[2].pos.y - (collisionBottom_Outer << 16);
                collisionEntity->angle      = sensors[2].angle;
            }
        }
        else if (sensors[2].collided) {
            collisionEntity->position.y = sensors[2].pos.y - (collisionBottom_Outer << 16);
            collisionEntity->angle      = sensors[2].angle;
        }
        else if (sensors[3].collided) {
            collisionEntity->position.y = sensors[3].pos.y - (collisionBottom_Outer << 16);
            collisionEntity->angle      = sensors[3].angle;
        }

        if (collisionEntity->angle > 0xA0 && collisionEntity->angle < 0xE0 && collisionEntity->collisionMode != CMODE_LWALL) {
            collisionEntity->collisionMode = CMODE_LWALL;
            collisionEntity->position.x -= 0x40000;
        }
        if (collisionEntity->angle > 0x20 && collisionEntity->angle < 0x60 && collisionEntity->collisionMode != CMODE_RWALL) {
            collisionEntity->collisionMode = CMODE_RWALL;
            collisionEntity->position.x += 0x40000;
        }

        int speed = 0;
        if (collisionEntity->angle < 0x80) {
            if (collisionEntity->angle < 0x10) {
                speed = collisionEntity->velocity.x;
            }
            else if (collisionEntity->angle >= 0x20) {
                speed = (abs(collisionEntity->velocity.x) <= abs(collisionEntity->velocity.y) ? collisionEntity->velocity.y
                                                                                              : collisionEntity->velocity.x);
            }
            else {
                speed = (abs(collisionEntity->velocity.x) <= abs(collisionEntity->velocity.y >> 1) ? (collisionEntity->velocity.y >> 1)
                                                                                                   : collisionEntity->velocity.x);
            }
        }
        else if (collisionEntity->angle > 0xF0) {
            speed = collisionEntity->velocity.x;
        }
        else if (collisionEntity->angle <= 0xE0) {
            speed =
                (abs(collisionEntity->velocity.x) <= abs(collisionEntity->velocity.y) ? -collisionEntity->velocity.y : collisionEntity->velocity.x);
        }
        else {
            speed = (abs(collisionEntity->velocity.x) <= abs(collisionEntity->velocity.y >> 1) ? -(collisionEntity->velocity.y >> 1)
                                                                                               : collisionEntity->velocity.x);
        }

        if (speed < -0x180000)
            speed = -0x180000;
        if (speed > 0x180000)
            speed = 0x180000;
        collisionEntity->groundVel  = speed;
        collisionEntity->velocity.x = speed;
        collisionEntity->velocity.y = 0;
    }

    if (movingUp == 2) {
        int sensorAngle = 0;
        if (sensors[4].collided && sensors[5].collided) {
            if (sensors[4].pos.y <= sensors[5].pos.y) {
                collisionEntity->position.y = sensors[5].pos.y - ((collisionTop_Outer + 1) << 16);
                sensorAngle                 = sensors[5].angle;
            }
            else {
                collisionEntity->position.y = sensors[4].pos.y - ((collisionTop_Outer + 1) << 16);
                sensorAngle                 = sensors[4].angle;
            }
        }
        else if (sensors[4].collided) {
            collisionEntity->position.y = sensors[4].pos.y - (collisionTop_Outer << 16);
            sensorAngle                 = sensors[4].angle;
        }
        else if (sensors[5].collided) {
            collisionEntity->position.y = sensors[5].pos.y - (collisionTop_Outer << 16);
            sensorAngle                 = sensors[5].angle;
        }
        sensorAngle &= 0xFF;

        if (sensorAngle > 0x40 && sensorAngle < 0x62) {
            collisionEntity->onGround      = true;
            collisionEntity->angle         = sensorAngle;
            collisionEntity->collisionMode = CMODE_RWALL;
            collisionEntity->position.x += 0x40000;
            collisionEntity->position.y -= 0x20000;
            if (collisionEntity->angle <= 0x60)
                collisionEntity->groundVel = collisionEntity->velocity.y;
            else
                collisionEntity->groundVel = collisionEntity->velocity.y >> 1;
        }
        if (sensorAngle > 0x9E && sensorAngle < 0xC1) {
            collisionEntity->onGround      = true;
            collisionEntity->angle         = sensorAngle;
            collisionEntity->collisionMode = CMODE_LWALL;
            collisionEntity->position.x -= 0x40000;
            collisionEntity->position.y -= 0x20000;
            if (collisionEntity->angle >= 0xA0)
                collisionEntity->groundVel = -collisionEntity->velocity.y;
            else
                collisionEntity->groundVel = -collisionEntity->velocity.y >> 1;
        }
        if (collisionEntity->velocity.y < 0)
            collisionEntity->velocity.y = 0;
    }
}
void ProcessPathGrip()
{
    int cosValue256;
    int sinValue256;
    sensors[4].pos.x = collisionEntity->position.x;
    sensors[4].pos.y = collisionEntity->position.y;
    for (int i = 0; i < 6; ++i) {
        sensors[i].angle    = collisionEntity->angle;
        sensors[i].collided = false;
    }
    SetPathGripSensors(sensors);
    int absSpeed  = abs(collisionEntity->groundVel);
    int checkDist = absSpeed >> 18;
    absSpeed &= 0x3FFFF;
    while (checkDist > -1) {
        if (checkDist >= 1) {
            cosValue256 = cosVal256[collisionEntity->angle] << 10;
            sinValue256 = sinVal256[collisionEntity->angle] << 10;
            checkDist--;
        }
        else {
            cosValue256 = absSpeed * cosVal256[collisionEntity->angle] >> 8;
            sinValue256 = absSpeed * sinVal256[collisionEntity->angle] >> 8;
            checkDist   = -1;
        }

        if (collisionEntity->groundVel < 0) {
            cosValue256 = -cosValue256;
            sinValue256 = -sinValue256;
        }

        sensors[0].collided = false;
        sensors[1].collided = false;
        sensors[2].collided = false;
        sensors[5].collided = false;
        sensors[4].pos.x += cosValue256;
        sensors[4].pos.y += sinValue256;
        int tileDistance = -1;

        switch (collisionEntity->collisionMode) {
            case CMODE_FLOOR: {
                sensors[3].pos.x += cosValue256;
                sensors[3].pos.y += sinValue256;

                if (collisionEntity->groundVel > 0) {
                    LWallCollision(&sensors[3]);
                    if (sensors[3].collided) {
                        sensors[2].pos.x = sensors[3].pos.x - (2 << 16);
                    }
                }

                if (collisionEntity->groundVel < 0) {
                    RWallCollision(&sensors[3]);
                    if (sensors[3].collided) {
                        sensors[0].pos.x = sensors[3].pos.x + (2 << 16);
                    }
                }

                if (sensors[3].collided) {
                    cosValue256 = 0;
                    checkDist   = -1;
                }

                for (int i = 0; i < 3; i++) {
                    sensors[i].pos.x += cosValue256;
                    sensors[i].pos.y += sinValue256;
                    FindFloorPosition(&sensors[i]);
                }

                tileDistance = -1;
                for (int i = 0; i < 3; i++) {
                    if (tileDistance > -1) {
                        if (sensors[i].collided) {
                            if (sensors[i].pos.y < sensors[tileDistance].pos.y)
                                tileDistance = i;

                            if (sensors[i].pos.y == sensors[tileDistance].pos.y && (sensors[i].angle < 0x08 || sensors[i].angle > 0xF8))
                                tileDistance = i;
                        }
                    }
                    else if (sensors[i].collided)
                        tileDistance = i;
                }

                if (tileDistance <= -1) {
                    checkDist = -1;
                }
                else {
                    sensors[0].pos.y = sensors[tileDistance].pos.y;
                    sensors[0].angle = sensors[tileDistance].angle;

                    sensors[1].pos.y = sensors[0].pos.y;
                    sensors[1].angle = sensors[0].angle;

                    sensors[2].pos.y = sensors[0].pos.y;
                    sensors[2].angle = sensors[0].angle;

                    sensors[4].pos.x = sensors[1].pos.x;
                    sensors[4].pos.y = sensors[0].pos.y - (collisionBottom_Outer << 16);
                }

                if (sensors[0].angle < 0xDE && sensors[0].angle > 0x80)
                    collisionEntity->collisionMode = CMODE_LWALL;
                if (sensors[0].angle > 0x22 && sensors[0].angle < 0x80)
                    collisionEntity->collisionMode = CMODE_RWALL;
                break;
            }
            case CMODE_LWALL: {
                sensors[3].pos.x += cosValue256;
                sensors[3].pos.y += sinValue256;

                if (collisionEntity->groundVel > 0)
                    RoofCollision(&sensors[3]);

                if (collisionEntity->groundVel < 0)
                    FloorCollision(&sensors[3]);

                if (sensors[3].collided) {
                    sinValue256 = 0;
                    checkDist   = -1;
                }
                for (int i = 0; i < 3; i++) {
                    sensors[i].pos.x += cosValue256;
                    sensors[i].pos.y += sinValue256;
                    FindLWallPosition(&sensors[i]);
                }

                tileDistance = -1;
                for (int i = 0; i < 3; i++) {
                    if (tileDistance > -1) {
                        if (sensors[i].pos.x < sensors[tileDistance].pos.x && sensors[i].collided) {
                            tileDistance = i;
                        }
                    }
                    else if (sensors[i].collided) {
                        tileDistance = i;
                    }
                }

                if (tileDistance <= -1) {
                    checkDist = -1;
                }
                else {
                    sensors[0].pos.x = sensors[tileDistance].pos.x;
                    sensors[0].angle = sensors[tileDistance].angle;

                    sensors[1].pos.x = sensors[0].pos.x;
                    sensors[1].angle = sensors[0].angle;

                    sensors[2].pos.x = sensors[0].pos.x;
                    sensors[2].angle = sensors[0].angle;

                    sensors[4].pos.y = sensors[1].pos.y;
                    sensors[4].pos.x = sensors[1].pos.x - (collisionBottom_Outer << 16);
                }

                if (sensors[0].angle > 0xE2)
                    collisionEntity->collisionMode = CMODE_FLOOR;
                if (sensors[0].angle < 0x9E)
                    collisionEntity->collisionMode = CMODE_ROOF;
                break;
            }
            case CMODE_ROOF: {
                sensors[3].pos.x += cosValue256;
                sensors[3].pos.y += sinValue256;

                if (collisionEntity->groundVel > 0)
                    RWallCollision(&sensors[3]);

                if (collisionEntity->groundVel < 0)
                    LWallCollision(&sensors[3]);

                if (sensors[3].collided) {
                    cosValue256 = 0;
                    checkDist   = -1;
                }
                for (int i = 0; i < 3; i++) {
                    sensors[i].pos.x += cosValue256;
                    sensors[i].pos.y += sinValue256;
                    FindRoofPosition(&sensors[i]);
                }

                tileDistance = -1;
                for (int i = 0; i < 3; i++) {
                    if (tileDistance > -1) {
                        if (sensors[i].pos.y > sensors[tileDistance].pos.y && sensors[i].collided) {
                            tileDistance = i;
                        }
                    }
                    else if (sensors[i].collided) {
                        tileDistance = i;
                    }
                }

                if (tileDistance <= -1) {
                    checkDist = -1;
                }
                else {
                    sensors[0].pos.y = sensors[tileDistance].pos.y;
                    sensors[0].angle = sensors[tileDistance].angle;

                    sensors[1].pos.y = sensors[0].pos.y;
                    sensors[1].angle = sensors[0].angle;

                    sensors[2].pos.y = sensors[0].pos.y;
                    sensors[2].angle = sensors[0].angle;

                    sensors[4].pos.x = sensors[1].pos.x;
                    sensors[4].pos.y = sensors[0].pos.y - ((collisionBottom_Outer + 1) << 16);
                }

                if (sensors[0].angle > 0xA2)
                    collisionEntity->collisionMode = CMODE_LWALL;
                if (sensors[0].angle < 0x5E)
                    collisionEntity->collisionMode = CMODE_RWALL;
                break;
            }
            case CMODE_RWALL: {
                sensors[3].pos.x += cosValue256;
                sensors[3].pos.y += sinValue256;

                if (collisionEntity->groundVel > 0)
                    FloorCollision(&sensors[3]);

                if (collisionEntity->groundVel < 0)
                    RoofCollision(&sensors[3]);

                if (sensors[3].collided) {
                    sinValue256 = 0;
                    checkDist   = -1;
                }
                for (int i = 0; i < 3; i++) {
                    sensors[i].pos.x += cosValue256;
                    sensors[i].pos.y += sinValue256;
                    FindRWallPosition(&sensors[i]);
                }

                tileDistance = -1;
                for (int i = 0; i < 3; i++) {
                    if (tileDistance > -1) {
                        if (sensors[i].pos.x > sensors[tileDistance].pos.x && sensors[i].collided) {
                            tileDistance = i;
                        }
                    }
                    else if (sensors[i].collided) {
                        tileDistance = i;
                    }
                }

                if (tileDistance <= -1) {
                    checkDist = -1;
                }
                else {
                    sensors[0].pos.x = sensors[tileDistance].pos.x;
                    sensors[0].angle = sensors[tileDistance].angle;

                    sensors[1].pos.x = sensors[0].pos.x;
                    sensors[1].angle = sensors[0].angle;

                    sensors[2].pos.x = sensors[0].pos.x;
                    sensors[2].angle = sensors[0].angle;

                    sensors[4].pos.y = sensors[1].pos.y;
                    sensors[4].pos.x = sensors[1].pos.x - ((collisionBottom_Outer + 1) << 16);
                }

                if (sensors[0].angle < 0x1E)
                    collisionEntity->collisionMode = CMODE_FLOOR;
                if (sensors[0].angle > 0x62)
                    collisionEntity->collisionMode = CMODE_ROOF;
                break;
            }
        }
        if (tileDistance != -1)
            collisionEntity->angle = sensors[0].angle;

        if (!sensors[3].collided)
            SetPathGripSensors(sensors);
        else
            checkDist = -2;
    }

    switch (collisionEntity->collisionMode) {
        case CMODE_FLOOR: {
            if (sensors[0].collided || sensors[1].collided || sensors[2].collided) {
                collisionEntity->angle = sensors[0].angle;
                if (!sensors[3].collided) {
                    collisionEntity->position.x = sensors[4].pos.x;
                }
                else {
                    if (collisionEntity->groundVel > 0)
                        collisionEntity->position.x = sensors[3].pos.x - (collisionRight_Outer << 16);

                    if (collisionEntity->groundVel < 0)
                        collisionEntity->position.x = sensors[3].pos.x - ((collisionLeft_Outer + 1) << 16);

                    collisionEntity->groundVel  = 0;
                    collisionEntity->velocity.x = 0;
                }
                collisionEntity->position.y = sensors[4].pos.y;
            }
            else {
                collisionEntity->onGround      = false;
                collisionEntity->collisionMode = CMODE_FLOOR;
                collisionEntity->velocity.x    = cosVal256[collisionEntity->angle] * collisionEntity->groundVel >> 8;
                collisionEntity->velocity.y    = sinVal256[collisionEntity->angle] * collisionEntity->groundVel >> 8;
                if (collisionEntity->velocity.y < -0x100000)
                    collisionEntity->velocity.y = -0x100000;

                if (collisionEntity->velocity.y > 0x100000)
                    collisionEntity->velocity.y = 0x100000;

                collisionEntity->groundVel = collisionEntity->velocity.x;
                collisionEntity->angle     = 0;
                if (!sensors[3].collided) {
                    collisionEntity->position.x += collisionEntity->velocity.x;
                }
                else {
                    if (collisionEntity->groundVel > 0)
                        collisionEntity->position.x = sensors[3].pos.x - (collisionRight_Outer << 16);
                    if (collisionEntity->groundVel < 0)
                        collisionEntity->position.x = sensors[3].pos.x - ((collisionLeft_Outer + 1) << 16);

                    collisionEntity->groundVel  = 0;
                    collisionEntity->velocity.x = 0;
                }
                collisionEntity->position.y += collisionEntity->velocity.y;
            }
            break;
        }
        case CMODE_LWALL: {
            if (!sensors[0].collided && !sensors[1].collided && !sensors[2].collided) {
                collisionEntity->onGround      = false;
                collisionEntity->collisionMode = CMODE_FLOOR;
                collisionEntity->velocity.x    = cosVal256[collisionEntity->angle] * collisionEntity->groundVel >> 8;
                collisionEntity->velocity.y    = sinVal256[collisionEntity->angle] * collisionEntity->groundVel >> 8;
                if (collisionEntity->velocity.y < -0x100000) {
                    collisionEntity->velocity.y = -0x100000;
                }
                if (collisionEntity->velocity.y > 0x100000) {
                    collisionEntity->velocity.y = 0x100000;
                }
                collisionEntity->groundVel = collisionEntity->velocity.x;
                collisionEntity->angle     = 0;
            }
            else {
                collisionEntity->angle = 0;
            }

            if (!sensors[3].collided) {
                collisionEntity->position.y = sensors[4].pos.y;
            }
            else {
                if (collisionEntity->groundVel > 0)
                    collisionEntity->position.y = sensors[3].pos.y - ((collisionRight_Outer + 1) << 16);

                if (collisionEntity->groundVel < 0)
                    collisionEntity->position.y = sensors[3].pos.y - (collisionLeft_Outer << 16);

                collisionEntity->groundVel = 0;
            }
            collisionEntity->position.x = sensors[4].pos.x;
            break;
        }
        case CMODE_ROOF: {
            if (!sensors[0].collided && !sensors[1].collided && !sensors[2].collided) {
                collisionEntity->onGround      = false;
                collisionEntity->collisionMode = CMODE_FLOOR;
                collisionEntity->velocity.x    = cosVal256[collisionEntity->angle] * collisionEntity->groundVel >> 8;
                collisionEntity->velocity.y    = sinVal256[collisionEntity->angle] * collisionEntity->groundVel >> 8;

                if (collisionEntity->velocity.y < -0x100000)
                    collisionEntity->velocity.y = -0x100000;

                if (collisionEntity->velocity.y > 0x100000)
                    collisionEntity->velocity.y = 0x100000;

                collisionEntity->angle     = 0;
                collisionEntity->groundVel = collisionEntity->velocity.x;
                if (!sensors[3].collided) {
                    collisionEntity->position.x = collisionEntity->position.x + collisionEntity->velocity.x;
                }
                else {
                    if (collisionEntity->groundVel > 0)
                        collisionEntity->position.x = sensors[3].pos.x - (collisionRight_Outer << 16);

                    if (collisionEntity->groundVel < 0)
                        collisionEntity->position.x = sensors[3].pos.x - ((collisionLeft_Outer - 1) << 16);

                    collisionEntity->groundVel = 0;
                }
            }
            else {
                collisionEntity->onGround      = false;
                collisionEntity->angle         = 0;
                collisionEntity->collisionMode = CMODE_FLOOR;
                collisionEntity->groundVel     = collisionEntity->velocity.x;
                if (!sensors[3].collided) {
                    collisionEntity->position.x = collisionEntity->position.x + collisionEntity->velocity.x;
                }
                else {
                    if (collisionEntity->groundVel > 0)
                        collisionEntity->position.x = sensors[3].pos.x - (collisionRight_Outer << 16);

                    if (collisionEntity->groundVel < 0)
                        collisionEntity->position.x = sensors[3].pos.x - ((collisionLeft_Outer + 1) << 16);
                    collisionEntity->groundVel = 0;
                }
            }
            collisionEntity->position.y = sensors[4].pos.y;
            break;
        }
        case CMODE_RWALL: {
            if (!sensors[0].collided && !sensors[1].collided && !sensors[2].collided) {
                collisionEntity->onGround      = false;
                collisionEntity->collisionMode = CMODE_FLOOR;
                collisionEntity->velocity.x    = cosVal256[collisionEntity->angle] * collisionEntity->groundVel >> 8;
                collisionEntity->velocity.y    = sinVal256[collisionEntity->angle] * collisionEntity->groundVel >> 8;
                if (collisionEntity->velocity.y < -0x100000)
                    collisionEntity->velocity.y = -0x100000;

                if (collisionEntity->velocity.y > 0x100000)
                    collisionEntity->velocity.y = 0x100000;

                collisionEntity->groundVel = collisionEntity->velocity.x;
                collisionEntity->angle     = 0;
            }
            else {
                collisionEntity->angle = 0;
            }
            if (!sensors[3].collided) {
                collisionEntity->position.y = sensors[4].pos.y;
            }
            else {
                if (collisionEntity->groundVel > 0)
                    collisionEntity->position.y = sensors[3].pos.y - (collisionRight_Outer << 16);

                if (collisionEntity->groundVel < 0)
                    collisionEntity->position.y = sensors[3].pos.y - ((collisionLeft_Outer + 1) << 16);

                collisionEntity->groundVel = 0;
            }
            collisionEntity->position.x = sensors[4].pos.x;
            break;
        }
        default: break;
    }
}

void SetPathGripSensors(CollisionSensor *cSensors)
{
    switch (collisionEntity->collisionMode) {
        case CMODE_FLOOR:
            cSensors[0].pos.y = cSensors[4].pos.y + (collisionBottom_Outer << 16);
            cSensors[1].pos.y = cSensors[4].pos.y + (collisionBottom_Outer << 16);
            cSensors[2].pos.y = cSensors[4].pos.y + (collisionBottom_Outer << 16);
            cSensors[3].pos.y = cSensors[4].pos.y + collisionOffset;
            cSensors[1].pos.x = cSensors[4].pos.x;
            cSensors[0].pos.x = cSensors[4].pos.x + (collisionLeft_Inner << 16) - 0x10000;
            cSensors[2].pos.x = cSensors[4].pos.x + (collisionRight_Inner << 16);
            if (collisionEntity->groundVel <= 0)
                cSensors[3].pos.x = cSensors[4].pos.x + ((collisionLeft_Outer << 16) - 0x10000);
            else
                cSensors[3].pos.x = cSensors[4].pos.x + (collisionRight_Outer << 16);
            break;
        case CMODE_LWALL:
            cSensors[3].pos.x = cSensors[4].pos.x;
            cSensors[0].pos.x = cSensors[4].pos.x + (collisionBottom_Outer << 16);
            cSensors[1].pos.x = cSensors[4].pos.x + (collisionBottom_Outer << 16);
            cSensors[2].pos.x = cSensors[4].pos.x + (collisionBottom_Outer << 16);
            cSensors[1].pos.y = cSensors[4].pos.y;
            cSensors[0].pos.y = cSensors[4].pos.y + (collisionLeft_Inner << 16) - 0x10000;
            cSensors[2].pos.y = cSensors[4].pos.y + (collisionRight_Inner << 16);
            if (collisionEntity->groundVel <= 0)
                cSensors[3].pos.y = cSensors[4].pos.y - (collisionLeft_Outer << 16);
            else
                cSensors[3].pos.y = cSensors[4].pos.y - (collisionRight_Outer << 16) - 0x10000;
            break;
        case CMODE_ROOF:
            cSensors[3].pos.y = cSensors[4].pos.y - collisionOffset;
            cSensors[0].pos.y = cSensors[4].pos.y - (collisionBottom_Outer << 16) - 0x10000;
            cSensors[1].pos.y = cSensors[4].pos.y - (collisionBottom_Outer << 16) - 0x10000;
            cSensors[2].pos.y = cSensors[4].pos.y - (collisionBottom_Outer << 16) - 0x10000;
            cSensors[1].pos.x = cSensors[4].pos.x;
            cSensors[0].pos.x = cSensors[4].pos.x + (collisionLeft_Inner << 16) - 0x10000;
            cSensors[2].pos.x = cSensors[4].pos.x + (collisionRight_Inner << 16);
            if (collisionEntity->groundVel <= 0)
                cSensors[3].pos.x = cSensors[4].pos.x - (collisionLeft_Outer << 16);
            else
                cSensors[3].pos.x = cSensors[4].pos.x - (collisionRight_Outer << 16) - 0x10000;
            break;
        case CMODE_RWALL:
            cSensors[3].pos.x = cSensors[4].pos.x;
            cSensors[0].pos.x = cSensors[4].pos.x - (collisionBottom_Outer << 16) - 0x10000;
            cSensors[1].pos.x = cSensors[4].pos.x - (collisionBottom_Outer << 16) - 0x10000;
            cSensors[2].pos.x = cSensors[4].pos.x - (collisionBottom_Outer << 16) - 0x10000;
            cSensors[1].pos.y = cSensors[4].pos.y;
            cSensors[0].pos.y = cSensors[4].pos.y + (collisionLeft_Inner << 16) - 0x10000;
            cSensors[2].pos.y = cSensors[4].pos.y + (collisionRight_Inner << 16);
            if (collisionEntity->groundVel <= 0)
                cSensors[3].pos.y = cSensors[4].pos.y + (collisionLeft_Outer << 16) - 0x10000;
            else
                cSensors[3].pos.y = cSensors[4].pos.y + (collisionRight_Outer << 16);
            break;
        default: break;
    }
}

void FindFloorPosition(CollisionSensor *sensor)
{
    int posX  = (sensor->pos.x >> 0x10);
    int posY  = (sensor->pos.y >> 0x10);
    int solid = 1 << 12;
    if (collisionEntity->collisionPlane)
        solid = 1 << 14;
    int startY = posY;

    for (int l = 0, layerID = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
        if (collisionEntity->collisionLayers & layerID) {
            TileLayer *layer = &tileLayers[l];
            int colX         = posX - layer->position.x;
            int colY         = posY - layer->position.y;
            int cy           = (colY & 0xFFFFFFF0) - 16;
            if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                ushort *layout = &layer->layout[(colX >> 4) + ((cy / TILE_SIZE) * layer->width)];
                for (int i = 0; i < 3; ++i) {
                    if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                        ushort tile = *layout;
                        if (tile < 0xFFFF && tile & solid) {
                            int mask      = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].floorMasks[colX & 0xF];
                            int ty        = cy + mask;
                            int tileAngle = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].floorAngle;
                            if (mask < 0xFF) {
                                if (!sensor->collided || startY >= ty) {
                                    if (abs(colY - ty) <= collisionTolerance) {
                                        if (tileAngle < 0)
                                            tileAngle += 0x100;

                                        if (tileAngle >= 0x100)
                                            tileAngle -= 0x100;

                                        if ((abs(sensor->angle - tileAngle) <= 0x20) || (abs(sensor->angle - 0x100 - tileAngle) <= 0x20)
                                            || (abs(sensor->angle + 0x100 - tileAngle) <= 0x20)) {
                                            sensor->collided = true;
                                            sensor->angle    = tileAngle;
                                            sensor->pos.y    = (ty + layer->position.y) << 0x10;
                                            startY           = ty;
                                            i                = 3;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    layout += layer->width;
                    cy += 16;
                }
            }
            posX = layer->position.x + colX;
            posY = layer->position.y + colY;
        }
    }
}
void FindLWallPosition(CollisionSensor *sensor)
{
    int posX  = (sensor->pos.x >> 0x10);
    int posY  = (sensor->pos.y >> 0x10);
    int solid = 1 << 13;
    if (collisionEntity->collisionPlane)
        solid = 1 << 15;
    int startX = posX;

    for (int l = 0, layerID = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
        if (collisionEntity->collisionLayers & layerID) {
            TileLayer *layer = &tileLayers[l];
            int colX         = posX - layer->position.x;
            int colY         = posY - layer->position.y;
            int cx           = (colX & -TILE_SIZE) - TILE_SIZE;
            if (colY >= 0 && colY < TILE_SIZE * layer->height) {
                ushort *layout = &layer->layout[(cx >> 4) + ((colY / TILE_SIZE) * layer->width)];
                for (int i = 0; i < 3; ++i) {
                    if (cx >= 0 && cx < TILE_SIZE * layer->width) {
                        ushort tile = *layout;
                        if (tile < 0xFFFF && tile & solid) {
                            int mask      = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].lWallMasks[colY & 0xF];
                            int tx        = cx + mask;
                            int tileAngle = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].lWallAngle;
                            if (mask < 0xFF) {
                                if (!sensor->collided || startX >= tx) {
                                    if (abs(colX - tx) <= collisionTolerance) {
                                        if (sensor->angle < 0)
                                            sensor->angle += 0x100;

                                        if (sensor->angle >= 0x100)
                                            sensor->angle -= 0x100;

                                        if (abs(sensor->angle - tileAngle) <= 0x20) {
                                            sensor->collided = true;
                                            sensor->angle    = tileAngle;
                                            sensor->pos.x    = (tx + layer->position.x) << 0x10;
                                            startX           = tx;
                                            i                = 3;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    layout++;
                    cx += 16;
                }
            }
            posX = layer->position.x + colX;
            posY = layer->position.y + colY;
        }
    }
}
void FindRoofPosition(CollisionSensor *sensor)
{
    int posX  = (sensor->pos.x >> 0x10);
    int posY  = (sensor->pos.y >> 0x10);
    int solid = 1 << 13;
    if (collisionEntity->collisionPlane)
        solid = 1 << 15;
    int startY = posY;

    for (int l = 0, layerID = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
        if (collisionEntity->collisionLayers & layerID) {
            TileLayer *layer = &tileLayers[l];
            int colX         = posX - layer->position.x;
            int colY         = posY - layer->position.y;
            int cy           = (colY & -TILE_SIZE) + TILE_SIZE;
            if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                ushort *layout = &layer->layout[(colX >> 4) + ((cy / TILE_SIZE) * layer->width)];
                for (int i = 0; i < 3; ++i) {
                    if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                        ushort tile = *layout;
                        if (tile < 0xFFFF && tile & solid) {
                            int mask      = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].roofMasks[colX & 0xF];
                            int ty        = cy + mask;
                            int tileAngle = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].roofAngle;
                            if (mask < 0xFF) {
                                if (!sensor->collided || startY <= ty) {
                                    if (abs(colY - ty) <= collisionTolerance) {
                                        if (tileAngle < 0)
                                            tileAngle += 0x100;

                                        if (tileAngle >= 0x100)
                                            tileAngle -= 0x100;

                                        if (abs(tileAngle - sensor->angle) <= 0x20) {
                                            sensor->collided = true;
                                            sensor->angle    = tileAngle;
                                            sensor->pos.y    = (ty + layer->position.y) << 0x10;
                                            startY           = ty;
                                            i                = 3;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    layout -= layer->width;
                    cy -= 16;
                }
            }
            posX = layer->position.x + colX;
            posY = layer->position.y + colY;
        }
    }
}
void FindRWallPosition(CollisionSensor *sensor)
{
    int posX  = (sensor->pos.x >> 0x10);
    int posY  = (sensor->pos.y >> 0x10);
    int solid = 1 << 13;
    if (collisionEntity->collisionPlane)
        solid = 1 << 15;
    int startX = posX;

    for (int l = 0, layerID = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
        if (collisionEntity->collisionLayers & layerID) {
            TileLayer *layer = &tileLayers[l];
            int colX         = posX - layer->position.x;
            int colY         = posY - layer->position.y;
            int cx           = (colX & -TILE_SIZE) + TILE_SIZE;
            if (colY >= 0 && colY < TILE_SIZE * layer->height) {
                ushort *layout = &layer->layout[(cx >> 4) + ((colY / TILE_SIZE) * layer->width)];
                for (int i = 0; i < 3; ++i) {
                    if (cx >= 0 && cx < TILE_SIZE * layer->width) {
                        ushort tile = *layout;
                        if (tile < 0xFFFF && tile & solid) {
                            int tx        = cx + collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].rWallMasks[colY & 0xF];
                            int tileAngle = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].rWallAngle;
                            if (collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].rWallMasks[colY & 0xF] < 0xFF) {
                                if (!sensor->collided || startX <= tx) {
                                    if (abs(colX - tx) <= collisionTolerance) {
                                        if (tileAngle < 0)
                                            tileAngle += 0x100;

                                        if (tileAngle >= 0x100)
                                            tileAngle -= 0x100;

                                        if (abs(tileAngle - sensor->angle) <= 0x20) {
                                            sensor->collided = true;
                                            sensor->angle    = tileAngle;
                                            sensor->pos.x    = (tx + layer->position.x) << 0x10;
                                            startX           = tx;
                                            i                = 3;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    layout--;
                    cx -= 16;
                }
            }
            posX = layer->position.x + colX;
            posY = layer->position.y + colY;
        }
    }
}

void FloorCollision(CollisionSensor *sensor)
{
    int posX  = (sensor->pos.x >> 0x10);
    int posY  = (sensor->pos.y >> 0x10);
    int solid = 1 << 12;
    if (collisionEntity->collisionPlane)
        solid = 1 << 14;
    for (int l = 0, layerID = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
        if (collisionEntity->collisionLayers & layerID) {
            TileLayer *layer = &tileLayers[l];
            int colX         = posX - layer->position.x;
            int colY         = posY - layer->position.y;
            int cy           = (colY & -TILE_SIZE) - TILE_SIZE;
            if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                ushort *layout = &layer->layout[(colX >> 4) + ((cy / TILE_SIZE) * layer->width)];
                for (int i = 0; i < 3; ++i) {
                    if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                        ushort tile = *layout;
                        if (tile < 0xFFFF && tile & solid) {
                            int mask = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].floorMasks[colX & 0xF];
                            int ty = cy + mask;
                            if (colY >= ty && abs(colY - ty) <= 14 && mask < 0xFF) {
                                sensor->collided = true;
                                sensor->angle    = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].floorAngle;
                                sensor->pos.y    = (ty + layer->position.y) << 0x10;
                            }
                        }
                    }
                    layout += layer->width;
                    cy += 16;
                }
            }
            posX = layer->position.x + colX;
            posY = layer->position.y + colY;
        }
    }
}
void LWallCollision(CollisionSensor *sensor)
{
    int posX  = (sensor->pos.x >> 0x10);
    int posY  = (sensor->pos.y >> 0x10);
    int solid = 1 << 13;
    if (collisionEntity->collisionPlane)
        solid = 1 << 15;
    for (int l = 0, layerID = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
        if (collisionEntity->collisionLayers & layerID) {
            TileLayer *layer = &tileLayers[l];
            int colX         = posX - layer->position.x;
            int colY         = posY - layer->position.y;
            int cx           = (colX & 0xFFFFFFF0) - 16;
            if (colY >= 0 && colY < TILE_SIZE * layer->height) {
                ushort *layout = &layer->layout[(cx >> 4) + ((colY / TILE_SIZE) * layer->width)];
                for (int i = 0; i < 3; ++i) {
                    if (cx >= 0 && cx < TILE_SIZE * layer->width) {
                        ushort tile = *layout;
                        if (tile < 0xFFFF && tile & solid) {
                            int mask = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].lWallMasks[colY & 0xF];
                            int tx = cx + mask;
                            if (colX >= tx && abs(colX - tx) <= 14 && mask < 0xFF) {
                                sensor->collided = true;
                                sensor->angle    = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].lWallAngle;
                                sensor->pos.x    = (tx + layer->position.x) << 0x10;
                            }
                        }
                    }
                    layout++;
                    cx += 16;
                }
            }
            posX = layer->position.x + colX;
            posY = layer->position.y + colY;
        }
    }
}
void RoofCollision(CollisionSensor *sensor)
{
    int posX  = (sensor->pos.x >> 0x10);
    int posY  = (sensor->pos.y >> 0x10);
    int solid = 1 << 13;
    if (collisionEntity->collisionPlane)
        solid = 1 << 15;
    for (int l = 0, layerID = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
        if (collisionEntity->collisionLayers & layerID) {
            TileLayer *layer = &tileLayers[l];
            int colX         = posX - layer->position.x;
            int colY         = posY - layer->position.y;
            int cy           = (colY & -TILE_SIZE) + TILE_SIZE;
            if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                for (int i = 0; i < 3; ++i) {
                    if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                        int tileX       = (colX / TILE_SIZE);
                        int tileY       = (cy / TILE_SIZE);
                        ushort tile     = layer->layout[tileX + (tileY * layer->width)];
                        if (tile < 0xFFFF && tile & solid) {
                            int mask = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].roofMasks[colX & 0xF];
                            int ty   = cy + mask;
                            if (colY < ty && abs(colY - ty) <= 14 && mask < 0xFF) {
                                sensor->collided = true;
                                sensor->angle    = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].roofAngle;
                                sensor->pos.y    = (ty + layer->position.y) << 0x10;
                            }
                        }
                    }
                    cy -= TILE_SIZE;
                }
            }
            posX = layer->position.x + colX;
            posY = layer->position.y + colY;
        }
    }
}
void RWallCollision(CollisionSensor *sensor)
{
    int posX  = (sensor->pos.x >> 0x10);
    int posY  = (sensor->pos.y >> 0x10);
    int solid = 1 << 13;
    if (collisionEntity->collisionPlane)
        solid = 1 << 15;
    for (int l = 0, layerID = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
        if (collisionEntity->collisionLayers & layerID) {
            TileLayer *layer = &tileLayers[l];
            int colX         = posX - layer->position.x;
            int colY         = posY - layer->position.y;
            int cx           = (colX & 0xFFFFFFF0) + 16;
            if (colY >= 0 && colY < TILE_SIZE * layer->height) {
                ushort *layout = &layer->layout[(cx / TILE_SIZE) + ((colY / TILE_SIZE) * layer->width)];
                for (int i = 0; i < 3; ++i) {
                    if (cx >= 0 && cx < TILE_SIZE * layer->width) {
                        ushort tile = *layout;
                        if (tile < 0xFFFF && tile & solid) {
                            int mask = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].rWallMasks[colY & 0xF];
                            int tx = cx + mask;
                            if (colX <= tx && abs(colX - tx) <= 14 && mask < 0xFF) {
                                sensor->collided = true;
                                sensor->angle    = collisionMasks[collisionEntity->collisionPlane][tile & 0xFFF].rWallAngle;
                                sensor->pos.x    = (tx + layer->position.x) << 0x10;
                            }
                        }
                    }
                    layout--;
                    cx -= 16;
                }
            }
            posX = layer->position.x + colX;
            posY = layer->position.y + colY;
        }
    }
}