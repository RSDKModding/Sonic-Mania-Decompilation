#include "RetroEngine.hpp"

Model modelList[MODEL_MAX];
Scene3D scene3DList[SCENE3D_MAX];

enum ModelFlags {
    MODEL_USENORMALS = 1,
    MODEL_USETEXTURES = 2,
    MODEL_USECOLOURS = 4,
};

void setIdentityMatrix(Matrix *matrix)
{
    matrix->values[0][0] = 0x100;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = 0x100;
    matrix->values[1][2] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = 0;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = 0x100;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB)
{
    int output[16];

    for (int i = 0; i < 0x10; ++i) {
        uint rowA = i / 4;
        uint rowB = i % 4;
        output[i] = (matrixA->values[rowA][3] * matrixB->values[3][rowB] >> 8) + (matrixA->values[rowA][2] * matrixB->values[2][rowB] >> 8)
                    + (matrixA->values[rowA][1] * matrixB->values[1][rowB] >> 8) + (matrixA->values[rowA][0] * matrixB->values[0][rowB] >> 8);
    }

    for (int i = 0; i < 0x10; ++i) dest->values[i / 4][i % 4] = output[i];
}
void matrixTranslateXYZ(Matrix *matrix, int x, int y, int z, bool32 setIdentity)
{
    if (setIdentity) {
        matrix->values[0][0] = 0x100;
        matrix->values[0][1] = 0;
        matrix->values[0][2] = 0;
        matrix->values[0][3] = 0;
        matrix->values[1][0] = 0;
        matrix->values[1][1] = 0x100;
        matrix->values[1][2] = 0;
        matrix->values[1][3] = 0;
        matrix->values[2][0] = 0;
        matrix->values[2][1] = 0;
        matrix->values[2][2] = 0x100;
        matrix->values[2][3] = 0;
        matrix->values[3][3] = 0x100;
    }
    matrix->values[3][0] = x >> 8;
    matrix->values[3][1] = y >> 8;
    matrix->values[3][2] = z >> 8;
}
void matrixScaleXYZ(Matrix *matrix, int scaleX, int scaleY, int scaleZ)
{
    matrix->values[0][0] = scaleX;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = scaleY;
    matrix->values[1][2] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = 0;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = scaleZ;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateX(Matrix *matrix, short rotationX)
{
    int sine             = sinVal1024[rotationX & 0x3FF] >> 2;
    int cosine           = cosVal1024[rotationX & 0x3FF] >> 2;
    matrix->values[0][0] = 0x100;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = cosine;
    matrix->values[1][2] = sine;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = 0;
    matrix->values[2][1] = -sine;
    matrix->values[2][2] = cosine;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateY(Matrix *matrix, short rotationY)
{
    int sine             = sinVal1024[rotationY & 0x3FF] >> 2;
    int cosine           = cosVal1024[rotationY & 0x3FF] >> 2;
    matrix->values[0][0] = cosine;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = sine;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = 0x100;
    matrix->values[1][2] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = -sine;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = cosine;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateZ(Matrix *matrix, short rotationZ)
{
    int sine             = sinVal1024[rotationZ & 0x3FF] >> 2;
    int cosine           = cosVal1024[rotationZ & 0x3FF] >> 2;
    matrix->values[0][0] = cosine;
    matrix->values[0][1] = 0;
    matrix->values[0][2] = sine;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = 0;
    matrix->values[1][1] = 0x100;
    matrix->values[1][2] = 0;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = -sine;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = cosine;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixRotateXYZ(Matrix *matrix, short rotationX, short rotationY, short rotationZ)
{
    int sinX = sinVal1024[rotationX & 0x3FF] >> 2;
    int cosX = cosVal1024[rotationX & 0x3FF] >> 2;
    int sinY = sinVal1024[rotationY & 0x3FF] >> 2;
    int cosY = cosVal1024[rotationY & 0x3FF] >> 2;
    int sinZ = sinVal1024[rotationZ & 0x3FF] >> 2;
    int cosZ = cosVal1024[rotationZ & 0x3FF] >> 2;

    matrix->values[0][0] = (cosZ * cosY >> 8) + (sinZ * (sinY * sinX >> 8) >> 8);
    matrix->values[0][1] = (sinZ * cosY >> 8) - (cosZ * (sinY * sinX >> 8) >> 8);
    matrix->values[0][2] = sinY * cosX >> 8;
    matrix->values[0][3] = 0;
    matrix->values[1][0] = sinZ * -cosX >> 8;
    matrix->values[1][1] = cosZ * cosX >> 8;
    matrix->values[1][2] = sinX;
    matrix->values[1][3] = 0;
    matrix->values[2][0] = (sinZ * (cosY * sinX >> 8) >> 8) - (cosZ * sinY >> 8);
    matrix->values[2][1] = (sinZ * -sinY >> 8) - (cosZ * (cosY * sinX >> 8) >> 8);
    matrix->values[2][2] = cosY * cosX >> 8;
    matrix->values[2][3] = 0;
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 0x100;
}
void matrixInverse(Matrix *dest, Matrix *matrix)
{
    double inv[16], det;
    double m[16];
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            m[(y << 2) + x] = matrix->values[y][x] / 256.0;
        }
    }

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return;

    det = 1.0 / det;

    for (int i = 0; i < 0x10; ++i) inv[i] = (int)((inv[i] * det) * 256);
    for (int i = 0; i < 0x10; ++i) dest->values[i / 4][i % 4] = inv[i];
}
void matrixCopy(Matrix *matDst, Matrix *matSrc)
{
    memcpy(matDst, matSrc, sizeof(Matrix));
}

ushort LoadMesh(const char *filename, Scopes scope)
{
    char buffer[0x100];
    StrCopy(buffer, "Data/Meshes/");
    StrAdd(buffer, filename);

    uint hash[4];
    StrCopy(hashBuffer, filename);
    GenerateHash(hash, StrLength(filename));

    for (int i = 0; i < MODEL_MAX; ++i) {
        if (memcmp(hash, modelList[i].hash, 4 * sizeof(uint)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < MODEL_MAX; ++id) {
        if (modelList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= MODEL_MAX)
        return -1;

    Model *model = &modelList[id];
    FileInfo info;
    MEM_ZERO(info);
    if (LoadFile(&info, filename)) {
        uint sig = ReadInt32(&info);

        if (sig != 0x4C444D) {
            CloseFile(&info);
            return -1;
        }

        model->scope = scope;
        memcpy(model->hash, hash, 4 * sizeof(uint));

        model->flags         = ReadInt8(&info);
        model->faceVertCount = ReadInt8(&info);

        model->vertCount  = ReadInt16(&info);
        model->frameCount = ReadInt16(&info);

        int vertSize = 0;
        AllocateStorage(sizeof(ModelVertex) * model->vertCount * model->frameCount, (void **)&model->vertices, DATASET_STG, true);
        if (model->flags & MODEL_USETEXTURES)
            AllocateStorage(sizeof(TexCoord) * model->vertCount, (void **)&model->texCoords, DATASET_STG, true);
        if (model->flags & MODEL_USECOLOURS)
            AllocateStorage(sizeof(Colour) * model->vertCount, (void **)&model->colours, DATASET_STG, true);

        if (model->flags & MODEL_USETEXTURES) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->texCoords[v].x = ReadSingle(&info);
                model->texCoords[v].y = ReadSingle(&info);
            }
        }

        if (model->flags & MODEL_USECOLOURS) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->colours[v].colour = ReadInt32(&info);
            }
        }

        model->indexCount = ReadInt16(&info);
        for (int i = 0; i < model->indexCount; ++i) {
            model->indices[i] = ReadInt16(&info);
        }

        for (int f = 0; f < model->frameCount; ++f) {
            for (int v = 0; v < model->vertCount; ++v) {
                model->vertices[(f * model->vertCount) + v].x = ReadSingle(&info);
                model->vertices[(f * model->vertCount) + v].y = ReadSingle(&info);
                model->vertices[(f * model->vertCount) + v].z = ReadSingle(&info);

                model->vertices[(f * model->vertCount) + v].nx = 0.0f;
                model->vertices[(f * model->vertCount) + v].ny = 0.0f;
                model->vertices[(f * model->vertCount) + v].nz = 0.0f;
                if (model->flags & MODEL_USENORMALS) {
                    model->vertices[(f * model->vertCount) + v].nx = ReadSingle(&info);
                    model->vertices[(f * model->vertCount) + v].ny = ReadSingle(&info);
                    model->vertices[(f * model->vertCount) + v].nz = ReadSingle(&info);
                }
            }
        }

        CloseFile(&info);
        return id;
    }
    return -1;
}
ushort Create3DScene(const char *name, ushort faceCnt, Scopes scope)
{
    uint hash[4];
    StrCopy(hashBuffer, name);
    GenerateHash(hash, StrLength(name));

    for (int i = 0; i < SCENE3D_MAX; ++i) {
        if (memcmp(hash, scene3DList[i].hash, 4 * sizeof(uint)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < SCENE3D_MAX; ++id) {
        if (scene3DList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SCENE3D_MAX)
        return -1;

    Scene3D *scene = &scene3DList[id];

    if (faceCnt > 0x4000 || !faceCnt)
        faceCnt = 0x4000;

    scene->scope = scope;
    memcpy(scene->hash, hash, 4 * sizeof(uint));
    scene->indexCount = faceCnt;

    return id;
}
void Init3DScene(ushort sceneID) {}
// void View_Something1;
// void View_Something2;
// void View_Something3;
void SetupMesh(ushort animID, ushort sceneID, byte drawMode, Matrix *matWorld, Matrix *matView, uint colour) {}
void SetupMeshAnimation(ushort animID, ushort sceneID, EntityAnimationData *data, byte drawMode, Matrix *matWorld, Matrix *matView, uint colour) {}
void Draw3DScene(ushort sceneID) {}