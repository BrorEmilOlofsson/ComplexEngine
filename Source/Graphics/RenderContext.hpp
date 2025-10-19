#pragma once

namespace Simple
{

	class RenderContext
	{
	public:

		RenderContext() = default;
		virtual ~RenderContext() = default;

		virtual void* GetNormalSRV();

	};
}