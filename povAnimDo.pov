//+KFF n keyframe final (nbr d'image demandé)
//+KFI n keyframe initial (numeor de la premiere image)
//+KCI x valeur initale de l'horloge
//+KCF x valeur finale de l'horloge
//l'horloge va de 0 a 1 de base, c'est mieux de le garder
//+KC horloge prévue pour les anims cycliques 
//est ce qu'on peut transmettre une valeur d'une frame a une autre ?
// attrib ? read file ?

//ffmpeg -framerate 24 -i povAnimDo%02d.png anim.mp4
//povray +ipovAnimDo.pov +h800 +w800 +KFF30

#version 3.7;
#include "colors.inc"   
#include "Math.inc"
#include "transforms.inc"

#declare G1 = box{<-1,0,-1>, <1,2,1>}
#declare M1 = material{texture{pigment{bozo}}}
#declare H1 = object{G1 material{M1}}

plane{y, 0 pigment{checker White White*.7}}
plane{z, 10 pigment{cells}}
//object{H1 rotate<0, /*clock*/0*2*360,0> translate <0, (sin(/*clock*/0*10*2*pi)+1)*3, 0>}

light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-3, 3, -10> 
    look_at < 0,1,0 > 
    angle 60
}


#macro ReadVect(StrFilePath)
	#fopen mFile StrFilePath read
	#local Result = <0, 0, 0>;
	#read (mFile, Result)
	#fclose mFile
	Result;
#end

#macro ReadFloat(StrFilePath)
	#fopen mFile StrFilePath read
	#local Result = 0;
	#read (mFile, Result)
	#fclose mFile
	Result; 
#end 


#macro ReadStr(StrFilePath)
	#fopen mFile StrFilePath read
	#local Result = "";
	#read (mFile, Result)
	#fclose mFile
	Result;
#end

#macro StoreVar(StrFilePath, VarToStore)
	#fopen mFile StrFilePath write
	#write (mFile, VarToStore)
	#fclose mFile
#end

#macro setAttr(StrVarName, StrAttrName, Val)
    StoreVar(concat(StrVarName, ".", StrAttrName), Val)
#end                                                   

#macro getStrAttr(StrVarName, StrAttrName)
    ReadStr(concat(StrVarName, ".", StrAttrName))
#end                                             

#macro getVectAttr(StrVarName, StrAttrName)
    ReadVect(concat(StrVarName, ".", StrAttrName))
#end                                              

#macro getFloatAttr(StrVarName, StrAttrName)
    ReadFloat(concat(StrVarName, ".", StrAttrName))
#end

#macro isPointInsideCube(Point, CubeName)
    #if(DEBUG) #debug concat("isPointInsideCube ", CubeName, vstr(3, Point, "/", 0, 1), "\n") #end
    #local TopBackRight = <0,0,0>;
    #local TopBackLeft = <0,0,0>;
    #local TopFrontRight = <0,0,0>;
    #local TopFrontLeft = <0,0,0>;
    #local BotBackRight = <0,0,0>;
    #local BotBackLeft = <0,0,0>;
    #local BotFrontRight = <0,0,0>;
    #local BotFrontLeft = <0,0,0>; 
    readAllCubePoints(CubeName, TopBackRight, TopBackLeft, TopFrontRight, TopFrontLeft,
    BotBackRight, BotBackLeft, BotFrontRight, BotFrontLeft)
    
    
    #local TopRes = isPointBelowPlane(TopBackRight, TopBackLeft, TopFrontLeft, Point);
    #local BotRes = isPointBelowPlane(BotFrontRight, BotFrontLeft, BotBackLeft, Point);
    #local RightRes = isPointBelowPlane(BotFrontRight, BotBackRight, TopFrontRight, Point);
    #local LeftRes = isPointBelowPlane(TopFrontLeft, TopBackLeft, BotFrontLeft, Point);
    #local FrontRes = isPointBelowPlane(TopFrontRight, TopFrontLeft, BotFrontRight, Point);
    #local BackRes = isPointBelowPlane(BotBackRight, BotBackLeft, TopBackRight, Point);
    #local Add = TopRes + BotRes + RightRes + LeftRes + FrontRes + BackRes;
    //#debug concat(str(TopRes, 0, 1)," ", str(BotRes, 0, 1), " ",str(RightRes, 0, 1)," ", str(LeftRes, 0, 1), " ",str(FrontRes, 0, 1), " ",str(BackRes, 0, 1), "\n")
    
    
    #if(Add = 0)   //TODO CA CEST 6 NORMALEMENT 
        #local Return = 1;
    #else
        #local Return = 0;
    #end                   
    Return;    
