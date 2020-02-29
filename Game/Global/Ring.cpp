#include "Ring.h"

void Ring_Update()
{
    void(*Update)(void);

    Update = *(void(**)(void))(CurrentEntity->XPos + 0x58);
    if (Update)                                 // Call State
        Update();
}


void Ring_Draw()
{
    void(*DrawState)(void);

    DrawState = *(void(**)(void))(CurrentEntity->XPos + 0x5C);
    if (DrawState)                              // Call DrawState
        DrawState();
}


void Ring_Setup(int Subtype)
{
    EntityRing* Ring;
    int PlaneFilter;
    char DrawOrder;
    bool v4; // zf
    int MoveType;
    int AmplitudeX;
    int AmplitudeY;
    EntityAnimationData* Animation;

    Ring = (EntityRing*)CurrentEntity->XPos;
    PlaneFilter = *(_DWORD*)(CurrentEntity->XPos + 0x64);
    *(_BYTE*)(CurrentEntity->XPos + 85) = 1;
    if (PlaneFilter > 0 && ((_BYTE)PlaneFilter - 1) & 2)
        DrawOrder = OBJ_ZONE->DrawOrderHigh;
    else
        DrawOrder = OBJ_ZONE->DrawOrderLow;
    v4 = Ring->Type == 1;
    Ring->Base.DrawOrder = DrawOrder + 1;
    if (v4)
    {
        Ring->Base.DrawFX |= FX_FLIP;
        Ring->RingAmount = 5;
    }
    Animation = &Ring->AnimData;
    if (Subtype)
    {
        Ring->Base.Priority = ACTIVE_NORMAL;
        RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, 0, Animation, 1, 0);
    }
    else
    {
        RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, Ring->Type, Animation, 1, 0);
        MoveType = Ring->MoveType;
        Ring->AmplitudeX >>= 10;
        Ring->AmplitudeY >>= 10;
        AmplitudeX = Ring->AmplitudeX;
        AmplitudeY = Ring->AmplitudeY;
        Ring->Base.Priority = ACTIVE_BOUNDS;
        switch (MoveType)
        {
            case 1:
                if (AmplitudeX < 0)
                    AmplitudeX = -AmplitudeX;
                Ring->Base.UpdateRangeX = (AmplitudeX + 0x1000) << 10;
                if (AmplitudeY < 0)
                    AmplitudeY = -AmplitudeY;
                Ring->State = RingState_Move;
                Ring->Base.UpdateRangeY = (AmplitudeY + 0x1000) << 10;
                Ring->StateDraw = RingStateDraw_Oscillating;
                break;
            case 2:
                if (AmplitudeX < 0)
                    AmplitudeX = -AmplitudeX;
                Ring->Base.UpdateRangeX = (AmplitudeX + 0x1000) << 10;
                if (AmplitudeY < 0)
                    AmplitudeY = -AmplitudeY;
                Ring->State = RingState_Circular;
                Ring->Base.UpdateRangeY = (AmplitudeY + 0x1000) << 10;
                Ring->StateDraw = RingStateDraw_Oscillating;// StateDraw = Draw_Oscillating
                break;
            case 3:
                if (AmplitudeX < 0)
                    AmplitudeX = -AmplitudeX;
                Ring->Base.UpdateRangeX = (AmplitudeX + 0x2000) << 9;
                if (AmplitudeY < 0)
                    AmplitudeY = -AmplitudeY;
                Ring->State = RingState_Path;
                Ring->Base.UpdateRangeY = (AmplitudeY + 0x2000) << 9;
                Ring->StateDraw = RingStateDraw_Oscillating;// StateDraw = Draw_Oscillating
                break;
            case 4:
                Ring->Base.UpdateRangeX = 0x400000;
                Ring->Base.UpdateRangeY = 0x400000;
                Ring->Base.Priority = ACTIVE_NEVER;
                Ring->State = RingState_Track;          // State = State_Track
                Ring->StateDraw = RingStateDraw_Oscillating;// StateDraw = Draw_Oscillating
                break;
            default:
                Ring->Base.UpdateRangeX = 0x400000;     // UpdateRange.x = 0x4000000
                Ring->Base.UpdateRangeY = 0x400000;     // UpdateRange.y = 0x4000000
                Ring->State = RingState_Normal;         // State = State.Normal
                Ring->StateDraw = RingStateDraw_Normal; // StateDraw = Draw.Normal
                break;
        }
    }
}

void Ring_StageLoad()
{
    __int16 v0; // ax
    int v1; // edx

    v0 = RSDK_LoadAnimation("Global/Ring.bin", RSDK_SCOPE_STAGE);
    Obj_Ring->SpriteIndex = v0;
    Obj_Ring->Hitbox.Left = -8;
    Obj_Ring->Hitbox.Top = -8;
    Obj_Ring->Hitbox.Right = 8;
    Obj_Ring->Hitbox.Bottom = 8;
    v1 = OBJ_DebugMode->DebugItemCount;
    if (v1 < 256)
    {
        OBJ_DebugMode->Sprites[v1] = Obj_Ring->StructSize;
        OBJ_DebugMode->Spawn[OBJ_DebugMode->DebugItemCount] = (int)DebugMode_SpawnRing;
        OBJ_DebugMode->Draw[OBJ_DebugMode->DebugItemCount++] = (int)DebugMode_DrawRing;
    }
    Obj_Ring->field_12 = RSDK_LoadSFX("Global/Ring.wav");
}

int DebugMode_SpawnRing()
{
    return RSDK_SpawnEntity(
             (RSDK_OBJECT*)Obj_Ring->StructSize,
             0,
             *(_DWORD*)CurrentEntity->XPos,
             *(_DWORD*)(CurrentEntity->XPos + 4));
}

//----- (00492270) --------------------------------------------------------
int DebugMode_DrawRing()
{
    RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, 0, &OBJ_DebugMode->DebugData, 1, 0);
    return RSDK_DrawSprite(&OBJ_DebugMode->DebugData, 0, 0);
}

