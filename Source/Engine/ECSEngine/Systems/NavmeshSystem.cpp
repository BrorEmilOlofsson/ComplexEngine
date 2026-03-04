#include "Engine/Precompiled/EnginePch.hpp"
#include "NavmeshSystem.hpp"
#include "Engine/Navmesh/Navmesh.hpp"
#include "Engine/Utility/DebugShapes.hpp"
#include "Engine/Navmesh/NavmeshRenderSettings.hpp"
#include "Engine/Scene/SceneSettings.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Input/InputState.hpp"
#include <External/fmt/core.h>

namespace Simple
{

	void RenderNavmesh(const Navmesh& navmesh, RenderList& renderList, const NavmeshRenderSettings& renderSettings)
	{
		PROFILER_FUNCTION(profiler::colors::Blue300);
		const NavmeshData& navmeshData = navmesh.GetNavmeshData();
		if (renderSettings.renderLines)
		{
			PROFILER_BEGIN("Render Navmesh Lines");
			for (std::size_t i = 0; i < navmeshData.mIndices.size(); i += 3)
			{
				const unsigned int index0 = navmeshData.mIndices[i];
				const unsigned int index1 = navmeshData.mIndices[i + 1];
				const unsigned int index2 = navmeshData.mIndices[i + 2];
				renderList.AddLine(DrawLine{ navmeshData.m3DVertices[index0], navmeshData.m3DVertices[index1], Colors::Red });
				renderList.AddLine(DrawLine{ navmeshData.m3DVertices[index1], navmeshData.m3DVertices[index2], Colors::Blue });
				renderList.AddLine(DrawLine{ navmeshData.m3DVertices[index2], navmeshData.m3DVertices[index0], Colors::LightGreen });
			}
			PROFILER_END();
		}

		if (renderSettings.renderNodeGrid)
		{
			PROFILER_BEGIN("Render Node Grid");

			auto& nodeGrid = navmesh.GetNodeGrid();

			RenderGrid2(nodeGrid.GetMinPosition(), nodeGrid.GetGridSize(), nodeGrid.GetCellSize(), nodeGrid.GetOffset(), -2.f, Colors::CafeLatte, renderList);

			PROFILER_END();
		}

		if (renderSettings.renderNodeIndices)
		{
			PROFILER_BEGIN("Render Navmesh Node Indices");
			const auto& nodes = navmesh.GetNodes();
			for (int i = 0; i < nodes.size(); i++)
			{
				renderList.AddText(Text3D(nodes[i].mCenter3D, std::to_string(i), Colors::DarkGray, 0.5f));
			}
			PROFILER_END();
		}

		if (renderSettings.renderVertexIndices)
		{
			PROFILER_BEGIN("Render Navmesh Vertex Indices");
			for (std::size_t i = 0; i < navmeshData.m3DVertices.size(); ++i)
			{
				renderList.AddText(Text3D(navmeshData.m3DVertices[i], fmt::format("{}", i), Colors::Navy, 0.5f));
			}
			PROFILER_END();
		}
	}

	void DebugFindPath(const Navmesh& navmesh, const InputState& input, RenderList& renderList, const Ray3f& mouseRay)
	{
		static Ray3f startRay;
		static Ray3f endRay;
		static std::optional<NavmeshPositionData> startHit;
		static std::optional<NavmeshPositionData> endHit;
		static std::vector<Point3f> path;
		if (input.IsKeyPressed(eInputKey::LMB))
		{
			startRay = mouseRay;
			startHit = navmesh.Raycast(startRay);
			if (startHit && endHit)
			{
				path = navmesh.FindPath(startHit->m3DPosition, endHit->m3DPosition);
			}
		}
		if (input.IsKeyPressed(eInputKey::RMB))
		{
			endRay = mouseRay;
			endHit = navmesh.Raycast(endRay);

			if (startHit && endHit)
			{
				path = navmesh.FindPath(startHit->m3DPosition, endHit->m3DPosition);
			}
		}
		if (startHit)
		{
			RenderSphere(Spheref::FromCenterAndRadius(startHit->m3DPosition, Radiusf(0.1f)), Colors::SkyBlue, renderList);
		}
		if (endHit)
		{
			RenderSphere(Spheref::FromCenterAndRadius(endHit->m3DPosition, Radiusf(0.1f)), Colors::Navy, renderList);
		}
		if (!path.empty())
		{
			if (startHit)
			{
				RenderLineSegment(startHit->m3DPosition, path[0], Colors::FrenchRose, renderList);
			}
		}
		if (path.size() >= 2)
		{
			for (std::size_t i = 0; i < path.size() - 1; ++i)
			{
				RenderLineSegment(path[i], path[i + 1], Colors::FrenchRose, renderList);
			}
		}
	}

	void NavmeshSystem::Render(const ECS&, [[maybe_unused]] const Blackboard& blackboard)
	{
#ifdef _EDITOR
		if (auto navmeshRef = blackboard.TryGet<Key_Navmesh>())
		{
			const Navmesh& navmesh = *navmeshRef;
			RenderState& renderState = blackboard.Get<Key_SceneRenderState>();
			const SceneSettings& sceneSettings = blackboard.Get<Key_SceneSettings>();
			const Ray3f mouseRay = blackboard.Get<Key_SceneManager>().GetCurrentScene().GetMouseRay();
			RenderNavmesh(navmesh, renderState.GetRenderList(), sceneSettings.navmeshRenderSettings);
			DebugFindPath(navmesh, blackboard.Get<Key_InputState>(), renderState.GetRenderList(), mouseRay);
		}
#endif
	}
}
