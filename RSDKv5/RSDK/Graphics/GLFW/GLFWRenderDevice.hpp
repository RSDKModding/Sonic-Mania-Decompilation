class RenderDevice : public RenderDeviceBase
{
public:
    struct WindowInfo {
        union {
            struct {
                int32 width;
                int32 height;
                int32 _pad[3];
                int32 refresh_rate;
            };
            GLFWvidmode internal;
        } * displays;
    };
    static WindowInfo displayInfo;

    static bool Init();
    static void CopyFrameBuffer();
    static void FlipScreen();
    static void Release(bool32 isRefresh);

    static void RefreshWindow();
    static void GetWindowSize(int32 *width, int32 *height);

    static void SetupImageTexture(int32 width, int32 height, uint8 *imagePixels);
    static void SetupVideoTexture_YUV420(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);
    static void SetupVideoTexture_YUV422(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);
    static void SetupVideoTexture_YUV444(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);

    static bool ProcessEvents();

    static void InitFPSCap();
    static bool CheckFPSCap();
    static void UpdateFPSCap();

    static void LoadShader(const char *fileName, bool32 linear);

    static inline void ShowCursor(bool32 shown) { glfwSetInputMode(window, GLFW_CURSOR, shown ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN); }

    static GLFWwindow *window;

    static GLuint screenTextures[SCREEN_COUNT];
    static GLuint imageTexture;

private:
    static bool InitShaders();
    static bool SetupRendering();
    static void InitVertexBuffer();
    static bool InitGraphicsAPI();

    static void GetDisplays();

    static void ProcessKeyEvent(GLFWwindow *, int32 key, int32 scancode, int32 action, int32 mods);
    static void ProcessFocusEvent(GLFWwindow *, int32 focused);
    static void ProcessMouseEvent(GLFWwindow *, int32 button, int32 action, int32 mods);
    static void ProcessJoystickEvent(int32 ID, int32 event);
    static void ProcessMaximizeEvent(GLFWwindow *, int32 maximized);

    static void SetLinear(bool32 linear);

    static int32 monitorIndex;

    static GLuint VAO;
    static GLuint VBO;

    static double lastFrame;
    static double targetFreq;

    static uint32 *videoBuffer;
};

struct ShaderEntry : public ShaderEntryBase {
    GLuint programID;
};
