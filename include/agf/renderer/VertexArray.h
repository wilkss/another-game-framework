#pragma once

#include "agf/renderer/VertexBufferLayout.h"

namespace agf {

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void SetBufferLayout(const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;

	private:
		unsigned m_Handle;
	};

}