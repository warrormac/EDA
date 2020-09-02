// Course of Dimensionality.cpp : Defines the entry point for the application.
//

#include "Course of Dimensionality.h"
#include <iostream>
#include <vector>
#include <time.h>

using namespace std;


int Mat(vector<vector<int>> vec, int row, int col)
{
    

    cout << "insert numer of rows: ";
    cin >> row;
    cout << "insert number od col: ";
    cin >> col; 



    srand(time(NULL));
    // Elements to insert in column 
    int num = 1 + rand() % (10000 - 1);

    // Inserting elements into vector 
    for (int i = 0; i < row; i++) {
        // Vector to store column elements 
        vector<int> v1;

        for (int j = 0; j < col; j++) {
            v1.push_back(num);
            num= 1 + rand() % (10000 - 1);
        }

        // Pushing back above 1D vector 
        // to create the 2D vector 
        vec.push_back(v1);
    }

    /* Displaying the 2D vector 
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec[i].size(); j++)
            cout << vec[i][j] << " ";
        cout << endl;
    }*/
    return 0;
    
}



int main() {

    int row=0, col=0;
    vector<vector<int>> vect;
    Mat(vect,row,col);

}

