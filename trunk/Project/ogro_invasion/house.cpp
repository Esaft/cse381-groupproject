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

GLuint House::m_houseTextureID = 0;
//GLuint House::m_vertexBuffer = 0;
//GLuint House::m_texCoordBuffer = 0;
std::auto_ptr<GLSLProgram> House::m_shaderProgram;

const string HOUSE_MODEL = "data/models/Ogro/tris.md2";
const string HOUSE_TEXTURE = "data/models/Ogro/Ogrobase.tga";

House::House(GameWorld* const world):
Entity(world)
{
    m_collider = new SphereCollider(this, 0.75f);
	string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
    string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";
    m_model = new MD2Model(vertexShader, fragmentShader);
    m_model->setAnimation(Animation::IDLE);
}

House::~House()
{
    delete m_collider;
}

void House::onPrepare(float dT)
{

}

//void House::initializeVertexBuffer()
//{
//const float radius = 0.5f;
//    //Push back 8 vertices that make up a cube
//    m_vertices.push_back(Vertex(-radius, -radius,  radius));//left bottom front 0
//    m_vertices.push_back(Vertex(-radius, -radius, -radius));//left bottom back 1
//    m_vertices.push_back(Vertex(radius, -radius, -radius));//right bottom back 2
//    m_vertices.push_back(Vertex(radius, -radius,  radius));//right bottom front 3
//
//    m_vertices.push_back(Vertex(-radius, radius,  radius));//left top front 4
//    m_vertices.push_back(Vertex(-radius, radius, -radius));//left top back 5 
//    m_vertices.push_back(Vertex( radius, radius, -radius));//right top back 6 
//    m_vertices.push_back(Vertex( radius, radius,  radius));//right top front 7
//
//	m_vertices.push_back(Vertex(-radius, -radius,  radius));//left bottom front  8(0)
//	m_vertices.push_back(Vertex(-radius, -radius, -radius));//left bottom back 9(1)
//	m_vertices.push_back(Vertex(-radius, radius,  radius));//left top front 10     (4)
//    m_vertices.push_back(Vertex(-radius, radius, -radius));//left top back 11    (5)
//	m_vertices.push_back(Vertex( radius, radius, -radius));//right top back 12    (6)
//	m_vertices.push_back(Vertex(radius, -radius, -radius));//right bottom back 13  (2)
//
//    //Push back the indices that make up the triangles for each face.
//	
//	//bottom
//    m_indices.push_back(0);
//    m_indices.push_back(2);
//    m_indices.push_back(3);
//    m_indices.push_back(0);
//    m_indices.push_back(1);
//    m_indices.push_back(2);
//	
//	//top
//    m_indices.push_back(4);
//    m_indices.push_back(7);
//    m_indices.push_back(6);
//    m_indices.push_back(4);
//    m_indices.push_back(6);
//    m_indices.push_back(5);
//
//	//left
//    m_indices.push_back(8);
//    m_indices.push_back(10);
//    m_indices.push_back(9);
//    m_indices.push_back(10);
//    m_indices.push_back(11);
//    m_indices.push_back(9);
//
//	//right
//    m_indices.push_back(3);
//    m_indices.push_back(13);
//    m_indices.push_back(12);
//    m_indices.push_back(12);
//    m_indices.push_back(7);
//    m_indices.push_back(3);
//
//	//front
//    m_indices.push_back(0);
//    m_indices.push_back(3);
//    m_indices.push_back(7);
//    m_indices.push_back(0);
//    m_indices.push_back(7);
//    m_indices.push_back(4);
//
//	//back
//    m_indices.push_back(9);
//    m_indices.push_back(11);
//    m_indices.push_back(12);
//    m_indices.push_back(9);
//    m_indices.push_back(12);
//    m_indices.push_back(13);
//    
//
//    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VERTEX_BUFFER]); //Bind the vertex buffer
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[INDEX_BUFFER]); //Bind the vertex buffer
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW); //Send the data to OpenGL
//}

