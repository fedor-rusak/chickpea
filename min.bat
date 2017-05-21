if EXIST release\win32\main.exe del release\win32\main.exe

cl main.c  /Iinclude /Fo./obj/ /link /libpath:lib/win32 glfw3.lib OpenGL32.lib  glew32.lib  /out:release/win32/main.exe 

if EXIST release\win32\main.exe CALL release\win32\main.exe