#end

#macro readAllCubePoints(Name, TopBackRight, TopBackLeft, TopFrontRight, TopFrontLeft,
    BotBackRight, BotBackLeft, BotFrontRight, BotFrontLeft)
    #if(DEBUG) #debug concat("readAllCubePoint ", Name, "\n") #end
    #declare TopBackRight = getVectAttr(Name, "TopBackRight");
    #declare TopBackLeft = getVectAttr(Name, "TopBackLeft");
    #declare TopFrontRight = getVectAttr(Name, "TopFrontRight");
    #declare TopFrontLeft = getVectAttr(Name, "TopFrontLeft");
    #declare BotBackRight = getVectAttr(Name, "BotBackRight");
    #declare BotBackLeft = getVectAttr(Name, "BotBackLeft");
    #declare BotFrontRight = getVectAttr(Name, "BotFrontRight");
    #declare BotFrontLeft = getVectAttr(Name, "BotFrontLeft");
#end

#macro getAllMidPoints(CubeName)
    #if(DEBUG) #debug concat("getAllMidPoints ", CubeName, "\n") #end
    #local TopBackRight = <0,0,0>;
    #local TopBackLeft = <0,0,0>;
    #local TopFrontRight = <0,0,0>;
    #local TopFrontLeft = <0,0,0>;
    #local BotBackRight = <0,0,0>;
    #local BotBackLeft = <0,0,0>;
    #local BotFrontRight = <0,0,0>;
    #local BotFrontLeft = <0,0,0>; 
    readAllCubePoints(CubeName, TopBackRight, TopBackLeft, TopFrontRight, TopFrontLeft,
    BotBackRight, BotBackLeft, BotFrontRight, BotFrontLeft)
    
    #local Top = (TopBackRight + TopBackLeft + TopFrontRight + TopFrontLeft)/4.0 ;
    #local Bot = (BotBackRight + BotBackLeft + BotFrontRight + BotFrontLeft)/4.0 ;
    #local Right = (TopBackRight + TopFrontRight + BotBackRight + BotFrontRight)/4.0;
    #local Left = (TopBackLeft + TopFrontLeft + BotBackLeft + BotFrontLeft)/4.0;
    #local Front = (TopFrontRight + TopFrontLeft + BotFrontRight + BotFrontLeft)/4.0;
    #local Back = (TopBackRight + TopBackLeft + BotBackRight + BotBackLeft)/4.0;
    #local Return = array[6]{Top, Bot, Right, Left, Front, Back};
    Return
#end

#macro getClosestMidPointForPoint(Point, CubeName)
    #if(DEBUG) #debug concat("getClosestMidPointForPoint ", CubeName, vstr(3, Point, "/", 0, 1), "\n") #end
    #local AllPoints = getAllMidPoints(CubeName);
    #local ClosestPoint = AllPoints[0];
    #local ClosestDist = vlength(ClosestPoint - Point);
    
    #for(I, 0, 5)
        #local Len = vlength(AllPoints[I] - Point);
        #if(Len < ClosestDist)
            #local ClosestPoint = AllPoints[I];
            #local ClosestDist = Len;
        #end
    #end
    
    ClosestPoint;
    
#end

