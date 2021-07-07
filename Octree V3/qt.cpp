#include "qt.h"
#include <QtMath>
#include <cmath>



bool MainWindow::pointIN(Point<float> a){
    if(sqrt(pow((figura.x-a.x),2)+pow((figura.y-a.y),2)+pow((figura.z-a.z),2)) <= pow(radio,2))
         return true;

    return false;
}

bool MainWindow::pointC(Point<float> a,Point<float> max, Point<float> min){
    if(a.x >= min.x && a.y >= min.y && a.z >= min.z &&
       a.x <= max.x && a.y <= max.y && a.z <= max.z){


         return true;
    }
    return false;
}


bool MainWindow::circleInt(Point<float> max, Point<float> min){
    if(pointIN(max) && pointIN(min)
      && pointIN(Point<float>(max.x,max.y,min.z)) && pointIN(Point<float>(min.x,max.y,max.z))
      && pointIN(Point<float>(max.x,min.y,max.z)) && pointIN(Point<float>(max.x,min.y,min.z))
      && pointIN(Point<float>(min.x,max.y,min.z)) && pointIN(Point<float>(min.x,min.y,max.z)) )
        return true;
    return false;

}

bool MainWindow::circleInt2(Point<float> max, Point<float> min){
    if(pointIN(max) || pointIN(min)
      || pointIN(Point<float>(max.x,max.y,min.z)) || pointIN(Point<float>(min.x,max.y,max.z))
      || pointIN(Point<float>(max.x,min.y,max.z)) || pointIN(Point<float>(max.x,min.y,min.z))
      || pointIN(Point<float>(min.x,max.y,min.z)) || pointIN(Point<float>(min.x,min.y,max.z)) )
        return true;
    return false;

}



MainWindow::MainWindow()
{

    setSurfaceType(QWindow::OpenGLSurface);


    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2,1);

    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();

    context->makeCurrent(this);
    openGLFunctions = context->functions();

    Point<float> min(-1.0,-1.0,-1.0);
    Point<float> max(1.0,1.0,1.0);

    oct = nullptr;
    oct = new OctTree(min,max);

    circulo.x = 0.0;
    circulo.y = 0.0;
    circulo.z = 0.0;

    figura.x = 0.0;
    figura.y = 0.0;
    figura.z = 0.0;

    angle= 0.2;

    zoomScale = 1.0;

    pressLB = false;
    radio = 0.5;


}

MainWindow::~MainWindow()
{
    delete oct;

}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
        //sens x,y
        circulo.x = float(ev->x())/float(this->width())-0.9;
        circulo.y = float(ev->y())/float(this->height())-0.5;
         circulo.x *= 5;
       circulo.y *= -5;

       figura.x = circulo.x ;
       figura.y = circulo.y ;

    //}
    update();
}

void MainWindow::wheelEvent(QWheelEvent *ev){

    QPoint numDegrees = ev->angleDelta();
    if (numDegrees.y() < 0)  zoomScale = zoomScale/1.1;
    if (numDegrees.y() > 0)  zoomScale = zoomScale*1.1;
    update(); // call paintGL()



}

void MainWindow::pushPoint(OctTree **p)
{
    if((*p)->leaf){
        for(int i=0; i<(*p)->data.size();i++){
            pSelec.push_back((*p)->data[i]);
        }
    }else{
        pushPoint(&(*p)->NEB);
        pushPoint(&(*p)->SEB);
        pushPoint(&(*p)->NWB);
        pushPoint(&(*p)->SWB);
        pushPoint(&(*p)->NEF);
        pushPoint(&(*p)->SEF);
        pushPoint(&(*p)->NWF);
        pushPoint(&(*p)->SWF);
    }
}

void MainWindow::findC(OctTree **p){

    //qDebug()<< "comenzOOOOOOOp" <<endl;

    if(circleInt((*p)->dimMax,(*p)->dimMin)){
        pushPoint(p);

        return;
    }


    if(circleInt2((*p)->dimMax,(*p)->dimMin) ||
            pointC(Point<float>(figura.x,figura.y,figura.z),(*p)->dimMax,(*p)->dimMin) ){

        if((*p)->leaf){
            for(int i=0;i<(*p)->data.size();i++){
                if(pointIN(*(*p)->data[i]))
                    pSelec.push_back((*p)->data[i]);
            }
            return;
        }else{
            findC(&(*p)->NEB);
            findC(&(*p)->SEB);
            findC(&(*p)->NWB);
            findC(&(*p)->SWB);
            findC(&(*p)->NEF);
            findC(&(*p)->SEF);
            findC(&(*p)->NWF);
            findC(&(*p)->SWF);

        }
    }





}

void MainWindow::keyPressEvent(QKeyEvent *ev){
    if(ev->key() == 70){

        pSelec.clear();
        findC(&oct);
        update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    //graph
    printQT(&oct);
    update();
}

void MainWindow::initializeGL()
{
    resizeGL(this->width(),this->height());
}


void MainWindow::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,w,h);

   qreal radio = qreal(w)/qreal(h);
   glOrtho(-1* radio,1*radio,-1,1,1,-1);


}



