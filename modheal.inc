
#Event AHDA "#*#can't cast spells while invulnerable#*#"

Sub LoadAHINIVals
 /declare a int local
 /call LoadINIVar AdvHeal AHCount 0 AHCount "${IniFile}" int
 /if (${AHCount}) {
   /call LoadINIVar AdvHeal AHCheckTime 2 AHCheckTime "${IniFile}" int
   /call LoadINIVar AdvHeal AHHealOOBC FALSE AHHealOOBC "${IniFile}" bool
   /call LoadINIVar AdvHeal AHHealMode "0|0|12" AHHealMode "${IniFile}" string
   /call LoadINIVar AdvHeal AHInterruptLevel 2 AHInterruptLevel "${IniFile}" int
   /call LoadINIVar AdvHeal AHClassPriority "enc,wiz,mag,nec,clr,dru,shm,pal,shd,war,bst,rng,ber,rog,brd,mnk" AHClassPriority "${IniFile}" string
   /call LoadINIVar AdvHeal AHAllowDismount TRUE AHAllowDismount "${IniFile}" bool
   /declare AHCheckTimer timer outer
   /declare AHGem[${AHCount}] string outer
   /declare AHSpell[${AHCount}] string outer
   /declare AHSpellFoci[${AHCount}] string outer
   /declare AHSpellAlias[${AHCount}] string outer
   /declare AHDurMod[${AHCount}] string outer
   /declare AHSpellMinMana[${AHCount}] int outer 0
   /declare AHClass[${AHCount}] string outer
   /declare AHTarCnt[${AHCount}] int outer 0
   /declare AHTargetName string outer
   /declare AHAnnounce[${AHCount}] string outer
   /declare AHCondition[${AHCount}] string outer
   /declare AHHealList string outer |
   /declare tSpellID int local
   /declare PRSpell int outer
   /for a 1 to ${AHCount}
     /call LoadINIVar AdvHeal AHGem${a} " " AHGem[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHSpell${a} " " AHSpell[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHSpellFoci${a} " " AHSpellFoci[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHDurMod${a} 0 AHDurMod[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHSpellMinMana${a} 0 AHSpellMinMana[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHSpellAlias${a} " " AHSpellAlias[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHAnnounce${a} " " AHAnnounce[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHTarCnt${a} 0 AHTarCnt[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHClass${a} "pc pet group hp0 war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec tnt mypet self" AHClass[${a}] ${IniFile}
     /call LoadINIVar AdvHeal AHPreCondition${a} "TRUE" AHCondition[${a}] ${IniFile}
     /if (${AHGem[${a}].NotEqual[script]} &&  ${AHSpell[${a}].Length}>2) {
      /if (${Select[${AHGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]} && ${AHSpell[${a}].Length}) {
  	   	 /docommand ${If[${Me.Book[${Me.Book[${AHSpell[${a}]} Rk. III]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AHSpell[${a}]} Rk. III]}].ID},${If[${Me.Book[${Me.Book[${AHSpell[${a}]} Rk. II]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AHSpell[${a}]} Rk. II]}].ID},${If[${Me.Book[${Me.Book[${AHSpell[${a}]}]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${AHSpell[${a}]}]}].ID},/bc Spell ${AHSpell[${a}]} Not found in your book.  Check your .ini settings.]}]}]}
  	     /if (${Me.Book[${Me.Book[${AHSpell[${a}]}]}].ID}!=${tSpellID}) /varset AHSpell[${a}] ${Spell[${tSpellID}].Name}
  	     /echo ${tSpellID} - ${AHSpell[${a}]}
      }
      /if (${Select[${AHGem[${a}]},item,potionbelt]}) {
        /if (${FindItem[${AHSpell[${a}]}].Spell.Name.NotEqual[NULL]}) /varset AHHealList ${AHHealList}${FindItem[${AHSpell[${a}]}].Spell}|
      } else {
        /varset AHHealList ${AHHealList}${AHSpell[${a}]}|
      }
      /if (${Select[${tSpellID},9755,9756,9757,14249,18270,18271,18272,18534,18535,18536,25165,25166,25167]}) /varset PRSpell ${a}

    }
   /next a
   /declare AHtTargetID int outer 0
| /echo AHHealList = ${AHHealList}
 }
/return

Sub AHHealCheck
 /if (${AHCheckTimer} || !${DoHeals} || !${AHCount}) /return
 /if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:AHHealCheck
 /varset AHCheckTimer ${AHCheckTime}s
 /declare a int local
 /declare b int local
 /declare x int local
 /declare tGem int local
 /declare tSpellID int local
 /declare tSpellRange int local
 /declare tSpellDuration int local
 /declare tHealClass string local
 /declare tTarCnt int local
 /declare tTargetID int local
 /declare AHtDurMod int local
 /declare NeedtoHeal bool local FALSE
 /declare sNBClnt string local ${NetBots.Client}
 /declare tgtLevel int local
 /declare PrevTarg int local 0    
 /declare AHClassL string local
 /declare AHClassR string local
  
 /if (!${Defined[AHtCnt]}) {
   /declare AHtCnt int outer 0
   /declare AHttl int outer 0
   /declare AHtSpellID int outer 0
   /declare AHtSpellRange int outer 0
   /declare AHtMembers string outer
   /declare AHtTargetHP int outer 0
   /declare AHtPctPS int outer 0
   /declare AHtDPS float outer 0
   /declare AHtPctAvr int outer 0
   /declare AHtDPSAvr int outer 0
   /declare AHtPctCntr float outer 0
   /declare tHurt int outer 0
   /declare AHtPreHP int outer 0
   /declare AHtPreID int outer 0
   /declare AHtTime int outer 0
   /declare AHtTimer timer outer 0
   /declare AHtPriority int outer
   /declare tMembers string outer
   /declare CastTimer timer outer 0
   /declare tPriority int outer 0
   /declare HealOnlyTimer timer outer 0
   /declare tPRSpell bool outer FALSE
}
/if (!${AHHealList.Find[${Me.Casting.Name}]} || !${Me.Casting.ID}) {
   /multiline ; /varset AHtCnt 0;/varset AHttl 0;/varset AHtSpellID 0;/varset AHtSpellRange 0;/varset AHtMembers ;/varset AHtTargetHP 0;/varset AHtPctPS 0;/varset AHtDPS 0;/varset AHtPctAvr 0;/varset AHtDPSAvr 0;/varset AHtPctCntr 0;/varset tHurt 0;/varset AHtPreHP 0;/varset AHtPreID 0;/varset AHtTime 0;/varset AHtTimer 0;/varset AHtPriority 0;/varset tMembers ;/varset CastTimer 0;/varset tPriority 0
}
:tHealTop
 /doevents AHDA
 /for a 1 to ${AHCount}
  /if (!${DoHeals} || ${MBPause} || ${Debuff.Silenced} || ${Debuff.Stunned}  || ${Debuff.Charmed}) /return
  /if (${Me.Ducking}) /keypress duck
  /if (${Debuff.Invulnerable}) {
  	/call event_AHDA
  	/return
  }
   /if (!${AHTarCnt[${a}]}) /goto :tNextHeal
   /multiline ; /varset tGem 0;/varset tSpellID 0
   /if (!${AHHealList.Find[${Me.Casting.Name}]}) /varset AHtTargetID 0

   /if (${Select[${AHGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]} ) {
|/echo was Spell
     /if (${Me.CurrentMana}<${Me.Book[${Me.Book[${AHSpell[${a}]}]}].Mana} || ${Me.PctMana}<=${AHSpellMinMana[${a}]}) /goto :tNextHeal
       /multiline ; /varset tSpellID ${Me.Book[${Me.Book[${AHSpell[${a}]}]}].ID} ;/varset tGem ${AHGem[${a}]}
       /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
         /squelch /memorize "${Spell[${tSpellID}]}|gem${tGem}"
         /delay 16s ${Me.Gem[${tGem}].ID}==${tSpellID}
         /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) /multiline ; /bc HEAL ${AHSpell[${a}]} - FAILED MEMORIZE;/goto :tNextHeal
       }
   } else /if (${AHGem[${a}].Find[item]} && ${Bool[${FindItem[=${AHSpell[${a}]}]}]}) {
|/echo was Item
       /if (${FindItem[=${AHSpell[${a}]}].Timer} ) /goto :tNextHeal
       /multiline ; /varset tSpellID ${FindItem[=${AHSpell[${a}]}].Spell.ID} ;/varset tGem 20
   } else /if (${AHGem[${a}].Equal[alt]}) {
|/echo was Alt
| /echo /if (!${Me.AltAbilityReady[${AHSpell[${a}]}]} || ${Me.CurrentMana}<${Me.AltAbility[${AHSpell[${a}]}].Spell.Mana} || ${Me.PctMana}<${AHSpellMinMana[${a}]} || !${Me.AltAbility[${AHSpell[${a}]}].ID}) /goto :tNextHeal
         /if (!${Me.AltAbilityReady[${AHSpell[${a}]}]} || ${Me.CurrentMana}<${Me.AltAbility[${AHSpell[${a}]}].Spell.Mana} || ${Me.PctMana}<${AHSpellMinMana[${a}]} || !${Me.AltAbility[${AHSpell[${a}]}].ID}) /goto :tNextHeal
         /multiline ; /varset tSpellID ${Me.AltAbility[${AHSpell[${a}]}].Spell.ID} ;/varset tGem 30
   } else /if (${AHGem[${a}].Equal[script]}) {
|/echo was Script
           /multiline ; /varset tGem 40;/varset tSpellID 0
   } else /if (${AHGem[${a}].Equal[ability]}) {
|/echo was Ability
						/if (!${Me.AbilityReady[${AHSpell[${a}]}]}) /goto :tNextHeal
             /multiline ; /varset tSpellID 0;/varset tSpellRange 50;/varset tGem 50
   } else /if (${AHGem[${a}].Equal[potionbelt]}) {
|/echo was potionbelt
   	/if (${FindItem[=${AHSpell[${a}]}].Timer} || !${FindItem[=${AHSpell[${a}]}].ID} || ${FindItem[=${AHSpell[${a}]}].Spell.SpellType.Equal[Beneficial]} && ${FindItem[=${AHSpell[${a}]}].Spell.Duration} && !${FindItem[=${AHSpell[${a}]}].Spell.Stacks[0]} || ${Me.PctHPs}>${AHClass[${a}].Arg[2,hp].Arg[1].Right[${Math.Calc[${AHClass[${a}].Arg[2,hp].Arg[1].Length}-1]}]}) /goto :tNextHeal
   	/echo Casting Potion
   		/for x 0 to 4
			  /docommand ${If[${Window[PotionBeltWnd].Child[PW_PotionSlot${x}_Button].Tooltip.Equal[${AHSpell[${a}]}]},/potionbelt Activate ${Math.Calc[${x}+1]},/next x]}
 			/goto :tNextHeal
   }
      
   /if (!${tGem}) /goto :tNextHeal

   /docommand ${If[${Spell[${tSpellID}].AERange} && ${tGem}<39,/varset tSpellRange ${Spell[${tSpellID}].AERange},/varset tSpellRange ${Spell[${tSpellID}].Range}]}
   /multiline ; /varset tHealClass ${AHClass[${a}]};/varset tHurt 0;/varset tMembers
   /if (${tHealClass.Find[self]} && ${Plugin[MQ2NetHeal].Name.Length}) {
   	/call CheckAHArgs "${tHealClass}"|hp
   	/if (${Me.PctHPs}<=${Macro.Return}) /multiline ; /varset tHurt 1;/varset tMembers ${Me.ID}
   }
   /if (!${Plugin[MQ2NetHeal].Name.Length}) {
   	/call GrpEval "${tHealClass}" ${tSpellRange}
   	/if (${Macro.Return.Equal[FALSE]}) /goto :tNextHeal
   	/if (${Macro.Return.Length}) {
   		/multiline ; /varset tHurt ${Macro.Return.Right[-${Math.Calc[${Macro.Return.Find[|]}]}-1]};/varset tMembers ${Macro.Return.Left[${Math.Calc[${Macro.Return.Find[|]}-1]}]}
   		/varset tMembers ${tMembers.Right[-1]}
   	}
  }
   /varset tHurt ${Math.Calc[${tHurt}+${NetWorst.Request[radius${tSpellRange} ${tHealClass}]}]}
   /if (!${tHurt} && ${AHtTargetID} && ${tTargetID} && ${AHtTargetID}==${tTargetID} && !${Me.Mount.ID} && ${AHtSpellID} && ${AHtSpellID}==${Me.Casting.ID} && (${Spawn[${AHtTargetID}].PctHPs}>99 && !${Spell[${Me.Casting.ID}].Duration})) {
     /if (!${Me.Mount.ID} || ${Me.Mount.ID} && ${AHAllowDismount}) /multiline ; /echo interrupt no heal needed;/interrupt
         /goto :tNextHeal
   }
   /if (!${tHurt} || ${AHTarCnt[${a}]}>1 && ${AHTarCnt[${a}]}>${tHurt}) /goto :tNextHeal
   /if (${NetWorst.Members.Length}) /varset tMembers ${tMembers} ${NetWorst.Members}
   /varset tTarCnt ${AHTarCnt[${a}]}
   /for b 1 to ${tHurt}
|   	/if (${tHurt} > 1 && ${AHHealList.Find[${Me.Casting.Name}]}) /echo tHurt - ${tHurt} : tMembers - ${tMembers} :  : a = ${a}
		/varset tTargetID ${tMembers.Arg[${b}]}
		/if (${Spawn[${tTargetID}].Type.Equal[corpse]} || ${tTarCnt}==1 && ${tTargetID}==${Me.ID} && ${Me.Song[${AHSpell[${a}]}].ID}) {
			/if (${b}<${tHurt}) /next b
			/goto :tNextHeal
		}
    /if (${AHCondition[${a}].Length}>4 || ${AHCondition[${a}].NotEqual[TRUE]}) {
      /if (${AHCondition[${a}].Left[1].Find[/]}) {
        /call IniCommand "${AHCondition[${a}]}" ${tTargetID} ${tSpellID}
        /if (${Macro.Return.NotEqual[TRUE]}) /goto :tNextHeal
      } else {
      	/call MBScript ${AHCondition[${a}]} ${tTargetID} ${tSpellID}    	
      	/if (!${Macro.Return}) /goto :tNextHeal
    	}
    }

    /if (${Spawn[${tTargetID}].Distance}>${ACLeash} && ${Select[${Spawn[${tTargetID}].Class.ShortName},shd,nec,mnk,brd]}) {
		 	    /if (!${NetBots[${tTargetID}].Feigning}) /docommand ${If[${tHurt}>1 && ${b}<${tHurt},/next b,/goto :tNextHeal]}
		      /if (${Plugin[MQ2Netbots].Version}>1.21) {
		        /if (${NetBots[${tTargetID}].CombatState}==0) /docommand ${If[${tHurt}>1 && ${b}<${tHurt},/next b,/goto :tNextHeal]}
		      }
     }
     /if (${tHealClass.Find[tnt]}) {
       /docommand ${If[(${ACMATarget} && ${ACState} && ${Spawn[${ACMATarget}].ID} && !${Spawn[${ACMATarget}].Type.Equal[corpse]}),/varset tTargetID ${ACMATarget},/goto :tNextHeal]}
     } else /if (${tHealClass.Find[self]}) {
        /docommand ${If[${tMembers.Find[${Me.ID}]},/varset tTargetID ${Me.ID},/goto :tNextHeal]}
     } else {
        /varset tgtLevel ${If[${Spawn[${tTargetID}].Type.Equal[pc]},${Spawn[${tTargetID}].Level},${Spawn[${tTargetID}].Master.Level}]}
        /if (${a}==${PRSpell} && (${${Spawn[${tTargetID}].Name}${PRSpell}} && ${${Spawn[${tTargetID}].Name}${PRSpell}}/10+1<${Spell[${tSpellID}].MyCastTime}-2 || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[${tSpellID}]})) /goto :tNextHeal
         /if (${tTarCnt}==1 && (${Spell[${AHSpell[${a}]}].Duration} && ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[${tSpellID}]} || (${AHCondition[${a}].Equal[pr]} || ${tSpellID}==13) && (${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[9755]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[9756]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[9757]} ||${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[18270]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[18271]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[18272]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[18534]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[18535]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[18536]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[25165]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[25166]} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[25167]}))) {
					 /if (${${Spawn[${tTargetID}].Name}${PRSpell}}/10+1<${Spell[${tSpellID}].MyCastTime} || ${Plugin[MQ2NetHeal].Name.Length})	/goto :tNextHeal
     		 }
         /if (${NetBots[${Spawn[${tTargetID}].Name}].ShortBuff.Find[${tSpellID}]} || ${tTarCnt}==1 && ${Spell[${tSpellID}].Duration} && !${Select[${AHGem[${a}]},item,alt]} && (${NetHeal[${tTargetID}].hot}>0 || ${Spell[${tSpellID}].Level}<255 && (${tgtLevel}<61 && ${Spell[${tSpellID}].Level}>65 || ${tgtLevel}<46 && ${Spell[${tSpellID}].Level}>60) || ${${Spawn[${tTargetID}].Name}${a}}>0)) /goto :tNextHeal
         /if (${tHealClass.Find[mypet]} && ${tTargetID}!=${Me.Pet.ID} || ${Spawn[${tTargetID}].Master.Type.Equal[pc]} && ${Select[${Spawn[${tTargetID}].Master.Class.ShortName},clr,wiz]} || !${AHHealOOBC} && (${Spawn[${tTargetID}].Type.Equal[pc]} && !${sNBClnt.Find[${Spawn[${tTargetID}].Name}]} || ${Spawn[${tTargetID}].Master.Type.Equal[pc]} && !${sNBClnt.Find[${Spawn[${tTargetID}].Master.CleanName}]})) /goto :tNextTarget
| *****************
 /if (${AHtTargetID} && !${Spawn[${AHtTargetID}].Type.Equal[corpse]}) /call CalcPPS ${tSpellID}
         

  /varset tPriority ${Select[${Spawn[${tTargetID}].Class.ShortName},${AHClassPriority}]}
  /if (!${Cast.Status.Equal[i]} && ${Me.Casting.ID}) {
| && !${Spell[${tSpellID}].Duration}
     /if ((!${AHHealList.Find[|${Spell[${Me.Casting.ID}].Name}|]} || ${Me.Casting.ID}==13 && ${tHurt}>1 && ${AHTarCnt[${a}]}>1) && (${AHtCnt} && ${AHtCnt}>=${AHInterruptLevel} && (${AHtCnt}>${a} && ${AHtSpellID} && ${AHtSpellID}!=${tSpellID} && ((${CastTimer}>(${Spell[${tSpellID}].MyCastTime}*10) && ${AHttl}>0 && ${Math.Calc[${CastTimer}*10]}>${AHttl}) || ${tTargetID}!=${AHtPreID} && ${AHtPriority} && ${AHtPriority}>${tPriority} )))) {
       /if (${Me.AltAbility[${AHSpell[${a}]}]} && !${Me.AltAbilityReady[${AHSpell[${a}]}]}) /goto :tNextHeal
       /if (!${Me.Mount.ID} || ${Me.Mount.ID} && ${AHAllowDismount}) {
         /echo Interrupted!!!
         /interrupt
     }
     /if (${Defined[interruptFlag]}) /varset interruptFlag TRUE
        /varset AHtTargetID 0
|           /goto :tHealTop
       /delay 1s (${Cast.Status.Equal[i]} && ${Cast.Ready[${AHSpell[${a}]}|${AHGem[${a}]}]})
    }
           
         }
         /if (${Spell[${tSpellID}].TargetType.Find[Group]}) {
           /varset tTargetID 99999
           } else /if (${Spell[${tSpellID}].TargetType.Equal[pet]} && ${tMembers.Find[${tTargetID}]} && ${Me.Pet.ID}) {
             /varset tTargetID ${Me.Pet.ID}
         }
     }
    /if (${AHtTargetID}) {
    	/if (${AHHealList.Find[${Me.Casting.Name}]} && ${Me.Casting.ID}) {
    		/next a
				/goto :tHealTop
			}
			/if (!${Me.Casting.ID} && ${Cast.Result.Equal[CAST_INTERRUPTED]}) /multiline ; /varset AHTTargetID 0;/goto :tHealTop
		}
