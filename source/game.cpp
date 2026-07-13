#include "game.h"

#include <GLES/egl.h>
#include <dirent.h>
#include <switch.h>

#include <cstring>

#include "achievement.h"
#include "audio/akb.h"
#include "bridge.h"
#include "config.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_gles1.h"
#include "imgui/imgui_impl_switch.h"
#include "menu/menu.h"
#include "menu/player_select.h"
#include "overlay.h"
#include "pad_manager.h"
#include "patches.h"
#include "util.h"
#include "vibration.h"

namespace game {

bool g_menuOpen          = false;
bool g_playerSelectOpen  = false;
bool g_encountersEnabled = true;
float g_gilMultiplier    = 1.0f;
float g_expMultiplier    = 1.0f;
bool g_saveAnywhere      = false;
bool g_debugMenu         = false;
bool g_UseFakeTouch      = false;
float g_FakeTouch[2]     = { 0.0f, 0.0f };
bool g_Launched          = false;
u64 g_MaxUpdatesPerFrame = 3;

GLuint capturedFrameTexture = 0;
bool hasCapturedFrame       = false;
int capturedWidth           = 0;
int capturedHeight          = 0;

EGLDisplay display = EGL_NO_DISPLAY;
EGLSurface surface = EGL_NO_SURFACE;
EGLContext context = EGL_NO_CONTEXT;

void init() {
    hidInitializeTouchScreen();
}

bool init_renderer() {
    EGLint numConfigs = 0;
    EGLConfig eglConfig;

    const EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 1,
        EGL_NONE
    };

    const EGLint configAttribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };

    NWindow* win = nwindowGetDefault();
    nwindowSetDimensions(win, config::screen_width, config::screen_height);

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (!display) {
        debugPrintf("EGL: Could not connect to display: %08x\n", eglGetError());
        return false;
    }

    eglInitialize(display, NULL, NULL);

    if (eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE) {
        debugPrintf("EGL: Could not set API: %08x\n", eglGetError());
        return false;
    }

    eglChooseConfig(display, configAttribs, &eglConfig, 1, &numConfigs);
    if (numConfigs <= 0) {
        debugPrintf("EGL: No matching config: %08x\n", eglGetError());
        return false;
    }

    surface = eglCreateWindowSurface(display, eglConfig, win, NULL);
    if (!surface) {
        debugPrintf("EGL: Could not create surface: %08x\n", eglGetError());
        return false;
    }

    context = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, contextAttribs);
    if (!context) {
        debugPrintf("EGL: Could not create context: %08x\n", eglGetError());
        return false;
    }

    eglMakeCurrent(display, surface, surface, context);
    if (eglSwapInterval(display, 1) == EGL_FALSE)
        debugPrintf("EGL: Could not set swap interval: %08x\n", eglGetError());

    debugPrintf("GL_EXTENSIONS: %s\n", glGetString(GL_EXTENSIONS));
    return true;
}

