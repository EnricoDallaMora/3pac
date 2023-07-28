#pragma once
#include "ofMain.h"
#include <vector>

class Trunk
{
private:
	float x;
	float y;
	float z;
	float xRotation;
	float yRotation;
	float zRotation;
	int circleNumPoints;
	float initialRadius;
	float height;
	int numCircles;
	float curvature;
	float curveNoiseFreq;
	ofMesh mesh;

public:
	Trunk::Trunk();
	
	void Trunk::addTrunk(float x, float y, float z, float xRotation, float yRotation, float zRotation, int circleNumPoints, float initialRadius, float heigth, int numCircles, float curvature, float curveNoiseFreq);
	void Trunk::buildMesh();
	void Trunk::draw();


	float Trunk::get_x();
	float Trunk::get_y();
	float Trunk::get_z();
	float Trunk::get_xRotation();
	float Trunk::get_yRotation();
	float Trunk::get_zRotation();
	int Trunk::get_circleNumPoints();
	float Trunk::get_initialRadius();
	float Trunk::get_height();
	int Trunk::get_numCircles();
	float Trunk::get_curvature();
	float Trunk::get_curveNoiseFreq();
	ofMesh& Trunk::get_mesh();

	void Trunk::set_x(float x);
	void Trunk::set_y(float y);
	void Trunk::set_z(float z);
	void Trunk::set_xRotation(float xRotation);
	void Trunk::set_yRotation(float yRotation);
	void Trunk::set_zRotation(float zRotation);
	void Trunk::set_circleNumPoints(int circleNumPoints);
	void Trunk::set_initialRadius(float initialRadius);
	void Trunk::set_height(float height);
	void Trunk::set_numCircles(int numCircles);
	void Trunk::set_curvature(float curvature);
	void Trunk::set_curveNoiseFreq(float curveNoiseFreq);

	std::vector< glm::vec3 > centersPath;
};
