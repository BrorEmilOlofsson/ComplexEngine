#pragma once
#include "Editor/Core/Base/EditorWindow.hpp"
#include <type_traits>

namespace CLX
{

	class PopUp : public EditorWindow
	{
	public:

		PopUp(const std::string& windowName);

		virtual void SetupBlackboard(Blackboard& blackboard) { blackboard; }
		void Update(const Blackboard& blackboard);
		virtual void Init() {}
		virtual void Render(const Blackboard& blackboard) = 0;
		virtual void OnSceneLoaded(class Scene&) {}
		virtual void OnSceneBeginPlay(class Scene&) {}
		virtual void OnSceneEndPlay(class Scene&) {}

	private:

		virtual void UpdateInternal(const Blackboard& blackboard) { blackboard; };
	};

	template<typename T>
	concept DerivedFromPopUpWindow = std::derived_from<T, PopUp>;
}