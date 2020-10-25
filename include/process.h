
#ifndef PROCESS_H
#define PROCESS_H

#include <Windows.h>
#define BUFFSIZE 1048

class processTest {
    private:
        HANDLE readPipe = NULL;
        HANDLE writePipe = NULL;
        HANDLE fileHandle;

        STARTUPINFOA childConsoleAtr{0}; // set zero memory
        PROCESS_INFORMATION pInfo{0};
        SECURITY_ATTRIBUTES seAttr;

        DWORD write;
        //char temp[]; // may cause error to heap (unassigned memory address)
    public:
        char *temp = NULL;
        processTest();
        ~processTest(){
            CloseHandle(writePipe);
            CloseHandle(readPipe);
            delete[] temp;
        }

        void run(char *cum);
    private:
        void thisFunctionCauseError(); // for testing
};



void processTest::thisFunctionCauseError(){
    //temp[0] = 'k';
    //std::cin.ignore();
    //testString = "fdsaflksdjflkjdsf";
    //std::cin >> temp;
    //std::cout << "dadadada" << std::endl;
}

void processTest::run(char *cum){
    /*
    if(!WriteFile(writePipe,cum,BUFFSIZE,&write,NULL)){
        printf("failed to write to file!!!");
    }*/
    WriteFile(writePipe,cum,BUFFSIZE,&write,NULL);
    //std::this_thread::sleep_for(std::chrono::seconds(1));
}


processTest::processTest(){
    //temp = new char[BUFFSIZE];

    seAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    seAttr.bInheritHandle = TRUE; // allow handle inheritance by child process
    seAttr.lpSecurityDescriptor = NULL; // ???

    if (!CreatePipe(&readPipe,&writePipe,&seAttr,NULL)){ 
        printf("Error, failed to create pipe!!!");
    }; 
    childConsoleAtr.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    childConsoleAtr.hStdInput = readPipe;
    childConsoleAtr.dwFlags = STARTF_USESTDHANDLES;

    SetHandleInformation(writePipe, HANDLE_FLAG_INHERIT, 0);

    char childproc[] = "testChild.exe";
    if (CreateProcessA(NULL,
        childproc,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &childConsoleAtr,&pInfo
    )){
        CloseHandle(pInfo.hThread);
        CloseHandle(pInfo.hProcess);
        //printf("%d\n",GetLastError());
    };
}
#endif // PROCESS_H