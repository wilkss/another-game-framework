#pragma once

namespace agf {

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned m_Handle;
	};

}