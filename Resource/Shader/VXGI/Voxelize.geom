#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

/* Vertex Input */
in VS_OUT{
    vec3 geoVoxelPos;
    vec2 TexCoords;
    vec3 Normal;
} vs_in[];

/* Geometry Output */
out GS_OUT{
    vec3 geoNormal;
    vec2 geoTexCoords;
}gs_out;

/* Uniform Vars */
uniform mat4[3] voxelPVMat;
uniform int voxelResolution;

int GetProjectionAxis();

void main()
{
	vec3 texCoord[3];
	int selectedIndex = GetProjectionAxis();

    for (int i = 0; i < gl_in.length(); i++)
    {
        texCoord[i] = vs_in[i].texCoord; 
    }
    mat4 voxelMatrix = voxelPVMat[selectedIndex];
	for(int i = 0 ;i < 3; ++i){
		gl_Position = voxelMatrix * gl_in[i].gl_Position;
        gs_out.geoVoxelPos = // todo
		gs_out.geoTexCoords = vs_in[i].TexCoords;
		gs_out.geoNormal = gs_in[i].Normal;
		EmitVertex();
		
	}
	EndPrimitive();
}

int GetProjectionAxis()
{
	vec3 p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 faceNormal = cross(p1, p2);

	float nDX = abs(faceNormal.x);
	float nDY = abs(faceNormal.y);
	float nDZ = abs(faceNormal.z);

	if( nDX > nDY && nDX > nDZ )
    {
		return 0;
	}
	else if( nDY > nDX && nDY > nDZ  )
    {
	    return 1;
    }
	else
    {
	    return 2;
	}
}

