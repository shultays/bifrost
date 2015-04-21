#include "TreeGenerator.h"

#include "gGlobals.h"


struct TreeTriangle {
	Vec3 vertices[3];
	Vec3 color;
};

void generateSphereRec(float radius, const Vec3& pointA, const Vec3& pointB, const Vec3& pointC, std::vector<Vec3>& points, int depth = 0) {

	Vec3 v0 = pointB - pointA;
	Vec3 v1 = pointC - pointA;
	float t0 = v0.y*v1.z - v0.z*v1.y;
	float t1 = v0.z*v1.x - v0.x*v1.z;
	float t2 = v0.x*v1.y - v0.y*v1.x;
	float areaSquared = (t0*t0 + t1*t1 + t2*t2) / 4.0f;
	if (depth < 2) {
		Vec3 midAB = (pointA + pointB) / 2.0f;
		Vec3 midBC = (pointB + pointC) / 2.0f;
		Vec3 midAC = (pointA + pointC) / 2.0f;
		midAB.normalize();
		midAB *= radius;
		midBC.normalize();
		midBC *= radius;
		midAC.normalize();
		midAC *= radius;
		generateSphereRec(radius, pointA, midAB, midAC, points, depth + 1);
		generateSphereRec(radius, midAB, pointB, midBC, points, depth + 1);
		generateSphereRec(radius, midAC, midBC, pointC, points, depth + 1);
		generateSphereRec(radius, midBC, midAC, midAB, points, depth + 1);
	} else {
		points.push_back(pointA);
		points.push_back(pointB);
		points.push_back(pointC);
	}
}


void generateSphere(const Vec3& mid, float radius, std::vector<Vec3>& points) {
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*radius, Vec3(0.0f, 1.0f, 0.0f)*radius, Vec3(0.0f, 0.0f, 1.0f)*radius, points);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*radius, Vec3(0.0f, 0.0f, 1.0f)*-radius, Vec3(0.0f, 1.0f, 0.0f)*radius, points);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*radius, Vec3(0.0f, 0.0f, 1.0f)*radius, Vec3(0.0f, 1.0f, 0.0f)*-radius, points);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*radius, Vec3(0.0f, 1.0f, 0.0f)*-radius, Vec3(0.0f, 0.0f, 1.0f)*-radius, points);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*-radius, Vec3(0.0f, 0.0f, 1.0f)*radius, Vec3(0.0f, 1.0f, 0.0f)*radius, points);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*-radius, Vec3(0.0f, 1.0f, 0.0f)*-radius, Vec3(0.0f, 0.0f, 1.0f)*radius, points);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*-radius, Vec3(0.0f, 1.0f, 0.0f)*radius, Vec3(0.0f, 0.0f, 1.0f)*-radius, points);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*-radius, Vec3(0.0f, 0.0f, 1.0f)*-radius, Vec3(0.0f, 1.0f, 0.0f)*-radius, points);

}

struct TrunkNode {

	Vec3 midPoint;
	int sideCount;
	std::vector<Vec3> sides;

	float height;
	float width;

	Mat3 mat;

	void createSides() {
		sides.clear();

		for (int i = 0; i < sideCount; i++) {
			float ang = i * pi * 2 / sideCount;
			float s = sin(ang)* width;
			float c = cos(ang)* width;
			sides.push_back(Vec3(midPoint + mat.row0*s + mat.row1*c));
		}
		sides.push_back(sides[0]);
	}
};

struct Branch {
	std::vector<TrunkNode> nodes;
	std::vector<Vec3> points;
	float height;
};

