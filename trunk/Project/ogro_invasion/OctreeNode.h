#ifndef OCTREENODE_H
#define OCTREENODE_H


#include <list>
#include <vector>
#include "geom.h"
#include "frustum.h"

class Entity;

enum OC3NODEID
{
	ROOT				= -1,
	TOP_LEFT_FRONT		=  0,
	BOTTOM_LEFT_FRONT	=  1,
	BOTTOM_RIGHT_FRONT	=  2,
	TOP_RIGHT_FRONT		=  3,
	TOP_LEFT_BACK		=  4,
	BOTTOM_LEFT_BACK	=  5,
	BOTTOM_RIGHT_BACK	=  6,
	TOP_RIGHT_BACK		=  7
};

class OctreeNode
{

public:
	OctreeNode(){}
	~OctreeNode(){}

	bool	InitAll(OC3NODEID id);						// Init Node
	void	DestroyAll();								// Cleanup

	void	SceneCull(std::list<Entity*> *visibleEntities, Frustum *f);

	void	Get_CenterOfNextNode(Vector3 currCenter,		// Center of current node
								 Vector3 *newCenter,		// New center
								 float width,			// Width
								 OC3NODEID nID);		// ID of the new node

	bool	SubdivideNode8(	std::vector<Entity*> *entities,				//entities of parent node
							OctreeNode *parent,			// Pointer to parent
							//int *nodesCreated,			// Number of nodes created
							int maxEntities				// Max triangles allowed in a node
							);

	bool	CreateSubNode(	std::vector<Entity*> *entities,				// entities of parent node
							//int *nodesCreated,			// Number of nodes created
							OC3NODEID id,				// ID of this sub node to be created
							int maxEntities,			// Max triangles allowed in a node
							std::vector<int> *entityAssignments	//indexs of entities to be assigned
							);	

	void	addEntity(Entity *e);
	void	removeEntity(Entity *e);
	void	addEntityFromChild(Entity *e);


	void			Set_ID(OC3NODEID id)		{			m_ID = id;			}
	OC3NODEID		Get_ID()					{	return	m_ID;				}
	int				Get_entityCount()			{	return	m_numEntities;		}	
	void			Set_center(Vector3 c)		{			m_center = c;		}
	Vector3			Get_center()				{	return	m_center;			}
	void			Set_size(float s)			{			m_size = s;			}
	float			Get_size()					{	return	m_size;				}
	bool			IsLeafNode()				{	return	m_isLeafNode;		}
	//void			Get_AABB(GLBoundingBox *aabb){			*aabb = m_AABB;		}


private:

	std::vector<Entity*> m_entities; //list of all entities in node
	OC3NODEID		m_ID;								// NodeID, ie. TOPLEFT, BOTTOMLEFT..., 
	OctreeNode		*m_pSubNodes[8], *m_pParent;		// Pointers to children and parent	
	
	bool m_isLeafNode;
	Vector3	m_center;							// Center position of the node
	float	m_size;								// Size of the node
	unsigned short	m_numEntities;				// Number of indices inside this node
	int		m_maxEntities;
	//GLBoundingBox	m_AABB;								// Bounding Box of the node

protected:
	std::vector<Entity*>* getEntities()	{return &m_entities;}
};

#endif