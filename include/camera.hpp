#pragma once
#ifndef __cplusplus
	#error "This header is intended for c++ only."
#endif // __cplusplus
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "logger.hpp"
#include "primitive_types.hpp"
#include "glm/trigonometric.hpp"
struct Camera{
    Camera();
    ~Camera();
    static inline constexpr auto WORLD_UP = glm::vec3{0,1,0};

    f32 zoom_sens {0.1f};
    f32 look_speed {1.0f};

    bool m_isZoomed{false};
    f32 zoom_multiplier {0.25f}, zoom_multiplier_min{0.10f}, zoom_multiplier_max{0.85f}; 
    void zoom_in(){ m_isZoomed = true; }
    void zoom_out(){ m_isZoomed = false; }

    // delta of 1.1 means the zoom will increase by 10%, and thus the zoomed_fov will DECREASE by 10%.
    auto increase_zoom_mult(f32 delta){
        ASSERT(delta >= 1.0f);
        zoom_multiplier = std::clamp(zoom_multiplier / delta, zoom_multiplier_min, zoom_multiplier_max);
    }
    // delta of 1.1 means the zoom will decrease by 10%, and thus the zoomed_fov will INCREASE by 10%.
    auto decrease_zoom_mult(f32 delta){
        ASSERT(delta >= 1.0f);
        zoom_multiplier = std::clamp(zoom_multiplier * delta, zoom_multiplier_min, zoom_multiplier_max);
    }
    auto vertical_fov() const -> f32{
        // TODO: one day make a smooth zoom animation
        if (m_isZoomed){
            return vfov * zoom_multiplier;
        }else{
            return vfov;
        }
    }

    glm::vec3 pos{6.4, 3.8, 4.5};
    f32 yaw{170.0f},pitch{-10.0f};

    void look_around(glm::vec2 rel){
        rotate_yaw(rel.x);
        rotate_pitch(rel.y);
    }

    glm::vec3 get_facing() const{
        return glm::vec3{
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)),
            glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        };
    } 

    auto get_proj_matrix(f32 aspect)const{
        return glm::perspective(glm::radians(vertical_fov()), aspect, zfar,znear); 
    }
    auto get_view_matrix()       const{
        return glm::lookAt(pos, pos + get_front(), WORLD_UP); 
    }
    glm::vec3  get_up() const {
        return glm::vec3{0,1.0f,0}; 
    }
    glm::vec3 get_right() const {
        return glm::cross(get_facing(), get_up());
    }
    glm::vec3 get_front() const {
        return glm::normalize(get_facing());
    }

    auto move(glm::vec3 direction, f32 scale=1.0f){
        pos += direction * scale * move_speed;
    }
    void rotate_pitch(f32 pitch_rel_01) {
        pitch = glm::clamp(pitch - pitch_rel_01 * vertical_fov()* look_speed, -89.0f, 89.0f); 
    }
    void rotate_yaw(f32 yaw_rel_01) { 
        yaw = yaw + yaw_rel_01 * vertical_fov() * look_speed;
    }

    void rotate_right(f32 rotate_dist){
        yaw += rotate_dist;
    }
    void rotate_left(f32 rotate_dist){
        yaw -= rotate_dist;
    }
    void rotate_up(f32 rotate_dist){
        pitch += rotate_dist;
    }
    void rotate_down(f32 rotate_dist){
        pitch -= rotate_dist;
    }

    static constexpr auto znear = f32{0.01f};
    static constexpr auto zfar = f32{100.0f};
    glm::vec3 facing{};
private:
    f32 move_speed {1.0f}, move_speed_min{0.1f}, move_speed_max{10.0f};
    f32 vfov {60.0f}, vfov_min {10.0f}, vfov_max {120.0f};
};
