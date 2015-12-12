#ifndef IMGUIBINDINGS_H_
#define IMGUIBINDINGS_H_

#ifndef IMGUI_API
#include <imgui.h>
#endif //IMGUI_API

// include openGL headers here:
#ifdef _WIN32
#pragma warning (disable: 4996)         // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#include <windows.h>
#include <imm.h>
#endif //_WIN32

//# define GLEW_STATIC		// Optional, depending on which glew lib you want to use
#ifdef IMGUI_USE_GLEW
#   ifdef __APPLE__   // or __MACOSX__ ?
#       include <OpenGL/glew.h>     // guessing...
#   else //__APPLE
#       include <GL/glew.h>
#   endif //__APPLE
#else //IMGUI_USE_GLEW
#   define GL_GLEXT_PROTOTYPES
#endif //IMGUI_USE_GLEW


#if (!defined(IMGUI_USE_WINAPI_BINDING) && !defined(IMGUI_USE_GLFW_BINDING) && !defined(IMGUI_USE_SDL2_BINDING) && !defined(IMGUI_USE_GLUT_BINDING))
#   ifdef _WIN32
#       define IMGUI_USE_WINAPI_BINDING
#   else //_WIN32
#       define IMGUI_USE_GLFW_BINDING
#   endif //_WIN32
#endif // !defined(...)

#ifdef IMGUI_USE_GLUT_BINDING
//-------------------------------------------------------------------------------
#   ifdef __APPLE__   // or __MACOSX__ ?
#       include <OpenGL/glut.h>             // guessing...
#   else //__APPLE
#       include <GL/glut.h>
#   endif //__APPLE
#ifndef __EMSCRIPTEN__
#   ifdef __FREEGLUT_STD_H__
#       ifdef __APPLE__   // or __MACOSX__ ?
#           include <OpenGL/freeglut_ext.h>     // guessing...
#       else //__APPLE
#           include <GL/freeglut_ext.h>
#       endif //__APPLE
#   endif //__FREEGLUT_STD_H__
#endif //__EMSCRIPTEN__
//-------------------------------------------------------------------------------
#elif IMGUI_USE_SDL2_BINDING
//-------------------------------------------------------------------------------
#   ifndef IMGUI_USE_GLEW
#       if (!defined(IMIMPL_SHADER_GLES) || defined (IMIMPL_SHADER_NONE))
#           include <SDL2/SDL_opengl.h>
#       else //IMIMPL_SHADER_GLES
#           include <SDL2/SDL_opengles2.h>
#       endif //IMIMPL_SHADER_GLES
#   endif //IMGUI_USE_GLEW
#   include <SDL2/SDL.h>
//-------------------------------------------------------------------------------
#elif IMGUI_USE_GLFW_BINDING
//-------------------------------------------------------------------------------
//# define GLFW_STATIC   //// Optional, depending on which glfw lib you want to use
#   include <GLFW/glfw3.h>
#   if (GLFW_VERSION_MAJOR<3)
#       error GLFW_VERSION_MAJOR < 3 is not supported
#   endif //GLFW_VERSION_MAJOR<3
#   ifdef _WIN32
#       define GLFW_EXPOSE_NATIVE_WIN32
#       define GLFW_EXPOSE_NATIVE_WGL
#       include <GLFW/glfw3native.h>    // glfwGetWin32Window(...) used by ImImpl_ImeSetInputScreenPosFn(...)
#   endif //_WIN32
//-------------------------------------------------------------------------------
#elif (defined(_WIN32) || defined(IMGUI_USE_WINAPI_BINDING))
//-------------------------------------------------------------------------------
#   ifndef IMGUI_USE_GLEW
//      I've never managed to make this branch work => when using Windows, ALWAYS use glew (on Linux it's much easier)
#       define GL_GLEXT_PROTOTYPES
#       ifdef __APPLE__   // or __MACOSX__ ?
#           include <OpenGL/glext.h>     // guessing...
#       else //__APPLE
#           include <GL/glext.h>
#       endif //__APPLE
#   endif // IMGUI_USE_GLEW
#   ifdef __APPLE__   // or __MACOSX__ ?
#       include <OpenGL/gl.h>       // guessing...
#   else //__APPLE
#       include <GL/gl.h>
#   endif //__APPLE
//--------------------------------------------------------------------------------
#else // IMGUI_USE_SOME_BINDING
#error: No IMGUI_USE_XXX_BINDING defined
#include "./addons/imguibindings/ImImpl_Binding_Glfw3.h"
#endif // IMGUI_USE_SOME_BINDING