#macro getNormalForMidPoint(CubeName, MidPoint)
    #if(DEBUG) #debug concat("getNormalForMidPoint ", CubeName, vstr(3, MidPoint, "/", 0, 1), "\n") #end
    #local AllPoints = getAllMidPoints(CubeName)
    #local TopBackRight = <0,0,0>;
    #local TopBackLeft = <0,0,0>;
    #local TopFrontRight = <0,0,0>;
    #local TopFrontLeft = <0,0,0>;
    #local BotBackRight = <0,0,0>;
    #local BotBackLeft = <0,0,0>;
    #local BotFrontRight = <0,0,0>;
    #local BotFrontLeft = <0,0,0>; 
    readAllCubePoints(CubeName, TopBackRight, TopBackLeft, TopFrontRight, TopFrontLeft,
    BotBackRight, BotBackLeft, BotFrontRight, BotFrontLeft)
    
    #for(I, 0, 5) 
        
        #if(VEq(MidPoint, AllPoints[I]))
           #switch(I)
               #case (0)
                    #local P1 = TopBackRight;
                    #local P2 = TopBackLeft;
                    #local P3 = TopFrontLeft; 
               #break
               #case (1)
                    #local P1 = BotFrontRight;
                    #local P2 = BotFrontLeft;
                    #local P3 = BotBackLeft;
               #break
               #case (2)
                    #local P1 = BotFrontRight;
                    #local P2 = BotBackRight;
                    #local P3 = TopFrontRight;
               #break
               #case (3)       
                    #local P1 = TopFrontLeft;
                    #local P2 = TopBackLeft;
                    #local P3 = BotFrontLeft;
               #break
               #case (4)       
                    #local P1 = TopFrontRight;
                    #local P2 = TopFrontLeft;
                    #local P3 = BotFrontRight;
               #break
               #case (5)       
                    #local P1 = BotBackRight;
                    #local P2 = BotBackLeft;
                    #local P3 = TopBackRight;
               #break
           #end 
        #end
    #end
    #local Return = vcross(P2-P1, P3-P1);
    
    Return;
#end 

#macro dispAllNormal(CubeName)
    #local All = getAllMidPoints(CubeName);
    #for(I, 0, 5)
        #local Normal = getNormalForMidPoint(CubeName, All[I])
        sphere{All[I], 0.1 pigment{Red}}
        sphere{All[I] + Normal, 0.2 pigment{Orange}}
    #end        
#end

#macro getNormalForPoint(Point, CubeName)
    #if(DEBUG) #debug concat("getNormalForPoint ", CubeName, vstr(3, Point, "/", 0, 1)) #end
     #local ClosestMidPoint = getClosestMidPointForPoint(Point, CubeName);    
     #local Normal = getNormalForMidPoint(CubeName, ClosestMidPoint);
     -Normal;//TODO LA NORMALE EST INVERSER LUL, TESTER SI CA LE FAIT AVEC TOUS LES COTES
#end 

#macro getCorrectedCollisionPosition(Point, CubeName, InverseResolution)
     #if(DEBUG) #debug concat("getCorrectedCollisionPosition ", CubeName, vstr(3, Point, "/", 0, 1),", ", str(InverseResolution, 0, 1), "\n") #end
    //can only be used if you know it's colliding 
    //inverse res : the smallest the more precise and more computer intense
    
    #local Normal = getNormalForPoint(Point, CubeName)
    #local IsInside = isPointInsideCube(Point, CubeName)
    
    #while(IsInside)
        #local Point = Point + (Normal * InverseResolution);  
        #local IsInside = isPointInsideCube(Point, CubeName);
    #end                                                     
    Point;
#end

#macro isPointBelowPlane(P1, P2, P3, Point)
    #if(DEBUG) #debug concat("isPointBelowPlane ", vstr(3, P1, "/", 0, 1), " ,", vstr(3, P2, "/", 0, 1), " ,", vstr(3, P3, "/", 0, 1), ",", vstr(3, Point, "/", 0, 1), "\n") #end
//P1 P2 et P3 definissent le plan
    #local UpVect = vcross(P2-P1, P3-P1);
    #local DotProd = vdot(UpVect, Point - P1);
    #if(DotProd < 0)
        #local Result = 1;
    #else
        #local Result = 0;
    #end                  
    Result
#end

