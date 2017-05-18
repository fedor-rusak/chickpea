DEL hello.exe

rustc hello.rs -L lib --extern libc=deps\liblibc.rlib

CALL hello.exe