int Ring_Collect()
{
    EntityRing* Ring; // edi
    bool NoXOffset; // zf
    Hitbox* RingHitbox; // ebx
    Hitbox* v4; // ecx
    Hitbox* v5;
    int v6;
    int v7;
    int RingCount; // edx
    int result;
    signed int v11;
    BOOL v12; // ecx
    int SparkleType; // ebx
    int v14;
    int YOffset; // ST10_4
    int XOffset;
    EntityRing* SpawnedRing; MAPDST
    unsigned int v19;
    int v20;
    EntityPlayer* Player; // [esp+10h] [ebp-10h] MAPDST
    int Min; // [esp+14h] [ebp-Ch]
    int YPos; // [esp+18h] [ebp-8h]
    int XPos; // [esp+1Ch] [ebp-4h]

    Ring = (EntityRing*)CurrentEntity->XPos;
    NoXOffset = *(_DWORD*)(CurrentEntity->XPos + 0x8C) == 0;
    XPos = *(_DWORD*)CurrentEntity->XPos;
    YPos = Ring->Base.YPos;
    if (!NoXOffset)
    {
        Ring->Base.XPos = Ring->XOffset;
        Ring->Base.YPos = Ring->YOffset;
    }
    Player = 0;
    if (RSDK_GetObjects(Obj_Player->StructSize, &Player) != 1)
    {
        LABEL_20:
        Ring->Base.XPos = XPos;
        result = YPos;
        Ring->Base.YPos = YPos;
        return result;
    }
    while (1)
    {
        RingHitbox = &Obj_Ring->Hitbox;
        if (!Player->field_1F0)
        {
            v4 = Player->Outerbox;
            if (!v4)
                v4 = RSDK_GetHitbox(&Player->PlayerSprite, 0);
            v5 = &stru_66B804;
            if (v4)
                v5 = v4;
            v6 = RSDK_CheckObjectCollisionTouch(Ring, RingHitbox, Player, v5);
            if (v6 == 1)
                break;
        }
        if (Ring->State != RingState_Attract
          && Player->Shield == SHIELD_LIGHTNING
          && RSDK_CheckObjectCollision2(Ring, &loc_500000, Player, 0x10000) == 1)
        {
            Ring->XOffset = 0;
            Ring->State = RingState_Attract;
            Ring->StateDraw = RingStateDraw_Normal;
            Ring->Base.Priority = ACTIVE_NORMAL;
            Ring->StoredPlayer = &Player->Base;
            return dword_EA7744();
        }
        LABEL_19:
        if (RSDK_GetObjects(Obj_Player->StructSize, &Player) != 1)
            goto LABEL_20;
    }
    v7 = Ring->PlaneFilter;
    if (v7 > 0 && Player->Base.CollisionPlane != (((_BYTE)v7 - 1) & 1))
        goto LABEL_19;
    if (Player->Sidekick)
        Player = (EntityPlayer*)RSDK_GetObjectByID(0);
    if (Ring->Type == 1)
    {
        Player->RingExtraLife += 100 * (Ring->RingAmount / 100);
        RingCount = Ring->RingAmount;
    }
    else
    {
        RingCount = 1;
    }
    Player_GiveRings(RingCount, Player, 1);
    v11 = 0x100000;
    v12 = Ring->Type == 1;
    if (Ring->Type != 1)
        v11 = 0x80000;
    SparkleType = 0;
    XPos = v11;
    YPos = 4 * v12 + 4;
    if (YPos > 0)
    {
        v14 = -v11;
        Min = -v11;
        do
        {
            YOffset = Ring->Base.YPos + Random(v14, v11);
            XOffset = Random(Min, v11);
            SpawnedRing = (EntityRing*)RSDK_SpawnEntity(
                                          (RSDK_OBJECT*)Obj_Ring->StructSize,
                                          0,
                                          Ring->Base.XPos + XOffset,
                                          YOffset);
            SpawnedRing->State = RingState_Sparkle;
            SpawnedRing->StateDraw = RingStateDraw_Sparkle;
            SpawnedRing->Base.Priority = 2;
            SpawnedRing->Base.Visible = 0;
            if (Ring->Base.DrawOrder == 1)
                SpawnedRing->Base.DrawOrder = 1;
            else
                SpawnedRing->Base.DrawOrder = OBJ_ZONE->DrawOrderHigh;
            RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, SparkleType % 3 + 2, &SpawnedRing->AnimData, 1, 0);
            v19 = SpawnedRing->AnimData.FrameCount;
            if (SpawnedRing->AnimData.AnimationID == 2)
            {
                SpawnedRing->Base.Alpha = 224;
                v19 >>= 1;
            }
            SpawnedRing->MaxFrameCount = v19 - 1;
            SpawnedRing->AnimData.AnimationSpeed = Random(6, 8);
            v20 = 2 * SparkleType++;
            SpawnedRing->Timer = v20;
            v11 = XPos;
            v14 = Min;
        }
        while (SparkleType < YPos);
    }
    RSDK_ResetObjectEntity(&Ring->Base, 0, 0);
    Ring->Base.Priority = ACTIVE_NEVER3;
    return dword_EA7744();
}

