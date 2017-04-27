#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::GameObject(std::string a_name = "GameObject", glm::vec3 a_position = glm::vec3(0.0f,0.0f,0.0f))
{
	std::string m_name = a_name;
	glm::vec3 m_position = a_position;
}


GameObject::~GameObject()
{
}
