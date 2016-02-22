#include modadvpath2.inc

#Event LocalRecord "[MQ2] record#*#"
#Event LocalPlot "[MQ2] plot#*#"
#Event LocalDistance "[MQ2] distance#*#"
#Event LocalPlay "[MQ2] play:#*#"
#Event LocalStop "[MQ2] stop#*#"
#Event LocalGoto "[MQ2] goto#*#"
#Event LocalPause "[MQ2] pause#*#"
#Event LocalFollow "[MQ2] follow#*#"
#Event LocalSilent "[MQ2] silent#*#"
#Event LocalPattern "[MQ2] pattern:#*#"
#Event LocalAddMod "[MQ2] +#*#"
#Event LocalSubMod "[MQ2] -#*#"
#Event ZoneEvent "#*#You have entered#*#"

Sub AdvPathPoll
/if (${PathingFlag}==1 && ${CheckLocTimer}<=0) /call IncrementLoc ${LocArray[${CurrLoc}].Arg[1]} ${LocArray[${CurrLoc}].Arg[2]} ${LocArray[${CurrLoc}].Arg[3]}
/if (${RecordPathFlag}==1) /call RecordPath
/if (${FollowFlag}==1) {
 /call TrackFollow
 /call CheckForTarget
 /if (${CheckLocTimer}<=0 && !${PauseFlag} && ${NumLocs}>${CurrLoc}) /call FollowIncLoc ${LocArray[${CurrLoc}].Arg[1]} ${LocArray[${CurrLoc}].Arg[2]} ${LocArray[${CurrLoc}].Arg[3]}
}
/return

Sub ShapePlot(ObjectNumSides,ObjectSize,CharStartY,CharStartX,CharStartZ)
/declare PointNumber int local
/for PointNumber 1 to ${ObjectNumSides}
 /varset XCoordinate[${PointNumber}] ${Math.Calc[${Math.Calc[${ObjectSize}*${Math.Cos[${Math.Calc[${Math.Calc[360/${ObjectNumSides}]}*${Math.Calc[${PointNumber}-1]}]}]}]}+${CharStartX}]}
 /varset YCoordinate[${PointNumber}] ${Math.Calc[${Math.Calc[${ObjectSize}*${Math.Sin[${Math.Calc[${Math.Calc[360/${ObjectNumSides}]}*${Math.Calc[${PointNumber}-1]}]}]}]}+${CharStartY}]}
 /varset ZCoordinate[${PointNumber}] ${CharStartZ}
/next PointNumber
/for PointNumber 1 to ${ObjectNumSides}
 /varset LocArray[${PointNumber}] ${YCoordinate[${PointNumber}]} ${XCoordinate[${PointNumber}]} ${ZCoordinate[${PointNumber}]}
/next PointNumber
/varset CurrLoc 1
/varset NumLocs ${ObjectNumSides}
/return

Sub TargetMob(TgtName)
/declare a int local
/if (${NearestSpawn[1,pc "${TgtName}"].ID}>0) {
 /if (${FollowFlag}==1 && !${ZoneTimer}) {
 /for a 0 to ${Group}
 /if (${Group.Member[${a}].Name.Equal[${TgtName}]}) /return
 /next a
 }
 /squelch /target clear
 /call Delay 2
 /squelch /target pc "${TgtName}"
 /call Delay 4
 /if (${Target.ID} && ${Target.CleanName.Equal["${TgtName}"]}) {
 /varset ZoneTimer 0
 /varset ZoneFlag 0
 /varset FollowFlag 1
 /varset FollowID ${Target.ID}
 /if (${CurrLoc}==1) /varset CurrFollLoc ${Spawn[${FollowID}].Y} ${Spawn[${FollowID}].X} ${Spawn[${FollowID}].Z}
 /if (!${SilentFlag}) /echo -[ Following -( ${Target.CleanName} )- ]-
 }
} else /if (${NearestSpawn[1,npc "${TgtName}"].ID}>0) {
 /squelch /target clear
 /call Delay 2
 /squelch /target npc "${TgtName}"
 /call Delay 4
 /if (${Target.ID}>0 && ${Target.CleanName.Equal["${TgtName}"]}) {
 /varset ZoneTimer 0
 /varset ZoneFlag 0
 /varset FollowFlag 1
 /varset FollowID ${Target.ID}
 /if (${CurrLoc}==1) /varset CurrFollLoc ${Spawn[${FollowID}].Y} ${Spawn[${FollowID}].X} ${Spawn[${FollowID}].Z}
 /if (!${SilentFlag}) /echo -[ Following -( ${Target.CleanName} )- ]-
 }
} else /if (${ZoneFollow}==1) {
 /if (${CurrLoc}==${NumLocs}) {
 /if (${ZoneTimer}>0) {
 /varset ZoneFlag 1
 /delay 1s
 } else {
 /if (!${ZoneFlag}) {
 /call ClearLocs
 /call TryToZone
 } else {
 /varset ZoneFlag 0
 /varset FollowID 0
 /call StopFunction
 }
 }
 }
}
/return

