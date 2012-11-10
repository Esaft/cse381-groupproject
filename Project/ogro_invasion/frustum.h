#ifndef BOGLGP_FRUSTUM_H
#define BOGLGP_FRUSTUM_H

struct Plane {
	float a, b, c, d;
};

class Frustum {
public:
	void updateFrustum();
	bool sphereInFrustum(float x, float y, float z, float radius);
	bool PointInFrustum(float x, float y, float z);
	bool CubeInFrustum( float x, float y, float z, float size);

private:
	Plane m_planes[6];

    enum {
	    PLANE_LEFT = 0,
	    PLANE_RIGHT,
	    PLANE_TOP,
	    PLANE_BOTTOM,
	    PLANE_FAR,
	    PLANE_NEAR
    };

	//int entitiesSentToFrustum;
	Plane extractPlane(float a, float b, float c, float d);
};

#endif