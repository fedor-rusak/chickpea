g++-4.8 -std=c++11  src/main.cpp src/openal_wrapper.cpp src/jx_wrapper.cpp src/opengl_wrapper.cpp src/freetype_wrapper.cpp -O3 -Wno-write-strings -I./include -fno-rtti -L/usr/local/lib -L./lib/ubuntu64 -lalut -lopenal -lfreetype -ljx -lv8_base -lv8_snapshot -lchrome_zlib -lsqlite3 -lhttp_parser -luv -lcares -lopenssl -lGLEW -lglfw3 -lrt -lXi -lXrandr -lm -ldl -lpthread  -o ./release/ubuntu1204_64/main

./release/ubuntu1204_64/main
 
