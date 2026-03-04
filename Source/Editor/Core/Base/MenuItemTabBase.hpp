#pragma once
#include <string>

namespace CLX
{
	class MainMenuTabBase
	{
	public:

		MainMenuTabBase(const std::string& name, const std::string& imGuiTag)
			: mName(name)
			, mImGuiTag(imGuiTag)
		{
			mImGuiName = mName + "##" + mImGuiTag;
		}

		virtual ~MainMenuTabBase() = default;
		virtual void Init() {};
		virtual void Render() = 0;

	protected:

		std::string mName;
		std::string mImGuiTag;
		std::string mImGuiName;
	};
}

namespace CLX
{
	template <typename T>
	concept DerivedFromMainMenuTabBase = std::is_base_of_v<MainMenuTabBase, T>&& std::is_class_v<T>;
}