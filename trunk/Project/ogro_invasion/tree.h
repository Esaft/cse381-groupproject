#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <memory>
#include "glee/GLee.h"
#include "entity.h"
#include "md2model.h"
#include "targa.h"

class GLSLProgram;

class Tree : public Entity
{
public:
    Tree(GameWorld* const world);
    virtual ~Tree();

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

    EntityType getType() const { return TREE; }
	int getHP() { return hp; }
	void cutTree() { destroy(); }
	void damageTree() { hp = hp - 25; }

    virtual void onCollision(Entity* collider) { }
private:
	MD2Model* m_model;
    TargaImage m_treeTexture;
    unsigned int m_treeTextureID;

	MD2Model* m_leafModel;
    TargaImage m_leafTexture;
    unsigned int m_leafTextureID;


	int hp;

    void initializeVBOs();

    Vector3 m_position;
    Collider* m_collider;
};

#endif // TREE_H_INCLUDED