unsigned int __usercall Player_LoseRings(EntityPlayer* Player, signed int Rings, char CollisionPlane)
{
  EntityPlayer* player; // edi MAPDST
signed int InnerRingCount; // ecx MAPDST
int OuterRingCount; MAPDST
int GrowRingCount; // ebx MAPDST
unsigned int StartingAngle; // ecx
int v8;
int v10; // ebx
EntityRing* Ring; MAPDST
int SineAngle;
unsigned int v13; // ecx
int v14;
int v15; // ecx
bool v18; // zf
unsigned int v19;
unsigned int result;
int v21; // ecx
int RingAngle; // [esp+1Ch] [ebp-4h] MAPDST

player = Player;
  InnerRingCount = 16;
  player = (EntityPlayer*) CurrentEntity->XPos;
  OuterRingCount = Rings - 16;
  GrowRingCount = Rings - 32;
  if (Rings <= 16 )
    InnerRingCount = Rings;
  if (OuterRingCount <= 16 )
  {
    if (OuterRingCount< 0 )
      OuterRingCount = 0;
  }
  else
  {
    OuterRingCount = 16;
  }
  if (GrowRingCount <= 16 )
  {
    if (GrowRingCount< 0 )
      GrowRingCount = 0;
  }
  else
  {
    GrowRingCount = 16;
  }
  StartingAngle = 0xC0 - 8 * (OuterRingCount & 0xFFFFFFFE);
  v8 = StartingAngle - 8;
  RingAngle = StartingAngle + 8;
  if ( !(OuterRingCount & 1) )
    RingAngle = v8;
  if (OuterRingCount > 0 )
  {
    v10 = OuterRingCount;
    do
    {
      Ring = (EntityRing*) RSDK_SpawnEntity(
                             (RSDK_OBJECT*) Obj_Ring->StructSize,
                             player,
                             player->Base.XPos,
                             player->Base.YPos);
Ring->Base.XVelocity = RSDK_Cos256(RingAngle) << 9;
      SineAngle = RSDK_Sin256(RingAngle);
RingAngle += 16;
      Ring->Base.YVelocity = SineAngle << 9;
      Ring->AnimData.AnimationSpeed = 512;
      Ring->Base.CollisionPlane = CollisionPlane;
      Ring->Base.InkEffect = 2;
      Ring->Base.Alpha = 256;
      Ring->Base.field_3C = 1;
      Ring->State = RingState_Bounce;
      Ring->StateDraw = RingStateDraw_Normal;
      Ring->Base.DrawOrder = player->Base.DrawOrder;
      Ring->MoveType = 1;
      --v10;
    }
    while (v10 );
  }
  v13 = 0xC0 - 8 * (InnerRingCount & 0xFFFFFFFE);
  v14 = v13 - 8;
  v15 = v13 + 8;
  if ( !(InnerRingCount & 1) )
    v15 = v14;
  if (InnerRingCount > 0 )
  {
    RingAngle = v15;
    do
    {
      Ring = (EntityRing*) RSDK_SpawnEntity(
                             (RSDK_OBJECT*) Obj_Ring->StructSize,
                             player,
                             player->Base.XPos,
                             player->Base.YPos);
Ring->Base.XVelocity = RSDK_Cos256(RingAngle) << 10;
      Ring->Base.YVelocity = RSDK_Sin256(RingAngle) << 10;
      RingAngle += 16;
      v18 = InnerRingCount-- == 1;
      Ring->AnimData.AnimationSpeed = 512;
      Ring->Base.CollisionPlane = CollisionPlane;
      Ring->Base.InkEffect = 2;
      Ring->Base.Alpha = 256;
      Ring->Base.field_3C = 1;
      Ring->State = RingState_Bounce;
      Ring->StateDraw = RingStateDraw_Normal;
      Ring->Base.DrawOrder = player->Base.DrawOrder;
    }
    while ( !v18 );
  }
  v19 = 0xC0 - 8 * (GrowRingCount & 0xFFFFFFFE);
  result = v19 - 8;
  v21 = v19 + 8;
  if ( !(GrowRingCount & 1) )
    v21 = v19 - 8;
  RingAngle = v21;
  if (GrowRingCount > 0 )
  {
    do
    {
      Ring = (EntityRing*) RSDK_SpawnEntity(
                             (RSDK_OBJECT*) Obj_Ring->StructSize,
                             player,
                             player->Base.XPos,
                             player->Base.YPos);
Ring->Base.XVelocity = RSDK_Cos256(RingAngle) << 11;
      Ring->Base.YVelocity = RSDK_Sin256(RingAngle) << 11;
      RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, 1, &Ring->AnimData, 1, 0);
RingAngle += 16;
      Ring->Base.ScaleX = 256;
      result = 512;
      Ring->AnimData.AnimationSpeed = 512;
      Ring->Base.ScaleY = 256;
      Ring->Base.DrawFX = 5;
      Ring->State = RingState_Grow;
      Ring->StateDraw = RingStateDraw_Normal;
      --GrowRingCount;
    }
    while (GrowRingCount );
  }
  return result;
}

int __usercall Player_LoseHyperRings(EntityPlayer* Player, int Rings, char CollisionPlane)
{
  int RingCount; // ebx
EntityPlayer* player; MAPDST
int PerRingAmount;
unsigned int StartAngle; // edx
int RingAngle; // edi
EntityRing* Ring;
int v9;
int RingAmount; // [esp+10h] [ebp-8h]
int RingID; // [esp+14h] [ebp-4h]

RingCount = Rings >> 2;
  player = Player;
  player = Player;
  if (Rings >> 2 <= 8 )
  {
    if (RingCount< 1 )
      RingCount = 1;
  }
  else
  {
    RingCount = 8;
  }
  RingID = 0;
  PerRingAmount = Rings / RingCount;
  StartAngle = 0xC0 - 0x10 * (RingCount & 0xFFFFFFFE);
  RingAmount = PerRingAmount;
  RingAngle = StartAngle + 0x10;
  if ( !(RingCount & 1) )
    RingAngle = StartAngle - 0x10;
  if (RingCount > 0 )
  {
    do
    {
      Ring = (EntityRing*) RSDK_SpawnEntity(
                             (RSDK_OBJECT*) Obj_Ring->StructSize,
                             player,
                             player->Base.XPos,
                             player->Base.YPos);
RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, 1, &Ring->AnimData, 1, 0);
Ring->Type = 1;
      Ring->Base.XVelocity = 768 * RSDK_Cos256(RingAngle);
v9 = RSDK_Sin256(RingAngle);
RingAngle += 32;
      Ring->Base.field_3C = 1;
      Ring->Base.DrawFX = 5;
      Ring->Base.Alpha = 256;
      Ring->Base.YVelocity = 768 * v9;
Ring->AnimData.AnimationSpeed = 384;
      Ring->Base.CollisionPlane = CollisionPlane;
      Ring->Base.Angle = RingID << 6;
      PerRingAmount = RingAmount;
      Ring->RingAmount = RingAmount;
      Ring->State = RingState_Big;
      Ring->StateDraw = RingStateDraw_Normal;
      ++RingID;
    }
    while (RingID<RingCount );
  }
  return PerRingAmount;
}

