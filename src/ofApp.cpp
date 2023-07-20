#include "ofApp.h"
#include <vector>

std::vector< glm::vec3 > path;
std::vector <ofSpherePrimitive> spheres;



void branch(int x, int y, int z, int xRotation, int yRotation, int zRotation, int circleNumPoints, int initialRadius, int heigth, int numCircles, float curvature, float curveNoiseFreq, ofMesh& mesh, int call) {

	size_t numVertices = mesh.getNumVertices();
	int radiusDelta = -initialRadius / numCircles;
	int curve = curvature * initialRadius;
	float curveNoiseRandSeedx = ofRandom(0, 10);
	float curveNoiseRandSeedz = ofRandom(0, 10);

	//POINT GENERATION AND PATH DETECTION
	for (int i = 0; i < numCircles; i = i + 1) {
		float radius = (initialRadius + i * radiusDelta + ofRandom(-1.f,1.f)*2*radiusDelta);
		float offsetx = ofMap(ofNoise(float(i) / numCircles * curveNoiseFreq, curveNoiseRandSeedx), 0, 1, -curve, curve);
		float offsetz = ofMap(ofNoise(float(i) / numCircles * curveNoiseFreq, curveNoiseRandSeedz), 0, 1, -curve, curve);
		float posy = i * heigth / numCircles;
		for (float deg = 0; deg <= circleNumPoints; deg = deg + 1) {
			float posx = (cos(deg * (2 * PI / circleNumPoints)) + ofMap(ofNoise(ofRandom(0, 1)), 0, 1, -1, 1));
			float posz = (sin(deg * (2 * PI / circleNumPoints)) + ofMap(ofNoise(ofRandom(0, 1)), 0, 1, -1, 1));
			float x1 = radius * posx - radius / 2 + offsetx;
			float y1 = posy;
			float z1 = radius * posz - radius / 2 + offsetz;
			//3D ROTATION MATRICES
			float x2 = x1 * (cos(yRotation) * cos(zRotation)) + y1 * (sin(xRotation) * sin(yRotation) * cos(zRotation) - cos(xRotation) * sin(zRotation)) + z1 * (cos(xRotation) * sin(yRotation) * cos(zRotation) + sin(xRotation) * sin(zRotation)); 
			float y2 = x1 * (cos(yRotation) * sin(zRotation)) + y1 * (sin(xRotation) * sin(yRotation) * sin(zRotation) + cos(xRotation) * cos(zRotation)) + z1 * (cos(xRotation) * sin(yRotation) * sin(zRotation) - sin(xRotation) * cos(zRotation));
			float z2 = x1 * (-sin(yRotation)) + y1 * (sin(xRotation) * cos(yRotation)) + z1 * (cos(xRotation) * cos(yRotation));
			mesh.addVertex(ofPoint(x2+x, y2+y, z2+z));
		}
		//PATH
		int resolution = 10;
		float newX = ( - radius / 2 + offsetx);
		float newY =  + posy;
		float newZ = ( - radius / 2 + offsetz);
		float newX2 = x + newX * (cos(yRotation) * cos(zRotation)) + newY * (sin(xRotation) * sin(yRotation) * cos(zRotation) - cos(xRotation) * sin(zRotation)) + newZ * (cos(xRotation) * sin(yRotation) * cos(zRotation) + sin(xRotation) * sin(zRotation));
		float newY2 = y + newX * (cos(yRotation) * sin(zRotation)) + newY * (sin(xRotation) * sin(yRotation) * sin(zRotation) + cos(xRotation) * cos(zRotation)) + newZ * (cos(xRotation) * sin(yRotation) * sin(zRotation) - sin(xRotation) * cos(zRotation));
		float newZ2 = z + newX * (-sin(yRotation)) + newY * (sin(xRotation) * cos(yRotation)) + newZ * (cos(xRotation) * cos(yRotation));
		if (path.size() >= 1) {
			glm::vec3 prev = path.at(path.size() - 1);
			float offX3 = (newX2 - prev.x) / resolution;
			float offY3 = (newY2 - prev.y) / resolution;
			float offZ3 = (newZ2 - prev.z) / resolution;
			for (int i = 0; i < resolution - 1; i++) {
				path.push_back(glm::vec3(prev.x + offX3 * i, prev.y + offY3 * i, prev.z + offZ3 * i));
			}
			path.push_back(glm::vec3(newX2, newY2, newZ2));
		}
		else {
			path.push_back(glm::vec3(newX2, newY2, newZ2));
		}
	}

	//INDEXES CONNECTS POINTS
	for (int i = 0; i < numCircles; i = i + 1) {
		for (int deg = 0; deg < circleNumPoints; deg = deg + 1) {
			mesh.addIndex(deg % circleNumPoints + (i * circleNumPoints)+ numVertices);
			mesh.addIndex((deg + 1) % circleNumPoints + (i * circleNumPoints ) + numVertices);
			mesh.addIndex((deg + 2) % circleNumPoints + (i * circleNumPoints ) + numVertices);

			mesh.addIndex(deg % circleNumPoints + (i * circleNumPoints) + numVertices);
			mesh.addIndex(deg % circleNumPoints + ((i + 1) * circleNumPoints) + numVertices);
			mesh.addIndex((deg + 1) % circleNumPoints + ((i + 1) * circleNumPoints) + numVertices);
		}
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("3pac");
	ofSetWindowShape(1920, 990);
	ofSetWindowPosition(0, 30);

	ofBackground(0, 0, 0);

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	//model.loadModel("tree.obj");
	//model.setScale(1, 1, 1);
	//path.push_back(glm::vec3(0, 0, 0));

	//BRANCHES
	branch(0, 0, 0, 0, 0, 0, 20, 50, 400, 20, 1.2, 1.2, mesh, 1);
	branch(0, 200, 0, PI / 3, 0, 0, 20, 20, 200, 20, 1.2, 1.2, mesh, 2);
	branch(0, 200, 0, PI / 3, 0, PI/3, 20, 30, 250, 20, 1.2, 1.2, mesh, 3);
	branch(0, 300, 0, PI / 2, 0, 0, 20, 20, 100, 20, 1.2, 1.2, mesh, 4);
	branch(0, 350, 0, 0, 0, PI / 3, 20, 10, 170, 20, 1.2, 1.2, mesh, 5);

	//attaccato al tronco
	//raggio minore di quello del tronco a quell'altezza
	//ruotato di PI/3 in x o z a random
	
	//Numero Rami
	//rami di rami
	//complessità
	//altezza dei rami (ad esempio molto diramato solo in alto oppure grossi branches che cambiano la struttura dell'albero)


	mesh.mergeDuplicateVertices(); //safety check

	//LIGHT
	light.setPosition(0, 600, 0);
	light.setPointLight();
	light.setAttenuation(0.0f, 0.005f, 0.0f);
	ofSetSmoothLighting(true);
	light.setDiffuseColor(ofFloatColor(.85, .85, .55));
	light.setSpecularColor(ofFloatColor(1.f, 1.f, 1.f));

	//CAM
	//cam.setDistance(700);
	cam.setPosition(glm::vec3(400, 400 / 2, 400));
	cam.setTarget(glm::vec3(0, 400 / 2, 0));
	//cam.setFov(60);
	cam.setNearClip(0.1);
	cam.setFarClip(50000);

	//MATERIALS
	lineMaterial.setSpecularColor(ofFloatColor(0.99, 0.99, 0.99));
	lineMaterial.setDiffuseColor(ofFloatColor(0.839, 0.671, 0.024, 1));
	lineMaterial.setAmbientColor(ofFloatColor(0.839, 0.671, 0.024, 1));
	lineMaterial.setEmissiveColor(ofFloatColor(0.839, 0.671, 0.024, 1));

	meshMaterial.setAmbientColor(ofFloatColor(0.6, 0.6, 0.6));
	meshMaterial.setDiffuseColor(ofFloatColor(0.99, 0.99, 0.99));

	//SPHERES LINE
	ofSetSphereResolution(10);
	int numSpheres = 100;
	for (int i = 0; i < numSpheres; i++) {
		spheres.push_back(ofSpherePrimitive());
		spheres.at(i).setRadius(1.f+7.f/(numSpheres)*i);
	}

}
//--------------------------------------------------------------

void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofEnableDepthTest();
	cam.begin();
	light.enable();

		//IMPLEMENT
		//mesh.save();

		ofRotateY(ofGetElapsedTimef() * 4);

		ofPushMatrix();
		//ofSetColor(255, 255, 255, 255);
		ofSetLineWidth(1);
		//ofNoFill();
		meshMaterial.begin();
		mesh.drawVertices();
		mesh.drawWireframe();
		meshMaterial.end();
		//mesh.drawFaces();
		ofPopMatrix();
	
		//ofPushMatrix();
		////ofSetColor(255, 255, 180, 255);
		//ofSetLineWidth(7);
		//lineMaterial.begin();
		//line.draw();
		//lineMaterial.end();
		//ofPopMatrix();

		ofPushMatrix();
		lineMaterial.begin();

		uint64_t frameNum = ofGetFrameNum();
		int inverseLymphSpeed = 1;
		int j = frameNum / inverseLymphSpeed % (path.size());
		int numSpheres = spheres.size();
		int spacing = path.size() / numSpheres;

		for (int i = 0; i < numSpheres; i++) {
			int index = (j + spacing * i) % path.size();
			light.setPosition(path.at(index));
			spheres.at(i).setPosition(path.at(index));
			spheres.at(i).draw();
		}

		lineMaterial.end();
		ofPopMatrix();


		/*
		model.drawVertices();
		model.drawWireframe();

		mesh1=model.getMesh(0);
		mesh1.drawVertices();
		mesh1.drawWireframe();
		mesh.draw();

		mesh2 = model.getMesh(1);
		mesh2.drawVertices();
		mesh2.drawWireframe();
		mesh2.draw();
		*/

	light.disable();
	cam.end();
	ofDisableDepthTest();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
