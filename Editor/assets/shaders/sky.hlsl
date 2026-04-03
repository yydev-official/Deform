// SM 2.0 compatible skybox shader
// Note: Cubemaps might not work in SM 2.0!

// Constant buffer - check if your program supports this in SM 2.0
// Some SM 2.0 implementations don't support constant buffers
float4x4 viewProjectionMatrix;
float4x4 worldMatrix;

// For SM 2.0, we often need to use global variables instead
// texture cubeTexture; // Cubemap texture - may not be supported!

struct VertexShaderOutput
{
    float4 position : POSITION;
    float3 texCoord : TEXCOORD0;  // TEXCOORD is supported
};

VertexShaderOutput main(float3 position : POSITION)
{
    VertexShaderOutput output;
    
    // Transform position
    float4 worldPos = mul(float4(position, 1.0f), worldMatrix);
    float4 clipPos = mul(worldPos, viewProjectionMatrix);
    
    // SM 2.0 doesn't support SV_POSITION, use POSITION
    // Also doesn't support .xyww trick reliably
    output.position = clipPos;
    
    // Pass through texture coordinates
    output.texCoord = position;
    
    return output;
}

// Pixel shader - but cubemap sampling may fail in SM 2.0!
float4 mainPS(VertexShaderOutput input) : COLOR
{
    // SM 2.0 doesn't support TextureCube properly
    // You might need a different approach
    return float4(0.5, 0.6, 0.8, 1.0); // Fallback color
}