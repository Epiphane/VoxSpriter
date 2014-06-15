#version 330 core

// Interpolated values from the geometry shaders
in vec3 fragColor;
in vec4 fragCoord;
in float intensity;

// Ouput data
out vec4 fragmentColor;

const vec4 fogcolor = vec4(0.6, 0.8, 1.0, 1.0);
const float fogdensity = 0.0000003;

void main()
{
   vec4 color;
   if((int(fragCoord.w) & 1) == 1 && fragCoord.w > 0)
      color = vec4(fragColor.x / 255.0, fragColor.y / 255.0, fragColor.z / 255.0, 1);
   else
      color = vec4(fragColor.x / 255.0, fragColor.y / 255.0, fragColor.z / 255.0, 1) * 0.85;
   
   float z = gl_FragCoord.z / gl_FragCoord.w;
   float fog = clamp(exp(-fogdensity * z * z), 0.2, 1);
   
   fragmentColor = intensity * mix(fogcolor, color, fog);
}