Sub TrackFollow
/declare TempIndex int local
/if (${FollowFlag}==1) {
 /if (${FollowID}>0) { 
 /if (${NumLocs}==1) {
 /if (${Spawn[${FollowID}].Distance3D}>${FollowDistance}) {
 /if (!${Me.Moving}) {
 /varset LocArray[${NumLocs}] ${CurrFollLoc}
 /varset NumLocs ${Math.Calc[${NumLocs}+1]}
 }
 /varset LocArray[${NumLocs}] ${Spawn[${FollowID}].Y} ${Spawn[${FollowID}].X} ${Spawn[${FollowID}].Z}
 /call StoreHeading ${NumLocs}
 /if (${LocArray[${NumLocs}].NotEqual["0 0 0"]} && ${LocArray[${NumLocs}].NotEqual["NULL NULL NULL"]}) /varset NumLocs ${Math.Calc[${NumLocs}+1]}
 }
 } else {
 /varset TempIndex ${Math.Calc[${NumLocs}-1]}
 /if (${Math.Distance[${LocArray[${TempIndex}].Arg[1]},${LocArray[${TempIndex}].Arg[2]},${LocArray[${TempIndex}].Arg[3]}:${Spawn[${FollowID}].Y},${Spawn[${FollowID}].X},${Spawn[${FollowID}].Z}]}>${If[${Spawn[${FollowID}].Speed}!=0,${Math.Calc[${Math.Calc[${Spawn[${FollowID}].Speed}/${SpeedSense}]}+${DistanceModifier}]},${Math.Calc[${Math.Calc[200/${SpeedSense}]}+${DistanceModifier}]}]}) {
 /varset LocArray[${NumLocs}] ${Spawn[${FollowID}].Y} ${Spawn[${FollowID}].X} ${Spawn[${FollowID}].Z}
 /if ((${LocArray[${NumLocs}].NotEqual["0 0 0"]} && ${LocArray[${NumLocs}].NotEqual["NULL NULL NULL"]})&&(${Math.Distance[${LocArray[${TempIndex}].Arg[1]},${LocArray[${TempIndex}].Arg[2]},${LocArray[${TempIndex}].Arg[3]}:${Spawn[${FollowID}].Y},${Spawn[${FollowID}].X},${Spawn[${FollowID}].Z}]}<500)) /varset NumLocs ${Math.Calc[${NumLocs}+1]}
 }
 }
 }
}
/doevents
/return

Sub StoreHeading(Index)
/varset StoreHeading ${Heading[${LocArray[${Index}].Arg[1]},${LocArray[${Index}].Arg[2]}].DegreesCCW}
/return

