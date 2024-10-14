#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int postprocessing;

const float offset = 1.0 / 300.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
    float kernel[9];
    vec3 sampleTex[9];
    float average;
    vec3 col;
    switch(postprocessing)
    {
        case 0: // No postprocessing
            col = texture(screenTexture, TexCoords).rgb;
            FragColor = vec4(col, 1.0);
            break;
        case 1: // Invert colors
            FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
            break;
        case 2: // Grayscale 1
            FragColor = texture(screenTexture, TexCoords);
            average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
            FragColor = vec4(average, average, average, 1.0);
            break;
        case 3: // Grayscale 2
            FragColor = texture(screenTexture, TexCoords);
            average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
            FragColor = vec4(average, average, average, 1.0);
            break;
        default: // Sharpen kernel
            switch(postprocessing) 
            {
                case 4: // Sharpen kernel
                    kernel = float[](
                        -1, -1, -1,
                        -1,  9, -1,
                        -1, -1, -1
                    );
                    break;
                case 5: // Blur kernel
                    kernel = float[](
                        1.0 / 16, 2.0 / 16, 1.0 / 16,
                        2.0 / 16, 4.0 / 16, 2.0 / 16,
                        1.0 / 16, 2.0 / 16, 1.0 / 16  
                    );
                    break;
                case 6: // Edge detection kernel
                    kernel = float[](
                        1,  1,  1,
                        1, -8,  1,
                        1,  1,  1  
                    );
                    break;
            }
            // Apply kernel
            for(int i = 0; i < 9; i++)
            {
                sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
            }
            col = vec3(0.0);
            for(int i = 0; i < 9; i++)
                col += sampleTex[i] * kernel[i];
            
            FragColor = vec4(col, 1.0);            
    }
} 