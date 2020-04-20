#version 430 core

in layout(location = 1) vec3 normal;
uniform layout(location = 10) vec3 L1;
uniform layout(location = 11) vec3 L2;

in vec2 TexCoords;
in vec3 surfNorm;

layout(binding = 0) uniform sampler2D texture1;
vec4 tex = texture(texture1, TexCoords);

layout(binding = 1) uniform sampler2D nrmTex;
vec4 nt = texture(nrmTex, TexCoords);

//PHONG ILLUMINATION

//ambient
vec3 ambient = vec3(0.1, 0.1, 0.1);
vec3 norm2 = normalize((nt.xyz * 2) - 1);

//diffuse
float d = length(L1.xyz - norm2) + length(L2.xyz - norm2);
float atten = 1.0f / (0.008f + d * 0.009f + d * d * 0.0002f);
vec3 ld1 = normalize(L1.xyz - surfNorm);
vec3 ld2 = normalize(L2.xyz - surfNorm);

float diff = max(dot(norm2, ld1), 0.0) + max(dot(norm2, ld2), 0.0);
vec3 diffColor = diff * vec3(0.4, 0.4, 0.4) * atten;

//specular
vec3 rLd1 = reflect(-ld1, norm2);
vec3 rLd2 = reflect(-ld2, norm2);

vec3 cam = {0, 0, 0};
vec3 dCam = normalize(cam.xyz - surfNorm);
float spec1 = pow(max(dot(dCam, rLd1), 0.0), 64);
float spec2 = pow(max(dot(dCam, rLd2), 0.0), 64);
vec3 specV = (spec1 + spec2) * vec3(1.0, 1.0, 1.0) * atten;


out vec4 color;

float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }
float dither(vec2 uv) { return (rand(uv)*2.0-1.0) / 256.0; }

void main()
{
    color = vec4((diffColor + ambient + specV + dither(TexCoords)), 1.0) * tex;
    //color = texture( texture1, TexCoords );
    //color = vec4(1.0, 1.0, 1.0, 1.0);
}