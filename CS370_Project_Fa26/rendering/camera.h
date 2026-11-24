//
// Created by ethan on 11/18/2026.
//

#ifndef CS370_FA26_CAMERA_H
#define CS370_FA26_CAMERA_H


#include "../common/vmath.h"


enum class CameraMode { ORTHO, PERSPECTIVE };

class Camera {
public:
    Camera() = default;
    explicit Camera(CameraMode mode);

    void set_mode(CameraMode mode);
    CameraMode get_mode() const;

    void set_eye(const vmath::vec3& e);

    vmath::vec3 get_eye() const;

    void set_center(const vmath::vec3& c);

    vmath::vec3 get_center() const;

    void set_up(const vmath::vec3& u);

    vmath::vec3 get_up() const;

    void set_azimuth(float a);
    float get_azimuth() const;

    void set_elevation(float e);
    float get_elevation() const;

    void set_radius(float r);          // mainly used by ortho
    float get_radius() const;

    void move_forward(float distance);
    void move_right(float distance);

    void add_azimuth(float delta);
    void add_elevation(float delta);

    void update_matrices(int width, int height);

    const vmath::mat4& get_proj_matrix() const;
    const vmath::mat4& get_view_matrix() const;

private:
    CameraMode mode = CameraMode::ORTHO;

    vmath::vec3 eye = {3.0f, 3.0f, 3.0f};
    vmath::vec3 center = {0.0f, 0.0f, 0.0f};
    vmath::vec3 up = {0.0f, 1.0f, 0.0f};

    float azimuth = 320.0f;
    float elevation = 60.0f;
    float radius = 17.5f;

    vmath::mat4 projMatrix;
    vmath::mat4 viewMatrix;
};

#endif //CS370_FA26_CAMERA_H
