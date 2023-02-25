#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 aInstanceMatrix;
layout (location = 5) in vec4 aCol;

out vec3 color;

uniform mat4 projection;

vec3 hue2rgb(vec3 hue) {
    hue = fract(hue); //only use fractional part
    float r = abs(hue.x * 6 - 3) - 1; //red
    float g = 2 - abs(hue.y * 6 - 2); //green
    float b = 2 - abs(hue.z * 6 - 4); //blue
    vec3 rgb = vec3(r,g,b); //combine components
    rgb = clamp(rgb, 0.0f, 1.0f); //clamp between 0 and 1
    return rgb;
}

void main()
{
  gl_Position = projection * aInstanceMatrix * vec4(aPos, 1.0f);
  color = aCol.xyz;
};

