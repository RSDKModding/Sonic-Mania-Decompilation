#ifndef OBJ_RING
#define OBJ_RING

#include "../ManiaGlobals.h"

class Ring : Object {
  byte StructSize;
  byte field_2;
  byte field_3;
  Hitbox Hitbox;
  int field_C;
  short SpriteIndex;
  short field_12;
};

class EntityRing : Entity {
  void *State;
  void *StateDraw;
  int Type;
  int PlaneFilter;
  int RingAmount;
  int Timer;
  int MaxFrameCount;
  int SparkleType2;
  Entity *StoredPlayer;
  int MoveType;
  int AmplitudeX;
  int AmplitudeY;
  int MoveSpeed;
  int XOffset;
  int YOffset;
  EntityAnimationData AnimData;
};

void Ring_Update();
void Ring_Draw();
void Ring_Setup(int Subtype);
void Ring_StageLoad();
int DebugMode_SpawnRing();
int DebugMode_DrawRing();
int Ring_Collect();
uint Player_LoseRings(EntityPlayer *Player, signed int Rings, byte CollisionPlane);
int Player_LoseHyperRings(EntityPlayer *Player, int Rings, byte CollisionPlane);
byte Ring_FakeLoseRings(int *a1, signed int a2, byte a3);
int Ring_CheckPlatformCollision(EntityPlatform *Platform);
int Ring_CheckObjectCollisions(int HitboxLR, int HitboxTB);
int RingState_Normal();
int RingState_Move();
int RingState_Circular();
int RingState_Track();
int RingState_Path();
int RingState_Bounce();
int RingState_Grow();
int RingState_Big();
int RingState_Attract();
int RingState_Sparkle();
int RingStateDraw_Normal();
int RingStateDraw_Oscillating();
int RingStateDraw_Sparkle();
void Ring_GetAttributes();

#endif