#include "Engine/Precompiled/EnginePch.hpp"
#include "SimpleNodeScript.hpp"
#include "NodeScript/Fly.hpp"
#include "NodeScriptDataTypes.hpp"
#include <set>

SimpleNodeScript::SimpleNodeScript()
{
}

SimpleNodeScript::~SimpleNodeScript()
{
	Fly::DestroyFoundation();
}

void SimpleNodeScript::Init()
{
	RegisterFundamentalDataTypes();
	Fly::InitializeFoundation();

	Fly::LoadAllFlyFiles(ASSET_FILE_PATH);
}
