#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>


using namespace std;

class Point {

private:
    int pID, cID;
    int dim;
    vector<double> v;

public:
    Point(int id, string line) {
        dim = 0;
        pID = id;
        stringstream is(line);
        double val;
        while (is >> val) {
            v.push_back(val);
            dim++;
        }
        cID = 0; //Initially not assigned to any cluster
    }

    int getdim() {return dim;}
    int getCluster() {return cID;}
    int getID() {return pID;}
    void setCluster(int val) {cID = val;}
    double getVal(int pos) {return v[pos];}
};


class Cluster {

private:
    int cID;
    vector<double> cen;
    vector<Point> points;

public:
    Cluster(int cID, Point cen) {
        this->cID = cID;
        for (int i = 0; i < cen.getdim(); i++) {
            this->cen.push_back(cen.getVal(i));
        }
        this->addPoint(cen);
    }

    void addPoint(Point p) {
        p.setCluster(this->cID);
        points.push_back(p);
    }
    
    bool borrar(int size, int pID)
    {
        for (int i = 0; i < size; i++)
        {
            if (points[i].getID() == pID)
            {
                points.erase(points.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool removePoint(int pID) {
        int size = points.size();
        return borrar(size, pID);
        
    }

    int getId() {return cID;}
    Point getPoint(int pos) {return points[pos];}
    int getSize() {return points.size();}
    double getcenByPos(int pos) {return cen[pos];}
    void setcenByPos(int pos, double val) {this->cen[pos] = val;}
};

class Kmeans {
private:
    int K, iters, dim, tp;
    vector<Cluster> clusters;


    void cal(double sum, double min_dist, Point p, int *ptr)
    {
        for (int i = 1; i < K; i++)
        {
            double dist;
            sum = 0.0;

            for (int j = 0; j < dim; j++)
                sum += pow(clusters[i].getcenByPos(j) - p.getVal(j), 2.0);

            dist = sqrt(sum);

            if (dist < min_dist)
            {
                min_dist = dist;
                *ptr = clusters[i].getId();
            }
        }
    }
    int getNearestcID(Point point) {
        double sum = 0.0, min_dist;
        int NearestcID;

        for (int i = 0; i < dim; i++)
            sum += pow(clusters[0].getcenByPos(i) - point.getVal(i), 2.0);

        min_dist = sqrt(sum);
        NearestcID = clusters[0].getId();
        cal( sum, min_dist, point , &NearestcID);
        

        return NearestcID;
    }

public:

    Kmeans(int K, int iterations) {
        this->K = K;
        this->iters = iterations;
    }

    void run(vector<Point>& all_points) {

  
        tp = all_points.size();
        dim = all_points[0].getdim();
        int count = 0;
        double scale = 0.0001; //  limite al decimal

        vector<int> used_pIDs;

        for (int i = 1; i <= K; i++)
        {
            while (true)
            {
                int index = rand() % tp;

                if (find(used_pIDs.begin(), used_pIDs.end(), index) == used_pIDs.end())
                {
                    used_pIDs.push_back(index);
                    all_points[index].setCluster(i);
                    Cluster cluster(i, all_points[index]);
                    clusters.push_back(cluster);
                    break;
                }
            }
        }
        cout << "Clusters initialized = " << clusters.size() << endl << endl;



        int iter = 1;
        while (true)
        {
            cout << "Iter - " << iter << "/" << iters << endl;
            bool done = true;
            count = 0;
            while (count < tp)
            {
                int currentcID = all_points[count].getCluster();
                int nearestcID = getNearestcID(all_points[count]);

                if (currentcID != nearestcID)
                {
                    if (currentcID != 0) {
                        for (int j = 0; j < K; j++) {
                            if (clusters[j].getId() == currentcID) 
                                clusters[j].removePoint(all_points[count].getID());
                        }
                    }

                    for (int j = 0; j < K; j++) {
                        if (clusters[j].getId() == nearestcID) 
                            clusters[j].addPoint(all_points[count]);  
                    }
                    all_points[count].setCluster(nearestcID);
                    done = false;
                }
                count++;
            }
            
            count = 0;
            while (count < K)
            {
                int ClusterSize = clusters[count].getSize();

                for (int j = 0; j < dim; j++)
                {
                    double sum = 0.0;
                    if (ClusterSize > 0)
                    {
                        for (int p = 0; p < ClusterSize; p++)
                            sum += clusters[count].getPoint(p).getVal(j);
                        clusters[count].setcenByPos(j, sum / ClusterSize);
                    }
                }
                count++;
            }
            

            if (done || iter >= iters)
            {
                cout << "Clustering completed in iteration : " << iter << endl << endl;
                break;
            }
            iter++;
        }

        //Imprime los elemtos de qcada cluster 
       /*for (int i = 0; i < K; i++) {
           cout << "punto in cluster " << clusters[i].getId() << " : ";
           for (int j = 0; j < clusters[i].getSize(); j++) {
               cout << clusters[i].getPoint(j).getID() << " ";
           }
           cout << endl << endl;
       }*/

       //Write cluster centers to file
        count = 0;
        while (count < K)
        {
            cout << "Cluster " << clusters[count].getId() << " ";
            for (int j = 0; j < dim; j++)
                cout << clusters[count].getcenByPos(j) << " ";
            cout << "\n";
            count++;
        }

    }
};

bool verify(string name)
{
    ifstream infile(name.c_str());
    if (!infile.is_open())
        return 1;
    return 0;
}

int main()
{
    int K = 2;
    bool temp = 0;
    int it = 100;
    string filename = "prueba 1.txt";
    ifstream infile(filename.c_str());

    temp = verify(filename);

    if (temp == 1)
    {
        cout << "Error: archivo no encontrado " << endl;
        return 0;
    }
    else {
        int ID = 1;
        vector<Point> all_punto;
        string line;

        while (getline(infile, line)) {
            Point point(ID, line);
            all_punto.push_back(point);
            ID++;
        }
        infile.close();

        if (all_punto.size() < K) {
            //holi;
            return 1;
        }

       
        Kmeans kmean(K, it);
        kmean.run(all_punto);
    }


}