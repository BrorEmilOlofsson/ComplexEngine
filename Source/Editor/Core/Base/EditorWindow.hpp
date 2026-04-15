#pragma once
#include "Engine/Utility/Blackboard.hpp"
#include <string>

namespace CLX
{

	class Scene;

	class EditorWindow
	{
	public:

		EditorWindow(const std::string& name, const std::string& tag);

		virtual ~EditorWindow() = default;
		virtual void Render(const Blackboard& blackboard) = 0;
		virtual void OnSceneLoaded(Scene&) {}
		virtual void OnSceneBeginPlay(Scene&) {}
		virtual void OnSceneEndPlay(Scene&) {}

	public:

		void SetWindowName(const std::string& newName);
		void SetImGuiTag(const std::string& newTag);

	public:

		bool IsActive() const;
		bool& IsActive();
		bool IsFocused() const;
		bool& IsFocused();

		const std::string& GetWindowName() const;
		const std::string& GetImGuiName() const;

	protected:

		std::string mName;
		std::string mImGuiTag;
		std::string mImGuiName;
		bool mIsActive = true;
		bool mIsFocus = false;
	};
}
