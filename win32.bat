CALL "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"

cl src/main.cpp src/openal_wrapper.cpp src/jx_wrapper.cpp src/opengl_wrapper.cpp src/freetype_wrapper.cpp  /Iinclude /IincludeGLM /Fo./obj/ /link /libpath:lib/win32 jx.lib  glfw3.lib User32.lib OpenGL32.lib  glew32.lib Gdi32.Lib shell32.lib OpenAL32.lib  alut.lib freetype.lib /NODEFAULTLIB:LIBCMT /out:release/win32/main.exe 

release\win32\main.exe