

#include <fstream>
#include "include/consoleGraphic.h"

typedef struct TRID{
    float x = 0;
    float y = 0;
    float z = 0;
}trid;

class Geometry:public ConsoleGraphic{
    private:
        float test = 1;
        float sensitivity = 10.00f / 1000.00f;


        // FORCE CODE
        float posX = 0, posY = 0, posZ = 0;

        // camera (project with plane: z = 0)
        float cx= getBuffWidth()/2 , cy=getBuffHeight()/2;
        float cz = -cx/tan(ANG(90/2));// tan(FOV/2) = cx/cz

        float yaw = 0,roll = 0,pitch = 0;

        float scale = 10;
        TRID cube[20] = { // x,y,z
            {-0.57735, -0.57735, 0.57735},
            {0.934172, 0.356822, 0},
            {0.934172, -0.356822, 0},
            {-0.934172, 0.356822, 0},
            {-0.934172, -0.356822, 0},
            {0, 0.934172,  0.356822},
            {0, 0.934172, -0.356822},
            {0.356822, 0, -0.934172},
            {-0.356822, 0, -0.934172},
            {0, -0.934172, -0.356822},
            {0, -0.934172, 0.356822},
            {0.356822, 0, 0.934172},
            {-0.356822, 0, 0.934172},
            {0.57735, 0.57735, -0.57735},
            {0.57735, 0.57735, 0.57735},
            {-0.57735, 0.57735, -0.57735},
            {-0.57735,  0.57735, 0.57735},
            {0.57735, -0.57735, -0.57735},
            {0.57735, -0.57735, 0.57735},
            {-0.57735, -0.57735, -0.57735}
        };

        int f[36][3] = {
            {19, 3, 2},
            {12  ,19  ,2},
            {15  ,12  ,2},
            {8  ,14  ,2},
            {18  ,8  ,2},
            {3  ,18  ,2},
            {20  ,5  ,4},
            {9  ,20  ,4},
            {16  ,9  ,4},
            {13  ,17  ,4},
            {1  ,13  ,4},
            {5  ,1  ,4},
            {7  ,16  ,4},
            {6  ,7  ,4},
            {17  ,6  ,4},
            {6  ,15  ,2},
            {7  ,6  ,2},
            {14  ,7  ,2},
            {10  ,18  ,3},
            {11  ,10  ,3},
            {19  ,11  ,3},
            {11  ,1  ,5},
            {10  ,11  ,5},
            {20  ,10  ,5},
            {20  ,9  ,8},
            {10  ,20  ,8},
            {18  ,10  ,8},
            {9  ,16  ,7},
            {8  ,9  ,7},
            {14  ,8  ,7},
            {12  ,15  ,6},
            {13  ,12  ,6},
            {17  ,13  ,6},
            {13  ,1  ,11},
            {12  ,13  ,11},
            {19  ,12  ,11}
        };

    public:
        Geometry(){};
    private:
        //void checkUserInput();

        mathExtra::matrix<float,3,3> getRotationMatrix();
        FCOORD projection(float dx, float dy, float dz);
        FCOORD parallelProjection(float x, float dz);
        
        int onInput(int keyCode) override;
        int onCreate() override;
        int onUpdate() override;
        std::string logger() override;
};



FCOORD Geometry::parallelProjection(float dx, float dz){
    float tempX = dx + cx;
    float tempY = dz + cz;
    return {tempX,tempY};
}


FCOORD Geometry::projection(float dx, float dy, float dz){
    return {cx - dx*(cz/dz),cy + dy*(cz/dz)}; // -cx left handed 
}

