#include "Mesh.inc"
#include "POVYX.inc"
#include "Rigidbody.inc"   


#if(clock = 0)
    Mesh_CreateMesh("mMesh", 
    array[12*3]{
    <0,0,0>, <0,0,1>, <0,1,0>,
    <0,1,1>, <0,1,0>, <0,0,1>,
    <0,0,1>, <1,0,1>, <0,1,1>,
    <1,1,1>, <0,1,1>, <1,0,1>,
    <1,0,1>, <1,0,0>, <1,1,1>,
    <1,1,0>, <1,1,1>, <1,0,0>,
    <1,0,0>, <0,0,0>, <1,1,0>,
    <0,1,0>, <1,1,0>, <0,0,0>,
    <1,1,0>, <0,1,0>, <1,1,1>,
    <0,1,1>, <1,1,1>, <0,1,0>,
    <0,0,0>, <1,0,0>, <0,0,1>,
    <1,0,1>, <0,0,1>, <1,0,0>
    }, 12*3) 
    //Mesh_RotateMesh("mMesh", <0, 0, 45>, <.5,.5,.5>)
    Mesh_CreateMesh("mMesh1", 
    array[12*3]{
    <0,0,0>, <0,0,1>, <0,1,0>,
    <0,1,1>, <0,1,0>, <0,0,1>,
    <0,0,1>, <1,0,1>, <0,1,1>,
    <1,1,1>, <0,1,1>, <1,0,1>,
    <1,0,1>, <1,0,0>, <1,1,1>,
    <1,1,0>, <1,1,1>, <1,0,0>,
    <1,0,0>, <0,0,0>, <1,1,0>,
    <0,1,0>, <1,1,0>, <0,0,0>,
    <1,1,0>, <0,1,0>, <1,1,1>,
    <0,1,1>, <1,1,1>, <0,1,0>,
    <0,0,0>, <1,0,0>, <0,0,1>,
    <1,0,1>, <0,0,1>, <1,0,0>
    }, 12*3)
    
    
    
    StoreVar("position",<0.4, 2, 0.4>)
    StoreVar("velocity", <0, 0, 0>) 
    
    POVYX_Init()
    POVYX_AddConstantForce(<0, -9, 0>)
    POVYX_AddCollider("mMesh", 1, 0.01)
    Mesh_MoveMesh("mMesh1", <0, 2, 0>)
    
    Rg_CreateRg("mMesh1", "mRg1", <0,0,0>)
#end

POVYX_CalculateRgFrame("mRg1")
Mesh_RenderVertex("mMesh1")
Mesh_RenderVertex("mMesh")
#local Center = Mesh_GetMeshCenter("mMesh1");
#local RgVel = Rg_GetVelocity("mRg1");
Mesh_DrawVector(Center, RgVel)

 
/*#declare Position = ReadVect("position");
#declare Velocity = ReadVect("velocity");

POVYX_CalculateFrame(Position, Velocity)

StoreVar("position", Position)
StoreVar("velocity", Velocity)

Mesh_RenderVertex("mMesh")
Mesh_RenderVertex("mMesh1")
    
sphere{Position, 0.1 pigment{Yellow}}
Mesh_DrawVector(Position, Velocity) */


light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-3, 3, -10> 
    look_at < 0,0,0 > 
    angle 60
}