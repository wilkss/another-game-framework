#pragma once

namespace agf {

	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned* data, unsigned count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned GetCount() const { return m_Count; }

	private:
		unsigned m_Handle;
		unsigned m_Count;
	};

}