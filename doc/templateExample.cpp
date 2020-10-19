//#include <iostream>
//using namespace std;

// http://www.cplusplus.com/doc/oldtutorial/templates/

/* 
    EXAMPLE 1:
    Help to adapt more than 1 type => without repeat code for each type
    For naming template parameters: class and typename are equivalent

    int x,y;
    example <int> (x,y); // T will be convert into int during compile time
    example (x,y); // also return int
*/
template<class T>
T example(T a, T b){
    return a*b;
}



/*
    EXAMPLE 2: For different parameters (eg.2 parameter)

    int x;
    long y;
    example2 <int,long> (x,y);
    example (x,y); // return type of x (int)
*/
template<class T, class U>
T example2(T a, U b){
    return a*b; // return type T
}

/*
    EXAMPLE 3: Class templates

    example3<int> obj(10,20);
    std::cout << obj.getMax();
*/
template<class T>
class example3{
    private:
        T a,b;
    public:
        example3(T first,T seccond){
            a = first;
            b = seccond;
        }
        T getMax(); // return T;
};

template<class T>
T example3<T>::getMax(){ // The <T> specify is also class parameter
    return a>b?a:b;
}


/*
    EXAMPLE 4: Template specialization (for some type)
*/

template<class T>
class example4{
    private:
        T test;
    public:
        example4(){

        }
};

template<>
class example4 <char>{ // Overload class
    private:
        char test;
    public:
        example4(){

        }
};