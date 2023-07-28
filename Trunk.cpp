#include "Trunk.h"


Trunk::Trunk() {
	Trunk::x = 0;
	Trunk::y = 0;
	Trunk::z = 0;
	Trunk::xRotation = 0;
	Trunk::yRotation = 0;
	Trunk::zRotation = 0;
	Trunk::circleNumPoints = 0;
	Trunk::initialRadius = 0;
	Trunk::height = 0;
	Trunk::numCircles = 0;
	Trunk::curvature = 0;
	Trunk::curveNoiseFreq = 0;
};

void Trunk::addTrunk(float x, float y, float z, float xRotation, float yRotation, float zRotation, int circleNumPoints, float initialRadius, float height, int numCircles, float curvature, float curveNoiseFreq) {
	Trunk::x = x;
	Trunk::y = y;
	Trunk::z = z;
	Trunk::xRotation = xRotation;
	Trunk::yRotation = yRotation;
	Trunk::zRotation = zRotation;
	Trunk::circleNumPoints = circleNumPoints;
	Trunk::initialRadius = initialRadius;
	Trunk::height = height;
	Trunk::numCircles = numCircles;
	Trunk::curvature = curvature;
	Trunk::curveNoiseFreq = curveNoiseFreq;

	Trunk::buildMesh();
}

