#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <cmath>
#include <vector>
#include "shader.h"

using std::vector;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void addPoint(float, float);
void deletePoint();
void calculateBezier();
void getVisualizePoints(float t);
float bernstein(int i, int n, float t);
vector<float> controlPoints;
vector<float> bezierPoints;
vector<float> visualizePoints;

int windowWidth = 1280;
int windowHeight = 720;

float lastX = windowWidth / 2, lastY = windowHeight / 2;

int main() {
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "CG_HW8", nullptr, nullptr);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// 创建着色器程序
	Shader shader("vertex.shader", "fragment.shader");

	unsigned int pointVAO, pointVBO, bezierVAO, bezierVBO, visualizeVAO, visualizeVBO;

	// 初始化ImGui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window)) {
		// 处理输入
		processInput(window);

		// 清屏
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float time = (float)glfwGetTime();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		shader.use();
		
		if (!controlPoints.empty()) {
			glViewport(0, 0, windowWidth, windowHeight);
			glGenBuffers(1, &pointVBO);
			glGenVertexArrays(1, &pointVAO);
			glBindVertexArray(pointVAO);
			glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * controlPoints.size(), &controlPoints[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glPointSize(10);
			glDrawArrays(GL_POINTS, 0, controlPoints.size() / 6);
			for (int i = 0; i < controlPoints.size() / 6 - 1; i++) {
				glDrawArrays(GL_LINE_STRIP, i, 2);
			}
			glBindVertexArray(0);
		}

		if (!bezierPoints.empty()) {
			glGenBuffers(1, &bezierVBO);
			glGenVertexArrays(1, &bezierVAO);
			glBindVertexArray(bezierVAO);
			glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)* bezierPoints.size(), &bezierPoints[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glPointSize(1);
			glDrawArrays(GL_POINTS, 0, bezierPoints.size() / 6);
			glBindVertexArray(0);
		}

		float t = (float)(time - (int)time / 10 * 10) / 10;
		getVisualizePoints(t);
		
		if (!visualizePoints.empty()) {
			glGenBuffers(1, &visualizeVBO);
			glGenVertexArrays(1, &visualizeVAO);
			glBindVertexArray(visualizeVAO);
			glBindBuffer(GL_ARRAY_BUFFER, visualizeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)* visualizePoints.size(), &visualizePoints[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			int n = controlPoints.size() / 6 - 1;
			float offset = n + 1;
			for (int i = 0; i < n - 1; i++) {
				for (int j = 0; j < n - i - 1; j++) {
					glDrawArrays(GL_LINE_STRIP, j + offset, 2);
				}
				offset += (n - i);
			}
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, visualizePoints.size() / 6);
			glBindVertexArray(0);
		}
	
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

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	lastX = xpos;
	lastY = ypos;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		float clickX = (lastX - windowWidth / 2) / (windowWidth / 2);
		float clickY = -(lastY - windowHeight / 2) / (windowHeight / 2);
		switch (button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				addPoint(clickX, clickY);
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				deletePoint();
				break;
			default:
				return;
		}
	}
}

void addPoint(float x, float y) {
	std::vector<float> newPoint = { x, y, 0, 1, 1, 1};
	controlPoints.insert(controlPoints.end(), newPoint.begin(), newPoint.end());
	calculateBezier();
}

void deletePoint() {
	if (!controlPoints.empty()) {
		controlPoints.erase(controlPoints.begin() + controlPoints.size() - 6, controlPoints.end());
		calculateBezier();
	}
}

void calculateBezier() {
	bezierPoints.clear();
	int n = controlPoints.size() / 6 - 1;
	if (n > 1) {
		for (float t = 0; t <= 1; t += 0.0001) {
			float x = 0, y = 0;
			for (int i = 0; i <= n; i++) {
				float b = bernstein(i, n, t);
				x += controlPoints[i * 6] * b;
				y += controlPoints[i * 6 + 1] * b;
			}
			std::vector<float> newPoint = { x, y, 0, 1, 1, 1 };
			bezierPoints.insert(bezierPoints.end(), newPoint.begin(), newPoint.end());			
		}
	}
}

float bernstein(int i, int n, float t) {
	float result = 1;
	for (int j = 0; j < i; j++) {
		result *= ((float)(n - j) / (float)(i - j));
	}
	result *= pow(t, i) * pow(1 - t, n - i);
	return result;
}

void getVisualizePoints(float t) {
	visualizePoints = vector<float>(controlPoints);
	int n = controlPoints.size() / 6 - 1;
	int offset = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - i; j++) {
			float x = (1 - t) * visualizePoints[(j + offset) * 6] + t * visualizePoints[(j + 1 + offset) * 6];
			float y = (1 - t) * visualizePoints[(j + offset) * 6 + 1] + t * visualizePoints[(j + 1 + offset) * 6 + 1];
			std::vector<float> newPoint = { x, y, 0, 1, 1, 1 };
			visualizePoints.insert(visualizePoints.end(), newPoint.begin(), newPoint.end());
		}
		offset += (n - i + 1);
	}
}