:tCastHeal
     /if (${tGem}>=40) {
       /if (${Target.ID}!=${tTargetID} && ${tTargetID}) /squelch /tar id ${If[${tTargetID}==99999,${Me.ID},${tTargetID}]}
       /if (${tGem}==40) /call MBScript ${AHSpell[${a}]}
       /if (${tGem}==50) /doability ${AHSpell[${a}]}
       /return
     }
     /varset NeedtoHeal TRUE
| ************************************************************
  /if (${TankName.Equal[${Me.CleanName}]} && ${Target.ID}==${ACMATarget}) /varset PrevTarg ${Target.ID}
  /call CastHeal ${a} ${tTargetID} ${tSpellID}   
  /docommand ${If[${AHDurMod[${a}]},/varcalc AHtDurMod ((${Spell[${AHtSpellID}].Duration.TotalSeconds}*(${AHDurMod[${a}]}/100))+${Spell[${AHtSpellID}].Duration.TotalSeconds})*1000+6000,/varcalc AHtDurMod ${Spell[${AHtSpellID}].Duration.TotalSeconds}*1000+6000]}
  /if (${tPRSpell}) /goto :tContinue

 :tNextTarget
   /if (${Window[RespawnWnd].Open}) /return
   /next b
 :tNextHeal
   /doevents
 /next a

 /if (${AHtTargetID}) {

   /if (${Cast.Status.NotEqual[i]} || ${Me.Casting.ID} || ${AHHealList.Find[${Me.Casting.Name}]}) /goto :tHealTop
:tContinue
   /if (${Spell[${AHtSpellID}].Duration} && ${Plugin[MQ2NetHeal].Name.Length}) {
     /if (${Spell[${AHtSpellID}].AERange}) {
       /if (${NetWorst.Request[radius${AHtSpellRange} pc hp100 all]}) /varset AHtMembers ${NetWorst.Members}
       /netheal hot ${AHtDurMod} ${AHtMembers}
      } else {
       /netheal hot ${AHtDurMod} ${AHtTargetID}
     }
   } else /if (${Spell[${AHtSpellID}].Duration} && !${Plugin[MQ2NetHeal].Name.Length}) {
   /docommand ${If[${AHDurMod[${AHtCnt}]},/varcalc AHtDurMod ((${Spell[${AHtSpellID}].Duration.TotalSeconds}*(${AHDurMod[${a}]}/100))+${Spell[${AHtSpellID}].Duration.TotalSeconds}*10),/varcalc AHtDurMod ${Spell[${AHtSpellID}].Duration.TotalSeconds}*10]}
   /if (${AHtCnt}==${PRSpell}) /varcalc AHtDurMod ${AHtDurMod}+60
   	 /if (${Spell[${AHtSpellID}].AERange} && ${AHtMembers.Count[ ]}) {
   		/for x 1 to ${Math.Calc[${AHtMembers.Count[ ]}]}
   			/if (!${Defined[${Spawn[${AHtMembers.Arg[${x}]}].Name}${AHtCnt}]}) /declare ${Spawn[${AHtMembers.Arg[${x}]}].Name}${AHtCnt} timer outer
   			/varset ${Spawn[${AHtMembers.Arg[${x}]}].Name}${AHtCnt} ${AHtDurMod}
   		/next x
   	 } else {
   		/if (!${Defined[${Spawn[${AHtTargetID}].Name}${AHtCnt}]}) /declare ${Spawn[${AHtTargetID}].Name}${AHtCnt} timer outer
   		/varset ${Spawn[${AHtTargetID}].Name}${AHtCnt} ${AHtDurMod}
   	 }
   }
   /if (${NeedtoHeal} || ${HealOnlyTimer} || ${AHHealList.Find[${Me.Casting.Name}]}) /goto :tHealTop
 }
 /if (${HealOnlyTimer}) /multiline ; /varset AHtTargetID 0;/goto :tHealTop
 /varset MBCurrentSpell
 /if (!${AHHealList.Find[${Me.Casting.Name}]}) /varset AHtTargetID 0
 /if (${AdvPath.Paused} && ${FollowFlag}==3) /squelch /afollow unpause
 /if (${PrevTarget}) /target id ${PrevTarget}
