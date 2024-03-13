#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

/* Vertex Input */
in VS_OUT{
    vec2 TexCoords;
    vec3 Normal;
} vs_in[];

/* Geometry Output */
out GS_OUT{
	vec3 geoVoxelPos;
    vec3 geoNormal;
    vec2 geoTexCoords;
}gs_out;

/* Uniform Vars */
uniform mat4[3] voxelPVMat;
uniform int voxelResolution;

int GetProjectionAxis();

void main()
{
	int selectedIndex = GetProjectionAxis();
    mat4 voxelMatrix = voxelPVMat[selectedIndex];
	for(int i = 0 ;i < 3; ++i){
		gl_Position = voxelMatrix * gl_in[i].gl_Position;
        gs_out.geoVoxelPos = ((gl_Position + 1.0f) * voxelResolution / 2.0f).xyz;
		gs_out.geoTexCoords = vs_in[i].TexCoords;
		gs_out.geoNormal = vs_in[i].Normal;
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