void MainWindow::paintGL()
{

    //color del fondo
    glClearColor(255, 255, 255, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    //el tama;o de los puntos
    glScaled(zoomScale, zoomScale, zoomScale);
    //qDebug() << zoomScale ;
    glPointSize(5);

    //el agulo que se mostrara
    gluPerspective(50.0,1.0,1.0,25.0);
    glTranslatef(0,0,-6);


    glRotatef(circulo.x*100,0,1,0);
    glRotatef(circulo.y*100,1,0,0);

    pressLB = false;



    //color
    glBegin(GL_POINTS);
    glColor3d(255,0,0);
    for(int i=0;i<int(puntosX.size());i++)
        glVertex3d(puntosX[i].x,puntosX[i].y,puntosX[i].z);

    glEnd();

    //qDebug() <<pSelec.size() << "puntos selec " << endl;

    glBegin(GL_POINTS);
    glColor3d(0,0,255);
    for(int i=0;i<int(pSelec.size());i++){

        glVertex3d(pSelec[i]->x,pSelec[i]->y,pSelec[i]->z);
    }
    glEnd();




    glBegin(GL_LINES);
    glColor3d(255,0,0);
    for(int i=0;i<int(linesA.size());i++){

        glVertex3d(linesA[i].x,linesA[i].y,linesA[i].z);
        glVertex3d(linesB[i].x,linesB[i].y,linesB[i].z);
    }

    glEnd();


    glFlush();


}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeGL(this->width(),this->height());
    this->update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    paintGL();

}

void MainWindow::printQT(OctTree **quad)
{


    if(!((*quad)->leaf)){

        float midX = ((*quad)->dimMax.x + (*quad)->dimMin.x)/2;
        float midY = ((*quad)->dimMax.y + (*quad)->dimMin.y)/2;
        float midZ = ((*quad)->dimMax.z + (*quad)->dimMin.z)/2;

        linesA.push_back(Point<float>((*quad)->dimMin.x,midY,midZ));
        linesB.push_back(Point<float>((*quad)->dimMax.x,midY,midZ));

        linesA.push_back(Point<float>(midX,midY,(*quad)->dimMin.z));
        linesB.push_back(Point<float>(midX,midY,(*quad)->dimMax.z));

        linesA.push_back(Point<float>(midX,(*quad)->dimMin.y,midZ));
        linesB.push_back(Point<float>(midX,(*quad)->dimMax.y,midZ));

        linesA.push_back(Point<float>((*quad)->dimMin.x,midY,(*quad)->dimMax.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,midY,(*quad)->dimMax.z));

        linesA.push_back(Point<float>(midX,(*quad)->dimMin.y,(*quad)->dimMax.z));
        linesB.push_back(Point<float>(midX,(*quad)->dimMax.y,(*quad)->dimMax.z));


        linesA.push_back(Point<float>((*quad)->dimMin.x,midY,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,midY,(*quad)->dimMin.z));

        linesA.push_back(Point<float>(midX,(*quad)->dimMin.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>(midX,(*quad)->dimMax.y,(*quad)->dimMin.z));

        linesA.push_back(Point<float>((*quad)->dimMin.x,midY,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMin.x,midY,(*quad)->dimMax.z));

        linesA.push_back(Point<float>((*quad)->dimMax.x,midY,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,midY,(*quad)->dimMax.z));


        linesA.push_back(Point<float>(midX,(*quad)->dimMax.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>(midX,(*quad)->dimMax.y,(*quad)->dimMax.z));
        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMax.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMax.y,(*quad)->dimMax.z));
        linesA.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMax.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMax.y,(*quad)->dimMax.z));

        linesA.push_back(Point<float>(midX,(*quad)->dimMin.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>(midX,(*quad)->dimMin.y,(*quad)->dimMax.z));
        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMin.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMin.y,(*quad)->dimMax.z));
        linesA.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMin.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMin.y,(*quad)->dimMax.z));


        linesA.push_back(Point<float>(midX,(*quad)->dimMax.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>(midX,(*quad)->dimMax.y,(*quad)->dimMax.z));

        linesA.push_back((*quad)->dimMin);
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMin.y,(*quad)->dimMin.z));

        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMax.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMax.y,(*quad)->dimMin.z));

        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMax.y,midZ));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMax.y,midZ));

        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMax.y,(*quad)->dimMax.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMax.y,(*quad)->dimMax.z));

        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMin.y,midZ));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMin.y,midZ));

        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMin.y,(*quad)->dimMax.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMin.y,(*quad)->dimMax.z));

        linesA.push_back((*quad)->dimMin);
        linesB.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMax.y,(*quad)->dimMin.z));

        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMin.y,midZ));
        linesB.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMax.y,midZ));

        linesA.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMin.y,(*quad)->dimMax.z));
        linesB.push_back(Point<float>((*quad)->dimMin.x,(*quad)->dimMax.y,(*quad)->dimMax.z));

        linesA.push_back((*quad)->dimMax);
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMin.y,(*quad)->dimMax.z));


        linesA.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMax.y,midZ));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMin.y,midZ));
        linesA.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMax.y,(*quad)->dimMin.z));
        linesB.push_back(Point<float>((*quad)->dimMax.x,(*quad)->dimMin.y,(*quad)->dimMin.z));


            printQT(&((*quad)->NEB));
            printQT(&((*quad)->SEB));
            printQT(&((*quad)->NWB));
            printQT(&((*quad)->SWB));
            printQT(&((*quad)->NEF));
            printQT(&((*quad)->SEF));
            printQT(&((*quad)->NWF));
            printQT(&((*quad)->SWF));

    }
}
