#include "pch.h"
#include "OiiAiGame.h" 
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "MyRender.h"
#include "Player.h"
#include "platform.h"
#include <iostream>
#include <assert.h>


using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

class D2DRenderSystem
{
public:
};

bool OiiAGame::Initialize()
{
    m_pGameTimer = new GameTimer();
    m_pGameTimer->Reset();
    render = new MyRender();


    const wchar_t* className = L"D2DLesson01";
    const wchar_t* windowName = L"Hello, DirectX";

    if (false == __super::Create(className, windowName, 800, 600))
    {
        return false;
    }

    render->InitMyRender(m_hWnd);

    m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        m_GameObjectPtrTable[i] = nullptr;
    }

    CreatePlayer();
    //////////////////////////////////////////////////
    //////////////////////////////////////////////////
    //디버깅 용 플랫폼 하나 초기화
    Platform* pNewObject = new Platform(ObjectType::PLATFORM);

    pNewObject->SetName("Platform");

    int x = (GetWidth()) / 2;
    int y = (GetHeight())-15;
    m_platformSpawnPos = { 0, 0 };

    pNewObject->SetPosition(x, y);
    pNewObject->SetSpeed(1.0f); // 일단, 임의로 설정   

    pNewObject->SetColliderBox(100.0f, 30.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
    pNewObject->platfCol = dynamic_cast<learning::ColliderBox*>(pNewObject->GetCollider());
    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어!
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            break;
        }
    }

    if (i == MAX_GAME_OBJECT_COUNT)
    {
        // 게임 오브젝트 테이블이 가득 찼습니다.
        delete pNewObject;
        pNewObject = nullptr;
    }
    //////////////////////////////////////////////////
    //////////////////////////////////////////////////



    return true;
}

void OiiAGame::Run()
{
    MSG msg = { 0 };     //디스패치하지 않고(프로시져를 거치지 않고) 이렇게 처리할 수도 있다.
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                OiiAGame::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_LBUTTONUP)
            {
                OiiAGame::OnLButtonUp(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_MOUSEMOVE)
            {
                OiiAGame::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_RBUTTONDOWN)
            {
                OiiAGame::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }
}

void OiiAGame::Finalize()
{
    delete m_pGameTimer;
    delete render;

    m_pGameTimer = nullptr;

    if (m_GameObjectPtrTable)
    {
        for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
        {
            if (m_GameObjectPtrTable[i])
            {
                delete m_GameObjectPtrTable[i];
                m_GameObjectPtrTable[i] = nullptr;
            }
        }
        delete m_GameObjectPtrTable;
    }
    __super::Destroy();
}

void OiiAGame::Update()
{
    m_pGameTimer->Tick();

    LogicUpdate();

    m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
    m_fFrameCount += m_fDeltaTime;


    while (m_fFrameCount >= 2.0f)
    {
        FixedUpdate();
        m_fFrameCount -= 2.0f;
    }
}

void OiiAGame::FixedUpdate()
{
    //플레이어 가속도 추가 (움직임)
    if (isMouseDown) {
        GetPlayer()->Accelat(m_fDeltaTime);
    }
    else {
        GetPlayer()->DeAccelat(m_fDeltaTime);
    }
    UpdateWholeIntersect();
    //UpdateEnemyInfo();
    if (m_CirEnemySpawnPos.x != 0 && m_CirEnemySpawnPos.y != 0)
    {
       // CreateCircleEnemy();
    }
    else if (m_platformSpawnPos.x != 0 && m_platformSpawnPos.y != 0) {
        CreatePlatform();
    }
}

void OiiAGame::LogicUpdate()
{

    UpdatePlayerInfo();
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(m_fDeltaTime);
        }
        else {
            break;
        }
    }
}

void OiiAGame::Render()
{
    render->Render(MAX_GAME_OBJECT_COUNT, m_GameObjectPtrTable);
}


