#pragma once
#include <vector>
#include <span>
#include "Utility/DrawPrimitives/DrawPrimitives.hpp"
#include "Graphics/Model/ModelInstance.hpp"
#include "Graphics/Light/PointLight.hpp"
#include "Graphics/Sprite/Sprite2D.hpp"
#include "Graphics/Sprite/Sprite3D.hpp"
#include "Graphics/Text/Text3D.hpp"

namespace Simple
{

	class RenderList final
	{
	public:

		constexpr RenderList() = default;

		[[nodiscard]] constexpr std::span<const DrawLine> GetLines() const noexcept;
		[[nodiscard]] constexpr std::span<const DrawSphere> GetWireSpheres() const noexcept;
		[[nodiscard]] constexpr std::span<const DrawBoundingBox> GetWireBoundingBoxes() const noexcept;
		[[nodiscard]] constexpr std::span<const DrawPyramid> GetPyramids() const noexcept;
		[[nodiscard]] constexpr std::span<const DrawCylinder> GetCylinders() const noexcept;
		[[nodiscard]] constexpr std::span<const DrawArrow> GetArrows() const noexcept;
		[[nodiscard]] constexpr std::span<const DrawPlane> GetPlanes() const noexcept;
		[[nodiscard]] constexpr std::span<const DrawSphere> GetSpheres() const noexcept;
		[[nodiscard]] constexpr std::span<const DrawBoundingBox> GetBoundingBoxes() const noexcept;
		[[nodiscard]] constexpr std::span<const Sprite2D> GetSprite2Ds() const noexcept;
		[[nodiscard]] constexpr std::span<const Sprite3D> GetSprite3Ds() const noexcept;
		[[nodiscard]] constexpr std::span<const Text3D> GetText3Ds() const noexcept;
		[[nodiscard]] constexpr std::span<const PointLight> GetPointLights() const noexcept;
		[[nodiscard]] constexpr std::span<const ModelInstance> GetModelInstances() const noexcept;

		constexpr void AddLine(const DrawLine& line);
		constexpr void AddLines(const std::span<const DrawLine> lines);
		constexpr void AddSphere(const DrawSphere& sphere, const bool wired = true);
		constexpr void AddBoundingBox(const DrawBoundingBox& boundingBox, const bool wired = true);
		constexpr void AddPlane(const Planef& plane, const Color& color);
		constexpr void AddArrow(const DrawArrow& arrow);
		constexpr void AddCylinder(const Cylinderf& cylinder, const Color& color);
		constexpr void AddSprite(const Sprite2D& sprite);
		constexpr void AddSprite(const Sprite3D& sprite);
		constexpr void AddText(Text3D&& text);
		constexpr void AddText(const Text3D& text);
		constexpr void AddTexts(std::span<const Text3D> texts);
		constexpr void AddModelInstance(const ModelInstance& modelInstance);
		constexpr void AddPointLight(const PointLight& pointLight);

		constexpr void Clear() noexcept;

	private:

		std::vector<ModelInstance> mStaticModelsToRender;
		std::vector<DrawLine> mDebugLines;
		std::vector<DrawSphere> mWireSpheres;
		std::vector<DrawBoundingBox> mWireBoundingBoxes;
		std::vector<DrawArrow> mDebugArrows;
		std::vector<DrawPyramid> mDebugPyramids;
		std::vector<DrawCylinder> mDebugCylinders;
		std::vector<DrawPlane> mPlanes;
		std::vector<DrawSphere> mSpheres;
		std::vector<DrawBoundingBox> mBoundingBoxes;
		std::vector<Sprite2D> mSprite2Ds;
		std::vector<Sprite3D> mSprite3Ds;
		std::vector<Text3D> mText3Ds;
		std::vector<PointLight> mPointLights;
	};

	constexpr std::span<const DrawLine> RenderList::GetLines() const noexcept
	{
		return std::span<const DrawLine>(mDebugLines);
	}

	constexpr std::span<const DrawSphere> RenderList::GetWireSpheres() const noexcept
	{
		return std::span<const DrawSphere>(mWireSpheres);
	}

	constexpr std::span<const DrawBoundingBox> RenderList::GetWireBoundingBoxes() const noexcept
	{
		return std::span<const DrawBoundingBox>(mWireBoundingBoxes);
	}

	constexpr std::span<const DrawCylinder> RenderList::GetCylinders() const noexcept
	{
		return std::span<const DrawCylinder>(mDebugCylinders);
	}

