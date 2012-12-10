#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "geom.h"
#include "entitytypes.h"
#include "uncopyable.h"
#include "OctreeNode.h"

class btRigidBody;
class GameWorld;
class Collider;

/**
    The entity is uncopyable because we will mainly be handling
    pointers (which can be copied around) but we want all entities
    to be initialized by the gameworld, so no copying :)
*/
class Entity : private Uncopyable {
    private:
        virtual void onPrepare(float dt) = 0;
        virtual void onRender() const = 0;
        virtual void onPostRender() = 0;
        virtual bool onInitialize() = 0;
        virtual void onShutdown() = 0;
        virtual void onCollision(Entity* collider) = 0;
		virtual void onRevive() = 0;

        bool m_canBeRemoved;

        GameWorld* m_world;
		OctreeNode* m_node;
    public:
        Entity(GameWorld* const gameWorld);
        virtual ~Entity();

        void prepare(float dt);
        void render() const;
        void postRender();
        bool initialize();
        void shutdown();
        bool canBeRemoved() const;
        void destroy();

		virtual bool isFading() { return false; }
		virtual bool isVisible() { return true; }

		void revive() { m_canBeRemoved = false; onRevive(); }

        void collide(Entity* collider);

        virtual Vector3 getPosition() const = 0;
        //virtual Vector3 getVelocity() const = 0;
        virtual void setPosition(const Vector3& position) = 0;
        virtual float getYaw() const = 0;
        virtual float getPitch() const = 0;
        virtual void setYaw(const float yaw) = 0;
        virtual void setPitch(const float pitch) = 0;

        virtual Collider* getCollider() = 0;

        virtual EntityType getType() const = 0;

        GameWorld* getWorld() {
            return m_world;
        }
		OctreeNode* getNode(){
			return m_node;
		}
		void setNode(OctreeNode* n) {m_node = n;} 
};

#endif // ENTITY_H_INCLUDED
