#include "EntityManager.h"

void EntityManager::reset()
{
	_generation.clear();
	_free_indices.clear();
}
