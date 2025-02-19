#include "shaders.h"

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 FragPos;
in vec3 Normal;

uniform vec3 cubeColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    // Ambient
    float ambientStrength = 0.2;  // Возвращаем к стандартному значению
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse - исправляем расчет диффузного освещения
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular - корректируем расчет бликов
    float specularStrength = 0.5;  // Возвращаем к стандартному значению
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  // Увеличиваем степень для более четких бликов
    vec3 specular = specularStrength * spec * lightColor;

    // Обеспечиваем корректное смешивание компонентов освещения
    vec3 result = (ambient + diffuse + specular) * cubeColor;
    FragColor = vec4(result, 1.0);
}
)";

const char* skyboxVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
)";

const char* skyboxFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

void main()
{
    // Нормализуем координаты для получения направления взгляда
    vec3 dir = normalize(TexCoords);

    // Вычисляем градиент на основе Y-координаты (высоты)
    // Значение dir.y находится в диапазоне [-1, 1]
    float t = (dir.y + 1.0) * 0.5; // преобразуем в [0, 1]

    // Цвета для градиента
    vec3 topColor = vec3(0.529, 0.808, 0.922);    // Светло-голубой
    vec3 bottomColor = vec3(0.118, 0.565, 1.000); // Более насыщенный синий

    // Смешиваем цвета
    vec3 finalColor = mix(bottomColor, topColor, t);

    FragColor = vec4(finalColor, 1.0);
}
)";