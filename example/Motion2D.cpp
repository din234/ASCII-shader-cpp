#include "include/consoleGraphic.h"

class Motion:public ConsoleGraphic{
    private:
        const float height = 10,width = 5;

        float sensitivity = 20.00f / 1000.00f;
        float midX = 0,midY = 0,rotate;

        mathExtra::matrix<float,3,2> cord { // midX = PosX + width/2; midY = posY + height/3
            -width/2 , -height/3,
            0,2*height/3,
            width/2, -height/3
        };

        inline FCOORD l(float x, float y) {
            return {x + getBuffWidth()/2,getBuffHeight()/2 - y};
        }
    public:
        Motion(){};
    private:
        int onInput(int keyCode) override;
        int onCreate() override;
        int onUpdate() override;
        std::string logger();
};




std::string Motion::logger(){
    return "Time Elappsed: " + std::to_string(elappsedTime) + 
    "\n OffsetX: " + std::to_string(midX) +
    "\n OffsetY: " + std::to_string(midY) + 
    "\n Rotation: " + std::to_string(rotate);
}

int Motion::onCreate(){
    return 0;
}

int Motion::onInput(int keyCode){
    switch (keyCode){
        case VK_ESCAPE: runTest = FALSE;break;
        case VK_LEFT:
            midX-= (elappsedTime*sensitivity)*sinf(ANG((rotate+90)));
            midY-= (elappsedTime*sensitivity)*cosf(ANG((rotate+90)));
            break;

        case VK_UP: 
            midX+= (elappsedTime*sensitivity)*sinf(ANG(rotate));
            midY+= (elappsedTime*sensitivity)*cosf(ANG(rotate));break;
            
        case 'Q': rotate = (rotate > 0)? rotate-=2: rotate+360;break;
        case 'E': rotate = (rotate < 360)? rotate+=2: rotate-360;break;
    }
    return 0;
}

int Motion::onUpdate(){
    cls();
    PIXEL_ATTR lineAttr{4,WHITE};
    FCOORD tri[3];


    mathExtra::matrix<float,2,2> rota {
        cosf(ANG(rotate)),-sinf(ANG(rotate)),
        sinf(ANG(rotate)),cosf(ANG(rotate))
    };

    auto temp = mathExtra::dotProduct(cord,rota);

    for (int i = 0; i < 3; i++){
        tri[i] = { l(midX + temp[i][0], midY + temp[i][1]) };
    }



    //drawRect(posX,posY,10,10,lineAttr);
    //drawLine(0,0, 10,10,lineAttr);
    drawTri(tri,lineAttr,lineAttr);
    return 0;
}