

#ifndef MATH_EXTRA_H
#define MATH_EXTRA_H

#include <array>
#include <vector>

#define pi 3.14159f
#define ANG(x) (float) x*pi/180


namespace mathExtra{

    template<typename T,size_t h,size_t w>
    using matrix = std::array<std::array<T,w>,h>;

    template<typename T,size_t h,size_t w,size_t s>
    matrix<T,h,w> dotProduct(matrix<T,h,s> a, matrix<T,s,w> b){ // tich vo huong
        matrix<T,h,w> c;
        for (int i = 0; i < h*w; i++){
            T temp = 0x00;
            int ih = i/w, iw = i - w*(i/w);
            for (int j = 0; j < s;j++){
                temp += a[ih][j]*b[j][iw];
            }
            c[ih][iw] = temp;
        }
        return c;
    }

    template<typename T, size_t s>
    matrix<T,s,1> crossProduct(matrix<T,s,1> a, matrix<T,s,1> b){ // tich vecto
        matrix<T,s,1> c;
        for (int i = 0; i < s;i++){
            c[i][0] = a[(i+1)%s][0] * b[(i+2)%s][0] - b[(i+1)%s][0] * a[(i+2)%s][0];
        }
        return c;
    }

    template<typename T,size_t h,size_t w>
    matrix<T,w,h> reverse(matrix<T,h,w> a){
        matrix<T,w,h> c;
        for (int i = 0; i < h*w; i++){
            int ih = i/w, iw = i - w*(i/w);
            c[iw][ih] = a[ih][iw];
        }
        return c;
    }

    /*

    template<typename T, size_t s>
    matrix<T,1,s> crossProduct(matrix<T,1,s> a, matrix<T,1,s> b){ // tich vecto
        matrix<T,1,s> c;
        for (int i = 0; i < s;i++){
            c[i] = a[0][(i+1)%s] * b[0][(i+2)%s] - b[0][(i+1)%s] * a[0][(i+2)%s];
        }
        return c;
    }*/






    template<typename T,int w>
    using pointer_of_array = T[w]; // typedef

    template<typename T,int s,int t>
    pointer_of_array<T,t>* test(T (&arr) [s][t]){
        return arr;
    }

    template<typename T,int h,int w,int s>
    pointer_of_array<T,w>* dotProduct2DArr(T (&a) [h][s],T (&b) [s][w], T (&c) [h][w]){
        for (int i = 0; i < h*w; i++){
            T temp = 0x00;
            int ih = i/w, iw = i - w*(i/w);
            for (int j = 0; j < s;j++){
                temp += a[ih][j]*b[j][iw];
            }
            c[ih][iw] = temp;
        }
        return c;
    }
    


    template<typename T>
    int checkElement(std::vector<T> *para,T n){
        for (unsigned int i = 0; i < para->size(); i++){
            if (para->at(i) == n){return 0;}
        }
        return 1;
    }

    template<typename T>
    int deleteElement(std::vector<T> *para, T n){
        std::vector<T> temp;
        for (size_t c = 0; c < para->size();c++){
            if (para->at(c) != n){temp.push_back(para->at(c));}
        }

        if (temp.size() < para->size()){
            para->resize(temp.size());
            for (size_t i = 0; i < temp.size(); i++){
                para->at(i) = temp.at(i);
            }
            return 1;
        }
        return 0;
    }
}

// cross product tich vecto (tich co huong)

#endif // MATH_EXTRA_H