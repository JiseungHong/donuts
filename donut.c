#define screen_width 42
#define screen_height 42

#define R1 1.0
#define R2 2.0

#define K2 5.0
#define K1 screen_width*K2*3/(8*(R1+R2))

#define theta_spacing 0.01
#define phi_spacing 0.01

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* void render_donut(float A, float b);

This is a function which draws a static(paused) donut.
We explicitly rotate this donut in main function using the variables A and B.
i.e. float A indicates the X-Axis angle and float B indicates the Z-Axis angle; then we can rotate the object with XZ-Axis as a pivot.
*/
void render_donut(float A, float B){
    /* Initialize output & z_buffer.

    char output[screen_width][screen_height] is an array of characters which we'll draw on a terminal.
    float z_buffer[screen_width][screen_height] is a z-axis buffer which identify the closest pixel from the television(the viewer).
    */ 
    char output[screen_width][screen_height];
    float z_buffer[screen_width][screen_height];

    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            output[j][i] = ' ';
            z_buffer[j][i] = 0;
        }
        
    }

    /* Matrix Multiplication.

    1. float theta
    'theta' is the angle used to construct a circle of radius R1 from the point (R2, 0, 0).
    'theta' goes from 0 to 2*pi by interval of 'theta_spacing'. Be aware that the value of the interval affects the rotation speed.

    2. float phi
    'phi' is the angle used to construct a Torus(a.k.a. Donut) of radius R2 from the y-axis.
    Since we are rotating a circle, we use a rotation matrix to obtain each coordinate of the Torus.

    3. float A and float B
    Last of all, A and B is a variable given by main function.
    A and B indicates the rotation_angle of x-axis and z-axis respectively.
    */
    float sin_A = sin(A), sin_B = sin(B), cos_A = cos(A), cos_B = cos(B);
    for (float theta = 0; theta < 2*M_PI; theta += theta_spacing)
    {
        float cos_theta = cos(theta), sin_theta = sin(theta);
        for (float phi = 0; phi < 2*M_PI; phi += phi_spacing)
        {
            float cos_phi = cos(phi), sin_phi = sin(phi);

            float dis_x = R2+R1*cos_theta;
            float dis_y = R1*cos_A*sin_theta;
            float temp_x = cos_B*cos_phi+sin_A*sin_B*sin_phi;
            float temp_y = cos_phi*sin_B-cos_B*sin_A*sin_phi;
            float temp_z = R1*sin_A*sin_theta;

            float x = dis_x*temp_x-dis_y*sin_B;
            float y = dis_x*temp_y+dis_y*cos_B;
            float z = K2+cos_A*dis_x*sin_phi+temp_z;

            /* Luminance of each coordinate.

            Luminance is what makes the Torus 3-Dimensional.
            Here, we lighted up surfaces facing behind and above the viewer: (0, 1, -1).
            Thus we multiply the surface normal by (0, 1, -1), defining this value as 'float luminance'.
            Based on luminance, we judged how bright each coordinate is.

            Also, ooz(the reverse value of z) becomes a criterion for whether or not to draw each coordinate.
            That's because the coordinate cannot be seen by the viewer when the z-value is smaller than another z-value which has the same (int) x-projected value and (int) y-projected value.
            *PLEASE* Be aware of stack overflow(index out of range inside the if statement; segmentation fault would appear in Command Line Interface.)
            If you define out-of-bound values for R1, R2, K2, etc., segmentation fault occurs. Refer to the document for details.
            */
            float luminance = cos_phi*cos_theta*sin_B-cos_A*cos_theta*sin_phi-sin_A*sin_theta+cos_B*(cos_A*sin_theta-cos_theta*sin_A*sin_phi);
            int luminanceIndex = luminance * 8;

            float ooz = 1/z;
            int projection_x = (int)(screen_width/2+K1*ooz*x);
            int projection_y = (int)(screen_height/2-K1*ooz*y);
            if (luminance>0 && ooz>z_buffer[projection_x][projection_y])
            {
                output[projection_x][projection_y] = ".,-~:;=!*#$@"[luminanceIndex];
                z_buffer[projection_x][projection_y] = ooz;
            }
        }
    }

    /* Commenting out printf("\x1b[H");

    I do not know well how printf("\x1b[H"); works, but this line holds the terminal within fixed size.
    Eliminating this term and manually fixing the terminal size experimentally by screen_hight X screen_width was better.
    */
    // printf("\x1b[H");
    for (int j = 0; j < screen_height; j++) {
        for (int i = 0; i < screen_width; i++) {
            putchar(output[i][j]);
        }
        putchar('\n');
    }
    
    return;
}

/* int main(-NO ARGUMENTS-);

Designed an infinite loop that rotates a donut within the interval of 'float rotating_angle'.
Current design is to rotate the donut about x-axis and z-axis by both 'float rotating_angle'.
(+ You can rotate by each axis differently by assigning each angle respectively.)

The interval of the variable 'float rotating_angle' indicates how slow(or elaborate) the rotation would be by each axis.
(+ Currently the interval is +0.015. Likewise, minus interval is available.)
*/
int main(){
    float rotating_angle = 0;
    
    while (1)
    {   
        render_donut(rotating_angle, rotating_angle);
        rotating_angle += 0.015;
        rotating_angle = fmodf(rotating_angle, 2*M_PI);
    }

    return 0;
}