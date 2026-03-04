#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/ECS/ECS.hpp"
#include <thread>

using namespace CLX;

static ECSRegistry CreateRegistry()
{
	return ECSRegistry();
}


TEST_CASE("ECS - Create Entity", "[ECS]")
{
	{
		ECS ecs(CreateRegistry());
		const EntityID entityID = ecs.CreateEntity();
		REQUIRE(entityID.id == 0);
	}

	{
		ECS ecs(CreateRegistry());
		const EntityID entityID1 = ecs.CreateEntity();
		const EntityID entityID2 = ecs.CreateEntity();
		REQUIRE(entityID1.id == 0);
		REQUIRE(entityID2.id == 1);
	}
}

TEST_CASE("ECS - Destroy Entity", "[ECS]")
{
	{
		ECS ecs(CreateRegistry());
		const EntityID entityID = ecs.CreateEntity();
		ecs.DestroyEntity(entityID);
		REQUIRE(entityID.id == 0);
	}

	{
		ECS ecs(CreateRegistry());
		const EntityID entityID1 = ecs.CreateEntity();
		const EntityID entityID2 = ecs.CreateEntity();

		ecs.DestroyEntity(entityID1);
		const EntityID entityID3 = ecs.CreateEntity();

		REQUIRE(entityID1.id == 0);
		REQUIRE(entityID2.id == 1);
		REQUIRE(entityID3.id == 0);
	}

	{
		ECS ecs(CreateRegistry());
		const EntityID entityID1 = ecs.CreateEntity();
		const EntityID entityID2 = ecs.CreateEntity();

		ecs.DestroyEntity(entityID1);
		const EntityID entityID3 = ecs.CreateEntity();

		ecs.DestroyEntity(entityID3);

		REQUIRE(ecs.GetEntityCount() == 1);
	}
}

TEST_CASE("ECS - Component Pool Copy", "[ECS]")
{
	ComponentPoolSBO c(std::type_identity<unsigned int>{});

	c.ResizeComponentIndices(10);

	for (unsigned int i = 0; i < 10; i++)
	{
		c.AddComponent<unsigned int>(EntityID{ i }, &i);
	}


	ComponentPoolSBO c1 = c;

	for (unsigned int i = 0; i < 10; i++)
	{
		unsigned int* a = c1.GetComponent<unsigned int>(EntityID{ i });
		REQUIRE(*a == i);
	}

}