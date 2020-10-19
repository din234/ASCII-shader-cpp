#include <thread>


/*
    Convenience way defining anonymous function object
*/



void lambdaExample(){
    std::thread th([=]()
    {

    });
}