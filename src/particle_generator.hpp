#pragma once

#include <vector>
#include "vertex.hpp"
#include "mesh_object.hpp"
#include "ogl_geometry_construction.hpp"
#include "ogl_geometry_factory.hpp"

struct ParticleAttributes {
				glm::vec3 position;
				glm::vec4 color;
				float size;
};

class ParticleGenerator : public MeshObject {
private:
				std::size_t mMaxParticlesCount;
				std::shared_ptr<OGLGeometry> mGeometry;

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

								GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleAttributes), reinterpret_cast<void*>(offsetof(ParticleAttributes, position))));
								GL_CHECK(glEnableVertexAttribArray(1));
								GL_CHECK(glVertexAttribDivisor(1, 1));

								GL_CHECK(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleAttributes), reinterpret_cast<void*>(offsetof(ParticleAttributes, color))));
								GL_CHECK(glEnableVertexAttribArray(2));
								GL_CHECK(glVertexAttribDivisor(2, 1));

								GL_CHECK(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleAttributes), reinterpret_cast<void*>(offsetof(ParticleAttributes, size))));
								GL_CHECK(glEnableVertexAttribArray(3));
								GL_CHECK(glVertexAttribDivisor(3, 1));
				}

				void updateInstanceattributes(const std::vector<ParticleAttributes>& instanceAttributes) {
								GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mGeometry->buffer.vbos[1].get()));
								GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ParticleAttributes) * instanceAttributes.size(), instanceAttributes.data()));
								mGeometry->buffer.instanceCount = static_cast<unsigned>(instanceAttributes.size());
				}

public:
				ParticleGenerator(std::size_t aMaxParticles) : mMaxParticlesCount(aMaxParticles) {
								IndexedBuffer buffer{ createVertexArray() };
								mGeometry = std::make_shared<OGLGeometry>(std::move(buffer));

								setupBuffers();
				}

				virtual std::shared_ptr<AGeometry> getGeometry(GeometryFactory& aGeometryFactory, RenderStyle aRenderStyle) override {
								return mGeometry;
				}

				void prepareRenderData(MaterialFactory& aMaterialFactory, GeometryFactory& aGeometryFactory) override {
								for (auto& mode : mRenderInfos) {
												mode.second.shaderProgram = aMaterialFactory.getShaderProgram(mode.second.materialParams.mMaterialName);
												getTextures(mode.second.materialParams.mParameterValues, aMaterialFactory);
												mode.second.geometry = getGeometry(aGeometryFactory, mode.second.materialParams.mRenderStyle);
								}
				}

				void update(double aDeltaTime) override {
								std::vector<ParticleAttributes> particles;

								particles.push_back({
												glm::vec3(-1.0f, 0.0f, 0.0f),      
												glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // red
												0.25f                              
												});

								particles.push_back({
												glm::vec3(0.0f, 0.0f, 0.0f),
												glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // green
												0.35f
												});

								particles.push_back({
												glm::vec3(1.0f, 0.0f, 0.0f),
												glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // blue
												0.45f
												});


								updateInstanceattributes(particles);
				}

};