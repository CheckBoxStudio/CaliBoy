#include "scenerender.h"
#include <QMouseEvent>
#include <QWheelEvent>

#include <gl\gl.h>
#include <gl\glu.h>
#include <cv.h>
#include "Calib\camera.h"

SceneRender::SceneRender(cameraScene *calibScene, QWidget *parent)
	: QGLWidget(parent)
{
	initData();
	setScene(calibScene);
}
SceneRender::~SceneRender()
{	
}
void SceneRender::initData()
{
	m_scene = 0;
	m_viewIdx = -1;
	m_fixCamera = true;
	m_showAll = true;
	
	m_l = 30.0;
	m_fov = 45.0;	
	m_dist = 100.0;
	m_angleX = 6.0;
	m_angleY = 36.0;
	m_bRotating = false;
}

void SceneRender::setScene(cameraScene *calibScene)
{
	m_scene = calibScene;
	updateScene();
}
void SceneRender::setView(const int viewIdx)
{
	m_viewIdx = viewIdx;
	update();
}
void SceneRender::updateScene()
{
	if (m_scene != 0 && m_scene->viewCount() != 0) {
		m_l = (m_scene->m_sizeX + m_scene->m_sizeY) / 2.0;
		if (m_l<1.0)
			m_l = 1.0;

		double lSum = 0.0;
		int n = 0;
		for (int i=0; i<m_scene->viewCount(); ++i) {
			cameraView *view = m_scene->m_views.at(i);
			double lt = cv::norm(view->t);
			if (view->isGridCornerPicked() && lt > 10.0 ) {
				n++;
				lSum += lt;
			}
		}
		if (n>0)
			m_dist = lSum/n;
	} else {
		m_l = 30.0;
		m_dist = 100.0;
	}

	setView(-1);
}

void SceneRender::setFixCamera(bool bOn)
{
	m_fixCamera = bOn;
	update();
}
void SceneRender::setShowAll(bool bOn)
{
	m_showAll = bOn;
	update();
}

void SceneRender::initializeGL()
{
	glClearColor( 1.0, 1.0, 1.0, 0.0 );
	glClearDepth( 1.0 );

	glEnable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}
void SceneRender::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//=========================================

	glMatrixMode(GL_PROJECTION);
	GLint rect[4];
	glGetIntegerv(GL_VIEWPORT, rect);
	if(rect[3] < 1) rect[3]=1;
	glLoadIdentity();
	gluPerspective(m_fov, 1.0*rect[2]/rect[3], 0.0, m_dist*2);

	//=========================================

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 参看控制
	glTranslated(0.0, 0.0, -m_dist);
	glRotated(180.0, 1.0, 0.0, 0.0);
	glRotated(m_angleY, 0.0, 1.0, 0.0);
	glRotated(m_angleX, 1.0, 0.0, 0.0);
	
	// 场景绘制
	drawCoordinate(m_l*1.5);
	if (m_fixCamera)
		drawScene_CameraFixed();
	else
		drawScene_PlaneFixed();
}
void SceneRender::resizeGL( int width, int height )
{
	if ( height == 0 ) height = 1;
	glViewport( 0, 0, (GLint)width, (GLint)height );
}

