#ifndef(POVYX_INC_VAR)
#declare POVYX_INC_VAR = 0;

#include "Mesh.inc" 
#include "DiskArray.inc" 
#include "Rigidbody.inc"

#macro POVYX_Init()
    DiskArray_CreateDiskArray("colliderList")
    DiskArray_CreateDiskArray("constantForceList")
    setAttr("POVYX", "airDrag", 0.01)
    setAttr("POVYX", "framerate", 24)
#end

#macro POVYX_SetAirDrag(Val)
    setAttr("POVYX", "airDrag", Val)
#end

#macro POVYX_SetFramerate(Val)
    setAttr("POVYX", "framerate", Val)
#end

#macro POVYX_AddConstantForce(Force)
    DiskArray_Add("constantForceList", Force)
#end                                          

#macro POVYX_AddCollider(MeshName, Bouncyness, SurfaceDrag)
    DiskArray_Add("colliderList", concat("\"", MeshName, "\""))
    setAttr(MeshName, "bouncyness", Bouncyness)
    setAttr(MeshName, "surfaceDrag", SurfaceDrag)
#end

#macro POVYX_CalculateFrame(Position, Velocity)
    #local Framerate = getFloatAttr("POVYX", "framerate");
    #local AirDrag = getFloatAttr("POVYX", "airDrag");
                                        
    //air drag                                     
    #declare Velocity = Velocity * (1-AirDrag);
    //constant forces
    #declare Velocity = POVYX_ApplyConstantForces(Velocity, Framerate);
    //apply to position 
    #declare Position = Position + (Velocity* (1/Framerate));
    //check for collision and correct
    POVYX_ApplyCollisions(Position, Velocity, Framerate)
    
         
#end 

#macro POVYX_ApplyConstantForces(Velocity, Framerate)
    #local NbConstantForces = DiskArray_GetCount("constantForceList");
    #local Return = Velocity;
    #for(I, 0, NbConstantForces-1)
        #local Force = DiskArray_GetVect("constantForceList", I);
        #local Return = Return + (Force*(1/Framerate));
    #end                                               
    Return;
#end

#macro POVYX_ApplyCollisions(Position, Velocity, Framerate)
    #local NbCollider = DiskArray_GetCount("colliderList");
    #for(I, 0, NbCollider-1)
        #local MeshName = DiskArray_GetStr("colliderList", I)
        #local IsInside = Mesh_IsPointInsideMesh(MeshName, Position);
        #if(IsInside)
            #declare Position = Mesh_PushPointOutside(MeshName, Position);
            #local ClosestFace = Mesh_GetFaceClosestToPoint(MeshName, Position);
            #local Normal = Mesh_GetFaceNormal(MeshName, ClosestFace);
            #local Bouncyness = getFloatAttr(MeshName, "bouncyness");
            #local SurfaceDrag = getFloatAttr(MeshName, "surfaceDrag");
            
            #declare Velocity = Velocity + (Normal * Bouncyness);      
            #declare Velocity = Velocity * (1-SurfaceDrag);
        #end
    #end
#end

#macro POVYX_CalculateRgFrame(RgName)
    #local Framerate = getFloatAttr("POVYX", "framerate");
    #local AirDrag = getFloatAttr("POVYX", "airDrag");
    
    #local Velocity = Rg_GetVelocity(RgName);
    #local Velocity = Velocity * (1-AirDrag);
    //constant forces
    #local Velocity = POVYX_ApplyConstantForces(Velocity, Framerate);
    Rg_SetVelocity(RgName, Velocity)
    Rg_ApplyVelocity(RgName, Framerate)
    
    
    #local NbCollider = DiskArray_GetCount("colliderList");
    #for(I, 0, NbCollider-1)
        #local MeshName = DiskArray_GetStr("colliderList", I)
        
        Rg_PushVectIfCollidingWithMesh(RgName, MeshName)
    #end
    
    
    
#end

#macro POVYX_ApplyRgConstantForces(RgName)

#end

#macro POVYX_ApplyRgCollision(RgMesh, Framerate)

#end




 

#end // end POVYX.inc

