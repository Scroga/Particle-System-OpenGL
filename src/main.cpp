#include <iostream>
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <array>

#include "ogl_resource.hpp"
#include "error_handling.hpp"
#include "window.hpp"
#include "shader.hpp"

#include "scene_definition.hpp"
#include "renderer.hpp"
#include "point_light.hpp"
#include "first_person_camera.hpp"

#include "ogl_geometry_factory.hpp"
#include "ogl_material_factory.hpp"

#include <glm/gtx/string_cast.hpp>

void toggle(const std::string& aToggleName, bool& aToggleValue) {

				aToggleValue = !aToggleValue;
				std::cout << aToggleName << ": " << (aToggleValue ? "ON\n" : "OFF\n");
}

void printInfo() {
				std::cout
								<< "\n=== Controls ===\n"
								<< "Camera movement:\n"
								<< "  W        - move forward\n"
								<< "  S        - move backward\n"
								<< "  A        - move left\n"
								<< "  D        - move right\n"
								<< "  Q        - move down\n"
								<< "  E        - move up\n"
								<< "  Mouse    - rotate camera\n"
								<< "\n"
								<< "Camera:\n"
								<< "  R        - reset camera position and rotation\n"
								<< "\n"
								<< "Scenes:\n"
								<< "  1       - snow scene\n"
								<< "  2       - sparks scene\n"
								<< "  3       - fire scene\n"
								<< "\n"
								<< "Info:\n"
								<< "  I        - print this info\n"
								<< "================\n\n";
}


struct Config {
				int currentSceneIdx = 0;
				bool showSolid = true;
				bool showWireframe = false;
				bool showNormals = false;
};

int main() {
				// Initialize GLFW
				if (!glfwInit()) {
								std::cerr << "Failed to initialize GLFW" << std::endl;
								return -1;
				}

				const float cameraNear = 0.01f;
				const float cameraFar = 500.0f;
				const glm::vec3 cameraPosition{ 0.0f, 0.0f, 3.0f };
				const float cameraSpeed{ 0.05f };

				try {
								Config config;

								auto window = Window(1080, 720, "Particle system");
								MouseTracking mouseTracking;
								FirstPersonCamera camera(window.aspectRatio(), cameraPosition, cameraNear, cameraFar);
								camera.setSpeed(cameraSpeed);

								window.lockCursor();

								window.onResize([&camera, &window](int width, int height) {
												camera.setAspectRatio(window.aspectRatio());
												});

								window.onCheckInput([&camera, &mouseTracking](GLFWwindow* aWin) {
												mouseTracking.update(aWin);

												if (glfwGetKey(aWin, GLFW_KEY_W) == GLFW_PRESS)
																camera.processKeyboard(FORWARD);
												if (glfwGetKey(aWin, GLFW_KEY_S) == GLFW_PRESS)
																camera.processKeyboard(BACKWARD);
												if (glfwGetKey(aWin, GLFW_KEY_A) == GLFW_PRESS)
																camera.processKeyboard(LEFT);
												if (glfwGetKey(aWin, GLFW_KEY_D) == GLFW_PRESS)
																camera.processKeyboard(RIGHT);
												if (glfwGetKey(aWin, GLFW_KEY_Q) == GLFW_PRESS)
																camera.processKeyboard(DOWN);
												if (glfwGetKey(aWin, GLFW_KEY_E) == GLFW_PRESS)
																camera.processKeyboard(UP);

												auto off = mouseTracking.offset();
												camera.processMouseMovement(off.x, off.y);
												});


								window.setKeyCallback([&config, &camera, &cameraPosition](GLFWwindow* aWin, int key, int scancode, int action, int mods) {
												if (action == GLFW_PRESS) {
																switch (key) {
																case GLFW_KEY_R:
																				camera.setPosition(cameraPosition);
																				camera.resetRotation();
																				break;
																case GLFW_KEY_I:
																				printInfo();
																				break;
																case GLFW_KEY_1:
																				config.currentSceneIdx = 0;
																				break;
																case GLFW_KEY_2:
																				config.currentSceneIdx = 1;
																				break;
																case GLFW_KEY_3:
																				config.currentSceneIdx = 2;
																				break;
																case GLFW_KEY_4:
																				config.currentSceneIdx = 3;
																				break;
																}
												}
												});

								OGLMaterialFactory materialFactory;
								materialFactory.loadShadersFromDir("./shaders/");
								materialFactory.loadTexturesFromDir("./resources/textures/");

								OGLGeometryFactory geometryFactory;

								PointLight pointLight{
												{ 0.0f, 2.0f, 0.0f }, // position
												{ 1.0f, 1.0f, 1.0f }, // ambient
												{ 1.0f, 1.0f, 1.0f }, // diffuse
												{ 1.0f, 1.0f, 1.0f }, // specular
												{ 1.0f },  // constant: makes the whole light weaker
												{ 0.05f }, // linear: reduces light gradually with distance
												{ 0.032f }, // quadratic: reduces light much faster as distance grows
								};

								std::array<SimpleScene, 3> scenes{
									createSnowScene(materialFactory, geometryFactory),
									createSparksScene(materialFactory, geometryFactory),
									createFireScene(materialFactory, geometryFactory)
								};

								Renderer renderer(materialFactory);

								double previousTime = glfwGetTime();

								renderer.initialize();
								window.runLoop([&] {
												double currentTime = glfwGetTime();
												double deltaTime = currentTime - previousTime;
												previousTime = currentTime;

												scenes[config.currentSceneIdx].update(static_cast<float>(deltaTime));

												renderer.clear();
												renderer.renderScene(scenes[config.currentSceneIdx], camera, pointLight);
												});
				}
				catch (ShaderCompilationError& exc) {
								std::cerr
												<< "Shader compilation error!\n"
												<< "Shader type: " << exc.shaderTypeName()
												<< "\nError: " << exc.what() << "\n";
								return -3;
				}
				catch (OpenGLError& exc) {
								std::cerr << "OpenGL error: " << exc.what() << "\n";
								return -2;
				}
				catch (std::exception& exc) {
								std::cerr << "Error: " << exc.what() << "\n";
								return -1;
				}

				glfwTerminate();
				return 0;
}
