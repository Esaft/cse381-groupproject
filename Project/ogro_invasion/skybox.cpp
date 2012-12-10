#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <iostream>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "skyBox.h"
#include "targa.h"
#include "glslshader.h"
#include "spherecollider.h"

using std::string;

GLuint SkyBox::m_skyBoxTexID = 0;
//GLuint SkyBox::m_vertexBuffer = 0;
//GLuint SkyBox::m_texCoordBuffer = 0;
std::auto_ptr<GLSLProgram> SkyBox::m_shaderProgram;

const string SKYBOX_TEXTURE = "data/textures/skyBlah.tga";

const string VERTEX_SHADER_120 = "data/shaders/glsl1.20/alpha_test.vert";
const string VERTEX_SHADER_130 = "data/shaders/glsl1.30/alpha_test.vert";

const string FRAGMENT_SHADER_120 = "data/shaders/glsl1.20/alpha_test.frag";
const string FRAGMENT_SHADER_130 = "data/shaders/glsl1.30/alpha_test.frag";

//SkyBox::SkyBox(GameWorld* const world):
//Entity(world)
//{
//    m_collider = new SphereCollider(this, 0.75f);
//}
SkyBox::SkyBox()
{

}

SkyBox::~SkyBox()
{
}

//void SkyBox::onPrepare(float dT)
//{
//
//}

void SkyBox::render(Vector3 camera) const
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    glPushMatrix();
	//glLoadIdentity();
    glTranslatef(camera.x, camera.y, camera.z);
	/*gluLookAt(
        0,0,0,
		camera.x,camera.y,camera.z,
        0,1,0);*/

    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    m_shaderProgram->bindShader();
    m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);

    

    glDisable(GL_CULL_FACE);

	glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, m_skyBoxTexID);

	//glColor4f(1.0, 1.0, 1.0,1.0f);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[INDEX_BUFFER]); //Bind the vertex buffer

    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VERTEX_BUFFER]);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[TEXTURE_BUFFER]); //is already bound!
	glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

	glPopAttrib();
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

//void SkyBox::onPostRender()
//{
//
//}

