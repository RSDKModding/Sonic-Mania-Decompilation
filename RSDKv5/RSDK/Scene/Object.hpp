#ifndef OBJECT_H
#define OBJECT_H

#if RETRO_USE_MOD_LOADER
#include <functional>
#endif

namespace RSDK
{

#define RSDK_SIGNATURE_OBJ (0x4A424F) // "OBJ"

#define OBJECT_COUNT (0x400)

// 0x800 scene objects, 0x40 reserved ones, and 0x100 spare slots for creation
#define RESERVE_ENTITY_COUNT (0x40)
#define TEMPENTITY_COUNT     (0x100)
#define SCENEENTITY_COUNT    (0x800)
#define ENTITY_COUNT         (RESERVE_ENTITY_COUNT + SCENEENTITY_COUNT + TEMPENTITY_COUNT)
#define TEMPENTITY_START     (ENTITY_COUNT - TEMPENTITY_COUNT)

#define TYPE_COUNT        (0x100)
#define EDITABLEVAR_COUNT (0x100)
#define TYPEGROUP_COUNT   (0x104)

#define FOREACH_STACK_COUNT (0x400)

// Used for DefaultObject & DevOutput
#define RSDK_THIS(class) Entity##class *self = (Entity##class *)sceneInfo.entity

enum StaticVariableTypes {
    SVAR_UINT8,
    SVAR_UINT16,
    SVAR_UINT32,
    SVAR_INT8,
    SVAR_INT16,
    SVAR_INT32,
    SVAR_BOOL,
    SVAR_POINTER,
    SVAR_VECTOR2,
    SVAR_STRING,
    SVAR_ANIMATOR,
    SVAR_HITBOX,
    SVAR_SPRITEFRAME,
};

enum TypeGroups {
    GROUP_ALL = 0,

    GROUP_CUSTOM0 = TYPE_COUNT,
    GROUP_CUSTOM1,
    GROUP_CUSTOM2,
    GROUP_CUSTOM3,
};

enum VariableTypes {
    VAR_UINT8,
    VAR_UINT16,
    VAR_UINT32,
    VAR_INT8,
    VAR_INT16,
    VAR_INT32,
    VAR_ENUM,
    VAR_BOOL,
    VAR_STRING,
    VAR_VECTOR2,
    VAR_FLOAT, // Not actually used in Sonic Mania so it's just an assumption, but this is the only thing that'd fit the 32 bit limit and make sense
    VAR_COLOR,
};

enum ActiveFlags {
    ACTIVE_NEVER,   // never update
    ACTIVE_ALWAYS,  // always update (even if paused/frozen)
    ACTIVE_NORMAL,  // always update (unless paused/frozen)
    ACTIVE_PAUSED,  // update if paused
    ACTIVE_BOUNDS,  // update if in x & y bounds
    ACTIVE_XBOUNDS, // update only if in x bounds (y bounds dont matter)
    ACTIVE_YBOUNDS, // update only if in y bounds (x bounds dont matter)
    ACTIVE_RBOUNDS, // update based on radius boundaries (updateRange.x == radius)

    // Not really even a real active value, but some objects set their active states to this so here it is I suppose
    ACTIVE_DISABLED = 0xFF,
};

enum DefaultObjects {
    TYPE_DEFAULTOBJECT = 0,
#if RETRO_REV02
    TYPE_DEVOUTPUT,
#endif

    TYPE_DEFAULT_COUNT, // max
};

struct Object {
    int16 classID;
    uint8 active;
};

struct Entity {
    Vector2 position;
    Vector2 scale;
    Vector2 velocity;
    Vector2 updateRange;
    int32 angle;
    int32 alpha;
    int32 rotation;
    int32 groundVel;
    int32 depth;
    uint16 group;
    uint16 classID;
    bool32 inBounds;
    bool32 isPermanent;
    bool32 tileCollisions;
    bool32 interaction;
    bool32 onGround;
    uint8 active;
#if RETRO_REV02
    uint8 filter;
#endif
    uint8 direction;
    uint8 drawOrder;
    uint8 collisionLayers;
    uint8 collisionPlane;
    uint8 collisionMode;
    uint8 drawFX;
    uint8 inkEffect;
    uint8 visible;
    uint8 activeScreens;
};

struct EntityBase : Entity {
    void *data[0x100];
};

struct ObjectClass {
    RETRO_HASH_MD5(hash);

    // Events
#if RETRO_USE_MOD_LOADER // using std::function makes it easier to use stuff like lambdas
    std::function<void()> update;
    std::function<void()> lateUpdate;
    std::function<void()> staticUpdate;
    std::function<void()> draw;
    std::function<void(void *)> create;
    std::function<void()> stageLoad;
    std::function<void()> editorDraw;
    std::function<void()> editorLoad;
    std::function<void()> serialize;
#else
    void (*update)();
    void (*lateUpdate)();
    void (*staticUpdate)();
    void (*draw)();
    void (*create)(void *);
    void (*stageLoad)();
    void (*editorDraw)();
    void (*editorLoad)();
    void (*serialize)();
#endif

