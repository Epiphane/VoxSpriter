#version 330 core

// Interpolated values from the geometry shaders
in vec3 fragColor;

// Ouput data
out vec4 fragmentColor;

void main()
{
   fragmentColor = vec4(fragColor.x / 255, fragColor.y / 255, fragColor.z / 255, 0.5);
}