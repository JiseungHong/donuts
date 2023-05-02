#define screen_width 50
#define screen_height 50

#define R 1.0
#define RS 0.8

#define K2 2.1
#define K1 6.0

#define theta_spacing 0.01
#define phi_spacing 0.01

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/* void render_sphere(float A, float b);

This is a function which draws a static(paused) sphere.
We explicitly rotate this sphere in main function using the variables A and B.
i.e. float A indicates the X-Axis angle and float B indicates the Z-Axis angle; then we can rotate the object with XZ-Axis as a pivot.
*/
void render_sphere(float A, float B){
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

    1. float theta, float phi
    'theta', 'phi' are the angles used to construct a sphere of radius R from the point (RS, RS, 0).
    radius R and value of RS are defined by macro.

    2. float A, float B
    A and B is a variable given by main function.
    A and B indicates the rotation_angle of x-axis and z-axis respectively.
    */
    float sin_A = sin(A), sin_B = sin(B), cos_A = cos(A), cos_B = cos(B);
    for (float theta = 0; theta < 2*M_PI; theta += theta_spacing)
    {
        float cos_theta = cos(theta), sin_theta = sin(theta);
        for (float phi = 0; phi < 2*M_PI; phi += phi_spacing)
        {
            float cos_phi = cos(phi), sin_phi = sin(phi);

            float x = R*(sin_theta*cos_phi*cos_B-sin_B*(sin_theta*sin_phi*cos_A-cos_theta*sin_A))+RS*cos_B-RS*sin_B*cos_A;
            float y = R*(sin_theta*cos_phi*sin_B+cos_B*(sin_theta*sin_phi*cos_A-cos_theta*sin_A))+RS*sin_B+RS*cos_B*cos_A;
            float z = K2+R*(sin_theta*sin_phi*sin_A+cos_A*cos_theta)+RS*sin_A;

            float nx = sin_theta*cos_phi*cos_B-sin_B*(sin_theta*sin_phi*cos_A-cos_theta*sin_A);
            float ny = sin_theta*cos_phi*sin_B+cos_B*(sin_theta*sin_phi*cos_A-cos_theta*sin_A);
            float nz = sin_theta*sin_phi*sin_A+cos_A*cos_theta;

            /* Luminance of each coordinate.

            Luminance is what makes the Sphere 3-Dimensional.
            Here, we lighted up surfaces facing behind and above the viewer: (0, 1, -1).
            Thus we multiply the surface normal by (0, 1, -1), defining this value as 'float luminance'.

            > (nx, ny, nz) o (0, 1, -1) = ny - nz

            Based on luminance, we judged how bright each coordinate is.
            */
            float luminance = ny-nz;

            /* Luminance Index | int luminanceIndex

            In order to draw outputs from luminance, we have to adjust the luminance value.
            The value of float luminance is between -1.415 ~ +1.415, so we adjust the value between 0 ~ 11.

            Also, ooz(the reverse value of z) becomes a criterion for whether or not to draw each coordinate.
            That's because the coordinate cannot be seen by the viewer when the z-value is smaller than another z-value which has the same (int) x-projected value and (int) y-projected value.
            *PLEASE* Be aware of stack overflow(index out of range inside the if statement; segmentation fault would appear in Command Line Interface.)
            If you define out-of-bound values for R, RS, K2, etc., segmentation fault occurs. Refer to the document for details.
            */
            int luminanceIndex = (luminance+1.415)*12/2.85;

            float ooz = 1/z;
            int projection_x = (int)(screen_width/2+K1*ooz*x);
            int projection_y = (int)(screen_height/2-K1*ooz*y);
            if (ooz>z_buffer[projection_x][projection_y])
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

Designed an infinite loop that rotates a sphere within the interval of 'float rotating_angle'.
Current design is to rotate the sphere about x-axis and z-axis by both 'float rotating_angle'.
(+ You can rotate by each axis differently by assigning each angle respectively.)

The interval of the variable 'float rotating_angle' indicates how slow(or elaborate) the rotation would be by each axis.
(+ Currently the interval is +0.015. Likewise, minus interval is available.)
*/
int main(){
    float rotating_angle = 0;
    
    while (1)
    {   
        render_sphere(rotating_angle, rotating_angle);
        rotating_angle += 0.015;
        rotating_angle = fmodf(rotating_angle, 2*M_PI);
    }

    return 0;
}