#include <string.h>
#include <stdio.h>

extern void InitGL();
extern void ResizeGL(int w,int h);
extern void DrawGL();
extern void DestroyGL();

// These variables can be declared extern and set at runtime-----------------------------------------------------
extern bool gImGuiPaused;// = false;
extern float gImGuiInverseFPSClampInsideImGui;// = -1.0f;    // CAN'T BE 0. < 0 = No clamping.
extern float gImGuiInverseFPSClampOutsideImGui;// = -1.0f;    // CAN'T BE 0. < 0 = No clamping.
extern bool gImGuiDynamicFPSInsideImGui;                   // Dynamic FPS inside ImGui: from 5 to gImGuiInverseFPSClampInsideImGui
extern bool gImGuiCapturesInput;             // When false the input events can be directed somewhere else
extern bool gImGuiWereOutsideImGui;
extern bool gImGuiBindingMouseDblClicked[5];
// --------------------------------------------------------------------------------------------------------------

struct ImImpl_InitParams	{
    friend struct FontData;
    // Holds info for physic or memory file (used to load TTF)
    struct FontData {
        char filePath[2048];
        const unsigned char* pMemoryData;
        size_t memoryDataSize;
        enum Compression {
            COMP_NONE=0,
            COMP_STB,
            COMP_STBBASE85
#if         (!defined(NO_IMGUIHELPER) && defined(IMGUI_USE_ZLIB))
            ,COMP_GZ
#           endif   //IMGUI_USE_ZLIB
        };
        Compression memoryDataCompression;
        float sizeInPixels;//=15.0f,
        const ImWchar* pGlyphRanges;
        ImFontConfig fontConfig;
        bool useFontConfig;

