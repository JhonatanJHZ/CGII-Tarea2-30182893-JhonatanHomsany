#version 330 core
in vec3 localPos;
uniform sampler3D volumeTex; 
out vec4 FragColor;

uniform vec3 cameraPosition;
uniform bool displayGas, displayLiquid, displayObjects, displayTerrain;
    
void main()
{
    vec3 rayOrigin = localPos;
    vec3 rayDir = normalize(localPos - cameraPosition);
    vec3 color = vec3(0.0);
    vec3 ray = rayOrigin;
    float t_max = 2.0;
    float stepSize = 1.0 / 256.0;
    float transparency = 1.0;
    const int MAX_STEPS = 512;

    for(int i = 0; i < MAX_STEPS; i++) {
        if (ray.x < 0.0 || ray.x > 1.0 ||
            ray.y < 0.0 || ray.y > 1.0 ||
            ray.z < 0.0 || ray.z > 1.0) break;
        vec4 voxel = texture(volumeTex, ray);
        float voxelOpacity = voxel.a;
        vec3 voxelColor = voxel.rgb;

        float alphaValue = voxelOpacity * 255.0;
        bool visible = true;

        if(alphaValue <= 75){
            visible = displayGas;
        }

        else if(alphaValue <= 150){
            visible = displayLiquid;
        }
        else if(alphaValue <= 254){
            visible = displayObjects;
        }
        else {
            visible = displayTerrain;
        }

        if(visible){
            color += transparency * voxelOpacity * voxelColor; 
            transparency *= (1.0 - voxelOpacity);
            if (transparency < 0.1) break;
        }

        ray += rayDir * stepSize;
    }

    float opacity = 1.0 - transparency;
    FragColor = vec4(color, opacity);
}