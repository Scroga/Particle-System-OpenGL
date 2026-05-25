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

uniform sampler2DArray u_textures;

uniform PointLight u_lightPoint; 
uniform Material u_material;
uniform vec3 u_viewPos;

in GS_OUT {
    vec3 worldPos;
    vec3 normal;
    vec2 texCoord;
    vec4 color;
    flat int texIndex;
} fs_in;

out vec4 FragColor;

void main()
{  
    vec4 color = texture(u_textures, vec3(fs_in.texCoord, fs_in.texIndex));
    color *= fs_in.color;
    float alpha = color.a;
    if (alpha < 0.1) discard;

    vec3 norm = normalize(fs_in.normal);

    vec3 lightDir = normalize(u_lightPoint.position - fs_in.worldPos);

    // Attenuation calculations
    float distance = length(u_lightPoint.position - fs_in.worldPos);
    float attenuation = 1.0 / (u_lightPoint.constant + u_lightPoint.linear * distance + u_lightPoint.quadratic * (distance * distance));

    // Ambient calculations
    vec3 ambient = u_lightPoint.ambient * color.xyz;

    // Diffuse calculations
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = u_lightPoint.diffuse * diff * u_material.diffuse;

    // Specular calculations
    vec3 viewDir = normalize(u_viewPos - fs_in.worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_lightPoint.specular * (spec * u_material.specular); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, alpha);
}