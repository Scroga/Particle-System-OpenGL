#version 430 core

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight u_lightPoint; 
uniform Material u_material;
uniform vec3 u_viewPos;

in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} fs_in;

out vec4 FragColor;

void main()
{  
    vec3 norm = normalize(fs_in.normal);

    vec3 lightDir = normalize(u_lightPoint.position - fs_in.position);

    // Attenuation calculations
    float distance = length(u_lightPoint.position - fs_in.position);
    float attenuation = 1.0 / (u_lightPoint.constant + u_lightPoint.linear * distance + u_lightPoint.quadratic * (distance * distance));

    // Ambient calculations
    vec3 ambient = u_lightPoint.ambient * vec3(u_material.diffuse); // color

    // Diffuse calculations
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = u_lightPoint.diffuse * diff * u_material.diffuse;

    // Specular calculations
    vec3 viewDir = normalize(u_viewPos - fs_in.position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_lightPoint.specular * (spec * u_material.specular); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}