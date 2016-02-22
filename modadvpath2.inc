Sub SimpleKeyPress(Function,Duration,DelayType)
/if (!${Defined[DelayType]}) {
 /declare DelayType int local
 /varset DelayType 1
}
/if (${Function.Equal[FORWARD]} && ${PBFace.Equal[faceb]}) {
 /varset Function BACK
} else /if (${Function.Equal[BACK]} && ${PBFace.Equal[faceb]}) {
 /varset Function FORWARD
}
/keypress ${Function}
/keypress ${Function} hold
/if (${DelayType}==1) {
 /call Delay ${Duration}
} else {
 /delay ${Duration}
}
/keypress ${Function}
/return

Sub CheckObstacle
/if (${PlotFlag}==0 && ${Math.Distance[${MyLastYLoc},${MyLastXLoc},${MyLastZLoc}]}<1) /call CheckDoor
/if (${PlotFlag}==0 && ${Math.Distance[${MyLastYLoc},${MyLastXLoc},${MyLastZLoc}]}<1) /call HitObstacle
/varset MyLastYLoc ${Me.Y}
/varset MyLastXLoc ${Me.X}
/varset MyLastZLoc ${Me.Z}
/varset ObstacleCount 0
/return

Sub CheckDoor
/declare CharDirection int local
/declare TempVarDoor int local
/declare TempStoreTgt int local
/varset TempStoreTgt 0
/if (${Target.ID}>0) /varset TempStoreTgt ${Target.ID}
/doortarget
/if (${Target.Name.Find["door"]} && ${Target.Distance}<=12) {
 /call Move 0
 /if (${Me.Heading.DegreesCCW}>=315) {
 /look 0
 /if (${PBFace.Equal[facef]}) {
 /face fast nolook heading 0
 /varset CharDirection 0
 } else {
 /face fast nolook heading 180
 /varset CharDirection 180
 }
 /goto :GotHeading
 }
 /if (${Me.Heading.DegreesCCW}<45) {
 /look 0
 /if (${PBFace.Equal[facef]}) {
 /face fast nolook heading 0
 /varset CharDirection 0
 } else {
 /face fast nolook heading 180
 /varset CharDirection 180
 }
 /goto :GotHeading
 }
 /if (${Me.Heading.DegreesCCW}>=45 && ${Me.Heading.DegreesCCW}<135) {
 /look 0
 /if (${PBFace.Equal[facef]}) {
 /face fast nolook heading 90
 /varset CharDirection 90
 } else {
 /face fast nolook heading 270
 /varset CharDirection 270
 }
 /goto :GotHeading
 }
 /if (${Me.Heading.DegreesCCW}>=135 && ${Me.Heading.DegreesCCW}<225) {
 /look 0
 /if (${PBFace.Equal[facef]}) {
 /face fast nolook heading 180
 /varset CharDirection 180
 } else {
 /face fast nolook heading 0
 /varset CharDirection 0
 }
 /goto :GotHeading
 }
 /if (${Me.Heading.DegreesCCW}>=225 && ${Me.Heading.DegreesCCW}<315) {
 /look 0
 /if (${PBFace.Equal[facef]}) {
 /face fast nolook heading 270
 /varset CharDirection 270
 } else {
 /face fast nolook heading 90
 /varset CharDirection 90
 }
 /goto :GotHeading
 }
 :GotHeading
 /varset TempVarDoor 0
 :DoorMoveCloser
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${TempVarDoor}>50) /goto :DoorDoneMoveCloser
 /if (${PBFace.Equal[facef]}) {
 /if (${Target.Distance}>10) /call SimpleKeyPress FORWARD 1
 /if (${Target.Distance}<8) /call SimpleKeyPress BACK 1
 } else {
 /if (${Target.Distance}>10) /call SimpleKeyPress BACK 1
 /if (${Target.Distance}<8) /call SimpleKeyPress FORWARD 1
 }
 /if (${Target.Distance}>10) /goto :DoorMoveCloser
 /if (${Target.Distance}<8) /goto :DoorMoveCloser
 :DoorDoneMoveCloser
 /call Delay 0
 /keypress USE HOLD
 /keypress USE
 /call Delay 1s
 /varset DoorMoveTimer 1s
 /varset TempVarDoor 0
 :DoorMoveAway
 /if (${DoorMoveTimer}>0) {
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${TempVarDoor}>50) /goto :DoorDoneMoveAway
 /if (${PBFace.Equal[facef]}) {
 /if (${Target.Distance}>21) /call SimpleKeyPress FORWARD 1
 /if (${Target.Distance}<16) /call SimpleKeyPress BACK 1
 } else {
 /if (${Target.Distance}>21) /call SimpleKeyPress BACK 1
 /if (${Target.Distance}<16) /call SimpleKeyPress FORWARD 1
 }
 /if (${Target.Distance}>21) /goto :DoorMoveAway
 /if (${Target.Distance}<16) /goto :DoorMoveAway
 :DoorDoneMoveAway
 }
 /call Delay 0
 /if ((${Target.Heading.DegreesCCW}>355)&&(${Target.Heading.DegreesCCW}<5)) {
 /if (${CharDirection}==0) {
 /varset TempVarDoor 0
 /look 0
 /face fast nolook heading 337
 :DoorNorth1
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${PBFace.Equal[facef]}) {
 /call SimpleKeyPress FORWARD 1
 } else {
 /call SimpleKeyPress BACK 1
 }
 /if (${TempVarDoor}<1) /goto :DoorNorth1
 /face fast nolook door
 } else {
 /varset TempVarDoor 0
 /look 0
 /face fast nolook heading 203
 :DoorNorth2
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${PBFace.Equal[facef]}) {
 /call SimpleKeyPress FORWARD 1
 } else {
 /call SimpleKeyPress BACK 1
 }
 /if (${TempVarDoor}<1) /goto :DoorNorth2
 /face fast nolook door
 }
 }
 /if ((${Target.Heading.DegreesCCW}>85)&&(${Target.Heading.DegreesCCW}<95)) {
 /if (${CharDirection}==90) {
 /varset TempVarDoor 0
 /look 0
 /face fast nolook heading 67
 :DoorWest1
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${PBFace.Equal[facef]}) {
 /call SimpleKeyPress FORWARD 1
 } else {
 /call SimpleKeyPress BACK 1
 }
 /if (${TempVarDoor}<1) /goto :DoorWest1
 /face fast nolook door
 } else {
 /varset TempVarDoor 0
 /look 0
 /face fast nolook heading 293
 :DoorWest2
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${PBFace.Equal[facef]}) {
 /call SimpleKeyPress FORWARD 1
 } else {
 /call SimpleKeyPress BACK 1
 }
 /if (${TempVarDoor}<1) /goto :DoorWest2
 /face fast nolook door
 }
 }
 /if ((${Target.Heading.DegreesCCW}>175)&&(${Target.Heading.DegreesCCW}<185)) {
 /if (${CharDirection}==180) {
 /varset TempVarDoor 0
 /look 0
 /face fast nolook heading 157
 :DoorSouth1
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${PBFace.Equal[facef]}) {
 /call SimpleKeyPress FORWARD 1
 } else {
 /call SimpleKeyPress BACK 1
 }
 /if (${TempVarDoor}<1) /goto :DoorSouth1
 /face fast nolook door
 } else {
 /varset TempVarDoor 0
 /look 0
 /face fast nolook heading 23
 :DoorSouth2
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${PBFace.Equal[facef]}) {
 /call SimpleKeyPress FORWARD 1
 } else {
 /call SimpleKeyPress BACK 1
 }
 /if (${TempVarDoor}<1) /goto :DoorSouth2
 /face fast nolook door
 }
 }
 /if ((${Target.Heading.DegreesCCW}>265)&&(${Target.Heading.DegreesCCW}<275)) {
 /if (${CharDirection}==270) {
 /varset TempVarDoor 0
 /look 0
 /face fast nolook heading 247
 :DoorEast1
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${PBFace.Equal[facef]}) {
 /call SimpleKeyPress FORWARD 1
 } else {
 /call SimpleKeyPress BACK 1
 }
 /if (${TempVarDoor}<1) /goto :DoorEast1
 /face fast nolook door
 } else {
 /varset TempVarDoor 0
 /look 0
 /face fast nolook heading 113
 :DoorEast2
 /varset TempVarDoor ${Math.Calc[${TempVarDoor}+1]}
 /if (${PBFace.Equal[facef]}) {
 /call SimpleKeyPress FORWARD 1
 } else {
 /call SimpleKeyPress BACK 1
 }
 /if (${TempVarDoor}<1) /goto :DoorEast2
 /face fast nolook door
 }
 }
 /if (${PBFace.Equal[facef]}) {
 /call Move 1
 } else {
 /keypress FORWARD hold
 /delay 8
 /keypress FORWARD
 /call Move 1
 }
}
/if (${TempStoreTgt}>0) {
 /if (${Spawn[${TempStoreTgt}].ID}) /target id ${TempStoreTgt}
} else {
 /target clear
}
/return