void SkyBox::initializeVBOs()
{
	const float radius = 0.5f;
    //Push back 8 vertices that make up a cube
    m_vertices.push_back(Vertex(-radius, -radius,  radius));//left bottom front 0
    m_vertices.push_back(Vertex(-radius, -radius, -radius));//left bottom back 1
    m_vertices.push_back(Vertex(radius, -radius, -radius));//right bottom back 2
    m_vertices.push_back(Vertex(radius, -radius,  radius));//right bottom front 3

    m_vertices.push_back(Vertex(-radius, radius,  radius));//left top front 4
    m_vertices.push_back(Vertex(-radius, radius, -radius));//left top back 5 
    m_vertices.push_back(Vertex( radius, radius, -radius));//right top back 6 
    m_vertices.push_back(Vertex( radius, radius,  radius));//right top front 7

	m_vertices.push_back(Vertex(-radius, radius, -radius));//left top back 8    (5)
	m_vertices.push_back(Vertex(-radius, -radius, -radius));//left bottom back 9(1)
	m_vertices.push_back(Vertex(-radius, -radius, -radius));//left bottom back 10(1)
    m_vertices.push_back(Vertex(-radius, radius, -radius));//left top back 11    (5)
	m_vertices.push_back(Vertex( radius, radius, -radius));//right top back 12    (6)
	m_vertices.push_back(Vertex(radius, -radius, -radius));//right bottom back 13  (2)

    //Push back the indices that make up the triangles for each face.
	
	//bottom
    m_indices.push_back(0);
    m_indices.push_back(3);
    m_indices.push_back(2);
    m_indices.push_back(0);
    m_indices.push_back(2);
    m_indices.push_back(1);
	
	//top
    m_indices.push_back(4);
    m_indices.push_back(6);
    m_indices.push_back(7);
    m_indices.push_back(4);
    m_indices.push_back(5);
    m_indices.push_back(6);

	//left
    m_indices.push_back(9);
    m_indices.push_back(4);
    m_indices.push_back(0);
    m_indices.push_back(9);
    m_indices.push_back(11);
    m_indices.push_back(4);

	//right
    m_indices.push_back(3);
    m_indices.push_back(7);
    m_indices.push_back(12);
    m_indices.push_back(3);
    m_indices.push_back(12);
    m_indices.push_back(13);

	//front
    m_indices.push_back(0);
    m_indices.push_back(7);
    m_indices.push_back(3);
    m_indices.push_back(0);
    m_indices.push_back(4);
    m_indices.push_back(7);

	//back
    m_indices.push_back(13);
    m_indices.push_back(12);
    m_indices.push_back(8);
    m_indices.push_back(13);
    m_indices.push_back(8);
    m_indices.push_back(10);
    

    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VERTEX_BUFFER]); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[INDEX_BUFFER]); //Bind the vertex buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW); //Send the data to OpenGL
    
	float modY = 0.0017f;
	float modX = 0.00125f;
	/*float x0 = 0.0f + modX;
	float x1 = 0.25f + modX;
	float x2 = 0.5f - modX;
	float x3 = 0.75f + modX;
	float x4 = 1.0f - modX;
	float y0 = 0.0f;
	float y1 = 0.33f;
	float y2 = 0.66f - modY;
	float y3 = 1.00f - modY * 2;*/
	float x0 = 0.0f;
	float x1 = 0.25f;
	float x2 = 0.5f;
	float x3 = 0.75f;
	float x4 = 1.0f;
	float y0 = 0.0f;
	float y1 = 0.33f;
	float y2 = 0.66f;
	float y3 = 1.00f;
	
	m_texCoordBuffer.push_back(TexCoord(x1,   y1));
	m_texCoordBuffer.push_back(TexCoord(x1,   y0));
	m_texCoordBuffer.push_back(TexCoord(x2,  y0));
	m_texCoordBuffer.push_back(TexCoord(x2,  y1));
	m_texCoordBuffer.push_back(TexCoord(x1,   y2));
	m_texCoordBuffer.push_back(TexCoord(x1,   y3));
	m_texCoordBuffer.push_back(TexCoord(x2,  y3));
	m_texCoordBuffer.push_back(TexCoord(x2,  y2));
	m_texCoordBuffer.push_back(TexCoord(x4,  y2));
	m_texCoordBuffer.push_back(TexCoord(x0,   y1));
	m_texCoordBuffer.push_back(TexCoord(x4,  y1));
	m_texCoordBuffer.push_back(TexCoord(x0,   y2));
	m_texCoordBuffer.push_back(TexCoord(x3,   y2));
	m_texCoordBuffer.push_back(TexCoord(x3,   y1)); 

    glBindBuffer(GL_ARRAY_BUFFER,  m_vbos[TEXTURE_BUFFER]); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_texCoordBuffer.size() * 2, &m_texCoordBuffer[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

bool SkyBox::initialize()
{
    if(m_skyBoxTexID == 0)
    {
        TargaImage skyBoxTexture;
        if (!skyBoxTexture.load(SKYBOX_TEXTURE))
        {
            std::cerr << "Couldn't load the skyBox texture" << std::endl;
            return false;
        }

		glGenBuffers(LAST_BUFFER, m_vbos);

        initializeVBOs();

        const string vertexShader = (GLSLProgram::glsl130Supported()) ? VERTEX_SHADER_130 : VERTEX_SHADER_120;
        const string fragmentShader = (GLSLProgram::glsl130Supported()) ? FRAGMENT_SHADER_130 : FRAGMENT_SHADER_120;

        m_shaderProgram = std::auto_ptr<GLSLProgram>(new GLSLProgram(vertexShader, fragmentShader));

        if (!m_shaderProgram->initialize())
        {
            std::cerr << "Could not initialize the skyBox shaders" << std::endl;
            return false;
        }

        m_shaderProgram->bindAttrib(0, "a_Vertex");
        m_shaderProgram->bindAttrib(1, "a_TexCoord");
        m_shaderProgram->linkProgram();

        glGenTextures(1, &m_skyBoxTexID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_skyBoxTexID);
        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //We load the texture with an alpha channel (RGBA)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, skyBoxTexture.getWidth(),
                      skyBoxTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                      skyBoxTexture.getImageData());
    }

    return true;

}

//void SkyBox::onShutdown()
//{
//
//}
