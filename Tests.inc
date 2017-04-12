

/*  
test sphere colliders
Primitive_CreateSphere("mSphere", <0,0,0>, 1)
Primitive_CreateSphere("mSphere2", <clock*-1*3,0,0>, 1)
#local Collide = Collision_Collide("mSphere", "mSphere2");
sphere{0, 1 pigment{Red}}
sphere{<clock*-1*3,0,0>, 1 pigment{#if(Collide) Yellow #else Red #end}}
*/