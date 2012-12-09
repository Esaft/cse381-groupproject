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

const string TREE_TEXTURE = "data/textures/log.tga";

const string VERTEX_SHADER_120 = "data/shaders/glsl1.20/alpha_test.vert";
const string VERTEX_SHADER_130 = "data/shaders/glsl1.30/alpha_test.vert";

const string FRAGMENT_SHADER_120 = "data/shaders/glsl1.20/alpha_test.frag";
const string FRAGMENT_SHADER_130 = "data/shaders/glsl1.30/alpha_test.frag";

const string LOG_MODEL = "data/models/Log/log.md2";
const string LOG_TEXTURE = "data/models/Log/plain.tga";

TreeLog::TreeLog(GameWorld* const world):
Entity(world)
{

	string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
    string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";

    m_collider = new SphereCollider(this, 0.75f);
	m_model = new MD2Model(vertexShader, fragmentShader);

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
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

	btTransform t;
	btRigidBody* body = m_collider->getBody();
	if (body->getMotionState() != NULL)
		body->getMotionState()->getWorldTransform(t);
	else
		t = body->getWorldTransform();

	t.getOpenGLMatrix(modelviewMatrix);

    glPushMatrix();
	glMultMatrixf(modelviewMatrix);

	glBindTexture(GL_TEXTURE_2D, m_logTextureID);
    m_model->render();

    glPopMatrix();
}

void TreeLog::onPostRender()
{

}

void TreeLog::initializeVBOs()
{
}

bool TreeLog::onInitialize()
{

	bool result = m_model->load(LOG_MODEL);
    if (result)
    {
        if (!m_logTexture.load(LOG_TEXTURE))
        {
            result = false;
        }
        else
        {
			glGenTextures(1, &m_logTextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_logTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_logTexture.getWidth(),
                              m_logTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                              m_logTexture.getImageData());
        }
    }

    return result;
}

void TreeLog::onShutdown()
{

}
