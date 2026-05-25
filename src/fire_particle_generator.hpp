#pragma once

#include "particle_generator_base.hpp"

#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>

class FireParticleGenerator : public ParticleGeneratorBase {
private:
				const float mUpAcceleration = 0.2f;
				
				float mAnimationSpeed;
				float mSpawnRadius;
				float mCenterPull;
				float mHorizontalDamping;
				glm::vec3 mCenter;

				void updateParticle(ParticleState& state, float aDeltaTime) override {
								state.life -= aDeltaTime;
								state.age += aDeltaTime;
								
								if (state.life <= 0.0f) {
												state.isAlive = false;
												return;
								}

								glm::vec3 toCenter = mCenter - state.position;
								toCenter.y = 0.0f;

								glm::vec3 centerAcceleration = toCenter * mCenterPull;

								glm::vec3 acceleration = state.acceleration + centerAcceleration;

								state.velocity += acceleration * aDeltaTime;
								state.velocity.x *= glm::max(0.0f, 1.0f - mHorizontalDamping * aDeltaTime);
								state.velocity.z *= glm::max(0.0f, 1.0f - mHorizontalDamping * aDeltaTime);

								state.position += state.velocity * aDeltaTime;
				}

				ParticleAttributes makeAttributes(const ParticleState& state, float aDeltaTime) const override {
								ParticleAttributes attributes;

								float lifeT = state.life / mMaxLiftTime;
								lifeT = glm::clamp(lifeT, 0.0f, 1.0f);

								int textureIndex =
												static_cast<int>(state.age * mAnimationSpeed) %
												static_cast<int>(mTexturesCount);

								glm::vec3 toCenter = state.position - mCenter;
								toCenter.y *= 0.3f;

								float distanceToCenter = glm::length(toCenter);

								float centerT = 1.0f - distanceToCenter / mSpawnRadius;
								centerT = glm::clamp(centerT, 0.0f, 1.0f);

								glm::vec3 outerColor = glm::vec3(1.0f, 0.5f, 0.3f);
								glm::vec3 innerColor = glm::vec3(1.0f, 1.0f, 1.0f);

								glm::vec3 finalColor = glm::mix(outerColor, innerColor, centerT);

								attributes.position = state.position;
								attributes.color = glm::vec4(finalColor, lifeT);
								attributes.size = centerT;
								attributes.textureIndex = textureIndex;

								return attributes;
				}

				ParticleState createParticle() override {
								ParticleState state;

								state.isAlive = true;
								state.life = mMaxLiftTime;
								state.age = 0.0f;

								float angle = randomNumber<float>(0.0f, glm::two_pi<float>());
								float radius = std::sqrt(randomNumber<float>(0.0f, 1.0f)) * mSpawnRadius;

								state.position = mCenter + glm::vec3(
												std::cos(angle) * radius,
												0.0f,
												std::sin(angle) * radius
								);

								state.textureIndex = randomNumber<int>(0, static_cast<int>(mTexturesCount) - 1);;
								state.velocity = glm::vec3(
												randomNumber<float>(-0.3f, 0.3f),
												randomNumber<float>(0.5f, 1.5f),
												randomNumber<float>(-0.3f, 0.3f)
								);

								state.acceleration = glm::vec3(0.0f, mUpAcceleration, 0.0f);

								return state;
				}

public:
				FireParticleGenerator(
								float aUpAcceleration,
								float aAnimationSpeed,
								float aSpawnRadius,
								float aCenterPull,
								float aHorizontalDamping,
								const glm::vec3& aCenter,
								std::size_t aMaxParticles,
								double aMaxLifeTime,
								double aSpawnRate,
								const std::vector<fs::path>& aTexturePaths)
								: ParticleGeneratorBase(
												aMaxParticles,
												aMaxLifeTime,
												aSpawnRate,
												aTexturePaths)
								, mUpAcceleration(aUpAcceleration)
								, mAnimationSpeed(aAnimationSpeed)
								, mSpawnRadius(aSpawnRadius)
								, mCenterPull(aCenterPull)
								, mHorizontalDamping(aHorizontalDamping)
								, mCenter(aCenter)
				{
				}
};