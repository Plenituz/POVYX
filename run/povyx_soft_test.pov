#include "SoftMesh.inc" 
#include "colors.inc"
#include "POVYX2.inc"
#include "SoftMeshCollision.inc" 
#include "SphereMesh.inc"

/*SoftMesh_CreateSoftMesh("mSoftMesh", 
array[6]{
<0,0,0>,
<1,0,0>,
<1,0,1>,
<0,1,0>,
<1,1,0>,
<1,1,1>
}, 
array[6]{
0, 1, 2,
3, 4, 5
}, 6, 6)*/

//povray +h800 +w800 +KFF48 +L"/home/dream/Bureau/dopov/POVYX"  povyx_soft_test.pov 
 

#macro DrawAxis()
    sphere_sweep{
        linear_spline, 
        2,
        0, 0.05
        <1,0,0>, 0.08
        pigment{Red}
       
    } 
    sphere_sweep{
        linear_spline, 
        2,
        0, 0.05
        <0,1,0>, 0.08
        pigment{Yellow}
       
    }
    sphere_sweep{
        linear_spline, 
        2,
        0, 0.05
        <0,0,1>, 0.08
        pigment{Blue}
       
    }
#end


//DrawAxis()

#macro mSoftMesh_GetTriangleShading(TriangleIndex, OutDecl)
	#local TriCount = SoftMesh_GetTriangleCount("mSoftMesh");
	#local Val = rand(seed(TriangleIndex));
	#local Val = str(Val, 0, -1);
	#declare OutDecl = concat("#declare MA_TEXT_", str(TriangleIndex, 0, 0), " = texture { pigment { color rgb<", Val, ", ", Val, ", ", Val, "> } }");	
	#local R = concat(" texture{MA_TEXT_", str(TriangleIndex, 0, 0),"}");
	R;
#end


#if(clock = 0)
    SoftMesh_CreateSoftMesh("mSoftMesh", VertexList, TriangleList, 47, 282, <0,0,0>, 0)
	setAttr("mSoftMesh", "triangleShadingEnabled", 1)   
     
     
     #declare CubeSize = 1;
#declare VertexList = array[8]{
    <-CubeSize, -CubeSize, -CubeSize>,
    <-CubeSize,  CubeSize, -CubeSize>,
    < CubeSize,  CubeSize, -CubeSize>,
    < CubeSize, -CubeSize, -CubeSize>,
    < CubeSize, -CubeSize,  CubeSize>,
    < CubeSize,  CubeSize,  CubeSize>,
    <-CubeSize,  CubeSize,  CubeSize>,
    <-CubeSize, -CubeSize,  CubeSize>
};
#declare TriangleList = array[36]{
    0, 1, 2, //   1: face arrière
    0, 2, 3,
    3, 2, 5, //   2: face droite
    3, 5, 4,
    5, 2, 1, //   3: face dessue
    5, 1, 6,
    3, 4, 7, //   4: face dessous
    3, 7, 0,
    0, 7, 6, //   5: face gauche
    0, 6, 1,
    4, 5, 6, //   6: face avant
    4, 6, 7
}; 
         
    SoftMesh_CreateSoftMesh("mSoftMeshCollider", VertexList, TriangleList, 8, 36, <0,0,0>, 0)
	SoftMesh_ScaleMesh("mSoftMeshCollider", <10,10,10>, 0)    
    SoftMesh_MoveMeshUniformly("mSoftMeshCollider", <0,-13,0>)
    SoftMesh_SetBouncyness("mSoftMeshCollider", 0)  
    
    POVYX2_Init()
    POVYX2_AddSoftBody("mSoftMesh") 
    POVYX2_AddConstantForce(<0,-9,0>)
	//POVYX2_AddConstantForce(<-1,0,0>)
    POVYX2_AddCollider("mSoftMeshCollider")
    POVYX2_ResetCacheRead()
#end             
//box{-1, 1 pigment{Grey} rotate <0,0,0> scale 10 translate <0, -13, 0>}
//SoftMesh_DrawDebugMesh("mSoftMeshCollider")

#if(0)
    SoftMesh_DrawDebugMesh("mSoftMesh")
    //SoftMesh_DrawAsMesh("mSoftMesh") 
    
    POVYX2_Update() 
#else
    //POVYX2_CacheSimulation(27)
    POVYX2_ReadNextCachedFrame("POVYX2Cache.inc")
#end
SoftMesh_DrawAsMesh("mSoftMeshCollider")






light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <0, 3, -10> 
    look_at < 0,-3,0 > 
    angle 80
}