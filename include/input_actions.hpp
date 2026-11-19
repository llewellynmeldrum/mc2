#pragma once
#include "glm_types.hpp"
#include "libassert/assert.hpp"
#include "auto_format.hpp"
// 'Actions' are associated with 'events' via a 'Binding'.
// A binding maps an Event (and its data) to a Binding (and its data)
struct MoveCameraAction{
    glm::vec3 delta_world; // must be scaled by the thingamabob
};
struct LookCameraAction{
    glm::vec2 delta;
};

// i.e optifine zoom button was pressed (or released)
struct ZoomCameraAction{
    bool zoom_in{true};
};
// i.e scrolling to change the zoom amount
struct ModifyZoomCameraAction{
    f32 delta; 
};
struct EngineQuitAction{
};
using Action = std::variant<
    MoveCameraAction,
    LookCameraAction,
    ZoomCameraAction,
    ModifyZoomCameraAction,
    EngineQuitAction
>;
