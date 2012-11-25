#ifndef TREELOG_H_INCLUDED
#define TREELOG_H_INCLUDED

#include <memory>
#include "glee/GLee.h"
#include "entity.h"

class GLSLProgram;

class TreeLog : public Entity
{
public:
    TreeLog(GameWorld* const world);
    virtual ~TreeLog();

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

    EntityType getType() const { return LOG; }

    virtual void onCollision(Entity* collider) { }
private:
    static GLuint m_treeTexID;
    static GLuint m_vertexBuffer;
    static GLuint m_texCoordBuffer;
    static std::auto_ptr<GLSLProgram> m_shaderProgram;

    void initializeVBOs();

    Vector3 m_position;
    Collider* m_collider;
};

#endif // TREELOG_H_INCLUDED