char __usercall Ring_FakeLoseRings@<al>(int* a1, signed int a2, char a3)
{
  int* v3;
bool v4; // zf
bool v5; // sf
unsigned __int8 v6; // of
  int v7; // edx
int v8;
signed int v9; // edi
int v10; // ebx
unsigned int v11; // ecx
int v12;
int v13; // ecx
EntityRing* v14;
int v15; // edi
int v16; // ebx
unsigned int v17; // ecx
int v18;
int v19; // ecx
EntityRing* v20;
int v21; // edi
int v22; // ebx
unsigned int v23; // edx
int v24;
int v25; // edx
EntityRing* v26;
int v28; // [esp+Ch] [ebp-14h]
signed int v29; // [esp+10h] [ebp-10h]
int* subObjectID; // [esp+14h] [ebp-Ch]
int v31; // [esp+18h] [ebp-8h]
int v32; // [esp+18h] [ebp-8h]
int v33; // [esp+18h] [ebp-8h]
signed int v34; // [esp+1Ch] [ebp-4h]

v34 = a2;
  v3 = a1;
  v6 = __OFSUB__(a2, 16);
v4 = a2 == 16;
  v5 = a2 - 16 < 0;
  v7 = a2 - 16;
  subObjectID = a1;
  v8 = v7 - 16;
  v9 = 2048;
  v10 = 256;
  if ( !((unsigned __int8)(v5 ^ v6) | v4) )
  {
    v9 = 1536;
    v34 = 16;
    v10 = 192;
  }
  if (v7 <= 16 )
  {
    if (v7< 0 )
      v7 = 0;
  }
  else
  {
    v9 -= 512;
    v7 = 16;
    v10 -= 64;
  }
  v28 = v7;
  if (v8 <= 16 )
  {
    if (v8< 0 )
      v8 = 0;
    v29 = v8;
  }
  else
  {
    v29 = 16;
  }
  v11 = 192 - 8 * (v7 & 0xFFFFFFFE);
  v12 = v11 - 8;
  v13 = v11 + 8;
  if ( !(v7 & 1) )
    v13 = v12;
  v31 = v13;
  if (v7 > 0 )
  {
    do
    {
      v14 = (EntityRing*) RSDK_SpawnEntity((RSDK_OBJECT*) Obj_Ring->StructSize, v3, * v3, v3[1]);
v14->Base.XVelocity = v9* RSDK_Cos256(v31);
v14->Base.YVelocity = v9* RSDK_Sin256(v31);
RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, 1, &v14->AnimData, 1, 0);
v31 += 16;
      v14->AnimData.AnimationSpeed = 512;
      v4 = v28-- == 1;
      v14->Base.ScaleX = v10;
      v14->Base.ScaleY = v10;
      v14->Base.DrawFX = 5;
      v14->Base.DrawOrder = a3;
      v14->State = RingState_Grow;
      v14->StateDraw = RingStateDraw_Normal;
      v3 = subObjectID;
    }
    while ( !v4 );
  }
  v15 = v9 + 512;
  v16 = v10 + 64;
  v17 = 192 - 8 * (v34 & 0xFFFFFFFE);
  v18 = v17 - 8;
  v19 = v17 + 8;
  if ( !(v34 & 1) )
    v19 = v18;
  v32 = v19;
  if (v34 > 0 )
  {
    do
    {
      v20 = (EntityRing*) RSDK_SpawnEntity((RSDK_OBJECT*) Obj_Ring->StructSize, v3, * v3, v3[1]);
v20->Base.XVelocity = v15* RSDK_Cos256(v32);
v20->Base.YVelocity = v15* RSDK_Sin256(v32);
RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, 1, &v20->AnimData, 1, 0);
v32 += 16;
      v20->AnimData.AnimationSpeed = 512;
      v4 = v34-- == 1;
      v20->Base.ScaleX = v16;
      v20->Base.ScaleY = v16;
      v20->Base.DrawFX = 5;
      v20->Base.DrawOrder = a3;
      v20->State = RingState_Grow;
      v20->StateDraw = RingStateDraw_Normal;
      v3 = subObjectID;
    }
    while ( !v4 );
  }
  v21 = v15 + 512;
  v22 = v16 + 64;
  v23 = 192 - 8 * (v29 & 0xFFFFFFFE);
  v24 = v23 - 8;
  v25 = v23 + 8;
  if ( !(v29 & 1) )
    v25 = v24;
  v33 = v25;
  if (v29 > 0 )
  {
    do
    {
      v26 = (EntityRing*) RSDK_SpawnEntity(
                            (RSDK_OBJECT*) Obj_Ring->StructSize,
                            subObjectID,
                            * subObjectID,
                            subObjectID[1]);
v26->Base.XVelocity = v21* RSDK_Cos256(v33);
v26->Base.YVelocity = v21* RSDK_Sin256(v33);
RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, 1, &v26->AnimData, 1, 0);
v33 += 16;
      v26->AnimData.AnimationSpeed = 512;
      v4 = v29-- == 1;
      LOBYTE(v24) = a3;
      v26->Base.ScaleX = v22;
      v26->Base.ScaleY = v22;
      v26->Base.DrawFX = 5;
      v26->Base.DrawOrder = a3;
      v26->State = RingState_Grow;
      v26->StateDraw = RingStateDraw_Normal;
    }
    while ( !v4 );
  }
  return v24;
}

