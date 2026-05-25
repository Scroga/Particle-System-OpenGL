#pragma once

#include "particle_generator_base.hpp"

#include <glm/glm.hpp>

class SparksParticleGenerator : public ParticleGeneratorBase {
private:
				const float mGravity = -9.8f;
				const float mBounciness = 0.2f;
				const float mFloorFriction = 0.7f;

				glm::vec3 mSpawnPosition;
				glm::vec3 mShootDirection;
				float mFloorY;
				float mSpread; // bigger = wider cone
				float mMinSpeed;
				float mMaxSpeed;
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

												if (state.velocity.y < 0.0f) {
																state.velocity.y = -state.velocity.y * mBounciness;
												}

												state.velocity.x *= mFloorFriction;
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
								state.position = mSpawnPosition;
								state.textureIndex = randomNumber<int>(0, mTexturesCount - 1);

								glm::vec3 randomOffset = randomVec3(-mSpread, mSpread);
								glm::vec3 direction = glm::normalize(mShootDirection + randomOffset);
								float speed = randomNumber<float>(mMinSpeed, mMaxSpeed);

								state.velocity = direction * speed;
								state.acceleration = glm::vec3(0.0f, mGravity, 0.0f);

								return state;
				}
				
public:
				SparksParticleGenerator(
								const glm::vec3& aSpawnPos,
								const glm::vec3& aShootDirection,
								float aFloorY,
								float aSpread,
								float aMinSpeed,
								float aMaxSpeed,
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
								, mSpawnPosition(aSpawnPos)
								, mFloorY(aFloorY)
								, mShootDirection(glm::normalize(aShootDirection))
								, mSpread(aSpread)
								, mMinSpeed(aMinSpeed)
								, mMaxSpeed(aMaxSpeed)
								, mSize(aSize)
				{
				}
};