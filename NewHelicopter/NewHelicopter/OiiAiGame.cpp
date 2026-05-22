#include "pch.h"
#include "OiiAiGame.h" 
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "MyRender.h"
#include "Player.h"
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
   // UpdateWholeIntersect();
    //UpdateEnemyInfo();
    if (m_CirEnemySpawnPos.x != 0 && m_CirEnemySpawnPos.y != 0)
    {
        CreateCircleEnemy();
    }
    else if (m_BoxEnemySpawnPos.x != 0 && m_BoxEnemySpawnPos.y != 0) {

        CreateBoxEnemy();
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
    }
}

void OiiAGame::Render()
{
    render->Render(MAX_GAME_OBJECT_COUNT, m_GameObjectPtrTable);
}


void OiiAGame::CreatePlayer()
{
    assert(m_GameObjectPtrTable[0] == nullptr && "Player object already exists!");

    GameObject* pNewObject = new Player(ObjectType::PLAYER);

    pNewObject->SetName("Player");
    
    int x = (GetWidth()) / 2;
    int y = (GetHeight()) / 2;
    pNewObject->SetPosition(x, y+200); // 일단, 임의로 설정 
    pNewObject->SetSpeed(0.0f); // 일단, 임의로 설정   

    pNewObject->SetColliderBox(50.0f,50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    m_GameObjectPtrTable[0] = pNewObject;
}

void OiiAGame::CreateCircleEnemy()
{
    GameObject* pNewObject = new GameObject(ObjectType::ENEMY);

    pNewObject->SetName("Enemy");

    float x = m_CirEnemySpawnPos.x;
    float y = m_CirEnemySpawnPos.y;

    m_CirEnemySpawnPos = { 0, 0 };

    pNewObject->SetPosition(x, y);
    pNewObject->SetSpeed(1.0f); // 일단, 임의로 설정   

    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    bool isInter = false;

    learning::Collider* thisCollider = nullptr;

    Vector2f firstDir;

    //충돌 처리
    int j = 0;
    int t = 0;

    while (j < MAX_GAME_OBJECT_COUNT) {
        thisCollider = pNewObject->GetCollider();
        GameObjectBase* target = m_GameObjectPtrTable[j];
        if (target == nullptr) break;

        learning::Collider* targetCollider = target->GetCollider();

        if (thisCollider->IsIntersect(targetCollider)) {
            // 그리는 위치 업데이트
            OiiAGame::SettingCirPos(thisCollider, targetCollider, pNewObject);
            j = 0;
            ++t;
            //판정상 무한루프를 빠질 때가 있어서 그것 처리
            if (t == MAX_GAME_OBJECT_COUNT) {
                isInter = true;
                break;
            }
            continue;
        }
        ++j;
    }


    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT && !isInter) //0번째는 언제나 플레이어!
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            break;
        }
    }

    if (i == MAX_GAME_OBJECT_COUNT || isInter)
    {
        // 게임 오브젝트 테이블이 가득 찼습니다.
        delete pNewObject;
        pNewObject = nullptr;
        isInter = false;
    }

}
void OiiAGame::CreateBoxEnemy()
{
    GameObject* pNewObject = new GameObject(ObjectType::ENEMY);

    pNewObject->SetName("Enemy");

    float x = m_BoxEnemySpawnPos.x;
    float y = m_BoxEnemySpawnPos.y;

    m_BoxEnemySpawnPos = { 0, 0 };

    pNewObject->SetPosition(x, y);
    pNewObject->SetSpeed(1.0f); // 일단, 임의로 설정   

    pNewObject->SetColliderBox(50.0f, 50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    bool isInter = false;

    learning::Collider* thisCollider = nullptr;

    Vector2f firstDir;

    //충돌 처리
    int j = 0;
    int t = 0;
    while (j < MAX_GAME_OBJECT_COUNT) {
        thisCollider = pNewObject->GetCollider();
        GameObjectBase* target = m_GameObjectPtrTable[j];
        if (target == nullptr) break;

        learning::Collider* targetCollider = target->GetCollider();

        if (thisCollider->IsIntersect(targetCollider)) {
            //firstDir이 초기값일 때 
            if (firstDir.x == 0 && firstDir.y == 0) {
                firstDir = OiiAGame::GetBoxDir(thisCollider, targetCollider);
            }
            // 그리는 위치 업데이트
            OiiAGame::SettingBoxPos(thisCollider, targetCollider, pNewObject, firstDir);
            j = 0;
            ++t;
            //판정상 무한루프를 빠질 때가 있어서 그것 처리
            if (t == MAX_GAME_OBJECT_COUNT) {
                isInter = true;
                break;
            }
            continue;
        }
        ++j;
    }



    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT && !isInter) //0번째는 언제나 플레이어!
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            break;
        }
    }

    if (i == MAX_GAME_OBJECT_COUNT || isInter)
    {
        // 게임 오브젝트 테이블이 가득 찼습니다.
        delete pNewObject;
        pNewObject = nullptr;
        isInter = false;
    }
}
void OiiAGame::SettingCirPos(learning::Collider* thisCir, learning::Collider* targetCir, GameObject* pThis) {
    auto a = dynamic_cast<ColliderCircle*>(thisCir);
    float radius = a->radius;
    Vector2f thisPos = thisCir->center;
    Vector2f targetPos = targetCir->center;
    Vector2f dir = thisPos - targetPos;
    float x = targetCir->center.x;
    float y = targetCir->center.y;
    //0.5f는 float 계산으로 생기는 오차 때문에 온전하게 밀어주는 길이가 radius * 2 가 되질 않아 더해준다.
    thisCir->center += dir.Normalized() * ((radius * 2 + 0.5f) * (1 - (dir.Length() / (radius * 2))));
    pThis->SetPosition(thisCir->center.x, thisCir->center.y);
}