void Trunk::buildMesh() {
	size_t numVertices = mesh.getNumVertices();
	float radiusDelta = -initialRadius / numCircles;
	float curve = curvature * initialRadius;
	float curveNoiseRandSeedx = ofRandom(0, 10);
	float curveNoiseRandSeedz = ofRandom(0, 10);

	//POINT GENERATION AND PATH DETECTION
	for (int i = 0; i < numCircles; i = i + 1) {
		float radius = (initialRadius + i * radiusDelta + ofRandom(-1.f, 1.f) * radiusDelta);
		float offsetx = ofMap(ofNoise(float(i) / numCircles * curveNoiseFreq, curveNoiseRandSeedx), 0, 1, -curve, curve);
		float offsetz = ofMap(ofNoise(float(i) / numCircles * curveNoiseFreq, curveNoiseRandSeedz), 0, 1, -curve, curve);
		float posy = i * height / numCircles;
		for (float deg = 0; deg < circleNumPoints; deg = deg + 1) {
			float posx = (cos(deg * (2 * PI / circleNumPoints)) + ofMap(ofNoise(ofRandom(0, 1)), 0, 1, -1, 1));
			float posz = (sin(deg * (2 * PI / circleNumPoints)) + ofMap(ofNoise(ofRandom(0, 1)), 0, 1, -1, 1));
			float x1 = radius * posx + offsetx;
			float y1 = posy;
			float z1 = radius * posz + offsetz;
			//3D ROTATION MATRICES
			float x2 = x1 * (cos(yRotation) * cos(zRotation)) + y1 * (sin(xRotation) * sin(yRotation) * cos(zRotation) - cos(xRotation) * sin(zRotation)) + z1 * (cos(xRotation) * sin(yRotation) * cos(zRotation) + sin(xRotation) * sin(zRotation));
			float y2 = x1 * (cos(yRotation) * sin(zRotation)) + y1 * (sin(xRotation) * sin(yRotation) * sin(zRotation) + cos(xRotation) * cos(zRotation)) + z1 * (cos(xRotation) * sin(yRotation) * sin(zRotation) - sin(xRotation) * cos(zRotation));
			float z2 = x1 * (-sin(yRotation)) + y1 * (sin(xRotation) * cos(yRotation)) + z1 * (cos(xRotation) * cos(yRotation));
			mesh.addVertex(glm::vec3(x2 + x, y2 + y, z2 + z));
		}
		////PATH
		//int resolution = 10;
		//float newX = ( - radius / 2 + offsetx);
		//float newY =  + posy;
		//float newZ = ( - radius / 2 + offsetz);
		//float newX2 = x + newX * (cos(yRotation) * cos(zRotation)) + newY * (sin(xRotation) * sin(yRotation) * cos(zRotation) - cos(xRotation) * sin(zRotation)) + newZ * (cos(xRotation) * sin(yRotation) * cos(zRotation) + sin(xRotation) * sin(zRotation));
		//float newY2 = y + newX * (cos(yRotation) * sin(zRotation)) + newY * (sin(xRotation) * sin(yRotation) * sin(zRotation) + cos(xRotation) * cos(zRotation)) + newZ * (cos(xRotation) * sin(yRotation) * sin(zRotation) - sin(xRotation) * cos(zRotation));
		//float newZ2 = z + newX * (-sin(yRotation)) + newY * (sin(xRotation) * cos(yRotation)) + newZ * (cos(xRotation) * cos(yRotation));
		//if (path.size() >= 1) {
		//	glm::vec3 prev = path.at(path.size() - 1);
		//	float offX3 = (newX2 - prev.x) / resolution;
		//	float offY3 = (newY2 - prev.y) / resolution;
		//	float offZ3 = (newZ2 - prev.z) / resolution;
		//	for (int i = 0; i < resolution - 1; i++) {
		//		path.push_back(glm::vec3(prev.x + offX3 * i, prev.y + offY3 * i, prev.z + offZ3 * i));
		//	}
		//	
		//}
		//else {
		//	path.push_back(glm::vec3(newX2, newY2, newZ2));
		//}

		centersPath.push_back(glm::vec3(offsetx+x, posy+y, offsetz+z));
	}

	// INDEXES CONNECT POINTS
	for (int i = 0; i < numCircles - 1; i = i + 1) {
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

void Trunk::draw() {
	mesh.drawWireframe();
	mesh.drawVertices();
	mesh.draw();
}

//GETTERS

float Trunk::get_x() {
	return x;
}

float Trunk::get_y() {
	return y;
}

float Trunk::get_z() {
	return z;
}

float Trunk::get_xRotation() {
	return xRotation;
}

float Trunk::get_yRotation() {
	return yRotation;
}

float Trunk::get_zRotation() {
	return zRotation;
}

int Trunk::get_circleNumPoints() {
	return circleNumPoints;
}

float Trunk::get_initialRadius() {
	return initialRadius;
}

float Trunk::get_height() {
	return height;
}

int Trunk::get_numCircles() {
	return numCircles;
}

float Trunk::get_curvature() {
	return curvature;
}

float Trunk::get_curveNoiseFreq() {
	return curveNoiseFreq;
}

ofMesh& Trunk::get_mesh() {
	return mesh;
}



//SETTERS

void Trunk::set_x(float x) {
	mesh.clear();
	Trunk::x = x;
	Trunk::buildMesh();
}

void Trunk::set_y(float y) {
	mesh.clear();
	Trunk::y = y;
	Trunk::buildMesh();
}

void Trunk::set_z(float z) {
	mesh.clear();
	Trunk::z = z;
	Trunk::buildMesh();
}

void Trunk::set_xRotation(float xRotation) {
	mesh.clear();
	Trunk::xRotation = xRotation;
	Trunk::buildMesh();
}

void Trunk::set_yRotation(float yRotation) {
	mesh.clear();
	Trunk::yRotation = yRotation;
	Trunk::buildMesh();
}

void Trunk::set_zRotation(float zRotation) {
	mesh.clear();
	Trunk::zRotation= zRotation;
	Trunk::buildMesh();
}

void Trunk::set_circleNumPoints(int circleNumPoints) {
	mesh.clear();
	Trunk::circleNumPoints= circleNumPoints;
	Trunk::buildMesh();
}

void Trunk::set_initialRadius(float initialRadius) {
	mesh.clear();
	Trunk::initialRadius= initialRadius;
	Trunk::buildMesh();
}

void Trunk::set_height(float height) {
	mesh.clear();
	Trunk::height = height;
	Trunk::buildMesh();
}

void Trunk::set_numCircles(int numCircles) {
	mesh.clear();
	Trunk::numCircles = numCircles;
	Trunk::buildMesh();
}

void Trunk::set_curvature(float curvature) {
	mesh.clear();
	Trunk::curvature = curvature;
	Trunk::buildMesh();
}

void Trunk::set_curveNoiseFreq(float curveNoiseFreq) {
	mesh.clear();
	Trunk::curveNoiseFreq = curveNoiseFreq;
	Trunk::buildMesh();
}