int __thiscall Ring_CheckPlatformCollision(EntityPlatform* Platform)
{
    EntityRing* Ring; // ebx
    int v3; // edi
    int(*RingState)();
    Hitbox* v5;
    Hitbox* v6;
    SceneLayer* MoveLayer;

    Ring = (EntityRing*)CurrentEntity->XPos;
    v3 = 0;
    RingState = (int(*)())Platform->State;
    if (RingState != PlatformState_Falling && (char*)RingState != (char*)PlatformState_OffscreenReset)
    {
        Platform->XPos = Platform->DrawX - Platform->CollisionXOffset;
        Platform->YPos = Platform->DrawY - Platform->CollisionYOffset;
        switch (Platform->CollisionType)
        {
            case 0u:
                v5 = RSDK_GetHitbox(&Platform->AnimData, 0);
                v3 = dword_EA7940(Platform, v5, Ring, &Obj_Ring->Hitbox, 1);
                break;
            case 1u:
            case 5u:
            case 6u:
            case 7u:
            case 8u:
            case 9u:
            case 0xAu:
            case 0xBu:
            case 0xCu:
            case 0xDu:
            case 0xEu:
            case 0xFu:
                v6 = RSDK_GetHitbox(&Platform->AnimData, 1);
                v3 = dword_EA793C(Platform, v6, Ring, &Obj_Ring->Hitbox, 1);
                break;
            case 2u:
                v3 = 0;
                if (RSDK_CheckObjectCollisionTouch(Platform, &Platform->Hitbox, Ring, &Obj_Ring->Hitbox)
                  && Ring->Base.field_50 & *(_WORD*)&OBJ_ZONE->field_172)
                {
                    MoveLayer = RSDK_GetSceneLayerInfo(OBJ_ZONE->MoveLayer);
                    MoveLayer->field_C = -(Platform->DrawX + Platform->TileOriginX) >> 16;
                    MoveLayer->field_10 = -(Platform->DrawY + Platform->TileOriginY) >> 16;
                }
                break;
            default:
                break;
        }
        Platform->XPos = Platform->CenterX;
        Platform->YPos = Platform->CenterY;
        if (v3 == 2 || v3 == 3)
            Ring->Base.XVelocity -= Platform->XVelocity;
    }
    return v3;
}

int __fastcall Ring_CheckObjectCollisions(int HitboxLR, int HitboxTB)
{
    int v2; // ebx
    EntityRing* Ring;
    int v4; // edi
    int v5; // edx
    int v6;
    int v7; // ecx
    int v8;
    int v9; // edi
    int v10;
    bool v11; // sf
    unsigned __int8 v12; // of
    int v13;
    int v14; // ecx
    int v15;
    int v16; // edx
    bool v17; // zf
    int result;
    int v19; // ebx
    int v20; // ebx
    int v21; // ebx
    int v22; // edi
    int v23; // edi
    int v24; // [esp+Ch] [ebp-2Ch]
    __int16 v25; // [esp+10h] [ebp-28h]
    __int16 v26; // [esp+12h] [ebp-26h]
    int YVelocity; // [esp+14h] [ebp-24h]
    int XVelocity; // [esp+18h] [ebp-20h]
    EntityRing* v29; // [esp+1Ch] [ebp-1Ch]
    int v30; // [esp+20h] [ebp-18h]
    int v31; // [esp+24h] [ebp-14h]
    int i; // [esp+30h] [ebp-8h]
    Entity* CollisionEntity; // [esp+34h] [ebp-4h]

    v2 = 0;
    Ring = (EntityRing*)CurrentEntity->XPos;
    v29 = Ring;
    XVelocity = Ring->Base.XVelocity;
    YVelocity = Ring->Base.YVelocity;
    if (Obj_Platform)
    {
        for (CollisionEntity = 0;
              RSDK_GetObjects(Obj_Platform->StructSize, &CollisionEntity) == 1;
              v2 |= 1 << Ring_CheckPlatformCollision((EntityPlatform*)CollisionEntity))
        {
            ;
        }
    }
    if (OBJ_Crate)
    {
        for (CollisionEntity = 0;
              RSDK_GetObjects(*(unsigned __int16 *)OBJ_Crate, &CollisionEntity) == 1;
        v2 |= 1 << Ring_CheckPlatformCollision((EntityPlatform*)CollisionEntity) )
    {
            ;
        }
    }
    if (OBJ_Bridge)
    {
        CollisionEntity = 0;
        while (RSDK_GetObjects(OBJ_Bridge->StructSize, &CollisionEntity) == 1)
        {
            v4 = Ring->Base.XPos;
            v30 = CollisionEntity[1].UpdateRangeY;
            if (v4 > v30)
            {
                v5 = CollisionEntity[1].Angle;
                if (v4 < v5 && Ring->Base.YVelocity >= 0)
                {
                    LOWORD(v24) = -1024;
                    v25 = 1024;
                    v6 = v4 - CollisionEntity[1].UpdateRangeY;
                    v7 = CollisionEntity[1].ScaleY;
                    v31 = v7;
                    if (v6 <= v7)
                    {
                        v8 = v6 << 7;
                    }
                    else
                    {
                        v7 = v5 - v31 - v30;
                        v8 = (v5 - v4) << 7;
                    }
                    v9 = (CollisionEntity[1].YVelocity * addr_sin512(v8 / v7) >> 9) - 0x80000;
                    v10 = v9 >> 16;
                    v12 = __OFSUB__(Ring->Base.YVelocity, 0x8000);
                    v11 = Ring->Base.YVelocity - 0x8000 < 0;
                    HIWORD(v24) = HIWORD(v9);
                    if (v11 ^ v12)
                    {
                        v26 = v10;
                        HIWORD(v24) = v10 - 8;
                    }
                    else
                    {
                        v26 = HIWORD(v24) + 8;
                    }
                    if (RSDK_CheckObjectCollisionTouch(CollisionEntity, &v24, Ring, &Obj_Ring->Hitbox) == 1)
                    {
                        v2 |= 2u;
                        Ring->Base.YPos = v9 + CollisionEntity->YPos - (Obj_Ring->Hitbox.Bottom << 16);
                    }
                }
            }
        }
    }
    for (i = 0;
          RSDK_GetObjects(OBJ_Spikes->StructSize, &i) == 1;
          v2 |= 1 << dword_EA793C(i, i + 128, Ring, &Obj_Ring->Hitbox, 1))
    {
        ;
    }
    if (OBJ_Ice)
    {
        for (CollisionEntity = 0;
              RSDK_GetObjects((unsigned __int16) * OBJ_Ice, &CollisionEntity) == 1;
        v2 |= 1 << dword_EA793C(CollisionEntity, &CollisionEntity[2].OnScreen, Ring, &Obj_Ring->Hitbox, 1) )
    {
            ;
        }
    }
    if (OBJ_BigSqueeze)
    {
        CollisionEntity = 0;
        while (RSDK_GetObjects((unsigned __int16) * OBJ_BigSqueeze, &CollisionEntity) == 1 )
    {
            v13 = Ring->Base.XPos;
            v14 = v2 | 8;
            if (v13 >= *((_DWORD*)OBJ_BigSqueeze + 51) + 0x200000)
                v14 = v2;
            v15 = *((_DWORD*)OBJ_BigSqueeze + 52) - 0x200000;
            v16 = v14 | 4;
            v12 = __OFSUB__(v13, v15);
            v17 = v13 == v15;
            v11 = v13 - v15 < 0;
            Ring = v29;
            if ((unsigned __int8)(v11 ^ v12) | v17 )
        v16 = v14;
            v2 = v16 | 2;
            if (v29->Base.YPos <= *((_DWORD*)OBJ_BigSqueeze + 55) - 0x80000)
                v2 = v16;
        }
    }
    result = (int)OBJ_SpikeCorridor;
    if (OBJ_SpikeCorridor)
    {
        CollisionEntity = 0;
        for (result = RSDK_GetObjects((unsigned __int16) * OBJ_SpikeCorridor, &CollisionEntity) ;
        result == 1;
        result = RSDK_GetObjects((unsigned __int16) * OBJ_SpikeCorridor, &CollisionEntity) )
    {
            if (*(_DWORD*)&CollisionEntity[1].Priority)
            {
                v19 = v2 | (1 << dword_EA793C(CollisionEntity, &CollisionEntity[1].Speed, Ring, &Obj_Ring->Hitbox, 1));
                v20 = v19 | (1 << dword_EA793C(CollisionEntity, &CollisionEntity[1].field_34, Ring, &Obj_Ring->Hitbox, 1));
                v21 = v20 | (1 << dword_EA793C(CollisionEntity, &CollisionEntity[1].field_3C, Ring, &Obj_Ring->Hitbox, 1));
                v2 = v21 | (1 << dword_EA793C(CollisionEntity, &CollisionEntity[1].field_44, Ring, &Obj_Ring->Hitbox, 1));
            }
        }
    }
    v22 = XVelocity;
    if (XVelocity <= 0)
    {
        if (!(v2 & 8))
        {
            result = addr_ObjectTileCollision(
                       Ring,
                       OBJ_ZONE->FGLayers,
                       C_RWALL,
                       (unsigned __int8)Ring->Base.CollisionPlane,
                 -HitboxLR,
                 0,
                 1);
            CHECK_XCOLLISION:
            if (result != 1)
                goto CHECK_YCOLLISION;
            goto HANDLE_XCOLLISION;
        }
    }
    else if (!(v2 & 4))
    {
        result = addr_ObjectTileCollision(
                   Ring,
                   OBJ_ZONE->FGLayers,
                   C_LWALL,
                   (unsigned __int8)Ring->Base.CollisionPlane,
               HitboxLR,
               0,
               1);
        goto CHECK_XCOLLISION;
    }
    HANDLE_XCOLLISION:
    Ring->Base.XVelocity = -v22;
    CHECK_YCOLLISION:
    v23 = YVelocity;
    if (YVelocity <= 0)
    {
        if (v2 & 0x10
          || (result = addr_ObjectTileCollision(
                         Ring,
                         OBJ_ZONE->FGLayers,
                         C_ROOF,
                         (unsigned __int8)Ring->Base.CollisionPlane,
                     0,
                     -HitboxTB,
                     1),
          result == 1) )
    {
            Ring->Base.YVelocity = -v23;
        }
    }
    else if (v2 & 2
           || (result = addr_ObjectTileCollision(
                          Ring,
                          OBJ_ZONE->FGLayers,
                          C_FLOOR,
                          (unsigned __int8)Ring->Base.CollisionPlane,
                        0,
                        HitboxTB,
                        1),
             result == 1) )
  {
        result = (v23 >> 2) - v23;
        Ring->Base.YVelocity = result;
        if (result > (signed int)-0x10000u )
      Ring->Base.YVelocity = -0x10000u;
    }
    return result;
}