Sub TryToZone
/declare InCurrZone string local
/declare TempStoreTgt int local
/varset TempStoreTgt 0
/varset InCurrZone ${Zone.Name}
/call Move 0
/if (${Target.ID}>0) /varset TempStoreTgt ${Target.ID}
/doortarget
/delay 3
/if (${Target.Name.Find["port"]} || ${Target.Name.Find["tele"]} || ${Target.Name.Find["prtl"]}) /goto :Port
/goto :NotPort
:Port
/if (${Target.Distance}<=50) {
 /if (${PBFace.Equal[facef]}) {
 /face fast nolook door
 } else {
 /face fast nolook away door
 }
 :ZoneMoveCloser
 /if (${Target.Distance}>15) /call SimpleKeyPress FORWARD 1 0
 /if (${Target.Distance}<11) /call SimpleKeyPress BACK 1 0
 /if (${Target.Distance}>15) /goto :ZoneMoveCloser
 /if (${Target.Distance}<11) /goto :ZoneMoveCloser
 /keypress USE hold
 /keypress USE
 /delay 1
 /keypress USE hold
 /keypress USE
 /varset ZoneTimer 5s
 :WaitForZone
 /delay 1
 /if (${Zone.Name.NotEqual["${InCurrZone}"]}) /goto :Zoned
 /if (${ZoneTimer}>0) /goto :WaitForZone
 /if (${TempStoreTgt}>0) {
 /if (${Spawn[${FollowID}].ID}>0) /squelch /target id ${TempStoreTgt}
 } else {
 /squelch /target clear
 }
 :Zoned
}
:NotPort
/if (${Zone.Name.Equal["${InCurrZone}"]}) {
 /if (${PBFace.Equal[facef]}) {
 /keypress FORWARD
 /keypress FORWARD hold
 /delay 2s
 /keypress FORWARD
 } else {
 /keypress BACK
 /keypress BACK hold
 /delay 2s
 /keypress BACK
 }
 /if (${TempStoreTgt}>0) {
 /if (${Zone.Name.Equal["${InCurrZone}"]} && ${Spawn[${TempStoreTgt}].ID}>0) /squelch /target id ${TempStoreTgt}
 } else {
 /squelch /target clear
 }
 /varset ZoneTimer 20s
 :WaitForZone2
 /delay 1
 /if (${Zone.Name.NotEqual["${InCurrZone}"]}) /goto :Zoned2
 /if (${ZoneTimer}>0) /goto :WaitForZone2
 /if (!${SilentFlag}) /echo -(( Couldn't zone properly. Stopping. ))- 
 /varset PauseHolder NULL
 /varset PauseFlag 0
 /varset PathingFlag 0
 /varset FollowFlag 0
 /varset RecordPathFlag 0
 /call ClearLocs
 :Zoned2
}
/call Move 0
/return

Sub FollowFunction(FuncText)
/if (!${PauseFlag}) {
 /if (!${PathingFlag}) {
 /if (!${RecordPathFlag}) {
 /if (!${FuncText.Length}) {
 /varset FollowName NULL
 } else {
 /varset FollowName ${FuncText}
 }
 /if (${FollowName.NotEqual["NULL"]}) {
 /call ClearLocs
 /call Move 0
 /call TargetMob "${FollowName}"
 /varset PBFace facef
 } else {
 /if (!${SilentFlag}) /echo -(( No follow target specified. You must provide the name of the npc/player you want to follow. ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot start following while recording a pathfile! ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot start following while playing back a pathfile! ))-
 }
} else {
 /if (!${SilentFlag}) /echo -(( You cannot start following while paused! ))-
}
/return

Sub Event_LocalFollow(EvtText)
/if (${EvtText.Mid[7,6].Equal["follow"]}) {
 /call FollowFunction "${EvtText.Right[-13]}"
}
/return

Sub PauseFunction
/if (!${PauseFlag}) {
 /if (!${PathingFlag}) {
 /if (!${FollowFlag}) {
 /if (!${RecordPathFlag}) {
 /if (!${SilentFlag}) /echo -(( There is nothing to pause or unpause! ))-
 } else {
 /varset PauseFlag 1
 /varset PauseHolder record
 /varset RecordPathFlag 0
 /if (!${SilentFlag}) /echo -[ PAUSED recording of path file -( ${RecordPFName} )- ]-
 /if (!${SilentFlag}) /echo -[ Issue the pause command again to resume recording. ]-
 }
 } else {
 /varset PauseFlag 1
 /varset PauseHolder follow
 /call Move 0
 /if (!${SilentFlag}) /echo -[ PAUSED following of -( ${FollowName} )- ]-
 /if (!${SilentFlag}) /echo -[ Issue the pause command again to resume following. ]-
 }
 } else {
 /varset PauseFlag 1
 /varset PauseHolder playback
 /varset PathingFlag 0
 /call Move 0
 /if (!${SilentFlag}) /echo -[ PAUSED playback of path file -( ${PBPFName} )- ]-
 /if (!${SilentFlag}) /echo -[ Issue the pause command again to resume playback. ]-
 }
} else {
 /if (${PauseHolder.Equal["record"]}) {
 /varset PauseFlag 0
 /varset RecordPathFlag 1
 /if (!${SilentFlag}) /echo -[ RESUMED recording of path file -( ${RecordPFName} )- ]-
 }
 /if (${PauseHolder.Equal["playback"]}) {
 /varset PauseFlag 0
 /varset PathingFlag 1
 /if (!${SilentFlag}) /echo -[ RESUMED playback of path file -( ${PBPFName} )- ]-
 }
 /if (${PauseHolder.Equal["follow"]}) {
 /varset PauseFlag 0
 /if (!${SilentFlag}) /echo -[ RESUMED following of -( ${FollowName} )- ]-
 }
}
/return

Sub Event_LocalPause(EvtText)
/if (${EvtText.Mid[7,5].Equal["pause"]}) {
 /call PauseFunction
}
/return

Sub PlotFunction
/if (${ManualRecord}==1) {
 /if (${RecordPathFlag}==1) {
 /varset LocArray[${NumLocs}] ${Me.Y} ${Me.X} ${Me.Z}
 /if (${SilentFlag}) /echo Location: Y:${LocArray[${NumLocs}].Arg[1]} X:${LocArray[${NumLocs}].Arg[2]} Z:${LocArray[${NumLocs}].Arg[3]} Plotted
 /varset NumLocs ${Math.Calc[${NumLocs}+1]}
 } else {
 /if (!${PauseFlag}) {
 /if (!${SilentFlag}) /echo -[ You aren't recording anything! ]-
 } else {
 /if (!${SilentFlag}) /echo -[ You must unpause your recording in order to plot! ]-
 }
 }
} else {
 /if (!${SilentFlag}) /echo -[ You are not in Manual Record Mode! ]-
}
/return

Sub Event_LocalPlot(EvtText)
/if (${EvtText.Mid[7,4].Equal["plot"]}) {
 /call PlotFunction
}
/return

Sub PlayFunction(FuncText)
/declare PlayNm string local
/declare PlayItr string local
/declare PlayDctn string local
/declare PlayPong string local
/declare PlayZn string local
/declare PlayFace string local
/declare TempCountVar int local
/if (!${PauseFlag}) {
 /if (!${PathingFlag}) {
 /if (!${RecordPathFlag}) {
 /if (!${FollowFlag}) {
 /if (${GetMoreCommands}==1) {
 /varset GetMoreCommands 0
 } else {
 /varset PlayQueueNum 1
 }
 :InitParse
 :ParsePlayCommands
 /if (${FuncText.Find[" "]}>0) /varset PlayNm ${FuncText.Left[${FuncText.Find[" "]}]}
 /if (${FuncText.Find[" "]}>0) /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[" "]}]}]}

 /if (${FuncText.Find[" "]}>0) /varset PlayItr ${FuncText.Left[${FuncText.Find[" "]}]}
 /if (${FuncText.Find[" "]}>0) /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[" "]}]}]}

 /if (${FuncText.Find[" "]}>0) /varset PlayDctn ${FuncText.Left[${FuncText.Find[" "]}]}
 /if (${FuncText.Find[" "]}>0) /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[" "]}]}]}

 /if (${FuncText.Find[" "]}>0) /varset PlayPong ${FuncText.Left[${FuncText.Find[" "]}]}
 /if (${FuncText.Find[" "]}>0) /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[" "]}]}]}

 /if (${FuncText.Arg[2].Find["facef"]}>0 || ${FuncText.Arg[2].Find["faceb"]}>0) {
 /if (${FuncText.Find[";"]}>0) {

 /if (${FuncText.Find[" "]}>0) /varset PlayZn ${FuncText.Left[${Math.Calc[${FuncText.Find[" "]}-1]}]}
 /if (${FuncText.Find[" "]}>0) /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[" "]}]}]}
 /varset PlayFace ${FuncText.Left[${Math.Calc[${FuncText.Find[";"]}-1]}]}
 /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[";"]}]}]}
 /if (${PlayNm.Length}!=0 && ${PlayItr.Length}!=0 && ${PlayDctn.Length}!=0 && ${PlayPong.Length}!=0 && ${PlayZn.Length}!=0 && ${PlayFace.Length}!=0) {
 /varset PlayCommandQueue[${PlayQueueNum}] ${PlayNm} ${PlayItr} ${PlayDctn} ${PlayPong} ${PlayZn} ${PlayFace}
 /varset PlayQueueNum ${Math.Calc[${PlayQueueNum}+1]}
 /goto :InitParse
 } else {
 /if (!${SilentFlag}) /echo -(( Playback syntax error detected. Please try your command again. ))-
 /return
 }
 } else {
 /if (${FuncText.Find["+"]}>0) {

 /if (${FuncText.Find[" "]}>0) /varset PlayZn ${FuncText.Left[${FuncText.Find[" "]}]}
 /if (${FuncText.Find[" "]}>0) /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[" "]}]}]}
 /varset PlayFace ${FuncText.Left[${Math.Calc[${FuncText.Find["+"]}-1]}]}
 /varset GetMoreCommands 1
 } else {
 /if (${FuncText.Find[" "]}>0) /varset PlayZn ${FuncText.Left[${FuncText.Find[" "]}]}
 /if (${FuncText.Find[" "]}>0) /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[" "]}]}]}
 /varset PlayFace ${FuncText}
 }
 /if (${PlayNm.Length}!=0 && ${PlayItr.Length}!=0 && ${PlayDctn.Length}!=0 && ${PlayPong.Length}!=0 && ${PlayZn.Length}!=0 && ${PlayFace.Length}!=0) {
 /varset PlayCommandQueue[${PlayQueueNum}] ${PlayNm} ${PlayItr} ${PlayDctn} ${PlayPong} ${PlayZn} ${PlayFace}
 /if (${GetMoreCommands}==1) /varset PlayQueueNum ${Math.Calc[${PlayQueueNum}+1]}
 /goto :DonePlayParse
 } else {
 /if (!${SilentFlag}) /echo -(( Playback syntax error detected. Please try your command again. ))-
 /return
 }
 }
 } else {
 /if (${FuncText.Find[";"]}>0) {
 /if (${FuncText.Find[";"]}>0) /varset PlayZn ${FuncText.Left[${Math.Calc[${FuncText.Find[";"]}-1]}]}
 /if (${FuncText.Find[";"]}>0) /varset FuncText ${FuncText.Right[${Math.Calc[${FuncText.Length}-${FuncText.Find[";"]}]}]}

 /if (${PlayNm.Length}!=0 && ${PlayItr.Length}!=0 && ${PlayDctn.Length}!=0 && ${PlayPong.Length}!=0 && ${PlayZn.Length}!=0) {
 /varset PlayCommandQueue[${PlayQueueNum}] ${PlayNm} ${PlayItr} ${PlayDctn} ${PlayPong} ${PlayZn} facef
 /varset PlayQueueNum ${Math.Calc[${PlayQueueNum}+1]}
 /goto :InitParse
 } else {
 /if (!${SilentFlag}) /echo -(( Playback syntax error detected. Please try your command again. ))-
 /return
 }
 } else {
 /if (${FuncText.Find["+"]}>0) {
 /varset PlayZn ${FuncText.Left[${Math.Calc[${FuncText.Find["+"]}-1]}]}
 /varset GetMoreCommands 1
 } else {
 /varset PlayZn ${FuncText}
 }
 /if (${PlayNm.Length}!=0 && ${PlayItr.Length}!=0 && ${PlayDctn.Length}!=0 && ${PlayPong.Length}!=0 && ${PlayZn.Length}!=0) {
 /varset PlayCommandQueue[${PlayQueueNum}] ${PlayNm} ${PlayItr} ${PlayDctn} ${PlayPong} ${PlayZn} facef
 /if (${GetMoreCommands}==1) /varset PlayQueueNum ${Math.Calc[${PlayQueueNum}+1]}
 /goto :DonePlayParse
 } else {
 /if (!${SilentFlag}) /echo -(( Playback syntax error detected. Please try your command again. ))-
 /return
 }
 }
 }
 :DonePlayParse
 /if (${GetMoreCommands}==1) {
 /if (!${SilentFlag}) /echo -[ Path Extender Detected. ${Math.Calc[${PlayQueueNum}-1].Int} commands queued. Continue entering lines. ]-
 /return
 }
 /if (!${SilentFlag}) /echo -[------------------------------------------------]-
 /if (!${SilentFlag}) /echo -[ There ${If[${PlayQueueNum}==1,is,are]} ${PlayQueueNum} ${If[${PlayQueueNum}==1,path,paths]} to be run.
 /if (!${SilentFlag}) /echo -[------------------------------------------------]-
 /for TempCountVar 1 to ${PlayQueueNum}
 /if (!${SilentFlag}) /echo -[ Path Command ${TempCountVar}: ${PlayCommandQueue[${TempCountVar}]}
 /next TempCountVar
 /if (!${SilentFlag}) /echo -[------------------------------------------------]-
 /varset PlayQueueCurr 1
 /call GetCommand
 /call LoadPathFile "${PBPFName}"
 /if (!${Macro.Return}) {
 /if (!${SilentFlag}) /echo -[ Started play back of path file -( ${PBPFName} )- ]-
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
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot start playback while in follow mode! ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot start playback while recording! ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You are already playing back path file -( ${PBPFName} )- ))-
 }
} else {
 /if (!${SilentFlag}) /echo -(( You cannot start playback while paused! ))-
}
/return

