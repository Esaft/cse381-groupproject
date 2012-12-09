#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <iostream>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "tree.h"
#include "targa.h"
#include "glslshader.h"
#include "spherecollider.h"

using std::string;

//const string TREE_TEXTURE = "data/textures/beech.tga";

const string VERTEX_SHADER_120 = "data/shaders/glsl1.20/alpha_test.vert";
const string VERTEX_SHADER_130 = "data/shaders/glsl1.30/alpha_test.vert";

const string FRAGMENT_SHADER_120 = "data/shaders/glsl1.20/alpha_test.frag";
const string FRAGMENT_SHADER_130 = "data/shaders/glsl1.30/alpha_test.frag";

const string TREE_MODEL = "data/models/Tree/tree.md2";
const string TREE_TEXTURE = "data/models/Log/plain.tga";//"data/models/Tree/tree_tex.tga";

const string LEAF_MODEL = "data/models/Tree/leaf.md2";
const string LEAF_TEXTURE = "data/models/Tree/leaf_tex.tga";

Tree::Tree(GameWorld* const world):
Entity(world)
{
    m_collider = new SphereCollider(this, 0.75f);
	hp = 100;
	string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
    string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";

    m_collider = new SphereCollider(this, 0.75f);
	m_model = new MD2Model(vertexShader, fragmentShader);
	m_leafModel = new MD2Model(vertexShader, fragmentShader);
}

Tree::~Tree()
{
    delete m_collider;
}

void Tree::onPrepare(float dT)
{

}

void Tree::onRender() const
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);

	glBindTexture(GL_TEXTURE_2D, m_treeTextureID);
    m_model->render();
	
	glBindTexture(GL_TEXTURE_2D, m_leafTextureID);
	m_leafModel->render();

    glPopMatrix();
}

void Tree::onPostRender()
{

}

void Tree::initializeVBOs()
{
}

bool Tree::onInitialize()
{

	bool result = m_model->load(TREE_MODEL);
    if (result)
    {
        if (!m_treeTexture.load(TREE_TEXTURE))
        {
            result = false;
        }
        else
        {
			glGenTextures(1, &m_treeTextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_treeTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_treeTexture.getWidth(),
                              m_treeTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                              m_treeTexture.getImageData());
        }
    }

	result = result && m_leafModel->load(LEAF_MODEL);
    if (result)
    {
        if (!m_leafTexture.load(LEAF_TEXTURE))
        {
            result = false;
        }
        else
        {
			glGenTextures(1, &m_leafTextureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_leafTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_leafTexture.getWidth(),
                              m_leafTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                              m_leafTexture.getImageData());
        }
    }

    return result;
}

void Tree::onShutdown()
{

}
