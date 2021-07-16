#include "../SonicMania.h"

ObjectSpringboard *Springboard;

void Springboard_Update(void)
{
    /*EntitySpringboard *entity; // esi
    char playerID;             // al
    int v4;                    // eax
    int v5;                    // ecx
    int v6;                    // edi
    unsigned int v7;           // eax
    int v8;                    // ecx
    int v9;                    // edi
    int v11;                   // ebx
    int v12;                   // eax
    int v13;                   // eax
    int v14;                   // eax
    int v15;                   // eax MAPDST
    bool32 v16;                  // zf
    signed int v17;            // eax
    unsigned __int8 v18;       // al
    Hitbox *v19;               // eax
    Hitbox *v20;               // ecx
    int v21;                   // eax
    int v22;                   // eax
    signed int v23;            // eax
    Hitbox *v24;               // eax
    Hitbox *v25;               // ecx
    Hitbox Hitbox;             // [esp+Ch] [ebp-20h] BYREF
    int v27;                   // [esp+14h] [ebp-18h]
    int v29;                   // [esp+1Ch] [ebp-10h]
    int v30;                   // [esp+20h] [ebp-Ch]
    EntityPlayer *player;      // [esp+28h] [ebp-4h] MAPDST BYREF

    RSDK_THIS(Springboard);
    RSDK.ProcessAnimation(&entity->animator);
    player = 0;
    while (RSDK.GetActiveEntities(Player->objectID, &player) == true) {
        playerID = RSDK.GetEntityID(player);
        if (player->velocity.y >= 0 && ((1 << playerID) & entity->playerBits) != 0) {
            v4 = (player->position.x - entity->position.x + 0x1C0000) >> 17;
            if (v4 > 28)
                v4 = 28;
            v5 = 0;
            if (v4 >= 0)
                v5 = v4;
            v6 = 28 - v5;
            if ((entity->direction & 1) == 0)
                v6 = v5;
            if (v6 >= 8) {
                if (entity->animator.frameID == 3)
                    RSDK.SetSpriteAnimation(Springboard->animID, 0, &entity->animator, true, 0);
                if (entity->animator.frameID == 2) {
                    v7 = player->playerAnimator.animationID;
                    if (v7 == 5 || v7 > 6 && v7 <= 9)
                        player->storedAnim = player->playerAnimator.animationID;
                    else
                        player->storedAnim = ANI_WALK;
                    player->state               = Player_State_Air;
                    player->onGround       = false;
                    player->tileCollisions = true;
                    RSDK.SetSpriteAnimation(player->spriteIndex, 23, &player->playerAnimator, true, 1);
                    player->groundVel = player->velocity.x;
                    v8                     = 2 * v6 - 16;
                    if (v8 > 39)
                        v8 = 39;
                    player->velocity.y = Springboard->array2[v8] - player->gravityStrength - entity->force;
                    player->jumpAbility     = 0;
                    RSDK.PlaySFX(Springboard->sfxID, 0, 255);
                }
            }
        }
    }
    player = 0;
    if (RSDK.GetActiveEntities(Player->objectID, &player) == true) {
        v9 = v27;
        while (1) {
            playerID = RSDK.GetEntityID(player);
            v11 = 0;
            v30 = player->groundVel;
            v12 = player->position.x - entity->position.x + 1835008;
            v29 = player->velocity.x;
            v13 = v12 >> 17;
            if (v13 >= 0)
                v11 = v13;
            if (v11 > 27)
                v11 = 27;
            if (!entity->direction)
                break;
            if (entity->direction == 1) {
                v14 = entity->animator.frameID;
                v11 = abs32(v11 - 27);
                if (v14 >= 0) {
                    if (v14 <= 2) {
                        v15 = Springboard->array4[v11];
                    }
                    else if (v14 == 3) {
                        v15 = Springboard->array3[v11];
                    }
                }
                *&Hitbox.right = -524260;
                Hitbox.top     = -v15;
                Hitbox.left    = -28;
                v16            = ((1 << playerID) & entity->playerBits) == 0;
                v27            = 1 << playerID;
                if (v16)
                    v17 = Player_CheckCollisionBox(player, entity, &Hitbox);
                else
                    v17 = Player_CheckCollisionPlatform(player, entity, &Hitbox);
                v9 = v17;
                switch (v17) {
                    case 0:
                    case 2:
                    case 4: break;
                    case 1: goto LABEL_40;
                    case 3:
                        player->groundVel  = v30;
                        player->velocity.x = v29;
                        break;
                    default: goto LABEL_42;
                }
                if (player->velocity.y >= 0 && (v27 & entity->playerBits) != 0) {
                    v19 = RSDK.GetHitbox(&player->playerAnimator, 0);
                    v20 = &defaultHitbox;
                    if (v19)
                        v20 = v19;
                    player->position.y = entity->position.y - (v20->bottom << 16) - (v15 << 16);
                    if (v17)
                    LABEL_72:
                        v17 = true;
                    else
                        v17 = player->position.x < entity->position.x;
                }
            LABEL_43:
                player->flailing = 0;
            }
            v18 = entity->playerBits;
            if (v9 == 1) {
                entity->playerBits = v18 | (1 << playerID);
                if (v11 >= 8 && !entity->field_5C)
                    entity->field_5C = 6;
            }
            else {
                entity->playerBits = v18 & ~(1 << playerID);
            }
            if (RSDK.GetActiveEntities(Player->objectID, &player) != true)
                return;
        }
        v21 = entity->animator.frameID;
        if (v21 >= 0) {
            if (v21 <= 2) {
                v22 = Springboard->array4[v11];
                v15 = v22;
                goto LABEL_61;
            }
            if (v21 == 3) {
                v22 = Springboard->array3[v11];
                v15 = v22;
            LABEL_61:
                *&Hitbox.right = -524260;
                Hitbox.top     = -v22;
                Hitbox.left    = -28;
                v16            = ((1 << playerID) & entity->playerBits) == 0;
                v27            = 1 << playerID;
                if (v16)
                    v23 = Player_CheckCollisionBox(player, entity, &Hitbox);
                else
                    v23 = Player_CheckCollisionPlatform(player, entity, &Hitbox);
                v9 = v23;
                switch (v23) {
                    case 0:
                    case 3:
                    case 4: goto LABEL_66;
                    case 1:
                    LABEL_40:
                        if (player->state != Player_State_MightyHammerDrop)
                            goto LABEL_43;
                        player->state = Player_State_Air;
                    LABEL_42:
                        break;
                    case 2:
                        player->groundVel  = v30;
                        player->velocity.x = v29;
                    LABEL_66:
                        if (player->velocity.y >= 0 && (v27 & entity->playerBits) != 0) {
                            v24 = RSDK.GetHitbox(&player->playerAnimator, 0);
                            v25 = &defaultHitbox;
                            if (v24)
                                v25 = v24;
                            player->position.y = entity->position.y - (v25->bottom << 16) - (v15 << 16);
                            if (v9 || player->position.x > entity->position.x)
                                goto LABEL_72;
                        }
                        goto LABEL_43;
                    default: goto LABEL_42;
                }
                goto LABEL_43;
            }
        }
        LOWORD(v22) = v15;
        goto LABEL_61;
    }//*/
    
}

void Springboard_LateUpdate(void)
{

}

void Springboard_StaticUpdate(void)
{

}

void Springboard_Draw(void)
{

}

void Springboard_Create(void* data)
{

}

void Springboard_StageLoad(void)
{

}

void Springboard_EditorDraw(void)
{

}

void Springboard_EditorLoad(void)
{

}

void Springboard_Serialize(void)
{

}

