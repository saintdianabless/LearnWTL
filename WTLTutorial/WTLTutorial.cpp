#include "atlbase.h"
#include "atlapp.h"

#include "framework.h"
#include "WTLTutorial.h"

CAppModule _Module;

class MyWindow : public CWindowImpl<MyWindow> {
public:
    DECLARE_WND_CLASS(_className);
    BEGIN_MSG_MAP(MyWindow)
        MESSAGE_HANDLER(WM_DESTROY, OnDestory)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
    END_MSG_MAP()
private:
    LRESULT OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled) {
        OutputDebugString(L"Exiting...\n");
        PostQuitMessage(0);
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled)
    {
        if (MessageBox(L"ȷ���˳�?", L"WTL", MB_OKCANCEL) == IDOK) {
            DestroyWindow();
        }
        return 0;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled) {
        ++this->_nResize;
        wchar_t buf[1024];
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        swprintf_s(buf, 1024, L"��%d��resize����: (%d, %d)\n", this->_nResize, width, height);
        OutputDebugString(buf);
        return 0;
    }

    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled) {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(&ps);  // ��ȡҪ���»����������Ϣ
        auto rc = ps.rcPaint;
        wchar_t buf[1024];
        swprintf_s(buf, 1024,
            L"��������: (%d, %d, %d, %d)\n", rc.left, rc.top, rc.right, rc.bottom);
        OutputDebugString(buf);

        // ALL PAINTING STUFF
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(&ps);  // ���Update Region, ���ٷ�WM_PAINT��Ϣ
        return 0;
    }

    int _nResize = 0;
    static const wchar_t* const _className;
};

const wchar_t* const MyWindow::_className = L"Sample Window Class";

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    CMessageLoop loop;
    _Module.Init(NULL, hInstance);
    _Module.AddMessageLoop(&loop);

    MyWindow mw;
    mw.Create(NULL, CWindow::rcDefault, L"WTL����", WS_OVERLAPPEDWINDOW);
    mw.ShowWindow(nCmdShow);
    mw.UpdateWindow();

    loop.Run();

    _Module.RemoveMessageLoop();
    _Module.Term();

    return 0;
}

