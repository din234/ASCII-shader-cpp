#include "include/consoleGraphic.h"

class SortAlg:public ConsoleGraphic{
    private:
        float sensitivity = 20.00f / 1000.00f;
        std::vector<int> array;

        int i,j;
        bool set = false;

        int col,width;
    public:
        SortAlg();
    private:
        int onInput(int keyCode) override;
        int onUpdate() override;
    private:
        void selectionSort(int loop);
        void insertionSort(int loop);
        std::vector<int> mergeSort(std::vector<int> vect);
};


SortAlg::SortAlg(){
    i = 0; j = 1;

    array.resize(250);
    col = array.size();

    setMinTimePerFrame(33);
    createBuffering(5,col,50);

    width = getBuffWidth()/col;

    //srand(time(NULL));
    for (int insert = 0; insert < array.size(); insert++){
        array[insert] = rand() % getBuffHeight();
    }

}

int SortAlg::onInput(int keyCode){
    switch (keyCode){
        case VK_ESCAPE: runTest = FALSE;break;
    }
    return 0;
}



void SortAlg::selectionSort(int loop){
    if (!set){
        i = 0; j = i+1;
        set = true;
    }

    for (int t = 0; t < loop;t++){
        if (j < col){
            if (array[i] < array[j]) { 
                std::swap(array[i], array[j]); // a4
	        }
            j++;
        } else if (i < col){
            i++;
            j = i+1;
        }
    }

    for (int r = 0; r < col; r++){
        if (r == i){
            drawRect( (float) r*width + 1, 1.0f, (float) width, (float) array[r], 
            {4, RED },{4,WHITE});
        } else {
            drawRect( (float) r*width + 1, 1.0f, (float) width, (float) array[r], 
            {4, (r == j)? GREEN:WHITE },{4,WHITE});
        }
    }
}

void SortAlg::insertionSort(int loop){
    if (!set){
        i = 1; j = i-1;
        set = true;
    }

    for (int t = 0; t < loop;t++){
        if (j >= 0 && array[j+1] < array[j]){
            std::swap(array[j+1], array[j]); //a3 (1+2+3+4..)
            j--;
        } else if (i < (col - 1)){
            i++;
            j = i-1;
        }
    }

    for (int r = 0; r < col; r++){
        if (r == i){
            drawRect( (float) r*width + 1, 1.0f, (float) width, (float) array[r], 
            {4, RED },{4,WHITE});
        } else {
            drawRect( (float) r*width + 1, 1.0f, (float) width, (float) array[r], 
            {4, (r == j)? GREEN:WHITE },{4,WHITE});
        }
    }
}

std::vector<int> SortAlg::mergeSort(std::vector<int> vect){
	int left = 0;
	int right = vect.size();
	int m = (left + right)/2;
 
	std::vector<int> L;
    std::vector<int> R;

    for (int i = left; i < m ;i++){L.push_back(vect[i]);}// n/2
    for (int j = m; j < right;j++){R.push_back(vect[j]);}// n/2
	

	// Conquer time = 2*T(n/2) + 4*T(n/4) + ... a*T(n/a) = log2(a) * T(n)
	if (L.size() > 2){
		L = mergeSort(L);
	}
	if (R.size() > 2){
		R = mergeSort(R);	
	}


	// Combine time = C(n)
	//cout << "SORTED: ";
	int lp = 0;
	int rp = 0;
	for (int tp = 0;tp < right;tp++){
		if ((L[lp] < R[rp] || rp >= right - m) && lp < m) {
			vect[tp] = L[lp];
			lp++;
		} else {
			vect[tp] = R[rp];
			//cout << rp;
			rp++;
		}
		//cout << vect[tp] << " ";
	}

	// cout << endl << "test================================" << endl;
	return vect;
}




int SortAlg::onUpdate(){
    cls();
    //array = mergeSort(array);
    //selectionSort(15);
    insertionSort(1);
    return 0;
}