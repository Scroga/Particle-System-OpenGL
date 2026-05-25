#pragma once

#include <memory>
#include <vector>
#include <ranges>

#include "vertex.hpp"
#include "scene_object.hpp"
#include "cube.hpp"
#include "instanced_cube.hpp"

#include "sparks_particle_generator.hpp"
#include "snow_particle_generator.hpp"
#include "fire_particle_generator.hpp"

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


inline std::shared_ptr<MeshObject> getFloor(const glm::vec3& color) {
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
																{ "u_material.diffuse", color },
																{ "u_material.specular", glm::vec3(0.3f, 0.3f, 0.3f) },
																{ "u_material.shininess", 48.0f }
												}
								)
				);
				return plane;
}

inline SimpleScene createSnowScene(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) {
				SimpleScene scene;

				auto floor = getFloor(glm::vec3(0.3f, 0.4f, 0.5f));
				floor->prepareRenderData(aMaterialFactory, aGeometryFactory);
				scene.addObject(floor);

				float spawnPosY = 17.0f;
				float floorPosY = floor->getPosition().y;
				float range = 15.0f;
				float size = 0.5f;
				std::size_t maxParticleCount = 5000;
				float maxLifeTime = 12.0f;
				float spawnRate = 150.0f;
				std::vector<fs::path> texturePaths = {
								"resources/textures/particles/snow/01.png",
								"resources/textures/particles/snow/02.png" };

				auto particleGenerator = std::make_shared<SnowParticleGenerator>(
								spawnPosY,
								floorPosY,
								range,
								size,
								maxParticleCount,
								maxLifeTime,
								spawnRate,
								texturePaths);

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
																{ "u_material.shininess", 64.0f },
																{ "u_textures", particleTextures }
												}
								)
				);

				particleGenerator->prepareRenderData(aMaterialFactory, aGeometryFactory);

				scene.addObject(particleGenerator);

				return scene;
}

inline SimpleScene createSparksScene(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) {
				SimpleScene scene;

				auto floor = getFloor(glm::vec3(0.3f, 0.4f, 0.5f));
				floor->prepareRenderData(aMaterialFactory, aGeometryFactory);
				scene.addObject(floor);

				float floorPosY = floor->getPosition().y;
				glm::vec3 spawnPos{ 0.0f, floorPosY, 0.0f };
				glm::vec3 shootDir{ 1.0f, 1.0f, 0.0f };
				float spread = 0.35f;
				float minSpeed = 4.0f;
				float maxSpeed = 8.0f;
				float size = 0.08f;
				std::size_t maxParticleCount = 1500;
				float maxLifeTime = 4.0f;
				float spawnRate = 200.0f;
				std::vector<fs::path> texturePaths = {
								"resources/textures/particles/sparks/01.png",
								"resources/textures/particles/sparks/02.png",
								"resources/textures/particles/sparks/03.png",
								"resources/textures/particles/sparks/04.png",
								"resources/textures/particles/sparks/05.png" };

				auto particleGenerator = std::make_shared<SparksParticleGenerator>(
								spawnPos,
								shootDir,
								floorPosY,
								spread,
								minSpeed,
								maxSpeed,
								size,
								maxParticleCount,
								maxLifeTime,
								spawnRate,
								texturePaths);

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
																{ "u_material.shininess", 64.0f },
																{ "u_textures", particleTextures }
												}
								)
				);

				particleGenerator->prepareRenderData(aMaterialFactory, aGeometryFactory);

				scene.addObject(particleGenerator);

				return scene;
}

inline SimpleScene createFireScene(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) {
				SimpleScene scene;

				auto floor = getFloor(glm::vec3(0.3f, 0.4f, 0.5f));
				floor->prepareRenderData(aMaterialFactory, aGeometryFactory);
				scene.addObject(floor);

				float upAcceleration = 0.2f;
				float animationSpeed = 15.0f;
				float spawnRadius = 1.2f;
				float centerPull = 0.3f;
				float horizontalDamping = 1.4f;
				const glm::vec3& center{ 0.0f, -2.7f, 0.0f };
				std::size_t maxParticleCount = 1000;
				float maxLifeTime = 3.0f;
				float spawnRate = 80.0f;
				std::vector<fs::path> texturePaths = {
								"resources/textures/particles/fire/01.png",
								"resources/textures/particles/fire/02.png",
								"resources/textures/particles/fire/03.png",
								"resources/textures/particles/fire/04.png",
								"resources/textures/particles/fire/05.png",
								"resources/textures/particles/fire/06.png",
								"resources/textures/particles/fire/07.png",
								"resources/textures/particles/fire/08.png",
								"resources/textures/particles/fire/09.png" };

				auto particleGenerator = std::make_shared<FireParticleGenerator>(
								upAcceleration,
								animationSpeed,
								spawnRadius,
								centerPull,
								horizontalDamping,
								center,
								maxParticleCount,
								maxLifeTime,
								spawnRate,
								texturePaths);

				TextureInfo particleTextures;
				particleTextures.textureData = particleGenerator->getTextureArray();

				particleGenerator->addMaterial(
								"solid",
								MaterialParameters(
												"particle",
												RenderStyle::Solid,
												{
																{ "u_material.diffuse", glm::vec3(1.0f, 0.6f, 0.3f) },
																{ "u_material.specular", glm::vec3(1.0f, 0.6f, 0.3f) },
																{ "u_material.shininess", 12.0f },
																{ "u_textures", particleTextures }
												}
								)
				);

				particleGenerator->prepareRenderData(aMaterialFactory, aGeometryFactory);

				scene.addObject(particleGenerator);

				return scene;
}
