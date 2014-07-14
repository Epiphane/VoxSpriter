#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

out vec4 fragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform float hue;

vec3 hsv2rgb(vec3 c)
{
   vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
   vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
   return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(){
   vec4 tex = texture( myTextureSampler, UV );
   vec3 rgb = hsv2rgb(vec3(hue, 1.0, 1.0));
   
   if(hue == -1) {
      fragColor = tex;
	}
   else {
      fragColor = vec4(tex.r * tex.a + rgb.r * (1 - tex.a), tex.g * tex.a + rgb.g * (1 - tex.a), tex.b * tex.a + rgb.b * (1 - tex.a), 1);
   }
	
}