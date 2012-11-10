#include "OctreeNode.h"
#include "entity.h"
#include <windows.h>
#include <assert.h>
#include <vector>
using std::list;


bool OctreeNode::InitAll(OC3NODEID id)
{
	m_ID		=	id;
	m_pParent	=	NULL;
	m_entities;
	m_numEntities =	0;
	m_size		=	0.0f;
	m_isLeafNode=	false;	
	m_maxEntities = 8;

	//m_AABB.Clear();
	m_center = Vector3(0.0f, 0.0f, 0.0f);
	for(int j=0; j<8; ++j)
		m_pSubNodes[j]=NULL;

	return true;
}

void OctreeNode::addEntity(Entity *e)
{
	if(m_isLeafNode)
	{
		if(e->getNode() != this)
		{
			if(e->getNode() != NULL)
					e->getNode()->removeEntity(e);
			m_entities.push_back(e);
		

			if(m_entities.size() > m_maxEntities)
				SubdivideNode8(&m_entities, this, //nodesCreated,
													m_maxEntities);
			else	
				e->setNode(this);
			
		}
	}
	else
	{
		Vector3 pos = e->getPosition();
		
		//Send Entity to correct subnode

		if(pos.x < m_center.x){
			if(pos.y < m_center.y){
				if(pos.z < m_center.z){
					//Left Bottom Back
					m_pSubNodes[BOTTOM_LEFT_BACK]->addEntity(e);
				}
				else{
					//Left Bottom Front
					m_pSubNodes[BOTTOM_LEFT_FRONT]->addEntity(e);
				}
			}
			else{
				if(pos.z < m_center.z){
					//Left Top Back
					m_pSubNodes[TOP_LEFT_BACK]->addEntity(e);
				}
				else{
					//Left Top Front
					m_pSubNodes[TOP_LEFT_FRONT]->addEntity(e);
				}
			}
		}
		else{
			if(pos.y < m_center.y){
				if(pos.z < m_center.z){
					//Right Bottom Back
					m_pSubNodes[BOTTOM_RIGHT_BACK]->addEntity(e);
				}
				else{
					//Right Bottom Front
					m_pSubNodes[BOTTOM_RIGHT_FRONT]->addEntity(e);
				}
			}
			else{
				if(pos.z < m_center.z){
					//Right Top Back
					m_pSubNodes[TOP_RIGHT_BACK]->addEntity(e);
				}
				else{
					//Right Top Front
					m_pSubNodes[TOP_RIGHT_FRONT]->addEntity(e);
				}
			}
		}
	}
}

void OctreeNode::removeEntity(Entity *e)
{

	Vector3 pos = e->getPosition();

	//For the sake of simplicity, assuming no two entities can have the same position
	int index = -1;
	for(int i = 0; i < m_entities.size(); i ++)
	{
		Vector3 checkPos = m_entities.at(i)->getPosition();
		if(checkPos.x == pos.x && checkPos.y == pos.y && checkPos.z == pos.z)
			index = i;
	}

	if(index >= 0)
	{
		m_entities.erase(m_entities.begin()+ index);

		//if(m_entities.empty());
			//m_parent
	}
}

/* =================================================================================
	CLEANUP
==================================================================================== */
void OctreeNode::DestroyAll()
{
	// IF IT'S A LEAF NODE
	if(m_isLeafNode)
	{
		// CLEANUP INDICES (IF ANY) IN THIS NODE
		if((m_numEntities>0) && (!m_entities.empty()))
		{
			m_entities.clear();
			m_numEntities = 0;
		}
	}
	else
	{
		// CLEAN UP NODES RECURSIVELY		
		for(int j=0; j<8; ++j)
		{
			if(m_pSubNodes[j]!=NULL)
			{
				m_pSubNodes[j]->DestroyAll();
				delete m_pSubNodes[j];
			}
			m_pSubNodes[j]=NULL;
		}

		//ADD CLEAN-UP FOR ENTITIES IN PARENT NODES
	}
}


void OctreeNode::SceneCull(std::list<Entity*> *visibleEntities, Frustum *f)
{
	if(f->CubeInFrustum(m_center.x, m_center.y, m_center.z, m_size))
	{
		if(m_isLeafNode)
		{
			for(int i = 0; i < m_entities.size(); i++)
				visibleEntities->push_back(m_entities.at(i));
		}
		else
		{
			for(int i=0; i<8; i++)
			{
				m_pSubNodes[i]->SceneCull(visibleEntities, f);
			}
		}
	}
}

