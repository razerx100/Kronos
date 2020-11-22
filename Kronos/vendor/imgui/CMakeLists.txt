include_directories(./)

file(GLOB KR_SRC imgui.h imconfig.h imgui.cpp imgui_internal.h imgui_demo.cpp imgui_draw.cpp imgui_widgets.cpp imstb_rectpack.h imstb_textedit.h imstb_truetype.h backends/imgui_impl_win32.cpp backends/imgui_impl_win32.h backends/imgui_impl_dx12.cpp backends/imgui_impl_dx12.h)

add_library(ImGui ${KR_SRC})
