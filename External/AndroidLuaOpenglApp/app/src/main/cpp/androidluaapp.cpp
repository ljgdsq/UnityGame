#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "androidluaapp::", __VA_ARGS__))

#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, "androidluaapp::", __VA_ARGS__))

#include "lua.hpp"
#include <string>

static void printGLString(const char* name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}


int lua_print(lua_State * L);
int get_string_for_print(lua_State * L, std::string* out);
extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidluaapp_MainActivity_RunLua(JNIEnv *env, jclass clazz) {
    LOGI("MainActivity_RunLua");

    lua_State *  L = luaL_newstate();
    luaL_openlibs(L);
    const luaL_Reg global_functions[] = {
            { "print", lua_print },
            { nullptr, nullptr }
    };
    luaL_register(L, "_G", global_functions);
    luaL_dostring(L,"print('lua init finish!')  print(\"Lua version: \" .. _VERSION)");
    lua_close(L);
}

int lua_print(lua_State * L)
{
    std::string t;
    get_string_for_print(L, &t);
    LOGI("[LUA] %s", t.c_str());

    return 0;
}
int get_string_for_print(lua_State * L, std::string* out)
{
    int n = lua_gettop(L);  /* number of arguments */
    int i;

    lua_getglobal(L, "tostring");
    for (i=1; i<=n; i++) {
        const char *s;
        lua_pushvalue(L, -1);  /* function to be called */
        lua_pushvalue(L, i);   /* value to print */
        lua_call(L, 1, 1);
        size_t sz;
        s = lua_tolstring(L, -1, &sz);  /* get result */
        if (s == NULL)
            return luaL_error(L, LUA_QL("tostring") " must return a string to "
                                 LUA_QL("print"));
        if (i>1) out->append("\t");
        out->append(s, sz);
        lua_pop(L, 1);  /* pop result */
    }
    return 0;
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

auto vertexShaderCode= R"(
    attribute vec4 vPosition;
    void main(){

        gl_Position=vPosition;
    }
)";

auto fragmentShaderCode=R"(
    void main(){
        gl_FragColor=vec4(0,0.5,0.5,1);
    }

)";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*)malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        LOGI("create shader ok!");
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*)malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;
GLuint gvPositionHandle;
int setupGraphics(int w, int h) {

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    gProgram = createProgram(vertexShaderCode, fragmentShaderCode);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }

    glUseProgram(gProgram); // 激活程序对象

    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    return 0;
}

const GLfloat gTriangleVertices[] = {0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f};

void renderFrame() {
    static float grey=0;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glClearColor(grey, grey, grey, 1.0f);
    checkGlError("glClearColor");
    checkGlError("glClear");
 glUseProgram(gProgram); // 激活程序对象
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0,
                          gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidluaapp_GL2JNILib_init(JNIEnv *env, jclass clazz, jint width, jint height) {


    setupGraphics(width,height);

    LOGI("setupGraphics(%d, %d)", width, height);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidluaapp_GL2JNILib_step(JNIEnv *env, jclass clazz) {
   renderFrame();
}