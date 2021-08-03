//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

// SKIN_TEXTURE - Vertex Shader

#version 400 core

// Uniforms
	// uniform mat4 world_matrix;  // buried in the boneWorld matrix
	uniform mat4 view_matrix;
	uniform mat4 proj_matrix;
	uniform mat4 invPose[100]; // should substitute with SBO
	uniform mat4 boneWorld[100];
	uniform vec3 vLightPos;
	uniform vec4 vLightColor;
	uniform vec4 vObjColor;

//	layout(std430, binding = 0) buffer InvPose
//	{
//		mat4 invPose[];
//	};
//
//	layout(std430, binding = 0) buffer BoneWorld
//	{
//		mat4 boneWorld[];
//	};

// Attributes
	layout (location = 0) in vec4 vert;  // x y z  (w=1)
	layout (location = 2) in vec3 norms;
	layout (location = 3) in vec4  boneWeight;
	layout (location = 4) in ivec4 boneIndex;

// Varying
	out VS_OUT
	{
		vec4 vFragColor;
	} vs_out;


void main(void)
{
	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!

	 mat4 skin_matrix = boneWorld[boneIndex.x] * invPose[boneIndex.x] * boneWeight.x +
					    boneWorld[boneIndex.y] * invPose[boneIndex.y] * boneWeight.y +
					    boneWorld[boneIndex.z] * invPose[boneIndex.z] * boneWeight.z +
					    boneWorld[boneIndex.w] * invPose[boneIndex.w] * boneWeight.w;

	// Ambient
		float ambientStrength = 0.3;
		vec3 ambient = ambientStrength * vLightColor.rgb;

	// Get vNorm
		mat4 mvMatrix = view_matrix * skin_matrix;
		mat3 mNormalMatrix;
		mNormalMatrix[0] = normalize(mvMatrix[0].xyz);
		mNormalMatrix[1] = normalize(mvMatrix[1].xyz);
		mNormalMatrix[2] = normalize(mvMatrix[2].xyz);
		vec3 vNorm = normalize(mNormalMatrix * norms);

	// Get vLightDir
		vec4 ecPosition;
		vec3 ecPosition3;
		ecPosition = mvMatrix * vert;
		ecPosition3 = ecPosition.xyz /ecPosition.w;
		vec3 vLightDir = normalize(vLightPos - ecPosition3);

	// Get vFragColor
		float fDot = max(0.0, dot(vNorm, vLightDir)); 
		vs_out.vFragColor.rgb = vObjColor.rgb * fDot + ambient;
		vs_out.vFragColor.a = vObjColor.a;
	
    gl_Position = proj_matrix * view_matrix * skin_matrix * vert;    
}

// --- End of File ---
