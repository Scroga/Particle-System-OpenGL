#pragma once

#include <memory>
#include <vector>
#include <ranges>

#include "vertex.hpp"
#include "scene_object.hpp"
#include "cube.hpp"
#include "instanced_cube.hpp"
#include "particle_generator.hpp"

#include "material_factory.hpp"
#include "geometry_factory.hpp"
#include "simple_scene.hpp"

#include "point_light.hpp"

constexpr unsigned int DIFFUSE = 1;
constexpr unsigned int SPECULAR = 1 << 1;
constexpr unsigned int BUMP = 1 << 2;
constexpr unsigned int PARALLAX = 1 << 3;
constexpr unsigned int AMBIENT_OCC = 1 << 4;
constexpr unsigned int SHADOW = 1 << 5;
constexpr unsigned int DEBUG = 1 << 7;


inline std::shared_ptr<MeshObject> getFloor() {
				auto plane = std::make_shared<LoadedMeshObject>("./resources/geometry/plane.obj");
				plane->setScale(glm::vec3(100.0));
				plane->setPosition(glm::vec3(0.0f, -3.0f, 0.0f));
				plane->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));

				plane->addMaterial(
								"solid",
								MaterialParameters(
												"object_light",
												RenderStyle::Solid,
												{
																{ "u_material.diffuse", glm::vec3(0.5f, 0.6f, 0.7f) },
																{ "u_material.specular", glm::vec3(0.3f, 0.3f, 0.3f) },
																{ "u_material.shininess", 32.0f }
												}
								)
				);
				return plane;
}

inline SimpleScene createInstancedCubesScene(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) {
				SimpleScene scene;
				std::vector<VertexColor> instanceAttributes;
				for (float x = -6.0f; x <= 6.0f; x += 1.5f) {
								for (float y = -6.0f; y <= 6.0f; y += 1.5f) {
												for (float z = -6.0f; z <= 6.0f; z += 1.5f) {
																float red = (((instanceAttributes.size() + 31415) * 325) % 255) / 255.0f;
																float green = (((instanceAttributes.size() + 81812) * 17) % 255) / 255.0f;
																float blue = (((instanceAttributes.size() + 563) * 999) % 255) / 255.0f;
																instanceAttributes.emplace_back(glm::vec3(x, y, z), glm::vec3(red, green, blue));
												}
								}
				}
				auto instancedCube = std::make_shared<InstancedCube>(std::move(instanceAttributes));
				instancedCube->setScale(glm::vec3(0.1, 0.1, 0.1));
				instancedCube->addMaterial(
								"solid",
								MaterialParameters(
												"instanced",
												RenderStyle::Solid,
												{
													{"u_solidColor", glm::vec4(0,0.5,0.7,1)}
												}
								)
				);
				instancedCube->addMaterial(
								"wireframe",
								MaterialParameters(
												"solid_color",
												RenderStyle::Wireframe,
												{}
								)
				);
				instancedCube->prepareRenderData(aMaterialFactory, aGeometryFactory);

				scene.addObject(instancedCube);
				return scene;
}

inline SimpleScene createParticleScene(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) {
				SimpleScene scene;

				std::vector<fs::path> texturePaths = {
								"resources/textures/particles/test/01.png",
								"resources/textures/particles/test/02.png",
								"resources/textures/particles/test/03.png",
								"resources/textures/particles/test/04.png"};

				auto particleGenerator = std::make_shared<ParticleGenerator>(10, texturePaths);

				TextureInfo particleTextures;
				particleTextures.textureData = particleGenerator->getTextureArray();

				particleGenerator->addMaterial(
								"solid",
								MaterialParameters(
												"particle",
												RenderStyle::Solid,
												{
																{ "u_material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f) },
																{ "u_material.specular", glm::vec3(1.0f, 1.0f, 1.0f) },
																{ "u_material.shininess", 32.0f },
																{ "u_textures", particleTextures }
												}
								)
				);

				particleGenerator->prepareRenderData(aMaterialFactory, aGeometryFactory);

				scene.addObject(particleGenerator);

				auto floor = getFloor();
				floor->prepareRenderData(aMaterialFactory, aGeometryFactory);
				scene.addObject(floor);

				return scene;
}