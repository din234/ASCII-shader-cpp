
#include <iostream>
#include <fstream>
#include "include/consoleGraphic.h"

typedef struct Vertex{
    float x = 0;
    float y = 0;
    float z = 0;
}vertex;

struct entity{
    float x;
    float y;
    float z;
    float vert;
};

class Geometry:public ConsoleGraphic{
    private:
        float sensitivity = 10.00f / 1000.00f;

        // FORCE CODE
        float currentYawAngle = 0,clickX = 0;
        float currentPitchAngle = 0,clickY = 0;

        float posX = 0.00f, posY = 0.00f, posZ = 0.00f;
        float yaw = 0.00f,roll = 0.00f,pitch = 0.00f;
        // camera (project with plane: z = 0)
        float cx,cy,cz;
        float scale = 1;
        std::vector<Vertex> vertices;
        std::vector<std::vector<int>> faces;
        std::vector<mathExtra::matrix<float,3,1>> normalVect;
    public:
        Geometry();
    private:
        //void checkUserInput();


        void readFile();
        void getNormal();
        mathExtra::matrix<float,3,3> getRotationMatrix();
        FCOORD projection(float dx, float dy, float dz);
        FCOORD parallelProjection(float x, float dz);
        
        int onInput(int keyCode) override;
        int onUpdate() override;
        std::string logger() override;
};





void Geometry::readFile(){
    std::ifstream file;
	std::string line;
    //file.open("teapot.txt");
    file.open("./image/teapot.txt");
    while (getline(file, line)){
        char type;
        file >> type;
        if (type == 'v'){
            float x,y,z;
            file >> x >> y >> z;
            vertices.push_back({scale*x,scale*y,scale*z});
        } else if (type == 'f') {
            int a,b,c;
            file >> a >> b >> c;
            faces.push_back({a,b,c});
        }
    }
    file.close();
}


void Geometry::getNormal(){
    for (unsigned int i = 0; i < faces.size(); i++){
        FCOORD cord[3] = {0};
        Vertex triCord[3] = {0};
        bool drawBool = true;
        for (int j = 0; j < 3; j++){
            triCord[j] = {
                vertices[ faces[i][j] - 1 ].x,
                vertices[ faces[i][j] - 1 ].y,
                vertices[ faces[i][j] - 1 ].z
            };
        }

        mathExtra::matrix<float,3,1> vector1 = {
            triCord[1].x - triCord[0].x,
			triCord[1].y - triCord[0].y,
			triCord[1].z - triCord[0].z
        };

        mathExtra::matrix<float,3,1> vector2 = {
            triCord[2].x - triCord[0].x,
			triCord[2].y - triCord[0].y,
			triCord[2].z - triCord[0].z
        };
        //mathExtra::matrix<float, 1,3> dot = {triCord[0].x, triCord[0].y, triCord[0].z};

        auto normal = mathExtra::crossProduct(vector1,vector2);
        normalVect.push_back({normal[0][0],normal[1][0],normal[2][0]});
        //auto temp1 = mathExtra::dotProduct(dot,normal);
    }
}




Geometry::Geometry(){
    //setMinTimePerFrame(17);
    createBuffering(5,130,130);
    cx= (float) getBuffWidth()/2;
    cy= (float) getBuffHeight()/2;
    cz = -cx/tan(ANG(90/2));// tan(FOV/2) = cx/cz

    readFile();
    getNormal();
}


FCOORD Geometry::parallelProjection(float dx, float dz){
    float tempX = dx + cx;
    float tempY = dz + cz;
    return {tempX,tempY};
}


FCOORD Geometry::projection(float dx, float dy, float dz){
    return {cx - dx*(cz/dz),cy + dy*(cz/dz)}; // -cx left handed 
}

mathExtra::matrix<float,3,3> Geometry::getRotationMatrix(){

	float temp = (float) cosf((float) ANG((float)roll));
    mathExtra::matrix<float,3,3> maRoll = {
        cosf(ANG(roll)), -sinf(ANG(roll)), 0.00f,
        sinf(ANG(roll)), cosf(ANG(roll)), 0.00f,
        0.00f, 0.00f, 1.00f
    };

    mathExtra::matrix<float,3,3> maPitch = {
        1, 0, 0,
        0, cosf(ANG(pitch)),-sinf(ANG(pitch)),
        0, sinf(ANG(pitch)), cosf(ANG(pitch))
    };



    mathExtra::matrix<float,3,3> maYaw = {
        cosf(ANG(yaw)), 0, sinf(ANG(yaw)),
        0, 1, 0,
        -sinf(ANG(yaw)), 0, cosf(ANG(yaw))
    };

    return mathExtra::dotProduct(maRoll,mathExtra::dotProduct(maPitch,maYaw));
}

