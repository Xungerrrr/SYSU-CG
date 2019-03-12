#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	// ����ID
	unsigned int shaderProgram;
	// ���������
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// ʹ��
	void use();
};

#endif