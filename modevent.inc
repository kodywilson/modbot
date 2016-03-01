#Event AECEventA "#*#|${AECustomEvent[1]}|#*#"
#Event AECEventB "#*#|${AECustomEvent[2]}|#*#"
#Event AECEventC "#*#|${AECustomEvent[3]}|#*#"

Sub Event_AECEventA
  /call MBScript AECustomEvent1
/return

Sub Event_AECEventB
  /call MBScript AECustomEvent2
/return

Sub Event_AECEventC
  /call MBScript AECustomEvent3
/return

Sub LoadAEINIVals
 /declare a int local
 
| /call LoadINIVar AdvEvent AECustom 0 AECustom "${IniFile}" int
 /declare AECustom int outer 3
 /if (${AECustom}) {
   /declare AECustomEvent[${AECustom}] string outer
   /for a 1 to ${AECustom}
      /call LoadINIVar AdvEvent AECustom${a} " " AECustomEvent[${a}] ${IniFile}
      /if (${AECustomEvent[${a}].Length}<=2) /varset AECustomEvent[${a}] ${Math.Rand[999]}${Math.Rand[999]}${Math.Calc[${Me.Name.Length}*77]}${Math.Rand[999]}${Math.Rand[999]}
   /next a
 }
 
 
 /call LoadINIVar AdvEvent AECount 0 AECount "${IniFile}" int
 /if (${AECount}) {
   /call LoadINIVar AdvEvent AECheckTime 8 AECheckTime "${IniFile}" int
   /declare AECheckTimer timer outer
   /declare AEGem[${AECount}] string outer
   /declare AESpell[${AECount}] string outer
   /declare AESpellFoci[${AECount}] string outer
   /declare AEDurMod[${AECount}] string outer
   /declare AEEventMinMana[${AECount}] int outer
   /declare AEEventMinHP[${AECount}] int outer
   /declare AEMinMana[${AECount}] int outer
   /declare AEMaxMana[${AECount}] int outer
   /declare AEMinHP[${AECount}] int outer
   /declare AEMaxHP[${AECount}] int outer
   /declare AETarType[${AECount}] string outer
   /declare AESpellAlias[${AECount}] string outer
   /declare AEAnnounce[${AECount}] string outer
   /declare AEDelay[${AECount}] int outer
   /declare AETotDelay[${AECount}] int outer
   /declare AETarCnt[${AECount}] int outer
   /declare tSpellID int local
   /for a 1 to ${AECount}
     /call LoadINIVar AdvEvent AEGem${a} " " AEGem[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AESpell${a} " " AESpell[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AESpellFoci${a} " " AESpellFoci[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEDurMod${a} 0 AEDurMod[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEDelay${a} 0 AEDelay[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEEventMinMana${a} 0 AEEventMinMana[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEEventMinHP${a} 0 AEEventMinHP[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEMinMana${a} 0 AEMinMana[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEMaxMana${a} 0 AEMaxMana[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEMinHP${a} 0 AEMinHP[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEMaxHP${a} 0 AEMaxHP[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AETarType${a} "war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec self tnt" AETarType[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AESpellAlias${a} " " AESpellAlias[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AEAnnounce${a} " " AEAnnounce[${a}] ${IniFile}
     /call LoadINIVar AdvEvent AETarCnt${a} 1 AETarCnt[${a}] ${IniFile}
     /if (${Select[${AEGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]}) {
  	   	 /docommand ${If[${Me.Book[${Me.Book[${AESpell[${a}]} Rk. III]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AESpell[${a}]} Rk. III]}].ID},${If[${Me.Book[${Me.Book[${AESpell[${a}]} Rk. II]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AESpell[${a}]} Rk. II]}].ID},${If[${Me.Book[${Me.Book[${AESpell[${a}]}]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AESpell[${a}]}]}].ID},/bc Spell ${AESpell[${a}]} Not found in your book.  Check your .ini settings.]}]}]}
  	     /if (${Me.Book[${Me.Book[${AESpell[${a}]}]}].ID}!=${tSpellID}) /varset AESpell[${a}] ${Spell[${tSpellID}].Name}
      }
|     /varset AETarCnt[${a}] 1
   /next a
 }
/return

Sub AEEventCheck
 /if (${MBPause} || !${AECount} || ${AdvPath.Following} && !${AdvPath.Idle}) /return
 /if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:AEEventCheck
 /varset AECheckTimer ${AECheckTime}s
 /declare a int local
 /declare b int local
 /declare c int local
 /declare tCT int local
 /declare tSpellID int local
 /declare tGem int local
 /declare tSpellRange int local
 /declare tSpellDuration int local
 /declare tSpellDelay int local
 /declare tCastTime timer local
 /declare AEtTargetID int local
 /declare tTargetID int local
 /declare tName string local
 /declare tAEMob int local
 /declare tNBCnt int local ${NetBots.Counts}
 /declare sNBClnt string local ${NetBots.Client}
 /declare tCntr int local
 /if (${ADCount} && !${ADMobCount}) /call ADSpawnCheck
| /if (${ADCount} && ${ADMobCount} && (${Select[${ADTarType[1]},10,11]} && !${ADMDL[1,1]} || ${ADCount}>1 && ${ADTarType[1]}==12 && !${ADMDL[2,1]} && ${ADMobList[1]}!=${NetBots[${TankName}].TargetID})) /return
 /if (${Me.Class.ShortName.Equal[shm]} && ${DoMelee} && !${ACState} && ${ADMobCount}) /return
 /for a 1 to ${AECount}
   /if (!${DoEvents}) /return
   /if (${AETotDelay[${a}]}>${Macro.RunTime} || ${Me.PctHPs}<${AEEventMinHP[${a}]} || ${Me.PctMana}<${AEEventMinMana[${a}]} || !${AETarCnt[${a}]}) /goto :tNextEvent
   /multiline ; /varset tGem 0;/varset tSpellID 0
   /if (${AETarType[${a}].Find[cbt]} && !${AETarType[${a}].Find[idle]} && !${ADMobCount}) /goto :tNextEvent
   /if (${AETarType[${a}].Find[self]} && ${Me.PctHPs}>=${AEMinHP[${a}]} && ${Me.PctHPs}<=${AEMaxHP[${a}]} && ${Me.PctMana}>=${AEMinMana[${a}]} && ${Me.PctMana}<=${AEMaxMana[${a}]}) /multiline ; /varset tTargetID ${Me.ID};/goto :tFoundBot
   /if (${AETarType[${a}].Find[tnt]} && ${NetBots[${TankName}].Attacking}) /multiline ; /varset tTargetID ${NetBots[${TankName}].TargetID};/goto :tFoundBot
   /if (${AETarType[${a}].Length}>2) {
     /docommand ${If[${AETarType[${a}].Find[any]},/varset tCntr ${AETarType[${a}].Count[ ]},/varcalc tCntr ${AETarType[${a}].Count[ ]}+1]}
       /for b 1 to ${tNBCnt}
      /for c 1 to ${Math.Calc[${AETarType[${a}].Count[ ]}+1]}
            /if (${AETarType[${a}].Arg[${c}, ].Find[${Spawn[pc ${sNBClnt.Arg[${b}]}].Class.ShortName}]}) {
              /multiline ; /varset tName ${sNBClnt.Arg[${b}]};/varset tTargetID ${NetBots[${tName}].ID}
              /if (${NetBots[${tName}].PctHPs}>${AEMinHP[${a}]} && ${NetBots[${tName}].PctHPs}<=${AEMaxHP[${a}]} && ${NetBots[${tName}].PctMana}>${AEMinMana[${a}]} && ${NetBots[${tName}].PctMana}<=${AEMaxMana[${a}]}) {
                /if (${tCntr}<=1 || ${tCntr}>1 && ${AETarType[${a}].Find[any]}) /goto :tFoundBot
                /varcalc tCntr ${tCntr}-1
              }
            }
          /next c
        /next b
     /goto :tNextEvent
    }
 :tFoundBot

   /if (${Select[${AEGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]} ) {
|/echo was Spell
     /if (${Me.CurrentMana}+${Me.ManaRegen}<${Me.Book[${Me.Book[${AESpell[${a}]}]}].Mana} || ${Me.PctMana}<${AEEventMinMana[${a}]}) /goto :tNextEvent
     /multiline ; /varset tSpellID ${Me.Book[${Me.Book[${AESpell[${a}]}]}].ID};/varset tGem ${AEGem[${a}]};/varcalc tCT ${Spell[${tSpellID}].MyCastTime}*10+40
     } else /if ( ${AEGem[${a}].Find[item]} && ${Bool[${FindItem[=${AESpell[${a}]}]}]} ) {
|/echo was Item
       /if (${FindItem[=${AESpell[${a}]}].Timer} ) /goto :tNextEvent
       /multiline ; /varset tSpellID ${FindItem[=${AESpell[${a}]}].Spell.ID};/varset tGem 20;/varcalc tCT ${FindItem[=${AESpell[${a}]}].CastTime}*10+40
       } else /if ( ${AEGem[${a}].Equal[alt]} && ${Me.AltAbility[${AESpell[${a}]}].ID} ) {
|/echo was AA
         /if (!${Me.AltAbilityReady[${AESpell[${a}]}]}) /goto :tNextEvent
         /multiline ; /varset tSpellID ${Me.AltAbility[${AESpell[${a}]}].Spell.ID};/varset tGem 30;/varcalc tCT ${Me.AltAbility[${AESpell[${a}]}].Spell.CastTime}*10+40
         } else /if (${AEGem[${a}].Equal[script]}) {
|/echo was Script
           /call MBScript ${AESpell[${a}]}
   }
   /if (!${DoEvents} || ${MBPause} || ${Window[RespawnWnd].Open}) /return
   /varset tSpellRange ${Spell[${tSpellID}].Range}
|   /if (!${tGem} || !${tSpellID} || !${tTargetID} || ${Spawn[${tTargetID}].Distance3D}>${tSpellRange}) /goto :tNextEvent
   /if (!${tGem} || !${tSpellID} || !${tTargetID} || (${tSpellRange} && ${Spawn[${tTargetID}].Distance3D}>${tSpellRange})) /goto :tNextEvent
   /if (${tGem}<19) {
     /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
       /squelch /memorize "${Spell[${tSpellID}]}|gem${tGem}"
       /delay 5s ${Me.Gem[${tGem}].ID}==${tSpellID}
       /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) /multiline ; /bc EVENTS - FAILED MEMORIZE;/goto :tNextEvent
     }
     /if (!${Cast.Ready[${tGem}]}) /varcalc tCT ${Spell[${tSpellID}].RecastTime}*10+${tCT}
   }
   /if (${Me.Class.ShortName.Equal[shm]} && ${Select[${Spell[${tSpellID}].Name},Pained Memory,Ancestral Bargain,Ancestral Hearkening,Cannibalize II]} && ${Me.CombatState.Equal[RESTING]} && !${Me.Mount.ID}) /goto :tNextEvent
   /if (${tGem}<19 && !${Cast.Ready[${tGem}]}) /varcalc tCT ${Spell[${tSpellID}].RecastTime}*10+${tCT}
   /if (${Spell[${tSpellID}].TargetType.Equal[self]}) /varset tTargetID 0
   /if (${Spell[${tSpellID}].TargetType.Find[group]}) /docommand ${If[${Target.Type.Equal[pc]},/varset tTargetID ${Me.ID},/varset tTargetID 0]}
   /multiline ; /varset AEtTargetID 0;/varset tSpellDelay ${AESpellDelay[${a}]};/varset tCastTime ${tCT};/varset CastResult UNKNOWN
 :tCastEvent
        /if (${Debuff.Silenced} || ${Debuff.Stunned}  || ${Debuff.Charmed}) /return
        /if (${Me.Class.ShortName.Equal[shm]} && ${Cast.Ready} && !${Cast.Ready[${tSpellID}|${AEGem[${a}]}]} && ${AEGem[${a}].NotEqual[alt]}) /goto :tNextEvent
   /if (${DoHeals} && ${AHCount}) /call AHHealCheck
   /if (${AEtTargetID}) {
     /if (${Me.Class.ShortName.Equal[BRD]}) {
       /doevents CastRst
       /doevents CastImm
       /doevents CastStn
       /if (!${CastReturn.Equal[pending]}) /goto :tNextEvent
       /docommand ${If[${Window[CastingWindow].Open},/goto :tCastEvent,/goto :tContEvent]}
     }
     /if (${Cast.Status.Equal[i]}) {
       /if (${Select[${Cast.Result},cast_immune,cast_takehold]}) /multiline ; /varset tSpellDelay 99999;/goto :tContEvent
       /if (${Select[${Cast.Result},cast_outofrange,cast_cannotsee,cast_distracted,cast_notarget,CAST_CANCELLED,CAST_NOTREADY]}) /goto :tNextEvent
       /if (${Cast.Result.Equal[cast_success]}) /goto :tContEvent
       /goto :tNextEvent
     }
    } else /if (${Cast.Ready[${AESpell[${a}]}|${AEGem[${a}]}]} || ${Me.Class.ShortName.Equal[BRD]}) {
       /varset AEtTargetID ${tTargetID}
       /if (${Me.Class.ShortName.Equal[BRD]}) {
         /squelch /stoptwist
         /if (${Target.ID}!=${AEtTargetID}) {
           /squelch /target clear
           /delay 1s !${Target.ID}
           /squelch /tar id ${AEtTargetID}
           /delay 1s ${Target.ID}==${AEtTargetID}
         }
         /multiline ; /squelch /twist ${AEGem[${a}]};/varset CastResult PENDING;/delay 5
        } else {
          /varset MBCurrentSpell AE|${a}|${AEtTargetID}
|         /casting "${AESpell[${a}]}|${AEGem[${a}]}" -targetid|${AEtTargetID} ${AESpellFoci[${a}]}
         /call MQ2Cast "${AESpell[${a}]}" ${If[${Select[${AEGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]},gem,]}${AEGem[${a}]} ${Math.Calc[${Spell[${tSpellID}].CastTime}+1]}s CastCheck -targetid|${AEtTargetID} -bandolier|${AESpellFoci[${a}]}
         /if (!${AEtTargetID}) /varset AEtTargetID ${Me.ID}
       }
       /if (!${Select[${Cast.Result},cast_immune,cast_takehold,cast_outofrange,cast_cannotsee,cast_distracted,cast_notarget]}) /goto :tContEvent

   }
   /if (${interruptFlag}) /return
   /if (${tCastTime} || ${Cast.Timing}) /goto :tCastEvent
   /echo WTF - Event : ${AESpell[${a}]}
   /goto :tNextEvent
 :tContEvent
   /varset MiscCheckTimer 5s
   /if (${AEAnnounce[${a}].Length}) /docommand ${If[${AEAnnounce[${a}].Length}>10,/call ParseAnnounce "${AEAnnounce[${a}]}" "${Spawn[${AEtTargetID}].CleanName}" "${AESpell[${a}]}",/docommand ${AEAnnounce[${a}]} Evented: -[ ${Spawn[${AEtTargetID}].CleanName} ]- with ${AESpell[${a}]}]}
|   /docommand ${AEAnnounce[${a}]} Evented: -[ ${Spawn[${AEtTargetID}].CleanName} ]- with ${AESpell[${a}]}
   /if (${Spell[${tSpellID}].Duration}) {
     /docommand ${If[${AEDurMod[${b}]},/varcalc tSpellDuration ${Spell[${tSpellID}].Duration.TotalSeconds}*1.${AEDurMod[${b}]}+1,/varcalc tSpellDuration ${Spell[${tSpellID}].Duration.TotalSeconds}+1]}
     /varcalc tSpellDelay ${tSpellDelay}+${AEDelay[${b}]}+${tSpellDuration}+${Macro.RunTime}-5
     } else {
     /varcalc tSpellDelay ${tSpellDelay}+${AEDelay[${b}]}+${Macro.RunTime}
   }
   /if (${AEGem[${a}].NotEqual[script]}) {
| /echo Delay: ${tSpellDelay} Time: ${Macro.RunTime}
   /if (${Spell[${tSpellID}].RecastTime} && (${tSpellDelay}<=${Macro.RunTime} || ${Math.Calc[${tSpellDelay}-${Macro.RunTime}]}<${Spell[${tSpellID}].RecastTime})) /varset tSpellDelay ${Math.Calc[${Spell[${tSpellID}].RecastTime}+${Macro.RunTime}+1]}
| /echo Delay: ${tSpellDelay} Time: ${Macro.RunTime}
}
   /varcalc AETotDelay[${a}] ${tSpellDelay}
   /varset MBCurrentSpell
   /return
 :tNextEvent
  /doevents
  /next a
/return