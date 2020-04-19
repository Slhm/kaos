#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform layout(location = 1) vec3 normal_in;
uniform layout(location = 2) vec2 textureCoordinates_in;
uniform layout(location = 7) float closeToViewCenter;

out layout(location = 1) vec3 normal_out;
out layout(location = 2) vec2 textureCoordinates_out;

//in vec3 surfNorm;
out vec3 surfNorm;

in SN_OUT{
    vec3 surfNorm;
} surfnormin[];

out SN_OUT{
    vec3 surfNorm;
} surfnormout;

in VS_OUT{
    vec2 texCoords;
} gs_in[];

out vec2 TexCoords;

vec4 explode(vec4 position, vec3 normal){
    vec3 direction = normal * closeToViewCenter * 70;
    return position + vec4(direction, 0.0);
}

vec3 GetNormal(){
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main(){

    surfnormout.surfNorm = surfnormin[0].surfNorm;

    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();

    EndPrimitive();
}
 
