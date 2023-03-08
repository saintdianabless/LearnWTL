#include "atlbase.h"
#include "atlapp.h"

#include "framework.h"
#include "WTLTutorial.h"

void OnSize(HWND, LPARAM lParam) {
    wchar_t buf[1024];
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    swprintf_s(buf, 1024, L"resize窗口: (%d, %d)\n", width, height);
    OutputDebugString(buf);
}

void OnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    auto hdc = BeginPaint(hwnd, &ps);  // 获取要更新绘制区域的信息
    auto rc = ps.rcPaint;
    wchar_t buf[1024];
    swprintf_s(buf, 1024, 
        L"绘制区域: (%d, %d, %d, %d)\n", rc.left, rc.top, rc.right, rc.bottom);
    OutputDebugString(buf);

    // ALL PAINTING STUFF
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    EndPaint(hwnd, &ps);  // 清空Update Region, 不再发WM_PAINT消息
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_SIZE:
    {
        OnSize(hwnd, lParam);
        return 0;
    }
    case WM_PAINT:
    {
        OnPaint(hwnd);
        return 0;
    }
    case WM_CLOSE:
        if (MessageBox(hwnd, L"确认退出?", L"Windows程序", MB_OKCANCEL) == IDOK) {
            DestroyWindow(hwnd);
        }
        return 0;  // 忽略该消息
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    auto hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Window编程",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,  // owner
        NULL,
        hInstance,
        NULL);
    
    if (hwnd == NULL) {
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);

    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