Sub CheckForTarget
/if ((${FollowFlag}==1)&&(${ObstacleCount}<7)&&(!${Target.CleanName.Equal["${FollowName}"]})) {
 /call TargetMob "${FollowName}"
}
/return

Sub CheckMoving
/if (${PauseFlag}==0 && ${Me.Speed}==0) {
 /call Move 1
}
/return

Sub ClearLocs
/varset CurrLoc 1
/varset NumLocs 1
/varset ObstacleCount 0
/varset TotalObstCount 0
/return

Sub Delay(DelayTime)
/varset DelayTimer ${DelayTime}
:DlyLoop
/delay 1
/if (${FollowFlag}==1) /call TrackFollow
/if (${DelayTimer}>0) /goto :DlyLoop
/return

Sub FindLookAngle(TY,TX,TZ)
/declare Angle float local
/declare LevOrWet int local
/declare TempFacing float local
/varset LevOrWet 0
/declare FY float local
/declare FX float local
/declare FZ float local
/varset FZ ${Me.Z}
/if (${Me.FeetWet}) {
 /varset TZ ${Math.Calc[${TZ}+3]}
 /varset LevOrWet ${Math.Calc[${LevOrWet}+1]}
}
/if (${Me.Levitating}) {
 /varset TZ ${Math.Calc[${TZ}+5]}
 /varset LevOrWet ${Math.Calc[${LevOrWet}+1]}
}
/varset FY ${Me.Y}
/varset FX ${Me.X}
/varset Angle ${Math.Abs[${Math.Asin[${If[${Math.Abs[${Math.Abs[${FZ}-${TZ}]}/${Math.Distance[${TY},${TX},${TZ}:${FY},${FX},${FZ}]}]}<=1,${Math.Calc[${Math.Abs[${FZ}-${TZ}]}/${Math.Distance[${TY},${TX},${TZ}:${FY},${FX},${FZ}]}]},1]}]}]}
/if (${FZ}>${TZ}) {
 /varset Angle ${Math.Calc[${Angle}*-1]}
} else /if (${TZ}==${FZ}) {
 /varset Angle 0
}
/if (${LevOrWet}==0) /varset Angle ${Math.Calc[${Math.Calc[${Angle}\20]}*20]}
/if (${FollowFlag}==1 && ${CurrLoc}<3 && ${Me.Speed}==0) {
 /if (${PBFace.Equal[facef]}) {
 /face nolook loc ${TY},${TX}
 } else {
 /face nolook away loc ${TY},${TX}
 }
 :WaitForTurnStart
 /varset TempFacing ${Me.Heading.DegreesCCW}
 /call Delay 1
 /if (${Me.Heading.DegreesCCW}!=${TempFacing}) /goto :WaitForTurnStart
}
/if (${UseFaceFast}==0) {
 /if (${Math.Abs[${Me.Heading.DegreesCCW}-${Heading[${TY},${TX}].DegreesCCW}]}>${If[${Me.Speed}>0,${Math.Calc[2700/${Me.Speed}]},10]}) {
 /call Move 0
 /if (${PBFace.Equal[facef]}) {
 /face nolook loc ${TY},${TX}
 } else {
 /face nolook away loc ${TY},${TX}
 }
 :WaitForTurnLook
 /varset TempFacing ${Me.Heading.DegreesCCW}
 /call Delay 1
 /if (${Me.Heading.DegreesCCW}!=${TempFacing}) /goto :WaitForTurnLook
 /call Delay 1
 } else {
 /if (${PBFace.Equal[facef]}) {
 /face nolook loc ${TY},${TX}
 } else {
 /face nolook away loc ${TY},${TX}
 }
 }
} else {
 /if (${PBFace.Equal[facef]}) {
 /face fast nolook loc ${TY},${TX}
 } else {
 /face fast nolook away loc ${TY},${TX}
 }
}
/look ${Angle}
/return

