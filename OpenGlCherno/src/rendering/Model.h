#ifndef _MODEL
#define _MODEL

#include "buffers/VertexBuffer.h"
#include "VertexArray.h"
#include "buffers/IndexBuffer.h"

#include <string>


/*
* Purpose: handle for OpenGL's VertexBuffer and IndexBuffer
* @author Nicholas Baron
*/
class Model {
private:
	bool is3D;
	//The factor to multiply by to get all vectors normalized
	float normalFactor;

	unsigned VERTEX_BUFFER_COUNT;
	float* vertex_buffer;

	unsigned INDICE_COUNT;
	unsigned* indicies;

	VertexBuffer* vb;
	VertexArray* va;
	IndexBuffer* ib;

	inline void cleanData();
	inline void genBuffers(bool textured = false);

public:
	static constexpr auto EMPTY_MODEL_SOURCE = "NONE";

	Model(const std::string& file = EMPTY_MODEL_SOURCE, bool is3D = false);
	~Model();

	inline VertexArray* vertexArray() const { return va; }
	inline IndexBuffer* indexBuffer() const { return ib; }
	//Scale by this to get all vertecies at most 1 magnitude.
	inline float scaleFactor() const { return normalFactor; }
	inline bool hasDepth() const { return is3D; }

	bool loadModel(const std::string& file);
	bool loadSquare(float sideLength, bool textured = false);
};

#endif // !_MODEL
