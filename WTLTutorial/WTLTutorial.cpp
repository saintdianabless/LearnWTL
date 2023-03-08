#include "atlbase.h"
#include "atlapp.h"

#include "framework.h"
#include "WTLTutorial.h"

template<typename Derived>
class BaseWindow {
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_CREATE) {
            BaseWindow* self = nullptr;
            auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            self = reinterpret_cast<BaseWindow*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
            self->_hwnd = hwnd;
            return 0;
        }

        Derived* derived = reinterpret_cast<Derived*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (derived) {
            return derived->HandleMessage(uMsg, wParam, lParam);
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0, HMENU hMenu = 0
    ){
        Derived* underlying = static_cast<Derived*>(this);

        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = BaseWindow<Derived>::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = underlying->ClassName();
        RegisterClass(&wc);

        _hwnd = CreateWindowEx(
            dwExStyle, underlying->ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

        return (_hwnd ? TRUE : FALSE);
    }

    HWND Window() const {
        return _hwnd;
    }
protected:
    HWND _hwnd;
};

class MainWindow: public BaseWindow<MainWindow> {
public:
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg)
        {
        case WM_SIZE:
        {
            OnSize(lParam);
            return 0;
        }
        case WM_PAINT:
        {
            OnPaint();
            return 0;
        }
        case WM_CLOSE:
            if (MessageBox(_hwnd, L"ȷ���˳�?", L"Windows����", MB_OKCANCEL) == IDOK) {
                DestroyWindow(_hwnd);
            }
            return 0;  // ���Ը���Ϣ
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            break;
        }
        return DefWindowProc(_hwnd, uMsg, wParam, lParam);
    }

    const wchar_t* ClassName() const {
        return _className;
    }
private:
    void OnSize(LPARAM lParam) {
        ++this->_nResize;
        wchar_t buf[1024];
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        swprintf_s(buf, 1024, L"��%d��resize����: (%d, %d)\n", this->_nResize, width, height);
        OutputDebugString(buf);
    }

    void OnPaint() {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(_hwnd, &ps);  // ��ȡҪ���»����������Ϣ
        auto rc = ps.rcPaint;
        wchar_t buf[1024];
        swprintf_s(buf, 1024,
            L"��������: (%d, %d, %d, %d)\n", rc.left, rc.top, rc.right, rc.bottom);
        OutputDebugString(buf);

        // ALL PAINTING STUFF
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(_hwnd, &ps);  // ���Update Region, ���ٷ�WM_PAINT��Ϣ
    }

    int _nResize;
    const wchar_t* const _className = L"Sample Window Class";
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MainWindow *mw = new MainWindow();

    if (!mw->Create(L"Window���", WS_OVERLAPPEDWINDOW)) {
        return 0;
    }
    ShowWindow(mw->Window(), nCmdShow);

    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

