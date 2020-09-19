// Implemetation of Octree in c++ 
#include <iostream> 
#include <vector> 
#include <sstream>
#include "fstream"
#include "string"
using namespace std;

#define TopLeftFront 0 
#define TopRightFront 1 
#define BottomRightFront 2 
#define BottomLeftFront 3 
#define TopLeftBottom 4 
#define TopRightBottom 5 
#define BottomRightBack 6 
#define BottomLeftBack 7 

// Structure of a point 
struct Point {
    int x;
    int y;
    int z;
    Point()
        : x(-1), y(-1), z(-1) {}

    Point(float a, float b, float c)
        : x(a), y(b), z(c) {}
};

// Octree class 
class Octree {

    // if point == NULL, node is internal node. 
    // if point == (-1, -1, -1), node is empty. 
    Point* point;

    // Represent the boundary of the cube 
    Point* topLeftFront, * bottomRightBack;
    vector<Octree*> children;

public:
    // Constructor 
    Octree()
    {
        // To declare empty node 
        point = new Point();
    }

    // Constructor with three arguments 
    Octree(float x, float y, float z)
    {
        // To declare point node 
        point = new Point(x, y, z);
    }

    // Constructor with six arguments 
    Octree(float x1, float y1, float z1, float x2, float y2, float z2)
    {
        // This use to construct Octree 
        // with boundaries defined 
        if (x2 < x1
            || y2 < y1
            || z2 < z1) {
            cout << "bounday poitns are not valid" << endl;
            return;
        }

        point = nullptr;
        topLeftFront
            = new Point(x1, y1, z1);
        bottomRightBack
            = new Point(x2, y2, z2);

        // Assigning null to the children 
        children.assign(8, nullptr);
        for (int i = TopLeftFront;
            i <= BottomLeftBack;
            ++i)
            children[i] = new Octree();
    }

    // Function to insert a point in the octree 
    void insert(int x, int y, int z)
    {

        // If the point already exists in the octree 
        if (find(x, y, z)) {
            cout << "Point already exist in the tree" << endl;
            return;
        }

        // If the point is out of bounds 
        if (x < topLeftFront->x || x > bottomRightBack->x
            || y < topLeftFront->y || y > bottomRightBack->y
            || z < topLeftFront->z || z > bottomRightBack->z)
        {
            return;
        }

        // Binary search to insert the point 
        int midx = (topLeftFront->x + bottomRightBack->x) / 2;
        int midy = (topLeftFront->y + bottomRightBack->y) / 2;
        int midz = (topLeftFront->z + bottomRightBack->z) / 2;

        int pos = -1;

        // Checking the octant of 
        // the point 
        if (x <= midx) {
            if (y <= midy)
                (z <= midz) ? pos = TopLeftFront : pos = TopLeftBottom;

            else
                (z <= midz) ? pos = BottomLeftFront : pos = BottomLeftBack;
        }
        else {
            if (y <= midy)
                (z <= midz) ? pos = TopRightFront : pos = TopRightBottom;
            else
                (z <= midz) ? pos = BottomRightFront : pos = BottomRightBack;
        }

        // If an internal node is encountered 
        if (children[pos]->point == nullptr) {
            children[pos]->insert(x, y, z);
            return;
        }

        // If an empty node is encountered 
        else if (children[pos]->point->x == -1) {
            delete children[pos];
            children[pos] = new Octree(x, y, z);
            return;
        }
        else {
            int x_ = children[pos]->point->x,
                y_ = children[pos]->point->y,
                z_ = children[pos]->point->z;
            delete children[pos];
            children[pos] = nullptr;
            if (pos == TopLeftFront) {
                children[pos] = new Octree(topLeftFront->x,
                    topLeftFront->y, topLeftFront->z, midx, midy, midz);
            }

            else if (pos == TopRightFront) {
                children[pos] = new Octree(midx + 1,
                    topLeftFront->y, topLeftFront->z, bottomRightBack->x,
                    midy, midz);
            }
            else if (pos == BottomRightFront) {
                children[pos] = new Octree(midx + 1,
                    midy + 1, topLeftFront->z, bottomRightBack->x,
                    bottomRightBack->y, midz);
            }
            else if (pos == BottomLeftFront) {
                children[pos] = new Octree(topLeftFront->x,
                    midy + 1, topLeftFront->z, midx, bottomRightBack->y, midz);
            }
            else if (pos == TopLeftBottom) {
                children[pos] = new Octree(topLeftFront->x,
                    topLeftFront->y, midz + 1, midx, midy, bottomRightBack->z);
            }
            else if (pos == TopRightBottom) {
                children[pos] = new Octree(midx + 1,
                    topLeftFront->y, midz + 1, bottomRightBack->x, midy,
                    bottomRightBack->z);
            }
            else if (pos == BottomRightBack) {
                children[pos] = new Octree(midx + 1, midy + 1, midz + 1,
                    bottomRightBack->x, bottomRightBack->y, bottomRightBack->z);
            }
            else if (pos == BottomLeftBack) {
                children[pos] = new Octree(topLeftFront->x,
                    midy + 1, midz + 1, midx,
                    bottomRightBack->y, bottomRightBack->z);
            }
            children[pos]->insert(x_, y_, z_);
            children[pos]->insert(x, y, z);
        }
    }