Vector2f OiiAGame::GetBoxDir(learning::Collider* thisBox, learning::Collider* targetBox) {
    auto a = dynamic_cast<ColliderBox*>(thisBox);
    float boxSize = a->halfSize.x * 2;
    Vector2f thisPos = thisBox->center;
    Vector2f targetPos = targetBox->center;
    Vector2f dir = thisPos - targetPos;
    return dir.Normalized();
}

void OiiAGame::SettingBoxPos(learning::Collider* thisBox, learning::Collider* targetBox, GameObject* pThis, Vector2f firstDir) {
    auto a = dynamic_cast<ColliderBox*>(thisBox);
    float boxSize = a->halfSize.x * 2;
    Vector2f thisPos = thisBox->center;
    Vector2f targetPos = targetBox->center;
    Vector2f dir = thisPos - targetPos;
    float mScala = dir.Length();
    float x = targetBox->center.x;
    float y = targetBox->center.y;
    thisBox->center += firstDir * ((boxSize * 1.41f) * (1 - (mScala / (boxSize * 1.41f))));
    thisBox->center.x = FClamp(thisBox->center.x, x - boxSize, x + boxSize);
    thisBox->center.y = FClamp(thisBox->center.y, y - boxSize, y + boxSize);
    pThis->SetPosition(thisBox->center.x, thisBox->center.y);
}

float OiiAGame::FClamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
};

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


        //적끼리 충돌
        for (int i = j + 1; i < MAX_GAME_OBJECT_COUNT; i++) {
            if (m_GameObjectPtrTable[i]) {
                GameObjectBase* targetEnemy = m_GameObjectPtrTable[i];
                learning::Collider* targetEnemyCollider = targetEnemy->GetCollider();
                if (targetCollider->IsIntersect(targetEnemyCollider)) {
                    targetCollider->isEnemyIntersect = true;
                    targetEnemyCollider->isEnemyIntersect = true;
                }
            }
        }

        //플레이어 와 적 오브젝트 끼리 검사
        if (playerCollider->IsIntersect(targetCollider)) {
            targetCollider->isPlayerIntersect = true;
            playerCollider->isPlayerIntersect = true;
        }

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
    /*  std::cout << __FUNCTION__ << std::endl;
   std::cout << "x: " << x << ", y: " << y << std::endl;*/

    m_CirEnemySpawnPos.x = x;
    m_CirEnemySpawnPos.y = y;
}

void OiiAGame::OnMButtonDown(int x, int y)
{
    m_BoxEnemySpawnPos.x = x;
    m_BoxEnemySpawnPos.y = y;
}
