#pragma once
#include "pch.h"
#include "Utillity.h"
#include <algorithm>
// [CHECK]. namespace 포함해서 전방 선언해야 함
// 헤더에서는 절대 using namespace 사용 금지

namespace renderHelp {
    struct BitmapInfo;
}

namespace learning
{
    struct Collider;
    struct ColliderCircle;
    struct ColliderBox;
}

enum class ObjectType
{
    PLAYER,
    ENEMY,
    PLATFORM,
    BULLET,
    ITEM,
    BACKGROUND,
};

constexpr int OBJECT_NAME_LEN_MAX = 15;

class GameObjectBase
{
    using Vector2f = learning::Vector2f;
public:
    GameObjectBase() = delete;
    GameObjectBase(const GameObjectBase&) = delete;

    GameObjectBase(ObjectType type) : m_type(type) {}

    virtual ~GameObjectBase() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Render(MyRender& render) = 0;
    virtual learning::Collider* GetCollider() = 0;

    void SetWPosition(float x, float y) { m_Wpos = { x, y }; }
    void SetCPosition(Vector2f pos) { m_Cpos =pos; }
    void SetDirection(Vector2f dir) { m_dir = dir; }
    void SetSpeed(float speed) { m_speed = speed; }
    void SetName(const char* name);

    ObjectType Type() const { return m_type; }

    const char* GetName() const { return m_name; }

    Vector2f GetWPosition() const { return m_Wpos; }
    Vector2f GetCPosition() const { return m_Cpos; }
    Vector2f GetDirection() const { return m_dir; }

    float GetSpeed() const { return m_speed; }

protected:

    void Move(float deltaTime)
    {
        m_Wpos.x += m_dir.x * m_speed * deltaTime;
        m_Wpos.y += m_dir.y * m_speed * deltaTime;
    }

protected:
    ObjectType m_type;

    Vector2f m_Wpos = { 0.0f, 0.0f };
    Vector2f m_Cpos = { 0.0f, 0.0f };
    Vector2f m_dir = { 0.0f, 0.0f }; // 방향 (단위 벡터)

    float m_speed = 0.0f; // 속력

    char m_name[OBJECT_NAME_LEN_MAX] = "";
};

class GameObject : public GameObjectBase
{

    using BitmapInfo = renderHelp::BitmapInfo;


public:
    GameObject(const GameObject&) = delete;
    GameObject(ObjectType type) : GameObjectBase(type) {}
    ~GameObject() override;

    virtual void Update(float deltaTime) override;
    virtual void Render(MyRender& render) override;

    learning::Collider* GetCollider()override;

    void SetColliderCircle(float radius);
    void SetColliderBox(float halfWidth, float halfHeight);

    void SetWidthAndHeight(float width, float height);


protected:
    void virtual Move(float deltaTime);

//    void UpdateFrame(float deltaTime);

    BitmapInfo* m_pBitmapInfo = nullptr;

    int m_width = 100;
    int m_height = 100;

    // Collider
    learning::Collider* myCollider = nullptr;
};