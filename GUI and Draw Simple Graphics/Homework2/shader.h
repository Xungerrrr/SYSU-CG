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
	// 程序ID
	unsigned int shaderProgram;
	// 编译和链接
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 使用
	void use();
};

#endif