#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    int shininess;
}; 
  
uniform Material material;

struct Light {
    vec3  position;
    vec3  direction;
    float cutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light; 

uniform bool invertSpecular; 

uniform bool useEmissionMaps;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    vec3 diffuse;
    vec3 specular;
    if(theta > light.cutOff) 
    {       
        // do lighting calculations
        
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        if(invertSpecular == true) 
        {
            specular = light.specular * spec * (vec3(1.0) - vec3(texture(material.specular, TexCoords)));
        } else
        {
            specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        }
    }
    else  // else, use ambient light so scene isn't completely dark outside the spotlight.
    {
        diffuse = vec3(0.0f);
        specular = vec3(0.0f);
    }    

    vec3 result;
    if(useEmissionMaps == true)
    {
        vec3 emission = vec3(texture(material.emission, TexCoords)) * (vec3(1.0) - vec3(texture(material.specular, TexCoords)));
        result = ambient + diffuse + specular + emission;
    } else
    {
        result = ambient + diffuse + specular;
    }
    
    FragColor = vec4(result, 1.0);
}