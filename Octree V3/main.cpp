#include "QT.h"
#include <QApplication>
#include <QString>
#include <QTextStream>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;


int main(int argc, char *argv[])
{
    int i=0;
    QApplication a(argc, argv);
    MainWindow x;
    QVector<float> puntos;
    QString archivo = "C:/Users/andre/Documents/untitled1/puntos3d/castle.txt";

    QFile mFile(archivo);

    if(!mFile.open(QFile::ReadOnly | QFile:: Text))
        return 0;


    QTextStream in(&mFile);
    QString mText = in.readAll();
    mText.remove('v');


    QStringList strList = mText.split(QRegExp("[ ]"),QString::SkipEmptyParts);
    float aum=0.5;

    for(int i=0; i<strList.length(); i++)
        puntos.push_back(strList[i].toFloat()*aum);


    mFile.close();

    while (i<puntos.size()-3)
    {
        x.oct->insert(new Point<float>(puntos[i],(puntos[i+1]-0.1),puntos[i+2]));
        x.puntosX.push_back(Point<float>(puntos[i],(puntos[i+1]-0.1),puntos[i+2]));
        i+=3;

    }




    x.paintGL();

    x.setTitle("Octree");
    x.resize(1000,1000);
    x.show();

    return a.exec();
}