void captureScreen() {
    if (capturedWidth != config::screen_width || capturedHeight != config::screen_height) {
        if (capturedFrameTexture != 0) {
            glDeleteTextures(1, &capturedFrameTexture);
            capturedFrameTexture = 0;
        }
        capturedWidth    = config::screen_width;
        capturedHeight   = config::screen_height;
        hasCapturedFrame = false;
    }

    if (capturedFrameTexture == 0) {
        glGenTextures(1, &capturedFrameTexture);
    }

    glBindTexture(GL_TEXTURE_2D, capturedFrameTexture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0,
        config::screen_width, config::screen_height, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    hasCapturedFrame = true;
}

void drawCapturedFrame() {
    if (!hasCapturedFrame || capturedFrameTexture == 0)
        return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrthof(0, 1, 1, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    GLfloat vertices[] = {
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
    };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, capturedFrameTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 16, vertices);
    glTexCoordPointer(2, GL_FLOAT, 16, vertices + 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void cleanupCapturedFrame() {
    if (capturedFrameTexture != 0) {
        glDeleteTextures(1, &capturedFrameTexture);
        capturedFrameTexture = 0;
    }
    hasCapturedFrame = false;
}

void start() {
    debugPrintf("=== FF4 NX BUILD v1 (stack canary fix — disable TLS-based __stack_chk) ===\n");
    debugPrintf_setMainThread();

    DIR* dir = opendir(OBB_MODS);
    if (dir) {
        struct dirent* ent;
        while ((ent = readdir(dir)) != NULL) {
            debugPrintf("Found file in mods: %s\n", ent->d_name);
            if (strstr(ent->d_name, ".obb") != nullptr) {
                debugPrintf("Found mod: %s\n", ent->d_name);
                char full_path[512];
                snprintf(full_path, sizeof(full_path), OBB_MODS "%s", ent->d_name);

                debugPrintf("Reading mod: %s\n", full_path);
                bridge::readHeader(full_path);
            }
        }
        closedir(dir);
    }

    bridge::readHeader(OBB_FILE);
    bridge::applyModOrder();
    bridge::buildFileMap();
    bridge::initFont();

    pad_manager::init();
    vibration::init();
    AKBSystem::Init();

    if (!init_renderer()) {
        debugPrintf("game::start: renderer initialization FAILED!\n");
        return;
    }
    debugPrintf("game::start: EGL initialized (display=%p, surface=%p, context=%p)\n",
        display, surface, context);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    debugPrintf("game::start: before font load!\n");
    std::string path  = std::string(FONTS_DIR) + config::font_filename;
    ImFont* font      = io.Fonts->AddFontFromFileTTF(path.c_str(), 32.0f * IMGUI_SCALE);
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    style.ScaleAllSizes(IMGUI_SCALE);

    ImGui_ImplSwitch_Init();
    ImGui_ImplGLES1_Init();

    debugPrintf("game::start: before menu init!\n");

    menu::init();
    achievement::init();

    g_Launched = config::skip_launcher;
    debugPrintf("game::start: before game loop!\n");

    static AppletOperationMode lastMode = (AppletOperationMode)-1;
    while (appletMainLoop()) {
        AppletOperationMode currentMode = appletGetOperationMode();
        if (lastMode != currentMode) {
            if (currentMode == AppletOperationMode_Console) {
                config::screen_width  = 1920;
                config::screen_height = 1080;
            } else {
                config::screen_width  = 1920;
                config::screen_height = 1080;
                // config::screen_width  = 1280;
                // config::screen_height = 720;
            }
            NWindow* win = nwindowGetDefault();
            nwindowSetDimensions(win, config::screen_width, config::screen_height);

            lastMode = currentMode;
        }

        pad_manager::update();
        pad_manager::handleSpecialKeys();

        float mult                = bridge::getFPSMultiplier();
        static bool isUpdateFrame = false;
        if (g_Launched) {
            isUpdateFrame = bridge::isUpdateFrame();

            if (isUpdateFrame) {
                if (!g_menuOpen) {
                    HidTouchScreenState touch;
                    float coordinates[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

                    hidGetTouchScreenStates(&touch, 1);

                    int reportNum = touch.count > 2 ? 2 : touch.count;
                    for (int n = 0; n < reportNum; n++) {
                        coordinates[n * 2]     = (float)touch.touches[n].x / 1280.0f;
                        coordinates[n * 2 + 1] = (float)touch.touches[n].y / 720.0f;
                    }

                    if (g_UseFakeTouch) {
                        reportNum      = 1;
                        coordinates[0] = g_FakeTouch[0];
                        coordinates[1] = g_FakeTouch[1];
                        g_UseFakeTouch = false;
                    }

                    AKBSystem::Update();
                    babil::touch(0, 0, reportNum, reportNum, coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
                }

                babil::render(patches::jni::fake_env, 0, config::screen_width);
                captureScreen();
            } else {
                drawCapturedFrame();
            }
        } else {
            g_menuOpen = true;
        }

        ImGui_ImplGLES1_NewFrame();
        ImGui_ImplSwitch_NewFrame();
        ImGui::NewFrame();

        if (g_playerSelectOpen) {
            player_select::update();
        } else {
            if (g_menuOpen) {
                menu::update();
            }
        }

        if (g_Launched) {
            if (config::show_fps || (config::show_game_speed_mult && mult != 1.0f) || config::show_frame_graph || config::show_ups || config::show_update_graph) {
                overlay::begin(isUpdateFrame);
                if (config::show_game_speed_mult)
                    overlay::drawGameSpeedMult();
                if (config::show_fps)
                    overlay::drawFPS();
                if (config::show_frame_graph)
                    overlay::drawFrameTimeGraph();
                if (config::show_ups)
                    overlay::drawUPS();
                if (config::show_update_graph)
                    overlay::drawUpdateTimeGraph();
                ImGui::End();
            }
        }

        achievement::draw();

        ImGui::Render();
        ImGui_ImplGLES1_RenderDrawData(ImGui::GetDrawData());
        eglSwapBuffers(display, surface);

        menu::postUpdate();
    }

    ImGui_ImplGLES1_Shutdown();
    ImGui::DestroyContext();
    pad_manager::exit();
    AKBSystem::Quit();
}

}

extern "C" void release_egl(void) {
    if (game::display != EGL_NO_DISPLAY) {
        eglMakeCurrent(game::display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (game::context != EGL_NO_CONTEXT) {
            eglDestroyContext(game::display, game::context);
            game::context = EGL_NO_CONTEXT;
        }
        if (game::surface != EGL_NO_SURFACE) {
            eglDestroySurface(game::display, game::surface);
            game::surface = EGL_NO_SURFACE;
        }

        eglTerminate(game::display);
        game::display = EGL_NO_DISPLAY;
    }
}