DEL hello.exe

rustc  -l glew32 -l glfw3  -L lib --extern libc=deps\liblibc.rlib hello.rs

CALL hello.exe