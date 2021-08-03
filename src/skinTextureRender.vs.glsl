//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

// SKIN_TEXTURE - Vertex Shader

#version 400 core

// Uniforms
	// uniform mat4 world_matrix;  // buried in the boneWorld matrix
	uniform mat4 view_matrix;
	uniform mat4 proj_matrix;
	uniform mat4 invPose[20];
	uniform mat4 boneWorld[20];

// Attributes
	layout (location = 0) in vec4 vert;  // x y z  (w=1)
	layout (location = 1) in vec2 uv;
	layout (location = 2) in vec3 norms;
	layout (location = 3) in vec4  boneWeight;
	layout (location = 4) in ivec4 boneIndex;

// Varying
	out VS_OUT
	{
		vec2 textCoordinate;
	} vs_out;


void main(void)
{
    vs_out.textCoordinate = uv;

	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!

	 mat4 skin_matrix = boneWorld[boneIndex.x] * invPose[boneIndex.x] * boneWeight.x +
					    boneWorld[boneIndex.y] * invPose[boneIndex.y] * boneWeight.y +
					    boneWorld[boneIndex.z] * invPose[boneIndex.z] * boneWeight.z +
					    boneWorld[boneIndex.w] * invPose[boneIndex.w] * boneWeight.w;
	
    gl_Position = proj_matrix * view_matrix * skin_matrix * vert;    
}

// --- End of File ---
