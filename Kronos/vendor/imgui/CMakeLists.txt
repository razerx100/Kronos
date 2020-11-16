file(GLOB KR_SRC imgui.h imgui.cpp imgui_internal.h imgui_demo.cpp imgui_draw.cpp imgui_widgets.cpp imstb_rectpack.h imstb_textedit.h imstb_truetype.h)

add_library(ImGui ${KR_SRC})
