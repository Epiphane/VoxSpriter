#version 330

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec3 color;

out vec3 vertexColor;
out vec4 vertexCoord;

void main(void) {
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = vertex;
   
	// The color of each vertex will be interpolated
	// to produce the color of each fragment
	vertexColor = color;
}