#version 330 core
in vec3 localPos;
uniform sampler3D volumeTex; 
out vec4 FragColor;

uniform vec3 cameraPosition;
uniform float densityMin;
uniform float densityMax;
uniform vec3 volumeScale;
uniform float gasOpacityScale, liquidOpacityScale, objectsOpacityScale, terrainOpacityScale;
uniform float gasOpacityLowerLimit, gasOpacityUpperLimit, liquidOpacityLowerLimit, liquidOpacityUpperLimit, objectsOpacityLowerLimit, objectsOpacityUpperLimit;
uniform float voxelSize;
    
void main()
{
    vec3 cameraLocal = cameraPosition / volumeScale;
    vec3 rayDir = normalize(localPos - cameraLocal);
    vec3 color = vec3(0.0);
    vec3 ray = localPos;
    float stepSize = (1.0 / 256.0) * voxelSize;
    float transparency = 1.0;
    int maxSteps = min(int(1.8 / stepSize) + 1, 2048);

    for(int i = 0; i < maxSteps; i++) {
        if (ray.x < 0.0 || ray.x > 1.0 ||
            ray.y < 0.0 || ray.y > 1.0 ||
            ray.z < 0.0 || ray.z > 1.0) break;
        vec4 voxel = texture(volumeTex, ray);
        if(voxel.a * 255 >= densityMin && voxel.a * 255 <= densityMax){
            float voxelOpacity = voxel.a;
            vec3 voxelColor = voxel.rgb;

            float alphaValue = voxelOpacity * 255.0;
            float opacityFactor = 1.0;

            if(alphaValue <= 75){
                if(alphaValue >= gasOpacityLowerLimit && alphaValue <= gasOpacityUpperLimit){
                    opacityFactor = gasOpacityScale;
                }
                else{
                    opacityFactor = 0;
                }
            }

            else if(alphaValue <= 150){
                if(alphaValue >= liquidOpacityLowerLimit && alphaValue <= liquidOpacityUpperLimit){
                    opacityFactor = liquidOpacityScale;
                }
                else{
                    opacityFactor = 0;
                }
            }
            else if(alphaValue <= 254){
                if(alphaValue >= objectsOpacityLowerLimit && alphaValue <= objectsOpacityUpperLimit){
                    opacityFactor = objectsOpacityScale;
                }
                else{
                    opacityFactor = 0;
                }
            }
            else {
                opacityFactor = terrainOpacityScale;
            }

            voxelOpacity *= opacityFactor;
            color += transparency * voxelOpacity * voxelColor; 
            transparency *= (1.0 - voxelOpacity);
            if (transparency < 0.1) break;
        }

        ray += rayDir * stepSize;
    }

    float opacity = 1.0 - transparency;
    FragColor = vec4(color, opacity);
}