float getRandomBranchRotation(float initialRotation, float maxRotationAmount) {

	const float maxHorizontalAngle = pi / 32;

	float rotationMinMult = 1.0f;
	float rotationMaxMult = 1.0f;

	float rotationMinWeight = get_clamped(initialRotation, -maxHorizontalAngle, 0) / -maxHorizontalAngle;
	rotationMinWeight = 1.0f - (rotationMinWeight);
	float rotationMaxWeight = get_clamped(initialRotation, 0, maxHorizontalAngle) / maxHorizontalAngle;
	rotationMaxWeight = 1.0f - (rotationMaxWeight);


	return initialRotation + randFloat(-maxRotationAmount * rotationMinMult, maxRotationAmount * rotationMaxMult);
}



void generateBranch(Vec3 pos, float maxHeight, float widthMultiplier, Mat3 mat, FixedSizedArray<Branch, 200>& branches, int depth, std::vector<TreeTriangle>& vertices) {
	branches.insert(Branch());
	Branch& branch = branches[branches.size() - 1];
	std::vector<TrunkNode>& nodes = branch.nodes;
	nodes.resize(1);

	nodes[0].mat = mat;

	nodes[0].sideCount = 4;
	nodes[0].height = 0.0f;
	nodes[0].midPoint = pos;
	nodes[0].sideCount = 4;

	float widthMultiplierViaHeight = get_clamped(maxHeight * 0.2f, 0.1f, 1.5f);

	nodes[0].width = 0.4f * widthMultiplier * widthMultiplierViaHeight;
	float minWidth = 0.15f * widthMultiplier * widthMultiplierViaHeight;
	nodes[0].createSides();

	float lastZRotation = randFloat(pi_2);

	while (nodes[nodes.size() - 1].height < maxHeight) {
		nodes.push_back(nodes[nodes.size() - 1]);
		TrunkNode& node = nodes[nodes.size() - 1];
		TrunkNode& oldNode = nodes[nodes.size() - 2];

		float heightDiff = randFloat(0.6f, 1.0f);
		node.height += heightDiff;
		node.midPoint = oldNode.midPoint + oldNode.mat.row2 * heightDiff;

		node.width = oldNode.width * 0.6f + minWidth*0.4f;


		node.mat.rotateByX(randFloat(-pi / 80, pi / 80));
		node.mat.rotateByY(randFloat(-pi / 80, pi / 80));
		node.mat.rotateByZ(randFloat(-pi / 10, pi / 10));


		node.createSides();

		float h = node.height - oldNode.height;
		float startH = oldNode.height;

		while (node.width > 0.04f && h > 0.0f) {
			if (1) {
				float branchH = randFloat(h);
				startH += branchH + 0.3f;
				h -= branchH + 0.3f;

				float branchStartH = oldNode.height + branchH;

				if (branchStartH > maxHeight * 0.3f && branchStartH < maxHeight * 0.7f) {
					float remainingH = maxHeight - branchStartH;


					Mat3 mat = oldNode.mat;

					mat.rotateBy(randFloat(0.2f*pi_d2, 0.7f*pi_d2), oldNode.mat.row0);
					mat.rotateBy(randFloat(0.2f*pi_d2, 0.7f*pi_d2), oldNode.mat.row1);
					mat.rotateBy(lastZRotation, oldNode.mat.row2);

					generateBranch(oldNode.midPoint + oldNode.mat.row2 * branchH, randFloat(remainingH * 0.5f, remainingH * 0.7f), widthMultiplier,
						mat,
						branches, depth + 1, vertices);

					lastZRotation += randFloat(pi_d2, pi);
				}
			} else {
				startH += 0.1f;
				h -= 0.1f;
			}
		}
	}

	float sphereRadius = maxHeight * 0.5f;
	sphereRadius = gmax(sphereRadius, 0.4f);
	generateSphere(nodes[nodes.size() - 1].midPoint, sphereRadius, branch.points);
	Vec3 mid = nodes[nodes.size() - 1].midPoint;

	mat.identity();
	mat.rotateByX(randFloat(-pi, +pi));
	mat.rotateByY(randFloat(-pi, +pi));
	mat.rotateByZ(randFloat(-pi, +pi));
	for (unsigned i = 0; i < branch.points.size(); i += 3) {
		/*debugRenderer.addLine(mid + branch.points[i + 0], mid + branch.points[i + 1], 0xFFFFFFFF, LIFE_TIME_INFINITE);
		debugRenderer.addLine(mid + branch.points[i + 1], mid + branch.points[i + 2], 0xFFFFFFFF, LIFE_TIME_INFINITE);
		debugRenderer.addLine(mid + branch.points[i + 2], mid + branch.points[i + 0], 0xFFFFFFFF, LIFE_TIME_INFINITE);*/
		TreeTriangle tri;

		tri.vertices[0] = mid + branch.points[i + 0] * mat;
		tri.vertices[1] = mid + branch.points[i + 1] * mat;
		tri.vertices[2] = mid + branch.points[i + 2] * mat;
		tri.color = Vec3::fromColor(0x0D7000);


		vertices.push_back(tri);
	}

	for (unsigned i = 0; i < nodes.size() - 1; i++) {
		TrunkNode& node = nodes[i];
		TrunkNode& nextNode = nodes[i + 1];

		for (int j = 0; j < node.sideCount; j++) {

			TreeTriangle tri;

			tri.vertices[0] = node.sides[j];
			tri.vertices[1] = nextNode.sides[j];
			tri.vertices[2] = nextNode.sides[j + 1];
			tri.color = Vec3::fromColor(0x663500);
			vertices.push_back(tri);

			tri.vertices[0] = node.sides[j];
			tri.vertices[1] = nextNode.sides[j + 1];
			tri.vertices[2] = node.sides[j + 1];
			tri.color = Vec3::fromColor(0x663500);
			vertices.push_back(tri);
			/*
			debugRenderer.addLine(node.sides[j], nextNode.sides[j], 0xFFFFFFFF, LIFE_TIME_INFINITE);
			if (i == 0) {
			debugRenderer.addLine(node.sides[j], node.sides[j + 1], 0xFFFFFFFF, LIFE_TIME_INFINITE);
			}
			debugRenderer.addLine(nextNode.sides[j], nextNode.sides[j + 1], 0xFFFFFFFF, LIFE_TIME_INFINITE);*/
		}
	}


}