        FontData(const unsigned char* _pMemoryData,size_t _memoryDataFile,Compression _memoryDataCompression=COMP_NONE,
                 float _sizeInPixels=15.f,const ImWchar* pOptionalGlyphRanges=NULL,ImFontConfig* pOptionalFontConfig=NULL)
        : pMemoryData(_pMemoryData) , memoryDataSize(_memoryDataFile), memoryDataCompression(_memoryDataCompression),
        sizeInPixels(_sizeInPixels),pGlyphRanges(pOptionalGlyphRanges?pOptionalGlyphRanges:ImImpl_InitParams::GetGlyphRangesDefault())
        ,useFontConfig(false)
        {IM_ASSERT(pMemoryData);IM_ASSERT(_memoryDataFile);
        //IM_ASSERT(sizeInPixels>0);
        filePath[0]='\0';
        if (pOptionalFontConfig) {useFontConfig=true;fontConfig=*pOptionalFontConfig;}}
        FontData(const char* _filePath,float _sizeInPixels=15.f,const ImWchar* pOptionalGlyphRanges=NULL,ImFontConfig* pOptionalFontConfig=NULL)
        : pMemoryData(NULL) , memoryDataSize(0),sizeInPixels(_sizeInPixels),pGlyphRanges(pOptionalGlyphRanges?pOptionalGlyphRanges:ImImpl_InitParams::GetGlyphRangesDefault())
        ,useFontConfig(false)
        {IM_ASSERT(_filePath && strlen(_filePath)>0);
         const size_t len = strlen(_filePath);IM_ASSERT(len>0 && len<2047);
         if (len<2047) strcpy(filePath,_filePath);//IM_ASSERT(sizeInPixels>0);
         if (pOptionalFontConfig) {useFontConfig=true;fontConfig=*pOptionalFontConfig;}}
    };
	ImVec2 gWindowSize;
	char gWindowTitle[1024];
    float gFpsClampInsideImGui;	// <0 -> no clamp
    float gFpsClampOutsideImGui;	// <0 -> no clamp
    bool gFpsDynamicInsideImGui;    // false
    ImVector<FontData> fonts;
    bool forceAddDefaultFontAsFirstFont;
    ImImpl_InitParams(
            int windowWidth=1270,
            int windowHeight=720,
            const char* windowTitle=NULL,
            const char* OptionalTTFFilePath=NULL,
            const unsigned char*    _pOptionalReferenceToTTFFileInMemory=NULL,
            size_t                  _pOptionalSizeOfTTFFileInMemory=0,
            const float OptionalTTFFontSizeInPixels=15.0f,
            const ImWchar* OptionalTTFGlyphRanges=NULL,
            ImFontConfig* pFontConfig=NULL,
            bool _forceAddDefaultFontAsFirstFont = false
    ) :
    gFpsClampInsideImGui(-1.0f),
    gFpsClampOutsideImGui(-1.0f),
    gFpsDynamicInsideImGui(false),
    forceAddDefaultFontAsFirstFont(_forceAddDefaultFontAsFirstFont)
	{
        gWindowSize.x = windowWidth<=0?1270:windowWidth;gWindowSize.y = windowHeight<=0?720:windowHeight;

		gWindowTitle[0]='\0';
		if (windowTitle)	{
			const size_t len = strlen(windowTitle);
			if (len<1023) strcat(gWindowTitle,windowTitle);
			else		  {
				memcpy(gWindowTitle,windowTitle,1023);
				gWindowTitle[1023]='\0';
			}
		}
		else strcat(gWindowTitle,"ImGui OpenGL Example");

        if (OptionalTTFFilePath && strlen(OptionalTTFFilePath)>0)
            fonts.push_back(FontData(OptionalTTFFilePath,OptionalTTFFontSizeInPixels,OptionalTTFGlyphRanges,pFontConfig));
        else if (_pOptionalReferenceToTTFFileInMemory && _pOptionalSizeOfTTFFileInMemory>0)
            fonts.push_back(FontData(_pOptionalReferenceToTTFFileInMemory,_pOptionalSizeOfTTFFileInMemory,FontData::COMP_NONE,OptionalTTFFontSizeInPixels,OptionalTTFGlyphRanges,pFontConfig));
	}
    ImImpl_InitParams(
            int windowWidth,
            int windowHeight,
            const char* windowTitle,
            const ImVector<ImImpl_InitParams::FontData> & _fonts,
            bool _forceAddDefaultFontAsFirstFont = false
    ) :
    gFpsClampInsideImGui(-1.0f),
    gFpsClampOutsideImGui(-1.0f),
    gFpsDynamicInsideImGui(false),
    //fonts(_fonts),    // Hehe: this crashes the program on exit (I guess ImVector can't handle operator= correctly)
    forceAddDefaultFontAsFirstFont(_forceAddDefaultFontAsFirstFont)
    {
        gWindowSize.x = windowWidth<=0?1270:windowWidth;gWindowSize.y = windowHeight<=0?720:windowHeight;
        fonts.reserve(_fonts.size());for (int i=0,isz=_fonts.size();i<isz;i++) fonts.push_back(_fonts[i]);  // Manual workaround that works

        gWindowTitle[0]='\0';
        if (windowTitle)	{
            const size_t len = strlen(windowTitle);
            if (len<1023) strcat(gWindowTitle,windowTitle);
            else		  {
                memcpy(gWindowTitle,windowTitle,1023);
                gWindowTitle[1023]='\0';
            }
        }
        else strcat(gWindowTitle,"ImGui OpenGL Example");
    }

    private:
    // Retrieve list of range (2 int per range, values are inclusive)
    static const ImWchar*   GetGlyphRangesDefault()
    {
        static const ImWchar ranges[] =
        {
            0x0020, 0x00FF, // Basic Latin + Latin Supplement
            0x20AC, 0x20AC,	// €
            0x2122, 0x2122,	// ™
            0x263A, 0x263A, // ☺
            0x266A, 0x266A, // ♪
            0,
        };
        return &ranges[0];
    }
};

#ifdef IMGUI_USE_WINAPI_BINDING
extern int ImImpl_WinMain(const ImImpl_InitParams* pOptionalInitParams,HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int iCmdShow);
#else //IMGUI_USE_WINAPI_BINDING
extern int ImImpl_Main(const ImImpl_InitParams* pOptionalInitParams=NULL,int argc=0, char** argv=NULL);
#endif //IMGUI_USE_WINAPI_BINDING


extern void InitImGuiFontTexture(const ImImpl_InitParams* pOptionalInitParams=NULL);
extern void DestroyImGuiFontTexture();

extern void InitImGuiProgram();
extern void DestroyImGuiProgram();

extern void InitImGuiBuffer();
extern void DestroyImGuiBuffer();
extern void ImImpl_RenderDrawLists(ImDrawData* draw_data);