/return

Sub CastHeal(int a, int tTargetID, int tSpellID)
     /varset tPRSpell FALSE
     /if (!${AHtTargetID} && ${Cast.Status.Equal[i]} && ${Cast.Ready[${AHSpell[${a}]}|${AHGem[${a}]}]}) {
     	/if (${Me.State.Equal[sit]}) /stand
        /if (${MakeCamp.Status.Equal[on]}) /makecamp pause
        /if (${Stick.Status.Equal[on]}) /multiline ; /stick pause;/delay 3 !${Me.Moving}
      /if (${AdvPath.Following} && !${AdvPath.Paused} && ${FollowFlag}<2) /multiline ; /varset FollowFlag 3;/afollow pause
        /varset MBCurrentSpell AH|${a}|${If[${tTargetID}==99999,0,${tTargetID}]}
				/if (${Spell[${tSpellID}].TargetType.Find[self]} && ${tTargetID}==${Me.ID} || ${tTargetID}==99999) /varset tTargetID ${Me.ID}
| /echo	/casting "${AHSpell[${a}]}|${AHGem[${a}]}" ${If[!${tTargetID}, ,-targetID|${tTargetID}]} ${AHSpellFoci[${a}]} -maxtries|2
       /casting "${AHSpell[${a}]}|${AHGem[${a}]}" ${If[!${tTargetID}, ,-targetID|${tTargetID}]} ${AHSpellFoci[${a}]} -maxtries|2
       /if (${AHHealMode.Arg[1,|]} && ${a}<=${AHHealMode.Arg[2,|]}) /varset HealOnlyTimer ${AHHealMode.Arg[3,|]}s
       /multiline ; /varset AHtTargetID ${tTargetID};/varset AHtSpellID ${tSpellID};/varset AHtSpellRange ${tSpellRange};/varset AHtCnt ${a};/varset AHtTime ${Math.Calc[${Spell[${tSpellID}].MyCastTime}*10]};/varset AHtTimer 1000;/varset CastTimer ${Math.Calc[${Spell[${tSpellID}].MyCastTime}*10]};/varset AHtMembers ${tMembers}
       /if (!${AHtTargetID} || ${AHtTargetID}==99999) /varset AHtTargetID ${Me.ID}
       /docommand ${If[${Target.ID}==${AHtTargetID},/varset AHtTargetHP ${Target.PctHPs},/varset AHtTargetHP ${Spawn[${AHtTargetID}].PctHPs}]}
       
       /if (${AHAnnounce[${a}].Length}) /docommand ${If[${AHAnnounce[${a}].Length}>10,/call ParseAnnounce "${AHAnnounce[${a}]}" "${Spawn[${AHtTargetID}].CleanName}" "${AHSpell[${a}]}",/docommand ${AHAnnounce[${a}]} Healing: -[ ${Spawn[${AHtTargetID}].CleanName} ]- with ${AHSpell[${a}]}]}
       /multiline ; /varset AHtDPS 1;/varset AHtDPSAvr 1;/varset AHtPctCntr 1;/varset AHtPreHP ${Spawn[${tTargetID}].PctHPs};/if (${tTargetID} && ${tTargetID}<9999) /varset AHtPreID ${tTargetID};/varset AHttl 100000;/varset AHtPriority ${tPriority}
     	/if (${AHtCnt}==${PRSpell}) /multiline ; /varset tPRSpell TRUE; /return
     }
     /multiline ; /varset tHurt 0;/varset tMembers
