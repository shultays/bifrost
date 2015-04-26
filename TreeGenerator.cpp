#include "TreeGenerator.h"

#include "gGlobals.h"
#include "gGeometry.h"

void generateSphereRec(float radius, const Vec3& pointA, const Vec3& pointB, const Vec3& pointC, std::vector<Vec3>& points, int depth = 0, float minRadius = 0.8f) {
	float a = Vec3::distanceSquared(pointA, pointB);
	float b = Vec3::distanceSquared(pointB, pointC);
	float c = Vec3::distanceSquared(pointC, pointA);

	float t = a + b - c;
	float areaSquared = (4 * a*b - t*t) / 16.0f;

	if (sqrt(radius) / (depth + 1) > minRadius) {
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


void generateSphere(float radius, std::vector<Vec3>& points, float minRadius = 0.8f) {
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*radius, Vec3(0.0f, 1.0f, 0.0f)*radius, Vec3(0.0f, 0.0f, 1.0f)*radius, points, 0, minRadius);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*radius, Vec3(0.0f, 0.0f, 1.0f)*-radius, Vec3(0.0f, 1.0f, 0.0f)*radius, points, 0, minRadius);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*radius, Vec3(0.0f, 0.0f, 1.0f)*radius, Vec3(0.0f, 1.0f, 0.0f)*-radius, points, 0, minRadius);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*radius, Vec3(0.0f, 1.0f, 0.0f)*-radius, Vec3(0.0f, 0.0f, 1.0f)*-radius, points, 0, minRadius);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*-radius, Vec3(0.0f, 0.0f, 1.0f)*radius, Vec3(0.0f, 1.0f, 0.0f)*radius, points, 0, minRadius);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*-radius, Vec3(0.0f, 1.0f, 0.0f)*-radius, Vec3(0.0f, 0.0f, 1.0f)*radius, points, 0, minRadius);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*-radius, Vec3(0.0f, 1.0f, 0.0f)*radius, Vec3(0.0f, 0.0f, 1.0f)*-radius, points, 0, minRadius);
	generateSphereRec(radius, Vec3(1.0f, 0.0f, 0.0f)*-radius, Vec3(0.0f, 0.0f, 1.0f)*-radius, Vec3(0.0f, 1.0f, 0.0f)*-radius, points, 0, minRadius);
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


	return initialRotation + random.randFloat(-maxRotationAmount * rotationMinMult, maxRotationAmount * rotationMaxMult);
}



