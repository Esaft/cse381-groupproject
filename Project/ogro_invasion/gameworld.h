#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <iomanip>
#include <list>
#include <string>
#include <sstream>
#include <memory>

#include "uncopyable.h"
#include "enemy.h"
#include "tree.h"
#include "TreeLog.h"
#include "physics.h"

class KeyboardInterface;
class MouseInterface;

class Camera;
class Player;
class Landscape;
class Frustum;
class Physics;

class GameWorld : private Uncopyable
{
	 private:
		Physics* physics;

        std::list<Entity*> m_entities; //!< Member variable "m_enemies"
        std::list<Collider*> m_colliders;

        void registerCollider(Collider* collider);
        void unregisterCollider(const Collider* collider);

        void registerEntity(Entity* entity);
        void unregisterEntity(const Entity* entity);

        static const unsigned int MAX_ENEMY_COUNT = 7;
        static const int TREE_COUNT = 50;

        Player* m_player;
        Landscape* m_landscape;
		OctreeNode	*m_pOctreeRoot;	

        Vector3 getRandomPosition() const;
		Vector3 getRandomPositionR(float maxRadius, float minRadius);

        void clearDeadEntities();

        std::auto_ptr<Camera> m_gameCamera;

        KeyboardInterface* m_keyboard;
        MouseInterface* m_mouse;

        float m_lastSpawn;
        float m_currentTime;

        float m_remainingTime;

        float m_relX, m_relY;

		int numSentToFrustum;
		int numRendered;
		bool gameLost;

        std::auto_ptr<Frustum> m_frustum;

		int timeOfLastTreeHit;
		Tree* currentTree;
		std::list<Vector3> logsToSpawn;

    public:
        /** Default constructor */
        GameWorld(KeyboardInterface* keyboardInterface, MouseInterface* mouseInterface);
        /** Default destructor */
        virtual ~GameWorld();

        Entity* spawnEntity(EntityType entity, Vector3);

		void playerHit(Entity*);
		Tree* getTreeToShow();

        bool initialize();
        void update(float dt);
        void render() const;

        typedef std::list<Entity*>::iterator EntityIterator;
        typedef std::list<Entity*>::const_iterator ConstEntityIterator;

        typedef std::list<Collider*>::iterator ColliderIterator;
        typedef std::list<Collider*>::const_iterator ConstColliderIterator;

        Player* getPlayer() { return m_player; }
        Landscape* getLandscape() const { return m_landscape; }

        KeyboardInterface* getKeyboard() { return m_keyboard; }
        MouseInterface* getMouse() { return m_mouse; }

        unsigned int getOgroCount()
        {
            unsigned int count = 0;
            for (EntityIterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                if ((*it)->getType() == OGRO) {
                    if (!(dynamic_cast<Enemy*>((*it))->isDead()))
                    {
                        ++count;
                    }
                }
            }

            return count;
        }

        Enemy* findDeadEnemy()
        { // Deactivate Enemy Resuse (Bugged)
            /*for (EntityIterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                if ((*it)->getType() == OGRO) {
                    Enemy* ogro = dynamic_cast<Enemy*>((*it));
                    if (ogro->isDead())
                    {
                        return ogro;
                    }
                }

            }*/

            return NULL;
        }

        std::string getRemainingTimeAsString()
        {
            float time = m_remainingTime / 60.0f;
            int minutes = int(time);
            float rem = time - float(minutes);
            int seconds = int(rem * 60.0f);

            std::stringstream t;
            t << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
            return t.str();
        }

        float getRemainingTime()
        {
            return m_remainingTime;
        }

        std::string getSpawnMessage()
        {
            if (m_currentTime - m_lastSpawn < 3.0f)
            {
                return "New enemy detected";
            }

            return "";
        }

        void getRelativeMousePos(float& x, float&y)
        {
            x = m_relX;
            y = m_relY;
        }

		int getNumRendered()		{	return numRendered;}
		int getNumSentToFrustum()	{	return numSentToFrustum;}
		int getNumSceneCulled()		{	return m_entities.size() - numSentToFrustum;}
		int getNumEntities()		{	return m_entities.size();}
		bool isGameLost()			{	return gameLost;}
		void loseGame()				{	gameLost = true;}


};

#endif // GAMEWORLD_H
