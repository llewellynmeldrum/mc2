#include <list>
#include <print>
#include <utility>
#include <variant>
#include <span>

#include "glm_types.hpp"
#include "engine.hpp"
#include "input_events.hpp"
#include "Engine_Gui.hpp"

#include "color_utils.hpp"
#include "input_actions.hpp"
#include "timer.hpp"
#include "auto_format.hpp"
#include "stable_type_name.hpp"

#include "format_specs.hpp"
#include "input_binds.hpp"

void apply_actions_to_engine(std::span<Action const> actions, Engine& engine, f64 dt_s){
    for (auto const& action: actions){
        std::visit(overloaded{
            [&](EngineQuitAction const& act){
                engine.m_shouldExit = true;
            },
            [&](auto const& act){ },
        }, action);
    }
}
void apply_actions_to_camera(std::span<Action const> actions, Camera & cam, f64 dt_s){
    // since dt is in seconds, the scale of the action is essentially the 'units per second'.
    for (auto const& action: actions){
//        LOG_DBG(
//            "Applying action '{}', {} {}",
//            act.type,
//            act.delta2==glm::vec2{0,0} ? "" : std::format("{}, ", act.delta2),
//            act.delta3==glm::vec3{0,0,0} ? "" : std::format("{}", act.delta3)
//       );
        auto scale = dt_s;
        std::visit(overloaded{
            [&](MoveCameraAction const& act){
                auto const d = act.delta_world;
                auto const cam_local = d.x * cam.get_right() + d.y*cam.get_up() - d.z*cam.get_front();
                cam.move(cam_local,scale);
            },
            [&](LookCameraAction const& act){
                cam.look_around(act.delta);
            },
            [&](ZoomCameraAction const& act){
                if (act.zoom_in){cam.zoom_in();}
                else {cam.zoom_out();}
            },
            [&](ModifyZoomCameraAction const& act){
                if (act.delta > 0.0f){
                    // scrolled up 
                    cam.increase_zoom_mult(std::abs(act.delta));
                }else {
                    // scrolled down 
                    cam.decrease_zoom_mult(std::abs(act.delta));
                }

            },
            [&](auto const& act){ },
        },action);
    }
}

struct FrameContext{};
auto begin_frame() -> FrameContext{
    return {};
}

void Engine::loop(){
    profiler.start_frame();
    auto dt_s = profiler.get_dt_seconds();
    if (ui.wants_mouse()){
        platform.uncapture_cursor();
    }else{
        platform.capture_cursor();
    }

    auto events = poll_events(platform);        // Platform polls for events
    input_state = update_input_states(                         // input is updated to reflect the events 
        events, 
        input_state, 
        ui.wants_mouse(), 
        ui.wants_keyboard()
    );
    actions = derive_actions_from_input(actions, input_state, bindings); // based on input, derive actions
    apply_actions_to_engine(actions,*this, dt_s);              // apply the actions.
    apply_actions_to_camera(actions, cam, dt_s);              // apply the actions.

    /*
    LOG_DBG("End of frame {} - frametime: {:g}ms",
    profiler.m_frameCount, timer::to_milliseconds(profiler.dt));
    */


//    auto frame = begin_frame();
//    auto frame = begin_frame();
//    draw_world(r3d,world);
//    draw_ui(r2d,ui);
    input_state.end_frame();
    profiler.end_frame();
}




Engine::~Engine(){
    LOG_THIS_DTOR();
}
Engine::Engine()
    : platform(initialWindowLogicalExtent)  // Creates window
    , input_state()                         // input state
    , r3d(platform.get_window_handle(),initialWindowLogicalExtent,platform.m_pixelDensity)
    , world()
    , cam()
    , ui(platform.get_window_handle())
    , profiler()
{

    LOG_THIS_CTOR();
    init_gui(ui, platform, r3d);
}
