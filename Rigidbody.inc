#ifndef(RIGIDBODY_INC_VAR)
#declare RIGIDBODY_INC_VAR = 1;  
    
#macro Rg_CreateRg(MeshName, RgName, Velocity)
    setAttr(RgName, "mesh", concat("\"", MeshName, "\""))
    setAttr(RgName, "velocity", Velocity)
#end 

#macro Rg_GetVelocity(RgName)
    #local Vel = getVectAttr(RgName, "velocity");
    Vel;
#end 

#macro Rg_SetVelocity(RgName, Vel)
    setAttr(RgName, "velocity", Velocity)
#end

#macro Rg_PushVectIfCollidingWithMesh(RgName, OtherMesh)
    #local RgMesh = getStrAttr(RgName, "mesh")
    //pour chaque point de notre mesh
    //check si ca collide avec l'autre mesh
    ////faire la meme avec l'autre mesh    
       
    #local Coll1 = Mesh_IsMeshCollidingWithMe(RgMesh, OtherMesh);
    //^otherMesh is inside RgMesh
    #local Coll2 = Mesh_IsMeshCollidingWithMe(OtherMesh, RgMesh);
    //^RgMesh is inside otherMesh
    #while(Coll1 | Coll2)
        #if(Coll1)
            #declare PushVect = Mesh_GetPushMeshOutsideOfMeVect(RgMesh, OtherMesh);
        #else
            #if(Coll2)
                #declare PushVect = Mesh_GetPushMeshOutsideOfMeVect(OtherMesh, RgMesh);
            #end
        #end 
        Mesh_MoveMesh(RgMesh, PushVect)        
        #debug concat("\npushing In RG GetPush=", vstr(3, PushVect, "/", 0, -1), "\n")  
        #local Coll1 = Mesh_IsMeshCollidingWithMe(RgMesh, OtherMesh);
        #local Coll2 = Mesh_IsMeshCollidingWithMe(OtherMesh, RgMesh);
    #end
     
    //si c'est le cas trouver le point qui collide le plus profond  et 
    // pousser la mesh d'autant qu"il est profond
    
     
                                                                     
#end 

#macro Rg_ApplyVelocity(RgName, Framerate)
     //#declare Position = Position + (Velocity* (1/Framerate));
     #local Velocity = getVectAttr(RgName, "velocity");
     #local MeshName = getStrAttr(RgName, "mesh")
     #local MoveBy = Velocity*(1/Framerate);
     
     Mesh_MoveMesh(MeshName, MoveBy)
#end

#end //end Rigidbody.inc