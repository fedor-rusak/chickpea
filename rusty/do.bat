DEL hello.exe

rustc -L lib --extern libc=deps\liblibc.rlib hello.rs

CALL hello.exe