gRenderable* TreeGenerator::generateTree(Vec3 pos) {
	std::vector<TreeTriangle> vertices;

	FixedSizedArray<Branch, 200> branches;

	Mat3 mat;
	mat.makeIdentity();
	mat *= Mat3::rotationX(0.0f);
	mat *= Mat3::rotationY(0.0f);
	mat *= Mat3::rotationZ(randFloat(-pi, +pi));


	generateBranch(pos, randFloat(1.0f, 7.0f), 1.0f, mat, branches, 0, vertices);

	gVertexBufferRenderable* renderable = new gVertexBufferRenderable(VERTEX_PROP_COLOR | VERTEX_PROP_POSITION | VERTEX_PROP_NORMAL, vertices.size() * 3);

	for (unsigned i = 0; i < vertices.size(); i++) {
		VertexPointer pointer0 = renderable->getVertexPointerAt(i * 3 + 0);
		VertexPointer pointer1 = renderable->getVertexPointerAt(i * 3 + 1);
		VertexPointer pointer2 = renderable->getVertexPointerAt(i * 3 + 2);

		*pointer0.position = vertices[i].vertices[0];
		*pointer1.position = vertices[i].vertices[1];
		*pointer2.position = vertices[i].vertices[2];

		*pointer0.color = *pointer1.color = *pointer2.color = Vec4(vertices[i].color, 1.0f);

		Vec3 normal = Vec3::cross(vertices[i].vertices[1] - vertices[i].vertices[0], vertices[i].vertices[2] - vertices[i].vertices[0]);
		normal.normalize();
		*pointer0.normal = *pointer1.normal = *pointer2.normal = normal;

	}
	renderable->build();
	return renderable;
}
