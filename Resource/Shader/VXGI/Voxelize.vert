#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;

out VertexData{
    vec2 TexCoord;
    vec3 Normal;
}vs_out;


void main()
{
	gl_Position = model * vec4(aPos,1.0);
	vs_out.TexCoord = aTexCoord;
	vs_out.Normal = normalize(aNormal);
}