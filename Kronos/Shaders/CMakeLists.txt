file(GLOB_RECURSE Shaders_SRC HLSL/*.hlsl)
add_library(Shaders ${Shaders_SRC})
set_target_properties(Shaders PROPERTIES LINKER_LANGUAGE CXX)
