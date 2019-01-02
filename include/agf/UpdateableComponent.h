#pragma once

#include "agf/Component.h"

namespace agf {

	class Camera;

	class UpdateableComponent : public Component
	{
	public:
		virtual void Update(Camera* camera) = 0;
	};

}