int RingState_Normal()
{
    EntityRing* Ring;
    int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring_Collect();
    result = OBJ_ZONE->RingFrame;
    Ring->AnimData.FrameID = result;
    return result;
}

int RingState_Move()
{
    EntityRing* Ring;
    int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->XOffset = Ring->AmplitudeX * RSDK_Sin1024(*(_DWORD*)(CurrentEntity->XPos + 0x88) * OBJ_ZONE->Timer)
                  + Ring->Base.XPos;
    Ring->YOffset = Ring->AmplitudeY * RSDK_Sin1024(Ring->MoveSpeed * OBJ_ZONE->Timer) + Ring->Base.YPos;
    Ring_Collect();
    result = OBJ_ZONE->RingFrame;
    Ring->AnimData.FrameID = result;
    return result;
}

int RingState_Circular()
{
    EntityRing* Ring;
    int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->XOffset = Ring->AmplitudeX
                  * RSDK_Cos1024(
                      *(_DWORD*)(CurrentEntity->XPos + 0x88) * OBJ_ZONE->Timer
                    + 4 * *(_DWORD*)(CurrentEntity->XPos + 0x20))
                  + Ring->Base.XPos;
    Ring->YOffset = Ring->AmplitudeY * RSDK_Sin1024(Ring->MoveSpeed * OBJ_ZONE->Timer + 4 * Ring->Base.Angle)
                  + Ring->Base.YPos;
    Ring_Collect();
    result = OBJ_ZONE->RingFrame;
    Ring->AnimData.FrameID = result;
    return result;
}

int RingState_Track()
{
    EntityRing* Ring;
    Entity* v1;
    int v2; // ecx
    int v3;
    int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->XOffset += *(_DWORD*)(CurrentEntity->XPos + 0x10);
    Ring->YOffset += Ring->Base.YVelocity;
    v1 = RSDK_GetObjectByID(LOWORD(Ring->MoveSpeed));
    v2 = v1->XPos;
    if (Ring->Base.XVelocity <= 0)
    {
        if (Ring->XOffset >= v2)
            goto LABEL_6;
    }
    else if (Ring->XOffset <= v2)
    {
        goto LABEL_6;
    }
    Ring->XOffset = v2;
    LABEL_6:
    v3 = v1->YPos;
    if (Ring->Base.YVelocity <= 0)
    {
        if (Ring->YOffset < v3)
        {
            LABEL_10:
            Ring->YOffset = v3;
            goto LABEL_11;
        }
    }
    else if (Ring->YOffset > v3)
    {
        goto LABEL_10;
    }
    LABEL_11:
    Ring_Collect();
    result = OBJ_ZONE->RingFrame;
    Ring->AnimData.FrameID = result;
    return result;
}