/return

Sub CalcPPS(int tSpellID)
/if (${AHtTargetID} && !${Spawn[${AHtTargetID}].Type.Equal[corpse]}) {
   	/if (!${AHtPreID}) /varset AHtPreID ${AHtTargetID}
| Calculate mob DPS and find the time-to-live to compare to cast time.
|          && ${AHtTimer} && ${AHtTimer}<${AHtTime} && ${AHtPreHP}>${Target.PctHPs}) {
    /if (${Target.ID}!=${AHtPreID} && !${Spawn[group ${AHtPreID}].ID} && ${AHtPreID} && ${Spawn[${AHtPreID}].ID}) /target id ${AHtPreID}
       /delay 5 (${Spawn[group ${AHtPreID}].ID}||${Target.ID}==${AHtPreID})
       /if (!${Spawn[group ${AHtPreID}].ID} && ${AHtPreHP}>${Target.PctHPs} && ${Target.PctHPs}!=100) {
       	/varcalc AHtDPS ${AHtDPS}+(${AHtPreHP}-${Target.PctHPs})
		} else {
     	/if (${Spawn[group ${AHtPreID}].ID} && ${AHtPreHP}>${Spawn[group ${AHtPreID}].PctHPs} && ${Spawn[group ${AHtPreID}].PctHPs}!=100) /varcalc AHtDPS ${AHtDPS}+(${AHtPreHP}-${Spawn[group ${AHtPreID}].PctHPs})
    }
    /varcalc AHtPctCntr ${AHtDPS}/((1001-${AHtTimer})/10)
    /if (${AHtTimer}<900) {
     /varcalc AHtDPS ${AHtPctCntr}
  	  /varcalc AHtTimer 1000
  	 }
     /if (!${AHtPctCntr}) {
     	/varset AHttl 10000
     } else {
     	/varcalc AHttl (${Spawn[${AHtPreID}].PctHPs}/${AHtPctCntr})*100
     }
     /varset AHtPreHP ${Target.PctHPs}
|     /echo Healing! CastTimer: ${CastTimer} TTL: ${AHttl}  == ${Math.Calc[${CastTimer}*10]}>${AHttl} ? (${CastTimer}>(${Math.Calc[${Spell[${tSpellID}].MyCastTime}*10]})
}
/return


sub event_AHDA
 /multiline ; /docommand /${BeepCommand};/bc DA on ME;/echo DA on ME
/return

sub CheckAHArgs(string AHLine,string Class)

/if (!${Defined[Class]}) /declare Class string local ${AHLine.Arg[2,|]}
/varset AHLine string local ${AHLine.Arg[1,|]}
/declare ParseLine string local ${AHLine.Arg[1,|]}
/declare ClassChk string local ${Class}
	/if (${AHLine.Find[${ClassChk}]}) {
| Find if class shortname has a value, if not - use hp setting after checking spwntype..
  /varset ParseLine ${AHLine}
:tRecheck
|  /echo ${ClassChk} - Found at ${AHLine.Find[${ClassChk}]} - ${AHLine.Right[-${Math.Calc[${AHLine.Find[${ClassChk}]}-1]}]}
		/if (${AHLine.Arg[1].Find[${ClassChk}]}) {
			/varset ParseLine ${AHLine.Arg[1]}
|			/echo ParseLine = ${ParseLine.Arg[1]}
		} else {		
			/varset ParseLine ${AHLine.Right[-${Math.Calc[${AHLine.Find[${ClassChk}]}-1]}]}
| 			/echo ${ClassChk} - Found at ${AHLine.Find[${ClassChk}]} - ${AHLine.Right[-${Math.Calc[${AHLine.Find[${ClassChk}]}-1]}]}
		}		
		/if (${AHLine.Find[hp]} && ${ClassChk.NotEqual[hp]} && (!${ParseLine.Arg[1].Find[${ClassChk}]} || ${ParseLine.Arg[1].Length}==${Class.Length} || ${ParseLine.Arg[1].Equal[NULL]})) {
			/varset ClassChk hp
			/goto :tRecheck
		}
|		/echo ${ParseLine.Arg[1]}
		/if (${ParseLine.Arg[1].Length}!=${ClassChk.Length}) /call IsNumber ${ParseLine.Arg[1].Right[-${ClassChk.Length}]}
		/if (${Macro.Return.Length} && ${Macro.Return.NotEqual[FALSE]}) /return ${ParseLine.Arg[1].Right[-${ClassChk.Length}]}
|		/if (${Macro.Return.Equal[FALSE]}) /echo False Return from ${ParseLine.Arg[1]}
	}
| 	/echo Problem determining heal percentage for Class ${Class} in AHClass line of ${AHLine}
|  /if (${AHLine.Find[mypet]}) /echo ${AHLine} - ${ParseLine} - ${ParseLine.Arg[1].Right[-${ClassChk.Length}]}	
/return FALSE

sub GrpEval(string AHLine,int radius)
| Check self then tank and grp HP, then look for netbots clients out of group and pets (use Netbots only if out of group). return string of ID's with count as last arg.
/declare i int local
/declare x int local
/declare matches string local
/declare totalcount int local 0
/declare GrpLowest int local
/declare NBLowest int local
/declare Worst int local
/declare ReturnArgL string local
/declare ReturnArgR string local
/declare TankID int local
	/if (${AHLine.Find[self]}) {
		  /call CheckAHArgs "${AHLine}"|self
		  /if (${Macro.Return}>=${Me.PctHPs}) /return "${Me.ID}|1"
	}
 	/if (${AHLine.Find[${Me.Class.ShortName}]} && ${AHLine.Find[pc]}) {
 		/if (${AHLine.Find[${Me.Class.ShortName}]}) /call CheckAHArgs "${AHLine}"|${Me.Class.ShortName}
		/if (${Macro.Return}>=${Me.PctHPs}) /varset matches ${Me.ID}
 	}
	/if (${AHLine.Find[pc]} && ${AHLine.Find[tank]} && (${Spawn[pc ${TankName}].ID} && ${Spawn[pc ${TankName}].PctHPs} || ${Spawn[mercenary ${TankName}].ID} && ${Spawn[mercenary ${TankName}].PctHPs})) {
		/docommand ${If[${Spawn[mercenary ${TankName}].ID},/varset TankID ${Spawn[mercenary ${TankName}].ID},/varset TankID ${Spawn[pc ${TankName}].ID}]}
		/if (${radius} && ${Spawn[${TankID}].Distance}>${radius}) /return FALSE
		/if (${Group.Member[${TankName}]} || !${NetBots[${TankName}].ID}) {
			/call CheckAHArgs "${AHLine}"|tank
			/if (${Macro.Return} && ${Macro.Return}>${Spawn[${TankID}].PctHPs}) /return "${TankID}|1"
		} else {
			/call CheckAHArgs "${AHLine}"|tank
			/if (${Macro.Return} && ${Macro.Return}>=${NetBots[${TankName}].PctHPs}) /return "${TankID}|1"
		}
	}
		/for x 1 to ${Math.Calc[${NetBots.Client.Count[ ]}+1]}
		/if (!${Spawn[group ${NetBots.Client.Arg[${x}]}].ID} && !${AHLine.Find[group]} && ${Spawn[pc ${NetBots.Client.Arg[${x}]}].PctHPs} && !${AHLine.Find[Self]} && !${AHLine.Find[mypet]} && !${Spawn[${NetBots.Client.Arg[${x}]}].Type.Equal[corpse]}) {
			/if (${radius} && ${Spawn[pc ${NetBots.Client.Arg[${x}]}].Distance}>${radius} || ${Spawn[${NetBots[${NetBots.Client.Arg[${x}]}].ID}].Type.Equal[corpse]}) /goto :tNextNB
			/if (${GrpLowest}) /varset NBLowest ${GrpLowest}
			/if (${AHLine.Find[${Spawn[${NetBots[${NetBots.Client.Arg[${x}]}].ID}].Class.ShortName}]}) {
				/call CheckAHArgs "${AHLine}"|${Spawn[${NetBots[${NetBots.Client.Arg[${x}]}].ID}].Class.ShortName}
				/if (${Macro.Return} && ${Macro.Return}>=${NetBots[${NetBots.Client.Arg[${x}]}].PctHPs} && ${AHLine.Find[pc]}) {
					/if (!${NBLowest} || ${NetBots[${NetBots.Client.Arg[${x}]}].PctHPs}<${NBLowest}) /multiline ; /varset NBLowest ${NetBots[${NetBots.Client.Arg[${x}]}].PctHPs};/varset Worst ${NetBots[${NetBots.Client.Arg[${x}]}].ID}
					/multiline ; /docommand ${If[${NetBots[${NetBots.Client.Arg[${x}]}].PctHPs}==${NBLowest} && ${matches.Arg[1].NotEqual[${Spawn[${TankName}].ID}]},/varset matches ${NetBots[${NetBots.Client.Arg[${x}]}].ID} ${matches},/varset matches ${matches} ${NetBots[${NetBots.Client.Arg[${x}]}].ID}]};/varset totalcount ${Math.Calc[${totalcount}+1]}
| 					/echo - NetBots heal needed - ${NetBots.Client.Arg[${x}]}
				}
			}
			/if (${NetBots[${NetBots.Client.Arg[${x}]}].PetID} && ${AHLine.Find[pet]} && ${Spawn[${NetBots[${NetBots.Client.Arg[${x}]}].PetID}].Distance}<${Radius} && ${Select[${Spawn[${NetBots[${NetBots.Client.Arg[${x}]}].ID}].Class.ShortName},shm,nec,mag,bst,dru,enc,shd]}) {
	  			/call CheckAHArgs "${AHLine}"|pet
	  			/if (${Macro.Return} && ${Macro.Return}>=${NetBots[${NetBots.Client.Arg[${x}]}].PetHP}) /multiline ; /varset matches ${matches} ${NetBots[${NetBots.Client.Arg[${x}]}].PetID};/varset totalcount ${Math.Calc[${totalcount}+1]}
	  	}
		}
:tNextNB		
	/next x
	
:grpcheck	
	/if (${Group.Members}>0 && !${AHLine.Find[Self]} && !${AHLine.Find[mypet]}) {
		/for i 0 to ${Group.Members}
			/if (${Spawn[${Group.Member[${i}].ID}].ID} && ${Group.Member[${i}].PctHPs} && !${Spawn[${Group.Member[${i}].ID}].Type.Equal[corpse]}) {
			/if (${AHLine.Find[${Group.Member[${i}].Class.ShortName}]} && ${radius} && ${Group.Member[${i}].Distance}>${radius} || ${Group.Member[${i}].Type.Equal[corpse]}) /goto :tNextGrp
	  		/call CheckAHArgs "${AHLine}"|${Group.Member[${i}].Class.ShortName}
	  		/if (${Macro.Return} && ${Macro.Return}>=${Group.Member[${i}].PctHPs} && ${AHLine.Find[pc]} && (!${Worst} || ${Worst}!=${Spawn[${TankName}].ID})) {
	  			/if (!${GrpLowest} || ${Group.Member[${i}].PctHPs}<${GrpLowest}) /multiline ; /varset GrpLowest ${Group.Member[${i}].PctHPs};/varset Worst ${Group.Member[${i}].ID}
	  			/multiline ; /docommand ${If[${Group.Member[${i}].ID}==${Worst} && ${matches.Arg[1].NotEqual[${Spawn[${TankName}].ID}]},/varset matches ${Group.Member[${i}].ID} ${matches},/varset matches ${matches} ${Group.Member[${i}].ID}]};/varset totalcount ${Math.Calc[${totalcount}+1]}
	  		}
	  	}
  		/if (${Group.Member[${i}].Pet.ID} && ${AHLine.Find[pet]} && ${Select[${Group.Member[${i}].Class.ShortName},shm,nec,mag,bst,dru,enc,shd]} && !${Group.Member[${i}].Pet.Name.Find[familiar]}) {
  			/call CheckAHArgs "${AHLine}"|Pet
  			/if (${Macro.Return} && ${Macro.Return}>=${Group.Member[${i}].Pet.PctHPs}) /multiline ; /varset matches ${matches} ${Group.Member[${i}].Pet.ID};/varset totalcount ${Math.Calc[${totalcount}+1]}
  		}
:tNextGrp	  	
		/next i
	}
| Check NetBots members only out of group since group check already done.

	/if (${AHLine.Find[mypet]} && ${Me.Pet.Distance}<${radius} && ${Me.Pet.ID}) {
		/call CheckAHArgs "${AHLine}"|mypet
		/if (${Macro.Return}>=${Me.Pet.PctHPs}) /multiline ; /varset matches ${matches} ${Me.Pet.ID};/varset totalcount ${Math.Calc[${totalcount}+1]}
	}
/if (${matches.Length} && ${totalcount}) /return "${matches}|${totalcount}"
/return FALSE
