#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{   
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); 
    // combine results
    vec3 ambient  = vec3(0.2)  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = vec3(0.5)  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = vec3(1.0) * spec * vec3(texture(texture_specular1, TexCoords));

    FragColor = vec4(lightColor * (ambient + diffuse + specular), TexCoords);
}