    // Classes
    Object **staticVars;
    int32 entityClassSize;
    int32 staticClassSize;

#if RETRO_USE_MOD_LOADER
    ObjectClass *inherited;
#endif
};

struct EditableVarInfo {
    RETRO_HASH_MD5(hash);
    int32 offset;
    int32 active;
    uint8 type;
};

struct ForeachStackInfo {
    int32 id;
};

struct TypeGroupList {
    int32 entryCount;
    uint16 entries[ENTITY_COUNT];
};

extern ObjectClass objectClassList[OBJECT_COUNT];
extern int32 objectClassCount;

// Loaded Global Objects
extern int32 globalObjectCount;
extern int32 globalObjectIDs[OBJECT_COUNT];

// Loaded Stage Objects (includes Globals if "loadGlobals" is enabled)
extern int32 stageObjectIDs[OBJECT_COUNT];

extern EntityBase objectEntityList[ENTITY_COUNT];

extern EditableVarInfo *editableVarList;
extern int32 editableVarCount;

extern ForeachStackInfo foreachStackList[FOREACH_STACK_COUNT];
extern ForeachStackInfo *foreachStackPtr;

extern TypeGroupList typeGroups[TYPEGROUP_COUNT];

extern bool32 validDraw;

void RegisterObject(Object **staticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize, void (*update)(),
                    void (*lateUpdate)(), void (*staticUpdate)(), void (*draw)(), void (*create)(void *), void (*stageLoad)(),
                    void (*editorDraw)(), void (*editorLoad)(), void (*serialize)());

#if RETRO_USE_MOD_LOADER
void RegisterObject_STD(Object **staticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize, std::function<void()> update,
                        std::function<void()> lateUpdate, std::function<void()> staticUpdate, std::function<void()> draw,
                        std::function<void(void *)> create, std::function<void()> stageLoad, std::function<void()> editorDraw,
                        std::function<void()> editorLoad, std::function<void()> serialize);
#endif

#if RETRO_REV02
void RegisterStaticVariables(void **varClass, const char *name, uint32 classSize);
#endif

void LoadStaticVariables(uint8 *classPtr, uint32 *hash, int32 readOffset);

#define RSDK_EDITABLE_VAR(object, type, var) RSDK.SetEditableVar(type, #var, (uint8)object->classID, offsetof(Entity##object, var))

// classID isn't used AND is a uint8, how strange
// assuming classID would be used in-editor (it is in RetroED2, but not sure about official RSDK SDK)
// not sure why it's a uint8, given the original value is a uint16, so there's some small warnings about that
inline void SetEditableVar(uint8 type, const char *name, uint8 classID, int32 offset)
{
    if (editableVarCount < EDITABLEVAR_COUNT - 1) {
        EditableVarInfo *var = &editableVarList[editableVarCount];

        GEN_HASH_MD5(name, var->hash);
        var->type   = type;
        var->offset = offset;
        var->active = true;

        editableVarCount++;
    }
}

inline void SetActiveVariable(int32 classID, const char *name)
{
    // Editor-Only function
}
inline void AddEnumVariable(const char *name)
{
    // Editor-Only function
}

void InitObjects();
void ProcessObjects();
void ProcessPausedObjects();
void ProcessFrozenObjects();
void ProcessObjectDrawLists();

uint16 FindObject(const char *name);

inline Entity *GetEntity(uint16 slot) { return &objectEntityList[slot < ENTITY_COUNT ? slot : (ENTITY_COUNT - 1)]; }
inline int32 GetEntitySlot(EntityBase *entity) { return (int32)(entity - objectEntityList < ENTITY_COUNT ? entity - objectEntityList : 0); }
int32 GetEntityCount(uint16 classID, bool32 isActive);

void ResetEntityPtr(Entity *entity, uint16 classID, void *data);
void ResetEntitySlot(uint16 slot, uint16 classID, void *data);
Entity *CreateEntity(uint16 classID, void *data, int32 x, int32 y);

inline void CopyEntity(void *destEntity, void *srcEntity, bool32 clearSrcEntity)
{
    if (destEntity && srcEntity) {
        memcpy(destEntity, srcEntity, sizeof(EntityBase));

        if (clearSrcEntity)
            memset(srcEntity, 0, sizeof(EntityBase));
    }
}

bool32 GetActiveEntities(uint16 group, Entity **entity);
bool32 GetEntities(uint16 classID, Entity **entity);

inline void BreakForeachLoop() { --foreachStackPtr; }

// CheckPosOnScreen but if range is NULL it'll use entity->updateRange
bool32 CheckOnScreen(Entity *entity, Vector2 *range);
// Checks if a position is on screen & within range
bool32 CheckPosOnScreen(Vector2 *position, Vector2 *range);

void ClearStageObjects();

} // namespace RSDK

#endif // !OBJECT_H
