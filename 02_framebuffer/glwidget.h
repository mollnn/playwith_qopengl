#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QImage>
#include <QOpenGLFramebufferObject>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>

class GlWidget : public QOpenGLWidget,public QOpenGLExtraFunctions
{
    Q_OBJECT

private:
    QVector<float> vertices;
    QOpenGLShaderProgram m_shaderTextureShader;
    QOpenGLBuffer m_vboVertexBufferObject;
    QOpenGLVertexArrayObject m_vaoVertexArrayObject;
    QOpenGLTexture *pTextureOld;
    QOpenGLTexture *pTextureResult;
    QOpenGLTexture *pTextureSource;
    GLuint fboSource;
    GLuint fboOld;
    GLuint fboResult;
    QOpenGLFunctions glFunctions;
    int width,height;

    void CopyFromFramebufferToTexture(GLuint fboSrc, GLuint texDest, int xo, int yo, int x, int y, int w, int h);

public:
    GlWidget(QWidget *parent = 0);
    ~GlWidget();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w,int h) override;
    virtual void paintGL() override;
};

#endif // WIDGET_H
