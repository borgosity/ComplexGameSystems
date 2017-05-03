#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"

class GameObject
{
public:
	GameObject();
	GameObject(std::string a_name, glm::vec3 a_position);
	virtual ~GameObject();

	virtual void update(float a_dt) = 0;

	// universal functions
	std::string name() { return m_name; };
	virtual glm::vec3 position() { return m_position; };
	virtual glm::vec3 position(glm::vec3 a_position) { m_position = a_position;  return m_position; };
	glm::vec4 colour() { return m_colour; };
	glm::vec4 colour(glm::vec4 a_colour) { m_colour = a_colour;  return m_colour; };

protected:
	std::string m_name;
	glm::vec3	m_position;
	glm::vec4	m_colour;
};

