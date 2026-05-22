#pragma once

#include "GameObject.h"

class platform : public GameObject {
public:
    platform(const GameObject&) = delete;
    platform(ObjectType type) : GameObject(type) {}
    ~platform() override {};
};