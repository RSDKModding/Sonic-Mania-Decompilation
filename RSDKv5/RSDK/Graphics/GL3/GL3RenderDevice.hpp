class RenderDevice : public RenderDeviceBase
{
public:
    static bool Init();
    static void CopyFrameBuffer();
    static void FlipScreen();
    static void Release(bool32 isRefresh);

    static void RefreshWindow();
    static void SetupImageTexture(int32 width, int32 height, uint8 *imagePixels);

    static bool ProcessEvents();

    static void InitFPSCap();
    static bool CheckFPSCap();
    static void UpdateFPSCap();

    static void LoadShader(const char *fileName, bool32 linear);

    static inline void ShowCursor(bool32 shown) { glfwSetInputMode(window, GLFW_CURSOR, shown ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN); }

    static GLFWwindow *window;

    static GLuint screenTextures[SCREEN_MAX];
    static GLuint imageTexture;

private:
    static bool InitShaders();
    static bool SetupRendering();
    static void InitVertexBuffer();
    static bool InitGraphicsAPI();

    static void GetDisplays();

    static void ProcessKeyEvent(GLFWwindow *, int key, int scancode, int action, int mods);
    static void ProcessFocusEvent(GLFWwindow *, int focused);
    static void ProcessMouseEvent(GLFWwindow *, int button, int action, int mods);
    static void ProcessJoystickEvent(int ID, int event);
    static void ProcessMaximizeEvent(GLFWwindow*, int maximized);

    static void SetLinear(bool32 linear);

    static int monitorIndex;

    static GLuint VAO;
    static GLuint VBO;

    static double lastFrame;
    static double targetFreq;
};

struct ShaderEntry : public ShaderEntryBase {
    GLuint programID;
};
