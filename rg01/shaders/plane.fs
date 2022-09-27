#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTextureGrass;
uniform sampler2D floorTextureRocky;
uniform sampler2D floorTextureSnow;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;
uniform float maxHeight;
uniform bool shadows; 

void main()
{         
	vec3 color;
	if (fs_in.FragPos.y / maxHeight >= 0.7){
		color = texture(floorTextureSnow, fs_in.TexCoords).rgb;
	}
	else if (fs_in.FragPos.y / maxHeight >= 0.55){
		float broj = (fs_in.FragPos.y / maxHeight) - 0.55;
		broj = broj * (1.0f / 0.15f);
		vec3 prvaTex = texture(floorTextureSnow, fs_in.TexCoords).rgb;
		vec3 drugaTex = texture(floorTextureGrass, fs_in.TexCoords).rgb;
		color = prvaTex* broj + drugaTex*(1.0-broj);
	}
	else if (fs_in.FragPos.y / maxHeight >= 0.3){
		color = texture(floorTextureGrass, fs_in.TexCoords).rgb;
	}
	else if (fs_in.FragPos.y / maxHeight >= 0.2){
		float broj = (fs_in.FragPos.y / maxHeight) - 0.2;
		broj = broj * 10;
		vec3 prvaTex = texture(floorTextureGrass, fs_in.TexCoords).rgb;
		vec3 drugaTex = texture(floorTextureRocky, fs_in.TexCoords).rgb;
		color = prvaTex * drugaTex;
		color = prvaTex* broj + drugaTex*(1.0-broj);
	}
	else {
		color = texture(floorTextureRocky, fs_in.TexCoords).rgb;
	}
	if (shadows){
		// ambient
		vec3 ambient = 0.05 * color;
		// diffuse
		vec3 lightDir = normalize(lightPos - fs_in.FragPos);
		vec3 normal = normalize(fs_in.Normal);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = diff * color;
		// specular
		vec3 viewDir = normalize(viewPos - fs_in.FragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = 0.0;
		if(!blinn)
		{
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
		}
		else
		{
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
		}
		vec3 specular = vec3(0.3) * spec; // assuming bright white light color
		FragColor = vec4(ambient + diffuse + specular, 1.0);
	}
	else {
		FragColor = vec4(color, 1.0);
	}
}