Sub Event_LocalPlay(EvtText)
/if (${EvtText.Mid[7,5].Equal["play:"]}) {
 /call PlayFunction "${EvtText.Right[-11]}"
}
/return 

Sub PatternFunction(FuncText)
/if (!${PauseFlag}) {
 /if (!${PathingFlag}) {
 /if (!${FollowFlag}) {
 /if (!${RecordPathFlag}) {
 /if (!${FuncText.Arg[3].Length}) {
 /if (${FuncText.Arg[1].Length}!=0 && ${FuncText.Arg[2].Length}!=0) {
 /call ShapePlot ${FuncText.Arg[1]} ${FuncText.Arg[2]} ${Me.Y} ${Me.X} ${Me.Z}
 /varset RecordPFName TempShape
 /varset RecordPathFlag 1
 /call StopFunction
 /varset PlotFlag 1
 /call PlayFunction "${RecordPFName} loop cf nopp noz facef"
 }
 } else {
 /if (${FuncText.Arg[1].Length}!=0 && ${FuncText.Arg[2].Length}!=0 && ${FuncText.Arg[3].Length}!=0) {
 /call ShapePlot ${FuncText.Arg[1]} ${FuncText.Arg[2]} ${Me.Y} ${Me.X} ${Me.Z}
 /varset RecordPFName TempShape
 /varset RecordPathFlag 1
 /call StopFunction
 /varset PlotFlag 1
 /if (${FuncText.Arg[3].Equal[faceb]}) {
 /call PlayFunction "${RecordPFName} loop cf nopp noz faceb"
 } else {
 /call PlayFunction "${RecordPFName} loop cf nopp noz facef"
 }
 }
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You are already recording path file -( ${RecordPFName} )- ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot start a pattern playback while in follow mode! ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot start a pattern playback while playing back a path! ))-
 }
} else {
 /if (!${SilentFlag}) /echo -(( You cannot start a pattern playback while paused! ))-
}
/return

Sub Event_LocalPattern(EvtText)
/if (${EvtText.Mid[7,8].Equal["pattern:"]}) {
 /call PatternFunction "${EvtText.Right[-14]}"
}
/return

Sub RecordFunction(FuncText)
/if (${FuncText.Arg[2].Equal["manual"]}) {
 /varset ManualRecord 1
} else {
 /varset ManualRecord 0
}
/if (!${PauseFlag}) {
 /if (!${PathingFlag}) {
 /if (!${FollowFlag}) {
 /if (!${RecordPathFlag}) {
 /varset RecordPFName ${FuncText.Arg[1]}
 /varset RecordPathFlag 1
 /call ClearLocs
 /if (!${SilentFlag}) /echo -[ Started recording path file -( ${RecordPFName} )- ]-
 } else {
 /if (!${SilentFlag}) /echo -(( You are already recording path file -( ${RecordPFName} )- ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot start recording while in follow mode! ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot start recording while playing back a path! ))-
 }
} else {
 /if (!${SilentFlag}) /echo -(( You cannot start recording while paused! ))-
}
/return

Sub Event_LocalRecord(EvtText)
/if (${EvtText.Mid[7,6].Equal["record"]}) {
 /call RecordFunction "${EvtText.Right[-13]}"
}
/return

Sub GotoFunction(TempYCoord,TempXCoord,TempZCoord)
/if ((${Defined[TempXCoord]}) && (${Defined[TempYCoord]})) /goto :Defined
 /if (!${SilentFlag}) /echo -(( You must at least provide the Y and X coordinates to run to. ))-
 /return
:Defined
/if (${Defined[TempZCoord]}) {
} else {
 /declare TempZCoord float local
 /varset TempZCoord ${Me.Z}
}
/if (!${PauseFlag}) {
 /if (!${PathingFlag}) {
 /if (!${FollowFlag}) {
 /if (!${RecordPathFlag}) {
 /call ClearLocs
 /varset LocArray[1] ${TempYCoord} ${TempXCoord} ${TempZCoord}
 /varset LocArray[2] ${TempYCoord} ${TempXCoord} ${TempZCoord}
 /varset RecordPFName GotoLoc
 /varset RecordPathFlag 1
 /varset CurrLoc 1
 /varset NumLocs 2
 /call StopFunction
 /call PlayFunction "${RecordPFName} 1 f nopp noz"
 /if (!${SilentFlag}) /echo -[ Running to location:${TempYCoord},${TempXCoord},${TempZCoord} ]-
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot use goto while recording. ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot use goto while in follow mode! ))-
 }
 } else {
 /if (!${SilentFlag}) /echo -(( You cannot use goto while playing back a path! ))-
 }
} else {
 /if (!${SilentFlag}) /echo -(( You cannot use goto while paused! ))-
}
/return

Sub Event_LocalGoto(EvtText)
/if (${EvtText.Mid[7,4].Equal["goto"]}) {
 /varset EvtText ${EvtText.Right[-11]}
 /declare TempYCrd string local
 /declare TempXCrd string local
 /declare TempZCrd string local
 /if (${EvtText.Arg[1].Length}!=0 && ${EvtText.Arg[2].Length}!=0) {
 /if (${EvtText.Find[" "]}>0) {
 /varset TempYCrd ${EvtText.Mid[1,${EvtText.Find[" "]}]}
 /varset EvtText ${EvtText.Mid[${Math.Calc[${EvtText.Find[" "]}+1]},${Math.Calc[${EvtText.Length}-${EvtText.Find[" "]}]}]}
 }
 /if (${EvtText.Find[" "]}>0) {
 /varset TempXCrd ${EvtText.Mid[1,${EvtText.Find[" "]}]}
 /varset EvtText ${EvtText.Mid[${Math.Calc[${EvtText.Find[" "]}+1]},${Math.Calc[${EvtText.Length}-${EvtText.Find[" "]}]}]}
 /if (${EvtText.Length}>0) {
 /varset TempZCrd ${EvtText.Arg[1]}
 } else {
 /varset TempZCrd ${Me.Z}
 }
 } else {
 /varset TempXCrd ${EvtText.Arg[1]}
 /varset TempZCrd ${Me.Z}
 }
 /call GotoFunction "${TempYCrd}" "${TempXCrd}" "${TempZCrd}"
 } else {
 /if (!${SilentFlag}) /echo -(( You must at least provide the Y and X coordinates to run to. ))-
 }

}
/return

Sub DistanceFunction(FuncText)
 /varset FollowDistance ${FuncText}
 /if (!${SilentFlag}) /echo -[ Follow Distance :[${Int[${FollowDistance}]}] ]-
/return


Sub Event_LocalDistance(EvtText)
/if (${EvtText.Mid[7,8].Equal["distance"]}) {
 /call DistanceFunction "${EvtText.Right[-15]}"
}
/return

Sub SilentFunction
/if (!${SilentFlag}) {
 /varset SilentFlag 1
 /echo -[ Silent Mode ON ]-
} else {
 /varset SilentFlag 0
 /echo -[ Silent Mode OFF ]-
}
/return

Sub Event_LocalSilent(EvtText)
/if (${EvtText.Right[-6].Equal["silent"]}) {
 /call SilentFunction
}
/return

Sub StopFunction
/declare AdvPathIniFile string local
/declare NumOfRecs int local
/declare ClearVar string local
/declare RecCounter int local
/declare RecClrCounter int local
/if (!${PathingFlag}) {
 /if (!${RecordPathFlag}) {
 /if (!${FollowFlag}) {
 /if (!${PauseFlag}) {
 /if (!${SilentFlag}) /echo -(( There is nothing to be stopped! ))-
 } else {
 /if (!${SilentFlag}) /echo -[ Pause stopped and cleared! ]-
 }
 } else {
 /varset FollowFlag 0
 /call ClearLocs
 /call Move 0
 /if (!${SilentFlag}) /echo -[ Stopped following -( ${FollowName} )- ]-
 }
 } else {
 /if (${NumLocs}>1) {
 /varset RecordPathFlag 0
 /if (!${SilentFlag}) /echo -[ Stopped recording path file -( ${RecordPFName} )- ]-
 /if (!${SilentFlag}) /echo -[ Saving path file -( ${RecordPFName} )- Stand By... ]-
 /varset AdvPathIniFile PathFile_${RecordPFName}.ini
 /varset RecClrCounter 0
 :ClearRF
 /varset ClearVar ${Ini["${AdvPathIniFile}",PFLocs,${Int[${RecClrCounter}]},NOTFOUND]}
 /if (${ClearVar.NotEqual["NOTFOUND"]}) {
 /ini "${AdvPathIniFile}" "PFLocs" "${Int[${RecClrCounter}]}" "Empty"
 /varset RecClrCounter ${Math.Calc[${RecClrCounter}+1]}
 /goto :ClearRF
 }
 /goto :DoneClearRF
 :DoneClearRF
 /varset RecCounter 0
 :CheckClearRF
 /if (${RecCounter}==${RecClrCounter}) /goto :DoneCheckClearRF
 /varset ClearVar ${Ini["${AdvPathIniFile}",PFLocs,${Int[${RecCounter}]},NOTFOUND]}
 /if (${ClearVar.Equal["Empty"]}) {
 /varset RecCounter ${Math.Calc[${RecCounter}+1]}
 /goto :CheckClearRF
 }
 /goto :CheckClearRF
 :DoneCheckClearRF
 /for NumOfRecs 0 to ${Math.Calc[${NumLocs}-2].Int}
 /ini "${AdvPathIniFile}" "PFLocs" "${Int[${NumOfRecs}]}" "${LocArray[${Math.Calc[${NumOfRecs}+1]}]}"
 /next NumOfRecs
 /varset RecCounter 0
 :CheckRF
 /if (${RecCounter}==${Math.Calc[${NumLocs}-1]}) /goto :DoneCheckRF
 /varset ClearVar ${Ini["${AdvPathIniFile}",PFLocs,${Int[${RecCounter}]},NOTFOUND]}
 /if (${ClearVar.Equal["NOTFOUND"]}) /goto :CheckRF
 /if (${ClearVar.Equal["Empty"]}) /goto :CheckRF
 /varset RecCounter ${Math.Calc[${RecCounter}+1]}
 /goto :CheckRF
 :DoneCheckRF
 /if (!${SilentFlag}) /echo -[ Save Completed. There were ${Math.Calc[${NumLocs}-1].Int} records total. ]-
 } else {
 /varset RecordPathFlag 0
 /if (!${SilentFlag}) /echo -(( There were no locations to record! ))-
 }
 }
} else {
 /if (!${SilentFlag}) /echo -[ Playback stopped! ]-
 /call Move 0
}
/varset PauseHolder NULL
/varset PauseFlag 0
/varset PathingFlag 0
/varset RecordPathFlag 0
/varset PlotFlag 1
/call ClearLocs
/return

Sub Event_LocalStop(EvtText)
/if (${EvtText.Right[-6].Equal["stop"]}) {
 /call StopFunction
}
/return

Sub Event_ZoneEvent
/varset FollowID 0
/if (${FollowFlag}==1) {
 /varset ZoneTimer 30s
 /call ClearLocs
 /call Move 0
 /delay 3s
 /call ClearLocs
 /call TargetMob "${FollowName}"
}
/if (${PathingFlag}==1) {
 /call ClearLocs
 /call Move 0
 /delay 3s
 /call ClearLocs
}
/call Move 0
/return

Sub SubModFunction
/if (${DistanceModifier}>0) /varset DistanceModifier ${Math.Calc[${DistanceModifier}-1]}
/if (!${SilentFlag}) /echo -[ Distance Modifier:[${Int[${DistanceModifier}]}] ]-
/return

Sub Event_LocalSubMod(EvtText)
/if (${EvtText.Right[-6].Equal["-"]}) {
 /call SubModFunction
}
/return

Sub AddModFunction
/varset DistanceModifier ${Math.Calc[${DistanceModifier}+1]}
/if (!${SilentFlag}) /echo -[ Distance Modifier:[${Int[${DistanceModifier}]}] ]-
/return

Sub Event_LocalAddMod(EvtText)
/if (${EvtText.Right[-6].Equal["+"]}) {
 /call AddModFunction
}
/return

Sub InitAPFVars(FaceF,SpeedS,FollowDst)
/declare CurrLoc int outer
/declare NumLocs int outer
/declare LocArray[5000] string outer
/declare PathingFlag int outer
/declare FollowID int outer
/declare SpeedSense int outer
/declare UseFaceFast int outer
/declare MyLastXLoc float outer
/declare MyLastYLoc float outer
/declare MyLastZLoc float outer
/declare ObstacleCount int outer
/declare CurrZone string outer
/declare LastLoc string outer
/declare TotalObstCount int outer
/declare FollowName string outer
/declare RecordPathFlag int outer
/declare PauseFlag int outer
/declare PauseHolder string outer
/declare PlayCommandQueue[100] string outer
/declare PlayQueueNum int outer
/declare PlayQueueCurr int outer
/declare RecordPFName string outer
/declare PBPFName string outer
/declare PBIterations string outer
/declare PBFace string outer
/declare Iterations int outer
/declare PBDirection string outer
/declare Direction string outer
/declare GetMoreCommands int outer
/declare PBPingPong string outer
/declare PingPongCount int outer
/declare PBZone string outer
/declare FollowFlag int outer
/declare ZoneFollow int outer
/declare SilentFlag int outer
/declare DelayTimer timer outer
/declare ZoneTimer timer outer
/declare ZoneFlag int outer
/declare XCoordinate[100] float outer
/declare YCoordinate[100] float outer
/declare ZCoordinate[100] float outer
/declare DistanceModifier int outer
/declare FollowDistance int outer
/declare CheckLocTimer timer outer
/declare PositionTimer timer outer
/declare StoreHeading int outer
/declare CurrFollLoc string outer
/declare DoorMoveTimer timer outer
/declare ManualRecord int outer
/declare PlotFlag int outer
/varset PlotFlag 0
/varset DoorMoveTimer 0
/varset PositionTimer 0
/varset CheckLocTimer 0
/varset DistanceModifier 0
/varset ZoneFlag 0
/if (${Defined[FaceF]}) {
 /varset UseFaceFast ${FaceF}
} else {
 /varset UseFaceFast 1
}
/if (${Defined[SpeedS]}) {
 /varset SpeedSense ${SpeedS}
} else {
 /varset SpeedSense 15
}
/if (${Defined[FollowDst]}) {
 /varset FollowDistance ${FollowDst}
} else {
 /varset FollowDistance 20
}
/varset SilentFlag 0
/varset ZoneFollow 1
/varset FollowFlag 0
/varset PauseHolder NULL
/varset PauseFlag 0
/varset GetMoreCommands 0
/varset PlayQueueCurr 1
/varset PlayQueueNum 1
/varset PBPFName NULL
/varset RecordPFName NULL
/varset RecordPathFlag 0
/varset CurrLoc 1
/varset NumLocs 1
/varset PathingFlag 0
/varset MyLastYLoc ${Me.Y}
/varset MyLastXLoc ${Me.X}
/varset MyLastZLoc ${Me.Z}
/varset ObstacleCount 0
/varset TotalObstCount 0
/echo Advpath.inc by ml2517 -- Initialized.
/return 