#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QGLWidget>

#include <iostream>
#include <vector>
#include "mainwindow.h"
#include <fstream>
#include <QWheelEvent>
#include <math.h>

using namespace std;

class QWheelEvent;
class GLWindow : public QGLWidget
{
    Q_OBJECT
public:
    GLWindow(QWidget *parent = 0);
    ~GLWindow();

    //void iniPointVec();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void draw();

    void drawPlatform();  //画平台框


    int pointNum;

    //处理载入的模型文件
    //void loadDxf(const wchar_t*);  //读文件，将点的坐标存在glyPts中
     //int getPointIndex(float,float);
    //判断某点是否是载入图形上的点，若是就返回该点在glyPts[0]中的编号

    bool getSetting();
    void getMousePos(float &p_x, float &p_y); //获取鼠标的坐标位置


signals:
    void SignalChosenPointChanged(int index,float xValue,float yValue);

private:

    void calcWheelIndex();									//工具函数

    bool choosePoint(float p_x, float p_y);

    int chosenPointIndex(int) const;

    float m_curZ;

    bool m_isChosen;

    bool m_isMultiChoose;  //是否多选

//    vector<vector<CDxfPt2d>> m_pPointPos; //图元的点集

    int m_pointIndex;  //模型上点的编号

    vector<int> m_tagVec;  //存选中的点的编号

    bool isSetting;
    bool m_isContextMenu;


    /*-----视图参数-----*/
    QPoint	m_lastPoint;
    float m_mousePosX;
    float m_mousePosY;

    static int s_wheelIndex;
    GLfloat m_scale;
    GLfloat m_translateX;
    GLfloat m_translateY;

    bool m_isPositive;
    bool m_isRotate;
    std::map<int, float> m_wheelIndexMap;

    QString wStr, mStr, estStr;
    QString selectModel;

    //滚轮缩放函数
    float m_baseStep;
    int m_wheelStep;
    int m_wheelIndex;
    std::vector<float> m_minStepVec;
    std::vector<float> m_wheelVec;
    std::vector<float> m_scaleVec;
    std::vector<float> m_showStepVec;


protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // GLWINDOW_H
