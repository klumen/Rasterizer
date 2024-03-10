#include "Camera.hpp"

Camera::Camera() : FOVy(60.0f), aspectRatio(16.0f/9.0f), nearPlane(-0.05f), farPlane(-1000.0f)
{
    position = Vector3f(0.0f, 0.0f, 0.0f);
    gazeDirection = Vector3f(0.0f, 0.0f, -1.0f);
    upDirection = Vector3f(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const Vector3f& pos, const Vector3f& gazeDir, const Vector3f& upDir, 
    const float& fov, const float& ratio, const float& near, const float& far)
    : position(pos), gazeDirection(gazeDir), upDirection(upDir), 
    FOVy(fov), aspectRatio(ratio), nearPlane(near), farPlane(far) {}

Camera::~Camera() {}

void Camera::set_position(const Vector3f& pos) { position = pos; }

Vector3f Camera::get_gaze_direction() const { return gazeDirection; }

Vector3f Camera::get_position() const { return position; }

Vector3f Camera::get_up_direction() const { return upDirection; }

float Camera::get_FOVy() const { return FOVy; }

float Camera::get_near_plane() const { return nearPlane; }

float Camera::get_aspect_ratio() const { return aspectRatio; }

float Camera::get_far_plane() const { return farPlane; }

void Camera::show_information() const
{
    std::cout << "FOV: " << FOVy << std::endl;
    std::cout << "Near plane: " << nearPlane << std::endl;
    std::cout << "far plane: " << farPlane << std::endl;
    std::cout << "Position: " << position << std::endl;
    std::cout << "Gaze direction: " << gazeDirection << std::endl;
    std::cout << "Up direction: " << upDirection << std::endl;
}
