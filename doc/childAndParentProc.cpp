// https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output

HANDLE childOutRead = NULL;
HANDLE childOutWrite = NULL;
HANDLE childInRead = NULL;
HANDLE childInWrite = NULL;

HANDLE inputFileH = NULL;


void writeToPipe(){
    DWORD dwRead,dwWrite;
    CHAR chBuf[4096];
    while (true){
        printf("oooo");
        if (!ReadFile(inputFileH,chBuf,4096,&dwRead,NULL) | dwRead == 0){break;}
        if (!WriteFile(childInWrite,chBuf,dwRead,&dwWrite,NULL)){break;}
    }

     if ( ! CloseHandle(childInWrite) ) {
        printf("fdsljafalksdjflajeifbuvcoab");
     }
}

void readToPipe(){
    DWORD dwRead,dwWrite;
    CHAR chBuf[4096];
    HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    while (true){
        if (!ReadFile(childOutRead,chBuf,4096,&dwRead,NULL) | dwRead == 0){break;}
        if (!WriteFile(hParentStdOut,chBuf,dwRead,&dwWrite,NULL)){break;}
    }
}

void createChildProcess(){
    char fileName[] = "child.exe";
    STARTUPINFOA childConsoleAtr{0}; // set zero memory
    PROCESS_INFORMATION pInfo{0}; // set zero memory
    childConsoleAtr.cb = sizeof(childConsoleAtr);
    childConsoleAtr.hStdError = childOutWrite;
    childConsoleAtr.hStdOutput = childOutWrite;
    childConsoleAtr.hStdInput = childInRead;
    childConsoleAtr.dwFlags |= STARTF_USESTDHANDLES;

    if (CreateProcessA(NULL, // no module name (command line)
                fileName, //lpCommandLine MAX_PATH character
                NULL, // 
                NULL,
                TRUE, // handles inherited from parent
                CREATE_NEW_CONSOLE,
                NULL, // parent environment
                NULL, // parent directory
                &childConsoleAtr,
                &pInfo
    )){
        std::cout << "success " << std::endl;
        //WaitForSingleObject(pInfo.hProcess,INFINITE);
        CloseHandle(pInfo.hThread);
        CloseHandle(pInfo.hProcess);

        CloseHandle(childOutWrite);
        CloseHandle(childInRead);
    } else {
        printf("%d\n",GetLastError());
    }
};

int main(int argc ,char *argv[])
{
    SECURITY_ATTRIBUTES seAttr;
    printf("Parent run: \n");

    seAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    seAttr.bInheritHandle = TRUE; // pipe handle are inherited
    seAttr.lpSecurityDescriptor = NULL;

    // Create pipe for child process stout
    if (!CreatePipe(&childOutRead,&childOutWrite,&seAttr,0)){
        printf("Failded\n");
        system("pause");
        return 0;
    };

    if (!SetHandleInformation(childOutRead,HANDLE_FLAG_INHERIT,0)){
         printf("Failded\n");
        system("pause");
        return 0;
    };

    if (!CreatePipe(&childInRead,&childInWrite,&seAttr,0)){
        printf("Failded\n");
        return 0;
    }

    createChildProcess();

    char fileName[] = "child.exe";
    inputFileH = CreateFileA(
        fileName, GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY,NULL
    );


    if (inputFileH == INVALID_HANDLE_VALUE){
        printf("INVALID HANDLE");
        system("pause");
        return 0;
    };

    //writeToPipe();
    /*

    //printf("%d\n",argc);
    printf("fdsfjksdahfhdsajkfhkjsdhf\n");
    //printf("YES\n");
    */
    readToPipe();
    std::cin.ignore();
    return 0;
}














// CHILD

#include <Windows.h>
#include <iostream>
using namespace std;

int main(){
    CHAR chBuff[4096];
    DWORD dwRead,dwWrite;
    HANDLE hIn,hOut;
    
    hIn = GetStdHandle(STD_OUTPUT_HANDLE);
    hOut = GetStdHandle(STD_INPUT_HANDLE);

    if ((hIn == INVALID_HANDLE_VALUE) || (hOut == INVALID_HANDLE_VALUE)){
        system("pause");
        return 0;
    }

    printf("Message from child process!!!");

    while (true){
        if (!ReadFile(hIn,chBuff,4096,&dwRead,NULL) || dwRead == 0){break;}
        if (!WriteFile(hOut,chBuff,dwRead,&dwWrite,NULL)){break;}
    }

    printf("childe from");

    //cout << "Hello" << endl;
    //system("pause");
    return 0;
}