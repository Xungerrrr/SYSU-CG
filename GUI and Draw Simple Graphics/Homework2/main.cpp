#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	// 初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "CG_HW2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 600, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		// 第一个三角形
		-1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

		// 第二个三角形
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		// 第三个三角形
		1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		// 线
		-1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		// 点
		-0.75f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.75f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
	};

	// 三角形索引
	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8
	};
	
	// 创建着色器程序
	Shader shader("vertex.shader", "fragment.shader");

	// 初始化ImGui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// 初始化颜色值
	std::vector<ImVec4> colors;
	colors.push_back(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	colors.push_back(ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	colors.push_back(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

	while (!glfwWindowShouldClose(window)) {
		// 处理输入
		processInput(window);

		// 黑色清屏
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 设计ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Color Setting");
		ImGui::ColorEdit3("Left corner color", (float*)&colors[0]);
		ImGui::ColorEdit3("Right corner color", (float*)&colors[1]);
		ImGui::ColorEdit3("Top corner color", (float*)&colors[2]);
		ImGui::End();

		// 更新颜色
		for (int i = 0; i < 3; i++) {
			vertices[6 * i + 3] = colors[i].x;
			vertices[6 * i + 4] = colors[i].y;
			vertices[6 * i + 5] = colors[i].z;
		}

		shader.use();
		unsigned int VBO, VAO, EBO;
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &EBO);

		// 绑定数据
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		// 设置顶点属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		// 绘制
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_LINES, 9, 4);
		glDrawArrays(GL_POINTS, 13, 2);
		glBindVertexArray(0);
		
		// 渲染ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}