#pragma once
#include "Vector.hpp"
#include <iostream>

class Camera
{
public:
	Camera();
    Camera(const Vector3f& pos, const Vector3f& gazeDir, const Vector3f& upDir, 
        const float& fov, const float& ratio, const float& near, const float& far);
	~Camera();

    Vector3f position;
    Vector3f gazeDirection;
    Vector3f upDirection;
    float FOVy, aspectRatio;
    float nearPlane, farPlane;

    void set_position(const Vector3f& pos);
    Vector3f get_gaze_direction() const;
    Vector3f get_position() const;
    Vector3f get_up_direction() const;
    float get_FOVy() const;
    float get_near_plane() const;
    float get_aspect_ratio() const;
    float get_far_plane() const;
    void show_information() const;

private:

};