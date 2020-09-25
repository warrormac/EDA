#include <iostream> 
#include <vector> 
#include <sstream>
#include "fstream"
#include "string"
using namespace std;

#define TLF 0 
#define TRF 1 
#define BRF 2 
#define BLF 3 
#define TLB 4 
#define TRB 5 
#define BRB 6 
#define BLB 7 


struct Point {
    int x;
    int y;
    int z;
    Point()
        : x(-1), y(-1), z(-1) {}

    Point(float a, float b, float c)
        : x(a), y(b), z(c) {}
};

template <typename T >
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
    Octree(T x, T y, T z)
    {
        // To declare point node 
        point = new Point(x, y, z);
    }

    // Constructor with six arguments 
    Octree(T x1, T y1, T z1, T x2, T y2, T z2)
    {
        // This use to construct Octree 
        // with boundaries defined 
        if (x2 < x1 || y2 < y1 || z2 < z1) {
            cout << "point not valid" << endl;
            return;
        }

        point = nullptr;
        topLeftFront = new Point(x1, y1, z1);
        bottomRightBack = new Point(x2, y2, z2);

        // Assigning null to the children 
        children.assign(8, nullptr);
        for (int i = TLF; i <= BLB; ++i)
            children[i] = new Octree();
    }
    
    void oct(int *pos, int *midx, int *midy, int *midz, int x, int y, int z)
    {
        *midx = (topLeftFront->x + bottomRightBack->x) / 2;
        *midy = (topLeftFront->y + bottomRightBack->y) / 2;
        *midz = (topLeftFront->z + bottomRightBack->z) / 2;

        if (x <= *midx) {
            if (y <= *midy)
                (z <= *midz) ? *pos = TLF : *pos = TLB;

            else
                (z <= *midz) ? *pos = BLF : *pos = BLB;
        }
        else {
            if (y <= *midy)
                (z <= *midz) ? *pos = TRF : *pos = TLB;
            else
                (z <= *midz) ? *pos = BRF : *pos = BRB;
        }

    }

    // Function to insert a point in the octree 
    void insert(int x, int y, int z)
    {

        // If the point already exists in the octree 
        if (find(x, y, z)) {
            cout << "Point already exists" << endl;
            return;
        }

        // If the point is out of bounds 
        if (x < topLeftFront->x || x > bottomRightBack->x
            || y < topLeftFront->y || y > bottomRightBack->y
            || z < topLeftFront->z || z > bottomRightBack->z)
            return;

        // Binary search to insert the point 
        int midx, midy, midz;

        int pos = -1;
        
        // Checking the octant of 
        // the point 
        oct(&pos, &midx, &midy, &midz, x, y, z);

        // If an internal node is encountered 
        if (children[pos]->point == nullptr) {
            children[pos]->insert(x, y, z);
            return;
        }

        // If an empty node is encountered     //////////////de esto hago el delete
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
            if (pos == TLF) {
                children[pos] = new Octree(topLeftFront->x,
                    topLeftFront->y, topLeftFront->z, midx, midy, midz);
            }

            else if (pos == TRF) {
                children[pos] = new Octree(midx + 1,
                    topLeftFront->y, topLeftFront->z, bottomRightBack->x,
                    midy, midz);
            }
            else if (pos == BRF) {
                children[pos] = new Octree(midx + 1,
                    midy + 1, topLeftFront->z, bottomRightBack->x,
                    bottomRightBack->y, midz);
            }
            else if (pos == BLF) {
                children[pos] = new Octree(topLeftFront->x,
                    midy + 1, topLeftFront->z, midx, bottomRightBack->y, midz);
            }
            else if (pos == TLB) {
                children[pos] = new Octree(topLeftFront->x,
                    topLeftFront->y, midz + 1, midx, midy, bottomRightBack->z);
            }
            else if (pos == TLB) {
                children[pos] = new Octree(midx + 1,
                    topLeftFront->y, midz + 1, bottomRightBack->x, midy,
                    bottomRightBack->z);
            }
            else if (pos == BRB) {
                children[pos] = new Octree(midx + 1, midy + 1, midz + 1,
                    bottomRightBack->x, bottomRightBack->y, bottomRightBack->z);
            }
            else if (pos == BLB) {
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
        if (x < topLeftFront->x || x > bottomRightBack->x
            || y < topLeftFront->y || y > bottomRightBack->y
            || z < topLeftFront->z || z > bottomRightBack->z)
            return 0;

        // Otherwise perform binary search 
        // for each ordinate 
        int midx, midy, midz;

        int pos = -1;

        // Deciding the position 
        // where to move 
        oct(&pos, &midx, &midy, &midz, x, y, z);

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

    void del(int x, int y, int z)
    {
        int midx, midy, midz;
        int pos = -1;
        oct(&pos, &midx, &midy, &midz, x, y, z);

        if (children[pos]->point == NULL) 
            return children[pos]->del(x, y, z);
        
        else {
            delete children[pos];
            children[pos] = new Octree();
            return;
        }

    }
  
};

// Driver code 
int main()
{
    Octree<float> tree(-999, -999, -999, 999, 999, 999);

    int i = 0;
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

    while (i < v.size())
    {
        //cout << v[i] << "\n" << v[i+1] << "\n" << v[i+2] << "\n";
        tree.insert(v[i], v[i + 1], v[i + 2]);
        i += 3;
    }
    //tree.insert(-55.081, 152.48, 10.6415);
    //cout << (tree.find(-55.081, 152.48, 10.6415) ? "1\n" : "0\n");
    //tree.del(-55.081, 152.48, 10.6415);
    //cout << (tree.find(-55.081, 152.48, 10.6415) ? "1\n" : "0\n");


    MyReadFile.close();
    return 0;
}