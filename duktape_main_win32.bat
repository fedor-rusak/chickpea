if NOT "%variables_were_set%"=="true" (
	CALL "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"
	SET variables_were_set=true
	echo "variables were set"
)

IF EXIST dukRelease\win32\main.exe del dukRelease\win32\main.exe

cl dukSrc/main.cpp dukSrc/engine.cpp dukSrc/glfw_wrapper.cpp dukSrc/openal_wrapper.cpp dukSrc/js_wrapper.cpp dukSrc/opengl_wrapper.cpp dukSrc/freetype_wrapper.cpp  /Iinclude /IincludeGLM /Fo./obj/ /link /libpath:lib/win32 duktape.lib  glfw3.lib User32.lib OpenGL32.lib  glew32.lib Gdi32.Lib shell32.lib OpenAL32.lib  alut.lib freetype.lib /out:dukRelease/win32/main.exe 

if EXIST dukRelease\win32\main.exe CALL dukRelease\win32\main.exe