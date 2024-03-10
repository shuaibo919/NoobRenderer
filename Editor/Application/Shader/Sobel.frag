#version 430 core
uniform sampler2D	texture;
uniform sampler2D	finalRT;
uniform float 		width;
uniform float 		height;
uniform vec3        edgeColor;
out vec4 color;
in vec2 TexCoords;

void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)
{
	float w = 3.0 / width;
	float h = 3.0 / height;

	n[0] = texture2D(tex, clamp(coord + vec2( -w, -h),0.0001f,0.9998f));
	n[1] = texture2D(tex, clamp(coord + vec2(0.0, -h),0.0001f,0.9998f));
	n[2] = texture2D(tex, clamp(coord + vec2(  w, -h),0.0001f,0.9998f));
	n[3] = texture2D(tex, clamp(coord + vec2( -w, 0.0),0.0001f,0.9998f));
	n[4] = texture2D(tex, clamp(coord,0.0001f,1.0f));
	n[5] = texture2D(tex, clamp(coord + vec2(  w, 0.0),0.0001f,0.9998f));
	n[6] = texture2D(tex, clamp(coord + vec2( -w, h),0.0001f,0.9998f));
	n[7] = texture2D(tex, clamp(coord + vec2(0.0, h),0.0001f,0.9998f));
	n[8] = texture2D(tex, clamp(coord + vec2(  w, h),0.0001f,0.9998f));
}

void main(void) 
{
	vec4 n[9];
	make_kernel( n, texture, TexCoords);

	vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

	color = texture2D(finalRT,TexCoords) + vec4( sobel.rgb * edgeColor, 1.0 );
}
