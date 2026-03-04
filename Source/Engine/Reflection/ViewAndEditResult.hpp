#pragma once

namespace CLX
{
	struct ViewAndEditResult final
	{
		bool isActive = false;
		bool isEdited = false;

		friend constexpr ViewAndEditResult& operator|=(ViewAndEditResult& a, const ViewAndEditResult other) noexcept
		{
			a.isActive |= other.isActive;
			a.isEdited |= other.isEdited;
			return a;
		}
	};
}