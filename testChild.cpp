#include <iostream>
#include <Windows.h>
using namespace std;

#define BUFFSIZE 1048


void cls()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

int main(){

    HANDLE Readhandle;
    CHAR buffer[BUFFSIZE];
    DWORD read;
    Readhandle = GetStdHandle(STD_INPUT_HANDLE);

    while (ReadFile(Readhandle, buffer, BUFFSIZE, &read, NULL)){
        system("cls");
        printf("%s",buffer);

        if (read == 0)break;
        /*
        if (ReadFile(Readhandle, buffer, 30, &read, NULL)){
            printf("%s", buffer);
        } else {
            fprintf(stderr, "Error reading from pipe");
            break;
        }*/
    }
    //system("pause");

    return 0;
}