#version 430

in VERTEX_DATA
{
	vec3 color;
} fs_in;

out vec4 fragment_color;

void main(){
	fragment_color = vec4(fs_in.color/255.f, 1);
}