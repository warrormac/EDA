#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>
#include <QMouseEvent>
#include "octree.h"

using namespace std;
class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);

    bool pointIN(Point<float> a);

    vector<Point<float>> puntosX;
    vector<Point<float>> linesA;
    vector<Point<float>> linesB;
    vector<Point<float>*> pSelec;

    virtual void paintGL();
    OctTree *oct ;

    void printQT(OctTree **quad);

    float zoomScale;

protected:
    Point<float> circulo, figura;
    bool pressLB;
    float radio, angle;

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    void findC(OctTree **p);
    bool circleInt(Point<float> tl, Point<float> br);
    bool circleInt2(Point<float> tl, Point<float> br);


    bool pointC(Point<float> a,Point<float> max, Point<float> min);
    void pushPoint(OctTree **p);


private:
    QOpenGLContext *context;
    QOpenGLFunctions *openGLFunctions;
};

