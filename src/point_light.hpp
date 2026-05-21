#pragma once
#include <vector>
#include "vertex.hpp"

struct PointLight {
				glm::vec3 position;

				glm::vec3 ambient;
				glm::vec3 diffuse;
				glm::vec3 specular;

				float constant;
				float linear;
				float quadratic;

				glm::vec3 movementCenter = glm::vec3(0.0f, 1.0f, -2.0f);
				float r = 4.0f;
				float angularSpeed = 1.0f;

				PointLight(
								glm::vec3 pos,
								glm::vec3 a,
								glm::vec3 d,
								glm::vec3 s,
								float c,
								float l,
								float q)
								: position(pos)
								, ambient(a)
								, diffuse(d)
								, specular(s)
								, constant(c)
								, linear(l)
								, quadratic(q) {}

				const glm::vec3& getPosition() const { return position; }
				const glm::vec3& getAmbient() const { return ambient; }
				const glm::vec3& getDiffuse() const { return diffuse; }
				const glm::vec3& getSpecular() const { return specular; }
				float getConstant() const { return constant; }
				float getLinear() const { return linear; }
				float getQuadratic() const { return quadratic; }

};