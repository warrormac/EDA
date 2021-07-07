#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <QtDebug>
using namespace std;

template <typename T>
struct Point
{

    T x,y,z;

    Point() {
        x=0.0;
        y=0.0;
        z=0.0;
    }

    Point(T a,T b, T c){
        x=a;
        y=b;
        z=c;
    }

    Point(T a,T b){
        x=a;
        y=b;
        z=0.0;
    }



};

class OctTree
{
public:
    OctTree(){

        dimMin = Point<float>(-1,-1,-1);
        dimMax = Point<float>(1,1,1);
        leaf = true;
    }
    OctTree(Point<float> min, Point<float> max){
        dimMax = max;
        dimMin = min;
        leaf = true;
    }



    OctTree *NWF;
    OctTree *SWF;
    OctTree *NEF;
    OctTree *SEF;
    OctTree *NWB;
    OctTree *SWB;
    OctTree *NEB;
    OctTree *SEB;


    OctTree* ubic(Point<float> *p, float *midX, float *midY, float *midZ){

        if(p->x<= *midX){
            if(p->y <= *midY){
                if(p->z <= *midZ)
                    return SWB;
                else
                    return SWF;

            }else{
                if(p->z <= *midZ)
                    return NWB;
                else
                    return NWF;

            }
        }else{
            if(p->y <= *midY){
                if(p->z <= *midZ)
                    return SEB;
                else
                    return SEF;

            }else{
                if(p->z <= *midZ)
                    return NEB;
                else
                    return NEF;
            }
        }
    }


    bool insert(Point<float> *pt)
    {
        int i=0;
        OctTree *point = nullptr;


        if(find(pt,&point)){
            return false;
        }

        if(point==nullptr)
            return false;

        if( point->stopCond() )
        {

            point->leaf = false;

            float midX = (point->dimMax.x + point->dimMin.x)/2;
            float midY = (point->dimMax.y + point->dimMin.y)/2;
            float midZ = (point->dimMax.z + point->dimMin.z)/2;

            point->NWF = new OctTree(Point<float>(point->dimMin.x,midY,midZ),Point<float>(midX,point->dimMax.y,point->dimMax.z));
            point->SWF = new OctTree(Point<float>(point->dimMin.x,point->dimMin.y,midZ),Point<float>(midX,midY,point->dimMax.z));
            point->NEF = new OctTree(Point<float>(midX,midY,midZ),point->dimMax);
            point->SEF = new OctTree(Point<float>(midX,point->dimMin.y,midZ),Point<float>(point->dimMax.x,midY,point->dimMax.z));
            point->NWB = new OctTree(Point<float>(point->dimMin.x,midY,point->dimMin.z),Point<float>(midX,point->dimMax.y,midZ));
            point->SWB = new OctTree(point->dimMin,Point<float>(midX,midY,midZ));
            point->NEB = new OctTree(Point<float>(midX,midY,point->dimMin.z),Point<float>(point->dimMax.x,point->dimMax.y,midZ));
            point->SEB = new OctTree(Point<float>(midX,point->dimMin.y,point->dimMin.z),Point<float>(point->dimMax.x,midY,midZ));

            while (i<int(point->data.size()))
            {
               (point->ubic(point->data[i],&midX,&midY,&midZ))->insert(point->data[i]);
               i++;
            }

            (point->ubic(pt,&midX,&midY,&midZ))->insert(pt);
            (point->data).clear();

        }
        else
            (point->data).push_back(pt);
        return true;
    }


    bool find(Point<float> *pt, OctTree **point ){
        float midX,midY,midZ;
        if(leaf){
            *point = this;
            for(int i=0;i<data.size();i++){

                if(data[i]->x == pt->x && data[i]->y == pt->y && data[i]->z == pt->z){
                    return true;
                }
            }
            return false;
        }
        return ubic(pt,&midX,&midY,&midZ)->find(pt,point);
    }




    void printQ(){
        qDebug() << data.size() << " tamanio";
    }

    bool leaf;
    Point<float> dimMax;
    Point<float> dimMin;
    vector<Point<float>*> data;

private:

    bool stopCond(){
        if(data.size() >= 10000){
            return true;
        }
        return false;

    }
};




#endif // OCTREE_H
