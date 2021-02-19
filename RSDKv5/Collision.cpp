#include "RetroEngine.hpp"

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
    int store, store2, store3;
    int collisionResult, collisionDir;

    if (thisEntity && otherEntity && thisHitbox && otherHitbox) {
        collisionResult = 0;
        collisionDir    = 0;
        int otherX          = otherEntity->position.x;
        int otherY      = otherEntity->position.y;
        int otherX2      = otherEntity->position.x;
        int otherY2      = otherEntity->position.y;
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
        int thisIX              = thisEntity->position.x >> 0x10;
        int thisIY              = thisEntity->position.y >> 0x10;
        int otherIX             = otherEntity->position.x >> 0x10;
        int otherIY             = otherEntity->position.y >> 0x10;

        otherHitbox->top++;
        otherHitbox->bottom--;

        if (otherIX <= (thisHitbox->right + thisHitbox->left + 2 * thisIX) >> 1) {
            if (otherHitbox->right + otherIX >= thisHitbox->left + thisIX && thisIY + thisHitbox->top < otherIY + otherHitbox->bottom
                && thisIY + thisHitbox->bottom > otherIY + otherHitbox->top) {
                collisionResult = 2;
                otherX          = thisEntity->position.x + ((thisHitbox->left - otherHitbox->right) << 16);
            }
        }
        else if (otherHitbox->left + otherIX < thisHitbox->right + thisIX && thisIY + thisHitbox->top < otherIY + otherHitbox->bottom
                 && thisIY + thisHitbox->bottom > otherIY + otherHitbox->top) {
            collisionResult = 3;
            otherX          = thisEntity->position.x + ((thisHitbox->right - otherHitbox->left) << 16);
        }
        otherHitbox->bottom++;
        otherHitbox->top--;
        otherHitbox->left++;
        otherHitbox->right--;

        if (otherIY >= (thisHitbox->top + thisHitbox->bottom + 2 * thisIY) >> 1) {
            if (otherHitbox->top + otherIY < thisHitbox->bottom + thisIY && thisIX + thisHitbox->left < otherIX + otherHitbox->right) {
                if (otherIX + otherHitbox->left < thisIX + thisHitbox->right) {
                    collisionDir = 4;
                    otherY2       = thisEntity->position.y + ((thisHitbox->bottom - otherHitbox->top) << 16);
                }
            }
        }
        else if (otherHitbox->bottom + otherIY >= thisHitbox->top + thisIY && thisIX + thisHitbox->left < otherIX + otherHitbox->right
                 && thisIX + thisHitbox->right > otherIX + otherHitbox->left) {
            collisionDir = 1;
            otherY2       = thisEntity->position.y + ((thisHitbox->top - otherHitbox->bottom) << 16);
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

        int ox = ((otherX - otherEntity->position.x) >> 16);
        int oy = ((otherY - otherEntity->position.y) >> 16);
        int ox2 = ((otherX2 - otherEntity->position.x) >> 16);
        int oy2 = ((otherY2 - otherEntity->position.y) >> 16);
        if (ox * ox + oy * oy >= ox2 * ox2 + oy2 * oy2) {
            if (collisionDir || !collisionResult) {
                otherEntity->position.x = otherX2;
                otherEntity->position.y = otherY2;
                if (setValues) {
                    if (collisionDir != 1) {
                        if (collisionDir == 4 && otherEntity->velocity.y < 0) {
                            otherEntity->velocity.y = 0;
                            return collisionDir;
                        }
                        return collisionDir;
                    }

                    if (otherEntity->velocity.y > 0)
                        otherEntity->velocity.y = 0;
                    if (otherEntity->onGround == false && otherEntity->velocity.y >= 0) {
                        otherEntity->groundVel = otherEntity->velocity.x;
                        otherEntity->angle     = 0;
                        otherEntity->onGround  = 1;
                    }
                }
                return collisionDir;
            }
        }
        else if (!collisionResult && collisionDir) {
            otherEntity->position.x = otherX2;
            otherEntity->position.y = otherY2;
            if (setValues) {
                if (collisionDir != 1) {
                    if (collisionDir == 4 && otherEntity->velocity.y < 0) {
                        otherEntity->velocity.y = 0;
                        return collisionDir;
                    }
                    return collisionDir;
                }

                if (otherEntity->velocity.y > 0)
                    otherEntity->velocity.y = 0;
                if (!otherEntity->onGround && otherEntity->velocity.y >= 0) {
                    otherEntity->groundVel = otherEntity->velocity.x;
                    otherEntity->angle     = 0;
                    otherEntity->onGround  = true;
                }
            }
            return collisionDir;
        }

        otherEntity->position.x = otherX;
        otherEntity->position.y = otherY;
        if (!setValues)
            return collisionResult;

        int vel = 0;
        if (otherEntity->onGround) {
            vel = otherEntity->groundVel;
            if (otherEntity->collisionMode == CMODE_ROOF)
                vel = -vel;
        }
        else {
            vel = otherEntity->velocity.x;
        }

        if (collisionResult == 2) {
            if (vel <= 0)
                return collisionResult;
        }
        else if (collisionResult != 3 || vel >= 0) {
            return collisionResult;
        }
        otherEntity->groundVel  = 0;
        otherEntity->velocity.x = 0;
        return collisionResult;
    }
    return 0;
}