Sub FollowIncLoc(TY,TX,TZ)
/if (${TY.Equal[NULL]} && ${TX.Equal[NULL]} && ${TZ.Equal[NULL]}) {
 /call Move 0
 /return
}
/declare TempFacing float local
/varset CheckLocTimer 1
/varset ObstacleCount ${Math.Calc[${ObstacleCount}+1]}
/varset TotalObstCount ${Math.Calc[${TotalObstCount}+1]}
/varset PositionTimer 1s
/if (${Spawn[${FollowID}].ID} && ${Spawn[${FollowID}].Distance3D}<=${Math.Calc[${FollowDistance}+${Math.Calc[${Me.Speed}/30]}]}) {
 /call Move 0
 /if (${Spawn[${FollowID}].ID} && ${Spawn[${FollowID}].Speed}==0) {
 /face nolook id ${FollowID}
 :WaitForTurnFoll
 /varset TempFacing ${Me.Heading.DegreesCCW}
 /call Delay 1
 /if (${Me.Heading.DegreesCCW}!=${TempFacing}) /goto :WaitForTurnFoll
 :FollowDistClose
 /if (${PositionTimer}>0 && ${Spawn[${FollowID}].ID}) {
 /if (${Spawn[${FollowID}].Distance3D}>${Math.Calc[${FollowDistance}+8]}) /call SimpleKeyPress FORWARD 1
 /if (${Spawn[${FollowID}].Distance3D}<${Math.Calc[${FollowDistance}-8]}) /call SimpleKeyPress BACK 1
 /if (${Spawn[${FollowID}].Distance3D}>${Math.Calc[${FollowDistance}+8]}) /goto :FollowDistClose
 /if (${Spawn[${FollowID}].Distance3D}<${Math.Calc[${FollowDistance}-8]}) /goto :FollowDistClose
 }
 }
 /call ClearLocs
 /if (${CurrLoc}==1) /varset CurrFollLoc ${Spawn[${FollowID}].Y} ${Spawn[${FollowID}].X} ${Spawn[${FollowID}].Z}
 /return
}
/if ((${Heading[${TY},${TX}].DegreesCCW}<${Math.Calc[${StoreHeading}-90]})||(${Heading[${TY},${TX}].DegreesCCW}>${Math.Calc[${StoreHeading}+90]})||(${Math.Distance[${TY},${TX},${TZ}]}<${If[${Me.Speed}!=0,${Math.Calc[${Math.Calc[${Me.Speed}/${SpeedSense}]}+${DistanceModifier}]},${Math.Calc[${Math.Calc[200/${SpeedSense}]}+${DistanceModifier}]}]})) {
 /varset LastLoc ${LocArray[${CurrLoc}]}
 /varset CurrLoc ${Math.Calc[${CurrLoc}+1]}
 /if (${CurrLoc}<${NumLocs}) /call StoreHeading ${CurrLoc}
 /varset ObstacleCount 0
 /varset TotalObstCount 0
}
/if (${CurrLoc}==${NumLocs}) {
 /call Move 0
 /if (${Spawn[${FollowID}].ID}) {
 /face nolook id ${FollowID}
 :WaitForTurnFoll2
 /varset TempFacing ${Me.Heading.DegreesCCW}
 /call Delay 1
 /if (${Me.Heading.DegreesCCW}!=${TempFacing}) /goto :WaitForTurnFoll2
 :FollowDistClose2
 /if (${PositionTimer}>0 && ${Spawn[${FollowID}].ID} && ${Spawn[${FollowID}].Speed}==0) {
 /if (${Spawn[${FollowID}].Distance3D}<${Math.Calc[${FollowDistance}+8]}) {
 /if (${Spawn[${FollowID}].Distance3D}>${Math.Calc[${FollowDistance}+8]}) /call SimpleKeyPress FORWARD 1
 /if (${Spawn[${FollowID}].Distance3D}<${Math.Calc[${FollowDistance}-8]}) /call SimpleKeyPress BACK 1
 /if (${Spawn[${FollowID}].Distance3D}>${Math.Calc[${FollowDistance}+8]}) /goto :FollowDistClose2
 /if (${Spawn[${FollowID}].Distance3D}<${Math.Calc[${FollowDistance}-8]}) /goto :FollowDistClose2
 }
 }
 }
 /call ClearLocs
 /if (${CurrLoc}==1) /varset CurrFollLoc ${Spawn[${FollowID}].Y} ${Spawn[${FollowID}].X} ${Spawn[${FollowID}].Z}
 /return
}
/if (${ObstacleCount}>6) /call CheckObstacle
/if (${CurrLoc}<${NumLocs}) {
 /if (${TotalObstCount}>50) {
 /varset LastLoc ${LocArray[${CurrLoc}]}
 /varset CurrLoc ${Math.Calc[${CurrLoc}+1]}
 /varset ObstacleCount 0
 /varset TotalObstCount 0
 }
} else {
 /echo -(( Cannot get to location because of obstacles! Aborting. ))-
 /call StopFunction
 /return
}
/if (${NumLocs}>${CurrLoc}) {
 /call FindLookAngle ${LocArray[${CurrLoc}].Arg[1]} ${LocArray[${CurrLoc}].Arg[2]} ${LocArray[${CurrLoc}].Arg[3]}
 /call CheckMoving
}
/return

