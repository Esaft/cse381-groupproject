#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <iostream>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "house.h"
#include "targa.h"
#include "glslshader.h"
#include "spherecollider.h"
#include "md2model.h"
#include "btBulletDynamicsCommon.h"

using std::string;

const string HOUSE_MODEL = "data/models/House/house.md2";
const string HOUSE_TEXTURE = "data/models/House/house.tga";

House::House(GameWorld* const world):
Entity(world)
{
    m_collider = new SphereCollider(this, 4.0f);
	string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
    string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";
    m_model = new MD2Model(vertexShader, fragmentShader);
    //m_model->setAnimation(Animation::IDLE);
}

House::~House()
{
    delete m_collider;
}

void House::onPrepare(float dT)
{

}


void House::onRender() const
{
    glPushMatrix();
		//glMultMatrixf(modelviewMatrix);
        Vector3 pos = getPosition();
        glTranslatef(pos.x, pos.y, pos.z); // Pos y-1
        //glRotatef(getYaw(), 0.0f, -1.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, m_houseTextureID);
        m_model->render();
    glPopMatrix();
}

void House::onPostRender()
{

}

void House::initializeVBOs()
{
}

bool House::onInitialize()
{

	bool result = m_model->load(HOUSE_MODEL);
    if (result)
    {
        if (!m_houseTexture.load(HOUSE_TEXTURE))
        {
            result = false;
        }
        else
        {
            glGenTextures(1, &m_houseTextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_houseTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_houseTexture.getWidth(),
                              m_houseTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                              m_houseTexture.getImageData());
        }
    }

    return result;
}

void House::onShutdown()
{

}
