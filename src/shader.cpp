
#include "shader.hpp"



std::string EntityShader::VERTEX = R"(

#version 330 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 _texturePosition;
layout (location = 2) in vec4 _textureColor;
layout (location = 3) in uint _textureIndex;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec2 texturePosition;
out vec4 textureColor;
flat out uint textureIndex;

void main ()
{   
    gl_Position = projectionMatrix * viewMatrix * vec4 (vertexPosition, 0.0, 1.0);

    texturePosition = _texturePosition;
    textureColor = _textureColor;
    textureIndex = _textureIndex;
}

)";

std::string EntityShader::FRAGMENT = R"(

#version 330 core

in vec2 texturePosition;
in vec4 textureColor;
flat in uint textureIndex;
uniform sampler2DArray textures;

out vec4 FragColor;

void main ()
{
    FragColor = texture(textures, vec3(texturePosition, textureIndex)) * textureColor;
}

)";



std::string ShapeShader::VERTEX = R"(

#version 330 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec4 vertexColor;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec4 fragmentColor;

void main ()
{   
    gl_Position = projectionMatrix * viewMatrix * vec4 (vertexPosition, 0.0, 1.0);

    fragmentColor = vertexColor;
}

)";

std::string ShapeShader::FRAGMENT = R"(

#version 330 core

in vec4 fragmentColor;

out vec4 FragColor;

void main ()
{
    FragColor = fragmentColor;
}

)";
