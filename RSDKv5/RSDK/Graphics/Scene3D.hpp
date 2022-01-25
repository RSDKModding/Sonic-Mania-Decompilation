#ifndef SCENE3D_H
#define SCENE3D_H

#define SCENE3D_MAX (0x20)
#define MODEL_MAX   (0x100)
#define SCENE3D_VERT_MAX (0x4000)

enum Scene3DDrawTypes {
    S3D_FLATCLR_WIREFRAME               = 0x0,
    S3D_FLATCLR                         = 0x1,
    S3D_UNKNOWN_2                       = 0x2,
    S3D_UNKNOWN_3                       = 0x3,
    S3D_FLATCLR_SHADED_WIREFRAME        = 0x4,
    S3D_FLATCLR_SHADED                  = 0x5,
    S3D_FLATCLR_SHADED_BLENDED          = 0x6,
    S3D_FLATCLR_SCREEN_WIREFRAME        = 0x7,
    S3D_FLATCLR_SCREEN                  = 0x8,
    S3D_FLATCLR_SHADED_SCREEN_WIREFRAME = 0x9,
    S3D_FLATCLR_SHADED_SCREEN           = 0xA,
    S3D_FLATCLR_SHADED_BLENDED_SCREEN   = 0xB,
};

struct ScanEdge {
    int32 start;
    int32 end;
    int32 startR;
    int32 endR;
    int32 startG;
    int32 endG;
    int32 startB;
    int32 endB;
};

struct Matrix {
    int32 values[4][4];
};

struct ModelVertex {
    int32 x;
    int32 y;
    int32 z;
    int32 nx;
    int32 ny;
    int32 nz;
};

struct TexCoord {
    int32 x;
    int32 y;
};

struct Model {
    uint32 hash[4];
    ModelVertex *vertices;
    TexCoord *texCoords;
    Colour *colours;
    uint16 *indices;
    uint16 vertCount;
    uint16 indexCount;
    uint16 frameCount;
    uint8 flags;
    uint8 faceVertCount;
    uint8 scope;
};

struct Scene3DVertex {
    int32 x;
    int32 y;
    int32 z;
    int32 nx;
    int32 ny;
    int32 nz;
    int32 tx;
    int32 ty;
    uint32 colour;
};

struct FaceBufferEntry {
    int32 depth;
    int32 index;
};

struct Scene3D {
    RETRO_HASH(hash);
    Scene3DVertex *vertices;
    Scene3DVertex *normals;
    FaceBufferEntry *faceBuffer;
    uint8 *faceVertCounts;
    int32 projectionX;
    int32 projectionY;
    int32 diffuseX;
    int32 diffuseY;
    int32 diffuseZ;
    int32 diffuseIntensityX;
    int32 diffuseIntensityY;
    int32 diffuseIntensityZ;
    int32 specularIntensityX;
    int32 specularIntensityY;
    int32 specularIntensityZ;
    uint16 vertLimit;
    uint16 vertexCount;
    uint16 faceCount;
    uint8 drawMode;
    uint8 scope;
};

extern Model modelList[MODEL_MAX];
extern Scene3D scene3DList[SCENE3D_MAX];

extern ScanEdge scanEdgeBuffer[SCREEN_YSIZE * 2];

void ProcessScanEdge(int32 x1, int32 y1, int32 x2, int32 y2);
void ProcessScanEdgeClr(uint32 c1, uint32 c2, int32 x1, int32 y1, int32 x2, int32 y2);

void setIdentityMatrix(Matrix *matrix);
void matrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB);
void matrixTranslateXYZ(Matrix *Matrix, int32 x, int32 y, int32 z, bool32 setIdentity);
void matrixScaleXYZ(Matrix *matrix, int32 scaleX, int32 scaleY, int32 scaleZ);
void matrixRotateX(Matrix *matrix, int16 angle);
void matrixRotateY(Matrix *matrix, int16 angle);
void matrixRotateZ(Matrix *matrix, int16 angle);
void matrixRotateXYZ(Matrix *matrix, int16 rotationX, int16 rotationY, int16 rotationZ);
void matrixInverse(Matrix *dest, Matrix *matrix);
void matrixCopy(Matrix *matDst, Matrix *matSrc);

uint16 LoadMesh(const char *filepath, Scopes scope);
uint16 Create3DScene(const char *name, uint16 faceCnt, Scopes scope);
inline void Prepare3DScene(uint16 sceneID)
{
    if (sceneID < SCENE3D_MAX) {
        Scene3D *scn     = &scene3DList[sceneID];
        scn->vertexCount = 0;
        scn->faceCount   = 0;
        memset(scn->vertices, 0, sizeof(Scene3DVertex) * scn->vertLimit);
        memset(scn->normals, 0, sizeof(Scene3DVertex) * scn->vertLimit);
        memset(scn->faceVertCounts, 0, sizeof(uint8) * scn->vertLimit);
        memset(scn->faceBuffer, 0, sizeof(FaceBufferEntry) * scn->vertLimit);
    }
}

inline void SetMeshAnimation(uint16 model, RSDK::Animator *animator, int16 speed, uint8 loopIndex, bool32 forceApply, uint16 frameID)
{
    if (model >= MODEL_MAX) {
        if (animator)
            animator->framePtrs = 0;
        return;
    }
    if (!animator)
        return;

    if (animator->animationID == model && !forceApply)
        return;
    animator->framePtrs          = (RSDK::SpriteFrame *)1;
    animator->timer  = 0;
    animator->frameID         = frameID;
    animator->frameCount      = modelList[model].frameCount;
    animator->speed  = speed;
    animator->prevAnimationID = animator->animationID;
    animator->frameDuration      = 0x100;
    animator->loopIndex       = loopIndex;
    animator->animationID     = model;
}
inline void SetDiffuseColour(uint16 sceneID, uint8 x, uint8 y, uint8 z)
{
    if (sceneID < SCENE3D_MAX) {
        Scene3D *scn  = &scene3DList[sceneID];
        scn->diffuseX = x;
        scn->diffuseY = y;
        scn->diffuseZ = z;
    }
}
inline void SetDiffuseIntensity(uint16 sceneID, uint8 x, uint8 y, uint8 z)
{
    if (sceneID < SCENE3D_MAX) {
        Scene3D *scn  = &scene3DList[sceneID];
        scn->diffuseIntensityX = x;
        scn->diffuseIntensityY = y;
        scn->diffuseIntensityZ = z;
    }
}
inline void SetSpecularIntensity(uint16 sceneID, uint8 x, uint8 y, uint8 z)
{
    if (sceneID < SCENE3D_MAX) {
        Scene3D *scn   = &scene3DList[sceneID];
        scn->specularIntensityX = x;
        scn->specularIntensityY = y;
        scn->specularIntensityZ = z;
    }
}
void AddModelToScene(uint16 animID, uint16 sceneID, uint8 drawMode, Matrix *matWorld, Matrix *matView, colour colour);
void AddMeshFrameToScene(uint16 animID, uint16 sceneID, RSDK::Animator *animator, uint8 drawMode, Matrix *matWorld, Matrix *matView, colour colour);
void Draw3DScene(uint16 sceneID);

#if RETRO_HARDWARE_RENDER
struct MatrixF {
    float values[4][4];
};
void matrixItoF(Matrix* matrixI, MatrixF* matrixF);
void matrixFtoI(MatrixF *matrixF, Matrix *matrixI);

#endif

#endif
