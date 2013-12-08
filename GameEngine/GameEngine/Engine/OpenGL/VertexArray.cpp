#include "VertexArray.hh"

namespace OpenGLTools
{
	VertexArray::VertexArray()
		: _withIndex(false)
	{
		glGenVertexArrays(1, &_id);
	}

	VertexArray::~VertexArray()
	{
	}

	VertexArray::VertexArray(VertexArray const &copy)
		: _withIndex(copy._withIndex),
		_attributes(copy._attributes),
		_data(copy._data),
		_indices(copy._indices),
		_id(copy._id)
	{
	}

	VertexArray &VertexArray::operator=(VertexArray const &vertexarray)
	{
		_withIndex = vertexarray._withIndex;
		_attributes = vertexarray._attributes;
		_data = vertexarray._data;
		_indices = vertexarray._indices;
		_id = vertexarray._id;
		return (*this);
	}

	VertexArray::Attribute::Attribute(size_t nbrElement,
										unsigned char nbrComponent,
										unsigned char nbrByte)
		: nbrElement(nbrElement),
		nbrComponent(nbrComponent),
		nbrByte(nbrByte)
	{
	}

	VertexArray::Attribute::~Attribute()
	{
	}

	VertexArray::Attribute::Attribute(Attribute const &attribute)
		: nbrElement(attribute.nbrElement),
		nbrComponent(attribute.nbrComponent),
		nbrByte(attribute.nbrByte)
	{
	}

	VertexArray::Attribute &VertexArray::Attribute::operator=(VertexArray::Attribute const &attribute)
	{
		nbrElement = attribute.nbrElement;
		nbrComponent = attribute.nbrComponent;
		nbrByte = attribute.nbrByte;
		return (*this);
	}

	void VertexArray::unload()
	{
		glDeleteVertexArrays(1, &_id);
	}

	void VertexArray::addAttribute(size_t nbrElement, unsigned char nbrComponent, unsigned char nbrByte, char *data)
	{
		glBindVertexArray(_id);
		_attributes.push_back(Attribute(nbrElement, nbrComponent, nbrByte));
		_data.pushBuffer(data, nbrElement * nbrComponent * nbrByte);
		glBindVertexArray(0);
	}

	VertexBuffer const &VertexArray::getIndices() const
	{
		return (_indices);
	}

	VertexBuffer const &VertexArray::getData() const
	{
		return (_data);
	}

	void VertexArray::setIndices(size_t nbrElement, unsigned int *buffer)
	{
		_withIndex = true;
		_indices.clearBuffer();
		_indices.pushBuffer(reinterpret_cast<char *>(buffer), nbrElement);
	}

	void VertexArray::setData(VertexBuffer const &vertexbuffer)
	{
		_data = vertexbuffer;
	}

	void VertexArray::clearIndices()
	{
		_indices.clearBuffer();
	}

	void VertexArray::clearData()
	{
		_data.clearBuffer();
	}

	void VertexArray::draw(GLenum draw) const
	{
		glBindVertexArray(_id);
		/*draw*/
		glBindVertexArray(0);
	}

	void VertexArray::transferGPU() const
	{
		/*read the attribute for send to GPU*/
	}
}