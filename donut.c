#define screen_width 1.0
#define screen_height 1.0

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

/*
void render_donut(float A, float b);

This is a function which draws a static(paused) donut.
We explicitly rotate this donut in main function using the variables A and B.
i.e. float A indicates the X-Axis angle and float B indicates the Z-Axis angle; then we can rotate the object with XZ-Axis as a pivot.
*/
void render_donut(float A, float B);

int main(int argc, char *argv[]){
    for (float rotating_angle = 0; rotating_angle < 2*M_PI; rotating_angle += M_PI/100){
        render_donut(rotating_angle, rotating_angle);
        sleep(0.1);
    }
    return 0;
}

void render_donut(float A, float B){
}