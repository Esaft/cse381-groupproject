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
#include "gameworld.h"
#include "landscape.h"

using std::string;

const string VERTEX_SHADER_120 = "data/shaders/glsl1.20/alpha_test.vert";
const string VERTEX_SHADER_130 = "data/shaders/glsl1.30/alpha_test.vert";

const string FRAGMENT_SHADER_120 = "data/shaders/glsl1.20/alpha_test.frag";
const string FRAGMENT_SHADER_130 = "data/shaders/glsl1.30/alpha_test.frag";

const string LOG_MODEL = "data/models/Log/log_longer.md2";
const string LOG_TEXTURE = "data/models/Log/brown_log_tex.tga";

TargaImage TreeLog::m_logTexture;
unsigned int TreeLog::m_logTextureID = 0;
bool TreeLog::textureLoaded = false;


TreeLog::TreeLog(GameWorld* const world):
Entity(world)
{

	string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
    string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";

    m_collider = new SphereCollider(this, 1.5f);
	m_model = new MD2Model(vertexShader, fragmentShader);

}

TreeLog::~TreeLog()
{
    delete m_collider;
	delete m_model;
}

void TreeLog::onRevive()
{
	m_model->setVisible(true);
}

void TreeLog::onPrepare(float dT)
{
	float minX = getWorld()->getLandscape()->getTerrain()->getMinX();
    float maxX = getWorld()->getLandscape()->getTerrain()->getMaxX();
    float minZ = getWorld()->getLandscape()->getTerrain()->getMinZ();
    float maxZ = getWorld()->getLandscape()->getTerrain()->getMaxZ();

    if (m_position.x < minX) destroy();
    else if (m_position.x > maxX) destroy();
    else if (m_position.z < minZ) destroy();
    else if (m_position.z > maxZ) destroy();

	m_model->updateFade();
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
		if(!textureLoaded)
		{
			if (!m_logTexture.load(LOG_TEXTURE))
			{
				result = false;
			}
			else
			{
				textureLoaded = true;
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
    }

    return result;
}

void TreeLog::onShutdown()
{

}
