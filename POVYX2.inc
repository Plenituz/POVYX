#ifndef(POVYX2_INC_VAR)
#declare POVYX2_INC_VAR = 0;
#include "DiskArray.inc"


#macro POVYX2_Init()
    DiskArray_CreateDiskArray("POVYX2.colliderList")
    DiskArray_CreateDiskArray("POVYX2.constantForceList")
    DiskArray_CreateDiskArray("POVYX2.softBodies")
    setAttr("POVYX2", "airDrag", 0.01)
    setAttr("POVYX2", "framerate", 24)
#end

#macro POVYX2_SetAirDrag(Val)
    setAttr("POVYX2", "airDrag", Val)
#end

#macro POVYX2_SetFramerate(Val)     
    setAttr("POVYX2", "framerate", Val)
#end

#macro POVYX2_AddConstantForce(Force)
    DiskArray_Add("POVYX2.constantForceList", Force)
#end 

#macro POVYX2_AddCollider(SoftMeshName)
    //this mesh is not going to move, or at least not by the collider system
    //you (probably) can add a collider as a softbody, that will might probably work :3
    DiskArray_Add("POVYX2.colliderList", concat("\"", SoftMeshName, "\""))
#end

#macro POVYX2_AddSoftBody(SoftMeshName)
    DiskArray_Add("POVYX2.softBodies", concat("\"", SoftMeshName, "\""))
#end

#macro POVYX2_Update()
    POVYX2_ApplyConstantForces()
    POVYX2_UpdateSoftMeshes() 
    POVYX2_ApplyCollisions()
    
    //update mesh : la mesh se reform et apply velocity et torque
#end                                                             

#macro POVYX2_ApplyConstantForces()
    #local ConstantForcesCount = DiskArray_GetCount("POVYX2.constantForceList");
    #local Framerate = getFloatAttr("POVYX2", "framerate");
    
    #for(I, 0, ConstantForcesCount-1)
        #local Force = DiskArray_GetVect("POVYX2.constantForceList", I); 
        #local Force = Force*(1/Framerate);
        POVYX2_ApplyConstantForce(Force)
    #end
#end 

#macro POVYX2_ApplyConstantForce(Force)
    
    #local SoftBodyCount = DiskArray_GetCount("POVYX2.softBodies");
    #for(I, 0, SoftBodyCount-1)
        #local SoftMeshName = DiskArray_GetStr("POVYX2.softBodies", I)
        SoftMesh_AddToVelocityUniformly(SoftMeshName, Force)
    #end
#end

#macro POVYX2_ApplyCollisions()
    #local ColliderCount = DiskArray_GetCount("POVYX2.colliderList");
    
    #for(I, 0, ColliderCount-1)
        #local ColliderName = DiskArray_GetStr("POVYX2.colliderList", I)
        POVYX2_ApplyCollision(ColliderName)
        //check for same name 
    #end
#end

#macro POVYX2_ApplyCollision(ColliderName)
    #local SoftBodyCount = DiskArray_GetCount("POVYX2.softBodies");
    #for(I, 0, SoftBodyCount-1)
        #local SoftMeshName = DiskArray_GetStr("POVYX2.softBodies", I) 
        
           
        
        #if(SoftMeshName != ColliderName)     
            #local UpdateThisCollider = getFloatAttr(ColliderName, "updateBoxColliderEveryFrameIfCollider");
            //update the simplified box collider                                                            
            #if(UpdateThisCollider)
                SoftMeshCollision_BuildBoxCollider(ColliderName)
            #end
            SoftMeshCollision_BuildBoxCollider(SoftMeshName) 
            //check if the meshs might collide with the box colliders first
            #local MightCollide = SoftMeshCollision_DoMeshsBoxCollidersIntersect(SoftMeshName, ColliderName);
            #if(MightCollide)
                #debug concat("\napplying collide on ", ColliderName, " (collider) with ", SoftMeshName, " (softbody)\n")
                SoftMeshCollision_CollideWith(SoftMeshName, ColliderName)
            #else
                #debug concat("\nno box collision in between ", ColliderName, " (collider) and ", SoftMeshName, " (softbody)\n" )
            #end
            
            
            
        #end
    #end
#end

#macro POVYX2_UpdateSoftMeshes()
    #local Framerate = getFloatAttr("POVYX2", "framerate");
    #local AirDrag = getFloatAttr("POVYX2", "airDrag");

    #local SoftBodyCount = DiskArray_GetCount("POVYX2.softBodies");
    #for(I, 0, SoftBodyCount-1)
        #local SoftMeshName = DiskArray_GetStr("POVYX2.softBodies", I)
        SoftMesh_Update(SoftMeshName, Framerate, AirDrag)
        //SoftMesh_SlowlyReform(SoftMeshName)
    #end
#end

#macro POVYX2_CacheSimulation(FrameNumber)
    #local FileContent = "#ifndef(POVYX2_CACHE_INC_VAR)\n#declare POVYX2_CACHE_INC_VAR = 1;"
    #local SoftBodyCount = DiskArray_GetCount("POVYX2.softBodies");
    
    #for(I, 0, FrameNumber)
        #debug concat("\nFRAME=", str(I,0,0), "\n")
        #local ThisFrameStr = concat("#declare Frame_", str(I,0,0)," = union{")

        #for(K, 0, SoftBodyCount-1)
            #local SoftMeshName = DiskArray_GetStr("POVYX2.softBodies", K)
            #local ThisSoftMesh = SoftMesh_GetMeshString(SoftMeshName);
            #local ThisFrameStr = concat(ThisFrameStr, "\n// ", SoftMeshName);
            #local ThisFrameStr = concat(ThisFrameStr, "\n\t", ThisSoftMesh);
        #end
        #local ThisFrameStr = concat(ThisFrameStr, "\n}");
        #local FileContent = concat(FileContent, "\n", ThisFrameStr, "\n");        
        
        POVYX2_Update()
    #end 
    #local FileContent = concat(FileContent, "\n#end // end POVYX2Cache.inc")
    StoreVar("POVYX2Cache.inc", FileContent)
#end

#macro POVYX2_ResetCacheRead()
    StoreVar("cacheReadFrame", 0)
#end
 
#macro POVYX2_ReadNextCachedFrame(FilePath)
    #local FileExists = file_exists("cacheReadFrame");
    #if(!FileExists)
        POVYX2_ResetCacheRead()
    #end 
                          
    #include FilePath 
    #local CurrentFrame = ReadFloat("cacheReadFrame");
    Parse_String(concat("Frame_", str(CurrentFrame,0,0)))
    #local CurrentFrame = CurrentFrame+1;
    StoreVar("cacheReadFrame", CurrentFrame)
#end




#end //end POVYX2.inc