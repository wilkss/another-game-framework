#pragma once

namespace agf {

	class Entity;

	class Component
	{
	public:
		virtual ~Component()
		{
		}

		inline void SetParent(Entity* parent) { m_Parent = parent; }
		inline Entity* GetParent() const { return m_Parent; }

	protected:
		Entity* m_Parent;
	};

}