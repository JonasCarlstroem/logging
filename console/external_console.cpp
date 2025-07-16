#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    AllocConsole();

    FILE *fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

#ifdef _DEBUG
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    std::cout << "Child stdin: " << hstdin << std::endl;
#endif

    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << line << std::endl;
    }

    return 0;
}