#include "include/consoleGraphic.h"

class Motion:public ConsoleGraphic{
    private:
        const float height = 10,width = 5;
        float midX = 0,midY = 0,rotate = 0;

        mathExtra::matrix<float,2,3> cord { // midY = PosY + width/2; midX = posX + height/3
            -height/3,-height/3, 2*height/3,
            -width/2, width/2, 0
        };

        float sensitivity = 20.00f / 1000.00f;

        inline FCOORD l(float x, float y) {
            return {(getBuffWidth()/2) + x,(getBuffHeight()/2) - y};
        }
    public:
        Motion(){
            setMinTimePerFrame(17);
            createBuffering(5,130,130);
        };
    private:
        int onInput(int keyCode) override;
        int onUpdate() override;
        std::string logger();
};



std::string Motion::logger(){
    FCOORD temp = l((float) mouse.dwMousePosition.X - getBuffWidth(),(float) mouse.dwMousePosition.Y);
    return "Time Elappsed: " + std::to_string(elappsedTime) + 
    "\n MOUSE X: "  + std::to_string(temp.x) + 
    " Y: " + std::to_string(temp.y) +
    "\nA: "+ std::to_string(sqrtf(powf(temp.x,2) + powf(temp.y,2))) + 
    "\n OffsetX: " + std::to_string(midX) +
    "\n OffsetY: " + std::to_string(midY) + 
    "\n Rotation: " + std::to_string(rotate);
}

int Motion::onInput(int keyCode){
    switch (keyCode){
        case VK_ESCAPE: runTest = FALSE;break;
        case VK_LEFT:
            midX+= (elappsedTime*sensitivity)*cosf(ANG((rotate + 90)));
            midY+= (elappsedTime*sensitivity)*sinf(ANG((rotate + 90)));break;
        case VK_UP:
            midX+= (elappsedTime*sensitivity)*cosf(ANG((rotate)));
            midY+= (elappsedTime*sensitivity)*sinf(ANG((rotate)));
            break;
    }
    return 0;
}

int Motion::onUpdate(){
    cls();
    PIXEL_ATTR lineAttr{4, WHITE};
    FCOORD tri[3];
    FCOORD ro = l((float) mouse.dwMousePosition.X - getBuffWidth(),(float) mouse.dwMousePosition.Y);
    float vectorX = ro.x - midX;
    float vectorY =ro.y - midY;
    drawRect(mouse.dwMousePosition.X,mouse.dwMousePosition.Y,10,10,{4,WHITE},{4,RED});

    rotate = (vectorX >= 0)? atan(vectorY/vectorX)*180/pi : 180 + atan(vectorY/vectorX)*180/pi;

    mathExtra::matrix<float,2,2> rota {
        cosf(ANG(rotate)),-sinf(ANG(rotate)),
        sinf(ANG(rotate)),cosf(ANG(rotate))
    };
    auto temp = mathExtra::dotProduct(rota,cord);

    for (int i = 0; i < 3; i++){
        tri[i] = { l(midX + temp[0][i], midY + temp[1][i]) };
    }





    //drawRect(posX,posY,10,10,lineAttr);
    drawLine(midX,midY, mouse.dwMousePosition.X,mouse.dwMousePosition.Y,{4,BLUE});
    drawTri(tri,lineAttr,lineAttr);
    return 0;
}