extern void WaitFor(unsigned int ms);
extern GLuint ImImpl_LoadTexture(const char* filename,int req_comp=0,GLenum magFilter=GL_LINEAR,GLenum minFilter=GL_LINEAR_MIPMAP_LINEAR,GLenum wrapS=GL_REPEAT,GLenum wrapT=GL_REPEAT);
extern GLuint ImImpl_LoadTextureFromMemory(const unsigned char* filenameInMemory,int filenameInMemorySize,int req_comp=0,GLenum magFilter=GL_LINEAR,GLenum minFilter=GL_LINEAR_MIPMAP_LINEAR,GLenum wrapS=GL_REPEAT,GLenum wrapT=GL_REPEAT);

#ifndef IMIMPL_SHADER_NONE
class ImImpl_CompileShaderStruct {
public:
    GLuint vertexShaderOverride,fragmentShaderOverride; // when !=0, the shader codes will be ignored, and these will be used.
    GLuint programOverride;             // when !=0, the shaders will be ignored completely.
    bool dontLinkProgram;               // when "true", shaders are attached to the program, but not linked, the program is returned to "programOverride" and the shaders are returned to "vertexShaderOverride" and "fragmentShaderOverride" if "dontDeleteAttachedShaders" is true.
                                        // however, when "false" and linking is performed, then the shaders are detached and deleted IN ANY CASE.
    bool dontDeleteAttachedShaders;     // After the program is linked, by default all the attached shaders are deleted IN ANY CASE.

#ifndef NO_IMGUISTRING
protected:
    typedef ImHashMap<ImString,ImString,ImHashFunctionImString,ImHashMapKeyEqualityFunctionDefault<ImString>,256> ImStringImStringMap;
    ImStringImStringMap mPreprocessorDefinitionsWithValue;
    ImVector < ImString > mPreprocessorDefinitions; // Without values (don't remember why I split them here)
    int mNumPreprocessorAdditionalLines;
    ImString mPreprocessorAdditionalShaderCode;
public:
    // TODO: Test all this staff
    void addPreprocessorDefinition(const ImString& name,const ImString& value="");
    void removePreprocessorDefinition(const ImString& name);
    void updatePreprocessorDefinitions();   // mandatory after a set of add/remove calls to generate an usable "mPreprocessorAdditionalShaderCode"
    inline const char* getPreprocessorDefinitionAdditionalCode() const {return mPreprocessorAdditionalShaderCode.c_str();}
    inline int getNumPreprocessorDefinitionAdditionalLines() const {return mNumPreprocessorAdditionalLines;}
    void resetPreprocessorDefinitions();
    void reset() {clearShaderOptions();resetPreprocessorDefinitions();}
#else  //NO_IMGUISTRING
public:
    void reset()  {clearShaderOptions();}
#endif //NO_IMGUISTRING
public:
    void clearShaderOptions() {vertexShaderOverride = fragmentShaderOverride = programOverride = 0;dontLinkProgram = dontDeleteAttachedShaders = false;}
    ImImpl_CompileShaderStruct() {reset();}
};
// returns the shader program ID. "optionalShaderCodePrefix" (if present) is just copied before the source of both shaders. "pOptionalOptions" can be used to tune dynamic definitions inside the shader code and some shader compilation and linking processing steps.
extern GLuint ImImpl_CompileShadersFromMemory(const GLchar** vertexShaderSource, const GLchar** fragmentShaderSource,ImImpl_CompileShaderStruct* pOptionalOptions=NULL, const GLchar** optionalVertexShaderCodePrefix=NULL, const GLchar** optionalFragmentShaderCodePrefix=NULL);
#   ifndef NO_IMGUIHELPER_SERIALIZATION
#       ifndef NO_IMGUIHELPER_SERIALIZATION_LOAD
// returns the shader program ID. "pOptionalOptions" can be used to tune dynamic definitions inside the shader code and some shader compilation and linking processing steps.
extern GLuint ImImpl_CompileShadersFromFile(const char* vertexShaderFilePath, const char* fragmentShaderFilePath,ImImpl_CompileShaderStruct* pOptionalOptions=NULL, bool allowProcessingASingleIncludeDirectivePlacedAtTheFirstLineOfTheShaderCode=false);
#       endif //NO_IMGUIHELPER_SERIALIZATION_LOAD
#   endif //NO_IMGUIHELPER_SERIALIZATION
#endif //IMIMPL_SHADER_NONE

#ifdef IMIMPL_FORCE_DEBUG_CONTEXT
extern "C" void GLDebugMessageCallback(GLenum source, GLenum type,
    GLuint id, GLenum severity,GLsizei length, const GLchar *msg,const void *userParam);
#endif //IMIMPL_FORCE_DEBUG_CONTEXT


#endif //IMGUIBINDINGS_H_