bool32 CheckObjectCollisionPlatform(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox,
                                                       bool32 setValues)
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
    int thisIY = (thisEntity->position.y >> 16);
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
            store              = otherHitbox->left;
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
    int posX     = (xOffset + entity->position.x) >> 16;
    int posY     = (yOffset + entity->position.y) >> 16;
    switch (cMode) {
        default: return false;
        case CMODE_FLOOR: {
            int solid = 0x1000;
            if (cPlane)
                solid = 0x4000;
            for (int l = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cy           = (colX & 0xFFFFFFF0) - 16;
                    if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                        ushort *layout = &layer->layout[(colX >> 4) + ((cy / TILE_SIZE) * layer->width)];
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

            if (setPos)
                entity->position.y = (posY << 16) - yOffset;
            return collided;
        }
        case CMODE_LWALL: {
            int solid = 0x2000;
            if (cPlane)
                solid = 0x8000;
            for (int l = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
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

            if (setPos)
                entity->position.x = (posX << 16) - xOffset;
            return collided;
        }
        case CMODE_ROOF: {
            int solid = 0x1000;
            if (cPlane)
                solid = 0x4000;
            for (int l = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cy           = (colX & 0xFFFFFFF0) + 16;
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

            if (setPos)
                entity->position.y = (posY << 16) - yOffset;
            return collided;
        }
        case CMODE_RWALL: {
            int solid = 0x2000;
            if (cPlane)
                solid = 0x8000;
            for (int l = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
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
                                    if (colX >= tx && abs(colX - tx) <= 14) {
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

            if (setPos)
                entity->position.x = (posX << 16) - xOffset;
            return collided;
        }
    }
}
bool32 ObjectTileGrip(Entity *entity, ushort cLayers, char cMode, char cPlane, int xOffset, int yOffset, int tolerance)
{
    int layerID     = 1;
    bool32 collided = 0;
    int posX        = (xOffset + entity->position.x) >> 16;
    int posY        = (yOffset + entity->position.y) >> 16;
    switch (cMode) {
        default: return false;
        case CMODE_FLOOR: {
            int solid = 0x1000;
            if (cPlane)
                solid = 0x4000;
            for (int l = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cy           = (colX & 0xFFFFFFF0) - 16;
                    if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                        ushort *layout = &layer->layout[(colX >> 4) + ((cy / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int ty = cy + collisionMasks[cPlane][tile & 0xFFF].floorMasks[colX & 0xF];
                                    if (collisionMasks[cPlane][tile & 0xFFF].floorMasks[colX & 0xF] < 0xFF) {
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

            entity->position.y = (posY << 16) - yOffset;
            return collided;
        }
        case CMODE_LWALL: {
            int solid = 0x2000;
            if (cPlane)
                solid = 0x8000;
            for (int l = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
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
                                    int tx = cx + collisionMasks[cPlane][tile & 0xFFF].rWallMasks[colY & 0xF];
                                    if (collisionMasks[cPlane][tile & 0xFFF].lWallMasks[colY & 0xF] < 0xFF) {
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

            entity->position.x = (posX << 16) - xOffset;
            return collided;
        }
        case CMODE_ROOF: {
            int solid = 0x1000;
            if (cPlane)
                solid = 0x4000;
            for (int l = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
                if (cLayers & layerID) {
                    TileLayer *layer = &tileLayers[l];
                    int colX         = posX - layer->position.x;
                    int colY         = posY - layer->position.y;
                    int cy           = (colX & 0xFFFFFFF0) + 16;
                    if (colX >= 0 && colX < TILE_SIZE * layer->width) {
                        ushort *layout = &layer->layout[(colX >> 4) + ((cy / TILE_SIZE) * layer->width)];
                        for (int i = 0; i < 3; ++i) {
                            if (cy >= 0 && cy < TILE_SIZE * layer->height) {
                                ushort tile = *layout;
                                if (tile < 0xFFFF && tile & solid) {
                                    int ty = cy + collisionMasks[cPlane][tile & 0xFFF].roofMasks[colX & 0xF];
                                    if (collisionMasks[cPlane][tile & 0xFFF].roofMasks[colX & 0xF] < 0xFF) {
                                        if (colY <= ty && abs(colY - ty) <= 14) {
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

            entity->position.y = (posY << 16) - yOffset;
            return collided;
        }
        case CMODE_RWALL: {
            int solid = 0x2000;
            if (cPlane)
                solid = 0x8000;
            for (int l = 1; l < LAYER_COUNT; ++l, layerID <<= 1) {
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
                                    if (collisionMasks[cPlane][tile & 0xFFF].rWallMasks[colY & 0xF] < 0xFF) {
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

            entity->position.x = (posX << 16) - xOffset;
            return collided;
        }
    }
}
