#pragma once
#include "stdfx.h"

struct BufferDesc
{
	BufferDesc() :numOfEle(0), stride(0), bufferSize(0), data(nullptr)
	{

	}
	BufferDesc(const BufferDesc& desc):numOfEle(desc.numOfEle),stride(desc.stride)
	{
		bufferSize = numOfEle * stride;
		data = desc.data;
	}
	BufferDesc& operator=(const BufferDesc& desc)
	{
		numOfEle = desc.numOfEle;
		stride = desc.stride;
		bufferSize = desc.bufferSize;
		data = desc.data;
	}
	~BufferDesc() {}

	size_t numOfEle;
	size_t stride;
	size_t bufferSize;
	void* data;
};

class Buffer
{
public:
	friend class RenderContext;
	Buffer(const BufferDesc& desc) :m_desc(desc)
	{
		m_data = new unsigned char[desc.bufferSize];
		std::memcpy(m_data, desc.data, desc.bufferSize);
	}
	~Buffer()
	{
		if (m_data != nullptr)
		{
			delete m_data;
		}
	}

	size_t getNumOfEle()
	{
		return m_desc.numOfEle;
	}

	void* getData()
	{
		return m_data;
	}

private:
	void* m_data;
	BufferDesc m_desc;
};
