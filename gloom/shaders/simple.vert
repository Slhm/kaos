#version 430 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal_in;
in layout(location = 2) vec2 textureCoordinates_in;

uniform layout(location = 3) mat4 P;
uniform layout(location = 4) mat4 model;
uniform layout(location = 5) mat3 R;
uniform layout(location = 6) mat4 view;

out layout(location = 1) vec3 normal_out;
out layout(location = 2) vec2 textureCoordinates_out;

out vec2 texCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;

out SN_OUT {
    vec3 surfNorm;
} sn_out;

out vec3 surfNorm;


void main()
{
    mat3 normMat = mat3(transpose(inverse(view * model)));
    vs_out.texCoords = textureCoordinates_in;
    normal_out = normalize(R * normalize(normal_in));
    sn_out.surfNorm = vec3(model * vec4(position, 1.0));

    textureCoordinates_out = textureCoordinates_in;
    gl_Position = P * view * model * vec4(position, 5.0f);
}