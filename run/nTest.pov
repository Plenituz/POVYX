//#include "Primitive.inc"
#include "Collision.inc"
//#include "Contact.inc"
#include "colors.inc"

//Primitive_CreatePlane("Plane1", <1,1,1>, <0,0,0>)
//Primitive_CreateSphere("Sphere1", <0,0,0>, 1)
//#declare Collide = Collision_Collide("Plane1", "Sphere1");
//sphere{<0,0,0>, 1 pigment{#if(Collide) Yellow #else Red #end}}
//plane{<1,1,1>, 0 pigment{Blue}}



light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <0, 3, -10> 
    look_at < 0,0,0 > 
    angle 50
}