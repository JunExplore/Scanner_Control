#include "glwindow.h"

int GLWindow::s_wheelIndex = 0;

GLWindow::GLWindow(QWidget *parent)
    :QGLWidget(parent)
{
    m_translateX = 0.0;
    m_translateY = 0.0;
    m_scale = 1.0;

    m_isMultiChoose = false;

    isSetting = false;
    m_isContextMenu = true;

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

GLWindow::~GLWindow()
{
    makeCurrent();
    //delete dxfEng;
}

void GLWindow::initializeGL()
{
    glEnable(GL_DOUBLE);
    glShadeModel(GL_FLAT);

    glClearColor(255 / 255.0f, 255 / 100, 255 / 255.0f, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void GLWindow::resizeGL(int width, int height)
{
    if (0 == height)
        height = 1;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-width / 2, width / 2, -height / 2, height / 2, -50.0, 50.0);
    glOrtho(-width, 0, 0, height, -50.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//int GLWindow::getPointIndex(float p_x, float p_y)
//{
//	for(int i = 0;i<m_pPointPos[0].size();i++)
//	{
//		float dist = sqrtf((p_x - m_pPointPos[0][i].x)*(p_x - m_pPointPos[0][i].x) + (p_y - m_pPointPos[0][i].y)*(p_y - m_pPointPos[0][i].y));
//		if(dist <= 0.5)  //某阙值，后期可以和步距关联，设置成一个变量
//		{
//			return i;
//		}
//	}
//	return -1;
//}
//

void GLWindow::getMousePos(float &p_x, float &p_y)
{
    p_x = (m_lastPoint.x()-width())/m_scale - m_translateX;
    p_y = (height()-m_lastPoint.y())/m_scale - m_translateY;
}

int GLWindow::chosenPointIndex(int index) const
{
//    return PointFile::getFilePoint()->getchosenIndex(index,m_tagVec);
    return 1;
}

bool GLWindow::choosePoint(float p_x, float p_y)
{
//    m_pointIndex = PointFile::getFilePoint()->getPointIndex(p_x,p_y);
    //将点的坐标按照顺序装载在一个vector中，然后在这里有一个函数，当函数拾取到某个点是，就将该点的编号赋给m_pointIndex
//    if (m_pointIndex == -1)
//    {
//        m_tagVec.clear();   //m_tagVec表示选中的编号的集合，我处理的时候也需要允许选择多个点
//        //如果什么也没选，即点到了其他地方，就将原本选中的绿框去掉  还没有写选中某点之后画小绿色圈
//        return false;
//    }
//    else
//    {
//        int indice = chosenPointIndex(m_pointIndex);  //原模型上的选中的点的编号对应m_tagVec中的点的编号
//        if (m_isMultiChoose)  //多选，通过键盘按住ctrl来控制同时选择几个
//        {
//            if (indice == -1)
//            {
//                m_tagVec.push_back(m_pointIndex);
//            }
//            else
//                return true;
//        }
//        else  //单选
//        {
//            if (indice == -1)
//            {
//                m_tagVec.clear();
//                m_tagVec.push_back(m_pointIndex);
//            }
//            else
//                return true;
//        }
//        return true;
//    }
    return true;
}

bool GLWindow::getSetting()
{
    return isSetting;
}

void GLWindow::mousePressEvent(QMouseEvent *event)
{
    m_isContextMenu = true;
    m_lastPoint =  event->pos();

    m_mousePosX = (m_lastPoint.x() - width())/m_scale - m_translateX;
    m_mousePosY = (height() -m_lastPoint.y())/m_scale - m_translateY;

    if (event->button() & Qt::LeftButton)
    {
        m_isChosen = choosePoint(m_mousePosX, m_mousePosY);
    }

    if (event->button() & Qt::RightButton)
    {
        if (choosePoint(m_mousePosX, m_mousePosY))
        {
//            emit SignalChosenPointChanged(m_pointIndex,PointFile::getFilePoint()->getChosenCoord(m_tagVec)[0],PointFile::getFilePoint()->getChosenCoord(m_tagVec)[1]);  //这是当选择的点改变时，它呈现在文件书上的信息也相应做了改变
        }
    }
    //emit SignalChosenPointChanged(m_pointIndex);  //这是当选择的点改变时，它呈现在文件书上的信息也相应做了改变
    update();
}

void GLWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::RightButton && m_isContextMenu)
        setContextMenuPolicy(Qt::CustomContextMenu);
    update();
}


void GLWindow::mouseMoveEvent(QMouseEvent *event)
{
    m_isContextMenu = false;
    GLfloat dx = GLfloat(event->x() - m_lastPoint.x())/m_scale;
    GLfloat dy = GLfloat(event->y() - m_lastPoint.y())/m_scale;

    if ((event->buttons() & Qt::RightButton))
    {
        m_translateX += dx;
        m_translateY -= dy;
    }

    update();
    m_lastPoint = event->pos();
}


void GLWindow::calcWheelIndex()
{
    m_baseStep = 20;
    m_wheelStep = 8;
    float minStepVec[] = {100, 50, 20, 10, 5, 2, 1, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01};
    int stepSize = sizeof(minStepVec)/sizeof(minStepVec[0]);
    m_minStepVec.resize(stepSize);
    int allScaleSize = (stepSize-1)*m_wheelStep+1;
    m_scaleVec.resize(allScaleSize);
    m_wheelVec.resize(allScaleSize);
    m_showStepVec.resize(allScaleSize);
    std::vector<float> scaleStepVec(stepSize);
    for (int i = 0;i < stepSize;i++)
    {
        m_minStepVec[i] = minStepVec[i];
        scaleStepVec[i] = m_baseStep/m_minStepVec[i];
    }
    float lastScale, currentScale;

    for(int i=0;i<stepSize-1;i++)
    {
        lastScale = scaleStepVec[i];
        currentScale = scaleStepVec[i+1];
        float minStep = (currentScale - lastScale)/m_wheelStep;
        for (int j=0;j<m_wheelStep;j++)
        {
            m_scaleVec[i*m_wheelStep+j] = (scaleStepVec[i] + j*minStep);
            m_showStepVec[i*m_wheelStep+j] = m_minStepVec[i];
        }
    }
    m_scaleVec[allScaleSize-1] = scaleStepVec[stepSize-1];
    m_showStepVec[allScaleSize-1] = m_minStepVec[stepSize-1];

    for (int i=0;i<allScaleSize;i++)
    {
        if (abs(m_scaleVec[i]-1)<0.0001)
        {
            m_wheelIndex = i;
            break;
        }
    }
    for (int i=0;i<allScaleSize;i++)
        m_wheelVec[i] = i-m_wheelIndex;
}

void GLWindow::wheelEvent(QWheelEvent *event)
{
    GLfloat tempScale = m_scale;
    double numDegrees = event->delta() / 8.0;
    double numSteps = numDegrees /15.0;

    s_wheelIndex += numSteps;
    float minWheel = m_wheelVec.at(0)-1;
    float maxWheel = m_wheelVec.at(m_wheelVec.size()-1)+1;

    if (s_wheelIndex < maxWheel && s_wheelIndex > minWheel)
    {
        m_scale = m_scaleVec[s_wheelIndex+m_wheelIndex];
        update();
    }
    else
    {
        m_scale = tempScale;
        s_wheelIndex -= numSteps;
    }
}

void GLWindow::keyPressEvent(QKeyEvent *event)
{
    if(Qt::Key_Escape == event->key())
    {
        m_tagVec.clear();
        update();
    }
    else if (Qt::Key_Control == event->key())
        m_isMultiChoose = true;
}

void GLWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (Qt::Key_Control == event->key())
        m_isMultiChoose = false;
}

//void GLWindow::loadDxf(const wchar_t* fileName)
//{
//	IDxfParse* dxfEng = newDxfFileEng();
//	if(!dxfEng->dxfRead(fileName, 0))
//	{
//		//打开文件错误
//		int errorCode = dxfEng->getErrorType(); //获取错误码
//		delete dxfEng;
//		return;
//	}
//	dxfEng->setPara(10, 50.0);  //弦高误差，最大步长
//	dxfEng->getGlyphEntity(m_pPointPos);   //现在载入的图形的点已经存在了m_pPointPos中
//	delete dxfEng;
//
//	pointNum = m_pPointPos[0].size();
//}

void GLWindow::draw()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(m_scale, m_scale, 1.0);
    glTranslatef(m_translateX, m_translateY, 0);

//    PointFile::getFilePoint()->draw2D(m_tagVec);

    glPopMatrix();
    glPopAttrib();
    update();
}

void GLWindow::drawPlatform()
{
    //glPushAttrib(GL_ALL_ATTRIB_BITS);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glScalef(m_scale, m_scale, 1.0);
    glTranslatef(m_translateX, m_translateY, 0);

    float machineWidth =  1500;
    float machineLength = 1500;

        glEnable(GL_MULTISAMPLE);
        glLineWidth(1.5);
        glColor3f(0,0,0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-machineWidth, machineLength);
        glVertex2f(-machineWidth, 0.0);
        glVertex2f(0.0, 0.0);
        glVertex3f(0.0, machineLength, 0.0);
        glEnd();
        glPopMatrix();
        glPopAttrib();

}

void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    //glScalef(m_scale, m_scale, 1.0);
    calcWheelIndex();
    drawPlatform();
    draw();
    glPopAttrib();

}

