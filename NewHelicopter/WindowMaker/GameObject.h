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

    void SetPosition(float x, float y) { m_pos = { x, y }; }
    void SetDirection(Vector2f dir) { m_dir = dir; }
    void SetSpeed(float speed) { m_speed = speed; }
    void SetName(const char* name);

    ObjectType Type() const { return m_type; }

    const char* GetName() const { return m_name; }

    Vector2f GetPosition() const { return m_pos; }
    Vector2f GetDirection() const { return m_dir; }

    float GetSpeed() const { return m_speed; }

protected:

    void Move(float deltaTime)
    {
        m_pos.x += m_dir.x * m_speed * deltaTime;
        m_pos.y += m_dir.y * m_speed * deltaTime;
    }

protected:
    ObjectType m_type;

    Vector2f m_pos = { 0.0f, 0.0f };
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

    // Bitmap 정보
    BitmapInfo* m_pBitmapInfo = nullptr;

    int m_width = 100;
    int m_height = 100;

    // 점진적으로 예쁘게 고쳐 보아요.
    struct FrameFPos
    {
        int x;
        int y;
    };
    // 프레임 정보: 왜 14개냐고 물으시면 셌다고 밖에...:)
    // 
   /* FrameFPos m_frameXY[14] = { {0, 0}, {102, 0}, {204, 0}, {306, 0}, {408, 0},
    {0, 95}, {102, 95}, {204, 95}, {306, 95}, {408, 95},
    {0, 189}, {102, 189}, {204, 189}, {306, 189} };*/
    FrameFPos m_frameXY[14] = { };
    int m_frameWidth = 101;
    int m_frameHeight = 93;
    int m_frameIndex = 0;
    int m_frameCount = 14; // 프레임 수

    float m_frameTime = 0.0f;
    float m_frameDuration = 70.0f; // 임의 설정




    // Collider
    learning::Collider* myCollider = nullptr;
};