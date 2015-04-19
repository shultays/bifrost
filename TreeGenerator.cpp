#include "TreeGenerator.h"

#include "gGlobals.h"

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



void generateBranch(Vec3 pos, float maxHeight, float widthMultiplier, Mat3 mat, FixedSizedArray<Branch, 200>& branches, int depth) {
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

				if (branchStartH > maxHeight * 0.4f && branchStartH < maxHeight * 0.9f) {
					float remainingH = maxHeight - branchStartH;


					Mat3 mat = oldNode.mat;

					mat.rotateBy(randFloat(0.2f*pi_d2, 0.7f*pi_d2), oldNode.mat.row0);
					mat.rotateBy(randFloat(0.2f*pi_d2, 0.7f*pi_d2), oldNode.mat.row1);
					mat.rotateBy(lastZRotation, oldNode.mat.row2);

					generateBranch(oldNode.midPoint + oldNode.mat.row2 * branchH, randFloat(remainingH * 0.5f, remainingH * 0.7f), widthMultiplier,
						mat,
						branches, depth + 1);

					lastZRotation += randFloat(pi_d2, pi);
				}
			} else {
				startH += 0.1f;
				h -= 0.1f;
			}
		}
	}

	for (unsigned i = 0; i < nodes.size() - 1; i++) {
		TrunkNode& node = nodes[i];
		TrunkNode& nextNode = nodes[i + 1];

		for (int j = 0; j < node.sideCount; j++) {
			debugRenderer.addLine(node.sides[j], nextNode.sides[j], 0xFFFFFFFF, LIFE_TIME_INFINITE);
			if (i == 0) {
				debugRenderer.addLine(node.sides[j], node.sides[j + 1], 0xFFFFFFFF, LIFE_TIME_INFINITE);
			}
			debugRenderer.addLine(nextNode.sides[j], nextNode.sides[j + 1], 0xFFFFFFFF, LIFE_TIME_INFINITE);
		}
	}
}

void TreeGenerator::generateTree(Vec3 pos) {

	FixedSizedArray<Branch, 200> branches;

	Mat3 mat;
	mat.makeIdentity();
	mat *= Mat3::rotationX(0.0f);
	mat *= Mat3::rotationY(0.0f);
	mat *= Mat3::rotationZ(randFloat(-pi, +pi));


	generateBranch(pos, randFloat(3.0f, 7.0f), 1.0f, mat, branches, 0);

}
