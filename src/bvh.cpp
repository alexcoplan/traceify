#include "bvh.hpp"

BBox BVHNode::getBBox() {
	return bbox;
}

BBox BVHNode::getBBox() const {
	return bbox;
}

// N.B. BVHNodes take charge of the objects they point to
// => any SceneObjects passed to BVH node must be heap-allocated
// and not freed by other code
BVHNode::~BVHNode() {
	if (left != NULL) delete left;
	if (right != NULL) delete right;
}

// copy constructor
BVHNode::BVHNode(const BVHNode &node) {
	left = node.left->makeCopy();
	right = node.right->makeCopy();
	bbox = node.bbox;
}

BVHNode::BVHNode(const std::vector<SceneObject *> &objs) : BVHNode(objs, X_AXIS) {}

// TODO: smart axis selection rather than just rotating through them
BVHNode::BVHNode(const std::vector<SceneObject *> &objs, int axis) {
	int n = objs.size();

	if (n == 0) {
		throw GeometryException("Can't create a BVH node with an empty object list.");
	}
	else if (n == 1) {
		left = objs[0];
		right = NULL;
		bbox = objs[0]->getBBox();
		return;
	}
	else if (n == 2) {
		left = objs[0];
		right = objs[1];
		bbox = objs[0]->getBBox();
		bbox.swallow(objs[1]->getBBox());
		return;
	}

	bbox = objs[0]->getBBox();
	for (int i = 1; i < n; i++) 
		bbox.swallow(objs[i]->getBBox());

	double midpoint = bbox.midpointForAxis(axis);
	
	std::vector<SceneObject *> leftList;
	std::vector<SceneObject *> rightList;
	for (int i = 0; i < n; i++) {
		if (objs[i]->getBBox().midpointForAxis(axis) <= midpoint)
			leftList.push_back(objs[i]);
		else
			rightList.push_back(objs[i]);
	}

	left = new BVHNode(leftList, (axis+1)%3);
	right = new BVHNode(rightList, (axis+1)%3);
}

// TODO: this is currently just bounding box intersection
// implement intersection with children...
IntersectionResult BVHNode::intersects(const Ray &ray) const {
	const vec3 e = ray.origin;
       	const vec3 d = ray.direction;

	double t_min = (bbox.min[X_AXIS] - e.x()) / d.x();
	double t_max = (bbox.max[X_AXIS] - e.x()) / d.x();

	if (t_min > t_max) {
		double tmp = t_max;
		t_max = t_min;
		t_min = tmp;
	}

	double t_y_min = (bbox.min[Y_AXIS] - e.y()) / d.y();
	double t_y_max = (bbox.max[Y_AXIS] - e.y()) / d.y();

	if (t_y_min > t_y_max) {
		double tmp = t_y_max;
		t_y_max = t_y_min;
		t_y_min = tmp;
	}

	if ((t_min > t_y_max) || (t_y_min > t_max))
		return IntersectionResult();

	if (t_y_min > t_min)
		t_min = t_y_min;

	if (t_y_max < t_max)
		t_max = t_y_max;

	double t_z_min = (bbox.min[Z_AXIS] - e.z()) / d.z();
	double t_z_max = (bbox.max[Z_AXIS] - e.z()) / d.z();

	if (t_z_min > t_z_max) {
		double tmp = t_z_max;
		t_z_max = t_z_min;
		t_z_min = tmp;
	}

	if ((t_min > t_z_max) || (t_z_min > t_max))
		return IntersectionResult();

	if (t_z_min > t_min)
		t_min = t_z_min;

	if (t_z_max < t_max)
		t_max = t_z_max;

	return IntersectionResult(t_min);
}

IntersectionResult BVHNode::intersects(const Ray &r) {
	return const_cast <const BVHNode *>(this)->intersects(r);
}

SceneObject *BVHNode::makeCopy() { 		return new BVHNode(*this); }
SceneObject *BVHNode::makeCopy() const { 	return new BVHNode(*this); }

