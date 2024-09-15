// BrickBreaking.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "BrickBreaking.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND winhWnd;
static Bar bar;
static std::vector<std::vector<Brick>> bricks
{
    { 1, 2, 3, 4, 5, 4, 3, 2, 1 },
    { 1, 2, 3, 4, 5, 4, 3, 2, 1 },
    { 1, 2, 3, 4, 5, 4, 3, 2, 1 },
    { 1, 2, 3, 4, 5, 4, 3, 2, 1 },
    { 1, 1, 3, 4, 5, 4, 3, 2, 1 },
};
static std::vector<Ball> balls(100);
static std::vector<Item> items(10);
float subPosX = 0;

void TrySpawnBall(Vector2 spawnPos)
{
    for (int i = 0; i < balls.size(); i++)
    {
        if (balls[i].isUsing == false)
        {
            balls[i].SpawnBall(spawnPos, Vector2(-2, -2), 10, 10);
            break;
        }
    }
}

void TrySpawnItem(Vector2 spawnPos)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].isUsing == false)
        {
            items[i].SpawnItem(spawnPos);
            break;
        }
    }
}

void SetBricks(std::vector<std::vector<Brick>>& Bricks)
{
    float startPosX = 150;
    float startPosY = 100;

    for (int y = 0; y < Bricks.size(); y++)
    {
        for (int x = 0; x < Bricks[y].size(); x++)
        {
            Bricks[y][x].SetBrick(Vector2{ startPosX + ((x * 40 * 2) + (x * 2)) , startPosY + ((y * 20 * 2) + (y * 2)) });
        }
    }
}

void OnSplitBallEvnet()
{
    for (int i = 0; i < balls.size(); i++)
    {
        if (balls[i].isUsing == false)
        {
            std::random_device rd;
            std::default_random_engine g(rd());
            std::uniform_int_distribution<int> random(-2, 2);
            Vector2 velocity = { (float)random(g), -2 };
            Vector2 initPos = { bar.GetPos().x, bar.GetPos().y - 20 };
            balls[i].SpawnBall(initPos, velocity, 10, 10);
            break;
        }
    }
}

void OnLengthenBarEvent()
{
    bar.SetLength(20);
}

void OnClingBallEvent()
{
    if (bar.IsClingBall() == false)
        bar.isClingBall = true;
}

void Start()
{
    balls[0].SpawnBall(Vector2(g_screenX / 2, g_screenY - 120), Vector2(-2, -2), 10, 10);
    SetBricks(bricks);
}

void Update()
{
    if (bar.IsClingBall() && GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        balls[bar.ballNum].isClingBar = false;
        bar.ShotBall();
        subPosX = 0;
    }

    // 볼
    int ballUsingCount = 0;
    for (int i = 0; i < balls.size(); i++)
    {
        if (balls[i].isUsing == false)
            continue;

        ballUsingCount++;
        balls[i].Update();

        if (balls[i].isClingBar)
        {
            balls[i].pos.x = bar.GetPos().x - subPosX;
        }

        if (balls[i].CheckCollisionBar(bar) && bar.isClingBall)
        {
            subPosX = bar.GetPos().x - balls[i].pos.x;

            bar.ConnectBall(i);
            balls[i].isClingBar = true;
            bar.isClingBall = false;
        }

        for (std::vector<Brick>& brick : bricks)
            for (Brick& b : brick)
                if (b.GetCount() > 0)
                    balls[i].CheckCollisionBrick(b);
    }

    if (ballUsingCount == 0)
    {
        // 게임 오버
    }

    int blockUsingCount = 0;
    for (std::vector<Brick>& brick : bricks)
        for (Brick& b : brick)
            if (b.GetCount() > 0)
                blockUsingCount++;

    if (blockUsingCount == 0)
    {
        // 게임 클리어
    }

    // 아이템
    for (Item& item : items)
    {
        if (item.isUsing == false)
            continue;

        item.Update();

        if (bar.IsCheckCollisionItem(item))
        {
            bar.OnUseItem(item.GetType());
            item.isUsing = false;
        }
    }

    // 바
    bar.Update();
}

void Draw(HWND& hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
    SelectObject(memDC, memBitmap);

    FillRect(memDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    // 아이템
    for (Item& item : items)
    {
        if (item.isUsing == false)
            continue;

        item.Draw(memDC);
    }

    // 블럭
    for (std::vector<Brick>& brick : bricks)
    {
        for (Brick& b : brick)
        {
            if (b.GetCount() > 0)
                b.Draw(memDC);
            else
                b = NULL;
        }
    }

    // 공
    for (Ball& ball : balls)
    {
        if (ball.isUsing == false)
            continue;

        ball.Draw(memDC);
    }

    // 바
    bar.Draw(memDC);

    BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, memDC, 0, 0, SRCCOPY);

    DeleteObject(memBitmap);
    DeleteDC(memDC);

    EndPaint(hWnd, &ps);
}

void OnMouseMoveEvent(LPARAM lParam)
{
    float posX = GET_X_LPARAM(lParam);
    bar.SetPosX(posX);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BRICKBREAKING, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BRICKBREAKING));
    MSG msg;

    DWORD previousTime = GetTickCount64();
    float deltaTime = 0.0f;

    while (1)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return (int)msg.wParam;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            DWORD currentTime = GetTickCount64();
            deltaTime += (currentTime - previousTime);
            previousTime = currentTime;

            if (deltaTime > 10)
            {
                Update();
                deltaTime = 0;
                InvalidateRect(winhWnd, NULL, FALSE);
            }
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0; 
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BRICKBREAKING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BRICKBREAKING);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, _T("인하프로젝트"), WS_OVERLAPPEDWINDOW,
        0, 0, g_screenX, g_screenY, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    winhWnd = hWnd; // 전역 핸들 설정
    return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;  
    case WM_CREATE:
        Start();
        break;
    case WM_MOUSEMOVE:
        OnMouseMoveEvent(lParam);
        break;
    case WM_PAINT:
        Draw(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