	constexpr std::span<const DrawPyramid> RenderList::GetPyramids() const noexcept
	{
		return std::span<const DrawPyramid>(mDebugPyramids);
	}

	constexpr std::span<const DrawArrow> RenderList::GetArrows() const noexcept
	{
		return std::span<const DrawArrow>(mDebugArrows);
	}

	constexpr std::span<const DrawPlane> RenderList::GetPlanes() const noexcept
	{
		return std::span<const DrawPlane>(mPlanes);
	}

	constexpr std::span<const DrawSphere> RenderList::GetSpheres() const noexcept
	{
		return std::span<const DrawSphere>(mSpheres);
	}

	constexpr std::span<const DrawBoundingBox> RenderList::GetBoundingBoxes() const noexcept
	{
		return std::span<const DrawBoundingBox>(mBoundingBoxes);
	}

	constexpr std::span<const Sprite2D> RenderList::GetSprite2Ds() const noexcept
	{
		return std::span<const Sprite2D>(mSprite2Ds);
	}

	constexpr std::span<const Sprite3D> RenderList::GetSprite3Ds() const noexcept
	{
		return std::span<const Sprite3D>(mSprite3Ds);
	}

	constexpr std::span<const Text3D> RenderList::GetText3Ds() const noexcept
	{
		return std::span<const Text3D>(mText3Ds);
	}

	constexpr std::span<const PointLight> RenderList::GetPointLights() const noexcept
	{
		return std::span(mPointLights);
	}

	constexpr std::span<const ModelInstance> RenderList::GetModelInstances() const noexcept
	{
		return std::span<const ModelInstance>(mStaticModelsToRender);
	}

	constexpr void RenderList::AddLine(const DrawLine& line)
	{
		mDebugLines.push_back(line);
	}

	constexpr void RenderList::AddLines(const std::span<const DrawLine> lines)
	{
		mDebugLines.insert(end(mDebugLines), begin(lines), end(lines));
	}

	constexpr void RenderList::AddSphere(const DrawSphere& sphere, const bool wired)
	{
		if (wired)
		{
			mWireSpheres.push_back(sphere);
		}
		else
		{
			mSpheres.push_back(sphere);
		}
	}

	constexpr void RenderList::AddBoundingBox(const DrawBoundingBox& boundingBox, const bool wired)
	{
		if (wired)
		{
			mWireBoundingBoxes.push_back(boundingBox);
		}
		else
		{
			mBoundingBoxes.push_back(boundingBox);
		}
	}

	constexpr void RenderList::AddPlane(const Planef& plane, const Color& color)
	{
		mPlanes.push_back(DrawPlane{ plane, color });
	}

	constexpr void RenderList::AddArrow(const DrawArrow& arrow)
	{
		mDebugArrows.push_back(arrow);
	}

	constexpr void RenderList::AddCylinder(const Cylinderf& cylinder, const Color& color)
	{
		mDebugCylinders.push_back(DrawCylinder{ cylinder, color });
	}

	constexpr void RenderList::AddSprite(const Sprite2D& aSprite)
	{
		mSprite2Ds.push_back(aSprite);
	}

	constexpr void RenderList::AddSprite(const Sprite3D& sprite)
	{
		mSprite3Ds.push_back(sprite);
	}

	constexpr void RenderList::AddText(Text3D&& text)
	{
		mText3Ds.push_back(std::move(text));
	}

	constexpr void RenderList::AddText(const Text3D& text)
	{
		mText3Ds.push_back(text);
	}

	constexpr void RenderList::AddTexts(std::span<const Text3D> texts)
	{
		mText3Ds.insert(mText3Ds.end(), texts.begin(), texts.end());
	}

	constexpr void RenderList::AddModelInstance(const ModelInstance& meshInstance)
	{
		mStaticModelsToRender.push_back(meshInstance);
	}

	constexpr void RenderList::AddPointLight(const PointLight& pointLight)
	{
		mPointLights.push_back(pointLight);
	}

	constexpr void RenderList::Clear() noexcept
	{
		mStaticModelsToRender.clear();
		mDebugLines.clear();
		mWireSpheres.clear();
		mWireBoundingBoxes.clear();
		mDebugArrows.clear();
		mDebugPyramids.clear();
		mDebugCylinders.clear();
		mPlanes.clear();
		mSpheres.clear();
		mBoundingBoxes.clear();
		mSprite2Ds.clear();
		mSprite3Ds.clear();
		mText3Ds.clear();
		mPointLights.clear();
	}
}