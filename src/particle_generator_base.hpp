#pragma once

#include <memory>
#include <vector>
#include <random>

#include "vertex.hpp"
#include "mesh_object.hpp"
#include "ogl_geometry_construction.hpp"
#include "ogl_geometry_factory.hpp"
#include "ogl_material_factory.hpp"

struct ParticleState {
				glm::vec3 position;
				glm::vec3 velocity;
				glm::vec3 acceleration;
				int textureIndex;

				float life;
				float age = 0.0f;
				bool isAlive = false;
};

struct ParticleAttributes {
				glm::vec3 position;
				glm::vec4 color;
				float size;
				int textureIndex;
};

class ParticleGeneratorBase : public MeshObject {
protected:
				std::size_t mMaxParticlesCount;
				std::size_t mTexturesCount;
				std::shared_ptr<OGLGeometry> mGeometry;
				std::shared_ptr<OGLTexture> mTextureArray;

				std::vector<ParticleState> mParticles;

				std::size_t mNextSpawnIndex = 0;

				float mMaxLiftTime = 2.0f; // in seconds
				float mSpawnRate = 50.0f; // particles per second
				float mSpawnAccumulator = 0.0f;

				void setupBuffers() {
								mGeometry->buffer.vbos.push_back(createBuffer()); // one base point
								mGeometry->buffer.vbos.push_back(createBuffer()); // particle instances

								mGeometry->buffer.vertexCount = 1;
								mGeometry->buffer.indexCount = 0;
								mGeometry->buffer.instanceCount = 0;
								mGeometry->buffer.mode = GL_POINTS;

								std::vector<glm::vec3> vertices = {
												glm::vec3(0.0f, 0.0f, 0.0f)
								};

								GL_CHECK(glBindVertexArray(mGeometry->buffer.vao.get()));

								GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mGeometry->buffer.vbos[0].get()));
								GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

								GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));
								GL_CHECK(glEnableVertexAttribArray(0));

								GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mGeometry->buffer.vbos[1].get()));
								GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleAttributes) * mMaxParticlesCount, nullptr, GL_DYNAMIC_DRAW));

								// position
								GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleAttributes), reinterpret_cast<void*>(offsetof(ParticleAttributes, position))));
								GL_CHECK(glEnableVertexAttribArray(1));
								GL_CHECK(glVertexAttribDivisor(1, 1));

								// color
								GL_CHECK(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleAttributes), reinterpret_cast<void*>(offsetof(ParticleAttributes, color))));
								GL_CHECK(glEnableVertexAttribArray(2));
								GL_CHECK(glVertexAttribDivisor(2, 1));

								// size
								GL_CHECK(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleAttributes), reinterpret_cast<void*>(offsetof(ParticleAttributes, size))));
								GL_CHECK(glEnableVertexAttribArray(3));
								GL_CHECK(glVertexAttribDivisor(3, 1));

								// texture index
								GL_CHECK(glVertexAttribIPointer(4, 1, GL_INT, sizeof(ParticleAttributes), reinterpret_cast<void*>(offsetof(ParticleAttributes, textureIndex))));
								GL_CHECK(glEnableVertexAttribArray(4));
								GL_CHECK(glVertexAttribDivisor(4, 1));
				}

				void setupParticlesState() {
								for (std::size_t i = 0; i < mMaxParticlesCount; i++) {
												ParticleState state;

												state.isAlive = false;
												state.life = 0.0F;
												state.position = glm::vec3(0.0f);
												state.velocity = glm::vec3(0.0f);

												mParticles[i] = state;
								}
				}

				glm::vec3 randomVec3(float minValue, float maxValue) {
								static std::random_device rd;
								static std::mt19937 gen(rd());

								std::uniform_real_distribution<float> dist(minValue, maxValue);

								return glm::vec3(
												dist(gen),
												dist(gen),
												dist(gen)
								);
				}

				template<typename T>
				T randomNumber(T minValue, T maxValue) {
								static std::random_device rd;
								static std::mt19937 gen(rd());

								if constexpr (std::is_integral_v<T>) {
												std::uniform_int_distribution<T> dist(minValue, maxValue);
												return dist(gen);
								}
								else if constexpr (std::is_floating_point_v<T>) {
												std::uniform_real_distribution<T> dist(minValue, maxValue);
												return dist(gen);
								}
								else {
												static_assert(std::is_arithmetic_v<T>, "randomNumber requires numeric type");
								}
				}

				void uploadInstanceattributes(const std::vector<ParticleAttributes>& aInstanceAttributes) {
								GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mGeometry->buffer.vbos[1].get()));
								GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ParticleAttributes) * aInstanceAttributes.size(), aInstanceAttributes.data()));
								mGeometry->buffer.instanceCount = static_cast<unsigned>(aInstanceAttributes.size());
				}

				bool findSpawnIndex() {
								for (std::size_t i = 0; i < mMaxParticlesCount; i++) {
												std::size_t spawnIndex = (i + mNextSpawnIndex) % mMaxParticlesCount;
												if (mParticles[spawnIndex].isAlive == false) {
																mNextSpawnIndex = spawnIndex;
																return true;
												}
								}
								return false;
				}

				virtual void updateParticle(ParticleState& state, float aDeltaTime) = 0;
				virtual ParticleAttributes makeAttributes(const ParticleState& state, float aDeltaTime) const = 0;
				virtual ParticleState createParticle() = 0;

