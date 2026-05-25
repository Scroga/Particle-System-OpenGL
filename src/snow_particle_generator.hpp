#pragma once

#include "particle_generator_base.hpp"

#include <glm/glm.hpp>

class SnowParticleGenerator : public ParticleGeneratorBase {
private:
				const float mGravity = -0.4f;
				const float mFloorFriction = 0.2f;

				float mSpawnPosY;
				float mRange;
				float mFloorY;
				float mSize;

				void updateParticle(ParticleState& state, float aDeltaTime) override {
								state.life -= aDeltaTime;
								if (state.life <= 0.0f) {
												state.isAlive = false;
												return;
								}
								state.velocity += state.acceleration * aDeltaTime;
								state.position += state.velocity * aDeltaTime;

								if (state.position.y < mFloorY) {
												state.position.y = mFloorY;

												state.velocity.x *= mFloorFriction;
												state.velocity.y = 0.0f;
												state.velocity.z *= mFloorFriction;
								}
				}

				ParticleAttributes makeAttributes(const ParticleState& state, float aDeltaTime) const override {
								ParticleAttributes attributes;

								float t = state.life / mMaxLiftTime;
								t = glm::clamp(t, 0.0f, 1.0f);

								attributes.position = state.position;
								attributes.color = glm::vec4(1.0f, 1.0f, 1.0f, t);
								attributes.size = mSize * t;
								attributes.textureIndex = state.textureIndex;

								return attributes;
				}

				ParticleState createParticle() override {
								ParticleState state;

								state.isAlive = true;
								state.life = mMaxLiftTime;
								state.position = glm::vec3(
												randomNumber<float>(-mRange, mRange),
												mSpawnPosY,
												randomNumber<float>(-mRange, mRange)
								);
								state.textureIndex = randomNumber<int>(0, mTexturesCount - 1);
								state.velocity = randomVec3(-1.0, 1.0);

								state.acceleration = glm::vec3(0.0f, mGravity, 0.0f);

								return state;
				}

public:
				SnowParticleGenerator(
								float aSpawnPosY,
								float aFloorY,
								float aRange,
								float aSize,
								std::size_t aMaxParticles,
								double aMaxLifeTime,
								double aSpawnRate,
								const std::vector<fs::path>& aTexturePaths)
								: ParticleGeneratorBase(
												aMaxParticles,
												aMaxLifeTime,
												aSpawnRate,
												aTexturePaths)
								, mSpawnPosY(aSpawnPosY)
								, mFloorY(aFloorY)
								, mRange(aRange)
								, mSize(aSize)
				{
				}
};