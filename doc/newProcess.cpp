#include <iostream>
using namespace std;


//#define _WIN32_WINNT 0x500 // window 2000 to later (= _WIN32_WINNT_WIN2K)

#include <windows.h>

int main(int argc ,char *argv[])
{
    STARTUPINFOA childConsoleAtr{0}; // set zero memory

    PROCESS_INFORMATION pInfo{0}; // set zero memory
    childConsoleAtr.cb = sizeof(childConsoleAtr);
    if (CreateProcessA(NULL, // no module name (command line)
                argv[0], //lpCommandLine MAX_PATH character
                NULL, // 
                NULL,
                FALSE, // handles inherited from parent
                0,//NORMAL_PRIORITY_CLASS |CREATE_NEW_CONSOLE,
                NULL, // parent environment
                NULL, // parent directory
                &childConsoleAtr,
                &pInfo
    )){
        cout << "success " << endl;
        WaitForSingleObject(pInfo.hProcess,INFINITE);
        CloseHandle(pInfo.hThread);
        CloseHandle(pInfo.hProcess);
        //printf("%d\n",GetLastError());
        //cin.ignore();
        //return 0;
    } else {
        printf("%d\n",GetLastError());
    }

    cin.ignore();
    return 0;
}