std::string Geometry::logger(){
    return "Time Elappsed: " + std::to_string(elappsedTime) + 
    "\n OffsetX: " + std::to_string(posX) +
    "\n OffsetY: " + std::to_string(posY) + 
    "\n OffsetZ: " + std::to_string(posZ) + 
    "\n Yaw: " + std::to_string(yaw) +
    "\n Pitch: " + std::to_string(pitch) +
    "\n Roll: " + std::to_string(roll);
}

int Geometry::onInput(int keyCode){
    switch (keyCode){
        case VK_ESCAPE: runTest = FALSE;break;
            
        case 'A': 
            posX-= (elappsedTime*sensitivity)*cosf(ANG(yaw));
            posZ-= (elappsedTime*sensitivity)*sinf(ANG(yaw));
            break;

        case 'D': 
            posX+= (elappsedTime*sensitivity)*cosf(ANG(yaw));
            posZ+= (elappsedTime*sensitivity)*sinf(ANG(yaw));
            break;

        case 'W': 
            posX+= (elappsedTime*sensitivity)*cosf(ANG((yaw + 90)));
            posZ+= (elappsedTime*sensitivity)*sinf(ANG((yaw + 90)));
            break;

        case 'S': 
            posX-= (elappsedTime*sensitivity)*cosf(ANG((yaw + 90)));
            posZ-= (elappsedTime*sensitivity)*sinf(ANG((yaw + 90)));
            break;

        case VK_SPACE: posY+= elappsedTime*sensitivity;break;
        case VK_SHIFT: posY-= elappsedTime*sensitivity;break;

        //case 'Q': roll = (roll > -360)? roll-=1: roll+360;break;
        //case 'E': roll = (roll < 360)? roll+=1: roll-360;break;
    }
    return 0;
}

int Geometry::onUpdate(){
    cls();
    drawRect((float) getBuffWidth()/2,(float) getBuffHeight()/2,3,3,{4,WHITE});
    PIXEL_ATTR lineAttr{4,WHITE};


    FCOORD ro = { (float) mouse.dwMousePosition.X, (float) mouse.dwMousePosition.Y};
    if (mouse.dwButtonState == 1){
        float vectorX = ro.x - clickX;
        float vectorY = ro.y - clickY;
        
        float setYawAngle = (vectorX >= 0)? 90+ atan(cz/vectorX)*180/pi : 90+ 180 + atan(cz/vectorX)*180/pi;
        float setPitchAngle = (vectorY >= 0)? 90+ atan(cz/vectorY)*180/pi : 90+ 180 + atan(cz/vectorY)*180/pi;
        yaw = currentYawAngle + setYawAngle;
        pitch = currentPitchAngle+ setPitchAngle;
    } else {
        clickX = ro.x;
        clickY = ro.y;
        currentYawAngle = yaw;
        currentPitchAngle = pitch;
    }
    auto rotate = getRotationMatrix();
    
    for (unsigned int i = 0; i < normalVect.size(); i++){
        FCOORD cord[3] = {0};
        bool drawBool = true;
        auto flatShade = mathExtra::dotProduct(rotate,normalVect[i]);
        for (short j = 0; j < 3; j++){
            mathExtra::matrix<float,3,1> maCord = {
                vertices[ faces[i][j] - 1 ].x - posX,
                vertices[ faces[i][j] - 1 ].y - posY,
                vertices[ faces[i][j] - 1 ].z - posZ
            };
            auto newCord = mathExtra::dotProduct(rotate,maCord);

            if (j == 0){
                auto backFace = mathExtra::dotProduct(mathExtra::reverse(newCord),flatShade);
                if (backFace[0][0] < 0){drawBool = false;break;}
            }
            if (newCord[2][0] < 0){drawBool = false;break;}
            
            cord[j] = projection(newCord[0][0],newCord[1][0],newCord[2][0]);
        }

        if (drawBool){
            //std::cout << "DSAD" << std::endl;
            int test = (int) (5*flatShade[2][0]/sqrtf(powf(flatShade[0][0],2) + powf(flatShade[1][0],2) + powf(flatShade[2][0],2)));
            
            /*int test = 5*normalVect[i][2][0]/sqrtf(
                powf(normalVect[i][0][0],2) + 
                powf(normalVect[i][1][0],2) + 
                powf(normalVect[i][2][0],2));*/
            drawTri(cord,{test,BLUE},{test,WHITE});
        }
    }
    return 0;
}

/*
    (a.c) . (b.c) = c^2.(a.b)
    (a1*c1 + a2*c2) . (b1*c1 + b2*c2)
*/