#version 430 core

// ------------------------------------------
//    Test Compute shader
// ------------------------------------------

// localize breakdown
layout(local_size_x = 1) in;

layout (std430, binding = 0) buffer InMatrix
{
	mat4 inMatrix[];
};

layout (std430, binding = 1) buffer OutMatrix
{
	mat4 outMatrix[];
};


// Uniforms
// ...


void main()
{
	uint boneIndex = gl_GlobalInvocationID.x;

	// Translation Matrix  
	//         0, 5, 10, 15, 20 by index

	vec4 vTrans = vec4(5.0f, 5.0f, 5.0f, 1.0f);
	vTrans = vTrans * float(boneIndex);

	mat4  mTrans;
		
	mTrans[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	mTrans[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	mTrans[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
	mTrans[3] = vTrans;

	outMatrix[boneIndex] =  mTrans * inMatrix[boneIndex];	
};