mathExtra::matrix<float,3,3> Geometry::getRotationMatrix(){
    mathExtra::matrix<float,3,3> maYaw = {
        1, 0, 0,
        0, cosf(ANG(pitch)),sinf(ANG(pitch)),
        0, -sinf(ANG(pitch)), cosf(ANG(pitch))
    };

    mathExtra::matrix<float,3,3> maPitch = {
        cosf(ANG(yaw)), 0, -sinf(ANG(yaw)),
        0, 1, 0,
        sinf(ANG(yaw)), 0, cosf(ANG(yaw))
    };

    mathExtra::matrix<float,3,3> maRoll = {
        cosf(ANG(roll)), sinf(ANG(roll)), 0,
        -sinf(ANG(roll)), cosf(ANG(roll)), 0,
        0, 0, 1
    };
    return mathExtra::dotProduct(mathExtra::dotProduct(maYaw,maPitch),maRoll);
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

int Geometry::onCreate(){
    /*
    FILE *file;
    file = fopen("teapot.txt","rb");
    if (file != NULL){
        
    }*/
    return 0;
}

int Geometry::onInput(int keyCode){
    switch (keyCode){
        case VK_ESCAPE: runTest = FALSE;break;
            
        case 'A': 
            posX-= (elappsedTime*sensitivity)*sinf(ANG((yaw+90)));
            posZ-= (elappsedTime*sensitivity)*cosf(ANG((yaw+90)));
            break;

        case 'D': 
            posX+= (elappsedTime*sensitivity)*sinf(ANG((yaw+90)));
            posZ+= (elappsedTime*sensitivity)*cosf(ANG((yaw+90)));
            break;

        case 'W': 
            posX+= (elappsedTime*sensitivity)*sinf(ANG(yaw));
            posZ+= (elappsedTime*sensitivity)*cosf(ANG(yaw));
            break;

        case 'S': 
            posX-= (elappsedTime*sensitivity)*sinf(ANG(yaw));
            posZ-= (elappsedTime*sensitivity)*cosf(ANG(yaw));
            break;

        case VK_SPACE: posY+= elappsedTime*sensitivity;break;
        case VK_SHIFT: posY-= elappsedTime*sensitivity;break;

        //case 'Q': roll = (roll < 360)? roll+=1: roll-360;break;
        //case 'E': roll = (roll > -360)? roll-=1: roll+360;break;

        case VK_UP: pitch = (pitch > -360)? pitch-=1: pitch+360;break;
        case VK_DOWN: pitch = (pitch < 360)? pitch+=1: pitch-360;break;
        case VK_LEFT: yaw = (yaw > -360)? yaw-=1: yaw+360;break;
        case VK_RIGHT: yaw = (yaw < 360)? yaw+=1: yaw-360;break;
    }
    return 0;
}

int Geometry::onUpdate(){
    cls();
    PIXEL_ATTR lineAttr{4,WHITE};
    auto temp = getRotationMatrix();

    for (int i = 0; i < sizeof(f)/sizeof(f[0]); i++){
        FCOORD cord[3] = {0};
        mathExtra::matrix<float,3,1> maCord;
        TRID triCord[3] = {0};
        for (int j = 0; j < 3; j++){
            float Lx = scale*cube[ f[i][j] - 1 ].x;
            float Ly = scale*cube[ f[i][j] - 1 ].y;
            float Lz = scale*cube[ f[i][j] - 1 ].z;
            maCord = {Lx - posX,Ly - posY,Lz - posZ};

            auto test = mathExtra::dotProduct(temp,maCord);
            test[2][0] = (test[2][0] < 0)? 0: test[2][0];

            //triCord[j] = {test[0][0],test[1][0],test[2][0]};
            cord[j] = projection(test[0][0],test[1][0],test[2][0]);
        }

        /*
        // FLAT SHADING (FORCE CODE)
        float vecto1[3] = {
			triCord[1].x - triCord[0].x,
			triCord[1].y - triCord[0].y,
			triCord[1].z - triCord[0].z
		};
		float vecto2[3] = {
			triCord[2].x - triCord[0].x,
			triCord[2].y - triCord[0].y,
			triCord[2].z - triCord[0].z
		};
		// tim vec to phap tuyen cua tung tam giac (cross product)
		float tchX = vecto1[1] * vecto2[2] - vecto1[2] * vecto2[1];
		float tchY = vecto1[2] * vecto2[0] - vecto1[0] * vecto2[2];
        float tchZ = vecto1[0] * vecto2[1] - vecto1[1] * vecto2[0];

		// tich vo huong (dot product)
		float tichVoHuong = triCord[2].x * tchX +
			triCord[2].y * tchY + (triCord[2].z - cz)*tchZ;

		int Le = 4 * (-tichVoHuong /
				(sqrt(pow(triCord[2].x, 2) + pow(triCord[2].y, 2) + pow(triCord[2].z - cz, 2))*
					sqrt(pow(tchX, 2) + pow(tchY, 2) + pow(tchZ, 2))));

                
        if (tichVoHuong > 0){ // BACKFACE CULLING
            drawTri(cord,{4,BLUE},{Le+1,WHITE});
        }*/
        drawTri(cord,{4,BLUE});
    }

    //float Lx,Ly,Lz;
    /*
    for (int i = 0; i < 12; i++){
    }*/

    /*
    cord[0] = {posX,posY};
    cord[1] = {10,30};
    cord[2] = {50,10};*/

    //test+= 0.05;
    return 0;
}