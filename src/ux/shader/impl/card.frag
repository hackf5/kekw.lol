#version 330 core
out vec4 FragColor;

uniform int state;

void main()
{
    switch(state)
    {
        case 1:
            FragColor = vec4(250.0 / 255.0, 140.0 / 255.0, 170.0 / 255.0, 1.0);
            break;
        case 2:
            FragColor = vec4(250.0 / 255.0, 200.0 / 255.0, 70.0 / 255.0, 1.0);
            break;
        default:
            FragColor = vec4(20.0 / 255.0, 10.0 / 255.0, 50.0 / 255.0, 1.0);
            break;
    }
}