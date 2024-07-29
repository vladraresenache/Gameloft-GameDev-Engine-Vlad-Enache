#include "stdafx.h"
#include "Camera.h"


Vector3 v4tov3(Vector4 vect)
{
	Vector3 new_vect(vect.x, vect.y, vect.z);
	return new_vect;
}

Camera::Camera()
{
	position = Vector3(0.0f, 0.0f, -1.0f);
	target = Vector3(0.0f, 0.0f, 0.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);
	moveSpeed = 20.9f;
	rotateSpeed = 0.5f;
	deltaTime = 0.0f;
	nearPlane = 0.001f;
	farPlane = 1000.0f;
	fov = 0.785398f;
	perspectiveMatrix.SetPerspective(fov, ((GLfloat)Globals::screenWidth) / ((GLfloat)Globals::screenHeight), nearPlane, farPlane);
	this->updateWorldView();

}

Camera::Camera(Vector3 pos, Vector3 tar, Vector3 u)
{
	position = pos;
	target = tar;
	up = u;
	moveSpeed = 0.5f;
	rotateSpeed = 0.5f;
	deltaTime = 0.0f;
	nearPlane = 0.2f;
	farPlane = 10.0f;
	fov = 0.785398f;
	this->updateWorldView();
}

void Camera::moveOx(int sens)
{
	Vector3 forward = xAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	this->updateWorldView();
}

void Camera::moveOy(int sens)
{
	Vector3 forward = yAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	this->updateWorldView();
}

void Camera::moveOz(int sens)
{
	Vector3 forward = zAxis * sens;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	this->updateWorldView();
}

void Camera::rotateOx(int sens)
{
	Vector4 localTarget(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	GLfloat angle = sens * rotateSpeed * deltaTime;
	Matrix mRotation = mRotation.SetRotationX(angle);
	Vector4 localUp(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 rotatedLocalUp = localUp * mRotation;
	up = v4tov3(rotatedLocalUp * worldMatrix);
	up = up.Normalize();
	Vector4 rotatedTarget = localTarget * mRotation;
	target = v4tov3(rotatedTarget * worldMatrix);
	this->updateWorldView();
}

void Camera::rotateOy(int sens)
{
	Vector4 localTarget(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	GLfloat angle = sens * rotateSpeed * deltaTime;
	Matrix mRotation = mRotation.SetRotationY(angle);
	Vector4 rotatedTarget = localTarget * mRotation;
	target = v4tov3(rotatedTarget * worldMatrix);
	this->updateWorldView();
}

void Camera::rotateOz(int sens)
{
	Vector4 localUp(0.0f, 1.0f, 0.0f, 0.0f);
	GLfloat angle = sens * rotateSpeed * deltaTime;
	Matrix mRotation = mRotation.SetRotationZ(angle);
	Vector4 rotatedLocalUp = localUp * mRotation;
	up = v4tov3(rotatedLocalUp * worldMatrix);
	up = up.Normalize();
	this->updateWorldView();
}

void Camera::updateAxes()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
}

void Camera::updateWorldView()
{
	this->updateAxes();
	GLfloat r[4][4] = { {xAxis.x, xAxis.y, xAxis.z, 0.0f},
					   {yAxis.x, yAxis.y, yAxis.z, 0.0f },
					   {zAxis.x, zAxis.y, zAxis.z, 0.0f },
					   { 0.0f , 0.0f, 0.0f, 1.0f } };
	Matrix R;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			R.m[i][j] = r[i][j];
	Matrix T;
	T.SetTranslation(position.x, position.y, position.z);
	worldMatrix = R * T;
	T.SetTranslation(-position.x, -position.y, -position.z);
	viewMatrix = T * R;
}

void Camera::setDeltaTime(GLfloat d)
{
	deltaTime = d;
}