Sub GetClosestLoc
/declare FindClosest int local
/declare TempClosest float local
/declare Closest int local
/varset TempClosest 9999
/varset Closest 1
/for FindClosest 1 to ${NumLocs}
 /if (${Math.Distance[${LocArray[${FindClosest}].Arg[1]},${LocArray[${FindClosest}].Arg[2]},${LocArray[${FindClosest}].Arg[3]}]}<${TempClosest}) {
 /varset TempClosest ${Math.Distance[${LocArray[${FindClosest}].Arg[1]},${LocArray[${FindClosest}].Arg[2]},${LocArray[${FindClosest}].Arg[3]}]}
 /varset Closest ${FindClosest}
 }
/next FindClosest
/varset CurrLoc ${Closest}
/return

Sub GetCommand
/varset PathingFlag 0
/varset PBPFName ${PlayCommandQueue[${PlayQueueCurr}].Arg[1]}
/varset PBIterations ${PlayCommandQueue[${PlayQueueCurr}].Arg[2]}
/varset PBDirection ${PlayCommandQueue[${PlayQueueCurr}].Arg[3]}
/varset PBPingPong ${PlayCommandQueue[${PlayQueueCurr}].Arg[4]}
/varset PBZone ${PlayCommandQueue[${PlayQueueCurr}].Arg[5]}
/varset PBFace ${PlayCommandQueue[${PlayQueueCurr}].Arg[6]}
/if (${PBDirection.Find["r"]}) {
 /varset Direction reverse
} else {
 /varset Direction forward
}
/if (${PBPingPong.Find["nopp"]}) {
 /varset PingPongCount 0
} else {
 /varset PingPongCount 1
}
/if (${PBIterations.Find["loop"]}) {
 /varset Iterations 1
} else {
 /varset Iterations ${PBIterations}
}
/return
Sub HitObstacle
/call Move 0
/if (${PBFace.Equal[facef]}) {
 /keypress BACK
 /keypress BACK hold
 /call Delay ${Math.Calc[${Math.Rand[3]}+3].Int}
 /keypress BACK
} else {
 /keypress FORWARD
 /keypress FORWARD hold
 /call Delay ${Math.Calc[${Math.Rand[3]}+3].Int}
 /keypress FORWARD
}
/if (${Math.Rand[99]}>49) {
 /keypress RIGHT
 /keypress RIGHT hold
} else {
 /keypress LEFT
 /keypress LEFT hold
}
/call Delay ${Math.Calc[${Math.Rand[3]}+3].Int}
/keypress RIGHT
/keypress LEFT
/call Move 1
/call Delay 3
/keypress JUMP
/call Delay 1
/if (${PBFace.Equal[facef]}) {
 /face fast nolook loc ${LocArray[${CurrLoc}].Arg[1]},${LocArray[${CurrLoc}].Arg[2]}
} else {
 /face fast nolook away loc ${LocArray[${CurrLoc}].Arg[1]},${LocArray[${CurrLoc}].Arg[2]}
}
/return

