cmake_minimum_required (VERSION 3.14.5)
project (janus-gateway-sfu)

add_library (sfu SHARED src/cxx_interface.h src/plugin.c src/glue.cpp src/sfu.h src/sfu.cpp)
set_target_properties(sfu PROPERTIES CXX_STANDARD 17)
target_include_directories (sfu PUBLIC /usr/include/janus-gateway)
target_include_directories (sfu PUBLIC /usr/include/glib-2.0)
target_include_directories (sfu PUBLIC /usr/lib64/glib-2.0/include)