    // Function that returns true if the point 
    // (x, y, z) exists in the octree 
    bool find(int x, int y, int z)
    {
        // If point is out of bound 
        if (x < topLeftFront->x
            || x > bottomRightBack->x
            || y < topLeftFront->y
            || y > bottomRightBack->y
            || z < topLeftFront->z
            || z > bottomRightBack->z)
            return 0;

        // Otherwise perform binary search 
        // for each ordinate 
        int midx = (topLeftFront->x + bottomRightBack->x) / 2;
        int midy = (topLeftFront->y + bottomRightBack->y) / 2;
        int midz = (topLeftFront->z + bottomRightBack->z) / 2;

        int pos = -1;

        // Deciding the position 
        // where to move 
        if (x <= midx) {
            if (y <= midy) {
                (z <= midz) ? pos = TopLeftFront : pos = TopLeftBottom;
            }
            else {
                (z <= midz) ? pos = BottomLeftFront : pos = BottomLeftBack;
            }
        }
        else {
            if (y <= midy) {
                (z <= midz) ? pos = TopRightFront : pos = TopRightBottom;
            }
            else {
                (z <= midz) ? pos = BottomRightFront : pos = BottomRightBack;
            }
        }

        // If an internal node is encountered 
        if (children[pos]->point == nullptr) {
            return children[pos]->find(x, y, z);
        }

        // If an empty node is encountered 
        else if (children[pos]->point->x == -1)
            return 0;

        else {

            // If node is found with 
            // the given value 
            if (x == children[pos]->point->x && y == children[pos]->point->y
                && z == children[pos]->point->z)
                return 1;
        }
        return 0;
    }
};




// Driver code 
int main()
{
    Octree tree(-999, -999, -999, 999, 999, 999);

    int i = 0;
    float p, q, r;
    float* ptr = &p;
    float* ptr2 = &q;
    float* ptr3 = &r;

    string myText;
    vector <float> v;
    ifstream MyReadFile("puntos.txt");

    while (getline(MyReadFile, myText))
    {
        istringstream is(myText);
        float n;
        while (is >> n)
            v.push_back(n);
    }

    while (i < 84)
    {
        //cout << v[i] << "\n" << v[i+1] << "\n" << v[i+2] << "\n";
        tree.insert(v[i], v[i + 1], v[i + 2]);
        i += 3;

    }


    //tree.insert(-55.081, 152.48, 10.6415);

   //cout << (tree.find(-55.081, 152.48, 10.6415) ? "Found\n" : "Not Found\n");


    MyReadFile.close();

    return 0;
}