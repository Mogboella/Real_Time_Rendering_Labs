#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 textureDir;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    textureDir = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);

    // mat4 viewNoTranslate = mat4(mat3(view));
    // vec4 pos = projection * viewNoTranslate * vec4(aPos, 1.0);

    // gl_Position = pos.xyww;
}

