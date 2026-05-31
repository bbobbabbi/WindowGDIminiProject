#pragma once

#include "NzWndBase.h"
#include "Player.h"
#include "Platform.h"
#include <wrl/client.h>



class GameTimer;
class GameObjectBase;
class GameObject;
class MyRender;
namespace renderHelp {
    class BitmapInfo;
}
namespace learning {
    class Vector2f;
    class Collider;
}


class OiiAGame : public NzWndBase
{
public:
    OiiAGame() = default;
    virtual ~OiiAGame() = default;

    bool Initialize();
    void Run();
    void Finalize();


private:
    void Update();
    void Render();

    void OnResize(int width, int height) override;
    void OnClose() override;

    void OnMouseMove(int x, int y);
    void OnLButtonDown(int x, int y);
    void OnLButtonUp(int x, int y);
    void OnRButtonDown(int x, int y);
    void OnMButtonDown(int x, int y);

    void FixedUpdate();
    void LogicUpdate();

    void CreatePlayer();
    void CreatePlatform();
    void UpdatePlayerInfo();
    //void UpdateEnemyInfo();
    void UpdateWholeIntersect();

    learning::Vector2f GetBoxDir(learning::Collider* thisBox, learning::Collider* targetBox);
    bool IsLandingOnPlatform(
        Player* player,
        learning::ColliderBox* playerBox,
        Platform* platform,
        learning::ColliderBox* platformBox
    );
    Player* GetPlayer() const { return (Player*)m_GameObjectPtrTable[0]; }

private:

    MyRender* render;
    // [CHECK] #8. 게임 타이머를 사용하여 프레임을 관리하는 예시.F
    GameTimer* m_pGameTimer = nullptr;
    float m_fDeltaTime = 0.0f;
    float m_fFrameCount = 0.0f;

    // [CHECK] #8. 게임 오브젝트를 관리하는 컨테이너.
    GameObjectBase** m_GameObjectPtrTable = nullptr;

    struct MOUSE_POS
    {
        int x = 0;
        int y = 0;

        bool operator!=(const MOUSE_POS& other) const //연산자 오버로딩 
        {
            return (x != other.x || y != other.y);
        }
    };

    MOUSE_POS m_MousePos = { 0, 0 };
    MOUSE_POS m_MousePosPrev = { 0, 0 };

    MOUSE_POS m_mousClickPos = { 0, 0 };
    MOUSE_POS m_CirEnemySpawnPos = { 0, 0 };
    MOUSE_POS m_platformSpawnWPos = { 0, 0 };
    MOUSE_POS m_platformSpawnCPos = { 0, 0 };


    renderHelp::BitmapInfo* m_pPlayerBitmapInfo = nullptr;

    renderHelp::BitmapInfo* m_pEnemyBitmapInfo = nullptr;
};
