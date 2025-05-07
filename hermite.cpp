#include <windows.h>
#include <cmath>

POINT points[4];
int count = 0;
int chosenAlgorithm = -1;

void drawBezierCurve(HDC hdc, POINT* points, int steps) {
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        float u = 1.0f - t;
        float u3 = u * u * u;
        float u2t = 3 * u * u * t;
        float ut2 = 3 * u * t * t;
        float t3 = t * t * t;

        float x = u3 * points[0].x + u2t * points[1].x + ut2 * points[2].x + t3 * points[3].x;
        float y = u3 * points[0].y + u2t * points[1].y + ut2 * points[2].y + t3 * points[3].y;

        SetPixel(hdc, round(x), round(y), RGB(0, 0, 255)); 
    }
}

// Function to draw Hermite curve
void drawHermiteCurve(HDC hdc, POINT* points, int steps) {
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        float t2 = t * t;
        float t3 = t2 * t;
        float h1 = 2 * t3 - 3 * t2 + 1;      // (2t^3 - 3t^2 + 1)
        float h2 = -2 * t3 + 3 * t2;         // (-2t^3 + 3t^2)
        float h3 = t3 - 2 * t2 + t;          // (t^3 - 2t^2 + t)
        float h4 = t3 - t2;                   

        float t1x = (points[1].x - points[0].x);
        float t1y = (points[1].y - points[0].y);
        float t2x = (points[3].x - points[2].x);
        float t2y = (points[3].y - points[2].y);

        float x = h1 * points[0].x + h2 * points[2].x + h3 * t1x + h4 * t2x;
        float y = h1 * points[0].y + h2 * points[2].y + h3 * t1y + h4 * t2y;

        SetPixel(hdc, round(x), round(y), RGB(255, 0, 0)); 
    }
}

LRESULT CALLBACK WndP(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_LBUTTONDOWN: {
            if (count < 4) {
                points[count].x = LOWORD(lp);
                points[count].y = HIWORD(lp);
                count++;
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        case WM_RBUTTONDOWN: { 
            count = 0;
            chosenAlgorithm = -1; 
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_KEYDOWN: {
            if (count == 4) { 
                if (wp == 'B' || wp == 'b') {
                    chosenAlgorithm = 0; 
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                else if (wp == 'H' || wp == 'h') {
                    chosenAlgorithm = 1; 
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT p;
            HDC hdc = BeginPaint(hwnd, &p);

            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); 
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            for (int i = 0; i < count; i++) {
                Ellipse(hdc, points[i].x - 5, points[i].y - 5, points[i].x + 5, points[i].y + 5);
            }

            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);

            if (count == 4 && chosenAlgorithm == -1) {
                TextOut(hdc, 10, 10, "Press (B)ezier (blue),(H)ermite (red)", 35);
            }

            if (count == 4 && chosenAlgorithm != -1) {
                int steps = 1000;
                if (chosenAlgorithm == 0) {
                    drawBezierCurve(hdc, points, steps);
                }
                else if (chosenAlgorithm == 1) {
                    drawHermiteCurve(hdc, points, steps);
                }
            }

            EndPaint(hwnd, &p);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hIn, HINSTANCE hpreIn, LPSTR lpc, int ncmd) {
    const char CLASS_NAME[] = "Curve Drawer";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndP;
    wc.hInstance = hIn;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow(CLASS_NAME, "Draw Curve", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hIn, NULL);
    ShowWindow(hwnd, ncmd);
    MSG m = {0};
    while (GetMessage(&m, NULL, 0, 0)) {
        TranslateMessage(&m);
        DispatchMessage(&m);
    }
    return 0;
}