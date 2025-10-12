#pragma once
#include <string>

namespace Simple
{
	class MenuItemBase
	{
	public:
		MenuItemBase(const std::string& name, const std::string& imGuiTag = "##DefaultMenuItemBase")
			: mName(name)
			, mImGuiTag(imGuiTag)
		{
			mImGuiName = mName + mImGuiTag;
		}

		virtual ~MenuItemBase() = default;
		virtual void Render() = 0;

	public:

		void SetImGuiTag(const char* imguiTag)
		{
			mImGuiTag = imguiTag;
			mImGuiName = mName + mImGuiTag;
		}

	public:

		const std::string& GetName() const { return mName; }

	protected:

		std::string mName;
		std::string mImGuiTag;
		std::string mImGuiName;
	};

	template <typename T>
	concept DerivedFromMenuItem = std::derived_from<T, MenuItemBase>;
}