void OiiAGame::CreatePlayer()
{
    assert(m_GameObjectPtrTable[0] == nullptr && "Player object already exists!");

    Player* pNewObject = new Player(ObjectType::PLAYER);

    pNewObject->SetName("Player");
    
    int x = (GetWidth()) / 2;
    int y = (GetHeight()) / 2;
    pNewObject->SetPosition(x, y+200); // 일단, 임의로 설정 
    pNewObject->SetSpeed(0.0f); // 일단, 임의로 설정   

    pNewObject->SetColliderBox(60.0f,40.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
    pNewObject->SetDetector(300);
    m_GameObjectPtrTable[0] = pNewObject;
}



void OiiAGame::CreatePlatform()
{
    Platform* pNewObject = new Platform(ObjectType::PLATFORM);

    pNewObject->SetName("Platform");

    float x = m_platformSpawnPos.x;
    float y = m_platformSpawnPos.y;
    m_platformSpawnPos = { 0, 0 };

    pNewObject->SetPosition(x, y);
    pNewObject->SetSpeed(1.0f); // 일단, 임의로 설정   

    pNewObject->SetColliderBox(100.0f, 30.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
    pNewObject->platfCol = dynamic_cast<learning::ColliderBox*>(pNewObject->GetCollider());
    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어!
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            break;
        }
    }

    if (i == MAX_GAME_OBJECT_COUNT)
    {
        // 게임 오브젝트 테이블이 가득 찼습니다.
        delete pNewObject;
        pNewObject = nullptr;
    }
}




Vector2f OiiAGame::GetBoxDir(learning::Collider* thisBox, learning::Collider* targetBox) {
    auto a = dynamic_cast<ColliderBox*>(thisBox);
    float boxSize = a->halfSize.x * 2;
    Vector2f thisPos = thisBox->center;
    Vector2f targetPos = targetBox->center;
    Vector2f dir = thisPos - targetPos;
    return dir.Normalized();
}

void OiiAGame::UpdateWholeIntersect() {
    static Player* pPlayer = GetPlayer();
    Vector2f playerPos = pPlayer->GetPosition();
    auto playerCollider = pPlayer->GetCollider();

    //전체 초기화
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; i++) {
        GameObjectBase* target = m_GameObjectPtrTable[i];
        if (target == nullptr) break;
        target->GetCollider()->isPlayerIntersect = false;
        target->GetCollider()->isEnemyIntersect = false;
    }

    int j = 1;
    while (j < MAX_GAME_OBJECT_COUNT) {
        GameObjectBase* target = m_GameObjectPtrTable[j];
        if (target == nullptr) break;

        learning::Collider* targetCollider = target->GetCollider();

        Platform* tempPlatform = dynamic_cast<Platform*>(target);
        //Enemy* tempEnemy = dynamic_cast<Enemy*>(target);

        //충돌한게 플랫폼일 때 
        if (playerCollider->IsIntersect(targetCollider)) {
            if (tempPlatform != nullptr) {
                playerCollider->isPlayerIntersect = true;
                targetCollider->isPlayerIntersect = true;
                //플랫폼에 플레이어 정보 넘기기
                tempPlatform->bumpedPlayer = pPlayer;
                tempPlatform->playerfCol = dynamic_cast<learning::ColliderBox*>(pPlayer->GetCollider());
            }
        }
        //else if(tempEnemy != nullptr){}
        tempPlatform = nullptr;
        ++j;
    }
}


void OiiAGame::UpdatePlayerInfo()
{
    static Player* pPlayer = dynamic_cast<Player*>(GetPlayer()) ;
    assert(pPlayer != nullptr);
    Vector2f playerDir = pPlayer->GetUpDir();
    pPlayer->SetDirection(playerDir);
}


//void OiiAGame::UpdateEnemyInfo()

