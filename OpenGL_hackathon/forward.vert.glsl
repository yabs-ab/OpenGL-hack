#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out VERTEX_DATA
{
	vec3 color;
} vs_out;

layout(location = 2) uniform mat4 world;

void main(){
	gl_Position = world * vec4(position, 1);
	vs_out.color = color;
}