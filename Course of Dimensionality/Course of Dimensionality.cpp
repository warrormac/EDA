// Course of Dimensionality.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Course of Dimensionality.h"
#include <cmath>
#include <vector>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;


int Mat(vector<vector<int>> &vec, int row, int col)
{
  

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

    return 0;
    
}

int distance(vector<int> vect1, vector<int> vect2)
{
    int suma = 0;
    for (size_t i = 0; i < vect1.size(); i++)
        suma += pow(vect1[i] - vect2[i], 2);
    return sqrt(suma);
}
void ecli(vector<vector<int>>& vect1, vector<vector<int>>& vect2, int col)
{
    for (size_t i = 0; i < col; i++)
    {
        for (size_t j = i + 1; j < col; j++)
        {
            distance(vect1[i], vect2[j]);
        }
    }
}


int main() {
   
    
    vector<vector<int>> vect1;
    vector<vector<int>> vect2;


    int row=0, col=0;
    cout << "insert numer of rows: ";
    cin >> row;
    cout << "insert number of col: ";
    cin >> col;
    
    Mat(vect1,row,col);
    Mat(vect2, row, col);

   
    auto begin = chrono::high_resolution_clock::now();
    ecli(vect1, vect2, col);
   
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
    cout << ms<<"ms" << endl;
  
   /* for (int i = 0; i < vect1.size(); i++) {
        for (int j = 0; j < vect1[i].size(); j++)
            cout << vect1[i][j] << " ";
        cout << endl;
    }

    cout << "\n";
    cout << "\n";
    cout << "\n";

    for (int i = 0; i < vect2.size(); i++) {
        for (int j = 0; j < vect2[i].size(); j++)
            cout << vect2[i][j] << " ";
        cout << endl;
    }
    */


}

