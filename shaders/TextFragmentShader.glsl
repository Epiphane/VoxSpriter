#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

out vec4 fragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

	fragColor = texture( myTextureSampler, UV );
	
	
}