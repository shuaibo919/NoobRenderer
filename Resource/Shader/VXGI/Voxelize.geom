#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 uProjx;
uniform mat4 uProjy;
uniform mat4 uProjz;

in	VertexData{
	vec2 TexCoord;
	vec3 Normal;
}gs_in[];


out FragData{
	vec3 WsPos;
	vec3 Normal;
	vec2 TexCoord;
	flat int axis;
}gs_out;


void main(){

	vec3 edge1 = gl_in[0].gl_Position.xyz-gl_in[1].gl_Position.xyz;
	vec3 edge2 = gl_in[2].gl_Position.xyz-gl_in[0].gl_Position.xyz;
	vec3 normal = abs(cross(edge1, edge2));

	if(normal.x >= normal.y && normal.x >= normal.z)
        gs_out.axis = 1;
    else if (normal.y >= normal.x  && normal.y >= normal.z)
        gs_out.axis = 2;
    else
        gs_out.axis = 3;

    mat4 projectionMatrix = gs_out.axis == 1 ? uProjx : gs_out.axis == 2 ? uProjy : uProjz;

	for(int i = 0;i < 3; i++)
	{
		gs_out.TexCoord = gs_in[i].TexCoord;
		gs_out.Normal = gs_in[i].Normal;
		gs_out.WsPos = gl_in[i].gl_Position.xyz;
		gl_Position = projectionMatrix * gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();

}