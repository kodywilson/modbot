
#event ImHit "#*# YOU, but #*#"
#Event ImHit "#*#YOU for#*#points of damage."

#Event CantHit "#*#can't hit them from here#*#"
#Event CantHit "#*#You cannot see your target#*#"

Sub LoadAPINIVals
/declare APAggro int outer 0
/declare APLoopCntr int outer 0
/declare APCheckTimer timer outer
/declare APPullDelay timer outer
/declare FailCheck bool outer FALSE
/call LoadINIVar AdvPull APCheckTime 0 APCheckTime "${IniFile}" int
/call LoadINIVar AdvPull APRadius 40 APRadius "${IniFile}" int
/call LoadINIVar AdvPull APMobMax 1 APMobMax "${IniFile}" int
/call LoadINIVar AdvPull APScript " " APScript "${IniFile}" string
/call LoadINIVar AdvPull APPath " " APPath "${IniFile}" string
/call LoadINIVar AdvPull APRetPath " " APRetPath "${IniFile}" string
/call LoadINIVar AdvPull APBefore " " APBefore "${IniFile}" string
/call LoadINIVar AdvPull APAfter " " APAfter "${IniFile}" string
/call LoadINIVar AdvPull APAnnounce " " APAnnounce "${IniFile}" string
/call LoadINIVar AdvPull APRetries 1 APRetries "${IniFile}" int
| /call LoadINIVar AdvPull APSpell " " APSpell "${IniFile}" string
/declare APTargetID int outer 0
/return

Sub AdvPull
  /if (${MBPause} || !${DoPull} || (${APCheckTimer} && !${Defined[Param0]})) /return
|  /if (${Spawn[npc loc 440.80 -242.50 radius 60].ID}) /return
|  /if (${SpawnCount[npc loc 440.80 -242.50 radius 60 zradius 100 noalert ${AlertList}]}<1) /return
  /if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:AdvPull
|  /echo APTargetID : ${APTargetID} - APAggro : ${APAggro} - MobCount : ${CurrentMobCount} - PathLoc : ${PathLoc}
  /call ADSpawnCheck
  /if (!${Defined[Param0]}) {
    /varset APCheckTimer ${APCheckTime}s
    /if (${CurrentMobCount} && !${ACAssistPct}>=100) /return
  }
  /if (${Spawn[${APTargetID}].PctHPs}<=0 || ${Spawn[${APTargetID}].Type.Equal[Corpse]} || !${Spawn[${APTargetID}].ID} || ${Spawn[${APTargetID}].Distance}>400 && !${CurrentMobCount}) /multiline ; /varset APTargetID 0;/varset APAggro 0
  /declare sReturn string local FALSE
  /declare sVarC string local
  /declare sVarL string local
  /declare a int local
  /declare b int local
  /declare tTarCnt int local
  /declare tTargetID int local
  /declare tDistance int local
  /declare Tries int local 0
  /declare tStr bool local
  /declare sNBClnt string local ${NetBots.Client}
  /doevents
  /if (${FailCheck}) /varset FailCheck FALSE
  /if (${APBefore.Length}>3) /call IniCommand "${APBefore}"

   /if (!${Defined[Param0]}) {
		/if (${Me.State.Equal[sit]} && (${APPath.Length}>2 || ${ADMobCount})) /stand
    /keypress centerview
      /if (!${CurrentMobCount} && ${APPath.Length}>2 && !${APTargetID}) {
/echo Trying to aggro a mob
:RunPath
        /if (${PathLoc}>1 || !${PathLoc} || ${PathLoc}==1 && !${CurrentMobCount} || ${FailCheck}) /call MBWayPlay ${APPath} b
        /if (${APRetPath.Length} && ${APTargetID}) {
/echo Doing Return path
           /call MBWayPlay ${APRetPath} b advpull
/echo Return Path ${Macro.Return}
        }
      }
   }
   /if (${CurrentMobCount} && (!${APAggro} || !${APTargetID})) /goto :tTop
  /if (${APAggro} && ${APTargetID} || ${Melee.Combat} || ${Spawn[npc ${APTargetID}].ID} || ${NetBots[${TankName}].Attacking}) /goto :tEnd
  :tTop
  /for a 0 to ${Group.Members}
    /if (!${NetBots[${Group.Member[${a}].Name}].ID}) /varset sNBClnt ${sNBClnt} ${Group.Member[${a}].Name}
  /next a

  /doevents ImHit
  /call ADSpawnCheck
  /if (!${CurrentMobCount} || !${DoPull}) /goto :tEnd
  :tRecheck
  /for a 1 to ${ADMobMax}
    /if (!${NearestSpawn[${a},npc].ID}) /next a
    /for b 1 to ${ADMobMax}
      /if (!${ADMobList[${b}]}) /next b
       /if (${ADMobList[${b}]} && ${NearestSpawn[${a},npc].ID}==${ADMobList[${b}]}) /multiline ; /varset tTargetID ${ADMobList[${b}]};/goto :tGotTarget
    /next b
  /next a
  /if (!${tTargetID}) {
  /for a 1 to ${ADMobMax}
    /if (!${NearestSpawn[${a},pet].ID}) /next a
    /for b 1 to ${ADMobMax}
    /if (!${ADMobList[${b}]}) /next b
      /if (${ADMobList[${b}]} && ${NearestSpawn[${a},pet].ID}==${ADMobList[${b}]}) /multiline ; /varset tTargetID ${ADMobList[${b}]};/goto :tGotTarget
    /next b
  /next a
  }
