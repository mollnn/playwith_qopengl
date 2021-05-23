#include "glwidget.h"

#include <QSurfaceFormat>

GlWidget::GlWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    vertices = {
        // 位置                  //纹理坐标
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // 左下
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // 右上
    };
}

GlWidget::~GlWidget()
{
}

void GlWidget::CopyFromFramebufferToTexture(GLuint fboSrc, GLuint texDest, int xo, int yo, int x, int y, int w, int h)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fboSrc);
    glBindTexture(GL_TEXTURE_2D, texDest);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, xo, yo, x, y, w, h);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlWidget::initializeGL()
{
    this->initializeOpenGLFunctions(); // init opengl

    // vsync
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSwapInterval(10);
    QSurfaceFormat::setDefaultFormat(surfaceFormat);

    // create and load source texture
    pTextureSource = new QOpenGLTexture(QOpenGLTexture::Target2D);
    pTextureSource->create();
    pTextureSource->setData(QImage("texture.jpg"));

    pTextureSource2 = new QOpenGLTexture(QOpenGLTexture::Target2D);
    pTextureSource2->create();
    pTextureSource2->setData(QImage("texture2.jpg"));

    width = QImage("texture.jpg").width();
    height = QImage("texture.jpg").height();

    QImage nullImage(width,height,QImage::Format_RGBA8888);
    nullImage.fill(0);

    // create old buffer texture
    pTextureOld = new QOpenGLTexture(QOpenGLTexture::Target2D);
    pTextureOld->create();
    pTextureOld->setData(nullImage);

    // create result buffer texture
    pTextureResult = new QOpenGLTexture(QOpenGLTexture::Target2D);
    pTextureResult->create();
    pTextureResult->setData(nullImage);

    // create framebuffer for textures

    glGenFramebuffers(1, &fboSource);
    glBindFramebuffer(GL_FRAMEBUFFER, fboSource);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTextureSource->textureId(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &fboSource2);
    glBindFramebuffer(GL_FRAMEBUFFER, fboSource2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTextureSource2->textureId(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &fboOld);
    glBindFramebuffer(GL_FRAMEBUFFER, fboOld);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTextureOld->textureId(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &fboResult);
    glBindFramebuffer(GL_FRAMEBUFFER, fboResult);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTextureResult->textureId(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // create and load shader
    m_shaderTextureShader.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shader.vert");
    m_shaderTextureShader.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shader.frag");
    m_shaderTextureShader.link();

    // create and load vertex data
    QOpenGLVertexArrayObject::Binder{&m_vaoVertexArrayObject};
    m_vboVertexBufferObject.create();
    m_vboVertexBufferObject.bind();
    m_vboVertexBufferObject.allocate(vertices.data(), sizeof(float) * vertices.size());

    // set attribute pointer
    m_shaderTextureShader.setAttributeBuffer("aPos", GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
    m_shaderTextureShader.enableAttributeArray("aPos");
    m_shaderTextureShader.setAttributeBuffer("aTexCoord", GL_FLOAT, sizeof(GLfloat) * 3, 2, sizeof(GLfloat) * 5);
    m_shaderTextureShader.enableAttributeArray("aTexCoord");
}

void GlWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h); //定义视口区域
}

void GlWidget::paintGL()
{
//    qDebug() << "paint GL";
    // clear
    this->glClearColor(0.1, 0.5, 0.7, 1.0);
    this->glClear(GL_COLOR_BUFFER_BIT);

    // bind shader
    m_shaderTextureShader.bind();

    // do texture copy
    int w0 = 10;

    CopyFromFramebufferToTexture(fboResult, pTextureOld->textureId(), 0, 0, 0, 0, width, height);
    CopyFromFramebufferToTexture(fboOld, pTextureResult->textureId(), 0, 0, w0, 0, width - w0, height);
    CopyFromFramebufferToTexture(rand() % 3 ? fboSource: fboSource2, pTextureResult->textureId(), width - w0, 0, 0, 0, w0, height);

    // bind texture and connect to texture unit
    pTextureResult->bind(0);
    m_shaderTextureShader.setUniformValue("ourTexture", 0);

    // bind vao and draw
    QOpenGLVertexArrayObject::Binder{&m_vaoVertexArrayObject};
    this->glDrawArrays(GL_POLYGON, 0, 4);

    this->update();
}
