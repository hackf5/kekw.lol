#version 330 core
out vec4 FragColor;

uniform bool highlight;

void main()
{
    if (highlight)
    {   
        FragColor = vec4(0.6, 0.4, 0.6, 1.0);
    }
    else
    {
        FragColor = vec4(0.2, 0.4, 0.6, 1.0);
    }
}