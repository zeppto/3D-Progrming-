#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_UV;

uniform mat4 world;
uniform mat4 lightSpace;

void main()
{
	gl_Position = lightSpace * world * vec4(vertex_position, 1.0);
}
