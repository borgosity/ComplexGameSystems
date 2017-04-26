#pragma once
#include <string>
#include "glm/glm.hpp"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

protected:
	glm::vec3	m_position;
	std::string m_name;
};

