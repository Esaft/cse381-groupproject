#include "player.h"
#include "gameworld.h"
#include "keyboardinterface.h"
#include "mouseinterface.h"
#include "spherecollider.h"
#include "landscape.h"
#include "btBulletDynamicsCommon.h"

Player::Player(GameWorld* const world):
Entity(world),
m_score(0),
m_position(Vector3()),
m_velocity(Vector3()),
m_yaw(0.0f),
m_pitch(0.0f)
{
    m_collider = new SphereCollider(this, 0.75f);
	hitNextTree = false;
}

Player::~Player()
{
    delete m_collider;
}

void Player::onPrepare(float dT)
{
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_UP) || getWorld()->getKeyboard()->isKeyHeldDown(KC_w))
    {
        moveForward(2.0f * dT);
    }
    else if (getWorld()->getKeyboard()->isKeyHeldDown(KC_DOWN) || getWorld()->getKeyboard()->isKeyHeldDown(KC_s))
    {
        moveForward(-2.0f * dT);
    } 
	else if (getWorld()->getKeyboard()->isKeyHeldDown(KC_d))
    {
        moveLeft(-2.0f * dT);
    }
    else if (getWorld()->getKeyboard()->isKeyHeldDown(KC_a))
    {
		moveLeft(2.0f * dT);
    }else {
		// Not Moving
		moveForward(0);
	}
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_RIGHT))
    {
        yaw(50.0f * dT);
    }
    if (getWorld()->getKeyboard()->isKeyHeldDown(KC_LEFT))
    {
        yaw(-50.0f * dT);
    }

	if (getWorld()->getKeyboard()->isKeyPressed(KC_SPACE) ||
        getWorld()->getMouse()->isButtonPressed(0))
    {
		hitTree();
    }

    float x, y;
    getWorld()->getRelativeMousePos(x, y);

    yaw(float(x) * 40.0f * dT);
    pitch(float(y)* -40.0f * dT);

    //m_position.y -= 8.0f * dT;

    float minX = getWorld()->getLandscape()->getTerrain()->getMinX() + 2.5f;
    float maxX = getWorld()->getLandscape()->getTerrain()->getMaxX() - 2.5f;
    float minZ = getWorld()->getLandscape()->getTerrain()->getMinZ() + 2.5f;
    float maxZ = getWorld()->getLandscape()->getTerrain()->getMaxZ() - 2.5f;

    if (m_position.x < minX) m_position.x = minX;
    if (m_position.x > maxX) m_position.x = maxX;
    if (m_position.z < minZ) m_position.z = minZ;
    if (m_position.z > maxZ) m_position.z = maxZ;
}

void Player::hitTree()
{
	//getWorld()->hitTreeAtVector(m_position, m_yaw);
	hitNextTree = true;
}

void Player::onRender() const
{

}

void Player::onCollision(Entity* en)
{
	if (en->getType() == TREE && hitNextTree == true) {

		getWorld()->playerHit(en);
		hitNextTree = false;
		
	}
}

void Player::onPostRender()
{
	hitNextTree = false;
}

bool Player::onInitialize()
{

    return true;
}

void Player::onShutdown()
{

}

void Player::yaw(const float val)
{
    m_yaw += val;

    if (m_yaw >= 360.0f) m_yaw -= 360.0f;
    if (m_yaw < 0.0f) m_yaw += 360.0f;
}

void Player::pitch(const float val)
{
    m_pitch += val;

    const float PITCH_LIMIT = 45.0f;

    if (m_pitch >= PITCH_LIMIT)
    {
        m_pitch = PITCH_LIMIT;
    }

    if (m_pitch <= -PITCH_LIMIT)
    {
        m_pitch = -PITCH_LIMIT;
    }

}

void Player::moveForward(const float speed)
{
    //Vector3 pos = getPosition();

    float cosYaw = cosf(degreesToRadians(m_yaw));
    float sinYaw = sinf(degreesToRadians(m_yaw));
    //pos.x += float(cosYaw)*speed;
    //pos.z += float(sinYaw)*speed;

	btVector3 linearVelocity = getCollider()->getBody()->getLinearVelocity();

	getCollider()->getBody()->setLinearVelocity(btVector3(btScalar(float(cosYaw)*speed*20.), linearVelocity.getY(), btScalar(float(sinYaw)*speed*20.)));

    //setPosition(pos);
}

void Player::moveLeft(const float speed)
{

	const float speed_multiplier = 20;

    //Vector3 pos = getPosition();

    float cosYaw = cosf(degreesToRadians(m_yaw - 90));
    float sinYaw = sinf(degreesToRadians(m_yaw - 90));
    //pos.x += float(cosYaw)*speed;
    //pos.z += float(sinYaw)*speed;

	btVector3 linearVelocity = getCollider()->getBody()->getLinearVelocity();

	getCollider()->getBody()->setLinearVelocity(btVector3(btScalar(float(cosYaw)*speed*speed_multiplier), linearVelocity.getY(), btScalar(float(sinYaw)*speed*speed_multiplier)));

    //setPosition(pos);
}


void Player::setPosition(const Vector3& pos) {
    m_position = pos;
}