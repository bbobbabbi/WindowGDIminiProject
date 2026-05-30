#pragma once
#include "pch.h"
#include "MyRender.h"
#include "renderHelp.h"
#include "GameObject.h"
#include <assert.h>


GameObject::~GameObject()
{
    if (myCollider)
    {
        delete myCollider;
        myCollider = nullptr;
    }
}

void GameObject::Update(float deltaTime)
{
    Move(deltaTime);
    //UpdateFrame(deltaTime);
    // Collider 업데이트
    if (myCollider)
    {
        myCollider->center = m_Wpos;
    }
}

void GameObject::Render(MyRender& render)
{
    render.DrawCollider(myCollider);
}


void GameObject::SetColliderCircle(float radius)
{
    if (myCollider)
    {
        delete myCollider;
        myCollider = nullptr;
    }

    learning::ColliderCircle* circleP = new learning::ColliderCircle;
    myCollider = dynamic_cast<learning::Collider*>(circleP);

    assert(circleP != nullptr && "Failed to create ColliderCircle!");
    assert(myCollider != nullptr && "Failed to create ColliderCircle!");

    circleP->radius = radius;
    circleP->center = m_Wpos;
}



void GameObject::SetColliderBox(float width, float height)
{
    if (myCollider)
    {
        delete myCollider;
        myCollider = nullptr;
    }

    learning::ColliderBox* boxP = new learning::ColliderBox;
    myCollider = dynamic_cast<learning::Collider*>(boxP);

    assert(boxP != nullptr && "Failed to create ColliderBox!");
    assert(myCollider != nullptr && "Failed to create ColliderBox!");

    boxP->center = m_Wpos;
    boxP->halfSize.x = width / 2.0f;
    boxP->halfSize.y = height / 2.0f;
}


void GameObject::SetWidthAndHeight(float width, float height) {
    GameObject::m_width = 100 * width;
    GameObject::m_height = 100 * height;
}


learning::Collider* GameObject::GetCollider() {

    if (myCollider) {
        return myCollider;
    }
}


void  GameObject::Move(float deltaTime) 
{
    GameObjectBase::Move(deltaTime);
}

void GameObjectBase::SetName(const char* name)
{
    if (name == nullptr) return;

    strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
    m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}