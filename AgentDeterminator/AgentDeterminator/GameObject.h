#pragma once
#include <string>
#include "glm/glm.hpp"

class GameObject
{
public:
	GameObject();
	GameObject(std::string a_name, glm::vec3 a_position);
	virtual ~GameObject();

	std::string name() { return m_name; };
	glm::vec3 position() { return m_position; };
	glm::vec3 position(glm::vec3 a_postion) { m_position = a_postion;  return m_position; };

protected:
	glm::vec3	m_position;
	std::string m_name;
};

