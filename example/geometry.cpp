
#include <iostream>
#include "include/consoleGraphic.h"
class Geometry:public ConsoleGraphic{
    private:
        float test = 1;
        float sensitivity = 500.00f / 1000.00f;
    public:
        Geometry(){

        };
    private:
        int onUpdate() override;
};

int Geometry::onUpdate(){
    cls();
    PIXEL_ATTR lineAttr{4,WHITE};
    float x1 = 10,x2 = 10,x3 = 40;
    float y1 = 10,y2 = 50,y3 = 40;
    float k1 = (y3 - y1)/(x3 - x1);
    float k2 = (y3 - y2)/(x3 - x2);
    // k = (y2 - y1)/(x2 - x1)
    // f(x) = (x-x1)*k=y-y1 => y = kx - (k*x1 - y1)
    
    //int i = 16;
    if (k1 >= -1 && k1 <=1 && k2 >= -1 && k2 <= 1){
        for (int i = 11; i < 40;i++){
            int giaoY1 = k1*i-(k1*x1-y1);
            int giaoY2 = k2*i-(k2*x2-y2);
            drawLine(i,giaoY2,i,giaoY1,{4,(int)test %WHITE});
        }
    } else {
        drawLine(10,20,10,30,{2,WHITE});
    }
    //drawRect(10,10,4+test,20,{4,WHITE},{4,WHITE});
    test+= 0.05;
    drawLine(x1,y1,x2,y2,lineAttr);
    drawLine(x1,y1,x3,y3,lineAttr);
    drawLine(x2,y2,x3,y3,lineAttr);
    return 0;
}