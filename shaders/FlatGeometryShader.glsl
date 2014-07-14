#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices=4) out;

in vec3 vertexColor[];

out vec3 fragColor;

uniform mat4 MVP;
uniform float hue;

void main()
{
   vec3 colors[4];
   colors[0] = vec3(0, 0, 0);
   colors[1] = vec3(0, 0, 0);
   colors[2] = vec3(255, 255, 255);
   colors[3] = vec3(0, 0, 0);
   
   /*int i;
   float f, p, q, t;
   int v;
   v = 255;
   float h = hue / 60;			// sector 0 to 5
	i = int(floor( h ));
	f = h - i;			// factorial posture of h
   for(int v = 0; v <= 255; v += 255) {
   for(int s = 0; s <= 1; s ++) {
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			colors[v * 2 / 255 + s].r = v;
			colors[v * 2 / 255 + s].g = t;
			colors[v * 2 / 255 + s].b = p;
			break;
		case 1:
			colors[v * 2 / 255 + s].r = q;
			colors[v * 2 / 255 + s].g = v;
			colors[v * 2 / 255 + s].b = p;
			break;
		case 2:
			colors[v * 2 / 255 + s].r = p;
			colors[v * 2 / 255 + s].g = v;
			colors[v * 2 / 255 + s].b = t;
			break;
		case 3:
			colors[v * 2 / 255 + s].r = p;
			colors[v * 2 / 255 + s].g = q;
			colors[v * 2 / 255 + s].b = v;
			break;
		case 4:
			colors[v * 2 / 255 + s].r = t;
			colors[v * 2 / 255 + s].g = p;
			colors[v * 2 / 255 + s].b = v;
			break;
		default:		// case 5:
			colors[v * 2 / 255 + s].r = v;
			colors[v * 2 / 255 + s].g = p;
			colors[v * 2 / 255 + s].b = q;
			break;
	}
   }
   }*/
   
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
      fragColor = vertexColor[0];
      gl_Position = MVP * vec4(vert[i].xyz, 1);
      EmitVertex();
   }
   EndPrimitive();
}