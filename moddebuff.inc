
#Event MezResist "#*#target resisted the #1# spell.#*#"

Sub LoadADINIVals
 /declare a int local
 /call LoadINIVar AdvDebuff ADCount 0 ADCount "${IniFile}" int
 /declare ADMobCount int outer 0
 /call LoadINIVar AdvDebuff ADMobMax 20 ADMobMax "${IniFile}" int
 /call LoadINIVar AdvDebuff ADCheckTime 2 ADCheckTime "${IniFile}" int
 /call LoadINIVar AdvDebuff ADAggroOnly 0 ADAggroOnly "${IniFile}" int
 /if (!${Select[${Me.Class.ShortName},enc,brd]}) /varset ADAggroOnly 0
 /call LoadINIVar AdvDebuff ADHold "0|1|1|   1=on 0=off|Debuff spell #|Time to wait for debuff|" ADHold "${IniFile}" string
 /call LoadINIVar Settings CheckTargetDebuffs FALSE CheckTargetDebuffs "${MBCommon}" bool
| /call LoadINIVar Immune "Imm<spell id>" "<max level>|<mob name>|<zone id>|<mob name>|<zone id>| etc etc." Imm00000 "${MBCommon}" string
 /declare ADCheckTimer timer outer
 /declare ADMDL[${ADMobMax},${If[${ADCount},${ADCount},1]}] int outer 0
 /declare ADMobList[${ADMobMax}] int outer 0
 /declare MLDebuffed[${ADMobMax}] bool outer FALSE
 /declare CurrentMobCount int outer 0
 /declare SpawnCheckTimer timer outer
 /declare AddSpawn bool outer FALSE
 /if (${ADCount}) {
   /declare ADGem[${ADCount}] string outer
   /declare ADSpell[${ADCount}] string outer
   /declare ADSpellFoci[${ADCount}] string outer
   /declare ADDurMod[${ADCount}] string outer
   /declare ADSpellMinMana[${ADCount}] int outer
   /declare ADSpellRecast[${ADCount}] int outer
   /declare ADSpellCastonResist[${ADCount}] string outer
   /declare ADSpellDelay[${ADCount}] int outer
   /declare ADTarCnt[${ADCount}] int outer
   /declare ADTarType[${ADCount}] int outer
   /declare ADTarBegHP[${ADCount}] int outer
   /declare ADTarEndHP[${ADCount}] int outer
   /declare ADSpellAlias[${ADCount}] string outer
   /declare ADAnnounce[${ADCount}] string outer
   /declare ADDiscTimer[${ADCount}] int outer
   /declare ADIfSpellImmune[${ADCount}] string outer
   /declare ADUseHoTT[${ADCount}] int outer
   /declare ADCondition[${ADCount}] string outer
   
   /declare tSpellID int local
   /for a 1 to ${ADCount}
     /call LoadINIVar AdvDebuff ADGem${a} " " ADGem[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADSpell${a} " " ADSpell[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADSpellFoci${a} " " ADSpellFoci[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADDurMod${a} 0 ADDurMod[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADSpellAlias${a} " " ADSpellAlias[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADAnnounce${a} " " ADAnnounce[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADSpellMinMana${a} 0 ADSpellMinMana[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADSpellRecast${a} 0 ADSpellRecast[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADSpellCastonResist${a} " " ADSpellCastonResist[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADSpellDelay${a} 0 ADSpellDelay[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADTarCnt${a} 0 ADTarCnt[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADTarType${a} 0 ADTarType[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADTarBegHP${a} 0 ADTarBegHP[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADTarEndHP${a} 0 ADTarEndHP[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADIfSpellImmune${a} " " ADIfSpellImmune[${a}] ${IniFile}
     /call LoadINIVar AdvDebuff ADUseHoTT${a} "0" ADUseHoTT[${a}] ${IniFile}
			/call LoadINIVar AdvDebuff ADPreCondition${a} "TRUE" ADCondition[${a}] ${IniFile}
     /if (${Select[${ADGem[${a}]},1,2,3,4,5,6,7,8,9,10,11,12]} && ${ADSpell[${a}].Length}) {
  	   	 /docommand ${If[${Me.Book[${Me.Book[${ADSpell[${a}]} Rk. III]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${ADSpell[${a}]} Rk. III]}].ID},${If[${Me.Book[${Me.Book[${ADSpell[${a}]} Rk. II]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${ADSpell[${a}]} Rk. II]}].ID},${If[${Me.Book[${Me.Book[${ADSpell[${a}]}]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${ADSpell[${a}]}]}].ID},/bc Spell ${ADSpell[${a}]} Not found in your book.  Check your .ini settings.]}]}]}
  	     /if (${Me.Book[${Me.Book[${ADSpell[${a}]}]}].ID}!=${tSpellID}) /varset ADSpell[${a}] ${Spell[${tSpellID}].Name}
      } else /if (${ADGem[${a}].Equal[alt]}) {
      	/if (!${Me.AltAbility[${ADSpell[${a}]}]} && !${Me.Shrouded}) /bc You don't have the ${ADSpell[${a}]} Alt.  Please check ADSpell${a} in your .ini
      }
      /call LoadINIVar "${Spell[${ADSpell[${a}]}].Name}" "MaxLevel" "150" "MaxLevel${Spell[${ADSpell[${a}]}].ID}" "${MBCommon}" string
      /if (${CheckTargetDebuffs} && ${Spell[${ADSpell[${a}]}].Duration} && ${Spell[${ADSpell[${a}]}].SpellType.Equal[Detrimental]}) /call LoadINIVar "${Spell[${ADSpell[${a}]}].Name}" "OverwritesID" "| Pipe delimited list of spell ID's this should overwrite - |Number|Number| " "OverWrite${Spell[${ADSpell[${a}]}].ID}" "${MBCommon}" string

      /if (!${Defined[Imm${Spell[${ADSpell[${a}]}].ID}]}) /declare Imm${Spell[${ADSpell[${a}]}].ID} string outer
      /if (${Ini["${MBCommon}","${Spell[${ADSpell[${a}]}].Name}","${Zone.ShortName}Immune"].Find[|]}) /call LoadINIVar "${Spell[${ADSpell[${a}]}].Name}" "${Zone.ShortName}Immune" " " "Imm${Spell[${ADSpell[${a}]}].ID}" "${MBCommon}" string
|      /echo /if (${Ini["${MBCommon}","${Spell[${ADSpell[${a}]}].Name}","${Zone.ShortName}Immune"].Find[|]}) /call LoadINIVar "${Spell[${ADSpell[${a}]}].Name}" "${Zone.ShortName}Immune" " " "Imm${Spell[${ADSpell[${a}]}].ID}" "${MBCommon}" string
|		 Sub LoadINIVar(IniSection,IniVar,IniValue,MacroVar,MyIni,VarType)
   /next a
   /if (${Defined[PetCast]} && !${Defined[ADCharmID]}) {
     /if (${PetCast.Find[charm ]}) {
       /declare ADCharmID int outer 0
       /declare ADCharmGem int outer ${PetCast.Arg[2]}
       /declare ADCharmType int outer ${PetCast.Arg[3]}
       /declare ADCharmName string outer ${PetCast.Arg[4]}
       /for a 1 to ${ADCount}
         /if (${ADGem[${a}]}==${ADCharmGem}) {
           /declare ADCharmSpell string outer ${ADSpell[${a}]}
           /if (${Me.Pet.ID} && (!${ADCharmType} || ${ADCharmType}==1 && !${Select[${Spawn[${Me.Pet.ID}].Body},undead,vampyre]} || ${ADCharmType}==2 && !${Select[${Spawn[${Me.Pet.ID}].Body},animal]} || ${ADCharmType}==3 && !${Select[${Spawn[${Me.Pet.ID}].Body},summoned]} || ${ADCharmType}==4 && ${ADCharmName.Find[${Spawn[${Me.Pet.ID}].CleanName}]})) /multiline ; /varset ADCharmID ${Me.Pet.ID};/varset PetAssist 1;/varset DoPet TRUE;/varset ADMobList[1] ${Me.Pet.ID};/varset ADMDL[1,${a}] ${Spell[${ADCharmSpell}].Duration.TotalSeconds}
         }
       /next a
     }
   }
 }
/return

Sub ADSpawnCheck
/if (${SpawnCheckTimer}) /return
 /declare a int local
 /declare b int local
 /declare y string local
 /declare z string local
 /declare tNPCCount int local 0
 /declare npcid int local 0
 /declare tADAdd bool local FALSE
 /multiline ; /varset ADMobCount 0;/varset AddSpawn FALSE
 /if ( ${MBDebug} ) /echo Macro.RunTime:${Macro.RunTime} ${Time}- Sub:ADSpawnCheck
| /echo Macro.RunTime:${Macro.RunTime} ${Time}- Sub:ADSpawnCheck
 /if ( ${Select[${Zone.ShortName},GuildHall,GuildLobby,PoKnowledge,Nexus,Bazaar,AbysmalSea]} ) /return FALSE
 /if ( ${ADAggroOnly} ) {
   /declare sNBClnt string local ${NetBots.Client}
   /for a 0 to ${Group.Members}
     /if ( !${NetBots[${Group.Member[${a}].Name}].ID} ) /varset sNBClnt ${sNBClnt} ${Group.Member[${a}].Name}
   /next a
   /if ( ${ADAggroOnly}<${Math.Calc[${Macro.RunTime}-3]} ) /multiline ; /squelch /alert clear 30;/varset ADAggroOnly ${Macro.RunTime}
 }
 /if (${ZoneName.NotEqual[${Zone.Name}]}) /call ProcessExcludeList
 /declare TotalNPCs int local ${SpawnCount[any radius ${Radius} zradius 100 noalert ${AlertList}]}
 :GetSpawn
 /varcalc tNPCCount ${tNPCCount}+1
 /if (${tNPCCount}>${TotalNPCs}) /goto :CleanseArray
 /varset npcid ${NearestSpawn[${tNPCCount},any radius ${Radius} zradius 100 noalert ${AlertList}].ID}
 /if (!${npcid}) /goto :CleanseArray
 /multiline ; /varset z ${Spawn[${npcid}].Name};/varset y ${Spawn[${npcid}].Type}
 /if (!${Spawn[${npcid}].LineOfSight} && ${ACMATarget}!=${npcid} || !${z.Length} || !${Spawn[${npcid}].Standing} || ${Select[${y},chest,untargetable,trigger,trap,timer,item,object,banner,campfire,mercenary,pc,mount,aura,corpse,flyer]} || ${Spawn[${npcid}].Body.Name.Equal[object]}) /goto :GetSpawn
  /if (${Spawn[${npcid}].Master.Type.Equal[pc]} || ${Spawn[${npcid}].Type.Equal[pc]} || (!${Spawn[${npcid}].LineOfSight} && ${Spawn[${npcid}].Distance3D}>25) || ${Spawn[${npcid}].Name.Left[1].Equal[_]} && ${Spawn[${npcid}].CleanName.Length}<2) /goto :GetSpawn
 /if (${ADAggroOnly}) {
    /if ((${FollowFlag} && !${Spawn[${FollowName}].ID}) || (!${Spawn[id ${npcid} noalert 30].ID})) /goto :GetSpawn
   /if (${Target.ID}!=${npcid}) /squelch /tar id ${npcid}
   /delay 1s ${Target.ID}==${npcid}
   /varset b 0
   /if (${Target.ID}==${npcid}) {
       /if (${Window[TargetOfTargetWindow].Open}) {
            /if (${sNBClnt.Find[${Me.TargetOfTarget.CleanName}]}) /varset b ${npcid}
          } else {
            /assist
            /timed 4 /assist
            /delay 5 ${Target.ID}!=${npcid}
            /if (${sNBClnt.Find[${Spawn[${Target.ID}].CleanName}]}) /varset b ${npcid}
       }
   }
   /if (${b}!=${npcid}) {
     /squelch /alert add 30 id ${npcid}
     /goto :GetSpawn
   }
 }
 /varcalc ADMobCount ${ADMobCount}+1
 /if (${ADMobCount}>${ADMobMax}) /multiline ; /echo Too many mobs to track;/return ${tADAdd}
 /if (${ADMobList[${ADMobCount}]} && (!${Spawn[${ADMobList[${ADMobCount}]}].ID} || ${Spawn[${ADMobList[${ADMobCount}]}].Type.Equal[Corpse]} || ${Spawn[${ADMobList[${ADMobCount}]}].Distance}>${Radius}) ) /varset ADMobList[${ADMobCount}] 0
 /for a 1 to ${ADMobMax}
   /if (${npcid}==${ADMobList[${a}]}) /goto :GetSpawn
 /next a
 /for a 1 to ${ADMobMax}
   /if ( !${ADMobList[${a}]} ) {
     /if ( !${tADAdd} ) /multiline ; /varset tADAdd TRUE;/varset AddSpawn TRUE
     /varset ADMobList[${a}] ${npcid}
     /varset MLDebuffed[${a}] FALSE
     /for b 1 to ${ADCount}
        /varset ADMDL[${a},${b}] 0
     /next b
     /goto :GetSpawn
   }
 /next a
:CleanseArray
 /if (${ADMobMax}>${ADMobCount}) {
   /for a ${Math.Calc[${ADMobMax}-${If[${ADMobCount},${ADMobCount},1]}]} to ${ADMobMax}
      /if (${ADMobList[${a}]}) {
        /varset MLDebuffed[${a}] FALSE
        /for b 1 to ${ADCount}
          /varset ADMDL[${a},${b}] 0
        /next b
      }
    /next a
 }
 /if (${DoPull}) {
    /if (!${ADMobCount} && ${CurrentMobCount}==1) /multiline ; /varset APCheckTimer ${APCheckTime}s;/varset APPullDelay 0
 }
  /docommand ${If[${CurrentMobCount} && ${CurrentMobCount}>${ADMobCount},/varcalc CurrentMobCount ${CurrentMobCount}-1,/varset CurrentMobCount ${ADMobCount}]}
|  /echo Macro.RunTime:${Macro.RunTime} ${Time}- Leaving Sub:ADSpawnCheck
  /varset SpawnCheckTimer 1s
/return ${tADAdd}

Sub ADDebuffCheck
 /if (${MBPause} || !${ADCount}) /return
 /varset ADCheckTimer ${ADCheckTime}s
 /if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:ADDebuffCheck
 /declare a int local
 /declare b int local
 /declare c int local
 /declare d int local
 /declare e int local
 /declare x int local
 /declare tGem int local
 /declare tSpellID int local
 /declare tSpellRange int local
 /declare tSpellDuration int local
 /declare tSpellDelay int local
 /declare tCastTime timer local
 /declare tCT int local
 /declare ADtTargetID int local
 /declare tTarCnt int local
 /declare tRecast int local
 /declare tCastonRes int local
 /declare existingTar int local
 /declare tADCount int local
 /declare tADTime int local ${Macro.RunTime}
 /declare ADHoldTimer timer local
 /declare sVarC string local
 /declare sVarL string local
 /declare sVarM string local
 /declare HoTTTarget bool local
 /call ADSpawnCheck
 /if (!${ADMobCount}) {
   /for a 1 to ${ADCount}
      /if (${ADTarType[${a}]}>99) {
        /varcalc ADTarType[${a}] ${ADTarType[${a}]}-100
        /if (${ADSpellDelay[${a}]}>=${Macro.RunTime}) /varset ADSpellDelay[${a}] 0
      }
   /next a
   /return
 }
 /if (${Melee.Combat}) {
   /if (${OffTank}) /varset existingTar ${Target.ID}
   /if (${Me.Class.ShortName.Equal[BRD]} && !${Twist}) /squelch /twist ${BardCombatGems}
 }
:tADTop
/varset tADTime ${Macro.RunTime}
 /varset tADCount 1
 :tADCountTop
 /for a 1 to ${ADMobMax}
   /if (!${ADMobList[${a}]}  || !${Spawn[${ADMobList[${a}]}].ID}) /goto :tNextMob
   /if (${Spawn[${ADMobList[${a}]}].Distance}<=${Radius}) {
|b
        /varset MLDebuffed[${a}] TRUE
    /if (!${ACState} && ${TankName.Length}>2 && ${Spawn[pc ${TankName}].Distance}<${Radius}) {
      /if (!${existingTar} && ${ACMATarget}) /varset existingTar ${ACMATarget}
      /if (${NetBots[${TankName}].ID}) {
        /varset ACMATarget ${NetBots[${TankName}].TargetID}
        } else /if (!${NetBots[${TankName}].ID}) {
        /call ACGetAssist ${TankName}
        /varset ACMATarget ${Macro.Return}
        /if (${PetAssist}>1 && (!${ACMATarget} || !${OffTank} && ${existingTar} && ${existingTar}!=${ACMATarget})) /varset PetAssist 1
      }
    }
|e
     /varset tGem 0
     /for b ${tADCount} to ${ADCount}
       /if (!${DoDebuffs} || ${MBPause}) /goto :tEnd
       /varset tCastonRes 0
|NEW CHARM CODE
       /if (${Defined[ADCharmID]} && ${DoPet}) {
         /if (${ADGem[${b}]}==${ADCharmGem}) {
           /if (${Me.Pet.ID} || (${ADCharmType}==1 && !${Select[${Spawn[${ADMobList[${a}]}].Body},undead,vampyre]} || ${ADCharmType}==2 && !${Select[${Spawn[${ADMobList[${a}]}].Body},animal]} || ${ADCharmType}==3 && !${Select[${Spawn[${ADMobList[${a}]}].Body},summoned]} || ${ADCharmType}==4 && !${ADCharmName.Find[${Spawn[${ADMobList[${a}]}].CleanName}]})) /goto :tNextDebuff
           /if (!${Me.Pet.ID}) {
             /if (${ADCharmID}) {
               /if (!${Spawn[${ADCharmID}].ID}) /multiline ; /varset ADCharmID 0;/goto :tCheckforAdds
               /multiline ; /echo Lost pet, Prior pet was ${ADCharmID} - ${Spawn[${ADMobList[${a}]}].CleanName};/varset ADMDL[${a},${b}] 0
             }
           }
         }
         /if (${ADMobList[${a}]}==${ADCharmID} && ${Me.Pet.ID}) /goto :tNextMob

       }
|END CHARM CODE
    /if (${ADMDL[${a},${b}]}>=${tADTime} || ${ADTarType[${b}]}>99) /goto :tWhatNext
|  /if (${Me.Pet.ID} && (${PetAssist}==1 || ${PetAssst}!=${ACMATarget}) && ${ADMobList[${a}]}==${ACMATarget} && ${Spawn[${ADMobList[${a}]}].PctHPs}<${ACAssistPct} && ${Spawn[${ADMobList[${a}]}].Distance}<=${ACLeash} && ${Spawn[${ADMobList[${a}]}].PctHPs}>${ACEndHP} && ${Target.ID}==${ADMobList[${a}]}) /multiline ; /pet attack;/varset PetAssist ${ADMobList[${a}]}
    /if (!${ADTarCnt[${b}]}) /goto :tNextDebuff
    /if (${Spawn[${ADMobList[${a}]}].Type.Equal[Corpse]} && ${Target.ID}==${Spawn[${ADMobList[${a}]}].ID} && ${Me.Casting.ID} && ${Spell[${Me.Casting.ID}].SpellType.Equal[detrimental]} && !${Me.Mount.ID}) /multiline ; /interrupt;/goto :tNextMob
		/if (${Spawn[${ADMobList[${a}]}].Type.Equal[Corpse]} || (${ADTarType[${b}]}>9 && ${ADMDL[${a},${b}]}>=${tADTime}) || (${ADMobList[${a}]}==${ACMATarget} && ${ADTarType[${b}]}==12) || (${ADMobList[${a}]}!=${ACMATarget} && ${ADTarType[${b}]}==11)) /goto :tNextMob
       /if (${ADCondition[${b}].Length}>4) {
       	/call IniCommand "${ADCondition[${b}]}" ${ADMobList[${a}]}
|       	/echo ADCondition[${b}] == ${Macro.Return}
       	/if (${Macro.Return.NotEqual[TRUE]}) /goto :tWhatNext
      }
    /if (!${ADHoldTimer} && ${Spawn[${ADMobList[${a}]}].ID} && ${ADHold.Arg[1,|]} && ${b}==${ADHold.Arg[2,|]} && ${ADMDL[${a},${b}]}<=${Macro.RunTime}+${ADHold.Arg[3,|]} && ${ADMDL[${a},${b}]}>=${Macro.RunTime} && ${ADMobList[${a}]}!=${ACMATarget} && ${ADTarType[${b}]}==12) /varset ADHoldTimer ${Math.Calc[${ADHold.Arg[3,|]}+.5]}s
|    /if (${ADHoldTimer}) /echo ${ADHoldTimer}
      /if (${ADTarType[${b}]}>99 && ${ADSpellDelay[${b}]} && ${ADSpellDelay[${b}]}<=${Macro.RunTime}) {
        /if (!${Ini["${IniFile}",AdvDebuff,ADSpellDelay${b}]}) /multiline ; /echo AE Reset 202;/varcalc ADTarType[${b}] ${ADTarType[${b}]}-100;/varset ADSpellDelay[${b}] 0
      }
      /if ((${ADTarCnt[${b}]}>1 && ${ADMobCount}<${ADTarCnt[${b}]}) || ${ADMDL[${a},${b}]}>=${tADTime} || ${Spawn[${ADMobList[${a}]}].PctHPs}>${ADTarBegHP[${b}]} || ${Spawn[${ADMobList[${a}]}].PctHPs}<${ADTarEndHP[${b}]} || ${NetBots[${TankName}].TargetID}==${ADMobList[${a}]} && ${NetBots[${TankName}].TargetHP}<${ADTarEndHP[${b}]} || (${ADMobList[${a}]}==${ACMATarget} && ${ADTarType[${b}]}==2) || (${ADMobList[${a}]}!=${ACMATarget} && ${ADTarType[${b}]}==1) || (${ADMobList[${a}]}==${Me.Pet.ID})) /goto :tNextDebuff
       /varset ADMDL[${a},${b}] ${tADTime}
       /if (${Select[${ADGem[${b}]},1,2,3,4,5,6,7,8,9,10,11,12]}) {
|  /echo was Spell - ${b}
         /if (${Me.CurrentMana}+20<${Me.Book[${Me.Book[${ADSpell[${b}]}]}].Mana} || ${Me.PctMana}<${ADSpellMinMana[${b}]}) /goto :tWhatNext
         /multiline ; /varset tSpellID ${Me.Book[${Me.Book[${ADSpell[${b}]}]}].ID} ;/varset tGem ${ADGem[${b}]} ;/varcalc tCT ${Spell[${tSpellID}].MyCastTime}*10+50
         /if (${Spell[${tSpellID}].SpellType.Equal[Beneficial]} && ${Spell[${tSpellID}].Duration} && ${Me.Buff[${Spell[${tSpellID}].Name}].ID}) /goto :tWhatNext
         /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
           /delay 2s ${Cast.Status.Equal[i]}
		       /squelch /memorize "${Spell[${tSpellID}]}|gem${tGem}"
           /delay 5s ${Cast.Status.Equal[i]}
           /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) /multiline ; /bc DEBUFF - ${ADSpell[${b}]} - FAILED MEMORIZE;/goto :tWhatNext
           /varcalc tCT ${Spell[${tSpellID}].RecastTime}*10+${tCT}
         }
         /if (${Me.Class.ShortName.Equal[BRD]} && (${ADTarType[${b}]}==1 || ${ADTarType[${b}]}==11)) {
           /if (${BardCombatGems.Find[${tGem}]}) /goto :tWhatNext
           /multiline ; /varset BardCombatGems ${BardCombatGems} ${tGem};/goto :tWhatNext
         }

         } else /if ( ${ADGem[${b}].Find[item]} && ${Bool[${FindItem[=${ADSpell[${b}]}]}]} ) {
|/echo was Item
          /if (${FindItem[=${ADSpell[${b}]}].Timer} ) /goto :tWhatNext
          /multiline ; /varset tSpellID ${FindItem[=${ADSpell[${b}]}].Spell.ID};/varset tGem 20 ;/varcalc tCT ${FindItem[=${ADSpell[${b}]}].CastTime}*10+40
          } else /if (${ADGem[${b}].Equal[alt]}) {
|/echo was Alt
          	/if (${Me.CurrentMana}<${Me.AltAbility[${ADSpell[${b}]}].Spell.Mana}) /goto :tNextDebuff
         	 	/if (${Me.Shrouded}) /delay 3s ${Me.AltAbilityReady[${ADSpell[${b}]}]}
          	/if (!${Me.AltAbility[${ADSpell[${b}]}]} && !${Me.Shrouded} && !${Me.AltAbilityReady[${ADSpell[${b}]}]} || !${Me.AltAbilityReady[${ADSpell[${b}]}]}) /goto :tNextDebuff
          	
            /multiline ; /varset tSpellID ${Me.AltAbility[${ADSpell[${b}]}].Spell.ID} ;/varset tGem 30 ;/varcalc tCT ${Me.AltAbility[${ADSpell[${b}]}].Spell.CastTime}*10+40
            } else /if (${ADGem[${b}].Equal[script]}) {
|/echo was Script

             /call MBScript ${ADSpell[${b}]}
             /multiline ; /varset tSpellDelay 0;/varset tGem 40;/varset ADtTargetID ${Target.ID};/varset tSpellID 0
             /goto :tContDebuff
             } else /if (${ADGem[${b}].Equal[ability]}) {
|/echo was Ability
             /if (${Target.ID}!=${ADMobList[${a}]}) /squelch /tar id ${ADMobList[${a}]}
             /delay 1s ${Target.ID}==${ADMobList[${a}]}
             /if (${Target.ID}!=${ADMobList[${a}]}) /goto :tWhatNext
             /doability ${ADSpell[${b}]}
             /multiline ; /varset tSpellDelay 0;/varset tGem 50;/varset ADtTargetID ${Target.ID};/varset tSpellID 0
             /goto :tContDebuff
       } else /if (${ADGem[${b}].Equal[disc]}) {
|/echo was disc
        /if (${Me.PctEndurance}<${ADSpellMinMana[${b}]} || ${ADDiscTimer[${b}]}>${Macro.RunTime} || !${Me.CombatAbilityReady[${ADSpell[${b}]}]}) /goto :tNextDebuff
        	/varset ADDiscTimer[${b}] ${Macro.RunTime}+${ADSpellRecast[${b}]}
            /if (${Target.ID}!=${ADMobList[${a}]}) /squelch /tar id ${ADMobList[${a}]}
            /delay 1s ${Target.ID}==${ADMobList[${a}]}
            /if (${Target.ID}!=${ADMobList[${a}]}) /goto :tWhatNext
            /disc ${ADSpell[${b}]}
            /multiline ; /varset tSpellDelay 0;/varset tGem 50;/varset ADtTargetID ${Target.ID};/varset tSpellID 0
            /goto :tContDebuff
      } else /if (${ADGem[${b}].Equal[potionbelt]}) {
|/echo was potionbelt
      	/if (${FindItem[=${ADSpell[${b}]}].Timer} || !${FindItem[=${ADSpell[${b}]}].ID}) /goto :tNextDebuff
      	/if (${Target.ID}!=${ADMobList[${a}]}) /tar id ${ADMobList[${a}]}
      	/delay 5 ${Target.ID}==${ADMobList[${a}]}
      	/if (${Target.ID}!=${ADMobList[${a}]}) /goto :tWhatNext
     		/for x 0 to 4
				  /docommand ${If[${Window[PotionBeltWnd].Child[PW_PotionSlot${x}_Button].Tooltip.Equal[${ADSpell[${b}]}]},/potionbelt Activate ${Math.Calc[${x}+1]},/next x]}
  			/goto :tNextDeBuff
      }
| /if (${ADMobList[${a}]}==${ACMATarget}) /echo Spell ${b} for ${ADMobList[${a}]}
      /if (${Spawn[${ADMobList[${a}]}].Type.Equal[corpse]} || !${tGem} || ${Spell[${tSpellID}].Range} && ${Spawn[${ADMobList[${a}]}].Distance}>${Spell[${tSpellID}].MyRange} || ${Spawn[${ADMobList[${a}]}].Level}>${MaxLevel${Spell[${ADSpell[${b}]}].ID}}) /goto :tWhatNext
| /echo Imm list ${Imm${Spell[${ADSpell[${b}]}].ID}}
      /if (${Imm${Spell[${ADSpell[${b}]}].ID}.Find[|${Spawn[${ADMobList[${a}]}].CleanName}|]} && !${Imm${Spell[${ADSpell[${b}]}].ID}.Find[|${Spawn[${ADMobList[${a}]}].CleanName}*|]}) {
      	/if (${ADIfSpellImmune[${b}].Length}>3) {
      		/target id ${ADMobList[${a}]}
      		/delay 1s ${Target.ID}==${ADMobList[${a}]}
      		/call IniCommand "${ADIfSpellImmune[${b}]}" ${ADMobList[${a}]}
      	}
      	/goto :tWhatNext
      }
|  || ${Spell[${tSpellID}].Range} && ${Spell[${tSpellID}].AERange} && ${Spawn[${ADMobList[${a}]}].Distance}>${Spell[${tSpellID}].AERange}
      /if (${Spell[${tSpellID}].AERange} && !${Select[${Spell[${tSpellID}].TargetType},PB AE,single]} && !${Spell[${tSpellID}].SpellType.Equal[Beneficial]} && !${Spell[${tSpellID}].CastOnYou.Find[drowsy]}) {
         /if (!${Spell[${tSpellID}].Range} && ${Spawn[${ADMobList[${a}]}].Distance}>${Spell[${tSpellID}].AERange} || ${Spell[${tSpellID}].Range} && ${Spawn[${ADMobList[${a}]}].Distance}<${Spell[${tSpellID}].AERange} && !${Spell[${tSpellID}].Name.Equal[Explosion of Hatred]}) /goto :tWhatNext
      }
      /if (${ADUseHoTT[${b}]} && ${Me.LAHoTT} && ${Me.GroupSize}>2) {
        /if (${Target.ID}!=${ADMobList[${a}]}) {
        	/squelch /tar id ${ADMobList[${a}]}
        	/delay 3s ${Target.ID}==${ADMobList[${a}]}
| Have to wait for ToT to update on new target.
        	/delay 1s !${Me.TargetOfTarget.ID}
        }
    	    /delay 2s ${Me.TargetOfTarget.ID}
					/if (${Target.ID} && (${Group.Member[${Me.TargetOfTarget.Name}]} || ${Spawn[raid id ${Me.TargetOfTarget.ID}].ID} || ${Me.TargetOfTarget.ID}==${Me.ID} || ${NetBots[${Me.TargetOfTarget.Name}].ID})) /varset HoTTTarget TRUE
|  && !${Select[${Spawn[id ${Me.TargetOfTarget.ID}].Class.ShortName},war,shd,mnk,pal,brd,rng]}
					/if (!${Me.TargetOfTarget.ID}) /varset HoTTTarget FALSE
  				/if (!${HoTTTarget} && ${Me.LAHoTT}) /multiline ; /echo Skipping mob - ${Spawn[${ADMobList[${a}]}].CleanName};/varset tGem 60;/varset tSpellDelay ${ADUseHoTT[${b}]};/goto :tContDebuff
       } else {
       		/varset HoTTTarget TRUE
       }
| /if (${ADMobList[${a}]}==${ACMATarget}) /echo Spell ${b} for ${ADMobList[${a}]}
       /multiline ; /varset ADtTargetID 0;/varset tRecast 0;/varset tSpellDelay 0;/varset tCastTime ${tCT};/varset CastResult UNKNOWN;/varset d 0
       /if (${Defined[ADCharmID]} && ${DoPet}) /if (!${Me.Pet.ID} && ${ADCharmID} && ${tSpellID}!=${Spell[${ADCharmSpell}].ID}) /multiline ; /echo charm code 185;/if (!${Me.Mount.ID}) /interrupt;/squelch /stoptwist;/goto :tCheckforAdds
 :tCastDebuff
        /if (${Debuff.Silenced} || ${Debuff.Stunned} || ${Window[RespawnWnd].Open}) /return

       /if (${Spell[${tSpellID}].RecastTime}>20 && ${ADTarType[${b}]}<10 && ${Cast.Ready} && !${Cast.Ready[${tSpellID}|${ADGem[${b}]}]} && ${Select[${ADGem[${b}]},1,2,3,4,5,6,7,8,9,10,11,12]}) /goto :tNextDebuff
       /if (${DoHeals} && ${AHCount}) {
          /call AHHealCheck
          /if (${AHFlag}) {
            /if (${Me.Shrouded}) /delay 15
            /multiline ; /varset tCastTime ${tCT};/varset ADtTargetID 0
          }
       }
       /if (${Me.Pet.ID} && ${Target.ID}==${ACMATarget} && ${DoMelee} && ${Target.PctHPs}<=${ACAssistPct} && ${PetAssist}==1) /multiline ; /pet attack;/varset PetAssist 2
       /doevents eqbc
       /if (${Spawn[${ADMobList[${a}]}].Type.Equal[corpse]}) /goto :tNextMob
       /if (${ADtTargetID}) {
         /if (${Spawn[${ADtTargetID}].Type.Equal[Corpse]} || !${Spawn[${ADtTargetID}].ID} || !${Me.Class.ShortName.Equal[brd]} && ${NetBots[${TankName}].ID} && ${Target.ID}==${NetBots[${TankName}].TargetID} && (${ADTarType[${b}]}==2 || ${ADTarType[${b}]}==12)) /multiline ; /if (!${Me.Mount.ID}) /interrupt;/goto :tNextMob
|CHARM CODE
         /if (${Defined[ADCharmID]} && ${DoPet}) /if (!${Me.Pet.ID} && ${ADCharmID} && ${tSpellID}!=${Spell[${ADCharmSpell}].ID}) /multiline ; /echo charm code 200;/if (!${Me.Mount.ID}) /interrupt;/squelch /stoptwist;/goto :tCheckforAdds
|END CHARM CODE
         /if (!${d}) {
           /for e 1 to ${ADCount}
             /if (${Spell[${ADSpell[${e}]}].ID}==${Spell[${ADSpell[${b}]}].ID} && ${e}!=${b}) /multiline ; /varset d ${e};/goto :tCastDebuff
           /next e
           /if (!${d}) /varset d 999
         }
         /if (${Me.Class.ShortName.Equal[BRD]}) {
           /doevents CastRst
           /doevents CastStn
           /doevents CastImm
           /if (!${CastResult.Equal[pending]}) {
             /if (${Select[${CastResult},success]}) /goto :tContDebuff
             /if (${Select[${CastResult},immune]}) {
               /bc ${Spawn[${ADMobList[${a}]}].CleanName} [+r+]IMMUNE[+x+] to ${ADSpell[${b}]}

     /if (${ADIfSpellImmune[${b}].Length}>3) /call IniCommand "${ADIfSpellImmune[${b}]}" ${ADMobList[${a}]}
						/doevents MezResist
           /if (!${Imm${Spell[${ADSpell[${b}]}].ID}.Find[|${Spawn[${ADtTargetID}].CleanName}|]} && !${Imm${Spell[${ADSpell[${b}]}].ID}.Find[|${Spawn[${ADtTargetID}].CleanName}*|]} && ${EventSpell.Find[${Cast.Stored.Name}]}) {
           	/bc Adding ${Spawn[${ADtTargetID}].CleanName} to the immune list.
              /varset Imm${Spell[${ADSpell[${b}]}].ID} ${Imm${Spell[${ADSpell[${b}]}].ID}}|${Spawn[${ADtTargetID}].CleanName}|
              /ini "${MBCommon}" "${Spell[${ADSpell[${b}]}].Name}" "${Zone.ShortName}Immune" "${Imm${Spell[${ADSpell[${b}]}].ID}}"
               }
               /multiline ; /varset tSpellDelay 4000;/goto :tContDebuff
             }
             /if ( ${Select[${ADTarType[${b}]},2,12]} && ${Select[${CastResult},resisted,stunned,reset]} && ${ADtTargetID}==${ACMATarget} ) /goto :tCheckforAdds
             /if (${Select[${CastResult},resisted,stunned,reset]} && (${ADSpellRecast[${b}]} && ${ADSpellRecast[${b}]}>=${tRecast} || !${ADSpellRecast[${b}]})) /multiline ; /varcalc tRecast ${tRecast}+1;/varset ADtTargetID 0;/if (${tRecast}<${ADSpellRecast[${b}]}) /goto :tCastDebuff
             /goto :tCheckforAdds
           }
           /delay 1s
           /if (${DoMelee} && ${ACState} && ${ACMATarget} && ${Target.ID}!=${ACMATarget}) /multiline ; /if (${Target.ID}) /target clear;/tar id ${ACMATarget};/delay 5;/squelch /stick off;/killthis
           /if (!${tCastTime} && !${Cast.Timing}) /varset CastResult SUCCESS
           /goto :tCastDebuff
          } else /if (${Cast.Status.Equal[i]}) {
           /if (${Select[${Cast.Result},cast_recover]}) /multiline ; /varset tCastTime ${tCT}+30;/varset ADtTargetID 0;/delay 5;/goto :tCastDebuff
           /if (${Spell[${ADSpell[${b}]}].TargetType.Find[ ae]} && !${Select[${Cast.Result},cast_fizzle]} && !${ADSpellRecast[${b}]}) {
            /varcalc ADTarType[${b}] ${ADTarType[${b}]}+100
            /if (${Me.Book[${Me.Book[${ADSpell[${b}]}]}].Duration}>2 && !${ADSpellDelay[${b}]} && ${ADTarCnt[${b}]}>1) /varcalc ADSpellDelay[${b}] ${Macro.RunTime}+${Math.Calc[${Me.Book[${Me.Book[${ADSpell[${b}]}]}].Duration.TotalSeconds}*10]}
            /goto :tContDebuff
          }
           /if (${Select[${Cast.Result},cast_immune]}) {
            /varset e 1
             /for e 1 to 5
               /bc ${Spawn[${ADMobList[${a}]}].CleanName} [+r+]IMMUNE[+x+] to ${ADSpell[${b}]}
             /next e
             /docommand /${BeepCommand}
             /if (${ADIfSpellImmune[${b}].Length}>3) /call IniCommand "${ADIfSpellImmune[${b}]}" ${ADMobList[${a}]}
							/doevents MezResist
             /if (!${Imm${Spell[${ADSpell[${b}]}].ID}.Find[|${Spawn[${ADtTargetID}].CleanName}|]} && !${Imm${Spell[${ADSpell[${b}]}].ID}.Find[|${Spawn[${ADtTargetID}].CleanName}*|]} && ${EventSpell.Find[${Cast.Stored.Name}]}) {
             	/echo Adding ${Spawn[${ADtTargetID}].CleanName} to the immune list.
               /varset Imm${Spell[${ADSpell[${b}]}].ID} ${Imm${Spell[${ADSpell[${b}]}].ID}}|${Spawn[${ADtTargetID}].CleanName}|
               /ini "${MBCommon}" "${Spell[${ADSpell[${b}]}].Name}" "${Zone.ShortName}Immune" "${Imm${Spell[${ADSpell[${b}]}].ID}}"
             }
             /multiline ; /varset tSpellDelay 4000;/goto :tContDebuff
           }
           /if (${Select[${Cast.Result},cast_takehold]}) /multiline ; /echo ${ADSpell[${b}]} did not take hold;/varset tSpellDelay 4000;/varset tGem 60;/goto :tContDebuff
           /if (${ADSpellRecast[${b}]} && ${ADSpellRecast[${b}]}>=${tRecast} && (${Select[${Cast.Result},cast_resist,cast_fizzle]} || ${Select[${Cast.Result},cast_interrupted]})) {
/echo recast or interrupted
             /if (${Cast.Result.Equal[cast_resist]} && ${ADSpellCastonResist[${b}].Length} && ${tCastonRes}<1) {
               /call CastCall ${Me.CleanName} "cast ${ADSpellCastonResist[${b}]}"
/echo Casting additional spell - Macro.Return: ${Macro.Return} : 1:${Macro.Return.Arg[1]} - 2:${Macro.Return.Arg[2]} - 3:${Macro.Return.Arg[3]} - 4:${Macro.Return.Arg[4]} - 5:${Macro.Return.Arg[5]}
               /if (${Macro.Return.Arg[1].Equal[cast_success]} && ${Macro.Return.Arg[2].Equal[ad]} && ${Macro.Return.Arg[5]}==${ADMobList[${a}]}) {
|                /echo /docommand ${If[${Spell[${Macro.Return.Arg[4]}].Duration},/varset ADMDL[${a},${Macro.Return.Arg[3]}] ${Math.Calc[${Macro.RunTime}+${Spell[${Macro.Return.Arg[4]}].Duration.TotalSeconds}-5]},/varset ADMDL[${a},${Macro.Return.Arg[3]}] ${Macro.RunTime}]}
                 /if (${Spell[${Macro.Return.Arg[4]}].Duration}) {
                  /varset ADMDL[${a},${Macro.Return.Arg[3]}] ${Math.Calc[${Macro.RunTime}+${Spell[${Macro.Return.Arg[4]}].Duration.TotalSeconds}-5]}
                 } else {
                  /varset ADMDL[${a},${Macro.Return.Arg[3]}] ${Macro.RunTime}
                 }
               }
               /varcalc tCastonRes ${tCastonRes}+1
             }
             /delay 2s ${Cast.Status.Equal[i]}
             /multiline ; /varcalc tRecast ${tRecast}+1;/varset tCastTime ${tCT}+30;/varset ADtTargetID 0;/goto :tCastDebuff
           } else /if (${ADSpellRecast[${b}]} && ${Cast.Result.Equal[cast_resist]} && ${tRecast}>=${ADSpellRecast[${b}]}) {
              /multiline ; /varset tSpellDelay 4000;/goto :tContDebuff
           }
           /if (${Select[${Cast.Result},cast_outofrange,cast_cannotsee,cast_distracted,cast_notarget,CAST_CANCELLED,CAST_NOTREADY]} || !${ADSpellRecast[${b}]} && ${Select[${Cast.Result},cast_resist,cast_fizzle,CAST_NOTREADY,CAST_CANCELLED]}) /multiline ; /echo fizzle or resisted;/goto :tCheckforAdds
           /if (${Select[${Cast.Result},cast_outdoors]}) /multiline ; /bc Outdoor only spell, turning off debuff [ ${ADSpell[${b}]} ];/varset ADTarCnt[${b}] 0;/goto :tNextDebuff
           /if (${Cast.Result.Equal[cast_success]}) /goto :tContDebuff
         }
        } else {
        	/if (!${Cast.Ready[${ADSpell[${b}]}|${ADGem[${b}]}]} && !${Me.Class.ShortName.Equal[BRD]} && ${Spawn[${ADMobList[${a}]}].Type.NotEqual[corpse]}) /delay 2s ${Cast.Ready[${ADSpell[${b}]}|${ADGem[${b}]}]}
        	/if (${Cast.Ready[${ADSpell[${b}]}|${ADGem[${b}]}]} || ${Me.Class.ShortName.Equal[BRD]} && ${Spawn[${ADMobList[${a}]}].Type.NotEqual[corpse]}) {
          /if (${Target.ID}!=${ADMobList[${a}]}) {
            /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
            /if (${Melee.Combat}) /attack off
            /squelch /tar id ${ADMobList[${a}]}
            /delay 1s ${Target.ID}==${ADMobList[${a}]}
          }
          /varset ADtTargetID ${ADMobList[${a}]}
          /varset tCastTime ${tCT}
          /if (${Spell[${ADSpell[${b}]}].SpellType.Equal[Detrimental]} && ${Spell[${ADSpell[${b}]}].Duration} && ${CheckTargetDebuffs} && ${Spell[${ADSpell[${b}]}].TargetType.Equal[single]} && !${Select[${ADTarType[${b}]},2,12]}) {
          	/if (!${Target.ID} || ${Target.ID}!=${ADMobList[${a}]}) {
          		/target id ${ADMobList[${a}]}
          		/delay 2s ${Target.ID}==${ADMobList[${a}]} && ${Target.BuffCount}
          	}
         		/for x 1 to ${Target.BuffCount}
        			/if (!${Spell[${ADSpell[${b}]}].WillStack[${Target.Buff[${x}]}]} && !${OverWrite${Spell[${ADSpell[${b}]}].ID}.Find[|${Target.Buff[${x}].ID}|]} || ${Target.Buff[${x}].ID}==${tSpellID}) {
        				/if (${Target.BuffDuration[${x}].TotalSeconds}>${Spell[${ADSpell[${b}]}].MyCastTime}) {
        					/if (${Target.Buff[${x}].ID}!=${tSpellID}) /echo Skipping debuff ${ADSpell[${b}]}.  Doesn't stack with ${Target.Buff[${x}]} - ID: ${Target.Buff[${x}].ID}
        					/if (${Target.Buff[${x}].ID}==${tSpellID}) /echo ${ADSpell[${b}]} is on Target.
									/multiline ; /varset tSpellDelay ${Math.Calc[${Target.BuffDuration[${x}].TotalSeconds}-${Spell[${ADSpell[${b}]}].MyCastTime}+5]};/varset tGem 60
        					/goto :tContDebuff
        				}
        			}
        		/next x
        	}
          /if (${Me.Class.ShortName.Equal[BRD]}) {
            /if (${tGem}==30 && ${Me.AltAbility[${ADSpell[${b}]}].Spell.CastTime}<1) {
                 /alt act ${AltAbility[${ADSpell[${b}]}].ID}
                 /delay 2
                 /varset tGem 41
                 /goto :tContDebuff
              } else {
                /multiline ; /squelch /twist ${ADGem[${b}]};/varset CastResult PENDING;/varset tCastTime 32
            }
           } else {
            /varset MBCurrentSpell AD|${b}|${ADtTargetID}
            /call mq2cast "${ADSpell[${b}]}" ${If[${Select[${ADGem[${b}]},1,2,3,4,5,6,7,8,9,10,11,12]},gem,]}${ADGem[${b}]} ${tCastTime} CastCheck -targetid|${ADtTargetID} -bandolier|${ADSpellFoci[${b}]}
          }
       }
      }
       /if (${ADtTargetID} && ${Target.ID}==${ADtTargetID} && ${Me.Casting.ID} && ${Spell[${Me.Casting.ID}].SpellType.Equal[detrimental]} && (${Spawn[${ADtTargetID}].Type.Equal[Corpse]} || !${Spawn[${ADtTargetID}].ID})) /multiline ; /if (!${Me.Mount.ID}) /interrupt;/goto :tNextMob
       /if (${interruptFlag}) /goto :tCheckforAdds
      /if (${tCastTime} || ${Cast.Timing}) /goto :tCastDebuff
      /if (${AHHealList.Find[${Cast.Stored}]} && ${Cast.Status.Equal[I]} && ${ADtTargetID} && ${Spawn[${ADtTargetID}].ID} && ${Spawn[${ADtTargetID}].Type.NotEqual[Corpse]}) /multiline ; /varset ADtTargetID 0;/goto :tCastDebuff
       /multiline ; /echo Cast took too long, aborting this cast ${Spell[${tSpellID}].Name};/echo Cast.Stored ${Cast.Stored} - Cast.Status ${Cast.Status} - Cast.Result ${Cast.Result} - TargetID ${ADtTargetID};/goto :tCheckforAdds
 :tContDebuff
       /varset MiscCheckTimer 5s
|       /if (${ADAnnounce[${b}].Length} && (!${Select[${Cast.Result},cast_immune,cast_takehold]} && !${CastResult.Equal[immune]} || ${ADTarCnt}<=1)) /docommand ${If[${ADAnnounce[${b}].Length}>10,/call ParseAnnounce "${ADAnnounce[${b}]}" "${Spawn[${ADtTargetID}].CleanName}" "${ADSpell[${b}]}",/docommand ${ADAnnounce[${b}]} Debuffed: -[ ${Spawn[${ADtTargetID}].CleanName} ]- with ${ADSpell[${b}]}]}
    /if (${HoTTTarget} && ${tGem}<60 && ${ADAnnounce[${b}].Length} && (!${Select[${Cast.Result},cast_immune,cast_takehold]} && !${CastResult.Equal[immune]} || ${ADTarCnt}<=1)) {
      /if (${ADAnnounce[${b}].Length}>10 && ${tGem}<60) {
      /call ParseAnnounce "${ADAnnounce[${b}]}" "${Spawn[${ADtTargetID}].CleanName}" "${ADSpell[${b}]}"
      } else {
       /docommand ${ADAnnounce[${b}]} Debuffed: -[ ${Spawn[${ADtTargetID}].CleanName} ]- with ${ADSpell[${b}]}
       }
      }
       /if (${Spell[${tSpellID}].Duration} && ${tGem}<40 || ${tGem}==60) {
         /docommand ${If[${ADDurMod[${b}]},/varcalc tSpellDuration ((${ADDurMod[${b}]}/100)*${Spell[${tSpellID}].Duration.TotalSeconds})+${Spell[${tSpellID}].Duration.TotalSeconds}-${Spell[${tSpellID}].MyCastTime}-5,/varcalc tSpellDuration ${Spell[${tSpellID}].Duration.TotalSeconds}-${Spell[${tSpellID}].MyCastTime}-5]}
         /varcalc tSpellDelay ${tSpellDelay}+${ADSpellDelay[${b}]}+${tSpellDuration}+${Macro.RunTime}
       } else {
         /varcalc tSpellDelay ${tSpellDelay}+${ADSpellDelay[${b}]}+${Macro.RunTime}-${Spell[${tSpellID}].MyCastTime}-5
       }
       /if ((!${tSpellDelay} || ${tSpellDelay}<${ADSpellDelay[${b}]}) && ${ADSpellDelay[${b}]}) /varset tSpellDelay ${ADSpellDelay[${b}]}+${Macro.RunTime}
      /if (${Spell[${tSpellID}].RecastTime} && (${tSpellDelay}<=${Macro.RunTime} || ${Math.Calc[${tSpellDelay}-${Macro.RunTime}]}<${Spell[${tSpellID}].RecastTime})) /varset tSpellDelay ${Spell[${tSpellID}].RecastTime}
       /varcalc ADMDL[${a},${b}] ${tSpellDelay}
       /if (${d} && ${d}!=999) /varset ADMDL[${a},${d}] ${ADMDL[${a},${b}]}
       /if (${Defined[ADCharmID]} && ${DoPet}) /if (${Me.Pet.ID} && ${ADGem[${b}]}==${ADCharmGem}) /multiline ; /varset ADCharmID ${ADMobList[${a}]};/varset PetAssist 1;/echo Charmed a pet ${ADMobList[${a}]}
       /if (${tGem}<40 && ${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
|       /if (${AdvPlug} && ${AdvPath.Paused}) /squelch /afollow unpause
       /varset MBCurrentSpell
       /doevents
 :tCheckforAdds
       /call ADSpawnCheck
       /if (!${ADMobCount}) /return
       /if  (${AddSpawn}) /multiline ; /echo Add detected starting debuffs;/goto :tADTop
 :tWhatNext
       /if (${ADTarType[${b}]}>9 && ${ADTarType[${b}]}<99 && ${ADTarCnt[${b}]}>0) /goto :tNextMob
 :tNextDebuff
| /if (${a}==${ADMobCount}) /varset a 1
     /next b
   }
 :tNextMob
   /next a
   /if (${ADTarType[${tADCount}]}>9) /multiline ; /varcalc tADCount ${tADCount}+1;/goto :tADCountTop
 :tEnd
 /if (${ADHoldTimer}) /goto :tADTop
 /if (${OffTank} && ${existingTar}) /tar ID ${existingTar}
/return

Sub Event_MezResist(string Line,string SpellName)
|    /echo Mob is immune to ${SpellName}
    /if (!${Defined[EventSpell]}) /declare EventSpell string outer
    /varset EventSpell ${SpellName}
/return