#macro addCubeCollider(Corner1, Corner2, Bouncyness, SurfaceDrag, Rotation, RotateAnchor)     
    #local Name = concat("CubeColl_", str(NbCubeCollider, 0, 0));
    
    
    #local TopBackRight = Corner2;
    #local TopBackLeft = <Corner1.x, Corner2.y, Corner2.z>;
    #local TopFrontRight = <Corner2.x, Corner2.y, Corner1.z>;
    #local TopFrontLeft = <Corner1.x, Corner2.y, Corner1.z>;
    #local BotBackRight = <Corner2.x, Corner1.y, Corner2.z>;
    #local BotBackLeft = <Corner1.x, Corner1.y, Corner2.z>;
    #local BotFrontRight = <Corner2.x, Corner1.y, Corner1.z>;
    #local BotFrontLeft = Corner1;
    
    #local TopBackRight = vtransform(TopBackRight,Rotate_Around_Trans(Rotation, RotateAnchor));
    #local TopBackLeft = vtransform(TopBackLeft,Rotate_Around_Trans(Rotation, RotateAnchor));
    #local TopFrontRight = vtransform(TopFrontRight,Rotate_Around_Trans(Rotation, RotateAnchor));
    #local TopFrontLeft = vtransform(TopFrontLeft,Rotate_Around_Trans(Rotation, RotateAnchor));
    #local BotBackRight = vtransform(BotBackRight,Rotate_Around_Trans(Rotation, RotateAnchor));
    #local BotBackLeft = vtransform(BotBackLeft,Rotate_Around_Trans(Rotation, RotateAnchor));
    #local BotFrontRight = vtransform(BotFrontRight,Rotate_Around_Trans(Rotation, RotateAnchor));
    #local BotFrontLeft = vtransform(BotFrontLeft,Rotate_Around_Trans(Rotation, RotateAnchor));
    
    
    setAttr(Name, "TopBackRight", TopBackRight)
    setAttr(Name, "TopBackLeft", TopBackLeft)
    setAttr(Name, "TopFrontRight", TopFrontRight)
    setAttr(Name, "TopFrontLeft", TopFrontLeft)
    setAttr(Name, "BotBackRight", BotBackRight)
    setAttr(Name, "BotBackLeft", BotBackLeft)
    setAttr(Name, "BotFrontRight", BotFrontRight)
    setAttr(Name, "BotFrontLeft", BotFrontLeft)
    
    setAttr(Name, "bouncyness", Bouncyness)
    setAttr(Name, "surfaceDrag", SurfaceDrag) 
    
    sphere{TopBackRight, .1 pigment{Blue}}
    sphere{TopBackLeft, .1 pigment{Blue}}
    sphere{TopFrontRight, .1 pigment{Blue}}
    sphere{TopFrontLeft, .1 pigment{Blue}}
    sphere{BotBackRight, .1 pigment{Blue}}
    sphere{BotBackLeft, .1 pigment{Blue}}
    sphere{BotFrontRight, .1 pigment{Blue}}
    sphere{BotFrontLeft, .1 pigment{Blue}}
    
    sphere{RotateAnchor, .1 pigment{Yellow}}
    box{Corner1, Corner2
        pigment{Red} Rotate_Around_Trans(Rotation, RotateAnchor)}
    
    #declare NbCubeCollider = NbCubeCollider + 1;
    StoreVar("NbCubeCollider", NbCubeCollider)
#end

#macro addConstantForce(Force)
    #local Name = concat("ConstantForce_", str(NbConstantForces, 0, 0));
    setAttr(Name, "value", Force)

	#declare NbConstantForces = NbConstantForces + 1;
	StoreVar("NbConstantForces", NbConstantForces)
#end 