int RingState_Path()
{
    EntityRing* Ring;
    int Timer; // ecx
    int Amplitude;
    int XOffset1; // ecx
    int YOffset2;
    int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Timer = OBJ_ZONE->Timer << 7;
    if ((BYTE2(Timer) & 1) == *(_BYTE*)(CurrentEntity->XPos + 0x4E))
    {
        Amplitude = Ring->AmplitudeY;
        Ring->XOffset = Ring->Base.XPos + ((unsigned __int16)Timer* Ring->AmplitudeX >> 6) -(Ring->AmplitudeX << 15);
        XOffset1 = (unsigned __int16)Timer* Amplitude >> 6;
        YOffset2 = Amplitude << 15;
    }
    else
    {
        Ring->XOffset = Ring->Base.XPos + (Ring->AmplitudeX << 15) - ((unsigned __int16)Timer* Ring->AmplitudeX >> 6);
        YOffset2 = (unsigned __int16)Timer* Ring->AmplitudeY >> 6;
        XOffset1 = Ring->AmplitudeY << 15;
    }
    Ring->YOffset = Ring->Base.YPos + XOffset1 - YOffset2;
    Ring_Collect();
    result = OBJ_ZONE->RingFrame;
    Ring->AnimData.FrameID = result;
    return result;
}

signed int RingState_Bounce()
{
    EntityRing* Ring;
    int YVelocity;
    int CollectDelay;
    unsigned int v4;
    signed int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->Base.YVelocity += 6144;
    YVelocity = Ring->Base.YVelocity;
    Ring->Base.XPos += Ring->Base.XVelocity;
    Ring->Base.YPos += YVelocity;
    if (Ring->MoveType)
    {
        if (YVelocity > 0
          && addr_ObjectTileCollision(
               Ring,
               OBJ_ZONE->FGLayers,
               C_FLOOR,
               (unsigned __int8)Ring->Base.CollisionPlane,
           0,
           0x80000,
           1) == 1 )
    {
            YVelocity = (Ring->Base.YVelocity >> 2) - Ring->Base.YVelocity;
            if (YVelocity > (signed int)0xFFFF0000 )
        YVelocity = 0xFFFF0000;
            Ring->Base.YVelocity = YVelocity;
        }
    }
    else
    {
        Ring_CheckObjectCollisions(0x80000, 0x80000);
    }
    RSDK_ProcessAnimation(&Ring->AnimData);
    CollectDelay = ++Ring->Timer;
    if (!(CollectDelay & 7))
    {
        v4 = (unsigned __int16)Ring->AnimData.AnimationSpeed;
        if (v4 > 0x40)
            Ring->AnimData.AnimationSpeed = v4 - 16;
    }
    if (CollectDelay > 63)
        Ring_Collect();
    result = Ring->Timer;
    if (result > 255)
        return RSDK_ResetObjectEntity(&Ring->Base, 0, 0);
    if (result >= 0xF0)
        Ring->Base.Alpha -= 16;
    return result;
}

int RingState_Grow()
{
    EntityRing* Ring;
    int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->Base.XPos += *(_DWORD*)(CurrentEntity->XPos + 16);
    Ring->Base.YVelocity += 0x1800;
    Ring->Base.YPos += Ring->Base.YVelocity;
    result = RSDK_ProcessAnimation(&Ring->AnimData);
    Ring->Base.ScaleX += 16;
    Ring->Base.ScaleY += 16;
    if (++Ring->Timer > 64)
        result = RSDK_ResetObjectEntity(&Ring->Base, 0, 0);
    return result;
}

int RingState_Big()
{
    EntityRing* Ring;
    unsigned int AnimationSpeed;
    int ScaleX;
    int ScaleY;
    int v4;
    int SineAngle;
    int v7;
    int v8;
    int v9;
    int v10;
    EntityRing* SpawnedRing;
    unsigned int v13;
    int result;
    int CollectDelay;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->Base.XPos += *(_DWORD*)(CurrentEntity->XPos + 16);
    AnimationSpeed = (unsigned __int16)Ring->AnimData.AnimationSpeed;
    Ring->Base.YVelocity += 0x1200;
    Ring->Base.YPos += Ring->Base.YVelocity;
    Ring->Base.Angle += AnimationSpeed >> 6;
    if (Ring->Timer <= 240)
    {
        SineAngle = RSDK_Sin256(Ring->Base.Angle);
        SineAngle = Ring->Base.Angle;
        Ring->Base.ScaleX = (-SineAngle >> 1) + 384;
        v4 = (RSDK_Sin256(SineAngle) >> 1) + 384;
    }
    else
    {
        ScaleX = Ring->Base.ScaleX;
        ScaleY = Ring->Base.ScaleY;
        if (ScaleX > ScaleY)
        {
            Ring->Base.ScaleX = ScaleX + 8;
            Ring->Base.ScaleY = ScaleY - (ScaleY >> 3);
            goto SKIP_SCALEY;
        }
        v4 = ScaleY + 8;
        Ring->Base.ScaleX = ScaleX - (ScaleX >> 3);
    }
    Ring->Base.ScaleY = v4;
    SKIP_SCALEY:
    v7 = Ring->Base.ScaleX << 11;
    v8 = Ring->Base.ScaleY << 11;
    Ring_CheckObjectCollisions(Ring->Base.ScaleX << 11, v8);
    if (!(Ring->Base.Angle & 0xF))              // Ring Sparkles
    {
        v9 = Ring->Base.YPos + Random(-v8, v8);
        v10 = Random(-v7, v7);
        SpawnedRing = (EntityRing*)RSDK_SpawnEntity((RSDK_OBJECT*)Obj_Ring->StructSize, 0, Ring->Base.XPos + v10, v9);
        SpawnedRing->State = RingState_Sparkle;
        SpawnedRing->StateDraw = RingStateDraw_Sparkle;
        SpawnedRing->Base.Priority = 2;
        SpawnedRing->Base.Visible = 0;
        RSDK_SetSpriteAnimation(Obj_Ring->SpriteIndex, LOWORD(Ring->SparkleType2) + 2, &SpawnedRing->AnimData, 1, 0);
        v13 = SpawnedRing->AnimData.FrameCount;
        if (SpawnedRing->AnimData.AnimationID == 2)
        {
            SpawnedRing->Base.Alpha = 224;
            v13 >>= 1;
        }
        SpawnedRing->MaxFrameCount = v13 - 1;
        SpawnedRing->AnimData.AnimationSpeed = 4;
        Ring->SparkleType2 = (Ring->SparkleType2 + 1) % 3;
    }
    result = RSDK_ProcessAnimation(&Ring->AnimData);
    CollectDelay = ++Ring->Timer;
    if (!(CollectDelay & 7))
    {
        result = (unsigned __int16)Ring->AnimData.AnimationSpeed;
        if ((unsigned __int16)result > 0x80u )
    {
            result -= 8;
            Ring->AnimData.AnimationSpeed = result;
        }
    }
    if (CollectDelay > 71)
        result = Ring_Collect();
    if (Ring->Timer > 255)
        result = RSDK_ResetObjectEntity(&Ring->Base, 0, 0);
    return result;
}