/echo NoTarget
  /goto :tEnd
 :tGotTarget
 /if (${Me.Sitting}) /stand
 /if (${APPath.Length} && ${SpawnCount[npc loc ${Spawn[${tTargetID}].X} ${Spawn[${tTargetID}].Y} radius ${APRadius}]}>${APMobMax} && ${Math.Distance[${Me.Y},${Me.X}:${MakeCampY},${MakeCampX}]}>15) {
   /echo I see ${ADMobCount} mobs, too many near by [${Spawn[${tTargetID}].CleanName}]
   /varset tTargetID 0
   /goto :tEnd
 }
  /if (${APPath.Length} && ${Melee.Enable} && ${meleemvi[melee]}<${Spawn[${tTargetID}].Distance3D}) /squelch /melee plugin=0
  /squelch /face nolook id ${tTargetID}
  /delay 5
  /if (${Target.ID}!=${tTargetID}) /squelch /tar id ${tTargetID}
  /delay 1s ${Target.ID}==${tTargetID}
|  Remove mob from ADMobList if it's agro on someone else.  (Don't pull other's mobs.)  Then re-check  only works with LAHoTT.
  /if (${Me.LAHoTT} && ${Target.ID}) {
  	/if (${SpawnCount[pc loc ${Spawn[${tTargetID}].X} ${Spawn[${tTargetID}].Y} radius 50]}>${SpawnCount[pc group loc ${Spawn[${tTargetID}].X} ${Spawn[${tTargetID}].Y} radius 50]}) /delay 3s ${Me.TargetOfTarget.ID}
    /if (${Me.TargetOfTarget.ID} && !${Spawn[group ${Me.TargetOfTarget}].ID} && !${NetBots[${Me.TargetOfTarget}].ID} && !${Group.Member[${Me.TargetOfTarget.ID}].Master} && ${Spawn[${Me.TargetOfTarget}].Type.NotEqual[corpse]} && ${Spawn[${Me.TargetOfTarget}].Distance}<300) {
      /for a 1 to ${ADMobMax}
        /if (${Target.ID}==${ADMobList[${a}]}) /varset ADMobList[${a}] 0
      /next a
      /echo Skipping mob that's aggro on someone else..
      /goto :tRecheck
    }
  }
    /if (${meleemvi[melee]}<${Spawn[${tTargetID}].Distance3D}) /echo [ ${Target.CleanName} ]
  /doevents
  /if (${Defined[Param0]} || !${APAggro} || !${APTargetID}) {
 :Dopull
    /varset tDistance ${Target.Distance}
    /if (${APScript.Length}>3) {
    	/varcalc Tries ${Tries}+1
    	/call MBScript ${APScript}
|    } else /if (!${APScript.Length} && !${APPath.Length} && ${APSpell.Length}) {
|    	/if (${Me.Book[${Me.Book[${APSpell.Arg[1,|]}]}].ID}) /call MQ2Cast "${APSpell.Arg[1,|]}" ${APSpell.Arg[2,|]} ${Spell[${APSpell.Arg[1,|]}].MyCastTime} -targetID|${tTargetID}
    }
    	
    /doevents CantHit
    /if (${FailCheck} && !${Me.CombatState.Equal[combat]}) {
|     /if (${PathLoc} && ${PathLoc}<2) {
|     	/if (${APPath.Length}) /call MBWayPlay ${APPath} b
|     	/goto :tTop
|     }
      /if (${Tries}<=${APRetries}) {
      	/echo PathLoc.. ${PathLoc}  Strafe...
          /if (${tStr}) {
             /keypress strafe_left hold
             /delay 3
             /keypress strafe_left
             /varset tStr FALSE
            } else {
             /keypress strafe_right hold
             /delay 6
             /keypress strafe_right
             /varset tStr TRUE
          } 
				/goto :DoPull
      }
      /call MBWayPlay ${APPath} AP${Math.Calc[${PathLoc}+1]}
      /goto :tEnd
    }
    /if ((${APPath.Length} && ${PathLoc}>1)) /delay 2s ${Target.Distance}<${tDistance} || ${Me.TargetOfTarget.ID}
    /if (${SpawnCount[pc group loc ${Spawn[${tTargetID}].X} ${Spawn[${tTargetID}].Y} radius 40]}<${SpawnCount[pc group]}) /delay 2s ${Me.CombatState.Equal[combat]} || ${sNBClnt.Find[${Spawn[${Me.TargetOfTarget.ID}].CleanName}]} || ${Target.Distance}<${tDistance}
    /if (${Me.CombatState.NotEqual[combat]} && ${sNBClnt.Find[${Spawn[${Me.TargetOfTarget.ID}].CleanName}]} && ${APRetries}>${Tries}) /goto :Dopull
    /if (!${Me.LAHoTT}) {
        /if (${Target.ID}==${tTargetID}) /assist
        /echo Assisted
        /delay 3s ${Target.ID}!=${tTargetID}
        /delay 2s ${Target.ID}==${Me.ID} || ${sNBClnt.Find[${Spawn[${Target.ID}].CleanName}]}
        /if (${Target.ID}!=${Me.ID} && !${sNBClnt.Find[${Spawn[${Target.ID}].CleanName}]} && !${NetBots[${Spawn[${Target.ID}].CleanName}].ID} ) {
        /echo Didn't aggro
        /goto :tEnd
       } else {
        /varset APTargetID ${tTargetID}
        /echo APTargetID Set to ${APTargetID} : tTargetID== ${tTargetID}
:targetloop
    /squelch /target id ${APTargetID}
        /delay 3s ${Target.ID}==${APTargetID}
        /if (${Target.ID}!=${APTargetID} || ${NetBots[${Spawn[${Target.ID}].CleanName}].ID}) /goto :targetloop
    }
    } else /if (${Me.LAHoTT}) {
    /delay 5s ${Me.TargetOfTarget.ID}==${Me.ID}
    /if (${Me.TargetOfTarget.ID}!=${Me.ID} && !${sNBClnt.Find[${Spawn[${Me.TargetOfTarget.ID}].CleanName}]}) /goto :tEnd
    /if (${Me.TargetOfTarget.ID}==${Me.ID} || ${sNBClnt.Find[${Spawn[${Me.TargetOfTarget.ID}].CleanName}]}) /varset APTargetID ${Target.ID}
    }
  }
