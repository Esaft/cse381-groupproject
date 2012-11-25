#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <iostream>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "TreeLog.h"
#include "targa.h"
#include "glslshader.h"
#include "spherecollider.h"
#include "collider.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

using std::string;

GLuint TreeLog::m_treeTexID = 0;
GLuint TreeLog::m_vertexBuffer = 0;
GLuint TreeLog::m_texCoordBuffer = 0;
std::auto_ptr<GLSLProgram> TreeLog::m_shaderProgram;

const string TREE_TEXTURE = "data/textures/beech.tga";

const string VERTEX_SHADER_120 = "data/shaders/glsl1.20/alpha_test.vert";
const string VERTEX_SHADER_130 = "data/shaders/glsl1.30/alpha_test.vert";

const string FRAGMENT_SHADER_120 = "data/shaders/glsl1.20/alpha_test.frag";
const string FRAGMENT_SHADER_130 = "data/shaders/glsl1.30/alpha_test.frag";

TreeLog::TreeLog(GameWorld* const world):
Entity(world)
{
    m_collider = new SphereCollider(this, 0.75f);
}

TreeLog::~TreeLog()
{
    delete m_collider;
}

void TreeLog::onPrepare(float dT)
{

}

void TreeLog::onRender() const
{

	
	/*
	btTransform t;
	float m[16];
	
	t.getOpenGLMatrix(m);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); 
	glMultMatrixf(m);
	// drawCube
	glPopMatrix();*/

    static float modelviewMatrix[16];
    static float projectionMatrix[16];

	btTransform t;
	btRigidBody* body = m_collider->getBody();
	if (body->getMotionState() != NULL)
		body->getMotionState()->getWorldTransform(t);
	else
		t = body->getWorldTransform();

	t.getOpenGLMatrix(modelviewMatrix);

	//glMultMatrixf(modelviewMatrix);

    glPushMatrix();
    //glTranslatef(m_position.x, m_position.y, m_position.z);
	glMultMatrixf(modelviewMatrix);

    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

	
	

    m_shaderProgram->bindShader();
    m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);

    glBindTexture(GL_TEXTURE_2D, m_treeTexID);

    glDisable(GL_CULL_FACE);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void TreeLog::onPostRender()
{

}

void TreeLog::initializeVBOs()
{
    GLfloat vertex [] = {
        -1.0f, -1.0f, 0.0f, //First Square vertex
         1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 0.0f,  //Last Square vertex
         0.0f, -1.0f, 1.0f, //First Square vertex
         0.0f, -1.0f,-1.0f,
         0.0f, 1.0f, 1.0f,  //Last Square vertex
         0.0f, 1.0f,-1.0f,

    };

    GLfloat texCoord [] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 3, &vertex[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_texCoordBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 2, &texCoord[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

bool TreeLog::onInitialize()
{
    if(m_treeTexID == 0)
    {
        TargaImage treeTexture;
        if (!treeTexture.load(TREE_TEXTURE))
        {
            std::cerr << "Couldn't load the tree texture" << std::endl;
            return false;
        }

        initializeVBOs();

        const string vertexShader = (GLSLProgram::glsl130Supported()) ? VERTEX_SHADER_130 : VERTEX_SHADER_120;
        const string fragmentShader = (GLSLProgram::glsl130Supported()) ? FRAGMENT_SHADER_130 : FRAGMENT_SHADER_120;

        m_shaderProgram = std::auto_ptr<GLSLProgram>(new GLSLProgram(vertexShader, fragmentShader));

        if (!m_shaderProgram->initialize())
        {
            std::cerr << "Could not initialize the tree shaders" << std::endl;
            return false;
        }

        m_shaderProgram->bindAttrib(0, "a_Vertex");
        m_shaderProgram->bindAttrib(1, "a_TexCoord");
        m_shaderProgram->linkProgram();

        glGenTextures(1, &m_treeTexID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_treeTexID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        //We load the texture with an alpha channel (RGBA)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, treeTexture.getWidth(),
                      treeTexture.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
                      treeTexture.getImageData());
    }

    return true;

}

void TreeLog::onShutdown()
{

}