//bool House::initializeTexture()
//{
//	if(m_houseTexID == 0)
//    {
//		TargaImage houseTexture;
//		if (!houseTexture.load(HOUSE_TEXTURE))
//		{
//			std::cerr << "Couldn't load the house texture" << std::endl;
//			return false;
//		}
//
//		glGenTextures(1, &m_houseTexID);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, m_houseTexID);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//		//We load the texture with an alpha channel (RGBA)
//        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, houseTexture.getWidth(),
//                      houseTexture.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
//                      houseTexture.getImageData());
//
//		m_texCoordBuffer.push_back(TexCoord(0.0f,   0.33f));
//		m_texCoordBuffer.push_back(TexCoord(0.0f,   0.0f));
//		m_texCoordBuffer.push_back(TexCoord(0.25f,  0.0f));
//		m_texCoordBuffer.push_back(TexCoord(0.25f,  0.33f));
//		m_texCoordBuffer.push_back(TexCoord(0.0f,   0.66f));
//		m_texCoordBuffer.push_back(TexCoord(0.0f,   1.0f));
//		m_texCoordBuffer.push_back(TexCoord(0.25f,  1.0f));
//		m_texCoordBuffer.push_back(TexCoord(0.25f,  0.66f));
//		m_texCoordBuffer.push_back(TexCoord(1.0f,  0.33f));
//		m_texCoordBuffer.push_back(TexCoord(0.75f,   0.33f));
//		m_texCoordBuffer.push_back(TexCoord(1.0f,  0.66f));
//		m_texCoordBuffer.push_back(TexCoord(0.75f,   0.66f));
//		m_texCoordBuffer.push_back(TexCoord(0.5f,   0.66f));
//		m_texCoordBuffer.push_back(TexCoord(0.5f,   0.33f));
//	
//		glBindBuffer(GL_ARRAY_BUFFER,  m_vbos[TEXTURE_BUFFER]); //Bind the vertex buffer
//		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_texCoordBuffer.size() * 2, &m_texCoordBuffer[0], GL_STATIC_DRAW); //Send the data to OpenGL
//	}
//	return true;
//	
//}

void House::onRender() const
{
    /*static float modelviewMatrix[16];
    static float projectionMatrix[16];

    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);

    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    m_shaderProgram->bindShader();
    m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);

    glBindTexture(GL_TEXTURE_2D, m_houseTexID);

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

    glPopMatrix();*/

	//static float modelviewMatrix[16];
 //   static float projectionMatrix[16];

	//glPushMatrix();
	//glTranslatef(m_position.x, m_position.y, m_position.z);
	////glScalef(2.0f, 2.0f, 2.0f);

	//glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
 //   glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

	//m_shaderProgram->bindShader();
 //   m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
 //   m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);
	//m_shaderProgram->sendUniform("texture0", 0);

	//glBindTexture(GL_TEXTURE_2D, m_houseTexID);
 //   glEnableVertexAttribArray(0);
 //   glEnableVertexAttribArray(1);

 //   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[INDEX_BUFFER]); //Bind the vertex buffer

 //   glBindBuffer(GL_ARRAY_BUFFER, m_vbos[VERTEX_BUFFER]);
 //   glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, m_vbos[TEXTURE_BUFFER]); //is already bound!
	//glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//
 //   glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	//glDisableVertexAttribArray(1);
 //   glDisableVertexAttribArray(0);

 //   glPopMatrix();


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
		glMultMatrixf(modelviewMatrix);
        Vector3 pos = getPosition();
        //glTranslatef(pos.x, pos.y, pos.z); // Pos y-1
        glRotatef(getYaw(), 0.0f, -1.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, m_houseTextureID);
        m_model->render();
    glPopMatrix();
}

void House::onPostRender()
{

}

void House::initializeVBOs()
{
    //GLfloat vertex [] = {
    //    -1.0f, -1.0f, 0.0f, //First Square vertex
    //     1.0f, -1.0f, 0.0f,
    //    -1.0f, 1.0f, 0.0f,
    //     1.0f, 1.0f, 0.0f,  //Last Square vertex
    //     0.0f, -1.0f, 1.0f, //First Square vertex
    //     0.0f, -1.0f,-1.0f,
    //     0.0f, 1.0f, 1.0f,  //Last Square vertex
    //     0.0f, 1.0f,-1.0f,

    //};

    //GLfloat texCoord [] = {
    //    0.0f, 0.0f,
    //    1.0f, 0.0f,
    //    0.0f, 1.0f,
    //    1.0f, 1.0f,
    //    0.0f, 0.0f,
    //    1.0f, 0.0f,
    //    0.0f, 1.0f,
    //    1.0f, 1.0f,
    //};

    //glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    //glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 3, &vertex[0], GL_STATIC_DRAW); //Send the data to OpenGL

    //glGenBuffers(1, &m_texCoordBuffer); //Generate a buffer for the vertices
    //glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the vertex buffer
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 2, &texCoord[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

bool House::onInitialize()
{
    /*if(m_houseTexID == 0)
    {
        
        initializeVertexBuffer();
		initializeTexture();

        string vertexShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.vert" : "data/shaders/glsl1.20/model.vert";
		string fragmentShader = (GLSLProgram::glsl130Supported())? "data/shaders/glsl1.30/model.frag" : "data/shaders/glsl1.20/model.frag";

        m_shaderProgram = std::auto_ptr<GLSLProgram>(new GLSLProgram(vertexShader, fragmentShader));

        if (!m_shaderProgram->initialize())
        {
            std::cerr << "Could not initialize the house shaders" << std::endl;
            return false;
        }

        m_shaderProgram->bindAttrib(0, "a_Vertex");
        m_shaderProgram->bindAttrib(1, "a_TexCoord");
        m_shaderProgram->linkProgram();

    }

    return true;*/

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
