#ifndef OGRO_H_INCLUDED
#define OGRO_H_INCLUDED

#include "enemy.h"
#include "targa.h"

class MD2Model;


enum AIState {
    OGRO_IDLE,
    OGRO_RUNNING,
    OGRO_CROUCH,
    OGRO_WALK,
    OGRO_DEAD
};

class Ogro : public Enemy
{
    public:
        Ogro(GameWorld* world);
        virtual ~Ogro();

        virtual EntityType getType() const { return OGRO; }

    private:
        virtual void onPrepare(float dT);
        virtual void onRender() const;
        virtual void onPostRender();
        virtual bool onInitialize();
        virtual void onShutdown();
		void onRevive();

        MD2Model* m_model;
       // TargaImage m_ogroTexture;
        //unsigned int m_ogroTextureID;

        void processAI();

        AIState m_AIState;
		int deathTime;

        float m_currentTime;
        float m_lastAIChange;

        virtual void onKill();
        virtual void onResurrection();

protected:
	static TargaImage m_ogroTexture;
	static unsigned int m_ogroTextureID;
	static bool textureLoaded;
};

#endif // OGRO_H_INCLUDED
