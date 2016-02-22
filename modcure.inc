
Sub LoadAQINIVals
/declare a int local
/call LoadINIVar AdvCure AQCount 0 AQCount "${IniFile}" int
/if (${AQCount}) {
   /call LoadINIVar AdvCure AQCheckTime 8 AQCheckTime "${IniFile}" int
   /declare AQCheckTimer timer outer
|once I enter this sub, do I leave before I'm 100% done?
   /declare AQPriority bool outer
   /declare AQGem[${AQCount}] string outer
   /declare AQSpell[${AQCount}] string outer
|curetype - poisoned, cursed, diseased, all
   /declare AQSpellCureType[${AQCount}] string outer
   /declare AQSpellMinMana[${AQCount}] string outer
|amount of counters cured per cast -  not used
   /declare AQSpellCntr[${AQCount}] int outer
   /declare AQSpellFoci[${AQCount}] string outer
   /declare AQSpellRecast[${AQCount}] int outer
   /declare AQTarCnt[${AQCount}] int outer
|tartype war shm nec ? grp ? self? cbt?
   /declare AQTarType[${AQCount}] string outer
   /declare AQSpellAlias[${AQCount}] string outer
   /declare AQAnnounce[${AQCount}] string outer
   /declare tSpellID int local
   /for a 1 to ${AQCount}
     /call LoadINIVar AdvCure AQGem${a} " " AQGem[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQSpell${a} " " AQSpell[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQSpellCntr${a} 0 AQSpellCntr[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQSpellFoci${a} " " AQSpellFoci[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQSpellCureType${a} "Cursed Diseased Poisoned EnduDrain LifeDrain ManaDrain Blinded CastingLevel Charmed Feared Healing Mesmerized Resistance Rooted Silenced Slowed Snared SpellCost SpellSlowed SpellDamage Trigger All" AQSpellCureType[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQSpellMinMana${a} 0 AQSpellMinMana[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQSpellRecast${a} " " AQSpellRecast[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQTarCnt${a} " " AQTarCnt[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQTarType${a} "grp cbt idle " AQTarType[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQSpellAlias${a} " " AQSpellAlias[${a}] ${IniFile}
     /call LoadINIVar AdvCure AQAnnounce${a} " " AQAnnounce[${a}] ${IniFile}
     /if (${Select[${AQGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]} && ${AQSpell[${a}].Length}) {
  	   	 /docommand ${If[${Me.Book[${Me.Book[${AQSpell[${a}]} Rk. III]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AQSpell[${a}]} Rk. III]}].ID},${If[${Me.Book[${Me.Book[${AQSpell[${a}]} Rk. II]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AQSpell[${a}]} Rk. II]}].ID},${If[${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID},/bc Spell ${AQSpell[${a}]} Not found in your book.  Check your .ini settings.]}]}]}
  	     /if (${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID}!=${tSpellID}) /varset AQSpell[${a}] ${Spell[${tSpellID}].Name}
      } 
   /next a
}
/return

Sub AQCureCheck
/varset AQCheckTimer ${AQCheckTime}s
/if (${MBPause} || !${AQCount}) /return
/if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:AQCureCheck
/declare a int local
/declare b int local
/declare c int local
/declare x int local
/declare tBotCnt int local ${NetBots.Counts}
/declare tBotClient string local ${NetBots.Client}
| /declare tBotQue[25] int local
/declare tTarCnt int local
| /for a 1 to ${tBotCnt}
|   /if (${NetCure[${NetBots[${tBotClient.Arg[${a}]}].Buff}].Counters} || ${NetCure[${NetBots[${tBotClient.Arg[${a}]}].Buff}].Detrimentals} && !${Select[${NetBots[${tBotClient.Arg[${a}]}].Buff.Arg[1]},756,757]}) /varcalc tTarCnt ${tTarCnt}+1
| /next a
| /if (!${tTarCnt}) /return

| /echo Cure needed
/declare tGem int local
/declare tSpellID int local
/declare tSpellRange int local
/declare tSpellDuration int local
/declare tSpellDelay int local
/declare tSpellType string local
/declare tCastTime timer local
/declare tCT int local
/declare tTargetID int local
/declare AQtTargetID int local
/declare tRecast int local
| /declare tName string local
/declare tUpdated int local
/declare existingTar int local ${Target.ID}
/declare tBuffs string local
/declare tSongs string local
/if (${Melee.Combat} && ${OffTank}) /varset existingTar ${Target.ID}
/for a 1 to ${AQCount}
   /if (!${AQTarCnt[${a}]} || !${AQTarType[${a}].Find[cbt]} && ${ADMobCount}|| ${AQTarType[${a}].Find[cbt]} && !${ADMobCount} && !${AQTarType[${a}].Find[idle]}) /goto :tNextCureSpell
   /for b 1 to ${tBotCnt}
   	 /varset tBuffs ${NetBots[${tBotClient.Arg[${b}]}].Buff}
   	 /varset tSongs ${NetBots[${tBotClient.Arg[${b}]}].ShortBuff}
  	 /if (${AQTarType[${a}].Find[grp]} && !${Group.Member[${tBotClient.Arg[${b}]}.Index]} || ${NetBots[${tBotClient.Arg[${b}]}].Buff.Arg[1].Equal[756]}) /goto :tNextBot
		 /for x 1 to ${Math.Calc[${NetBots[${tBotClient.Arg[${b}]}].Buff.Count[ ]}+1]}
  	 	 /if (${Spell[${tBuff.Arg[${x}]}].CounterType.NotEqual[none]} && (${AQSpellCureType[${a}].Find[${Spell[${tBuff.Arg[${x}]}].CounterType}]} || ${AQSpellCureType[${a}].Find[all]})) /goto :cSpellFound
|  	 	 /echo ${tBotClient.Arg[${b}]} - Buff ${x} - /if (${Spell[${tBuffs.Arg[${x}]}].SpellType.Equal[detrimental]} && (${AQSpellCureType[${a}].Find[${Spell[${tBuff.Arg[${x}]}].SpellType}]} || ${AQSpellCureType[${a}].Find[all]})) /goto :cSpellFound
  	 	 /if (${Spell[${tBuffs.Arg[${x}]}].SpellType.Equal[detrimental]} && (${AQSpellCureType[${a}].Find[${Spell[${tBuff.Arg[${x}]}].SpellType}]} || ${AQSpellCureType[${a}].Find[all]})) /goto :cSpellFound
  	 /next x
  	 /if (${AQSpellCureType[${a}].Find[song]}) {
  	 	 /for c 1 to ${Math.Calc[${NetBots[${tBotClient.Arg[${b}]}].ShortBuff.Count[ ]}+1]}
  	 	   /if (${Spell[${tSongs.Arg[${c}]}].CounterType.NotEqual[none]} && (${AQSpellCureType[${a}].Find[${Spell[${tSongs.Arg[${c}]}].CounterType}]} || ${AQSpellCureType[${a}].Find[all]})) /goto :cSpellFound
  	 		 /if (${Select[${Spell[${tSongs.Arg[${c}]}].SpellType},detrimental]} && (${AQSpellCureType[${a}].Find[${Spell[${tSongs.Arg[${c}]}].SpellType}]} || ${AQSpellCureType[${a}].Find[all]})) /goto :cSpellFound
  	 	 /next c
  	 }
  	 /if (!${Debuff[${NetBots[${tBotClient.Arg[${b}]}].Buff}].Counters} && !${Debuff[${NetBots[${tBotClient.Arg[${b}]}].Buff}].Detrimentals}) /goto :tNextBot
     /if (${Debuff[${NetBots[${tBotClient.Arg[${b}]}].Buff}].Counters} || ${Debuff[${NetBots[${tBotClient.Arg[${b}]}].Buff}].Detrimentals} && ${NetBots[${tBotClient.Arg[${b}]}].Buff.Arg[1].NotEqual[756]}) {
  		  /for c 1 to ${Math.Calc[${AQSpellCureType[${a}].Count[ ]}+1]}
				  /if (${AQSpellCureType[${a}].Find[all]}) {
				    /multiline ; /varset tBotQue[${b}] ${a};/goto :cSpellFound
				  } else /if (${Debuff[${NetBots[${tBotClient.Arg[${b}]}].Buff}].${AQSpellCureType[${a}].Arg[${c}, ]}}) {
				    /multiline ; /varset tBotQue[${b}] ${a};/goto :cSpellFound
				  }
				/next c  
 		}
 		
		/goto :tNextBot
:cSpellFound
/echo Cure Needed
       /if (${Select[${AQGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]}) {
|/echo was Spell
        /multiline ; /varset tSpellID ${Me.Book[${Me.Book[${AQSpell[${a}]}]}].ID} ;/varset tGem ${AQGem[${a}]} ;/varcalc tCT ${Spell[${tSpellID}].MyCastTime}*10+40
        /if (${Me.CurrentMana}<${Me.Book[${Me.Book[${AQSpell[${a}]}]}].Mana} || ${Me.PctMana}<${AQSpellMinMana[${a}]} || ${Spell[${tSpellID}].Range} && ${Spawn[${tBotClient.Arg[${b}]}].Distance}>${Spell[${tSpellID}].Range} || !${Spell[${tSpellID}].Range} && ${Spell[${tSpellID}].AERange} && ${Spawn[${tBotClient.Arg[${b}]}].Distance}>${Spell[${tSpellID}].AERange}) /goto :tNextCureSpell
         
         /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
          /if (${Me.Casting.ID}) /delay 2s !${Me.Casting.ID}
           /squelch /memorize "${Spell[${tSpellID}]}|gem${tGem}"
           /delay 5s ${Me.Gem[${tGem}].ID}==${tSpellID}
           /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) /multiline ; /bc CURE - ${AQSpell[${a}]} - FAILED MEMORIZE;/goto :tNextCureSpell
           /varcalc tCT ${Spell[${tSpellID}].RecastTime}*10+${tCT}
         }
        } else /if ( ${AQGem[${a}].Find[item]} && ${Bool[${FindItem[=${AQSpell[${a}]}]}]} ) {
|/echo was Item
          /if (${FindItem[=${AQSpell[${a}]}].Timer} ) /goto :tNextCureSpell
          /multiline ; /varset tSpellID ${FindItem[=${AQSpell[${a}]}].Spell.ID};/varset tGem 20 ;/varcalc tCT ${FindItem[=${AQSpell[${a}]}].CastTime}*10+40
        } else /if (${AQGem[${a}].Equal[alt]} && ${Me.AltAbility[${AQSpell[${a}]}].ID}) {
/echo was Alt
				/varset tSpellID ${Me.AltAbility[${AQSpell[${a}]}].ID}
            /if (!${Me.AltAbilityReady[${AQSpell[${a}]}]} || ${Me.CurrentMana}<${Me.AltAbility[${AQSpell[${a}]}].Spell.Mana} || ${Spell[${tSpellID}].Range} && ${Spawn[${tBotClient.Arg[${b}]}].Distance}>${Spell[${tSpellID}].Range} || !${Spell[${tSpellID}].Range} && ${Spell[${tSpellID}].AERange} && ${Spawn[${tBotClient.Arg[${b}]}].Distance}>${Spell[${tSpellID}].AERange}) /goto :tNextCureSpell
            /multiline ; /varset tSpellID ${Me.AltAbility[${AQSpell[${a}]}].Spell.ID} ;/varset tGem 30 ;/varcalc tCT ${Me.AltAbility[${AQSpell[${a}]}].Spell.CastTime}*10+40
        } else /if (${AQGem[${a}].Equal[script]}) {
|/echo was Script
             /call MBScript ${AQSpell[${a}]}
             /varset tSpellDelay 0
             /goto :tContCure
       }
     /if (${Me.PctMana}<${AQSpellMinMana[${a}]} || !${tGem} || !${tSpellID}) /goto :tNextCureSpell
       /varset tTargetID ${NetBots[${tBotClient.Arg[${b}]}].ID}
       /if (!${tTargetID}) /goto :tNextBot
       /multiline ; /varset AQtTargetID 0;/varset tRecast 0;/varset tCastTime ${tCT}
:tCastCure
		/if (${Debuff.Silenced} || ${Debuff.Stunned} || ${Window[RespawnWnd].Open}) /return
       /doevents eqbc
| /echo ModCure - c ${c} - ${tBotQue[${b},1].Count[ ]}
       /if (${AQtTargetID}) {
         /if (${Spawn[${AQtTargetID}].State.Equal[dead]} || !${Spawn[${AQtTargetID}].ID}) /multiline ; /if (!${Me.Mount.ID}) /interrupt;/squelch /stoptwist;/goto :tNextBot
         /if (${Me.Class.ShortName.Equal[BRD]}) {
           /doevents
           /if (!${CastResult.Equal[pending]}) {
             /if (${Select[${CastResult},success]}) /goto :tContCure
             /if (${Select[${CastResult},immune]}) /multiline ; /varset tSpellDelay 4000;/goto :tContCure
             /if (${Select[${CastResult},resisted,stunned,reset]} && (${AQSpellRecast[${a}]} && ${AQSpellRecast[${a}]}>=${tRecast} || !${AQSpellRecast[${a}]})) /multiline ; /varcalc tRecast ${tRecast}+1;/varset AQtTargetID 0;/if (${AQSpellRecast[${a}]}) /goto :tCastCure
             /goto :tContCure
           }
           /delay 5
| BARD TESTING MELEE CHANGE DURING CAST
           /if (${DoMelee} && ${ACState} && ${ACMATarget} && ${Target.ID}!=${ACMATarget}) /multiline ; /if (${Target.ID}) /target clear;/tar id ${ACMATarget};/delay 5;/squelch /stick off;/killthis
           /if (!${tCastTime} && !${Cast.Timing}) /varset CastResult SUCCESS
           /goto :tCastCure
          } else /if (${Cast.Status.Equal[i]}) {
           /if (${Select[${Cast.Result},cast_recover]}) /multiline ; /varset tCastTime ${tCT}+30;/varset AQtTargetID 0;/echo recovery time issue;/goto :tCastCure
           /if (${Select[${Cast.Result},cast_immune,cast_takehold]}) /multiline ; /echo immune or did not take hold;/varset tSpellDelay 4000;/goto :tContCure
           /if (${AQSpellRecast[${a}]} && (${Select[${Cast.Result},cast_resist,cast_fizzle,CAST_CANCELLED,CAST_NOTREADY]} && ${AQSpellRecast[${a}]}>=${tRecast} || ${Select[${Cast.Result},cast_interrupted,CAST_CANCELLED]})) /multiline ; /varcalc tRecast ${tRecast}+1;/varset tCastTime ${tCT}+30;/varset AQtTargetID 0;/echo recast or interrupted;/goto :tCastCure
           /if (${Select[${Cast.Result},cast_outofrange,cast_cannotsee,cast_distracted,cast_notarget,CAST_CANCELLED,CAST_NOTREADY]} || !${AQSpellRecast[${a}]} && ${Select[${Cast.Result},cast_resist,cast_fizzle,CAST_CANCELLED,CAST_NOTREADY]}) /multiline ; /echo fizzle or resisted;/goto :tNextBot
           /if (${Cast.Result.Equal[cast_success]}) /goto :tContCure
         }
        } else /if (${Cast.Ready[${AQSpell[${a}]}|${AQGem[${a}]}]} || ${Me.Class.ShortName.Equal[BRD]}) {
          /if (${Target.ID}!=${tTargetID}) {
            /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
            /if (${Melee.Combat}) /attack off
            /squelch /tar id ${tTargetID}
            /delay 1s ${Target.ID}==${tTargetID}
          }
          /varset AQtTargetID ${tTargetID}
          /varset tCastTime ${tCT}
          /if (${Me.Class.ShortName.Equal[BRD]}) {
            /multiline ; /squelch /twist ${AQGem[${a}]};/varset CastResult PENDING;/varset tCastTime 32
           } else {
            /varset MBCurrentSpell AQ|${a}|${AQtTargetID}
|            /casting "${AQSpell[${a}]}|${AQGem[${a}]}" -targetid|${AQtTargetID} ${AQSpellFoci[${a}]}
            /call MQ2Cast "${AQSpell[${a}]}" ${If[${Select[${AQGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]},gem,]}${AQGem[${a}]} ${Spell[${AQSpell[${a}]}].MyCastTime}s AHHealCheck -targetid|${AQtTargetID} -bandolier|${AQSpellFoci[${a}]}
            /delay 5
          }
       }
       /if (${tCastTime} || ${Cast.Timing} || ${Cast.Status.Equal[i]}) /goto :tCastCure
       /multiline ; /echo Cast took too long, aborting this cast ${Spell[${tSpellID}].Name};/echo ${Cast.Status} - ${Cast.Result} - ${AQtTargetID} ${Spawn[${AQtTargetID}].Name};/echo a=${a} b=${b} c=${c} 
       /goto :tNextBot
:tContCure
| /if (${AdvPlug} && ${AdvPath.Paused}) /squelch /afollow unpause
/varset MBCurrentSpell  
       /if (${AQAnnounce[${a}].Length} && !${Select[${Cast.Result},cast_immune,cast_takehold]} && !${CastResult.Equal[immune]}) /docommand ${If[${AQAnnounce[${a}].Length}>5,/call ParseAnnounce "${AQAnnounce[${a}]}" "${Spawn[${AQtTargetID}].CleanName}" "${AQSpell[${a}]}",/docommand ${AQAnnounce[${a}]} Curing: -[ ${Spawn[${AQtTargetID}].CleanName} ]- with ${AQSpell[${a}]}]}
|       /docommand ${AQAnnounce[${a}]} Curing: -[ ${Spawn[${AQtTargetID}].CleanName} ]- with ${AQSpell[${a}]}
       /return
       /delay 5
|/echo ModCure - c ${c} - ${tBotQue[${b},1].Count[ ]}
:tNextBot
   /next b
:tNextCureSpell
/next a
/return
