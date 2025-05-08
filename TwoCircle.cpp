#include <windows.h>
#include <vector>
#include <cmath>
//  g++ TwoCircle.cpp -o TwoCircle.exe -mwindows -lgdi32 -luser32
// .\TwoCircle.exe

using namespace std;

vector<POINT> Points;

void drawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 > x1) ? 1 : -1;
    int sy = (y2 > y1) ? 1 : -1;
    bool isSteep = dy > dx;

    if (isSteep) {
        swap(x1, y1);
        swap(x2, y2);
        swap(dx, dy);
        swap(sx, sy);
    }

    int d = 2 * dy - dx;
    int x = x1;
    int y = y1;

    for (int i = 0; i <= dx; i++) {
        if (isSteep)
            SetPixel(hdc, y, x, color);
        else
       SetPixel(hdc, x, y, color);
        x += sx;
        if (d > 0) {
            y += sy;
            d -= 2 * dx;
        }
        d += 2 * dy;
    }
}

void drawDiagonalsInSectors(HDC hdc, int xc, int yc, int r, COLORREF color) {
    const double PI = 3.14159265358979323846;
    const double angleStep = PI / 180;

    for (double angle = 0; angle <= PI / 4; angle += angleStep) {
        int x = (xc + r * cos(angle));
        int y = (yc - r * sin(angle));
        drawLineBresenham(hdc, xc, yc, x, y, color);
    }

    for (double angle = PI; angle <= 5 * PI / 4; angle += angleStep) {
        int x = (xc + r * cos(angle));
        int y = (yc - r * sin(angle));
        drawLineBresenham(hdc, xc, yc, x, y, color);
    }
}

void draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color) {
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);
    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc + y, yc - x, color);
    SetPixel(hdc, xc - y, yc - x, color);
}

void drawBresenhamCircle(HDC hdc, int xc, int yc, int r, COLORREF color) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    draw8Points(hdc, xc, yc, x, y, color);
    while (x <= y) {
        x++;
        if (d < 0)
            d = d + 4 * x + 6;
        else {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        draw8Points(hdc, xc, yc, x, y, color);
    }
}

void DrawTwoCircle(HDC hdc) {
    if (Points.size() < 3) return;

    int x1 = Points[0].x, y1 = Points[0].y;
    int x2 = Points[1].x, y2 = Points[1].y;
    int x3 = Points[2].x, y3 = Points[2].y;

    int r1 = (int)hypot(x2 - x1, y2 - y1);
    int r2 = (int)hypot(x3 - x2, y3 - y2);

    drawBresenhamCircle(hdc, x1, y1, r1, RGB(255, 0, 0));
    drawBresenhamCircle(hdc, x3, y3, r2, RGB(0, 0, 255));
    drawDiagonalsInSectors(hdc, x3, y3, r2, RGB(180, 155, 200));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_LBUTTONDOWN: {
            if (Points.size() < 3) {
                POINT p = { LOWORD(lParam), HIWORD(lParam) };
                Points.push_back(p);
            }
            if (Points.size() == 3)
                InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawTwoCircle(hdc);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const char szWindowClass[] = "TwoCircleClass";
    WNDCLASSA wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szWindowClass;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    RegisterClassA(&wc);

    HWND hWnd = CreateWindowA(szWindowClass, "Two Circles", WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg = { 0 };
    while (GetMessageA(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}
