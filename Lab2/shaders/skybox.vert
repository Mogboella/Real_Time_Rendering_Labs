#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 vDir;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vDir = aPos;

    mat4 viewNoTranslate = mat4(mat3(view));
    vec4 pos = projection * viewNoTranslate * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}

