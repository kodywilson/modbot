
#event OUTDOORS "#*#You can only cast this spell in the outdoors#*#"
#event OUTDOORS "#*#You can't levitate in this zone#*#"

Sub LoadABINIVals
/declare a int local
/call LoadINIVar AdvBuff ABCount 0 ABCount "${IniFile}" int
/if (${ABCount}) {
   /call LoadINIVar AdvBuff ABMobMax 18 ABMobMax "${IniFile}" int
   /call LoadINIVar AdvBuff ABCheckTime 8 ABCheckTime "${IniFile}" int
   /declare DoBuffsCBT bool outer FALSE
   /declare ABMBL[${ABMobMax}] string outer
   /declare ABCheckTimer timer outer
   /declare ABGem[${ABCount}] string outer
   /declare ABSpell[${ABCount}] string outer
   /declare ABSpellFoci[${ABCount}] string outer
   /declare ABDurMod[${ABCount}] string outer
   /declare ABSpellMinMana[${ABCount}] int outer
   /declare ABTarCnt[${ABCount}] int outer
   /declare ABTarType[${ABCount}] string outer
   /declare ABSpellAlias[${ABCount}] string outer
   /declare ABAnnounce[${ABCount}] string outer
   /declare ABScriptTime[${ABCount}] int outer
   /declare ABRecast[${ABCount}] bool outer
   /declare ABSpellIcon[${ABCount}] string outer
   /declare ABPreCondition[${ABCount}] string outer
   /declare ABAliasList string outer
   /declare tSpellID int local
   /declare ABGemInUse string outer
   /declare SMPetCast bool outer FALSE
   /declare OutDoorsFail bool outer FALSE
   /for a 1 to ${ABCount}
     /call LoadINIVar AdvBuff ABGem${a} " " ABGem[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABSpell${a} " " ABSpell[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABSpellFoci${a} " " ABSpellFoci[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABDurMod${a} 0 ABDurMod[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABSpellAlias${a} " " ABSpellAlias[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABAnnounce${a} " " ABAnnounce[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABSpellMinMana${a} 0 ABSpellMinMana[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABTarCnt${a} 0 ABTarCnt[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABTarType${a} "tank war shd pal rng mnk rog brd bst ber shm clr dru wiz mag enc nec self mypet grp pet cbt idle" ABTarType[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABRecast${a} FALSE ABRecast[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABSpellIcon${a} " " ABSpellIcon[${a}] ${IniFile}
     /call LoadINIVar AdvBuff ABPreCondition${a} "TRUE" ABPreCondition[${a}] ${IniFile}
     /if (${ABSpellIcon[${a}].Length}>1 && !${Spell[${ABSpellIcon[${a}]}].ID}) /echo ABSpellIcon${a} does not evaluate to a spell name.  Please check your spelling.
     /if (${ABTarType[${a}].Find[cbt]}) /varset DoBuffsCBT TRUE
     /varset ABAliasList ${ABAliasList} ${ABSpellAlias[${a}]}|
     /if (${Select[${ABGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]} && ${ABSpell[${a}].Length}) {
  	 /docommand ${If[${Me.Book[${Me.Book[${ABSpell[${a}]} Rk. III]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${ABSpell[${a}]} Rk. III]}].ID},${If[${Me.Book[${Me.Book[${ABSpell[${a}]} Rk. II]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${ABSpell[${a}]} Rk. II]}].ID},${If[${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID},/bc Spell ${ABSpell[${a}]} Not found in your book.  Check your .ini settings.]}]}]}
  	 /if (${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID}!=${tSpellID}) /varset ABSpell[${a}] ${Spell[${tSpellID}].Name}
|  	     /echo ${tSpellID} - ${ABSpell[${a}]}
     }
     /if ((${Spell[${tSpellID}].TargetType.Find[Group]} || ${ABTarType[${a}].Find[self]}) && ${ABRecast[${a}]}) /varset ABRecast[${a}] FALSE
   /next a
}
/return

Sub ABBuffCheck
  /varset ABCheckTimer ${ABCheckTime}s
  /if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:ABBuffCheck
  /declare a int local
  /declare b int local
  /declare c int local
  /declare d int local
  /declare x int local
  /declare tCT int local
  /declare sVar string local
  /declare sVarL string local
  /declare sVarM string local
  /declare sVarR string local
  /declare sVarT string local
  /declare sVarSpell string local
  /declare tSpellSID string local
  /declare tTime int local
  /declare tSpellID int local
  /declare tGem int local
  /declare tSpellRange int local
  /declare tSpellDuration int local
  /declare tSpellDelay int local
  /declare tCastTime timer local
  /declare ABtTargetID int local
  /declare tTarCnt int local
  /declare tTargetID int local
  /declare tName string local
  /declare tABMob int local
  /declare tPet bool local FALSE
  /declare tBot bool local TRUE
  /declare tNBCnt int local ${NetBots.Counts}
  /declare tCnt int local ${NetBots.Counts}
  
  /declare zPetID int local
  /declare zID int local
  /declare zGrouped bool local
  /declare zShortName string local
  /declare tBuffs string local
  /declare tgtLevel int local
  /call ADSpawnCheck
  /declare sNBClnt string local 
  /for b 1 to ${NetBots.Counts}
     /varset sNBClnt ${sNBClnt}${NetBots.Client.Arg[${b}]}|
  /next b
  /if (${Group.Members}) {
     /for b 0 to ${Group.Members}
        /if (!${NetBots[${Group.Member[${b}].Name}].ID} && ${Spawn[${Group.Member[${b}].Name}].ID}) {
	    /varset sNBClnt ${sNBClnt}${Group.Member[${b}].Name}|
	    /varcalc tCnt ${tCnt}+1
	}
	/next b
     }
|/echo ${sNBClnt}
  /for a 1 to ${ABCount}
     /if (${FollowFlag} && (${Spawn[FollowID].Distance}>${FollowDistance} || ${Me.Moving})) /return
     /if (${Cursor.ID}) /call ClearCursor
     /if (!${ABTarCnt[${a}]}) /goto :tNextBuff
     /if (${ABSpell[${a}].Left[5].Equal[Yaulp]} && ${Me.Mount.ID}) /goto :tNextBuff
     /multiline ; /varset tGem 0;/varset tSpellID 0;/varset tTarCnt 0
     /if (${Select[${ABGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]}) {
|/echo was Spell
        /multiline ; /varset tSpellID ${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID};/varset tGem ${ABGem[${a}]};/varcalc tCT ${Spell[${tSpellID}].MyCastTime}*10+40
        /if (${Me.CurrentMana}+${Me.ManaRegen}<${Spell[${tSpellID}].Mana} || ${Me.PctMana}<${ABSpellMinMana[${a}]} || ${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Find[${tSpellID}|]}) /goto :tNextBuff
        /if (${Me.Class.ShortName.Equal[BRD]} && ${ABTarCnt[${a}]}>0) {
      	   /if (${ABTarType[${a}].Find[aura]}) {
      	      /if (!${Me.Aura[${ABSpell[${a}]}].Length} && ${ABSpellAlias[${a}].Length}) /call CastCall ${Me.CleanName} "cast ${ABSpellAlias[${a}]}"
      	      /goto :tNextBuff
      	   }
           /if (${ABTarCnt[${a}]}>1 && ${Math.Calc[${Group.Members}+1]}<${ABTarCnt[${a}]}) /goto :tNextBuff
           /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
              /delay 2s ${Cast.Status.Equal[i]}
              /squelch /memorize "${Spell[${tSpellID}]}|gem${tGem}"
              /delay 14s ${Cast.Status.Equal[i]}
              /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) /multiline ; /bc BUFF - ${ABSpell[${a}]} - FAILED MEMORIZE;/goto :tNextBuff
              /varcalc tCT ${Spell[${tSpellID}].RecastTime}*10+${tCT}
           }
           /if (${ABTarType[${a}].Find[cbt]} && !${ABTarType[${a}].Find[idle]}) {
              /if (!${BardCombatGems.Find[${tGem}]}) /varset BardCombatGems ${BardCombatGems} ${tGem}
              /if (!${ADMobCount}) /goto :tNextBuff
           } else /if (${ABTarType[${a}].Find[cbt]} && ${ABTarType[${a}].Find[idle]}) {
              /if (!${BardCombatGems.Find[${tGem}]}) /varset BardCombatGems ${BardCombatGems} ${tGem}
              /if (!${BardRestGems.Find[${tGem}]}) /varset BardRestGems ${BardRestGems} ${tGem}
           } else /if (!${ABTarType[${a}].Find[cbt]}) {
              /if (!${BardRestGems.Find[${tGem}]}) /varset BardRestGems ${BardRestGems} ${tGem}
              /if (${ADMobCount}) /goto :tNextBuff
           }
           /if (${BardRestGems.Find[${tGem}]} || ${BardCombatGems.Find[${tGem}]}) /goto :tNextBuff
           /if (${ABAnnounce[${a}].Length}) {
              /if(${ABAnnounce[${a}].Length}>=5) {
                 /call ParseAnnounce "${ABAnnounce[${a}]}" "${Me.Class}" "${ABSpell[${a}]}"
              } else {
                 /docommand ${ABAnnounce[${a}]} Buffed: -[ ${Me.Class} ]- with ${ABSpell[${a}]}]}
                 /goto :tNextBuff
              }
           }
           /if (${Twist}) /squelch /stoptwist
        }
     } else /if (${ABGem[${a}].Find[item]} && ${Bool[${FindItem[=${ABSpell[${a}]}]}]}) {
|/echo was Item
        /if (${FindItem[=${ABSpell[${a}]}].Timer}) /goto :tNextBuff
        /multiline ; /varset tSpellID ${FindItem[=${ABSpell[${a}]}].Spell.ID};/varset tGem 20;/varcalc tCT ${FindItem[=${ABSpell[${a}]}].CastTime}*10+40
     } else /if (${ABGem[${a}].Equal[alt]} && ${Me.AltAbility[${ABSpell[${a}]}].ID}) {
|/echo was Alt
        /if (!${Me.AltAbilityReady[${ABSpell[${a}]}]} || ${Me.CurrentMana}<${Me.AltAbility[${ABSpell[${a}]}].Spell.Mana}) /goto :tNextBuff
        /multiline ; /varset tSpellID ${Me.AltAbility[${ABSpell[${a}]}].Spell.ID};/varset tGem 30;/varcalc tCT ${Me.AltAbility[${ABSpell[${a}]}].Spell.CastTime}*10+40 
     } else /if (${ABGem[${a}].Equal[potionbelt]}) {
|/echo was potionbelt
        /if (${FindItem[=${ABSpell[${a}]}].Timer} || !${FindItem[=${ABSpell[${a}]}].ID} || ${FindItem[=${ABSpell[${a}]}].Spell.Duration} && !${FindItem[=${ABSpell[${a}]}].Spell.Stacks[0]}) /goto :tNextBuff
     	/for x 0 to 4
	   /docommand ${If[${Window[PotionBeltWnd].Child[PW_PotionSlot${x}_Button].Tooltip.Equal[${ABSpell[${a}]}]},/potionbelt Activate ${Math.Calc[${x}+1]},/next x]}
        /goto :tNextBuff
     }

     /if ((${ABTarType[${a}].Find[petspell]} || ${ABTarType[${a}].Find[petcast]}) && !${Me.Pet.ID} && ${DoPet} && ${ABTarCnt[${a}]}) {
    	/if (${ADMobCount} && !${ABTarType[${a}].Find[cbt]}) /goto :tNextBuff
/echo Casting pet.
	/varset MBCurrentSpell AB|${a}
    	/if (${Me.AltAbility[Persistent Minion]} && !${SMPetCast}) /multiline ; /varset SMPetCast TRUE;/call MQ2Cast "Suspended Minion" alt 3s CastCheck
    	/delay 1s ${Me.Pet.ID}
    	/if (!${Me.Pet.ID}) /call MQ2Cast "${ABSpell[${a}]}" ${If[${Select[${ABGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]},gem,]}${ABGem[${a}]} 5s CastCheck
    	/delay 1s ${Me.Pet.ID}
    	/goto :tNextBuff
     }
     /if (${ABSpellIcon[${a}].Length}>1) /multiline ; /varset tSpellID ${Spell[${ABSpellIcon[${a}]}].ID};/if (!${Spell[${ABSpellIcon[${a}]}].ID}) /goto :tNextBuff
     /multiline ; /varset tSpellSID ${tSpellID};/docommand ${If[${Spell[${tSpellID}].AERange},/varset tSpellRange ${Spell[${tSpellID}].AERange},/varset tSpellRange ${Spell[${tSpellID}].Range}]}
     /for b 1 to ${tCnt}
        /if (${b}>${tNBCnt}) {
           /varset tBot FALSE
           /varset tName ${sNBClnt.Arg[${b},|]}
           /multiline ; /varset zID ${Group.Member[${tName}].ID};/varset zPetID ${Group.Member[${tName}].Pet.ID}
           /if (!${zID} && !${zPetID} && ${Spawn[group ${tName}].ID}) /varset zID ${Spawn[group ${tName}].ID}
           /multiline ; /varset zGrouped TRUE;/varset zShortName ${Group.Member[${tName}].Class.ShortName}
        } else {
           /varset tBot TRUE
           /varset tName ${sNBClnt.Arg[${b},|]}
           /multiline ; /varset zID ${NetBots[${tName}].ID};/varset zPetID ${NetBots[${tName}].PetID}
           /multiline ; /varset zGrouped ${NetBots[${sNBClnt.Arg[${b},|]}].Grouped};/varset zShortName ${Spawn[${zID}].Class.ShortName}
        }
        /varset tPet FALSE
:tFoundPet
        /if (${FollowFlag} && (${Spawn[FollowID].Distance}>${FollowDistance} || ${Me.Moving})) /return
        /if (!${DoBuffs} || ${MBPause} || ${Me.State.Equal[feign]}) /return
        /if (${tPet} && ${zPetID}) {
           /varset tTargetID ${zPetID}
           /varset tName ${Spawn[${tTargetID}].CleanName}
           /varset tPet FALSE
        } else {
           /varset tName ${sNBClnt.Arg[${b},|]}
           /varset tTargetID ${zID}
           /if (${zPetID} && ${Select[${zShortName},shm,nec,mag,bst,enc,shd]}) /varset tPet TRUE
        }
        /for c 1 to ${ABMobMax}
           /if (${tName.Equal[${ABMBL[${c}].Arg[1,|]}]}) /multiline ; /varset tABMob ${c};/goto :tFoundBot
           /if (!${ABMBL[${c}].Length}) /multiline ; /varset ABMBL[${c}] ${tName}|;/varset tABMob ${c};/goto :tFoundBot
        /next c
        /for c 1 to ${ABMobMax}
           /if (${ABMBL[${c}].Length} && !${Spawn[${ABMBL[${c}].Arg[1,|]}].ID}) /multiline ; /varset ABMBL[${c}]  ;/varset tPet FALSE;/varset tName DoneScrubbing
        /next c
        /if (${tName.Equal[donescrubbing]}) /goto :tFoundPet
        /bc CAN NOT TRACK ANYMORE
        /return
:tFoundBot
|/echo 218 ABSpell - ${ABSpell[${a}]}  tTargetID - ${tTargetID}
	/if (${ABPreCondition[${a}].Length}>4 && ${ABPreCondition[${a}].Left[4].NotEqual[TRUE]}) {
	   /call IniCommand "${ABPreCondition[${a}]}" ${tTargetID}
	   /if (${Macro.Return.Equal[FALSE]}) /goto :tNextBot
	}
        /if (${ADMobCount} && ((${DoMelee} && !${ACState}) || !${DoBuffsCBT})) /return
        /if ((!${ABTarType[${a}].Find[cbt]} && ${ADMobCount}) || (${ABTarType[${a}].Find[cbt]} && !${ABTarType[${a}].Find[idle]} && !${ADMobCount})) /goto :tNextBuff
        /if (${ABTarCnt[${a}]}>1) {
           /if (${tTargetID}!=${Me.ID} && !${ABTarType[${a}].Find[raid]} || ${ABTarType[${a}].Find[raid]} && !${NetBots[${Spawn[${tTargetID}].Name}].Grouped} || ${NetBots[${Spawn[${tTargetID}].Name}].Buff.Find[${tSpellID}]} || ${ABTarType[${a}].Find[raid]} && !${ABTarType[${a}].Find[${Spawn[${tTargetID}].Class.ShortName}]} || !${NetBots[${Spawn[${tTargetID}].Name}].FreeBuffSlots}) /goto :tNextBot
           /call StacksCheck ${tSpellID} "${NetBots[${Spawn[${tTargetID}].Name}].Buff}"
           /if (${Macro.Return.Equal[FALSE]}) /goto :tNextBot
|/echo ${tSpellID} - ${tTargetID} - "${NetBots[${Spawn[${tTargetID}].Name}].Buff}"
           /varset tTarCnt 0
           /for d 1 to ${tCnt}
              /if (${d}<=${tNBCnt}) {
                 /if (${NetBots[${sNBClnt.Arg[${d},|]}].Grouped} && (${ABTarType[${a}].Find[${NetBots[${sNBClnt.Arg[${d},|]}].Class.ShortName}]} || ${ABTarType[${a}].Find[self]}) && ${Spawn[radius ${tSpellRange} PC ${sNBClnt.Arg[${d},|]}].ID}) /varcalc tTarCnt ${tTarCnt}+1
              } else /if (${Spawn[${sNBClnt.Arg[${d},|]}].ID}) {
                 /if (${Spawn[radius ${tSpellRange} ${sNBClnt.Arg[${d},|]}].ID} && (${ABTarType[${a}].Find[${Spawn[${sNBClnt.Arg[${d},|]}].Class.ShortName}]} || ${ABTarType[${a}].Find[self]})) /varcalc tTarCnt ${tTarCnt}+1
              }
           /next d
           /if (${tTarCnt}<${ABTarCnt[${a}]}) /goto :tNextBuff
        }
        /if (${ABTarType[${a}].Find[tank]}) {
           /if (${TankName.NotEqual[${Spawn[${tName}].CleanName}]}) /goto :tNextBot
           /goto :tSetsVar
        }
        /if (${ABTarType[${a}].Find[aura]}) {
     	   /if (!${Me.Class.ShortName.Equal[brd]} && !${Cast.Status.Equal[I]}) /delay 1s ${Cast.Status.Equal[I]}
     	   /if (!${ABTarType[${a}].Find[self]} && !${ABTarType[${a}].Find[mypet]}) /goto :tNextBuff
           /if (${ABGem[${a}].NotEqual[disc]} && ${ABTarType[${a}].Find[self]} && !${ABSpell[${a}].Find[${Me.Aura[1]}]} && !${ABSpell[${a}].Find[${Me.Aura[2]}]} && (!${Spell[${Me.Aura}].ID} || (${Me.AltAbility[Auroria Mastery]} || ${Me.AltAbility[Spirit Mastery]}) && (!${Spell[${Me.Aura[1]}].ID} || !${Spell[${Me.Aura[2]}].ID}))) /goto :tSetsVar
           /if (${Me.Class.ShortName.Equal[mag]} && ${ABTarType[${a}].Find[mypet]} && ${Me.Pet.ID} && !${Me.PetBuff[${ABSpell[${a}]} Effect]} && ${Spell[${ABSpell[${a}]} Effect].StacksPet}) /multiline ; /varset tName ${Me.Pet.CleanName};/varset tTargetID ${Me.Pet.ID};/goto :tSetsVar
           /if (${ABGem[${a}].Equal[disc]} && !${Spell[${Me.Aura}].ID}) {
              /disc ${ABSpell[${a}]}
              /delay 1s ${Me.Casting.ID}
              /delay ${Spell[${ABSpell[${a}]}].MyCastTime}s !${Me.Casting.ID}
           }
           /goto :tNextBuff
        }
        /if ((${Spawn[${tTargetID}].Type.Equal[pet]} && ((${ABTarType[${a}].Find[ pet]} || ${ABTarType[${a}].Left[3].Equal[pet]}) && ${Spawn[${tTargetID}].Master.ID}!=${Me.ID} || ${ABTarType[${a}].Find[mypet]} && ${Spawn[${tTargetID}].Master.ID}==${Me.ID}) || ((${Spawn[group id ${tTargetID}].ID} || ${Spawn[raid id ${tTargetID}].ID} || ${NetBots[${tName}].ID}) && ${Spawn[${tTargetID}].Type.NotEqual[pet]}) && ${ABTarType[${a}].Find[${Spawn[${tTargetID}].Class.ShortName}]} || ${ABTarType[${a}].Find[self]} && ${tTargetID}==${Me.ID}) && ((${ABTarType[${a}].Find[grp]} && (${Group.Member[${Spawn[${tTargetID}].Master}].Index} || ${Spawn[${tTargetID}].Master.ID}==${Me.ID} && ${Me.Grouped} || ${Group.Member[${tName}].Index} || ${tName.Equal[${Me}]} && ${Me.Grouped})) || (!${ABTarType[${a}].Find[grp]} && (!${Group.Member[${Spawn[${tTargetID}].Master}].Index} || ${Spawn[${tTargetID}].Master.ID}==${Me.ID} && !${Me.Grouped} || !${Group.Member[${tName}].Index} || ${tTargetID}==${Me.ID} && !${Me.Grouped} || ${ABTarType[${a}].Find[oog]} && !${Group.Member[${tName}].Index})))) /goto :tSetSVar
        /docommand ${If[${tPet},/goto :tFoundPet,/goto :tNextBot]}
:tSetsVar
        /if (${ABGem[${a}].Equal[script]}) {
|/echo was Script
           /if (${ABScriptTime[${a}]}<=${Macro.RunTime}) {
              /call MBScript ${ABSpell[${a}]} "${tName}" ${tTargetID} ${a}
              /varset ABScriptTime[${a}] ${Math.Calc[${ABDurMod[${a}]}+${Macro.RunTime}]}
           }
           /goto :tNextBot
        }
        /varset sVar ${ABMBL[${tABMob}].Mid[${ABMBL[${tABMob}].Find[b${tSpellSID}]}|,14]}
|/multiline ; /varset sVarT ${sVar.Arg[2,|]};/varset tTime ${sVarT.Mid[2,${sVarT.Length}]}
        /varset sVarT ${sVar.Arg[2,|]}
        /varset tTime ${sVarT.Mid[2,${sVarT.Length}]}
        /if (!${DoBuffs} || !${ABTarCnt[${a}]} || !${ABTarType[${a}].Find[cbt]} && ${ADMobCount} || (${ABTarType[${a}].Find[cbt]} && !${ABTarType[${a}].Find[idle]} && !${NetBots[${TankName}].Attacking} && !${ADMobCount}) || ${Me.CurrentMana}<${Spell[${tSpellID}].Mana} && ${tGem}<20 || ${Me.PctMana}<${ABSpellMinMana[${a}]} || ${Spell[${tSpellID}].TargetType.Find[group]} && ${Me.GroupSize}<${ABTarCnt[${a}]} && ${ABTarCnt[${a}]}>1) /goto :tNextBuff
        /if (${Select[${Spell[${tSpellID}].TargetType},self,single]} && (${tTargetID}==${Me.ID} || ${ABTarType[${a}].Find[self]}) && (${Me.Buff[${Spell[${tSpellID}].Name}].Duration}>6 || !${Me.Buff[${Spell[${tSpellID}]}].ID} && !${Me.FreeBuffSlots} || !${Spell[${tSpellID}].Stacks[0]})) /goto :tNextBot
        /varset tgtLevel ${If[!${Spawn[${tTargetID}].Type.Equal[pet]},${Spawn[${tTargetID}].Level},${Spawn[${tTargetID}].Master.Level}]}
        /if ((${Spell[${tSpellID}].Level}==255 && ${Spell[${ABSpellIcon[${a}]}].ID}==${tSpellID})) /goto :tNextBot
        /varset tBuffs ${If[${Spawn[${tTargetID}].Type.Equal[pc]},${NetBots[${tName}].Buff} ${NetBots[${tName}].ShortBuff},${NetBots[${Spawn[${tTargetID}].Master.CleanName}].PetBuff}]}
|    /if (${tBot} && !${Spell[${tSpellID}].TargetType.Find[group]}) {
     /if (${tBot}) { 
        /if (${Spawn[${tTargetID}].Type.Equal[pc]} && (!${tBuffs.Find[${tSpellID}]} && !${NetBots[${tName}].FreeBuffSlots})) /goto :tNextBot
        /for x 1 to ${tBuffs.Count[ ]}+1
           /if (${tBuffs.Arg[${x}, ].Equal[${tSpellID}]} && !${tTime}) /goto :tNextBot
           /if (${tBuffs.Arg[${x}, ].NotEqual[${tSpellID}]} && !${tBuffs.Find[${tSpellID}]}) {
              /docommand ${If[${Spell[${tBuffs.Arg[${x}, ]}].WillStack[${tSpellID}]} && (${ABTarType[${a}].Find[${Spawn[${tTargetID}].Class.ShortName}]} || (${ABTarType[${a}].Find[self]} && ${tTargetID}==${Me.ID}) || (${ABTarType[${a}].Find[pet]} && ${Spawn[${tTargetID}].Master.ID}) || ${ABTarType[${a}].Find[tank]}),/varset tTime 0,/goto :tNextBot]}
           }
           /if (${Spawn[${tTargetID}].Type.Equal[pet]} && ${tBuffs.Find[${tSpellID}]}) /goto :tNextBot
        /next x
     }
|   } else /if (${tBot} && ${Spell[${tSpellID}].TargetType.Find[group]} && (${ABTarType[${a}].Find[grp]} || ${ABTarCnt[${a}]}>1 )) {
|   	/if (${Group.Members}<${ABTarCnt[${a}]}) /goto :tNextBuff
|   	/for x 0 to ${Group.Members}
|   		/varset tBuffs ${NetBots[${Group.Member[${x}].Buff}
|   		/if (${Group.Member[${x}].Type.Equal[pc]} && !${tBuffs.Find[${tSpellID}]} && !${NetBots[${Group.Member[${x}]}].FreeBuffSlots}) /next x
|   		/if (!${tBuffs.Find[${tSpellID}]} && ${Spawn[pc ${Group.Member[${x}]}].ID} && ${Group.Member[${x}].Distance}<${tSpellRange} || !${NetBots[${Group.Member[${x}].ID}) /varset tTime 0
|   	  /if (${tBuffs.Find[${tSpellID}]} && ${Spawn[pc ${Group.Member[${x}]}].ID} && ${Group.Member[${x}].Distance}<${tSpellRange} && ${NetBots[${Group.Member[${x}]}].Duration[${NetBots[${Group.Member[${x}]}].Buff.Left[${NetBots[${Group.Member[${x}]}].Buff.Find[9755]}].Count[ ]}]}<${tTime}) /varset tTime ${NetBots[${Group.Member[${x}]}].Duration[${NetBots[${Group.Member[${x}]}].Buff.Left[${NetBots[${Group.Member[${x}]}].Buff.Find[9755]}].Count[ ]}]}
| |  		${NetBots[${Group.Member[${x}]}].Duration[${NetBots[${Group.Member[${x}]}].Buff.Left[${NetBots[${Group.Member[${x}]}].Buff.Find[9755]}].Count[ ]}]}
|   }
     /if (${tName.Equal[${Me.CleanName}]} && ${ABTarType[${a}].Find[self]} && (${Spell[${tSpellID}].Stacks[0]} && ${Me.FreeBuffSlots} && !${Me.Buff[${Spell[${tSpellID}]}].ID}) && (${ABTarCnt[${a}]}<=${SpawnCount[pc group radius ${tSpellRange}]} || ${ABTarCnt[${a}]}==1)) /varset tTime 0
     /if (!${tGem} || !${tSpellID} || !${tTargetID} || !${tName.Length} || ${tTime}>${Macro.RunTime} || ${Spawn[${tTargetID}].Distance3D}>${tSpellRange}) ${If[${tPet},/goto :tFoundPet,/goto :tNextBot]}
     /if (${tGem}<19) {
        /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
           /if (${ABGemInUse.Length}>4 && ${ABGemInUse.Arg[1,|].Equal[${ABGem[${a}]}]} && ${ABGemInUse.Arg[3,|]}>${Macro.RunTime}) /goto :tNextBuff
           /squelch /memorize "${ABSpell[${a}]}|gem${tGem}"
           /delay 1s ${Cast.Status.Equal[m]}
           /delay 14s ${Cast.Status.Equal[i]}
           /if (${Me.Gem[${tGem}].ID}!=${Me.Book[${Me.Book[${ABSpell[${a}]}]}].ID}) /multiline ; /bc BUFF - ${ABSpell[${a}]} - FAILED MEMORIZE;/goto :tNextBuff
        }
        /if (!${ABGemInUse.Length} || ${ABGemInUse.Arg[2,|].NotEqual[${ABSpell[${a}]}]}) /varset ABGemInUse ${ABGem[${a}]}|${ABSpell[${a}]}|${Math.Calc[${Macro.RunTime}+${Spell[${ABSpell[${a}]}].RecastTime}+10]}
|/echo ${ABGemInUse}
       /if (!${Cast.Ready[${tGem}]}) /varcalc tCT ${Spell[${tSpellID}].RecastTime}*10+${tCT}
     }
     /if (${Spell[${tSpellID}].TargetType.Equal[self]}) /varset tTargetID 0
|    /if (${Spell[${tSpellID}].TargetType.Find[group]} && (${Group.Member[${tName}].Index} || ${Me.Name.Find[${tName}]}) && (${Spawn[${tTargetID}].Distance}<${Spell[${tSpellID}].Range} || ${Spawn[${tTargetID}].Distance}<${Spell[${tSpellID}].AERange}) && (!${Target.ID} || ${Group.Member[${Target.Name}].Index} || ${Target.Type.Equal[npc]})) /varset tTargetID 0
|    /if (${Spell[${tSpellID}].TargetType.Find[group]} && ${Group.Member[${tName}].Index} && ${Spawn[${tTargetID}].Distance}<${Spell[${ABSpell[${a}]}].Range} && (!${Target.ID} || ${Group.Member[${Target.Name}].Index} || ${Target.Type.Equal[npc]})) /varset tTargetID 0
|    /if (${ADMobCount} && (${DoMelee} && !${ACState} || !${DoBuffsCBT})) /return
     /multiline ; /varset ABtTargetID 0;/varset tSpellDelay 0;/varset tCastTime ${tCT};/if (${Window[SpellBookWnd].Open}) /keypress ctrl+b;/varset CastResult UNKNOWN
:tCastBuff
     /if (${Debuff.Silenced} || ${Debuff.Stunned}  || ${Debuff.Charmed} || ${Window[RespawnWnd].Open}) /return
|    /if (${DoHeals} && ${AHCount}) /call AHHealCheck
     /if (!${Cast.Ready} && !${ABtTargetID}) /delay 1s ${Cast.Ready}
     /if (!${ABtTargetID} && ${Cast.Ready} && !${Cast.Ready[${ABSpell[${a}]}]} && ${Spell[${tSpellID}].RecastTime}>5) /goto :tNextBuff
     /if (${ABtTargetID}) {
        /if (${Cast.Status.Equal[i]}) {
      	   /doevents OUTDOORS
      	   /if (${Select[${Cast.Result},CAST_OUTDOORS]} || ${OutDoorsFail}) {
      	      /bc Outdoor only spell, turning off buff [ ${ABSpell[${a}]} ] for this zone.  Check "${MBCommon}" if incorrect setting.
|      	      ;/varset ABTarCnt[${a}] 0;
              /if (!${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Find[${tSpellID}|]}) {
                 /if (${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Length} >0 ) {
        	    /ini "${MBCommon}" RestrictedList ${Zone.ID} ${Ini["${MBCommon}",RestrictedList,${Zone.ID}]}${tSpellID}|
        	 } else {
        	    /ini "${MBCommon}" RestrictedList ${Zone.ID} ${tSpellID}|
        	    /echo /ini "${MBCommon}" RestrictedList ${Zone.ID} ${tSpellID}|
        	 }
       	      }
              /multiline ; /varset OutDoorsFail FALSE;/goto :tNextBuff
     	   }
           /if (${Cast.Result.Equal[cast_success]} || ${Me.Class.ShortName.Equal[BRD]}) /goto :tContBuff
           /if (${Select[${Cast.Result},CAST_FIZZLE,CAST_RECOVER,CAST_OUTOFMANA]}) /multiline ; /if (${Me.CurrentMana}<${Spell[${tSpellID}].Mana} || ${Me.PctMana}<${ABSpellMinMana[${a}]}) /goto :tNextBuff;/varset tCastTime ${tCT};/varset ABtTargetID 0
           /if (${Select[${Cast.Result},CAST_IMMUNE,CAST_TAKEHOLD]}) /multiline ; /varset tSpellDelay 900;/goto :tContBuff
           /if (${Select[${Cast.Result},CAST_OUTOFRANGE,CAST_CANNOTSEE,CAST_DISTRACTED,CAST_NOTARGET,CAST_INTERRUPTED,CAST_CANCELLED,CAST_NOTREADY]} && !${OutDoorsFail}) /goto :tNextBot
        }
     } else /if (${Cast.Ready[${ABSpell[${a}]}|${ABGem[${a}]}]}) {
        /varset ABtTargetID ${tTargetID}
        /if (${Me.Shrouded}) /delay 1s
        /if (${Me.Class.ShortName.Equal[BRD]}) /multiline ; /squelch /twist stop;/delay 3s !${Twist}
        /varset MBCurrentSpell AB|${a}|${ABtTargetID}
        /doevents flush
        /call MQ2Cast "${ABSpell[${a}]}" ${If[${Select[${ABGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]},gem,]}${ABGem[${a}]} ${Math.Calc[${Spell[${ABSpell[${a}]}].MyCastTime}+1]}s CastCheck ${If[${ABtTargetID},-targetid|${ABtTargetID}, ]} -bandolier|${ABSpellFoci[${a}]}
        /if (${Me.Class.ShortName.Equal[BRD]}) /delay 3s
        /if (!${ABtTargetID}) /varset ABtTargetID ${Me.ID}
        /varset tCastTime 5
     }
     /if (${interruptFlag}) /return
     /goto :tCastBuff
     /echo WTF - Buff
     /goto :tNextBot
:tContBuff
     /varset MiscCheckTimer 5s
     /if (${ABAnnounce[${a}].Length} && (!${Select[${Cast.Result},cast_immune,cast_takehold]} || ${ABTarCnt[${a}]}>1)) { 
        /if (${ABAnnounce[${a}].Length}>10) { 
           /call ParseAnnounce "${ABAnnounce[${a}]}" "${Spawn[${ABtTargetID}].CleanName}" "${ABSpell[${a}]}" 
        } else { 
           /docommand ${ABAnnounce[${a}]} Buffed: -[ ${Spawn[${ABtTargetID}].CleanName} ]- with ${ABSpell[${a}]} 
        } 
     }
     /call AddtoBuffArray ${tTargetID} ${tSpellID} ${tSpellDelay} ${a}
     /varset ABGemInUse
     /varset MBCurrentSpell
     /if (${ADMobCount} || ${MBPause} || ${FollowFlag} && ${Spawn[pc ${FollowID}].Distance}>${FollowDistance}) /return
     /if (${Cursor.ID}) /call ClearCursor
     /if (${DoMount} && !${Me.Mount.ID} && ${MountCast.Length}>5 && !${Me.Invis}) /call PMACheck "m"
     /call ADSpawnCheck
:tNextBot
     /doevents
     /if (${tPet}) /goto :tFoundPet
     /next b
:tNextBuff
  /next a
/return


Sub AddtoBuffArray(int tTargetID,string tSpellID, int tSpellDelay, int a)
| /echo AddtoString  - Target: ${tTargetID} - Spell: ${Spell[${tSpellID}].Name}  - Delay: ${tSpellDelay}  -BuffNumber: ${a}
  /declare tABMob int local 0
  /declare c int local 0
  /declare tSpellDuration int local 0
  /declare sVarL string local
  /declare sVarR string local
  /if (!${tTargetID}) /varset tTargetID ${Me.ID}
  /for c 1 to ${ABMobMax}
     /if (${ABMBL[${c}].Arg[1,|].Equal[${Spawn[id ${tTargetID}].Name}]} || ${ABMBL[${c}].Arg[1,|].Equal[${Spawn[id ${tTargetID}].CleanName}]}) /varset tABMob ${c}
|    /if (${ABMBL[${c}].Arg[1,|].Equal[${Me.CleanName}]} && (!${tTargetID} || ${tTargetID}==${Me.ID})) /varset tABMob ${c}
  /next c
  /if (!${tABMob}) /echo Target not found in array.  Buff not added.
  /if (${a}) {
     /if (${Me.Shrouded}) {
        /varcalc tSpellDuration 900*1.${ABDurMod[${a}]}+1+${tSpellDelay}+${Macro.RunTime}-20
     } else {
        /varcalc tSpellDuration ((${ABDurMod[${a}]}/100)*${Spell[${tSpellID}].Duration.TotalSeconds})+${Spell[${tSpellID}].Duration.TotalSeconds}+${tSpellDelay}+${Macro.RunTime}-20
     }
  } else {
     /varset tSpellDuration ${tSpellDelay}+${Macro.RunTime}-20
  }
  /if (${ABMBL[${tABMob}].Find[|b${tSpellID}]}) {
     /varset sVarL ${ABMBL[${tABMob}].Left[${Math.Calc[${ABMBL[${tABMob}].Find[|b${tSpellID}]}+${tSpellID.Length}+1]}]}
     /varset sVarR ${ABMBL[${tABMob}].Right[-${Math.Calc[${ABMBL[${tABMob}].Find[|b${tSpellID}]}+${tSpellID.Length}+2]}]}
     /varset sVarR ${sVarR.Right[-${sVarR.Arg[1,|].Length}]}
|/echo Left: ${sVarL}  Right: ${sVarR}
     /varset ABMBL[${tABMob}] ${sVarL}|t${tSpellDuration}${sVarR}
|/echo ${ABMBL[${tABMob}]}
  } else {
     /varset ABMBL[${tABMob}] ${ABMBL[${tABMob}]}b${tSpellID}|t${tSpellDuration}|
  }
/return

sub Event_OUTDOORS()
/echo Outdoor Spell..
  /varset OutDoorsFail TRUE
  /if (${Me.Class.ShortName.Equal[brd]}) {
     /declare x int local 0
     /echo ${Cast.Stored} will be turned off for this zone.  Re-start macro or use alias to turn it back on.
     /for x 1 to ${ABCount}
        /if (${ABSpell[${x}].Equal[${Cast.Stored}]}) /varset ABTarCnt[${x}] 0
     /next x
     /if (!${ACState}) /multiline ; /varset BardRestGems;/varset BardCombatGems
     /call ABBuffCheck
  }
/return

sub StacksCheck(string tSpellID,string tBuffs)
  /if (!${tBuffs.Length}) /return TRUE
  /declare x int local 0
  /for x 1 to ${tBuffs.Count[ ]}+1
     /if (${tBuffs.Arg[${x}, ].Equal[${tSpellID}]}) /multiline ; /echo Buff on Target;/return FALSE
     /if (${tBuffs.Arg[${x}, ].NotEqual[${tSpellID}]} && !${tBuffs.Find[${tSpellID}]}) {
        /docommand ${If[${Spell[${tBuffs.Arg[${x}, ]}].WillStack[${tSpellID}]},/next x,/return FALSE]}
     }
  /next x
/return