//void OiiAGame::UpdateEnemyInfo()
//{
//    static GameObject* pPlayer = GetPlayer();
//    assert(pPlayer != nullptr);
//
//    const Vector2f playerPos = pPlayer->GetPosition();
//    learning::ColliderCircle* playerCollider = dynamic_cast<ColliderCircle*>(pPlayer->GetCollider());
//    assert(playerCollider != nullptr);
//
//
//    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i)
//    {
//        GameObjectBase* enemy = m_GameObjectPtrTable[i];
//        if (enemy == nullptr)
//            continue;
//        //반지름을 가져오기 위해 콜라이더 가져오기
//        learning::ColliderCircle* enemyCollider = dynamic_cast<ColliderCircle*>(enemy->GetCollider());
//        if (enemyCollider == nullptr)
//            continue;
//
//        Vector2f enemyPos = enemy->GetPosition();
//        //적으로 부터 플레이어의 방향 얻기
//        Vector2f chaseDir = playerPos - enemyPos;
//        float chaseDirLength = chaseDir.Length();
//
//        if (chaseDirLength > 0.0f)
//            chaseDir = chaseDir.Normalized();
//
//        // 만약 플레이어랑 충돌 하지 않고 있다면
//        if (!enemyCollider->isPlayerIntersect)
//        {
//            float buffer = 1.0f;
//            // 밑의 거리만큼 떨어져 있을 때만 플레이어 방향으로 이동 //충돌 처리를 예쁘게 하고 싶어 buffer 추가
//            if (chaseDirLength >= (playerCollider->radius + enemyCollider->radius) + buffer) {
//                enemy->SetDirection(chaseDir);
//                enemy->SetSpeed(0.1f);
//            }
//        } // 플레이어랑 충돌하고 있다면
//        else
//        {   //떨림 방지를 위한 멈춤
//            enemy->SetSpeed(0.0f);
//            //충돌중이니 둘간의 사이가 반지름 합보다 작음
//            float pushLeng = (playerCollider->radius + enemyCollider->radius) - chaseDirLength;
//            // enemy 에서 플레이어 반대 방향으로 겹친 거리의 절반 만큼 이동한 위치
//            Vector2f pushedPos = (enemyPos - (chaseDir * pushLeng / 2));
//            enemy->SetPosition(pushedPos.x, pushedPos.y);
//        }
//
//        // 적끼리의 충돌이 없다면 넘어가기
//        if (!enemyCollider->isEnemyIntersect)
//            continue;
//
//        Vector2f nowPos = enemyPos;
//        // 현재 적 객체와 나머지 적 전체와 충돌 유무 비교와 거리 비교
//        for (int j = i + 1; j < MAX_GAME_OBJECT_COUNT; ++j)
//        {
//            GameObjectBase* targetEnemy = m_GameObjectPtrTable[j];
//            if (targetEnemy == nullptr)
//                continue;
//
//            learning::ColliderCircle* targetEnemyCollider = dynamic_cast<ColliderCircle*>(targetEnemy->GetCollider());
//            if (targetEnemyCollider == nullptr)
//                continue;
//
//            // 충돌한 적이 얘가 아니라면 넘어가기
//            if (!targetEnemyCollider->isEnemyIntersect)
//                continue;
//
//            Vector2f targetPos = targetEnemy->GetPosition();
//            Vector2f pushDir = targetPos - nowPos;
//            float pushDirLength = pushDir.Length();
//
//            //
//            if (pushDirLength <= 0.0f)
//                continue;
//
//            if (pushDir.Length() > 0.0f)
//                pushDir = pushDir.Normalized();
//
//            float pushLeng = ((targetEnemyCollider->radius + enemyCollider->radius) - pushDirLength) / 2;
//            // 충돌이 떠있는 적이여도 직접 닿고 있는 얘가 아니라면 넘어가기
//            if (pushLeng <= 0.0f)
//                continue;
//
//            targetPos = targetPos + (pushDir * pushLeng);
//            targetEnemy->SetPosition(targetPos.x, targetPos.y);
//        }
//    }
//}



void OiiAGame::OnResize(int width, int height)
{
    std::cout << __FUNCTION__ << std::endl;

    learning::SetScreenSize(width, height);
    __super::OnResize(width, height);
    render->OnResize(m_width, m_height);

}

void OiiAGame::OnClose()
{
    render->OnClose(m_hWnd);
}

void OiiAGame::OnMouseMove(int x, int y)
{
    /*   std::cout << __FUNCTION__ << std::endl;
       std::cout << "x: " << x << ", y: " << y << std::endl;*/
    m_MousePosPrev = m_MousePos;
    m_MousePos = { x, y };
}

void OiiAGame::OnLButtonDown(int x, int y)
{
    /*  std::cout << __FUNCTION__ << std::endl;
 std::cout << "x: " << x << ", y: " << y << std::endl;*/
    isMouseDown = true;
    m_PlayerTargetPos.x = x;
    m_PlayerTargetPos.y = y;

}
void OiiAGame::OnLButtonUp(int x, int y) {
    isMouseDown = false;
}


void OiiAGame::OnRButtonDown(int x, int y)
{
    m_platformSpawnPos.x = x;
    m_platformSpawnPos.y = y;
}

void OiiAGame::OnMButtonDown(int x, int y)
{
 
}
