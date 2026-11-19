#pragma once
#include "ui/ui.hpp"
#include "util/cppslop.hpp"
#include "platform.hpp"
#include "r3d.hpp"
namespace vk::raii{
    FORWARD_DECLARE_STRUCT(CommandBuffer);
}
auto init_gui(Gui& ui, Platform& platform, Renderer3D& r3d) -> void;
auto record_imgui_commands(vk::raii::CommandBuffer const& cmdBuf) -> void;
