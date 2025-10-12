#include <External/Catch2/catch_amalgamated.hpp>


#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"

using namespace Simple;

template<std::size_t I>
struct FakeComponent
{
	std::array<Vector3f, I> is;
};

TEST_CASE("Benchmarking 2")
{
	ECSRegistry registry;

	registry.RegisterComponentType<TransformComponent>();
	registry.RegisterComponentType<TransformHierarchyComponent>();
	registry.RegisterComponentType<NameComponent>();
	registry.RegisterComponentType<FakeComponent<0>>();
	registry.RegisterComponentType<FakeComponent<1>>();
	registry.RegisterComponentType<FakeComponent<2>>();
	registry.RegisterComponentType<FakeComponent<3>>();
	registry.RegisterComponentType<FakeComponent<4>>();
	registry.RegisterComponentType<FakeComponent<5>>();
	registry.RegisterComponentType<FakeComponent<6>>();
	registry.RegisterComponentType<FakeComponent<7>>();
	registry.RegisterComponentType<FakeComponent<8>>();
	registry.RegisterComponentType<FakeComponent<9>>();
	registry.RegisterComponentType<FakeComponent<10>>();
	registry.RegisterComponentType<FakeComponent<11>>();
	registry.RegisterComponentType<FakeComponent<12>>();
	registry.RegisterComponentType<FakeComponent<13>>();
	ECS ecs(registry);

	for (std::size_t i = 0; i < 100000; i++)
	{
		EntityID entityID = ecs.CreateEntity();
		ecs.AddComponent<TransformComponent>(entityID);
		ecs.AddComponent<NameComponent>(entityID).name = "Entityffffffffff";
		ecs.AddComponent<TransformHierarchyComponent>(entityID).children = std::vector{ EntityID(4), EntityID(1), EntityID(0), EntityID(5) };
		if (i % 13 == 0)
		{
			ecs.AddComponent<FakeComponent<0>>(entityID);
		}
		else if (i % 13 == 1)
		{
			ecs.AddComponent<FakeComponent<1>>(entityID);
		}
		else if (i % 13 == 2)
		{
			ecs.AddComponent<FakeComponent<2>>(entityID);
		}
		else if (i % 13 == 3)
		{
			ecs.AddComponent<FakeComponent<3>>(entityID);
		}
		else if (i % 13 == 4)
		{
			ecs.AddComponent<FakeComponent<4>>(entityID);
		}
		else if (i % 13 == 5)
		{
			ecs.AddComponent<FakeComponent<5>>(entityID);
		}
		else if (i % 13 == 6)
		{
			ecs.AddComponent<FakeComponent<6>>(entityID);
		}
		else if (i % 13 == 7)
		{
			ecs.AddComponent<FakeComponent<7>>(entityID);
		}
		else if (i % 13 == 8)
		{
			ecs.AddComponent<FakeComponent<8>>(entityID);
		}
		else if (i % 13 == 9)
		{
			ecs.AddComponent<FakeComponent<9>>(entityID);
		}
		else if (i % 13 == 10)
		{
			ecs.AddComponent<FakeComponent<10>>(entityID);
		}
		else if (i % 13 == 11)
		{
			ecs.AddComponent<FakeComponent<11>>(entityID);
		}
	}

	BENCHMARK("Copy ECS")
	{
		ECS ecs2 = ecs;
	};

	BENCHMARK("Move ECS")
	{
		ECS ecs2 = std::move(ecs);
	};
}