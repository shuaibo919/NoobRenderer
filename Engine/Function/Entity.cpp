#include "Engine/Function/Entity.h"
using namespace NoobRenderer;

void Entity::Destroy()
{
    m_registry->destroy(m_entity);
}

bool Entity::Valid()
{
    return m_registry && m_registry->valid(m_entity);
}

bool Entity::IsActive()
{
    if (HasComponent<component::ActiveComponent>())
        return m_registry->get<component::ActiveComponent>(m_entity).active;

    return true;
}
void Entity::SetActive(bool active)
{
    GetOrAddComponent<component::ActiveComponent>().active = active;
}
