
enum AttributeTypes {
	ATTRIBUTE_UINT8,
	ATTRIBUTE_UINT16,
	ATTRIBUTE_UINT32,
	ATTRIBUTE_INT8,
	ATTRIBUTE_INT16,
	ATTRIBUTE_INT32,
	ATTRIBUTE_VAR,
	ATTRIBUTE_BOOL,
	ATTRIBUTE_STRING,
	ATTRIBUTE_VECTOR2,
	ATTRIBUTE_VECTOR3,
	ATTRIBUTE_COLOUR,
};

enum ActiveFlags {
	ACTIVE_NEVER3 = -1,
	ACTIVE_NEVER,
	ACTIVE_ALWAYS,
	ACTIVE_NORMAL,
	ACTIVE_PAUSED,
	ACTIVE_BOUNDS,
	ACTIVE_XBOUNDS,
	ACTIVE_YBOUNDS,
	ACTIVE_BOUNDS2,
};

class Entity
{
    int XPos;
    int YPos;
    int ScaleX;
    int ScaleY;
    int XVelocity;
    int YVelocity;
    int UpdateRangeX;
    int UpdateRangeY;
    int Angle;
    int Alpha;
    int Rotation;
    int Speed;
    int field_30;
    short field_34;
    ushort ObjectID;
    int OnScreen;
    int field_3C;
    int TileCollisions;
    int field_44;
    int Grounded;
    byte Priority;
    byte Filter;
    byte Direction;
    byte DrawOrder;
    byte field_50;
    byte CollisionPlane;
    byte CollisionMode;
    byte DrawFX;
    byte InkEffect;
    byte Visible;
    byte field_56;
    byte field_57;
};