Sub IncrementLoc(TY,TX,TZ)
/varset CheckLocTimer 1
/declare HeadingDelta int local
/varset ObstacleCount ${Math.Calc[${ObstacleCount}+1]}
/varset TotalObstCount ${Math.Calc[${TotalObstCount}+1]}
/varcalc HeadingDelta ${Math.Abs[${Heading[${TY},${TX}].DegreesCCW}-${StoreHeading}]}
/if (${HeadingDelta}>90 && ${HeadingDelta}<270) {
 /varset LastLoc ${LocArray[${CurrLoc}]}
 /if (${Direction.Equal["forward"]}) {
 /varset CurrLoc ${Math.Calc[${CurrLoc}+1]}
 /call StoreHeading ${CurrLoc}
 } else {
 /varset CurrLoc ${Math.Calc[${CurrLoc}-1]}
 /call StoreHeading ${CurrLoc}
 }
 /varset ObstacleCount 0
 /varset TotalObstCount 0
 /varset TY ${LocArray[${CurrLoc}].Arg[1]}
 /varset TX ${LocArray[${CurrLoc}].Arg[2]}
 /varset TZ ${LocArray[${CurrLoc}].Arg[3]}
}
/if (${PBFace.Equal[facef]}) {
 /if (${Math.Distance[${TY},${TX},${TZ}]}<${If[${Me.Speed}!=0,${Math.Calc[${Math.Calc[${Me.Speed}/${SpeedSense}]}+${DistanceModifier}]},${Math.Calc[${Math.Calc[200/${SpeedSense}]}+${DistanceModifier}]}]}) {
 /varset LastLoc ${LocArray[${CurrLoc}]}
 /if (${Direction.Equal["forward"]}) {
 /varset CurrLoc ${Math.Calc[${CurrLoc}+1]}
 /call StoreHeading ${CurrLoc}
 } else {
 /varset CurrLoc ${Math.Calc[${CurrLoc}-1]}
 /call StoreHeading ${CurrLoc}
 }
 /varset ObstacleCount 0
 /varset TotalObstCount 0
 }
} else {
 /if (${Math.Distance[${TY},${TX},${TZ}]}<${If[${Me.Speed}!=0,${Math.Calc[${Math.Calc[${Me.Speed}/${SpeedSense}]}+${DistanceModifier}+10]},${Math.Calc[${Math.Calc[200/${SpeedSense}]}+${DistanceModifier}+10]}]}) {
 /varset LastLoc ${LocArray[${CurrLoc}]}
 /if (${Direction.Equal["forward"]}) {
 /varset CurrLoc ${Math.Calc[${CurrLoc}+1]}
 /call StoreHeading ${CurrLoc}
 } else {
 /varset CurrLoc ${Math.Calc[${CurrLoc}-1]}
 /call StoreHeading ${CurrLoc}
 }
 /varset ObstacleCount 0
 /varset TotalObstCount 0
 }
}
/if (${Direction.Equal["forward"]}) {
 /if (${CurrLoc}>${NumLocs}) {
 /if (${PingPongCount}==0) {
 /if (${PBIterations.NotEqual["loop"]}) {
 /varset Iterations ${Math.Calc[${Iterations}-1]}
 } else {
 /varset CurrLoc 1
 /call StoreHeading ${CurrLoc}
 }
 /if (${Iterations}==0) {
 /if (${PBZone.Equal["z"]}) /call TryToZone
 /if (${PlayQueueCurr}==${PlayQueueNum}) {
 /call Move 0
 /varset PlayQueueCurr 1
 /varset PlayQueueNum 1
 /varset PathingFlag 0
 /call ClearLocs
 /if (${SilentFlag}==0) /echo -[ Done with playback! ]-
 } else {
 /varset PlayQueueCurr ${Math.Calc[${PlayQueueCurr}+1]}
 /call Move 0 
 /call GetCommand
 /call LoadPathFile "${PBPFName}"
 /if (${Macro.Return}==0) {
 /if (${SilentFlag}==0) /echo -[ Started play back of path file -( ${PBPFName} )- ]-
 /if (${Direction.Equal["forward"]}) {
 /if (${PBDirection.Find["c"]}) {
 /call GetClosestLoc
 /call StoreHeading ${CurrLoc}
 } else {
 /varset CurrLoc 1
 /call StoreHeading ${CurrLoc}
 }
 } else {
 /if (${PBDirection.Find["c"]}) { 
 /call GetClosestLoc
 /call StoreHeading ${CurrLoc}
 } else {
 /varset CurrLoc ${NumLocs}
 /call StoreHeading ${CurrLoc}
 }
 }
 /varset PathingFlag 1
 }
 }
 } else {
 /if (${PBPingPong.Equal["pp"]}) { 
 /varset PingPongCount 1
 /varset Direction reverse
 /varset CurrLoc ${NumLocs}
 /call StoreHeading ${CurrLoc}
 }
 }
 } else {
 /varset PingPongCount ${Math.Calc[${PingPongCount}-1]}
 /varset Direction reverse
 /varset CurrLoc ${NumLocs}
 /call StoreHeading ${CurrLoc}
 }
 }
 /if (${ObstacleCount}>6) /call CheckObstacle
 /if (${TotalObstCount}>50) {
 /varset LastLoc ${LocArray[${CurrLoc}]}
 /if (${CurrLoc}<${NumLocs}) {
 /varset CurrLoc ${Math.Calc[${CurrLoc}+1]}
 /call StoreHeading ${CurrLoc}
 /varset ObstacleCount 0
 /varset TotalObstCount 0
 } else {
 /echo -(( Cannot get to location because of obstacles! Aborting. ))-
 /call StopFunction
 /return
 }
 }
 /if (${PathingFlag}==1) {
 /call FindLookAngle ${LocArray[${CurrLoc}].Arg[1]} ${LocArray[${CurrLoc}].Arg[2]} ${LocArray[${CurrLoc}].Arg[3]}
 /call CheckMoving
 }
} else {
 /if (${CurrLoc}<1) {
 /if (${PingPongCount}==0) {
 /if (${PBIterations.NotEqual["loop"]}) {
 /varset Iterations ${Math.Calc[${Iterations}-1]}
 } else {
 /varset CurrLoc ${NumLocs}
 /call StoreHeading ${CurrLoc}
 }
 /if (${Iterations}==0) {
 /if (${PBZone.Equal["z"]}) /call TryToZone
 /if (${PlayQueueCurr}==${PlayQueueNum}) {
 /call Move 0
 /varset PlayQueueCurr 1
 /varset PlayQueueNum 1
 /varset PathingFlag 0
 /call ClearLocs
 /if (${SilentFlag}==0) /echo -[ Done with playback! ]-
 } else {
 /varset PlayQueueCurr ${Math.Calc[${PlayQueueCurr}+1]}
 /call Move 0
 /call GetCommand
 /call LoadPathFile "${PBPFName}"
 /if (${Macro.Return}==0) {
 /if (${SilentFlag}==0) /echo -[ Started play back of path file -( ${PBPFName} )- ]-
 /if (${Direction.Equal["forward"]}) {
 /if (${PBDirection.Find["c"]}) {
 /call GetClosestLoc
 /call StoreHeading ${CurrLoc}
 } else {
 /varset CurrLoc 1
 /call StoreHeading ${CurrLoc}
 }
 } else {
 /if (${PBDirection.Find["c"]}) {
 /call GetClosestLoc
 /call StoreHeading ${CurrLoc}
 } else {
 /varset CurrLoc ${NumLocs}
 /call StoreHeading ${CurrLoc}
 }
 }
 /varset PathingFlag 1
 }
 }
 } else {
 /if (${PBPingPong.Equal["pp"]}) {
 /varset PingPongCount 1
 /varset Direction forward
 /varset CurrLoc 1
 /call StoreHeading ${CurrLoc}
 }
 }
 } else {
 /varset PingPongCount ${Math.Calc[${PingPongCount}-1]}
 /varset Direction forward
 /varset CurrLoc 1
 /call StoreHeading ${CurrLoc}
 }
 }
 /if (${ObstacleCount}>6) /call CheckObstacle
 /if (${TotalObstCount}>50) {
 /if (${CurrLoc}>1) {
 /varset LastLoc ${LocArray[${CurrLoc}]}
 /varset CurrLoc ${Math.Calc[${CurrLoc}-1]}
 /call StoreHeading ${CurrLoc}
 /varset ObstacleCount 0
 /varset TotalObstCount 0
 } else {
 /echo -(( Cannot get to location because of obstacles! Aborting. ))-
 /call StopFunction
 /return
 }
 }
 /if (${PathingFlag}==1) {
 /call FindLookAngle ${LocArray[${CurrLoc}].Arg[1]} ${LocArray[${CurrLoc}].Arg[2]} ${LocArray[${CurrLoc}].Arg[3]}
 /call CheckMoving
 }
}
/doevents
/return

