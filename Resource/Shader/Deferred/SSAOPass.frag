#version 460 core
out float FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D mNoise;
uniform mat4 projection;
uniform vec3 samples[64];
uniform vec2 viewportSize;
uniform float far;
uniform float near;
uniform mat4 view;
uniform mat4 invTView;

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 1.0;


float LinearizeDepth(float depth, float pNear, float pFar);

void main()
{
    // Get input for SSAO algorithm
    vec2 noiseScale = vec2(viewportSize.x / 4.0f, viewportSize.y / 4.0f); // tile noise texture over screen based on screen dimensions divided by noise size
    vec3 viewSpaceFragPos = vec3(view * vec4(texture(gPositionDepth, TexCoords).rgb, 1.0f));  // In world space
    vec3 viewSpaceNormal =  normalize(mat3(invTView) * texture(gNormal, TexCoords).rgb);
    vec3 randomVec = texture(mNoise, TexCoords * noiseScale).rgb;
    // Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - viewSpaceNormal * dot(randomVec, viewSpaceNormal));
    vec3 bitangent = cross(viewSpaceNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, viewSpaceNormal);
    // Iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 kSample = TBN * samples[i]; // From tangent to view-space
        kSample = viewSpaceFragPos + kSample * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(kSample, 1.0);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = - LinearizeDepth(texture(gPositionDepth, offset.xy).w, near, far); // Get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(viewSpaceFragPos.z - sampleDepth ));
        occlusion += (sampleDepth >= kSample.z ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = pow(occlusion,2.2);
    // FragColor = texture(gPositionDepth, TexCoords).a; for debug
}

float LinearizeDepth(float depth, float pNear, float pFar)
{
    float z = depth * 2.0 - 1.0; // 回到NDC
    return (2.0 * pNear * pFar) / (pFar + pNear - z * (pFar - pNear));    
}
