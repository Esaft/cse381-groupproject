#ifndef SKYBOX_H_INCLUDED
#define SKYBOX_H_INCLUDED

#include <memory>
#include "glee/GLee.h"
#include "entity.h"
#include <vector>
using std::vector;

class GLSLProgram;

class SkyBox
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

    /*SkyBox(GameWorld* const world);*/
	SkyBox();
    virtual ~SkyBox();

    /*virtual void onPrepare(float dT);
    virtual void onRender() const;
    virtual void onPostRender();
    virtual bool onInitialize();
    virtual void onShutdown();*/

	void render(Vector3 camera) const;
	bool initialize();

    void setPosition(const Vector3& v) { m_position = v; }
    Vector3 getPosition() const { return m_position; }

    float getYaw() const { return 0.0f; }
    float getPitch() const { return 0.0f; }

    void setYaw(const float val) { }
    void setPitch(const float val) { }

    Collider* getCollider() { return m_collider; }

    EntityType getType() const { return SKYBOX; }

    //virtual void onCollision(Entity* collider) { }
private:
    static GLuint m_skyBoxTexID;
	static std::auto_ptr<GLSLProgram> m_shaderProgram;

    vector<Color> m_colorBuffer;
	vector<TexCoord> m_texCoordBuffer;
	vector<Vertex> m_vertices;
    vector<unsigned int> m_indices;
	

    unsigned int m_vbos[LAST_BUFFER];
    

    void initializeVBOs();

    Vector3 m_position;
    Collider* m_collider;
};

#endif // TREE_H_INCLUDED
