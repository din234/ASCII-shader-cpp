#include "include/consoleGraphic.h"

class GD:public ConsoleGraphic{
    private:
        double b_1 = 0;
        double w_1 = 0;

        double n = 0.00001;
        double b = b_1;
        double w = w_1;
        int epochs = 1000;

        int temp = 0;
    public:
        GD(){
            setMinTimePerFrame(66);
            createBuffering(3,240,160);
        };

        double cost();
    private:
        int onUpdate() override;
        std::string logger();
};

std::string GD::logger(){
    return "First iteration point is bias = "+std::to_string(b_1)+" and slope = "+std::to_string(w_1)+"\n"
            "After "+std::to_string(epochs)+" iteration step with learning rate = "+std::to_string(n)+":\n"
            "Bias (y-interception) = "+std::to_string(b)+"\n"
            "Weight (slope) = "+std::to_string(w)+"\n";
}

int GD::onUpdate(){

    if (temp == 0){
        cls();
        //printf("First iteration point is bias = %.4f and slope = %.4f\n",b,w);
        for (int i = 0; i < epochs; i++){
            double db = -2*(280 - (b*8 + 343*w))/8;
            double dw = -2*(14370 - (b*343 + 17756*w))/8;
            b = b-n*db;
            w = w-n*dw;
            drawLine(0,getBuffHeight()-(b*10),getBuffWidth(),getBuffHeight()-(getBuffWidth()*w + (b*10)),{4,BLUE});
        }

        double b_0 = 1.7529;
        double w_0 = 0.7754;

        drawLine(0,getBuffHeight()-(b_0*10),getBuffWidth(),getBuffHeight()-(getBuffWidth()*w_0 + (b_0*10)),{4,WHITE});
        drawLine(0,getBuffHeight()-(b*10),getBuffWidth(),getBuffHeight()-(getBuffWidth()*w + (b*10)),{4,RED});
        
        temp++;
    }
    return 0;
}