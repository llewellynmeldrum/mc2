#pragma once
#include "input/input_keycodes.hpp"
#ifndef __cplusplus
	#error "This header is intended for c++ only."
#endif // __cplusplus
#include <thread>

#include "platform.hpp"
#include "r3d.hpp"
#include "input/input.hpp"
#include "world.hpp"
#include "camera.hpp"
#include "profiler.hpp"
#include "input/input_actions.hpp"
#include "input/input_binds.hpp"

#include "transformations.hpp"
#include "mesh_id.hpp"
#include "ui/ui.hpp"
#include "input/input_binds.hpp"

struct Engine{
 public:
    Engine();
    ~Engine();
    auto loop() -> void;

    static constexpr auto initialWindowLogicalExtent = glm::ivec2{
        1280,720
    };

    Platform platform;
    InputState input_state;
    Renderer3D r3d;
    Renderer2D r2d;
    World world;
    Camera cam;
    Gui ui;
    Profiler profiler;
    std::vector<Binding> bindings{
        Binding{HoldButtonBind{
            .button = ButtonCode::KEY_W,
            .action = MoveCameraAction{ glm::vec3{0,-1,0}}
        }},
        Binding{PlatformQuitBind{ 
            .action = EngineQuitAction{},
        }},
        Binding{ToggleButtonBind{ 
            .button = ButtonCode::KEY_ESCAPE,
            .action = EngineQuitAction{},
        }},
        Binding{ToggleButtonBind{ 
            .button = ButtonCode::KEY_C,
            .mods = KeyMod{KeyModBits::eCTRL},
            .action = EngineQuitAction{},
        }}
    };


    std::vector<Action> actions;
    auto should_run() const{ return !m_shouldExit; }
    bool m_shouldExit{false};
};


auto derive_actions_from_input(
    std::vector<Action>& out_actions,
    InputState const& input,
    std::span<const Binding> bindings
) -> std::vector<Action>&;
