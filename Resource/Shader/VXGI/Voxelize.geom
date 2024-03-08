#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

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
uniform mat4 voxelProjection;
uniform int voxelSize;

vec4 ApplyProjection(vec4 pos, int axis);

void main()
{
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;

    vec3 norm = abs(cross(p1 - p0, p2 - p0));
    int major_axis = 0;
    if (norm.y > norm.x){
        major_axis = 1;
    }
    if(norm.z > norm.y && norm.z > norm.x){
        major_axis = 2;
    }

    for(int i = 0; i < 3; i++)
    {
        vec4 temp0 = voxelProjection * gl_in[i].gl_Position;
        temp0.z = -temp0.z;
        gs_out.geoVoxelPos = ((temp0 + 1.0f) * 0.5f * voxelSize).xyz;
        gs_out.geoNormal = vs_in[i].Normal;
        gs_out.geoTexCoords = vs_in[i].TexCoords;
        gl_Position = voxelProjection * projection(gl_in[i].gl_Position, major_axis);
        EmitVertex();
    }
    EndPrimitive();
}

vec4 ApplyProjection(vec4 pos, int axis)
{
    vec4 temp = pos;
    // temp.z = -pos.z;
    if (axis == 0){
        //return pos.zyxw;
        return vec4(-pos.z, pos.yxw);
    }else if (axis == 1){
        // return pos.xzyw;
        return vec4(pos.x, -pos.z, pos.yw);
    }else if (axis == 2){
        // return pos;
        return vec4(pos.xy, -pos.z, pos.w);
    }else{
        
    }
}