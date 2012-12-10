#ifndef TREELOG_H_INCLUDED
#define TREELOG_H_INCLUDED

#include <memory>
#include "glee/GLee.h"
#include "entity.h"
#include "targa.h"
#include "md2model.h"

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

	bool isFading() { return m_model->isFading(); }
	bool isVisible() { return m_model->isVisible(); }

    void setYaw(const float val) { }
    void setPitch(const float val) { }

    Collider* getCollider() { return m_collider; }

    EntityType getType() const { return LOG; }

	MD2Model* getModel() { return m_model; }

    virtual void onCollision(Entity* collider) { }
private:
	MD2Model* m_model;
    //TargaImage m_logTexture;
    //unsigned int m_logTextureID;

    void initializeVBOs();

    Vector3 m_position;
    Collider* m_collider;

protected:
	static TargaImage m_logTexture;
	static unsigned int m_logTextureID;
	static bool textureLoaded;
};

#endif // TREELOG_H_INCLUDED