Sub LoadPathFile(PFIniName)
/declare AdvPathIniFile string local
/varset AdvPathIniFile PathFile_${PFIniName}.ini
/call ClearLocs
:LoadPlayFile
/varset LocArray[${NumLocs}] ${Ini["${AdvPathIniFile}",PFLocs,${Int[${Math.Calc[${NumLocs}-1]}]},NOTFOUND]}
/if (${LocArray[${NumLocs}].Equal["NOTFOUND"]}) {
 /varset NumLocs ${Math.Calc[${NumLocs}-1]}
 /goto :DoneLoadPlayFile
}
/if (${LocArray[${NumLocs}].Equal["Empty"]}) {
 /varset NumLocs ${Math.Calc[${NumLocs}-1]}
 /goto :DoneLoadPlayFile
}
/varset NumLocs ${Math.Calc[${NumLocs}+1]}
/goto :LoadPlayFile
:DoneLoadPlayFile
/if (${NumLocs}<1) {
 /if (${SilentFlag}==0) /echo -(( No pathing information found in -( ${PFIniName} )- ))-
 /varset PathingFlag 0
 /call ClearLocs
 /return 1
}
/return 0

Sub Move(MoveCommand)
/if (${PauseFlag}==0) {
 /if (${MoveCommand}==1) {
 /if (${PBFace.Equal[facef]}) {
 /keypress FORWARD
 /keypress FORWARD hold
 } else {
 /keypress BACK
 /keypress BACK hold
 }
 } else {
 /keypress FORWARD
 /keypress RIGHT
 /keypress LEFT
 /keypress BACK
 }
} else {
 /if (${MoveCommand}==0) {
 /keypress FORWARD
 /keypress RIGHT
 /keypress LEFT
 /keypress BACK
 }
}
/return

Sub RecordPath
/declare TempIndex int local
/if (${ManualRecord}==0) {
 /if (${RecordPathFlag}==1) {
 /if (${NumLocs}==1) {
 /varset LocArray[${NumLocs}] ${Me.Y} ${Me.X} ${Me.Z}
 /varset NumLocs ${Math.Calc[${NumLocs}+1]}
 } else {
 /varset TempIndex ${Math.Calc[${NumLocs}-1]}
 /if (${Math.Distance[${LocArray[${TempIndex}].Arg[1]},${LocArray[${TempIndex}].Arg[2]},${LocArray[${TempIndex}].Arg[3]}]}>12) {
 /varset LocArray[${NumLocs}] ${Me.Y} ${Me.X} ${Me.Z}
 /if (${SilentFlag}==0) /echo Location: Y:${LocArray[${NumLocs}].Arg[1]} X:${LocArray[${NumLocs}].Arg[2]} Z:${LocArray[${NumLocs}].Arg[3]} Plotted
 /varset NumLocs ${Math.Calc[${NumLocs}+1]}
 }
 }
 }
}
/doevents
/return