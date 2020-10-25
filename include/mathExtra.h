

#ifndef MATH_EXTRA_H
#define MATH_EXTRA_H

#include <array>
#include <vector>

#define pi 3.14159
#define ANG(x) x*pi/180


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


    template<typename T>
    int checkElement(std::vector<T> *para,T n){
        for (int i = 0; i < para->size(); i++){
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