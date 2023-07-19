#include "ofApp.h"
#include <vector>

std::vector< glm::vec3 > path;


void branch(int x, int y, int z, int xRotation, int yRotation, int zRotation, int circleNumPoints, int initialRadius, int heigth, int numCircles, float curvature, float curveNoiseFreq, ofMesh& mesh, int call) {

	int radiusDelta = -initialRadius / numCircles;
	int curve = curvature * initialRadius;
	float curveNoiseRandSeedx = ofRandom(0, 10);
	float curveNoiseRandSeedz = ofRandom(0, 10);

	//POINT GENERATION AND PATH DETECTION
	for (int i = 0; i < numCircles; i = i + 1) {
		float radius = (initialRadius + i * radiusDelta);
		float offsetx = ofMap(ofNoise(float(i) / numCircles * curveNoiseFreq, curveNoiseRandSeedx), 0, 1, -curve, curve);
		float offsetz = ofMap(ofNoise(float(i) / numCircles * curveNoiseFreq, curveNoiseRandSeedz), 0, 1, -curve, curve);
		float posy = i * heigth / numCircles;
		for (float deg = 0; deg <= circleNumPoints; deg = deg + 1) {
			float posx = (cos(deg * (2 * PI / circleNumPoints)) + ofMap(ofNoise(ofRandom(0, 1)), 0, 1, -1, 1));
			float posz = (sin(deg * (2 * PI / circleNumPoints)) + ofMap(ofNoise(ofRandom(0, 1)), 0, 1, -1, 1));
			float x1 = x + radius * posx - radius / 2 + offsetx;
			float y1 = y + posy;
			float z1 = z + radius * posz - radius / 2 + offsetz;
			//3D ROTATION MATRICES
			float x2 = x1 * (cos(yRotation) * cos(zRotation)) + y1 * (sin(xRotation) * sin(yRotation) * cos(zRotation) - cos(xRotation) * sin(zRotation)) + z1 * (cos(xRotation) * sin(yRotation) * cos(zRotation) + sin(xRotation) * sin(zRotation)); 
			float y2 = x1 * (cos(yRotation) * sin(zRotation)) + y1 * (sin(xRotation) * sin(yRotation) * sin(zRotation) + cos(xRotation) * cos(zRotation)) + z1 * (cos(xRotation) * sin(yRotation) * sin(zRotation) - sin(xRotation) * cos(zRotation));
			float z2 = x1 * (-sin(yRotation)) + y1 * (sin(xRotation) * cos(yRotation)) + z1 * (cos(xRotation) * cos(yRotation));
			mesh.addVertex(ofPoint(x2, y2, z2));
		}
		//PATH
		int resolution = 20;
		glm::vec3 prev = path.at(path.size() - 1);
		float newX = ((x - radius / 2 + offsetx) - prev.x) / resolution;
		float newY = (y + posy - prev.y) / resolution;
		float newZ = (z - radius / 2 + offsetz - prev.z) / resolution;
		for (int i = 0; i < resolution - 1; i++) {
			path.push_back(glm::vec3(prev.x + newX*i, prev.y + newY*i, prev.z + newZ*i));
		}
		path.push_back(glm::vec3(x - radius / 2 + offsetx, y + posy, z - radius / 2 + offsetz));
	}

	//INDEXES CONNECTS POINTS
	for (int i = 0; i < numCircles; i = i + 1) {
		for (int deg = 0; deg < circleNumPoints; deg = deg + 1) {
			mesh.addIndex(deg % circleNumPoints*call + (i * circleNumPoints * call));
			mesh.addIndex((deg + 1) % circleNumPoints * call + (i * circleNumPoints * call));
			mesh.addIndex((deg + 2) % circleNumPoints * call + (i * circleNumPoints * call));

			mesh.addIndex(deg % circleNumPoints * call + (i * circleNumPoints * call));
			mesh.addIndex(deg % circleNumPoints * call + ((i + 1) * circleNumPoints * call));
			mesh.addIndex((deg + 1) % circleNumPoints * call + ((i + 1) * circleNumPoints * call));
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
	//ofSetFrameRate(60);

	//model.loadModel("tree.obj");
	//model.setScale(1, 1, 1);
	path.push_back(glm::vec3(0, 0, 0));
	branch(0, 0, 0, 0, 0, 0, 20, 50, 400, 20, 1.2, 1.2, mesh, 1);
	branch(200, 0, 0, 0, 0, 0, 20, 50, 400, 20, 1.2, 1.2, mesh, 2);
	branch(-200, 0, 0, 0, 0, 0, 20, 50, 400, 20, 1.2, 1.2, mesh, 3);

	mesh.mergeDuplicateVertices(); //safety check

	light.setPosition(0, 400, 0);
	light.setPointLight();
	light.setAttenuation(0.0f, 0.005f, 0.0f);
	//cam.setDistance(700);
	cam.setPosition(glm::vec3(400, 400 / 2, 400));
	cam.setTarget(glm::vec3(0, 400 / 2, 0));
	//cam.setFov(60);
	cam.setNearClip(0.1);
	cam.setFarClip(50000);

	//ofEnableDepthTest();
}
//--------------------------------------------------------------

void ofApp::update(){

	float time = ofGetElapsedTimef();
	light.setPosition(path.at(int(time * 30) % (path.size() - 1)));
	line.addVertex(path.at(int(time*30)%(path.size()-1)));

	if (line.size() > 800) {
		line.getVertices().erase(
			line.getVertices().begin()
		);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	//ofEnableDepthTest();
	light.enable();
	cam.begin();

	//model.drawVertices();
	//model.drawWireframe();

	ofRotateY(ofGetElapsedTimef() * 4);

	ofPushMatrix();
	ofSetColor(255, 255, 255, 255);
	ofSetLineWidth(1);
	//ofNoFill();
	mesh.drawVertices();
	mesh.drawWireframe();
	//mesh.drawFaces();
	ofPopMatrix();
	
	//IMPLEMENT
	//mesh.save();
	ofPushMatrix();

	ofSetColor(255, 255, 180, 255);
	ofSetLineWidth(7);
	line.getSmoothed(1000, 1.0f);
	line.draw();
	ofPopMatrix();

	/*
	mesh1=model.getMesh(0);
	mesh1.drawVertices();
	mesh1.drawWireframe();
	//mesh.draw();

	mesh2 = model.getMesh(1);
	mesh2.drawVertices();
	mesh2.drawWireframe();
	//mesh2.draw();
	*/
	cam.end();
	light.disable();
	//ofDisableDepthTest();
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
