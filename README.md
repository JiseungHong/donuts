# donuts
spinning all kinds of 3-dimensional objects in terminal.
**Any comments, Issues, Pull Requests are always welcome.**
---


### donuts.c
All descriptions are commented in _donuts.c_.

>gcc -o donut donut.c
>
>./donut


Then, experimentally manipulate the terminal size.

Reference: https://www.a1k0n.net/2011/07/20/donut-math.html


### sphere.c

>gcc -o sphere sphere.c
>
>./sphere


All except following features are same as the donut.

1. I made a sphere of radius R from the point (RS, RS, 0). Refer to _sphere.c_ for variable descriptions.
2. Then I rotated the sphere about x-axis and z-axis.
3. Each coordinate (x, y, z) is obtained by the **parametric equation of sphere * parallel translation matrix * rotation matrix**.

![donut](https://github.com/JiseungHong/donuts/assets/51814680/49d13338-f299-43fc-814c-8af94566c910)