|  /if (${tTargetID} && !${Target.ID}==${tTargetID}) /squelch /target id ${tTargetID}
  /if (${APAnnounce.Length}>2) /docommand ${APAnnounce}
  /multiline ; /varset APAggro 1;/varset sReturn TRUE;/varset APLoopCntr 0
 :tEnd
| /echo ModPull - tEnd - tTargetID : ${tTargetID} - APTargetID : ${APTargetID}
  /if (!${APTargetID} || ${Target.ID}!=${APTargetID}) /squelch /target clear
 /if (${APTargetID} && ${Target.ID}!=${APTargetID}) /squelch /target id ${APTargetID}
 /if (!${Melee.Enable} && (!${PathLoc} || ${PathLoc}==1)) /squelch /melee plugin=1
 /if (!${sReturn} && ${APPath.Length}>2 && !${CurrentMobCount} && ${PathLoc}==1) {
    /varset APCheckTimer ${APCheckTime}s
    /if (${ACStartHeading} && ${PathLoc}==1) /face Heading ${ACStartHeading}
 }
 /multiline ; /varset APPullDelay 30s
 /return ${sReturn}

Sub CompleteAP
    /if (${MBPause} || !${DoPull}) /return
  /declare sVarC string local
  /declare sVarL string local
  /declare a int local
    /if (${APAfter.Length}>3 && ${APAggro}) {
      /call IniCommand "${APAfter}"
      /varset APAggro 0
    }
    /if (${DoMelee} && (${APTargetID} || ${SpawnCount[npc radius ${ACLeash} zradius 100]} || !${ACState})) /call AdvCombat
/return

Sub Event_ImHit
  /if (${DoPull} && !${APTargetID}) {
  /if (${MBDebug}) /echo Event_ImHit DoPull: ${DoPull} APTargetID: ${APTargetID} (should be 0)
    /varset APAggro 1
    /if (${Target.ID}!=${Spawn[npc radius 25].ID} || ${Target.ID}==${Me.ID}) /squelch /target clear
    /delay 2
    /delay 1s ${Target.ID}
    /if (${Target.ID} && ${Target.ID}!=${Me.ID}) /varset APTargetID ${Target.ID}
  } else /if (!${DoPull}) {
  	/call MBScript Defense
  }  

  /if (${Me.CombatState.Equal[combat]}) /varset SitCheckTimer 5s
/return

Sub Event_CantHit
  /if (${DoPull} && !${APTargetID} && !${APAggro}) /multiline ; /varset FailCheck TRUE;/echo Can't hit, moving on.
/return