void generateBranch(const Vec3& pos, float maxHeight, float widthMultiplier, Mat3 mat, FixedSizedArray<Branch, 200>& branches, int depth, std::vector<TreeTriangle>& vertices, std::vector<Sphere>& spheres, gRandom& random) {
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

	float lastZRotation = random.randFloat(pi_2);

	while (nodes[nodes.size() - 1].height < maxHeight) {
		nodes.push_back(nodes[nodes.size() - 1]);
		TrunkNode& node = nodes[nodes.size() - 1];
		TrunkNode& oldNode = nodes[nodes.size() - 2];

		float heightDiff = random.randFloat(0.6f, 1.0f);
		node.height += heightDiff;
		node.midPoint = oldNode.midPoint + oldNode.mat.row2 * heightDiff;

		node.width = oldNode.width * 0.6f + minWidth*0.4f;


		node.mat.rotateByX(random.randFloat(-pi / 80, pi / 80));
		node.mat.rotateByY(random.randFloat(-pi / 80, pi / 80));
		node.mat.rotateByZ(random.randFloat(-pi / 10, pi / 10));


		node.createSides();
	}

	float sphereRadius = maxHeight * 0.5f;
	sphereRadius = gmax(sphereRadius, 0.4f);
	Sphere sphere(nodes[nodes.size() - 1].midPoint, sphereRadius);
	bool inside = false;
	for (unsigned i = 0; i < spheres.size(); i++) {
		if (gGeometry::contains(spheres[i], sphere)) {
			inside = true;
			break;
		}
	}

	if (inside == false) {
		generateSphere(sphereRadius, branch.points);
		Vec3 mid = nodes[nodes.size() - 1].midPoint;

		mat.identity();
		mat.rotateByX(random.randFloat(-pi, +pi));
		mat.rotateByY(random.randFloat(-pi, +pi));
		mat.rotateByZ(random.randFloat(-pi, +pi));

		Vec3 color = lerp(Vec3::fromColor(0xB6E016), Vec3::fromColor(0x07B508), random.randFloat());
		for (unsigned i = 0; i < branch.points.size(); i += 3) {
			TreeTriangle tri;

			tri.vertices[0] = mid + branch.points[i + 0] * mat;
			tri.vertices[1] = mid + branch.points[i + 1] * mat;
			tri.vertices[2] = mid + branch.points[i + 2] * mat;
			tri.color = color;

			vertices.push_back(tri);

			float distance = ((tri.vertices[0] + tri.vertices[1] + tri.vertices[2]) * 0.33333f).distance(mid);

			spheres.push_back(Sphere(mid, distance));
		}
	}

	for (unsigned i = 0; i < nodes.size() - 1; i++) {
		TrunkNode& node = nodes[i];
		TrunkNode& nextNode = nodes[i + 1];

		float radiusMin = 10000.0f;
		for (int j = 0; j < node.sideCount; j++) {
			float t1 = ((node.sides[j] + node.sides[j + 1]) * 0.5f).distanceSquared(node.midPoint);
			float t2 = ((nextNode.sides[j] + nextNode.sides[j + 1]) * 0.5f).distanceSquared(nextNode.midPoint);

			radiusMin = gmin(radiusMin, t1);
			radiusMin = gmin(radiusMin, t2);
		}
		inside = false;
		Capsule capsule(node.midPoint, nextNode.midPoint, sqrtf(radiusMin));
		for (unsigned i = 0; i < spheres.size(); i++) {
			if (gGeometry::contains(spheres[i], capsule)) {
				inside = true;
				break;
			}
		}
		if (inside == false) {
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
			}
		}
	}


	for (unsigned i = 1; i < nodes.size(); i++) {

		TrunkNode& node = nodes[i];
		TrunkNode& oldNode = nodes[i - 1];

		float h = node.height - oldNode.height;
		float startH = oldNode.height;

		while (node.width > 0.04f && h > 0.0f) {
			if (1) {
				float branchH = random.randFloat(h);
				startH += branchH + 0.3f;
				h -= branchH + 0.3f;

				float branchStartH = oldNode.height + branchH;

				if (branchStartH > maxHeight * 0.3f && branchStartH < maxHeight * 0.7f) {
					float remainingH = maxHeight - branchStartH;


					Mat3 mat = oldNode.mat;

					mat.rotateBy(random.randFloat(0.2f*pi_d2, 0.7f*pi_d2), oldNode.mat.row0);
					mat.rotateBy(random.randFloat(0.2f*pi_d2, 0.7f*pi_d2), oldNode.mat.row1);
					mat.rotateBy(lastZRotation, oldNode.mat.row2);

					generateBranch(oldNode.midPoint + oldNode.mat.row2 * branchH, random.randFloat(remainingH * 0.5f, remainingH * 0.7f), widthMultiplier,
						mat,
						branches, depth + 1, vertices, spheres, random);

					lastZRotation += random.randFloat(pi_d2, pi);
				}
			} else {
				startH += 0.1f;
				h -= 0.1f;
			}
		}
	}


}

void TreeGenerator::generateTree(const Vec3& pos, std::vector<TreeTriangle>& vertices, gRandom& random) {
	std::vector<Sphere> spheres;
	FixedSizedArray<Branch, 200> branches;

	Mat3 mat;
	mat.makeIdentity();
	mat *= Mat3::rotationX(0.0f);
	mat *= Mat3::rotationY(0.0f);
	mat *= Mat3::rotationZ(random.randFloat(-pi, +pi));


	generateBranch(pos, random.randFloat(1.0f, 7.0f), 1.0f, mat, branches, 0, vertices, spheres, random);
}


void TreeGenerator::generateCloud(const Vec3& pos, const Vec3& dir, std::vector<TreeTriangle>& triangles, gRandom& random) {
	float cloudLen = random.randFloat(400.0f, 700.0f);
	Vec3& current = pos - cloudLen * 0.5f;


	Vec3 side = Vec3::cross(dir, Vec3(0.0f, 0.0f, 1.0f));

	while (cloudLen > 0.0f) {
		std::vector<Vec3> points;
		float radius = random.randFloat(200.0f, 500.0f);
		float pushSide = random.randFloat();
		pushSide *= pushSide;
		if (random.randBool()) pushSide = -pushSide;

		generateSphere(radius, points, 130.0f);

		float move = random.randFloat(70.0f, 120.0f) * radius;
		cloudLen -= move;
		current += dir * move;

		Vec3 mid = current + side * pushSide * 20.0f;

		Mat3 mat;
		mat.makeIdentity();
		mat.rotateByX(random.randFloat(-pi, +pi));
		mat.rotateByY(random.randFloat(-pi, +pi));
		mat.rotateByZ(random.randFloat(-pi, +pi));

		for (unsigned i = 0; i < points.size(); i += 3) {
			TreeTriangle tri;

			tri.vertices[0] = mid + points[i + 0] * mat;
			tri.vertices[1] = mid + points[i + 1] * mat;
			tri.vertices[2] = mid + points[i + 2] * mat;
			tri.color = Vec3(1.0f);
			triangles.push_back(tri);
		}
	}
}