void SceneRender::drawScene_CameraFixed()
{
	
	if (m_scene != 0 && m_scene->m_camera.cx > 10.0) {
		// 场景不为空,初步判断相机标定过(无法判断是否为当前刚标定的!)

		// 1.绘制固定相机
		double s = 1/10.0;	// 绘制相机缩放比例
		glColor3f(1.0,0.0,1.0);
		drawCamera((m_scene->m_camera.fx+m_scene->m_camera.fy)/2.0 * s,
				   m_scene->m_camera.cx * s , m_scene->m_camera.cy * s);

		// 2.绘制棋盘格
		for (int i=0; i<m_scene->m_views.size(); ++i) {
			cameraView *view = m_scene->m_views.at(i);
			if ((i == m_viewIdx || m_showAll) &&
				view->isGridCornerPicked() && cv::norm(view->t)>10.0) {
				// a. 是否需要花
				// b. 是否有棋盘格且是否标定过(无法判断是否为当前刚标定的!)
				
				glPushMatrix();


				// i.获取位置信息及棋盘格尺寸
				cv::Mat t = view->t;
				cv::Mat r = view->r;
				int nx, ny;
				view->getGridSize(nx,ny);
				// ii.绘制
				glTranslatef(t.at<double>(0),t.at<double>(1),t.at<double>(2));
				glRotatef(cv::norm(r)*180.0/CV_PI,r.at<double>(0),r.at<double>(1),r.at<double>(2));
				glColor3f(0.0,0.0,0.0);
				renderText(-15.0,-15.0, 0.0, QString("%1").arg(i+1));	// 打标签
				if (i == m_viewIdx)										// 设置颜色
					glColor3f(1.0,0.0,0.0);
				else
					glColor3f(0.0,1.0,1.0);
				drawPlane(nx,ny,m_scene->m_sizeX, m_scene->m_sizeY);	// 画平面

				glPopMatrix();
			}
		}
	}
}
void SceneRender::drawScene_PlaneFixed()
{
	if (m_scene != 0 && m_scene->m_camera.cx > 10.0) {
		// 场景不为空,初步判断相机标定过(无法判断是否为当前刚标定的!)

		int NX, NY, nx, ny;	// 统计棋盘格最大尺寸
		double s = 1/10.0;	// 绘制相机缩放比例
		NX = 2; NY = 2;

		for (int i=0; i<m_scene->m_views.size(); ++i) {
			cameraView *view = m_scene->m_views.at(i);
			if ((i == m_viewIdx || m_showAll) &&
				view->isGridCornerPicked() && cv::norm(view->t)>10.0) {
				// a. 是否需要花
				// b. 是否有棋盘格且是否标定过(无法判断是否为当前刚标定的!)

				glPushMatrix();
				
				// i.获取位置信息
				cv::Mat t = view->t;
				cv::Mat r = view->r;
				// ii.绘制
				glRotatef(-cv::norm(r)*180.0/CV_PI,r.at<double>(0),r.at<double>(1),r.at<double>(2));
				glTranslatef(-t.at<double>(0),-t.at<double>(1),-t.at<double>(2));
				glColor3f(0.0,0.0,0.0);
				renderText(-15.0,-15.0, 0.0, QString("%1").arg(i+1));				// 打标签
				if (i == m_viewIdx)													// 设置颜色
					glColor3f(1.0,0.0,0.0);
				else
					glColor3f(1.0,0.0,1.0);
				drawCamera((m_scene->m_camera.fx+m_scene->m_camera.fy)/2.0 * s,		// 画相机
						   m_scene->m_camera.cx * s , m_scene->m_camera.cy * s);

				glPopMatrix();


				// 统计最大棋盘格尺寸
				view->getGridSize(nx,ny);
				if (nx>NX) NX = nx;
				if (ny>NY) NY = ny;
			}
		}
		// 最后画棋盘格
		glColor3f(0.0,1.0,1.0);
		drawPlane(NX,NY,m_scene->m_sizeX,m_scene->m_sizeY);
	}
}
void SceneRender::drawCoordinate(const double l)
{
	GLfloat	fCurrentColor[4];
	glGetFloatv(GL_CURRENT_COLOR, fCurrentColor);

	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(l,0.0,0.0);
		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,l,0.0);
		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,l);
	glEnd();

	glColor4fv(fCurrentColor);
}
void SceneRender::drawCamera(const double f, const double halfx, const double halfy)
{
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(-halfx,-halfy,f);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f( halfx,-halfy,f);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f( halfx, halfy,f);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(-halfx, halfy,f);

		glVertex3f(-halfx,-halfy,f);
		glVertex3f( halfx,-halfy,f);
		glVertex3f( halfx,-halfy,f);
		glVertex3f( halfx, halfy,f);
		glVertex3f( halfx, halfy,f);
		glVertex3f(-halfx, halfy,f);
		glVertex3f(-halfx, halfy,f);
		glVertex3f(-halfx,-halfy,f);
	glEnd();
	drawCoordinate(f/2.0);
}
void SceneRender::drawPlane(const int nX, const int nY, const double lX, const double lY)
{
	const double xLength = (nX-1)*lX;
	const double yLength = (nY-1)*lY;

	glBegin(GL_LINES);
		// Y方向线
		for(int i=0; i<nX; i++)
		{
			glVertex3f(i*lX, 0.0, 0.0);
			glVertex3f(i*lX, yLength, 0.0);
		}
		// X方向线
		for(int i=0; i<nY; i++)
		{
			glVertex3f(0.0, i*lY, 0.0);
			glVertex3f(xLength, i*lY, 0.0);
		}
	glEnd();

	drawCoordinate((lX+lY)/2.0);
}

void SceneRender::mousePressEvent(QMouseEvent *e)
{
	_mx = e->pos().x();
	_my = e->pos().y();
	if (Qt::LeftButton == e->button())
		m_bRotating = true;

	update();
	QGLWidget::mousePressEvent(e);
}
void SceneRender::mouseReleaseEvent(QMouseEvent *e)
{
	if (Qt::LeftButton == e->button())
		m_bRotating = false;
	update();
	QGLWidget::mouseReleaseEvent(e);
}
void SceneRender::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bRotating) {
		GLint Dx = e->pos().x()-_mx;
		GLint Dy = e->pos().y()-_my;

		if (e->pos().y()<rect().height()/2.0)
			Dx = -Dx;

		double oldAngleX = m_angleX;	
		if ((oldAngleX>0 && oldAngleX<180) ||
			(oldAngleX<0 && oldAngleX<-180))
			m_angleY -= Dx*2.0;
		else
			m_angleY += Dx*2.0;
		if (m_angleY>360) m_angleY -= 360;
		if (m_angleY<-360) m_angleY += 360;

		m_angleX += Dy*2.0;
		if (m_angleX>360) m_angleX -= 360;
		if (m_angleX<-360) m_angleX += 360;

		_mx = e->pos().x();
		_my = e->pos().y();
	}
	
	update();
	QGLWidget::mouseMoveEvent(e);
}
void SceneRender::wheelEvent(QWheelEvent *e)
{
#if 0	// 调节视场角缩放,纯缩放
	m_fov += e->delta()/120.0;
	if (m_fov<=0.0)
		m_fov = 1.0;
	else if (m_fov>90.0)
		m_fov = 90.0;
#else	// 调节距离缩放,存在透视变化
	m_dist += e->delta()/24.0;
#endif

	update();

	QGLWidget::wheelEvent(e);
}