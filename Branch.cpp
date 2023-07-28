#include "Branch.h"

Branch::Branch() {}

void Branch::addBranch(Trunk& t, int circle, float expo, float curveNoiseFreq, float curvature) {

	ofMesh& mesh = t.get_mesh();
	int circleNumPoints = t.get_circleNumPoints();
	int numCircles = t.get_numCircles();
	float initialRadius = t.get_initialRadius();
	float radiusDelta = -initialRadius / numCircles;
	float oldRadius = initialRadius + circle * radiusDelta;
	float newDelta = -oldRadius/numCircles;
	float height = t.get_height()*(numCircles-circle)/numCircles;
	float curveNoiseRandSeedx = ofRandom(0, 10);
	float curveNoiseRandSeedz = ofRandom(0, 10);
	float curve = curvature * initialRadius;
	float deltaY = height / numCircles;
	float offsetx;
	float offsetz;
	size_t numVertices = mesh.getNumVertices();

	//initial circle
	int startIndex = circle * circleNumPoints;
	for (int i = 0; i < circleNumPoints; i++) {
		int currentIndex = startIndex + i;
		glm::vec3 oldPoint = mesh.getVertex(currentIndex);
		mesh.addVertex(glm::vec3(oldPoint.x, oldPoint.y, oldPoint.z));
	}


	//circle centers for tilting circles perpendicularly to direction of branch (line that connects the centers)
	std::vector< glm::vec3 > centers;
	centers.push_back(t.centersPath.at(circle));
	float startY = mesh.getVertex(mesh.getNumVertices() - 1).y;


	for (int i = 1; i < numCircles; i = i + 1) {
		float radius = (oldRadius + i * newDelta + ofRandom(-1.f, 1.f) * newDelta);
		float posy = i * deltaY;
		if (i == 1) { offsetx = 0; offsetz = 0; }
		else {
			float offsetx = ofMap(ofNoise(float(i) / numCircles * curveNoiseFreq, curveNoiseRandSeedx), 0, 1, -curve, curve);
			float offsetz = ofMap(ofNoise(float(i) / numCircles * curveNoiseFreq, curveNoiseRandSeedz), 0, 1, -curve, curve);
		}
		float shape = pow(ofMap((i), 1, numCircles - 1, 0, 1), expo) * deltaY * (numCircles);

		centers.push_back(glm::vec3(centers.at(0).x + shape + offsetx, posy + centers.at(0).y, centers.at(0).z+offsetz));

		// Calculate the direction vector between circle centers and normalize it
		glm::vec3 centerToNext = centers[i] - centers[i-1];
		centerToNext = glm::normalize(centerToNext);

		// Calculate the rotation angles around the x, y, and z axes based on the direction vector
		float xRotation = asin(centerToNext.y);
		float yRotation = atan2(centerToNext.x, centerToNext.z);
		float zRotation = 0; // To keep the sections perpendicular to the line, we set zRotation to 0
		
		
		for (float deg = 0; deg < circleNumPoints; deg = deg + 1) {
			//unitary circle
			float posx = (cos(deg * (2 * PI / circleNumPoints)) + ofMap(ofNoise(ofRandom(0, 1)), 0, 1, -1, 1));
			float posz = (sin(deg * (2 * PI / circleNumPoints)) + ofMap(ofNoise(ofRandom(0, 1)), 0, 1, -1, 1));
			float x1 = radius * posx ;
			float y1 = 0;
			float z1 = radius * posz;
			//3D ROTATION MATRICES (applied in origin)
			float x2 = x1 * (cos(yRotation) * cos(zRotation)) + y1 * (sin(xRotation) * sin(yRotation) * cos(zRotation) - cos(xRotation) * sin(zRotation)) + z1 * (cos(xRotation) * sin(yRotation) * cos(zRotation) + sin(xRotation) * sin(zRotation));
			float y2 = x1 * (cos(yRotation) * sin(zRotation)) + y1 * (sin(xRotation) * sin(yRotation) * sin(zRotation) + cos(xRotation) * cos(zRotation)) + z1 * (cos(xRotation) * sin(yRotation) * sin(zRotation) - sin(xRotation) * cos(zRotation));
			float z2 = x1 * (-sin(yRotation)) + y1 * (sin(xRotation) * cos(yRotation)) + z1 * (cos(xRotation) * cos(yRotation));
			//translation
			float x3 = centers.at(0).x + x2 + shape + offsetx;
			float y3 = centers.at(0).y + y2 + posy;
			float z3 = centers.at(0).z + z2 + offsetz;

			mesh.addVertex(glm::vec3(x3, y3, z3));
		}
	}


	// INDEXES CONNECT POINTS
	for (int i = 0; i < numCircles-1; i = i + 1) {
		for (int deg = 0; deg < circleNumPoints; deg = deg + 1) {
			int currentIndex = deg + (i * circleNumPoints) + numVertices;
			int nextIndex = (deg + 1) % circleNumPoints + (i * circleNumPoints) + numVertices;
			int nextNextIndex = (deg + 2) % circleNumPoints + (i * circleNumPoints) + numVertices;
			int nextCircleIndex = deg + ((i + 1) * circleNumPoints) + numVertices;
			int nextCircleNextIndex = (deg + 1) % circleNumPoints + ((i + 1) * circleNumPoints) + numVertices;

			// Add the triangle vertices in a counter-clockwise direction
			mesh.addIndex(currentIndex);
			mesh.addIndex(nextIndex);
			mesh.addIndex(nextNextIndex);

			mesh.addIndex(currentIndex);
			mesh.addIndex(nextIndex);
			mesh.addIndex(nextCircleIndex);

			mesh.addIndex(nextIndex);
			mesh.addIndex(nextCircleNextIndex);
			mesh.addIndex(nextCircleIndex);
		}
	}
}