/* =================================================================================
	CALCULATE THE CENTER OF A NEW NODE
==================================================================================== */
void OctreeNode::Get_CenterOfNextNode(Vector3 currCenter, Vector3 *newCenter,
														  float width, OC3NODEID nextID)
{
	Vector3 tempV;
	float w2 = width/2.0f;	

	(*newCenter) = Vector3(0.0f, 0.0f, 0.0f);	

	if(nextID != ROOT)
	{		
		switch(nextID)
		{
			case TOP_LEFT_FRONT:
				(*newCenter) = Vector3(currCenter.x-w2, currCenter.y+w2, currCenter.z+w2);
				break;
			case BOTTOM_LEFT_FRONT:
				(*newCenter) = Vector3(currCenter.x-w2, currCenter.y-w2, currCenter.z+w2);
				break;
			case BOTTOM_RIGHT_FRONT:
				(*newCenter) = Vector3(currCenter.x+w2, currCenter.y-w2, currCenter.z+w2);
				break;
			case TOP_RIGHT_FRONT:
				(*newCenter) = Vector3(currCenter.x+w2, currCenter.y+w2, currCenter.z+w2);
				break;
			case TOP_LEFT_BACK:
				(*newCenter) = Vector3(currCenter.x-w2, currCenter.y+w2, currCenter.z-w2);
				break;
			case BOTTOM_LEFT_BACK:
				(*newCenter) = Vector3(currCenter.x-w2, currCenter.y-w2, currCenter.z-w2);
				break;
			case BOTTOM_RIGHT_BACK:
				(*newCenter) = Vector3(currCenter.x+w2, currCenter.y-w2, currCenter.z-w2);
				break;
			case TOP_RIGHT_BACK:
				(*newCenter) = Vector3(currCenter.x+w2, currCenter.y+w2, currCenter.z-w2);
				break;
		}
	}
	else
		assert("INVALID OCTREE ID NODE, OctreeNode::Get_CentreOfNextNode()");	
}

/* =================================================================================
	CREATE A NEW NODE
==================================================================================== */
bool OctreeNode::CreateSubNode(std::vector<Entity*> *entities, //int *nodesCreated,
								OC3NODEID id, int maxEntities, std::vector<int> *entityAssignments)
{
	bool nodeCreated = false;

	assert((id>=0)&&(id<=7));

	// CREATE THE NEW NODE AND INITIALIZE IT	
	m_pSubNodes[id] = new OctreeNode;
	assert(m_pSubNodes[id] != NULL && "OctreeNode::CreateSubNode");

	if(m_pSubNodes[id] != NULL)
	{
		m_pSubNodes[id]->InitAll(id);
	
		// SET SIZE AND CENTER
		Vector3 newNodeCenter;
		Get_CenterOfNextNode(m_center, &newNodeCenter, m_size, id);

		m_pSubNodes[id]->Set_size((m_size/2.0f));
		m_pSubNodes[id]->Set_center(newNodeCenter);

		std::vector<Entity*> temp;

		for(int i = 0; i < entityAssignments->size(); i++)
		{
			Entity* e = entities->at(entityAssignments->at(i));
			temp.push_back(e);

			if(e->getNode() != this)
			{
				e->setNode(m_pSubNodes[id]);
			}
		}

		// SUBDIVIDE	
		if(m_pSubNodes[id]->SubdivideNode8(&temp,
										this, //nodesCreated,
										maxEntities))
		{
			nodeCreated = true;
		}

		temp.clear();
	}

	return nodeCreated;
}