int RingState_Attract()
{
    EntityRing* Ring;
    EntityPlayer* Player; // ecx
    int XPos; // edx
    int XVel;
    int XVelocity;
    int YVel;
    int YPos; // edi
    int YVelocity;
    int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Player = *(EntityPlayer**)(CurrentEntity->XPos + 120);
    if (Player->Shield == 4)
    {
        XPos = Ring->Base.XPos;
        XVel = Ring->Base.XVelocity;
        if (Ring->Base.XPos <= Player->Base.XPos)
        {
            if (XVel >= 0)
                XVelocity = XVel + 0x3000;
            else
                XVelocity = XVel + 0xC000;
        }
        else if (XVel <= 0)
        {
            XVelocity = XVel - 0x3000;
        }
        else
        {
            XVelocity = XVel - 0xC000;
        }
        Ring->Base.XVelocity = XVelocity;
        YVel = Ring->Base.YVelocity;
        YPos = Ring->Base.YPos;
        if (YPos <= Player->Base.YPos)
        {
            if (YVel >= 0)
                YVelocity = YVel + 0x3000;
            else
                YVelocity = YVel + 0xC000;
        }
        else if (YVel <= 0)
        {
            YVelocity = YVel - 0x3000;
        }
        else
        {
            YVelocity = YVel - 0xC000;
        }
        Ring->Base.YVelocity = YVelocity;
        Ring->Base.XPos = XPos + Ring->Base.XVelocity;
        Ring->Base.YPos = YPos + Ring->Base.YVelocity;
    }
    else
    {
        Ring->State = RingState_Bounce;
        Ring->AnimData.AnimationSpeed = 128;
        Ring->Base.Alpha = 256;
        Ring->Base.InkEffect = 2;
    }
    Ring_Collect();
    result = OBJ_ZONE->RingFrame;
    Ring->AnimData.FrameID = result;
    return result;
}

int RingState_Sparkle()
{
    EntityRing* Ring;
    int v1;
    int result;

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->Base.XPos += *(_DWORD*)(CurrentEntity->XPos + 16);
    Ring->Base.YPos += Ring->Base.YVelocity;
    v1 = Ring->Timer;
    if (v1 <= 0)
    {
        Ring->Base.Visible = 1;
        RSDK_ProcessAnimation(&Ring->AnimData);
        result = Ring->AnimData.FrameID;
        if (result >= Ring->MaxFrameCount)
            result = RSDK_ResetObjectEntity(&Ring->Base, 0, 0);
    }
    else
    {
        result = v1 - 1;
        Ring->Base.Visible = 0;
        Ring->Timer = result;
    }
    return result;
}

int RingStateDraw_Normal()
{
    EntityRing* Ring; // ecx

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->Base.Direction = *(_DWORD*)(CurrentEntity->XPos + 0x98) > 8;
    return RSDK_DrawSprite(&Ring->AnimData, 0, 0);
}

int RingStateDraw_Oscillating()
{
    EntityRing* Ring; // ecx

    Ring = (EntityRing*)CurrentEntity->XPos;
    Ring->Base.Direction = *(_DWORD*)(CurrentEntity->XPos + 0x98) > 8;// if FrameID > 8; FlipX = true
    return RSDK_DrawSprite(&Ring->AnimData, (Position*)&Ring->XOffset, 0);
}

int RingStateDraw_Sparkle()
{
    EntityRing* Ring;

    Ring = (EntityRing*)CurrentEntity->XPos;
    if (*(_DWORD*)(CurrentEntity->XPos + 0x24) == 224)
    {
        Ring->AnimData.FrameID += 16;
        Ring->Base.InkEffect = INK_ADD;
        RSDK_DrawSprite(&Ring->AnimData, 0, 0);
        Ring->Base.InkEffect = 0;
        Ring->AnimData.FrameID -= 16;
    }
    return RSDK_DrawSprite(&Ring->AnimData, 0, 0);
}

void Ring_GetAttributes()
{
    RSDK_GetAttribute(RSDK_ATTRIBUTETYPES_VAR, "type", Obj_Ring->StructSize, 0x60);
    RSDK_GetAttribute(RSDK_ATTRIBUTETYPES_VAR, "planeFilter", Obj_Ring->StructSize, 0x64);
    RSDK_GetAttribute(RSDK_ATTRIBUTETYPES_VAR, "moveType", Obj_Ring->StructSize, 0x7C);
    RSDK_GetAttribute(RSDK_ATTRIBUTETYPES_VECTOR2, "amplitude", Obj_Ring->StructSize, 0x80);
    RSDK_GetAttribute(RSDK_ATTRIBUTETYPES_VAR, "speed", Obj_Ring->StructSize, 0x88);
    RSDK_GetAttribute(RSDK_ATTRIBUTETYPES_INT32, "angle", Obj_Ring->StructSize, 0x20);
}