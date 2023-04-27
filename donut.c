#define screen_width 10
#define screen_height 10

#define R1 1.0
#define R2 2.0

#define K2 5.0
#define K1 screen_width*K2*3/(8*(R1+R2))

#define theta_spacing 0.07
#define phi_spacing 0.02

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* void render_donut(float A, float b);

This is a function which draws a static(paused) donut.
We explicitly rotate this donut in main function using the variables A and B.
i.e. float A indicates the X-Axis angle and float B indicates the Z-Axis angle; then we can rotate the object with XZ-Axis as a pivot.
*/
float *XYZ_3D(float sin_A, float sin_B, float sin_t, float sin_p, float cos_A, float cos_B,
    float cos_t, float cos_p);
void render_donut(float A, float B);

int main(int argc, char *argv[]){
    for (float rotating_angle = 0; rotating_angle < 2*M_PI; rotating_angle += M_PI/100){
        render_donut(rotating_angle, rotating_angle);
        sleep(0.1);
    }
    return 0;
}

float *XYZ_3D(float sin_A, float sin_B, float sin_t, float sin_p, float cos_A, float cos_B,
    float cos_t, float cos_p){
    float ret[4];
    
    float dis_x = R2+R1*cos_t;
    float dis_y = R1*cos_A*sin_t;
    float temp_x = cos_B*cos_p+sin_A*sin_B*sin_p;
    float temp_y = cos_p*sin_B-cos_B*sin_A*sin_p;
    float temp_z = R1*sin_A*sin_t;
    
    ret[0] = dis_x*temp_x-dis_y*sin_B;
    ret[1] = dis_x*temp_y+dis_y*cos_B;
    ret[2] = cos_A*dis_x*sin_p+temp_z;

    float luminance = cos_p*cos_t*sin_B-cos_A*cos_t*sin_p-sin_A*sin_t+cos_B*(cos_A*sin_t-cos_t*sin_A*sin_p);
    ret[3] = luminance;

    return ret;
}

void render_donut(float A, float B){
    /* Initialize output & z_buffer.

    char output[screen_width][screen_height] is an array of characters which we'll draw on a terminal.
    float z_buffer[screen_width][screen_height] is a z-axis buffer which identify the closest pixel from the television(the viewer).
    */ 
    char output[screen_width][screen_height];
    float z_buffer[screen_width][screen_height];

    for (int i = 0; i < screen_width; i++)
    {
        for (int j = 0; j < screen_height; j++)
        {
            output[i][j] = ' ';
            z_buffer[i][j] = 0;
        }
        
    }

    for (float theta = 0; theta < 2*M_PI; theta += theta_spacing)
    {
        float cos_theta = cos(theta), sin_theta = sin(theta);
        for (float phi = 0; phi < 2*M_PI; phi += phi_spacing)
        {
            float cos_phi = cos(phi), sin_phi = sin(phi);
            float *displacement = XYZ_3D(sin(A), sin(B), sin_theta, sin_phi, cos(A), cos(B),
                cos_theta, cos_phi);
            float L = displacement[3];
            int luminanceIndex = L*8;
            float x = displacement[0];
            float y = displacement[1];
            float z = displacement[2];
            float ooz = 1/z;
            int projection_x = (int)(screen_width/2+K1*ooz*x);
            int projection_y = (int)(screen_height/2-K1*ooz*y);

            if (L>0 && ooz>z_buffer[projection_x][projection_y])
            {
                output[projection_x][projection_y] = ".,-~:;=!*#$@"[luminanceIndex];
            }
        }
    }

    printf("\x1b[H");
    for (int j = 0; j < screen_height; j++) {
        for (int i = 0; i < screen_width; i++) {
            putchar(output[i,j]);
        }
        putchar('\n');
    }
    
    return;
}