public:
				ParticleGeneratorBase(
								std::size_t aMaxParticles,
								float aMaxLifeTime,
								float aSpawnRate,
								const std::vector<fs::path>& aTexturePaths)
								: mMaxParticlesCount(aMaxParticles)
								, mGeometry(std::make_shared<OGLGeometry>(std::move(IndexedBuffer(createVertexArray()))))
								, mTextureArray(std::make_shared<OGLTexture>(createTextureArrayFromDir(aTexturePaths), GL_TEXTURE_2D_ARRAY))
								, mParticles(aMaxParticles)
								, mMaxLiftTime(aMaxLifeTime)
								, mSpawnRate(aSpawnRate)
								, mTexturesCount(aTexturePaths.size())
				{
								setupBuffers();
								setupParticlesState();
				}

				virtual ~ParticleGeneratorBase() = default;

				virtual std::shared_ptr<AGeometry> getGeometry(GeometryFactory& aGeometryFactory, RenderStyle aRenderStyle) override {
								return mGeometry;
				}

				std::shared_ptr< OGLTexture> getTextureArray() {
								return mTextureArray;
				}

				void prepareRenderData(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) override {
								for (auto& mode : mRenderInfos) {
												mode.second.shaderProgram = aMaterialFactory.getShaderProgram(mode.second.materialParams.mMaterialName);
												getTextures(mode.second.materialParams.mParameterValues, aMaterialFactory);
												mode.second.geometry = getGeometry(aGeometryFactory, mode.second.materialParams.mRenderStyle);
								}
				}

				void update(float aDeltaTime) override {
								mSpawnAccumulator += mSpawnRate * aDeltaTime;

								while (mSpawnAccumulator >= 1.0 && findSpawnIndex()) {
												mParticles[mNextSpawnIndex] = createParticle();
												mNextSpawnIndex = (mNextSpawnIndex + 1) % mMaxParticlesCount;
												mSpawnAccumulator -= 1.0;
								}

								std::vector<ParticleAttributes> instanceAttributes;

								for (ParticleState& particle : mParticles) {
												if (!particle.isAlive)
																continue;

												updateParticle(particle, aDeltaTime);

												if (particle.isAlive)
																instanceAttributes.push_back(makeAttributes(particle, aDeltaTime));
								}

								uploadInstanceattributes(instanceAttributes);
				}
};