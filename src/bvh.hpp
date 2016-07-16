/* bvh.hpp
 *
 * Implements Bounding Volume Hierarchies for ray-intersection acceleration
 */

#include <vector>

#include "geometry.hpp"

class BVHNode : public SceneObject {
private:
	SceneObject *left;
	SceneObject *right;
	BBox bbox;
public:	
	IntersectionResult intersects(const Ray &r);
	IntersectionResult intersects(const Ray &r) const;
	SceneObject *makeCopy();
	SceneObject *makeCopy() const;
	BBox getBBox();
	BBox getBBox() const;

	~BVHNode();
	BVHNode(const BVHNode &node);
	BVHNode(const std::vector<SceneObject *> &objs);
	BVHNode(const std::vector<SceneObject *> &objs, int axis);
};

