#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <string>
#include "string_calculator.h"
#include "calculator.h"

TCHAR szClassName[] = TEXT("Calculator");

WNDPROC EditWndProc;

LRESULT CALLBACK EditProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
		if (wParam == 0x0D) {
			PostMessage(GetParent(hWnd), WM_COMMAND, IDOK, 0);
			return 0;
		}
		break;
	default:
		break;
	}
	return CallWindowProc(EditWndProc, hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton;
	static HWND hInputEdit;
	static HWND hOutputEdit;
	static HFONT hFont;
	switch (msg)
	{
	case WM_CREATE:
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("計算"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hInputEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hOutputEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		SendMessage(hInputEdit, EM_LIMITTEXT, 0, 0);
		SendMessage(hOutputEdit, EM_LIMITTEXT, 0, 0);
		SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(hInputEdit, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(hOutputEdit, WM_SETFONT, (WPARAM)hFont, 0);
		SetFocus(hInputEdit);
		EditWndProc = (WNDPROC)SetWindowLongPtr(hInputEdit, GWLP_WNDPROC, (LONG_PTR)EditProc1);
		break;
	case WM_SIZE:
		MoveWindow(hButton, LOWORD(lParam) - 128, HIWORD(lParam) - 60, 128, 60, TRUE);
		MoveWindow(hInputEdit, 0, HIWORD(lParam) - 60, LOWORD(lParam) - 128, 60, TRUE);
		MoveWindow(hOutputEdit, 0, 0, LOWORD(lParam), HIWORD(lParam) - 60, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			DWORD dwTextLength = GetWindowTextLength(hInputEdit);
			LPWSTR lpszText = (LPWSTR)GlobalAlloc(0, sizeof(WCHAR) * (dwTextLength + 1));
			if (lpszText) {
				GetWindowText(hInputEdit, lpszText, dwTextLength + 1);
				std::wstring fullExpression(lpszText);
				std::wstring answer;
				dwTextLength = (DWORD)SendMessage(hOutputEdit, WM_GETTEXTLENGTH, 0, 0);
				SendMessage(hOutputEdit, EM_SETSEL, (WPARAM)dwTextLength, (LPARAM)dwTextLength);
				SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)lpszText);
				GlobalFree(lpszText);
				if (!validate(fullExpression)) {
					SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)L"(入力された式が正しくありません。)");
					SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
					return 0;
				}
				fullExpression = postfix(fullExpression);
				try {
					answer = calculate(fullExpression);
				} catch (const std::exception& error) {
					SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)L"(");
					SendMessageA(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)error.what());
					SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)L")\r\n");
					return 0;
				}
				SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)L"=");
				SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)answer.c_str());
				SendMessage(hOutputEdit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
				SetWindowText(hInputEdit, 0);
			}
			return 0;
		}
		break;
	case WM_DESTROY:
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Calculator"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