/* =================================================================================
	RECURSIVE FUNCTION THAT CREATES 8 NODES...
==================================================================================== */
bool OctreeNode::SubdivideNode8(std::vector<Entity*> *entities, OctreeNode *parent, //int *nodesCreated, 
																								int maxEntities)
{
	
	m_pParent = parent;
	//m_AABB.InitAll(m_center.x, m_center.y, m_center.z, (m_size*2.0f));

	// IF THE INDICES LIST IS NOT EMPTY
	if(entities->size() > 0)
	{	
		// DO WE NEED TO KEEP SUBDIVIDING?
		if( entities->size() > maxEntities)
		{
			// IT WON'T BE A LEAF NODE... SUBDIVIDE...
			m_isLeafNode = false;

			std::vector<int>* entityAssignments[8];
			for(int i = 0; i < 8; i++)
			{
				entityAssignments[i] = new std::vector<int>();
			}

			// ========================================================================
			// ASSIGN EACH ENTITY TO A SUBNODE
			// ========================================================================

			//NOT CHECKING FOR OVERLAPPING YET
			
			for (int i = 0; i < entities->size(); i ++)
			{
				Vector3 pos = entities->at(i)->getPosition();

				if(pos.x < m_center.x){
					if(pos.y < m_center.y){
						if(pos.z < m_center.z){
							//Left Bottom Back
							entityAssignments[BOTTOM_LEFT_BACK]->push_back(i);
						}
						else{
							//Left Bottom Front
							entityAssignments[BOTTOM_LEFT_FRONT]->push_back(i);
						}
					}
					else{
						if(pos.z < m_center.z){
							//Left Top Back
							entityAssignments[TOP_LEFT_BACK]->push_back(i);
						}
						else{
							//Left Top Front
							entityAssignments[TOP_LEFT_FRONT]->push_back(i);
						}
					}
				}
				else{
					if(pos.y < m_center.y){
						if(pos.z < m_center.z){
							//Right Bottom Back
							entityAssignments[BOTTOM_RIGHT_BACK]->push_back(i);
						}
						else{
							//Right Bottom Front
							entityAssignments[BOTTOM_RIGHT_FRONT]->push_back(i);
						}
					}
					else{
						if(pos.z < m_center.z){
							//Right Top Back
							entityAssignments[TOP_RIGHT_BACK]->push_back(i);
						}
						else{
							//Right Top Front
							entityAssignments[TOP_RIGHT_FRONT]->push_back(i);
						}
					}
				}
				
			}//end for

			//(*nodesCreated)   += 8;
	
			// SET UP CHILDREN NODES
			CreateSubNode( entities, //nodesCreated, 
							TOP_LEFT_FRONT, maxEntities, entityAssignments[0]);

			CreateSubNode( entities, //nodesCreated, 
							BOTTOM_LEFT_FRONT, maxEntities, entityAssignments[1]);

			CreateSubNode( entities, //nodesCreated, 
							BOTTOM_RIGHT_FRONT, maxEntities, entityAssignments[2]);

			CreateSubNode( entities, //nodesCreated, 
							TOP_RIGHT_FRONT, maxEntities, entityAssignments[3]);

			CreateSubNode( entities, //nodesCreated, 
							TOP_LEFT_BACK, maxEntities, entityAssignments[4]);

			CreateSubNode( entities, //nodesCreated, 
							BOTTOM_LEFT_BACK, maxEntities, entityAssignments[5]);

			CreateSubNode( entities, //nodesCreated, 
							BOTTOM_RIGHT_BACK, maxEntities, entityAssignments[6]);

			CreateSubNode( entities, //nodesCreated, 
							TOP_RIGHT_BACK, maxEntities, entityAssignments[7]);

			// CLEANUP
			for(int i = 0; i < 8; i++)
			{
				entityAssignments[i]->clear();
				delete entityAssignments[i];
			}
			
			m_entities.clear();//if node is subdivided, it no longer has entities
		}
		else // ELSE, NO NEED TO KEEP SUBDIVIDING
		{		
			// RECURSION FINISHED, ASSIGN THE INDICES TO THIS NODE			
			m_isLeafNode = true;
			m_numEntities = (entities->size());

			m_entities.assign(entities->begin(), entities->end());
			//m_pIndices = new int [m_numIndices];
			//assert(m_pIndices != NULL && "OctreeNode::SubdivideNode8");
			//memcpy(m_pIndices, indices, (m_numIndices * sizeof(int)));

			// SINCE IT'S A LEAF NODE, SET CHILDREN NODES AS NULL
			for(int i=0; i<8; ++i)
				m_pSubNodes[i]=NULL;
		}
	}
	else // ELSE, NUMINDICES IS ZERO
	{
		m_isLeafNode = true;
		m_numEntities = 0;
		for(int i=0; i<8; ++i)
			m_pSubNodes[i]=NULL;
	}

	return true;
}