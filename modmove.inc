#Event GuildPortal "#*#The Guildhall Portal has been aligned#*#"

Sub Event_GuildPortal
   /echo GuildPortal Event
/return

Sub LoadAMINIVals
|  /if (!${Defined[MBWayPnt]}) /declare MBWayPnt string outer MBWayPnt.INI
/call LoadINIVar Settings MBWayPntLocation "MBWayPnt.ini" MBWayPnt "${MBCommon}" string
  /if (!${Defined[ZoneName]}) /declare ZoneName string outer ${Zone.Name}
  /if (!${Defined[PathLoc]}) /declare PathLoc int outer
/return

Sub MBZone(tZoneName)
 /declare tZoneTimer timer local
 /declare tZoneTurn int local
 /declare tZoneTF bool local FALSE
 /declare tZoneID int local ${Zone.ID}
 /squelch /doortarget
 /if (${NearestSpawn[Priest_Of_Discord npc los radius 40].ID} || (${Zone.ID}==345 && ${DoorTarget.Distance3D}>90) || (${tZoneName.Length} && (${Zone.ID}==344 && ${DoorTarget.Distance3D}>40 && ${NearestSpawn[Magus npc radius 100].ID}) || (${Zone.ID}!=344 && ${NearestSpawn[Magus npc radius 100].ID}))) {
 | lobby = 344, hall 345
    /if (${Zone.ID}==345) {
     /moveto loc -23 -134
     /delay 1s
    :waitforwindow 
     /doevents Event_GuildPortal
     /if (!${Window[LargeDialogWindow].Open}) { 
       /delay 5s ${Window[LargeDialogWindow].Open} 
       /goto :waitforwindow 
     } 
     /if (${Window[LargeDialogWindow].Open}==0) /echo Specified window not detected open 
     /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup 
|     /goto :tZoned
   } else /if (${NearestSpawn[Priest_Of_Discord npc radius 100].ID}) {
      /squelch /tar Priest_Of_Discord npc 
      /delay 5
      /face 
      /delay ${Math.Calc[${Math.Rand[8]}+1]}s
      /docommand ${If[${Zone.ID}==302,/say go home,/say wish to go]}      
      /delay 15s
   } else /if (${NearestSpawn[Magus npc radius 100].ID}) {
      /squelch /tar magus npc
      /delay 5
      /face 
      /delay ${Math.Calc[${Math.Rand[8]}+1]}s
      /if (${Select[${tZoneName},butcher,butcherblock]}) /say butcherblock
      /if (${Select[${tZoneName},nedaria]}) /say nedaria
      /if (${Select[${tZoneName},southro,south ro]}) /say south ro
      /if (${Select[${tZoneName},northro,north ro]}) /say north ro
      /if (${Select[${tZoneName},everfrost]}) /say everfrost
      /if (${Select[${tZoneName},ecommonlands,east commonlands]}) /say commonlands
      /if (${Select[${tZoneName},natambi]}) /say natambi
      /if (${Select[${tZoneName},abysmal]}) /say abysmal sea
   }
 } else /if (${Zone.ID}==219 || ${Zone.ID}==202 && ${NearestSpawn[Herald_of_Druzzil npc los radius 40].ID} && ${Spawn[Herald_of_Druzzil npc].Distance}<40) {
:TargCheck
  /if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]}) /return
  /if (${Spawn[Herald_of_Druzzil npc].Distance}>39) /goto :NoMagusOrHoD
 	/if (${Zone.ID}==219 && ${Spawn[Herald_of_Druzzil npc].Distance}<40) {
 		/squelch /target id ${Spawn[Herald_of_Druzzil npc los radius 40].ID}
 		/delay 1s ${Target.ID}==${Spawn[Herald_of_Druzzil npc los radius 40].ID}
 		/delay ${Math.Calc[${Math.Rand[15]}+10]}
 		/if (${Target.ID}!=${Spawn[Herald_of_Druzzil npc los radius 40].ID}) /goto :TargCheck
 		
 		/say send
 		/delay 2s !${Me.ID}
 		/return
 	}
 	/if (${Zone.ID}==202 && ${Spawn[Herald_of_Druzzil npc].Distance}<40) {
 		/squelch /target id ${Spawn[Herald_of_Druzzil npc los radius 40].ID}
 		/delay 1s ${Target.ID}==${Spawn[Herald_of_Druzzil npc los radius 40].ID}
 		/delay ${Math.Calc[${Math.Rand[15]}+10]}
 		/if (${Target.ID}!=${Spawn[Herald_of_Druzzil npc los radius 40].ID}) /goto :TargCheck
 		
 		/say time
 		/delay 2s !${Me.ID}
 		/return
 	}
  	
 } else {
:NoMagusOrHoD
/if (!${Me.ID} || !${ZoneName.Equal[${Zone.Name}]}) /return
   /delay 3
   /varset tZoneTimer 10s
   /if (${DoorTarget.Distance}<=70 && (${DoorTarget.Name.Find[cave]} || ${DoorTarget.Name.Find[_crystal]} || ${DoorTarget.Name.Find[trans_]} || ${DoorTarget.Name.Find[_book_]} || ${DoorTarget.Name.Find[crate]} || ${DoorTarget.Name.Find[pontree]} || ${DoorTarget.Name.Find[zonebox]} || ${DoorTarget.Name.Find[potran]} || ${DoorTarget.Name.Find[port]} || ${DoorTarget.Name.Find[rock]} || ${DoorTarget.Name.Find[tele]} || ${DoorTarget.Name.Find[prtl]} || ${DoorTarget.Name.Find[potrhut]} || ${DoorTarget.Name.Find[door]} || ${DoorTarget.Name.Find[grave]} || ${DoorTarget.Name.Find[switch]})) {
     :tMoveToDoor
     /doevents DelayOnZone
     /if (${tZoneID}==${Zone.ID}) {
        /call MBDoor
        /delay 3
        /if (${Window[LargeDialogWindow].Open}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup
        /delay 4s !${Window[LargeDialogWindow].Open}
        /doevents DelayOnZone
        /if (${tZoneID}!=${Zone.ID}) /return TRUE
        /if (${DoorTarget.Distance}>15) {
          /keypress forward hold
          /delay 3
          /keypress forward
        } else /if (${DoorTarget.Distance}<10) {
          /keypress back hold   
          /delay 1
          /keypress back
        } else /if (${tZoneTimer}<50) {
          /if (${tZoneTurn}) {
             /keypress strafe_left hold
             /delay 5
             /keypress strafe_left
             /varset tZoneTurn 0
            } else {
             /keypress strafe_right hold
             /delay 5
             /keypress strafe_right
             /varset tZoneTurn 1
          }
        }
        /if (${Target.ID}) /squelch /tar clear
        /squelch /doortarget
        /squelch /face door nolook
        /keypress home
        /delay 1
        /keypress u hold
        /delay 2
        /keypress u
        /delay 2s
        /if (${tZoneTimer}) /goto :tMoveToDoor
     }
   } else {
    :tNotaDoor
     /if (${Select[${tZoneTurn},0,2,4]}) {
       /keypress forward hold
       /delay 3s
       /keypress forward
     }
     /if (${Select[${tZoneTurn},1,3]}) {
       /keypress back hold
       /delay 8
       /keypress back
       /if (${tZoneTurn}==1) {
         /keypress left hold
         /delay 5
         /keypress left
         } else /if (${tZoneTurn}==3) {
         /keypress right hold
         /delay 9
         /keypress right
       }
     }
    /varcalc tZoneTurn ${tZoneTurn}+1
    /if (${tZoneTurn}>=5) /varset tZoneTimer 0
    /if (${tZoneTimer}) /goto :tNotaDoor
   }
 }
 /delay 5s
 /doevents DelayOnZone
 /docommand ${If[${tZoneID}==${Zone.ID},/varset tZoneTF FALSE,/varset tZoneTF TRUE]}
/return ${tZoneTF}

Sub MBMoveTo(MBy,MBx,MBp)
  /if (!${Defined[MBp]}) /declare MBp string local FALSE
  /declare tDistThen float local
  /declare tDistNow float local
  /declare tMoveCheck timer local 22
  /declare tMoveTimer timer local 2m
  /declare tMB bool local FALSE
  /declare tZoneID int local ${Zone.ID}
  /declare tDoor int local
  /squelch /stick off
  /squelch /moveto unpause
  /squelch /moveto loc ${MBy} ${MBx}
:tMoveTop
  /doevents ImHit
  /if (!${tMoveCheck}) {
    /if (${APAggro} && !${MBp}) /multiline ; /varset tMB TRUE;/goto :tEnd
    /varset tDistNow ${Math.Distance[${Me.Y},${Me.X}:${MBy},${MBx}]}
    /doevents
    /if (${tZoneID}!=${Zone.ID}) /multiline ; /varset tMB ZONED;/goto :tEnd
    /if (${tDistThen}-${tDistNow}<2 && ${Me.Moving} && !${Me.Stunned}) {
      /keypress forward
      /keypress back hold
      /delay 5
      /keypress back
      /if (${DoorTarget.Distance}<15 && !${tDoor}) {
        /call MBDoor
        /if (!${Me.ID} || ${ZoneName.NotEqual[${Zone.Name}]}) /goto :tEnd
        /if (!${Macro.Return}) {
          /squelch /tar clear
          /keypress forward hold
          /delay 2
          /keypress forward
          /call MBDoor
        }
        /varset tDoor 1
       } else {
        /if (${Math.Rand[99]}>50) {
          /keypress strafe_right hold
          } else {
          /keypress strafe_left hold
        }
        /delay 1s
        /keypress strafe_right
        /keypress strafe_left
      }
      /delay 1s !${Me.Moving}
    }
    /if (!${Me.Moving}) {
      /squelch /moveto loc ${MBy} ${MBx}
      /keypress jump
    }
    /varset tMoveCheck 15
  }
  /if (${tMoveCheck}>=14) /multiline ; /varset tDistThen ${Math.Distance[${Me.Y},${Me.X}:${MBy},${MBx}]};/varset tDoor 0
  /if (${Select[${MoveTo},off]}) /goto :tEnd
  /if (${tMoveTimer}) /goto :tMoveTop
:tEnd
  /if (${Math.Distance[${Me.Y},${Me.X}:${MBy},${MBx}]}<10) /varset tMB TRUE
/return ${tMB}

Sub MBWayRecord(Section)
/echo Entering MBWayRecord
  /if (!${Defined[Section]}) /multiline ; /echo Syntax Error - mbwayplay <path name>;/return
  /declare WPCount int local 0
/echo Entering MBWayRecord - Must endmacro or sit to eject from loop
 :tMBRTop
  /if (${Me.State.Equal[duck]}) {
    /varcalc WPCount ${WPCount}+1
    /ini "${MBWayPnt}" "${Section}" "WPCount" "${WPCount}"
    /ini "${MBWayPnt}" "${Section}" "WP${WPCount}" "${Me.Y} ${Me.X}"
    /echo WayPoint[${WPCount}] ${Me.Y} ${Me.X}
    /keypress duck
  }
  /if (${Me.State.Equal[sit]}) /return
  /goto :tMBRTop
/echo Exiting MBWAyRecord
/return

Sub MBDoor
 /declare tDoorTimer timer local 2s
 /declare tMB bool local FALSE
 :tTop
 /if (!${Me.ID} || ${ZoneName.NotEqual[${Zone.Name}]}) /return TRUE
  /if (${Target.ID}) /squelch /tar clear
  /squelch /doortarget
  /squelch /face door
  /if (!${Switch.Open}) {
     /keypress u hold
     /delay 2
     /keypress u
  }
  /delay 2
  /if (${Window[LargeDialogWindow].Open}) {
  	/nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup
  	/multiline ; /varset tMB TRUE;/delay 5;/goto :tEnd
  }
  /if (${Target.ID}) {
  	/delay 7
  	/keypress forward hold
    /delay 2
    /keypress forward
  }
  /if (!${Zone.Name.Equal[${ZoneName}]} || !${Me.ID}) /multiline ; /varset tMB TRUE;/delay 5;/goto :tEnd
  /delay 2
  /if (${Switch.Open}) /multiline ; /varset tMB TRUE;/delay 5;/goto :tEnd
  /if (${tDoorTimer}) /goto :tTop
  :tEnd
/return ${tMB}

Sub MBWayPlay(Section,DirectionUD,ZoneTF,DataA,DataB)
| Syntax MBWayPlay([name of INI section],[direction <u>p,<d>own,<b>egining,<e>nd],[try to zone <t>rue,<f>alse],[Options,<c>hain,<pull>,<sell>,<buy>],[Options,<[merchant name]>,<[pull script]>)
/echo Entering MBWayPlay
  /if (${Select[${MakeCamp},on]}) /squelch /makecamp off
  /if (!${Defined[Section]} || !${Defined[DirectionUD]}) /multiline ; /echo Syntax Error - mbwayplay <path name> <(up down begin end)u,d,b,e> <(zone)t,f> <(chain paths)c>;/return
  /if (!${Defined[ZoneTF]}) /declare ZoneTF string local f
  /if (!${Defined[DataA]}) /declare DataA string local
  /if (!${Defined[DataB]}) /declare DataB string local
  /declare a int local
  /declare b int local
  /declare x int local
  /declare y int local
  /declare Sx int local
  /declare Sy int local
  /declare WPStart int local 1
  /declare tMB bool local FALSE
  /declare tZoneName string local
  /declare tZoneID int local ${Zone.ID}
  /declare tZoneTF string local
  /declare tPull bool local FALSE
  /declare tMBWayINI string local ${MBWayPnt}
 :tTop  
  /if (${Select[${Section.Arg[1,-]},net,network]}) { 
        /varset tMBWayINI ${NetworkINI}
        /varset Section ${Section.Right[${Math.Calc[${Section.Length}-${Section.Find[-]}]}]}
  } 
  /docommand ${If[!${Defined[WPLoop]},/declare WPLoop bool local ${Ini["${tMBWayINI}",${Section},WPLoop]},/varset WPLoop ${Ini["${tMBWayINI}",${Section},WPLoop]}]}
|  /echo Looped Return == ${WPLoop}   (WPLoop)
  /declare WPCount int local ${Ini["${tMBWayINI}",${Section},WPCount]}
  /declare WP[${WPCount}] string Local
  /varset b 0 
  /for a ${If[${Select[${DirectionUD},u,e]},${WPCount},${If[${DirectionUD.Find[AP]},${DirectionUD.Right[-2]},1]}]} ${If[${Select[${DirectionUD},u,e]},downto,to]} ${If[${Select[${DirectionUD},u,e]},1,${WPCount}]} 
    /varcalc b ${b}+1
    /varset WP[${b}] ${Ini["${tMBWayINI}",${Section},WP${a}]}
    /multiline ; /varset y ${WP[${b}].Arg[1]};/varset x ${WP[${b}].Arg[2]}
    /if (${b}==1) /multiline ; /varset Sy ${y};/varset Sx ${x}
    /if (${Select[${DirectionUD},u,d]} && ${Math.Distance[${Me.Y},${Me.X}:${y},${x}]}<${Math.Distance[${Me.Y},${Me.X}:${Sy},${Sx}]}) /multiline ; /varset Sy ${y};/varset Sx ${x};/varset WPStart ${b}
    /if (!${tPull} && ${WP[${b}].Find[ pull]}) /varset tPull TRUE
  /next a
  /for a ${If[${WPStart}>1,${WPStart},1]} to ${WPCount}
    /varset PathLoc ${a}
|/echo   for a ${If[${WPStart}>1,${WPStart},1]} to ${WPCount}
/if (${Me.Casting.ID} && ${Me.Class.Name.NotEqual[Bard]}) /delay 3s !${Me.Casting.ID}


/if (${DoPull} && ${tPull} && !${Section.Find[${APRetPath}]} && ${WP[${a}].Arg[3].Equal[pull]} && ${WP[${a}].Arg[4].Equal[check]} && !${APAggro}) {

|/echo MBWayPlay - ${a} - ${WP[${a}]} - ${SpawnCount[npc loc ${WP[${a}].Arg[2]} ${WP[${a}].Arg[1]} radius ${APRadius} zradius 100]}
        /docommand ${If[${SpawnCount[npc loc ${WP[${a}].Arg[2]} ${WP[${a}].Arg[1]} radius ${APRadius} zradius 100]},/varset WP[${a}] ${WP[${a}].Arg[1]} ${WP[${a}].Arg[2]} pull mob,/varset WP[${a}] ${WP[${a}].Arg[1]} ${WP[${a}].Arg[2]} pull fail]}
|/echo MBWayPlay - ${a} - ${WP[${a}]}

|/echo !${Select[${WP[${a}].Arg[4]},fail]} && ${Bool[${If[${DoPull} && !${Section.Find[${APRetPath}]},true,false]}]} && ${Bool[${If[${DoPull} && ${Section.Find[${APRetPath}]} && !${APAggro},true,false]}]} &&

}
|/echo ${Bool[${If[${DoPull} && ${Section.Find[${APRetPath}]} && ${APAggro},true,false]}]}

|/echo MBWayPlay - ${a} - ${WP[${a}]}


/if (!${Select[${WP[${a}].Arg[4]},fail]} && !${Bool[${If[${DoPull} && ${Section.Find[${APRetPath}]} && ${APAggro},true,false]}]} && !${Bool[${If[!${DoPull} && ${tPull},true,false]}]} && (!${DoPull} || ${a}!=1 || (${a}==1 && ${Math.Distance[${Me.Y},${Me.X}:${WP[${a}].Arg[1]},${WP[${a}].Arg[2]}]}>30 && ${SpawnCount[npc loc ${WP[${a}].Arg[1]} ${WP[${a}].Arg[2]} radius ${Radius}]}) || ${FailCheck})) /call MBMoveTo ${WP[${a}].Arg[1]} ${WP[${a}].Arg[2]}



|/if (!${Section.Find[${APRetPath}]} && (!${DoPull} || ${a}!=1 || (${a}==1 && ${Math.Distance[${Me.Y},${Me.X}:${WP[${a}].Arg[1]},${WP[${a}].Arg[2]}]}>30 && ${SpawnCount[npc loc ${WP[${a}].Arg[1]} ${WP[${a}].Arg[2]} radius ${Radius}]}) || ${FailCheck})) /call MBMoveTo ${WP[${a}].Arg[1]} ${WP[${a}].Arg[2]}

  /if (${DoPull} && (${tZoneID}!=${Zone.ID} || ${Select[${Macro.Return},zoned]})) /goto :tMWAbort
     /if (${WP[${a}].Arg[3].Equal[pull]} || ${tPull} && ${APAggro} || ${a}==${WPCount} || ${a}==1 && ${CurrentMobCount} && !${APTargetID}) {


|/echo        /if (${WP[${a}].Arg[3].Equal[pull]} || ${tPull} && ${APAggro} || ${a}==1 && ${CurrentMobCount} && !${APTargetID}) /call AdvPull ${WP[${a}].Arg[4]}
        /if (${WP[${a}].Arg[3].Equal[pull]} || ${tPull} && ${APAggro} || ${a}==1 && ${CurrentMobCount} && !${APTargetID}) /call AdvPull ${WP[${a}].Arg[4]}



      /if (${DoPull} && !${Macro.Return} && !${APAggro} && ${a}!=${WPCount} || ${DoPull} && ${Macro.Return.Equal[NULL]} && !${APAggro} && ${a}!=${WPCount} || ${FailCheck}) /next a
        /if (${DoPull}) {
          /if (${Macro.Return} || ${a}==${WPCount} ||  ${Me.CombatState.Equal[combat]} && (${APTargetID} || ${APAggro})) {
           /if (${WPLoop} || (${APRetPath.Length} && ${Section.Find[${APRetPath}]})) {
              /if (${a}>=1) {
                 /for b ${a} downto 1
                    /call MBMoveTo ${WP[${b}].Arg[1]} ${WP[${b}].Arg[2]} ${If[(${APRetPath.Length} && ${Section.Find[${APRetPath}]}),TRUE,${tPull}]}
                    /if (${DoPull} && ${tZoneID}!=${Zone.ID}) /goto :tMWAbort
                    /varset PathLoc ${b}
                 /next b
              }
           } else /if (!${APRetPath.Length} && !${WPLoop}) {
              /call MBMoveTo ${WP[1].Arg[1]} ${WP[1].Arg[2]} ${APAggro}
              /if (${DoPull} && ${tZoneID}!=${Zone.ID}) /goto :tMWAbort
              /varset PathLoc 1
           }
           /if (${APTargetID}) {
              /multiline ; /face id ${APTargetID};/varset tMB TRUE
             } else {
              /multiline ; /if (${MakeCampD}) /face Heading ${MakeCampD};/varset tMB FALSE
              /if (${WP[${WPCount}].Arg[4].Length}) {
/echo Different Pull path detected
                 /varset a ${WPCount}
                 /varset WP[${a}] empty empty ${WP[${a}].Arg[4]} b
                 /goto :tChainPull

              }
           }
/echo Was pull good - ${tMB}
           /goto :tMWAbort
        }
     }
    } 
     /if (${WP[${a}].Arg[3].Equal[door]}) /call MBDoor
     /if (${WP[${a}].Arg[3].Equal[sell]}) {
        /call SellCheck ${WP[${a}].Arg[4]}
        /if (!${Macro.Return}) /goto :tMWAbort
     }
  /next a
  /if (${Math.Distance[${Me.Y},${Me.X}:${WP[${b}].Arg[1]},${WP[${b}].Arg[2]}]}<15) /varset tMB TRUE  
  /if (${ZoneTF.Equal[t]}) {
/echo Trying to Zone ${ZoneTF}
    /varset a 0
    /if (${DataA.Length}>2) /varset a 1
    /if (${DataB.Length}) /multiline ; /varset DataA ${DataA} ${DataB};/varset a 2
    /if (${a}) {
      /call MBZone ${DataA.Arg[${a}]}
     } else {
      /call MBZone
    }
    /if (!${Macro.Return}) /goto :tMWAbort
    /delay 600s ${Me.ID}
  }
  /if (${DataA.Arg[1].Equal[c]}) {
/echo Chaining Waypoints
    /varset a 0
    /if ((${DirectionUD.Find[u]} || ${DirectionUD.Find[e]}) && ${WP[1].Arg[3].Length}) /varset a 1
    /if ((${DirectionUD.Find[d]} || ${DirectionUD.Find[b]}) && ${WP[${WPCount}].Arg[3].Length}) /varset a ${WPCount}
    /if (${a}>=1) {
:tChainPull
/echo Chain Found
      /varset Section ${WP[${a}].Arg[3]}
      /varset DirectionUD ${WP[${a}].Arg[4]}
      /multiline ; /varset ZoneTF ;/varset DataA ;/varset DataB
      /if (${WP[${a}].Arg[5].Length}) /varset ZoneTF ${WP[${a}].Arg[5]}
      /if (${WP[${a}].Arg[6].Length}) /varset DataA ${WP[${a}].Arg[6]}
      /if (${WP[${a}].Arg[7].Length}) /varset DataB ${WP[${a}].Arg[7]}
      /if (!${Section.Length} || !${DirectionUD.Length}) /goto :tMWAbort
      /deletevar WP
      /deletevar WPCount
      /delay 3s
      /goto :tTop
    } else {
        /echo Chain Not Found
    }
  /varset tMB TRUE
 :tMWAbort
  }
/echo Exiting MBWayPlay ${tMB}
/keypress back
/if (${tPull}) {
  /if (${APLoopCntr}>3 && ${ACAssistPct}!=100) {
    /multiline ; /varset APLoopCntr 0;/varset APCheckTimer 6m;/echo Looped through a few times, No mobs, waiting 6 mins, time now ${Time}
    } else {
    /varcalc APLoopCntr ${APLoopCntr}+1
  }
}
/return ${tMB}

Sub SitCheck
/if (${FollowFlag} && ${Spawn[${FollowID}].Distance}>=15 && !${AdvPath.Idle} || ${Me.Casting.ID} || ${Me.Moving} || (${Melee.Combat}&&!${Me.CombatState.Equal[Active]}) || ${TankName.Equal[${Me.Name}]} && ${ADMobCount}) /multiline ; /varset SitCheckTimer 3s;/return
/declare MobHeading int local
/declare MeHeading int local
/declare MiniIterate int local
/declare AggroFlag int local
/declare AggroID int local
/if (${Me.PctMana}>97 || ${Select[${Me.Class.ShortName},WAR,ROG,MNK]} && ${Me.PctEndurance}>97 || ${Me.CombatState.Equal[Resting]}) /goto :BreakOutCheckAggro
/if (${AdvPath.Following} && ${Spawn[${FollowID}].Distance}>=9 && ${Target.ID}==${FollowID} && !${Me.State.Equal[sit]}) {
  /face id ${Target.ID}
  /keypress up hold
  /delay 3s ${Spawn[${FollowID}].Distance}<8
  /keypress up
}
/multiline ; /varset MiniIterate 1;/varset AggroFlag 0;/varset AggroID 0
:CheckNext
/varset AggroID ${NearestSpawn[${MiniIterate},npc radius ${SitAggroRadiusCheck} zradius 100].ID}
/if (${AggroID}) {
  /varset MobHeading ${Spawn[${AggroID}].Heading.Degrees}
  /docommand ${If[${Math.Abs[${Math.Calc[${Spawn[${AggroID}].HeadingTo.Degrees}+180]}]}>=359.99,/varcalc MeHeading ${Spawn[${AggroID}].HeadingTo.Degrees}-180+360,/varcalc MeHeading ${Spawn[${AggroID}].HeadingTo.Degrees}+180+360]}
  /if (${MobHeading}+360>${MeHeading}-15 && ${MobHeading}+360<${MeHeading}+15 && ${Me.CombatState.Equal[combat]}) /multiline ; /varset AggroFlag 1;/if (!${Me.Mount.ID} && !${Me.Standing}) /stand;/goto :BreakOutCheckAggro
 } else {
  /if (!${AggroFlag}) {
    /if (${Defined[ADCharmID]}) /if (${ADCharmID} && ${ADCharmID}==${Me.Pet.ID} && !${Me.Sitting}) /multiline ; /squelch /face id ${ADCharmID};/delay 1s
    /if (!${Me.Mount.ID} && !${Me.Sitting}) /sit on
    /goto :BreakOutCheckAggro
  }
}
/varcalc MiniIterate ${MiniIterate}+1
/goto :CheckNext
:BreakOutCheckAggro
/return

Sub AdvPathCall
/declare tZoneName string local
/declare tZoneTimer timer local
/if ((${Me.State.Equal[feign]} || ${Spawn[${FollowID}].Type.Equal[corpse]}) && ${AdvPath.Following} && !${AdvPath.Paused}) /multiline ; /afollow off;/return
/if (${FollowFlag} && !${Me.State.Equal[feign]}) {
  /if (${Me.Level}<51 && ${Me.Sneaking}&&${Me.GroupSize}) /multiline ; /doability sneak;/delay 5
  /if ((${Spawn[${FollowID}].Distance3D}>${FollowDistance} && ${Spawn[${FollowID}].Name.Find[${FollowName}]}) || (!${NetBots[${FollowName}].InZone} || !${Spawn[${FollowName}].ID}) && (!${Me.Mount.ID} && !${Me.Standing})) /sit off
  /if ((${NetBots[${FollowName}].InZone} || ${Defined[${Spawn["${FollowName}"]}]}) && ${FollowID}!=${Spawn[pc ${FollowName}].ID}) /multiline ; /squelch /stick off;/varset FollowID ${Spawn[pc ${FollowName}].ID};/varset FollowFlag 1
  /if (${Defined[CurrLoc]}) {
    /call AdvPathPoll
    /return
  } else /if (${AdvPath.Active} && ${AdvPlug}) {
    /call AdvPlugCall
|    /return
    /goto :tZoneCheck
  }

  /if (${Stick.Status.Equal[paused]}&&${Cast.Status.Equal[I]}&&!${Me.Casting.ID}) /stick unpause
  /if (${Select[${Stick},off]}) {
    /if (${FollowFlag}>=2) {
      /if (!${Zone.Name.Equal[${ZoneName}]}) /return
      /delay 5s ${Spawn[${FollowID}].ID}
    }
    /if ((${NetBots[${FollowName}].InZone} || ${Spawn[${FollowName}].ID}) && ${Spawn[${FollowName}].ID}) {
      /if (${FollowFlag}==2) /multiline ; /varset FollowFlag 0;/return
|      /if (${FollowFlag}==2 && ${Spawn[pc radius ${Math.Calc[${BotFollowDistance}+15]} ${FollowName}].ID}) /return
      /if (${Target.ID}!=${FollowID}) /tar id ${FollowID}
      /delay 8s ${Target.ID}==${FollowID}
      /if (${Target.ID}!=${FollowID}) /multiline ; /tar id ${FollowID};/delay 5
      /multiline ; /squelch /stick ${FollowDistance} id ${FollowID} uw;/varset FollowFlag 2;/return
    }
:tZoneCheck
    /if (!(${NetBots[${FollowName}].InZone} || ${Spawn[${FollowName}].ID}) && ${FollowFlag}<10) {
      /if (${Select[${NetBots[${FollowName}].State},dead]} || ${Spawn[${FollowName}'s corpse].ID} || ${FollowFlag}>=${FollowFlagZoneDelay}+20) /multiline ; /squelch /stick off;/if (${AdvPath.Following}) /squelch /afollow off;/varset FollowFlag 0;/return
        /if (${Zone.Name.Equal[${ZoneName}]}) {
          /if (!${Me.Mount.ID} && !${Me.Standing}) /stand
          /bc Trying to Zone
          /varset FollowFlag 10
          /if (${Spawn[Magus radius 100].ID}) {
            /varset tZoneTimer 300s
 :tMasterZone
            /if (!${NetBots[${FollowName}].ID} && ${tZoneTimer}) /goto :tMasterZone
            /if (!${Zone[${NetBots[${FollowName}].Zone}].ShortName.Length} || ${Me.Invis} || !${NetBots[${FollowName}].ID}) /multiline ; /bc Can't zone through;/varset FollowFlag 0;/return
            /varset tZoneName ${Zone[${NetBots[${FollowName}].Zone}].ShortName}
           }
        /call MBZone ${tZoneName}
        /if (!${Macro.Return}) /multiline ; /bc Can't zone through;/varset FollowFlag 0
      }
    }
  }
}
/return

Sub AdvPlugCall(string Toggle)
| Toggle : Can be [on//pause/off/stop/,ToonName]
/if (!${AdvPath.Active} || ${AdvPath.Paused} && ${FollowFlag}<2 && !${ACState} && !${Defined[Toggle]}) /multiline ; /varset FollowFlag 0;/goto :aPlugCallEnd
| Check if AdvPath getting stuck, or sitting when should be following..  Not needed with revised plugin.  (added keypress)
 /if (!${AdvPath.Paused} && !${AdvPath.Idle} && !${Defined[Toggle]} && ${Spawn[pc ${FollowName}].Distance}>30 && (${Me.State.Equal[sit]} || !${Me.Moving})) {
  /if (${Me.State.Equal[sit]}) /stand
  /delay 1s ${Me.State.Equal[STAND]}
  /if (!${Me.Moving}) /multiline ; /squelch /afollow off;/goto :aFollowStick
  /return
 }
/if (${Defined[Toggle]}) {
  /if (${Select[${Toggle},off,stop]}) /multiline ; /squelch /afollow off;/varset FollowFlag 0;/goto :aPlugCallEnd
  /if (${Select[${Toggle},off,stop]} && ${AdvPath.Following}) /multiline ; /varset FollowFlag 0;/squelch /afollow off;/goto :aPlugCallEnd
  /if (${AdvPath.Following} && ${Toggle.Find[pause]}) /multiline ;/squelch /afollow pause;/varset FollowFlag 4;/goto :aPlugCallEnd
  /if (${Toggle.Equal[on]} && !${AdvPath.Following} && ${FollowName.Length} && ${FollowID} && ${Spawn[pc ${FollowName}].ID}) /goto :aFollowStick
  /if (${Spawn[pc ${Toggle}].ID}) {
    /if (${AdvPath.Following} && ${Spawn[pc ${Toggle}].ID}!=${Spawn[pc ${AdvPath.Monitor}].ID} || ${AdvPath.Paused}) /squelch /afollow off
    /if (!${FollowID} || !${FollowName.Length} || ${FollowID}!=${Spawn[pc ${Toggle}].ID}) /multiline ; /varset FollowID ${Spawn[pc ${Toggle}].ID};/varset FollowName ${Spawn[pc ${Toggle}].CleanName}
    /goto :aFollowStick
  }
}
/if (${FollowFlag} && ${AdvPath.Paused}) /return
/if (${Advpath.Following} && !${ACState} && !${Toggle.Length} && ${FollowFlag}) /return
/if (${Advpath.Paused} && ${ACState} && ${Spawn[pc ${TankName}].Distance}<=${ACLeash}) /return
/if (${FollowName.Length} && ${FollowID} && !${ACState} && !${AdvPath.Following} && !${AdvPath.Paused}) /goto :aFollowStick
/if (${AdvPath.Following} && ${ACState} && !${AdvPath.Paused}) /multiline ; /squelch /afollow pause;/goto :aPlugCallEnd
| /if (${AdvPath.Paused} && ${FollowFlag}==1 && !${ACState}) /squelch /afollow unpause;/goto :aPlugCallEnd

/return
:aFollowStick
  /if (!${Spawn[pc ${FollowName}].ID} || !${FollowID} && !${FollowName.Length} || ${FollowName.Equal[${Me.CleanName}]} && (!${FollowID} || ${FollowID}==${Me.ID})) /return
  /if (${Spawn[pc ${FollowName}].ID}!=${FollowID} && ${FollowName.NotEqual[${Me.CleanName}]}) /varset FollowID ${Spawn[pc ${FollowName}].ID}
  /squelch /target id ${FollowID}
  /delay 1s ${Target.ID}==${FollowID}
 /squelch /afollow on
  /varset FollowFlag 1
:aPlugCallEnd
/return
