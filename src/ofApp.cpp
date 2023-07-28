#include "ofApp.h"
#include <vector>


//std::vector <ofSpherePrimitive> spheres;
Trunk tree;
Branch branch;




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

	//BRANCHES
	int circleNumPoints = 20;
	int numCircles = 30;
	int height = 400;
	int radius = 50;
	int numBranches = 3;


	tree.addTrunk(0, 0, 0, 0, 0, 0, circleNumPoints, radius, height, numCircles, 1.2, 1.2);
	branch.addBranch(tree, 10, 0.7, 0.8, 1.2);
	branch.addBranch(tree, 20, 1.7, 3, 10);


	//old  branching tech

	//for (int i = 1; i <= numBranches; i++) {
	//	float branchHeightPos = ofMap(i, 1, numBranches, 0.3, 0.8); //0-1
	//	float branchHeight = height * ofMap(branchHeightPos, 0, 1, 0.7, 0.1);
	//	float branchRadius = radius * ofMap(branchHeightPos, 0, 1, 0.7, 0.1);
	//	float index = int(circleNumPoints * numCircles * branchHeightPos) - int(ofRandom(circleNumPoints));
	//	glm::vec3 pos = mesh.getVertex(index);
	//	glm::vec3 normalXZ;
	//	for (int j = 1; j <= circleNumPoints; j++) {
	//		normalXZ = normalXZ + pos - mesh.getVertex(index+j);
	//	}
	//	float branchXrotation;
	//	float branchZrotation;
	//	if (normalXZ.x < 0) { branchXrotation = ofRandom(PI/4, PI/2); }
	//	else { branchXrotation = ofRandom(-PI / 4, -PI / 2); }
	//	if (normalXZ.z < 0) { branchZrotation = ofRandom(PI / 4, PI / 2); }
	//	else { branchZrotation = ofRandom(-PI / 4, -PI / 2); }
	//	trunk(pos.x, pos.y, pos.z, branchXrotation, 0, branchZrotation, circleNumPoints, branchRadius, branchHeight, numCircles, 1, 1, mesh);
	//}


	

	//LIGHT
	light.setPosition(0, 600, 0);
	light.setPointLight();
	light.setAttenuation(0.1f, 0.0f, 0.0f);
	ofSetSmoothLighting(true);
	light.setDiffuseColor(ofFloatColor(1, 1, .65));
	light.setSpecularColor(ofFloatColor(1.f, 1.f, 1.f));


	//CAM
	cam.setPosition(glm::vec3(400, 400 / 2, 400));
	cam.setTarget(glm::vec3(0, 400 / 2, 0));
	cam.setNearClip(0.1);
	cam.setFarClip(50000);


	//MATERIALS
	lineMaterial.setSpecularColor(ofFloatColor(0.99, 0.99, 0.99));
	lineMaterial.setDiffuseColor(ofFloatColor(0.839, 0.671, 0.024, 1));
	lineMaterial.setAmbientColor(ofFloatColor(0.839, 0.671, 0.024, 1));
	lineMaterial.setEmissiveColor(ofFloatColor(0.839, 0.671, 0.024, 1));

	meshMaterial.setAmbientColor(ofFloatColor(0.6, 0.6, 0.6));
	meshMaterial.setDiffuseColor(ofFloatColor(0.99, 0.99, 0.99));
	//meshMaterial.setEmissiveColor(ofFloatColor(0.99, 0.99, 0.99));

	//SPHERES LINE
	//ofSetSphereResolution(10);
	//int numSpheres = 100;
	//for (int i = 0; i < numSpheres; i++) {
	//	spheres.push_back(ofSpherePrimitive());
	//	spheres.at(i).setRadius(1.f+7.f/(numSpheres)*i);
	//}

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

	
		ofPushMatrix();
		ofSetLineWidth(1);
		ofRotateY(ofGetElapsedTimef() * 4);
		meshMaterial.begin();
		tree.draw();
		meshMaterial.end();
		//mesh.drawFaces();
		ofPopMatrix();
	

		//LYMPH
		/*ofPushMatrix();
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
		ofPopMatrix();*/

		//model.drawVertices();
		//model.drawWireframe();
		//ofPushMatrix();
		//meshMaterial.begin();
		//	mesh1=model.getMesh(0);
		//	mesh1.drawVertices();
		//	mesh1.drawWireframe();
		//	mesh.draw();
		//	mesh2 = model.getMesh(1);
		//	mesh2.drawVertices();
		//	mesh2.drawWireframe();
		//	mesh2.draw();
		//meshMaterial.end();
		//ofPopMatrix();
	


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
