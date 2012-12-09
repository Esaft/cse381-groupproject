#ifndef HOUSE_H_INCLUDED
#define HOUSE_H_INCLUDED

#include <memory>
#include "glee/GLee.h"
#include "entity.h"
#include "targa.h"
class MD2Model;
class GLSLProgram;

class House : public Entity
{
public:
	enum Buffers {
        VERTEX_BUFFER = 0,
        INDEX_BUFFER,
        COLOR_BUFFER,
		TEXTURE_BUFFER,
		NORMAL_BUFFER,
        LAST_BUFFER
    };

    House(GameWorld* const world);
    virtual ~House();

    virtual void onPrepare(float dT);
    virtual void onRender() const;
    virtual void onPostRender();
    virtual bool onInitialize();
    virtual void onShutdown();

    void setPosition(const Vector3& v) { m_position = v; }
    Vector3 getPosition() const { return m_position; }

    float getYaw() const { return 0.0f; }
    float getPitch() const { return 0.0f; }

    void setYaw(const float val) { }
    void setPitch(const float val) { }

    Collider* getCollider() { return m_collider; }

    EntityType getType() const { return HOUSE; }

    virtual void onCollision(Entity* collider) { }
private:
    static GLuint m_houseTextureID;
    //static GLuint m_vertexBuffer;
    //static GLuint m_texCoordBuffer;
    static std::auto_ptr<GLSLProgram> m_shaderProgram;

	unsigned int m_vbos[LAST_BUFFER];
	
	MD2Model* m_model;
	TargaImage m_houseTexture;

	//vector<Vertex> m_vertices;
  //  vector<unsigned int> m_indices;
	//vector<TexCoord> m_texCoordBuffer;

    void initializeVBOs();

    Vector3 m_position;
    Collider* m_collider;

	//void initializeVertexBuffer();
	//bool initializeTexture();
};

#endif 