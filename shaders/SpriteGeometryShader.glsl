#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices=4) out;

in vec2 UV[];

out vec2 fragUV;

void main()
{
   vec2 colors[4];
   
   colors[0] = vec2(UV[0].x, UV[1].y);
   colors[1] = UV[1];
   colors[2] = UV[0];
   colors[3] = vec2(UV[1].x, UV[0].y);
   
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
   
   // Emit the vertices of the quad
   for(int i = 0; i < 4; i ++) {
      fragUV = colors[i];
      gl_Position = vec4(vert[i].xyz, 1);
      EmitVertex();
   }
   EndPrimitive();
}