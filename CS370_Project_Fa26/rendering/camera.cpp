//
// Created by ethan on 11/18/2026.
//

#include "camera.h"

Camera::Camera(CameraMode mode) : mode(mode) {}

void Camera::set_mode(CameraMode m) {
    mode = m;
}

CameraMode Camera::get_mode() const {
    return mode;
}

void Camera::set_eye(const vmath::vec3& e) {
    eye = e;
}

vmath::vec3 Camera::get_eye() const {
    return eye;
}

void Camera::set_center(const vmath::vec3& c) {
    center = c;
}

vmath::vec3 Camera::get_center() const {
    return center;
}

void Camera::set_up(const vmath::vec3& u) {
    up = u;
}

vmath::vec3 Camera::get_up() const {
    return up;
}

void Camera::set_azimuth(float a) {
    azimuth = a;
}

float Camera::get_azimuth() const {
    return azimuth;
}

void Camera::set_elevation(float e) {
    elevation = e;
}

float Camera::get_elevation() const {
    return elevation;
}

void Camera::set_radius(float r) {
    radius = r;
}

float Camera::get_radius() const {
    return radius;
}

void Camera::add_azimuth(float delta) {
    azimuth += delta;
}

void Camera::add_elevation(float delta) {
    elevation += delta;
}

void Camera::update_matrices(int width, int height) {
    float xRatio = 1.0f;
    float yRatio = 1.0f;

    if (width <= height)
        yRatio = static_cast<float>(height) / static_cast<float>(width);
    else
        xRatio = static_cast<float>(width) / static_cast<float>(height);

    if (mode == CameraMode::PERSPECTIVE) {
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        projMatrix = vmath::perspective(60.0f, aspect, 0.1f, 200.0f);

        float yaw   = vmath::deg2rad(azimuth);
        float pitch = vmath::deg2rad(elevation);

        vmath::vec3 dir = vmath::normalize(vmath::vec3(
            cos(yaw) * cos(pitch),
            sin(pitch),
            sin(yaw) * cos(pitch)
        ));

        center = eye + dir;
        viewMatrix = vmath::lookat(eye, center, up);
    } else {
        projMatrix = vmath::ortho(
            -radius * xRatio,  radius * xRatio,
            -radius * yRatio,  radius * yRatio,
            -100.0f, 100.0f
        );
        viewMatrix = vmath::lookat(eye, center, up);
    }
}

const vmath::mat4& Camera::get_proj_matrix() const {
    return projMatrix;
}

const vmath::mat4& Camera::get_view_matrix() const {
    return viewMatrix;
}

void Camera::move_forward(float distance) {
    float yaw   = vmath::deg2rad(azimuth);
    float pitch = vmath::deg2rad(elevation);

    vmath::vec3 forward(
        cosf(yaw) * cosf(pitch),
        sinf(pitch),
        sinf(yaw) * cosf(pitch)
    );
    forward = vmath::normalize(forward);

    eye    += forward * distance;
    center += forward * distance;
}

void Camera::move_right(float distance) {
    float yaw = vmath::deg2rad(azimuth);

    vmath::vec3 right(
        sinf(yaw),
        0.0f,
        -cosf(yaw)
    );
    right = vmath::normalize(right);

    eye    += right * distance;
    center += right * distance;
}