#macro calculateFrame()
    //air drag
    #declare Direction = Direction*(1-AirDrag);
    
    //constants forces (gravity et autres wind)
    #for(I, 0, NbConstantForces-1) 
        #local Name = concat("ConstantForce_", str(I, 0, 0))
        #local Force = getVectAttr(Name, "value");
    	#declare Direction = Direction + (Force*(1/Framerate));
    #end 
    
    //velocity
    #declare Position = Position + (Direction*(1/Framerate));
    
    //rebond ground
    #if (Position.y < 0)
    	#declare Position = <Position.x, 0, Position.z>;
    	#declare Direction = <Direction.x, Direction.y*-GroundBouncyness, Direction.z>;
    #end
    
    //cube collisions
    #for(I, 0, NbCubeCollider-1)
        #local Name = concat("CubeColl_", str(I, 0, 0)) 
       // dispAllNormal(Name)
        
        #local TopBackRight = <0,0,0>;
        #local TopBackLeft = <0,0,0>;
        #local TopFrontRight = <0,0,0>;
        #local TopFrontLeft = <0,0,0>;
        #local BotBackRight = <0,0,0>;
        #local BotBackLeft = <0,0,0>;
        #local BotFrontRight = <0,0,0>;
        #local BotFrontLeft = <0,0,0>; 
        readAllCubePoints(Name, TopBackRight, TopBackLeft, TopFrontRight, TopFrontLeft,
        BotBackRight, BotBackLeft, BotFrontRight, BotFrontLeft)        
        /*sphere{TopBackRight, .1 pigment{Blue}}
        sphere{TopBackLeft, .1 pigment{Blue}}
        sphere{TopFrontRight, .1 pigment{Blue}}
        sphere{TopFrontLeft, .1 pigment{Blue}}
        sphere{BotBackRight, .1 pigment{Blue}}
        sphere{BotBackLeft, .1 pigment{Blue}}
        sphere{BotFrontRight, .1 pigment{Blue}}
        sphere{BotFrontLeft, .1 pigment{Blue}}  */
        
        #declare Normal = getNormalForPoint(Position, Name)
        #declare Normal = Normal *.1;                                                    
         sphere{Position + Normal, .1 pigment{Yellow}}                                         
        //#declare C1 = getVectAttr(Name, "BotFrontLeft");
        //#declare C2 = getVectAttr(Name, "TopBackRight");
        #declare Res = isPointInsideCube(Position, Name)
        //box{C1, C2 pigment{Red}}
        
        #if(Res)
            #declare Position = getCorrectedCollisionPosition(Position, Name, 0.001)
            #declare Normal = getNormalForPoint(Position, Name); 
           
            
            //bounce
            #declare Direction = Direction + Normal * getFloatAttr(Name, "bouncyness");  
            
            //surface drag (friction kinda)
            #declare Direction = Direction - Direction * getFloatAttr(Name, "surfaceDrag");
            
        #end
    #end
    object{H1 scale 0.1 rotate<0,0,0> translate Position}
#end

#macro calculateFrames(MaxFrame)
    #for(FRAME, 0, MaxFrame)
       calculateFrame() 
    #end   //end FRAME LOOP
#end

#if(clock = 0)
//var init
    StoreVar("direction", <3,0,0>)
	StoreVar("position", <-3,5,0>)
    StoreVar("NbCubeCollider", 0)
    StoreVar("NbConstantForces", 0)
#end 
 
//constant values 
#declare AirDrag = 0.01;
#declare GroundBouncyness = 0.7;
#declare Gravity = <0,-9,0>;
#declare Framerate = 24.0;
#declare PREVIEW_MODE = 0;    
#declare DEBUG = 0;

//var reading
#declare NbCubeCollider = ReadFloat("NbCubeCollider")
#declare NbConstantForces = ReadFloat("NbConstantForces")
#declare Direction = ReadVect("direction")
#declare Position = ReadVect("position")

#if(clock = 0) 
//POVSYX INIT
	//addCubeCollider(<-1,0,-1> + <2, -1, 0>, <1,2,1>  + <2, -1, 0>, 10, 0)       
	addCubeCollider(<-1,0,-1>, <1,2,1>, 1, .1, <0,0,25>, (<-1,0,-1> + <1,2,1>)/2)
	//addCubeCollider(<-2,0,-1>, <0,1.5,1>, 1, .1, <0,0,25>, (<-2,0,-1> + <0,1.5,1>)/2)
	addConstantForce(Gravity)
#end     
box{<-1,0,-1>, <1,2,1>
        pigment{Red} Rotate_Around_Trans(<0,0,25>, (<-1,0,-1> + <1,2,1>)/2)} 

box{<-2,0,-1>, <0,1.5,1>
        pigment{Red} Rotate_Around_Trans(<0,0,25>, (<-2,0,-1> + <0,1.5,1>)/2)} 
  
 
//CREER UN  object CUBE AVEC TOP BOT ETC DEDANS
//et permettre la rotation des cube avec ca  et  translation scale etc
//pour collide des objets plus gros, cr�er un object qui a plusieurs points de collision
//et on peut "pousser" un point de collision et les autres vont suivre 
//ou alors on pousse juste tout l'objet d'une coup tien 
                 
                 //LE CLOSEST MID POINT C PAS TJRS LE BON SI LA FORME EST RECTANGULAIRE
#if(PREVIEW_MODE)
    calculateFrames(120)           
    //#local Str = "CubeColl_0";
    //#local mPoi = getClosestMidPointForPoint(<-.7, 1.9,0>, Str)
    //#debug concat("swag is ", vstr(3, mPoi, "/", 0, 1))
#else
    calculateFrame()
#end   
  


//store var for next frame
StoreVar("direction", Direction)
StoreVar("position", Position)




