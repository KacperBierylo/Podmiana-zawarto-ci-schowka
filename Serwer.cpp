#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<CommCtrl.h>
#pragma comment(lib, "WSOCK32.lib")
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND hButton;
HWND hTextBox;
char buf[27] = "22222222222222222222222222";
LPWSTR ptr;
wchar_t wbuf[27];
SOCKET ss;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	WSADATA wsas;
	int result;
	WORD wersja;
	wersja = MAKEWORD(1, 1);
	result = WSAStartup(wersja, &wsas);
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sa;
	memset((void*)(&sa), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(12345);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(s, (struct sockaddr FAR*) & sa, sizeof(sa));
	result = listen(s, 5);

	struct sockaddr_in sc;
	int lenc;

	lenc = sizeof(sc);
	ss = accept(s, (struct sockaddr FAR*) & sc, &lenc);
	
	const wchar_t CLASS_NAME[] = L"SWindow";
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"OknoS1", WS_OVERLAPPEDWINDOW, 
		 800, 400, 250, 200, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	MSG message;
	send(ss, buf, 27, NULL);
	while (GetMessage(&message, (HWND)NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		hButton = CreateWindowExW(0, TEXT("BUTTON"), TEXT("Podmień!"), WS_VISIBLE | WS_CHILD,
			20, 80, 200, 20, hwnd, (HMENU)1, NULL, NULL);

		mbstowcs(wbuf, buf, strlen(buf) + 1);//Plus null
		ptr = wbuf;

		hTextBox = CreateWindowExW(0, TEXT("EDIT"), ptr, WS_VISIBLE | WS_CHILD,
			0, 0, 210, 20, hwnd, NULL, NULL, NULL);
		return (0);

	case WM_DESTROY:
		PostQuitMessage(0);
		return (0);

	case WM_COMMAND:
		switch (LOWORD(wParam))	{
		case 1:
			LPWSTR pMem;
			int cTextLen = GetWindowTextLength(hTextBox);
			pMem = (LPWSTR)VirtualAlloc((LPVOID)NULL, (DWORD)(cTextLen + 1), MEM_COMMIT, PAGE_READWRITE);
			GetWindowText(hTextBox, pMem, cTextLen + 1);
			wcstombs(buf, pMem, 27);
			send(ss, buf, 27, NULL);
		}

		return (0);
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}