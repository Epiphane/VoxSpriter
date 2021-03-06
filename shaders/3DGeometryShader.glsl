#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices=4) out;

in vec3 vertexColor[];

out vec3 fragColor;
out vec4 fragCoord;
out float intensity;

const vec3 sundir = normalize(vec3(0.5, 1, 0.25));
const float ambient = 0.8;

uniform mat4 MVP;

void main()
{
   
   // Two input vertices will be the first and last vertex of the quad
   vec4 vert[4];
   vert[0] = gl_in[0].gl_Position;
   vert[3] = gl_in[1].gl_Position;
   
   // Save intensity information from second input vertex
   float baseIntensity = vert[3].w / 127.0;
   vert[3].w = vert[0].w;
   
   // Calculate the middle two vertices of the quad
   vert[1] = vert[0];
   vert[2] = vert[0];
   
   if(vert[0].y == vert[3].y) { // y same
      vert[2].z = vert[3].z;
      vert[1].x = vert[3].x;
   } else { // x or z same
      vert[2].y = vert[3].y;
      vert[1].xz = vert[3].xz;
   }
   
   // Calculate surface normal
   vec3 normal = normalize(cross(vert[0].xyz - vert[1].xyz, vert[1].xyz - vert[2].xyz));
   
   // Surface intensity depends on angle of solar light
   // This is the same for all the fragments, so we do the calculation in the geometry shader
   float sunl = ambient + (1 - ambient) * clamp(dot(normal, sundir), 0, 1);
   intensity = baseIntensity * sunl;
   
   // Emit the vertices of the quad
   for(int i = 0; i < 4; i ++) {
      fragColor = vertexColor[i / 2];
      
      fragCoord = vert[i];
      gl_Position = MVP * vec4(vert[i].xyz, 1);
      EmitVertex();
   }
   EndPrimitive();
}