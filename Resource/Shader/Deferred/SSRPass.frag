#version 460 core

out vec3 FragColor;
in vec3 viewSpaceFragPos;

const float PI = 3.14159265359;
uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;  
uniform sampler2D uColorBlurMap;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 invTView;
uniform vec2 viewportSize;
uniform float maxRayDistance;

bool RayIsOutofScreen(vec2 ray); 
vec3 GetScreenCoordFromViewSpace(vec3 pViewSpaceVec);

// Todo: Consider the roughness.

vec3 TraceRay(vec3 rayPos, vec3 dir, int iterationCount)
{
	float sampleDepth;
	vec3 hitColor = vec3(0);
	bool hit = false;

	for(int i = 0; i < iterationCount; i++){
		rayPos += dir;
		if(RayIsOutofScreen(rayPos.xy)){
			break;
		}

		sampleDepth =  texture(gPositionDepth, rayPos.xy).a;
		float depthDif = rayPos.z - sampleDepth;
		if(depthDif >= 0 && depthDif < 0.0005){ //we have a hit
			hit = true;
			hitColor =  texture(uColorBlurMap, rayPos.xy).rgb;
			break;
		}
	}
	return hitColor;
}

void main()
{
    // Discard non-relative areas
    vec3 screenSpaceRayStart = GetScreenCoordFromViewSpace(viewSpaceFragPos);
    if(RayIsOutofScreen(screenSpaceRayStart.xy))
    {
        FragColor = vec3(0.f);
		return;
    }
    float viewSpaceDepthDiff = screenSpaceRayStart.z - texture(gPositionDepth, screenSpaceRayStart.xy).a;
    if(viewSpaceDepthDiff >= 0.0001)
    {
        FragColor = vec3(0.f);
		return;
    }

    // View Space ray calculation
    vec3 viewSpaceNormal = normalize(mat3(invTView) * texture(gNormal, screenSpaceRayStart.xy).rgb);	
	vec3 viewSpaceRelfect = normalize(reflect(viewSpaceFragPos, viewSpaceNormal));


    if(viewSpaceRelfect.z > 0){
		FragColor = vec3(0.f);
		return;
	 }
	vec3 viewSpaceRayEnd = viewSpaceFragPos + viewSpaceRelfect * maxRayDistance;

	vec3 screenSpaceRayEnd = GetScreenCoordFromViewSpace(viewSpaceRayEnd);
	vec3 screenSpaceRayDir = screenSpaceRayEnd - screenSpaceRayStart;

	ivec2 screenSpaceStartPosition = ivec2(screenSpaceRayStart.x * viewportSize.x, screenSpaceRayStart.y * viewportSize.y); 
	ivec2 screenSpaceEndPosition = ivec2(screenSpaceRayEnd.x * viewportSize.x, screenSpaceRayEnd.y * viewportSize.y); 
	ivec2 rayDistance = screenSpaceEndPosition - screenSpaceStartPosition;
	int traceCounts = max(abs(rayDistance.x), abs(rayDistance.y));
	screenSpaceRayDir /= max(traceCounts * 2 /5, 0.001f);

	// trace the ray
	vec3 outColor = TraceRay(screenSpaceRayStart, screenSpaceRayDir, traceCounts);
    FragColor = outColor;
}

bool RayIsOutofScreen(vec2 ray)
{
	return (ray.x > 1 || ray.y > 1 || ray.x < 0 || ray.y < 0) ? true : false;
}

vec3 GetScreenCoordFromViewSpace(vec3 pViewSpaceVec)
{
	vec4 positionInScreen = projection * vec4(pViewSpaceVec,1.0);
	positionInScreen /= positionInScreen.w;
	positionInScreen.xyz = (positionInScreen.xyz + vec3(1.0f)) / 2.0f;
    return positionInScreen.xyz;
}

