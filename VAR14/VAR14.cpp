// Var14.cpp : Определяет точку входа для приложения.
//

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <commctrl.h>
#include <commdlg.h>
#define ID_LIST 502

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("VAR14");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Ничего себе.");

HINSTANCE hInst;
HWND hProgress;
DWORD IDC_TIMER, nCounter, IDC_PROGRESS1 = 1000;
HWND hCombo, hList;
HWND ReplDialog;
UINT uFindMsgString;
FINDREPLACE findrep;
TCHAR WhatFind[256] = TEXT("\0"), WhatReplace[256] = TEXT("\0");


// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1000, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    HWND hWnd_button = CreateWindow(TEXT("button"), TEXT("Replace"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 80, 30, hWnd, (HMENU)10000, hInstance, NULL);
    HWND hWnd_button2 = CreateWindow(TEXT("button"), TEXT("Save as"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 120, 80, 30, hWnd, (HMENU)20000, hInstance, NULL);

    hProgress = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 75, 150, 20, hWnd, (HMENU)IDC_PROGRESS1, hInst, NULL);
    SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 10));
    SendMessage(hProgress, PBM_SETSTEP, (WPARAM)1, 0);

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }


    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Не понял как, но вывел это");
    unsigned int key; char szText[] = "";
    RegisterHotKey(hWnd, 1, MOD_ALT, VK_UP);

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.
        TextOut(hdc,
            10, 50,
            greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 10000)
        {

            memset(WhatFind, 0, _countof(WhatFind));
            if (uFindMsgString == 0) {
                uFindMsgString = RegisterWindowMessage(FINDMSGSTRING);
            }
            if (IsWindow(ReplDialog) == false) {
                findrep.lStructSize = sizeof(FINDREPLACE);
                findrep.hInstance = hInst;
                findrep.hwndOwner = hWnd;
                findrep.lpstrFindWhat = WhatFind;
                findrep.lpstrReplaceWith = WhatReplace;
                findrep.wFindWhatLen = 80;
                findrep.wReplaceWithLen = _countof(WhatReplace);
                findrep.Flags = FR_FINDNEXT;
                ReplDialog = ReplaceTextW(&findrep);
            }
            break;
        }
        if (LOWORD(wParam) == 20000)
        {
            OPENFILENAME ofn;
            wchar_t buf[255] = L"\0";
            wchar_t filter[] = L"Data Files(*.csv)\0*.csv\0All Files\0*.*\0\0";
            wchar_t filterExt[][6] = { L".csv" };
            wchar_t cCustomFilter[256] = L"\0\0";
            int nFilterIndex = 0;
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hWnd;
            ofn.hInstance = hInst;
            ofn.lpstrFilter = filter;
            ofn.lpstrCustomFilter = cCustomFilter;
            ofn.nMaxCustFilter = 256;
            ofn.nFilterIndex = nFilterIndex;
            ofn.lpstrFile = buf;
            ofn.nMaxFile = 255;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.lpstrTitle = 0;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.nFileOffset = 0;
            ofn.nFileExtension = 0;
            ofn.lpstrDefExt = NULL;
            ofn.lCustData = NULL;
            ofn.lpfnHook = NULL;
            ofn.lpTemplateName = NULL;

            GetSaveFileName(&ofn);
        }
        break;
    case WM_MBUTTONDOWN:
        SetTimer(hWnd, IDC_TIMER, 200, 0);
        break;
    case WM_MBUTTONUP:
        KillTimer(hWnd, IDC_TIMER);
        break;
    case WM_MBUTTONDBLCLK:
        MessageBox(hWnd, _T("Средняя кнопка нажата 2 раза"), _T(""), 0);
        break;
    case WM_MOUSEWHEEL:
        nCounter++;
        if (nCounter == 2)
        {
            MessageBox(hWnd, _T("Вы прокрутили колесо"), _T(""), 0);
            nCounter = 0;
        }
        break;
    case WM_CHAR:
        MessageBox(hWnd, _T("Вы ЧТО-ТО нажали"), _T(""), 0);
        break;
    case WM_MOVING:
        MessageBox(hWnd, _T("АГА!"), _T(""), 0);
        break;
    case WM_KEYUP:
        MessageBox(hWnd, _T("АГА!2"), _T(""), 0);
        break;
    case WM_HOTKEY:
        MessageBox(hWnd, _T("Нажата комбинация Alt+стрелка вверх"), _T(""), 0);
        break;
    case WM_TIMER: {
        SendMessage(hProgress, PBM_STEPIT, 0, 0);
        break;
    }
    case WM_CREATE:
        //создаём список
        hList = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD,
            200, 10, 200, 100, hWnd, (HMENU)ID_LIST, hInst, NULL);
        // Отменяем режим перерисовки списка
        SendMessage(hList, WM_SETREDRAW, FALSE, 0L);
        // Добавляем в список несколько строк
        SendMessage(hList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"Первый");
        SendMessage(hList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"Второй");
        SendMessage(hList, LB_ADDSTRING, 0,
            (LPARAM)(LPSTR)L"Третий");
        // Включаем режим перерисовки списка
        SendMessage(hList, WM_SETREDRAW, TRUE, 0L);

        // Перерисовываем список
        InvalidateRect(hList, NULL, TRUE);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
