#include<Windows.h>
#include <cmath>
#pragma comment(lib, "WSOCK32.lib")
void charToInts(char* msg, long long* n1, long long* n2);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

    SOCKET s;
    struct sockaddr_in sa;
    WSADATA wsas;
    WORD wersja;
    wersja = MAKEWORD(2, 0);
    WSAStartup(wersja, &wsas);
    s = socket(AF_INET, SOCK_STREAM, 0);
    memset((void*)(&sa), 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(12345);
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    int result;
    result = connect(s, (struct sockaddr FAR*) & sa, sizeof(sa));
    if (result == SOCKET_ERROR) {

        return 0;
    }
    char buf[27];
    HWND tst = FindWindowExA(NULL, NULL, NULL, "OknoK1");
    long long n1, n2;
    while (1) {
        if (recv(s, buf, 27, NULL) == SOCKET_ERROR) {
            closesocket(s);
            WSACleanup();
            break;
        }
        charToInts(buf, &n1, &n2);
        PostMessageW(tst, 2345, n1, n2);
        Sleep(1000);
    }
    closesocket(s);
    WSACleanup();
}
void charToInts(char* msg, long long* n1, long long* n2) {

    *n1 = 0;
    *n2 = 0;
    for (int i = 12; i >= 0; i--) {
        *n1 += (msg[i] - '0')*std::pow(10, 12 - i);
    }

    for (int i = 25; i > 12; i--) {
        *n2 += (msg[i] - '0') * pow(10, 25 - i);
    }
}