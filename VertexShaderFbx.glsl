#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_UV;
layout(location = 3) in vec4 s_vWeights;
layout(location = 4) in ivec4 s_vIDs;

out vec2 vertexUV; //texCoords
out vec3 vertexPos;
out vec3 vertexNormal;
out vec4 weights;
out ivec4 bonesID;


void main()
{
	bonesID = s_vIDs;
	weights = s_vWeights;
	vertexPos = vertex_position;
	vertexUV = vertex_UV;
	vertexNormal = vertex_normal;

	gl_Position = vec4(vertex_position, 1);
}
