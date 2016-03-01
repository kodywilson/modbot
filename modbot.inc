
#Event Invite "#*#invites you to join a group.#*#"
|#Event Slain "#*#Returning to Bind Location#*#"
#Event Slain "#*#You have been slain by#*#"
#Event DelayOnZone "#*#You have entered#*#"
#Event DelayOnZone "#*#LOADING, PLEASE WAIT.#*#"
#Event SummonBoth "You are low on food and drink."
#Event SummonBoth "You are out of food and drink."
#Event SummonBoth "You are out of drink and low on food."
#Event SummonBoth "You are out of food and low on drink."
#Event SummonDrink "You are low on drink."
#Event SummonFood "You are low on food."
#Event SummonDrink "You are out of drink."
#Event SummonFood "You are out of food."
#Event CastRst "Your target resisted the#*#"
| #Event CastImm "Your target is immune#*#"
 #Event CastImm "Your target cannot be#*#"
#Event CastStn "You are stunned#*#"
#Event CastStn "You can't cast spells while stunned!#*#"
#Event CastStn "You miss a note#*#"
#Event CharmBroke "Your |${ADCharmSpell}| spell has worn off#*#"
|#event ResetMelee "You cannot see your target."
#Event WornOff        "#*#Your #1# spell has worn off of #2#."
#Event Camping "#*#more seconds to prepare your camp#*#"
#Event GoM "#*#granted a gift of#*#"
|#Event GoRM "#*#granted a gift of radiant mana#*#"
|#Event GoERM "#*#granted a gift of exquisite radiant mana#*#"
#Event OutOfStuff "Sorry, but you don't have everything you need for this recipe in your general inventory."
#Event Broke "#*#you cannot afford#*#"
#Event Broke "#*#you can't afford#*#"
#Event ClickVoid "#*#Click void portal#*#"
#Event Teeky "#*#Get the daily Teek quests#*#"


|  #Event CastLnd "#*#

#Event EQBC "<#1#> #2#"
#Event EQBC "[#1#(msg)] #2#"
#Event EQBC "[MQ2] mb- #2#"

Sub Event_ClickVoid
  /if ( ${Select[${Zone.ShortName},Potimea]} ) {
    /doortarget OBJ_VOIDPORTAL
    /face door
    /moveto loc ${DoorTarget.Y} ${DoorTarget.X}
	/delay 10s
	/click left door
  }
/return

Sub Event_Teeky
  /if ( ${Select[${Zone.ShortName},PoKnowledge]} ) {
	/delay ${Math.Calc[${Math.Rand[45]}+5]}s
	/bc [+g+] Getting Teek dailies
  	/tar Teek
	/delay 2s
	/moveto ID
  	/delay ${Math.Calc[${Math.Rand[10]}+5]}s
    /say level 85
    /delay 2s
    /notify TaskSelectWnd TSEL_AcceptButton leftmouse 1
	/delay 6s
	/say alternate
	/delay 2s
    /notify TaskSelectWnd TSEL_AcceptButton leftmouse 1
	/delay 6s
	/say level 90
	/delay 2s
    /notify TaskSelectWnd TSEL_AcceptButton leftmouse 1
	/delay 6s
	/itemnotify "Fellowship Registration Insignia" rightmouseup
	/delay 6s
  }
/return

Sub Event_EQBC(EQBCSay,EQBCSender,EQBCCommand)
| /echo ${EQBCSay} : ${EQBCSender} : ${EQBCCommand}
	/varset EQBCCommand ${EQBCSay.Right[-${Math.Calc[${EQBCSay.Find[${EQBCCommand}]}-1]}]}
|	/echo ${EQBCCommand}
  /if (!${DoBuffBot} && !${Select[${EQBCSender},${MasterList}]} && ${EQBCSender.Length}) /return
  /if (${Select[${EQBCSender},${MasterList}]} && (${Select[${EQBCCommand.Arg[1]} || ${EQBCCommand.Arg[2]},Casted:,Buffed:,Debuffed:,Healing:,Meleeing:,Selling:,Setting]} || ${EQBCCommand.Find[immune to]})) /return
  /if (!${Defined[EQBCSender]}) /declare EQBCSender string local LOCAL
  /if (${EQBCCommand.Arg[1].Equal[${Me.Name}]}) /varset EQBCCommand ${EQBCCommand.Mid[${Math.Calc[${Me.Name.Length}+1]},${Math.Calc[${EQBCCommand.Length}-${Me.Name.Length}]}]}
  /varset CurrentEQBCCommand ${EQBCCommand}
  /call Event_chat "eqbc" "${EQBCSender}" "${EQBCCommand}"
/return

Sub Event_chat(MsgType,MsgFrom,MsgText)
/if (${MsgType.Equal[eqbc]}) /varset MsgText ${CurrentEQBCCommand}
| /echo Event chat - ${MsgType} ${MsgFrom} ${MsgText}
/declare a int local
/if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:Event_Chat

/if (${MsgType.NotEqual[EQBC]} && ${MsgFrom.Left[1].Compare[ ]}<0) /varset MsgFrom ${MsgFrom.Right[-2].Left[-1]}
   /if (!${DoBuffBot} && !${Select[${MsgFrom},${MasterList},LOCAL]} || ${MsgFrom.Equal[${Me.Name}]}) {
     /if (!${NetBots[${MsgFrom}].ID} && ${MsgType.Equal[TELL]} && ${DoTells} && !${MsgFrom.Equal[${Me.Name}]} && !${MsgFrom.Equal[${Me.Pet.CleanName}]} && !${Spawn[npc ${MsgFrom}].ID}) /call TellEvent ${MsgFrom} "${MsgText}"
     /if (${MsgType.NotEqual[eqbc]} && !${Select[${MsgFrom},${MasterList},LOCAL]}) {
       /for a 1 to ${Math.Calc[${MsgText.Count[ ]}+1]}
          /if (${ABAliasList.Find[${MsgText.Arg[${a}]}]}) /call CastCall ${MsgFrom} "cast ${MsgText.Arg[${a}]}"
        /next a
     }
     /return
  }
   /docommand ${If[${MsgFrom.Equal[local]},/varset SenderName ${Me.Name},/varset SenderName ${MsgFrom}]}
   /if (${DoBuffBot}) /varset MsgText cast ${MsgText}
   /if (${MsgText.Left[5].Equal[abort]}) {
     /if (${ACState}) /multiline ; /varcalc ACState ${ACState}+2;/call AdvCombat
     /if (${Me.Casting.ID}) /multiline ; /squelch /stoptwist;/interrupt;/squelch /stoptwist
     /if (${Me.Mount.ID}) /dismount
     /multiline ; /varset DoMelee FALSE;/varset DoDebuffs FALSE;/varset DoHeals FALSE;/if (!${Me.Class.ShortName.Equal[brd]}) /varset DoBuffs FALSE;/bc Setting - DoHeals, DoBuffs, DoDebuffs & DoMelee FALSE and Following ${SenderName};/varset MsgText follow close
   }
   /if (${MsgText.Left[7].Equal[addbuff]}) {
   	/if (${MsgText.Arg[2].Arg[1,|].Length} && ${MsgText.Arg[2].Arg[2,|].Length}) /varcalc ABCount ${ABCount}+1
   	/ini "${IniFile}" AdvBuff ABCount ${ABCount}
   	/echo ${MsgText.Right[-8]} : ABCount = ${ABCount}
   	/ini "${IniFile}" AdvBuff ABGem${ABCount} ${MsgText.Arg[2].Arg[2,|]}
   	/ini "${IniFile}" AdvBuff ABSpell${ABCount} "${MsgText.Arg[2].Arg[1,|]}"
   	/if (${MsgText.Arg[3].Length}) /ini "${IniFile}" AdvBuff ABSpellAlias${ABCount} "${MsgText.Arg[3]}"
   	/if (${MsgText.Arg[4].Length}) /ini "${IniFile}" AdvBuff ABTarType${ABCount} "${MsgText.Arg[4]}"
   	/bc Restart to add to memory.
    /return
   }  
   /if (${MsgText.Left[7].Equal[DoBuffs]} && !${Me.Class.ShortName.Equal[brd]}) {
      /multiline ; /docommand ${If[${DoBuffs},/varset DoBuffs FALSE,/varset DoBuffs TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoBuffs TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoBuffs FALSE;/bc Setting - DoBuffs ${DoBuffs};/return
   }
   /if (${MsgText.Left[7].Equal[DoSongs]} && ${Me.Class.ShortName.Equal[brd]}) {
      /multiline ; /docommand ${If[${DoBuffs},/varset DoBuffs FALSE,/varset DoBuffs TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoBuffs TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoBuffs FALSE;/bc Setting - DoBuffs ${DoBuffs};/return
   }
   /if (${MsgText.Left[9].Equal[DoDebuffs]}) {
     /multiline ; /docommand ${If[${DoDebuffs},/varset DoDebuffs FALSE,/varset DoDebuffs TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoDebuffs TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoDebuffs FALSE;/bc Setting - DoDebuffs ${DoDebuffs};/return
   }
   /if (${MsgText.Left[7].Equal[DoHeals]}) {
     /multiline ; /docommand ${If[${DoHeals},/varset DoHeals FALSE,/varset DoHeals TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoHeals TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoHeals FALSE;/bc Setting - DoHeals ${DoHeals};/return
   }
   /if (${MsgText.Left[7].Equal[DoCures]}) {
     /multiline ; /docommand ${If[${DoCures},/varset DoCures FALSE,/varset DoCures TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoCures TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoCures FALSE;/bc Setting - DoCures ${DoCures};/return
   }
   /if (${MsgText.Left[8].Equal[DoEvents]}) {
     /multiline ; /docommand ${If[${DoEvents},/varset DoEvents FALSE,/varset DoEvents TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoEvents TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoEvents FALSE;/bc Setting - DoEvents ${DoEvents};/return
   }
   /if (${MsgText.Left[7].Equal[DoMelee]}) {
     /multiline ; /docommand ${If[${DoMelee},/varset DoMelee FALSE,/varset DoMelee TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoMelee TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoMelee FALSE;/bc Setting - DoMelee ${DoMelee};/return
   }
   /if (${MsgText.Left[5].Equal[DoAfk]}) {
     /multiline ; /docommand ${If[${DoAfk},/varset DoAfk FALSE,/varset DoAfk TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoAfk TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoAfk FALSE;/bc Setting - DoAfk ${DoAfk};/return
   }
   /if (${MsgText.Left[5].Equal[DoSit]}) {
     /multiline ; /docommand ${If[${DoSit},/varset DoSit FALSE,/varset DoSit TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoSit TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoSit FALSE;/bc Setting - DoSit ${DoSit};/return
   }
   /if (${MsgText.Left[8].Equal[DoForage]}) {
     /multiline ; /docommand ${If[${DoForage},/varset DoForage FALSE,/varset DoForage TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoForage TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoForage FALSE;/bc Setting - DoForage ${DoForage};/return
   }
   /if (${MsgText.Left[6].Equal[DoLoot]}) {
     /multiline ; /docommand ${If[${DoLoot},/varset DoLoot FALSE,/varset DoLoot TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoLoot TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoLoot FALSE;/bc Setting - DoLoot ${DoLoot};/return
   }
   /if (${MsgText.Left[5].Equal[DoPet]}) {
     /multiline ; /docommand ${If[${DoPet},/varset DoPet FALSE,/varset DoPet TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoPet TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoPet FALSE;/bc Setting - DoPet ${DoPet};/return
   }
   /if (${MsgText.Left[7].Equal[DoMount]}) {
     /multiline ; /docommand ${If[${DoMount},/varset DoMount FALSE,/varset DoMount TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoMount TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoMount FALSE;/bc Setting - DoMount ${DoMount};/return
   }
   /if (${MsgText.Left[4].Equal[DoFW]}) {
     /multiline ; /docommand ${If[${DoFW},/varset DoFW FALSE,/varset DoFW TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoFW TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoFW FALSE;/bc Setting - DoFW ${DoFW};/return
   }
   /if (${MsgText.Left[3].Equal[MBP]} || ${MsgText.Left[7].Equal[MBPause]}) {
     /multiline ; /docommand ${If[${MBPause},/varset MBPause FALSE,/varset MBPause TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset MBPause TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset MBPause FALSE;/bc Setting - MBPause ${MBPause};/if (${MBPause} && ${CampStatus}) /makecamp off
     /if (${MBPause} && ${Me.Class.ShortName.Equal[brd]} && ${Twist}) /twist off
     /return
   }
   /if (${MsgText.Left[8].Equal[MBWayRec]}) {
     /call MBWayRecord ${MsgText.Right[-9]}
     /return
   }
   /if (${MsgText.Left[9].Equal[MBWayPlay]}) {
     /call MBWayPlay ${MsgText.Right[-10]}
     /if  (${Macro.Return.Equal[false]}) /bc Problem with playback
     /return
   }
   /if (${MsgText.Left[9].Equal[DoBuffBot]}) {
      /multiline ; /docommand ${If[${DoBuffBot},/varset DoBuffBot FALSE,/varset DoBuffBot TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoBuffBot TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoBuffBot FALSE;/bc Setting - DoBuffBot ${DoBuffBot};/return
   }
   /if (${MsgText.Left[7].Equal[OffTank]}) {
     /multiline ; /docommand ${If[${OffTank},/varset OffTank FALSE,/varset OffTank TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset OffTank TRUE;/if (${MsgText.Arg[2].Equal[off]} || ${MsgText.Find[false]}) /varset OffTank FALSE;/bc Setting - OffTank ${OffTank};/return
         /docommand ${If[${OffTank},/multiline ;/varset ACMATarget 0;/varset ACTarID 0,]}
   }
   /if (${MsgText.Left[4].Equal[goto]}) /multiline ; /if (!${MsgText.Arg[2].Length} || !${MsgText.Arg[3].Length}) /return;/if (!${Me.Standing}) /stand;/docommand ${If[${Defined[CurrLoc]},/call GotoFunction ${MsgText.Arg[2]} ${MsgText.Arg[3]},/moveto loc ${MsgText.Arg[2]} ${MsgText.Arg[3]}]};/return
   /if (${MsgText.Left[6].Equal[dolist]}) /multiline ; /bc DoBuffs ${DoBuffs};/bc DoDebuffs ${DoDebuffs};/bc DoHeals ${DoHeals};/bc DoEvents ${DoEvents};/bc DoMelee ${DoMelee};/bc DoAfk ${DoAfk};/bc DoSit ${DoSit};/bc DoLoot ${DoLoot};/bc DoForage ${DoForage};/bc DoFW ${DoFW};/return
   /if (${MsgText.Left[4].Equal[mana]}) /multiline ; /bc ${Me.PctMana}%;/return
   /if (${MsgText.Left[8].Equal[letsroll]}) /multiline ; /bc Setting - DoCures,DoBuffs,DoHeals,DoMelee,DoEvents,DoDebuffs TRUE;/varset DoDebuffs TRUE;/varset DoHeals TRUE;/varset DoMelee TRUE;/varset DoEvents TRUE;/varset DoBuffs TRUE;/varset DoCures TRUE;/varset MakeCampD ${Me.Heading.DegreesCCW};/return
   /if (${MsgText.Left[6].Equal[holdup]}) /multiline ; /bc Setting - DoBuffs,DoDebuffs,DoEvents,DoMelee,DoCures,DoLoot,DoPull FALSE;/varset DoDebuffs FALSE;/varset DoEvents FALSE;/varset DoHeals TRUE;/if (!${Me.Class.ShortName.Equal[brd]}) /varset DoBuffs FALSE;/varset DoMelee FALSE;/varset DoCures FALSE;/varset DoPull FALSE;/varset DoLoot FALSE;/return
   /if (${MsgText.Left[6].Equal[buffup]}) /multiline ; /bc Setting - DoBuffs & DoEvents & DoHeals TRUE;/varset DoEvents TRUE;/varset DoHeals TRUE;/varset DoBuffs TRUE;/return
   /if (${MsgText.Left[7].Find[attack]}) {
     /multiline ; /bc Setting - DoMelee TRUE & Attacking;/varset ACState 100;/varset DoMelee TRUE
     /if (!${MsgText.Arg[2].Length}) /varset MsgText ${MsgText} ${Me.CleanName}
     /call AdvCombat ${If[${MsgText.Arg[2].Length},${MsgText.Arg[2]},]} ${If[${MsgText.Arg[2].Length} && ${MsgText.Arg[3].Length},${MsgText.Arg[3]},]} ${If[${MsgText.Arg[2].Length} && ${MsgText.Arg[3].Length} && ${MsgText.Arg[4].Length},${MsgText.Arg[4]},]}
     /return
   }
   /if (${MsgText.Left[4].Equal[stop]}) {
    /if (${Select[${ACState},2,4]}) /varset ACState 1
    /if (${AdvPath.Following} || ${AdvPath.Paused}) {
      /call AdvPlugCall off
      /multiline ; /varset FollowFlag 0;/varset FollowID 0;/varset FollowName
    } else {
      /docommand ${If[${Defined[CurrLoc]},/call StopFunction,/squelch /stick off]}
      /multiline ; /varset FollowFlag 0;/varset ACStartY ${Me.Y};/varset ACStartX ${Me.X}
      /docommand ${If[${CampStatus}==1,/multiline ; /varset MakeCampX ${Me.X};/varset MakeCampY ${Me.Y};/varset MakeCampD ${Me.Heading.DegreesCCW};/makecamp on,/makecamp off]}
    }
    /if (${AdvPlug} && !${AdvPath.Following} || !${AdvPath.Following} && ${Stick.Status.Equal[off]}) /bc No longer on AF
    /return
  }
   /if (${MsgText.Left[8].Equal[makecamp]}) /multiline ; /docommand ${If[${Defined[CurrLoc]},/call StopFunction,/squelch /stick off]};/if (${AdvPath.Following}) /afollow off;/varset FollowFlag 0;/makecamp on;/varset MakeCampX ${Me.X};/varset MakeCampY ${Me.Y};/varset MakeCampD ${Me.Heading.DegreesCCW};/varset CampStatus 1;/bc Hanging out;/return
   /if (${MsgText.Left[5].Equal[cast ]} || (${Me.Class.ShortName.Equal[brd]} && (${MsgText.Left[4].Equal[sing]} || ${MsgText.Left[4].Equal[sung]})) ) {
     /call CastCall "${SenderName}" "${MsgText}"
     /return
   }
   /if (${MsgText.Left[7].Equal[exclude]}) {
     /if (${MsgText.Right[-8].Equal[save]}) {
       /bc Saving Exclude List
       /call ProcessExcludeList save
      } else {
       /bc Excluding ${MsgText.Right[-8]}
       /if (!${ExcludeList.Find[|${MsgText.Right[-8]}|]}) /varset ExcludeList ${ExcludeList}${MsgText.Right[-8]}|
       /call ProcessExcludeList
     }
     /return
   }
   /if (${MsgText.Left[4].Equal[sell]} && !${MsgText.Left[7].Equal[selling]} && (${MsgText.Right[-4].Length} || ${Select[${Target.Class},Merchant]})) {
    /if (!${MsgText.Right[-5].Length} && ${Select[${Target.Class},Merchant]}) {
      /declare MerchName string local ${Target.CleanName}
    } else /if (${MsgText.Right[-5].Length}) {
      /declare MerchName string local ${MsgText.Right[-5]}
    }
     /bc Selling items to ${MerchName}
     /call SellCheck "${MerchName}"
   }
   /if (${MsgText.Left[4].Equal[give]} && ${MsgText.Right[-5].Length}) {
        /bc Giving items to ${MsgText.Arg[2]}
        /call GiveCheck ${MsgText.Right[-5]}
        /return
   }
   /if (${MsgText.Left[7].Equal[include]}) {
     /bc Including ${MsgText.Right[-8]}
     /if (${ExcludeList.Find[${MsgText.Right[-8]}]}) {
       /declare sVarM string local ${MsgText.Right[-8]}|
       /if (${ExcludeList.Find[#${MsgText.Right[-8]}]}) /varset sVarM #${sVarM}
       /declare sVarL string local ${ExcludeList.Mid[1,${Math.Calc[${ExcludeList.Find[${sVarM}]}-1]}]}
       /declare sVarR string local ${ExcludeList.Mid[${Math.Calc[${sVarL.Length}+${sVarM.Length}+1]},${ExcludeList.Length}]}
       /varset ExcludeList ${sVarL}${sVarR}
     }
     /call ProcessExcludeList
     /return
   }
   /if (${MsgText.Left[6].Equal[follow]}) {
     /squelch /makecamp off
     /varset CampStatus 0
     /if (${MoveTo.Moving}) /moveto off
     /if ((${MsgText.Equal[follow me]})||(${MsgText.Equal[follow]})||(${MsgText.Equal[follow close]})) {
       /if (${Defined[CurrLoc]}) /call FollowFunction "${SenderName}"
       /varset FollowID ${Spawn[pc ${SenderName}].ID}
       /if (${AdvPlug} && ${AdvPath.Active} && !${MsgText.Equal[follow close]}) /call AdvPlugCall "${SenderName}"
     } else {
       /if (${Defined[CurrLoc]}) /call FollowFunction "${MsgText.Right[-7]}"
       /varset FollowID ${Spawn[${MsgText.Right[-7]}].ID}
       /if (${AdvPlug} && ${AdvPath.Active}) /call AdvPlugCall ${Spawn[${MsgText.Right[-7]}].CleanName}
     }
     /multiline ; /varset FollowName ${Spawn[${FollowID}].Name};/docommand ${If[!${FollowID} || ${Spawn[${FollowID}].Type.Equal[corpse]},/varset FollowFlag 0,/varset FollowFlag 1]};/call AdvPathCall
     /if (${MsgText.Equal[follow close]} && ${FollowID}) {
     	 /if (${AdvPlug} && ${AdvPath.Active}) /afollow off
       /if (${Target.ID}!=${FollowID}) {
       	/tar id ${FollowID}
       	/delay 5 ${Target.ID}==${FollowID}
       }
       /squelch /stick 5
     }
     /if (${Stick.Status.Equal[on]} || ${AdvPath.Following}) /bc AFing ${Spawn[${FollowID}].CleanName}
     /return
   }
   /if (${MsgText.Left[6].Equal[Script]}) {
    /call MBScript ${MsgText.Right[-7]}
    /return
  }
   /if (${MsgText.Left[7].Equal[campout]}) {
    /if (${AdvPath.Following}) /squelch /afollow off
    /if (${Stick.Status.Equal[on]}) /stick off
    /if (${Me.Casting.ID}) /delay 5s !${Me.Casting.ID}
     /if (${Me.Mount.ID}) /multiline ; /dismount;/delay 1s
     /if (${Me.AltAbilityReady[Suspended Minion]} && ${Me.Pet.ID} && ${PetCast.Arg[3,|].Find[sm]}) {
        /casting "Suspended Minion|alt" -maxtries|5
        /delay 5s !${Me.Pet.ID}
        /docommand ${If[${Me.Pet.ID},/bc hmm pet didn't cache,/bc Cached the pet]}
     }
     /if (${MsgText.Equal[campout char]}) {
        /bc Camping to character select
        /multiline ; /docommand /camp;/delay 3s;/docommand /endmacro
     }
     /if (${MsgText.Right[-7].Length}>3) {
     	/if (${MsgText.Right[-7].Find[trader]}) {
     		/bc Camping and selecting this account's trader to login
     		/call Login "${TraderName}"
     	} else {
       /bc Camping and selecting ${MsgText.Right[-7]} to login
       /call Login ${MsgText.Right[-7]}
      }
       /bc We had a problem Houston, ending mac
       /multiline ; /delay 3s;/docommand /endmacro
     }
     /bc Camping to desktop
     /multiline ; /docommand /camp desktop;/delay 3s;/docommand /endmacro
   }
   /if (${MsgText.Left[6].Equal[dopull]}) {
    /multiline ; /docommand ${If[${DoPull} && !${MsgText.Arg[2].Length},/varset DoPull FALSE,/varset DoPull TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoPull TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoPull FALSE;/bc Setting - DoPull ${DoPull}
    /if (!${DoPull}) /return
    /if (${MsgText.Arg[2].Length} && !${Select[${MsgText.Arg[2]},on,off]}) /multiline ; /varset APPath ${MsgText.Arg[2]};/bc APPath set to [ ${MsgText.Arg[2]} ]
    /if (${TankName.Equal[${Me.Name}]}) /varset ACAssistPct 110
    /if (!${CampStatus}) /mb makecamp
    /return
  }
   /if (${MsgText.Left[7].Equal[DoTells]}) {
     /multiline ; /docommand ${If[${DoTells},/varset DoTells FALSE,/varset DoTells TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoTells TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoTells FALSE;/bc Setting - DoTells ${DoTells};/return
   }
   /if (${MsgText.Left[6].Equal[recast]}) {
    /declare b int local 0
    /declare c int local 0
    /for b 1 to ${ABCount}
      /for c 1 to ${Math.Calc[${ABSpellAlias[${b}].Count[|]}+1]}
        /if (${MsgText.Arg[2].Equal[${ABSpellAlias[${b}].Arg[${c},|]}]}) {
          /if (!${ABRecast[${b}]}) /multiline ; /varset ABRecast[${b}] TRUE;/bc ${ABSpell[${b}]} will now recast when worn off.
        }
      /next c
    /next b
    /return
   }
   /if (${MsgText.Left[9].Equal[inviteall]}) {
    /declare i int local
    /echo Starting modbot and inviting all clients.
    /noparse /bca //if (!${Zone.Name.Equal[The Bazaar]}) /mac modbot
    /delay 15s
    /for i 1 to ${Math.Calc[${NetBots.Counts}]}
      /if (${NetBots.Client.Arg[${i}].NotEqual[${Me.Name}]} && !${Group.Member[${NetBots.Client.Arg[${i}]}]}) /multiline ; /invite ${NetBots.Client.Arg[${i}]};/delay ${Math.Rand[20]}
    /if (${i}<6) /next i
  }
  /if (${MsgText.Left[7].Equal[doquest]}) {
 	  /if (${Select[${MsgText.Arg[2]},on,off,true,false]} || !${MsgText.Arg[2].Length}) {
 		  /multiline ; /docommand ${If[${DoQuest},/varset DoQuest FALSE,/varset DoQuest TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoQuest TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoQuest FALSE
 		  /bc Setting - DoQuest ${DoQuest}
 		}
    /if (${Cursor.ID} && !${Target.ID} && !${Select[${MsgText.Arg[2]},on,off,true,false]} && ${MsgText.Arg[3].Length}) {
    	/declare qName string local
    	/declare i int local
    	/for i 3 to ${Math.Calc[${MsgText.Count[ ]}+1]}
    		/varset qName ${qName} ${MsgText.Arg[${i}]}
    	/next i
       /ini "${LootIni}" ${Cursor.Name.Left[1]} "${Cursor.Name}" "Quest|${MsgText.Arg[2]}|${qName}|"
       /echo Added - "Quest|${MsgText.Arg[2]}|${qName}|" - to loot.ini for ${Cursor.Name}
    }
  }
  /if (${MsgText.Left[9].Equal[docombine]}) {
  	/multiline ; /docommand ${If[${DoCombines},/varset DoCombines FALSE,/varset DoCombines TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset DoCombines TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset DoCombines FALSE;/bc Setting - DoCombines ${DoCombines};/return

  }
  /if (${MsgText.Left[9].Equal[setinivar]}) {
  	/if (!${MsgText.Arg[2].Length} || !${MsgText.Arg[3].Length}) /multiline ; /echo Usage: /mb setinivar <Varname> <Setting>  (use quotes around multiple word settings);/return
		/declare IniSection string local
		/declare b int local
		/declare IniVar string local ${MsgText.Arg[2]}
		/declare IniSetting string local ${MsgText.Arg[3]}
		/declare VarNumber int local
		/if (${IniVar.Right[1].Equal[]]} && ${IniVar.Find[[]}) {
			/varset VarNumber ${MsgText.Arg[2].Arg[2,[].Left[-1]}
			/varset IniVar ${MsgText.Arg[2].Arg[1,[]}${VarNumber}
| 			/echo ${VarNumber} and ${IniVar}
		}
		/if (${MsgText.Arg[4].Length}) {
			/for b 4 to ${Math.Calc[${MsgText.Count[ ]}+1]}
				/varset IniSetting ${IniSetting} ${MsgText.Arg[${b}]}
			/next b
		}
  	/for a 1 to ${Ini["${IniFile}"].Count[|]}
  		/varset IniSection ${Ini["${IniFile}"].Arg[${a},|]}
  		/if (!${IniSection.Length}) /return
  		/echo ${IniFile},${IniSection}
  		/for b 1 to ${Ini["${IniFile}",${IniSection}].Count[|]}+1
|  			/if (!${Ini["${IniFile}",${IniSection}].Arg[${b},|].Find[${IniVar}]}) /next a
|  			/echo ${Ini["${IniFile}",${IniSection}].Arg[${b},|]}
| /echo  			/if (${Ini["${IniFile}",${IniSection}].Arg[${b},|].Equal[${IniVar}]}) {
  			/if (${Ini["${IniFile}",${IniSection}].Arg[${b},|].Equal[${IniVar}]}) {
  				/ini "${IniFile}" "${IniSection}" "${IniVar}" "${IniSetting}"
  				/echo You're setting for ${MsgText.Arg[2]} is now ${Ini["${IniFile}",${IniSection},${IniVar}]} in your .ini file.
  				/if (${Defined[${MsgText.Arg[2]}]}) {
  					/varset ${MsgText.Arg[2]} ${IniSetting}
  				} else {
  					/varset IniVar ${MsgText.Arg[2].Arg[1,[]}
  					/if (!${Defined[${IniVar}]}) /multiline ; /echo Cannot set variable in memory.  Defined variable does not match. -Restart required to set it.;/return
  					/varset ${MsgText.Arg[2]} ${IniSetting}
  				}
  				/goto :IniVarDone
  			}
  		/next b
  	/next a
  	/echo Variable not found
:IniVarDone
  	/return
  }
  /if (${MsgText.Left[6].Equal[handin]}) {
  	/declare tTarget string local ${Target.CleanName}
  	/declare cItem string local ${Cursor.Name}
  	/if (${MsgText.Arg[2].Length}) /varset tTarget ${MsgText.Arg[2]}
  	/if (${MsgText.Arg[3].Length}) /varset cItem ${MsgText.Arg[3]}
  	/if (!${tTarget.Length} || !${cItem.Length}) /echo Incorrect usage.  /mb handin "TargetName" "ItemName" or hold item and target..
		/echo Handing in all ${Cursor.Name}'s to ${Target.CleanName}
		/call GiveCheck "${tTarget}" "${cItem}" FALSE ${Math.Calc[${FindItemCount[=${Cursor.Name}]}+${Cursor.Stack}]}
		/return
  }
  /if (${MsgText.Left[7].Equal[getbank]}) {
  	/if (!${MsgText.Right[-7].Length} && !${Cursor.ID}) /multiline ; /echo Usage - Hold Item to get on Cursor or getbank <ItemName> <InStacks> <Quantity>
  	/if (${Cursor.ID}) {
  		/call GetBankItem "${Cursor.Name}" ${MsgText.Right[-7]}
  	} else {
  		/call GetBankItem ${MsgText.Right[-7]}
  	}
|  	Sub GetBankItem(ItemName,bool InStacks,int Quant)
  	/return
  }
  /if (${MsgText.Left[9].Equal[PetShrink]}) {
     /multiline ; /docommand ${If[${PetShrink},/varset PetShrink FALSE,/varset PetShrink TRUE]};/if (${MsgText.Find[on]} || ${MsgText.Find[true]}) /varset PetShrink TRUE;/if (${MsgText.Find[off]} || ${MsgText.Find[false]}) /varset PetShrink FALSE;/bc Setting - PetShrink ${PetShrink};/return
	}  
/return

Sub LoadINIVar(IniSection,IniVar,IniValue,MacroVar,MyIni,VarType)
| /echo LoadIniVar - MacroVar == ${MacroVar}  File : ${MyIni} VarType: ${VarType}
  /if (!${Defined[${MacroVar}]} && ${Defined[VarType]}) /declare ${MacroVar} ${VarType} outer
  /declare IniString string local ${Ini["${MyIni}",${IniSection},${IniVar},NOTFOUND]}
  /varset ${MacroVar} ${IniString}
  /if (${IniString.Equal["NOTFOUND"]}) {
    /varset ${MacroVar} ${IniValue}
    /ini "${MyIni}" "${IniSection}" "${IniVar}" "${${MacroVar}}"
  }
/return

Sub Event_CastRst
  /varset CastResult RESISTED
/return
Sub Event_CastStn
  /varset CastResult STUNNED
/return
Sub Event_CastImm
  /if (${Line.Find[to the stun portion]}) /return
  /varset CastResult IMMUNE
/return

Sub Event_CharmBroke
/bc LOST PET - LOST PET
  /if (!${Defined[ADCharmID]} || !${DoPet}) /return
  /if (${PetAssist}) /varset PetAssist 1
  /if (!${Me.Mount.ID} && ${Me.Sitting}) /sit off
  /varset ADCheckTimer 0
  /call ADDebuffCheck
/return

Sub PMACheck(PMA)
/declare i int local
/if (${MBPause} || !${InvSlot[pack1].Item.ID} && !${InvSlot[primary].Item.ID} || ${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Find[mount|]} && ${PMA.Equal[m]}) /return
/if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:PMACheck:${PMA}
/if (!${Defined[PetCastSM]}) /declare PetCastSM bool outer FALSE
  /declare WaitTimer timer local
  /if (${PMA.Equal[m]}&&!${Me.Mount.ID}&&!${Me.FreeBuffSlots}) /return
 /if (${PMA.Equal[NULL]}) /return FALSE
 /declare PMACast string local ${If[${PMA.Equal[p]},${PetCast},${If[${PMA.Equal[m]},${MountCast},${If[${PMA.Equal[a]},${AuraCast},]}]}]}
 /if (${PMACast.Length}<5) /return FALSE
 /declare tPSpellID int local
 /declare sGem string local ${PMACast.Arg[2,|]}
 /if (${sGem.Find[-invis]}) {
     /if (${Me.Invis}) /return
     /varset sGem ${PMACast.Arg[2,|].Arg[1]}
 }
 /declare tGem int local ${If[${Select[${PMACast.Arg[2,|].Mid[4,${Math.Calc[${sGem.Length}-3]}]},1,2,3,4,5,6,7,8,9,10,11,12]},${PMACast.Arg[2,|].Mid[4,${Math.Calc[${sGem.Length}-3]}]},0]}
 /declare tSpellID int local ${If[${tGem},${Spell[${PMACast.Arg[1,|]}].ID},${If[${sGem.Equal[item]},${FindItem[=${PMACast.Arg[1,|]}].Spell.ID},${If[${sGem.Equal[alt]},${Me.AltAbility[${PMACast.Arg[1,|]}].Spell.ID},0]}]}]}
 /if (${PMA.Equal[p]} && ${PMACast.Arg[3,|].Find[sm]} && ${Me.AltAbilityReady[Suspended Minion]} && !${PetCastSM}) {
   /varset PetCastSM TRUE
   /casting "Suspended Minion|alt" -maxtries|5
   /delay 10s ${Me.Pet.ID}
   /if (${Me.Pet.ID}) /goto :tJump
   /if (!${Me.Pet.ID}) /multiline ; /bc Problem using minion AA, will attempt to create a new pet;/varset PMACast ${PMACast.Arg[1,|]}|${PMACast.Arg[2,|]}
 }
 /if (${PMACast.Arg[2,|].Equal[disc]} && ${PMA.Equal[a]} && !${Melee.Combat}) /multiline ; /disc ${PMACast.Arg[1,|]};/delay 10 ${Cast.Ready};/return
 /if (!${tSpellID} || !${sGem.Length}) /return
 /if (${tGem} && ${Me.CurrentMana}<${Spell[${tSpellID}].Mana}) /return
 /bc ${If[${PMA.Equal[p]},Making Pet,${If[${PMA.Equal[m]},Getting Mount,${If[${PMA.Equal[a]},Casting Aura,]}]}]}, could be a minute, please wait...
 /if (${Me.Casting.ID}) /delay 3s !${Me.Casting.ID}
 /if (${tGem} && ${Me.Gem[${tGem}].ID}!=${tSpellID}) /multiline ; /varset tPSpellID ${Spell[${Me.Gem[${tGem}]}].ID};/memorize "${PMACast.Arg[1,|]}|gem${tGem}"
 /if (${PMACast.Arg[2,|].Find[gem]} && !${Me.Gem[${PMACast.Arg[1,|]}]}) /delay 4s ${Me.Gem[${PMACast.Arg[1,|]}]}
 /varset WaitTimer 250
:waitloop
 /if (${MBPause} || ${Me.State.Equal[feign]}) /return
 /if (${PMACast.Arg[2,|].Find[gem]} && !${Me.Gem[${PMACast.Arg[1,|]}]}) /return
 /if (${DoHeals} && ${AHCount}) /call AHHealCheck
 /doevents
 /if ((!${Spawn[${NetBots[${TankName}].TargetID}].Type.Equal[npc]} && ${ACState} || ${isNPC[${NetBots[${TankName}].TargetID}]} && !${ACState})) /call AdvCombat
 /if (${DoDebuffs} && ${ADCount} && !${ADCheckTimer} && (${ADMobCount}||${APTargetID})) /call ADDebuffCheck
 /if (${WaitTimer} && (!${Cast.Ready[${PMACast.Arg[1,|]}|${PMACast.Arg[2,|]}]} && !${Me.Class.ShortName.Equal[brd]} || ${Me.Class.ShortName.Equal[brd]} && !${Me.Gem[${PMACast.Arg[1,|]}]})) /goto :waitloop
:DoBard
  /if (${Select[${Me.Class.ShortName},brd]} && ${PMA.Equal[a]}) {
    /declare CurrentTwist string local ${Twist.List}
    /squelch /twist off
    /delay 2s !${Me.Casting.ID}
    /if (${Me.Casting.ID}) /stopsong
    /if (${Me.State.Equal[sit]}) /stand
    /squelch /twist ${tGem}
    /delay 5s ${Me.Casting.ID}==${Spell[${PMACast.Arg[1,|]}].ID}
|    /echo Should twist

    /delay 5s ${Me.Casting.ID}==${Spell[${PMACast.Arg[1,|]}].ID} || ${Twist.Current}==${tGem}
    /delay 10s ${Me.Aura.Length}
    /squelch /twist ${CurrentTwist}
    /return
  }
  /varset MBCurrentSpell PM|1|${Me.ID}
  /if (${Cast.Ready}) /call MQ2Cast "${PMACast.Arg[1,|]}" ${sGem} 10s CastCheck -targetid|${Me.ID} -maxtries|2 ${If[${PMA.Equal[p]} && ${PetFoci.Length}>2,-bandolier|${PetFoci},]}
 /delay 30s ${If[${PMA.Equal[p]},${Me.Pet.ID},${If[${PMA.Equal[m]},${Me.Mount.ID},${If[${PMA.Equal[a]},${Me.Aura.Length},0]}]}]} || ${PMA.Equal[m]} && !${Me.Casting.ID} && ${Cast.Result.Equal[CAST_OUTDOORS]}
:tJump
 /if (${Cast.Result.Equal[CAST_OUTDOORS]} && !${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Find[mount|]} && ${PMA.Equal[m]}) /ini "${MBCommon}" RestrictedList ${Zone.ID} ${If[${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Length},${Ini["${MBCommon}",RestrictedList,${Zone.ID}]},]}mount|
 /if (!${Me.Pet.ID} && ${PMA.Equal[p]} || !${Me.Mount.ID} && ${PMA.Equal[m]} || !${Me.Aura[1].Length} && !${Me.Aura[2].Length} && ${PMA.Equal[q]}) /multiline ; /bc Must have been a problem with ${If[${PMA.Equal[p]},making the pet,${If[${PMA.Equal[m]},getting the mount,${If[${PMA.Equal[a]},casting my aura,]}]}]}, setting it to FALSE;/docommand ${If[${PMA.Equal[p]},/varset DoPet FALSE,${If[${PMA.Equal[m]},/varset DoMount FALSE,${If[${PMA.Equal[a]},/varset DoAura FALSE,/echo PMACheck problem]}]}]}
 /if (${PMA.Equal[p]} && ${Me.Pet.ID}) {
   /if (${Select[${Me.Class.ShortName},wiz]}) /multiline ; /bc Being a Wizard, I'm dumping pet, setting DoPet FALSE;/pet get lost;/varset DoPet FALSE
   /if (${Me.Pet.ID} && !${PetAggro}) /pet taunt off
 }
 /varset MBCurrentSpell
 /if (${tPSpellID}) {
   /delay 3s ${Cast.Status.Equal[i]}
   /memorize "${Spell[${tPSpellID}]}|gem${tGem}"
   /delay 5s ${Me.Gem[${tGem}].ID}==${tPSpellID}
 }
/return

Sub AFK
 /docommand ${If[(${AFKMessage.Length} && !${AFKMessage.Equal[AFK Message]}),/afk ${AFKMessage},/afk]}
/return

Sub Event_Broke
   /if (${Defined[Broke]}) /varset Broke TRUE
   /echo Error: You are out of money!
   /docommand /${BeepCommand}
/return TRUE

Sub Event_SummonBoth
/if (!${DoFW} || !${InvSlot[pack1].Item.ID} && !${InvSlot[primary].Item.ID} || (${Me.Invis} && !${Me.Class.ShortName.Equal[rog]})) /return
/call Event_SummonFood
/autoinv
/call Event_SummonDrink
/autoinv
/return

Sub Event_SummonDrink
/if (!${DoFW} || ${SumDrink.Length}<3 || ${MBPause} || !${InvSlot[pack1].Item.ID} && !${InvSlot[primary].Item.ID} || (${Me.Invis} && !${Me.Class.ShortName.Equal[rog]})) /return
/if (${Me.Casting.ID}) /delay 5s !${Me.Casting.ID}
/call MQ2Cast "${SumDrink.Arg[1,|]}" ${SumDrink.Arg[2,|]} 5s CastCheck
/delay 10s ${Cursor.ID}
/call ClearCursor
/return

Sub Event_SummonFood
/if (!${DoFW} || ${SumFood.Length}<3 || ${MBPause} || !${InvSlot[pack1].Item.ID} && !${InvSlot[primary].Item.ID} || (${Me.Invis} && !${Me.Class.ShortName.Equal[rog]})) /return
/if (${Me.Casting.ID}) /delay 5s !${Me.Casting.ID}
/call MQ2Cast "${SumFood.Arg[1,|]}" ${SumFood.Arg[2,|]} 5s CastCheck
/delay 10s ${Cursor.ID}
/call ClearCursor
/return

Sub Event_Invite(EvtText)
/if (${Select[${EvtText.Arg[1]},${MasterList}]} || ${NetBots[${EvtText.Arg[1]}].ID}) {
  /if (${Me.Casting.ID}) /delay 10s !${Me.Casting.ID}
  /invite
  /notify ConfirmationDialogBox CD_Yes_Button leftmouseup
}
/return

Sub Event_DelayOnZone
/echo Zoning Detected.
/declare a int local
/declare b int local
/varset DoDebuffs FALSE
/varset DoMelee FALSE
/varset DoPull FALSE
/if (${FollowFlag}) {
  /varset FollowFlag 10
  /if (${Defined[CurrLoc]}) {
    /call StopFunction
    } else {
    /stick off
  }
}
/if (${Select[${MakeCamp},on]} || ${CampStatus}) /multiline ; /makecamp off;/varset CampStatus 0
/if (${Defined[ADCharmID]}) /if (${ADCharmID}) /varset ADCharmID 0
/if (${PetAssist}) /varset PetAssist 1
/if (${Melee.Combat}) /melee reset
/if (${Defined[ADMobTimer]}) {
  /for a 1 to ${ADMobCount}
    /varset ADMobList[${a}] 0
    /for b 1 to ${ADCount}
      /varset ADMDL[${a},${b}] 0
    /next b
  /next a
  /varset ADMobCount 0
  /varset ACMATarget 0
  /varset ACState 0
}
/delay 600s ${Me.ID}
/delay 5s
/if (${DoAfk} && !${Me.AFK}) /call AFK
/multiline ; /varset ACStartY ${Me.Y};/varset ACStartX ${Me.X};/varset ACState 0;/varset ACMATarget 0;/varset ADMobCount 0
| /echo Calling exclude list
/call ProcessExcludeList zone
/varset ZoneName ${Zone.Name}
/if (${ADCount}) {
	/for a 1 to ${ADCount}
		/if (!${Ini["${MBCommon}","${Spell[${ADSpell[${a}]}].Name}","${Zone.ShortName}Immune"].Find[|]} && ${Imm${Spell[${ADSpell[${a}]}].ID}.Length}) /varset Imm${Spell[${ADSpell[${a}]}].ID}
		/if (${Ini["${MBCommon}","${Spell[${ADSpell[${a}]}].Name}","${Zone.ShortName}Immune"].Find[|]}) /call LoadINIVar "${Spell[${ADSpell[${a}]}].Name}" "${Zone.ShortName}Immune" " " "Imm${Spell[${ADSpell[${a}]}].ID}" "${MBCommon}" string
		 
	/next a
}
/doevents flush
/squelch /alert clear 25
/return

Sub Event_Slain
/bc - DEATH DETECTED, putting all functions on hold. -
/bc - You must issue the appropriate command to restart each function or restart. -
/multiline ; /varset DoDebuffs FALSE ;/varset DoEvents FALSE;/varset DoHeals FALSE;/varset DoMelee FALSE;/varset DoBuffs FALSE;/varset DoCures FALSE;/varset DoPull FALSE
/consent group
/delay 3s
/consent raid
/delay 3s
/consent guild
/stick off
/varset FollowFlag 0
/keypress left
/keypress right
/call Wait4Rez
/delay 600s ${Me.ID}
/delay 6s
/delay 20s ${Cast.Status.Equal[i]}
/return

Sub ClearCursor
/if (${Window[InventoryWindow].Open} && !${DoCombines}) /return
/declare x int local
/declare tTime int local ${Macro.RunTime}
:auto_inv
/if (${Cursor.ID}) {
  /if (${Cursor.Container}) {
    /for x 1 to 8
    /if (!${InvSlot[pack${x}].Item.ID}) /nomodkey /itemnotify pack${x} leftmouseup
    /next x
    /if (${Cursor.Container}) /return
  } else {
    /call ItemSort
  }
  /if (${Macro.RunTime}<${tTime}+10) /goto :auto_inv
}
/return

Sub Buy(string itemToBuy, int amount, string vendor)
/if (!${Defined[Broke]}) /declare Broke bool outer FALSE
/if (!${Defined[vendor]} || !${Defined[itemToBuy]} || !${Defined[amount]} || ${Broke}) /return FALSE
/echo Buying ${itemToBuy} from ${vendor}!
/declare QTY int local
/declare l2 int local
/tar npc ${vendor}
/delay 1s
/nomodkey /click right target
/delay 5s
/varset QTY ${Math.Calc[${amount}-${FindItemCount[=${itemToBuy}]}]}
/delay 1s
/if (${FindItemCount[=${itemToBuy}]}>= ${amount}) {
     /echo Done!
     /return TRUE
}
/if (!${Window[MerchantWnd].Open}) /return FALSE
/varset l2 ${Window[MerchantWnd].Child[itemList].List[=${itemToBuy},2]}
/if (!${l2}) {
        /echo couldn't find ${itemToBuy}
        /return FALSE
    } else {
        /notify MerchantWnd itemList listselect ${l2}
        /notify MerchantWnd itemList leftmouse ${l2}
        /delay 2
}
/echo Buying ${itemToBuy} Till I get ${amount}
:Loop
   /if (${Broke}) /return FALSE
   /if (${QTY}>19) {
       /buyitem 20
       /delay 1s
       /doevents
       /varset QTY ${Math.Calc[${amount}-${FindItemCount[=${itemToBuy}]}]}
       /goto :Loop
       } ELSE {
   /if (${QTY}>0 && ${QTY}<20) {
       /buyitem ${QTY}
       /delay 1s
       /doevents
       /varset QTY ${Math.Calc[${amount}-${FindItemCount[=${itemToBuy}]}]}
       }
}
/if (${QTY}==0) {
       /nomodkey /notify MerchantWnd MW_Done_Button leftmouseup
       /echo Done!
       /return TRUE
       }
/goto :Loop
/return TRUE

Sub Sell(ItemToSell)
 /declare OldMoney int local
 /call ClearCursor
:tFinding 
 /if (${ItemToSell.Equal[NULL]} || !${ItemToSell.Length} || !${Window[MerchantWnd].Open}) /return
  /if (${FindItemCount[=${ItemToSell}]} ) {
    /nomodkey /itemnotify ${FindItem[=${ItemToSell}].InvSlot} leftmouseup
    /if (${SelectedItem.ID}==${FindItem[=${ItemToSell}].ID} && ${SelectedItem.Value}) {
      /delay ${Math.Calc[${Math.Rand[3]}+1]}s
      /varset OldMoney ${Me.Cash}
      /nomodkey /shift /notify MerchantWnd MW_Sell_Button leftmouseup
      /delay 2s (${Me.Cash}!=${OldMoney})
    }
    /goto :tFinding
  }
/return

Sub SellCheck(VendorName)
 /call ClearCursor
 /if (${VendorName.Equal[NULL]}) /return FALSE
 /squelch /target ${VendorName}
 /delay 3s ${Target.Name.Find[${VendorName}]} || ${Target.CleanName.Find[${VendorName}]}
 /if (!${Target.Name.Find[${VendorName}]} && !${Target.CleanName.Find[${VendorName}]} || ${Spawn[${Target.ID}].Distance3D}>30) /return FALSE
 /echo selling
 /squelch /face ${VendorName}
 /click right target
 /delay 2s ${Window[MerchantWnd].Open}
 /declare tPack int local
 /declare tItem int local
 /declare mItem int local
 /declare mItemName string Local
 /declare sItemName string local
  /nomodkey /keypress OPEN_INV_BAGS
 :loop
  /if (${Window[TradeSkillWnd].Open}) /notify TradeSkillWnd ExperimentButton leftmouseup
  /delay 4 !${Window[TradeSkillWnd].Open}
  /if (${Window[TradeSkillWnd].Open}) /goto :loop
 /varset mItem ${Merchant.Items}
 /delay 2s
 /if (${Merchant.Items}!=${mItem}) /goto :loop
 /for tPack 1 to 10
   /for tItem 1 to 40
     /docommand ${If[${InvSlot[pack${tPack}].Item.Container},/varset sItemName ${InvSlot[pack${tPack}].Item.Item[${tItem}]},${If[(${tItem}>1 || !${InvSlot[pack${tPack}].Item.ID}),/varset sItemName NULL,/varset sItemName ${InvSlot[pack${tPack}].Item}]}]}
     /if (${sItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${sItemName.Left[1]}","${sItemName}"].Equal[Sell]}) /call Sell "${sItemName}"
   /next tItem
   /if (${Window[Pack${tPack}].Open}) /nomodkey /itemnotify pack${tPack} rightmouseup
   /delay 3s !${Window[Pack${tPack}].Open}
 /next tPack
|    /for mItem 1 to ${Merchant.Items}
|      /varset mItemName ${Merchant.Item[${mItem}].Name}
|      /if (${Ini["${LootIni}","${mItemName.Left[1]}","${mItemName}"].EqualCS[KEEP]}) /bc ${mItemName} found on ${VendorName}
|    /next mItem
 /nomodkey /notify MerchantWnd MW_Done_Button leftmouseup
 /squelch /target clear
/return TRUE

Sub GetBankItem(ItemName,bool InStacks,int Quant)
/if (!${ItemName.Length} || !${Window[BankWnd].Open} && !${Window[BigBankWnd].Open}) /multiline ; /echo Error! Usage - Bank Window must be Open - getbank <ItemName> <Quantity> <InStacks>;/return
/if (!${Defined[InStacks]}) /declare InStacks bool local TRUE
:CursorCheck
/if (${Cursor.ID}) /autoinv
/if (${Cursor.ID}) /goto :CursorCheck
/declare BankSlots int local
/declare i int local
/declare j int local
/declare Gathered int local
  /if (${Window[BankWnd].Open}) {    
    /varset BankSlots 16 
  } else { 
    /if (${Window[BigBankWnd].Open}) { 
      /varset BankSlots 24 
  	}
  }
  /if (${Category.Find[${ItemName}]}) /echo ${ItemName} is a Category
/if (!${FindItemBankCount[=${ItemName}]} && !${Category.Find[${ItemName}]}) /multiline ; /echo Can't Find ${ItemName} in Bank.;/return
  /call OpenAllBankContainers ${BankSlots}
	/for i 1 to ${BankSlots}
		/for j 1 to ${Me.Bank[${i}].Container}
			/if ((${Me.Bank[${i}].Item[${j}].Name.Equal[${ItemName}]} || ${Category.Find[${ItemName}]} && ${Ini["${LootIni}",${Me.Bank[${i}].Item[${j}].Name.Left[1]},${Me.Bank[${i}].Item[${j}].Name}].Find[${ItemName}]}) && !${FindItem[=${Me.Bank[${i}].Item[${j}].Name}].FreeStack} && !${Me.FreeInventory}) /echo No Room for ${Me.Bank[${i}].Item[${j}].Stack} - ${Me.Bank[${i}].Item[${j}].Name}
			/if ((${Me.Bank[${i}].Item[${j}].Name.Equal[${ItemName}]} || ${Category.Find[${ItemName}]} && ${Ini["${LootIni}",${Me.Bank[${i}].Item[${j}].Name.Left[1]},${Me.Bank[${i}].Item[${j}].Name}].Find[${ItemName}]}) && (${FindItem[=${Me.Bank[${i}].Item[${j}].Name}].FreeStack} || ${Me.FreeInventory})) {
				/echo Getting ${Me.Bank[${i}].Item[${j}].Stack} - ${Me.Bank[${i}].Item[${j}].Name} From Bank
			 	/if (${InStacks}) {
			 		/shiftkey /itemnotify in bank${i} ${j} leftmouseup
			 	} else {
			 		/ctrlkey /itemnotify in bank${i} ${j} leftmouseup
			 	}
			  /delay 2s ${Cursor.ID}		 		
  			/call ClearCursor
:CursorClear
				/if (${Cursor.ID}) /autoinv
				/if (${Cursor.ID}) /goto :CursorClear
				/varset Gathered Gathered+1
				/if (${Defined[Quant]}) /if (${Gathered}>=${Quant}) /multiline ; /echo Got ${Gathered} - ${ItemName}'s from Bank.;/return
			}
		/next j
	/next i
	/echo Done getting Items  with names matching ${ItemName} from Bank.
/return

Sub OpenAllBankContainers(int BankSlots)
   /declare i int local 1 
   /for i 1 to ${BankSlots} 
      /if (${Int[${Me.Bank[${i}].Container}]}) { 
         /if (!${Window[bank${i}].Open}) { 
            /nomodkey /itemnotify bank${i} rightmouseup 
            /delay 5s ${Window[bank${i}].Open} 
         } 
         /if (!${Window[bank${i}].Open}) /echo ERROR -- Possible problem opening bank container at # ${i}. 
         /delay 2 
      } 
   /next i 
/return 1 

Sub GiveCheck(TargetName,ItemNames,InStacks,ItemCount)
| /echo ${TargetName} : ${ItemNames} : ${InStacks} : ${ItemCount}
|  /echo /if (${TargetName.Equal[NULL]} || !${Spawn[${TargetName}].ID} || !${ItemNames.Length}) /return FALSE
  /if (${TargetName.Equal[NULL]} || !${Spawn[${TargetName}].ID} || !${ItemNames.Length}) /return FALSE
  /if (!${Defined[InStacks]}) /declare InStacks bool local FALSE
  /declare tID int local ${Spawn[${TargetName}].ID}
  /declare tItemID int local
  /declare tItem string local
  /declare a int local
  /declare b int local
  /declare NumItems int local 1
  /declare x int local ${Me.X}
  /declare y int local ${Me.Y}
  /declare tGC bool local FALSE
  /declare PackNum int local 1
	/declare SlotNum int local 1
	/declare iCount int local
  /call ClearCursor
  :Cursorloop
  /if (${Cursor.ID}) /autoinv
  /if (${Cursor.ID}) /goto :CursorLoop
  /if (${Spawn[${TargetName}].Type.Equal[npc]}) /varset InStacks FALSE
  /if (${Target.ID}!=${tID}) {
      /squelch /target id ${tID}
      /delay 1s ${Target.ID}==${tID}
  }
  /if (!${Target.ID}) /goto :tEnd
  /if (${Target.Distance3D}>12) {
    /call MBMoveTo ${Target.Y} ${Target.X}
    /if (!${Macro.Return}) /multiline ; /echo problem getting to ${TargetName}, Sub GiveCheck;/goto :tEnd
  }
 | /for a 1 to 8
 |    /if (${InvSlot[pack${a}].Item.Container}) {
 |      /if (!${Window[Pack${a}].Open}) /nomodkey /itemnotify pack${a} rightmouseup
 |      /delay 3s ${Window[Pack${a}].Open}
 |    }
 | /next a
/keypress OPEN_INV_BAGS
/delay 1s ${Window[Pack8].Open}
 /for a 1 to ${Math.Calc[${ItemNames.Count[|]}+1]}
 	/if (${ItemCount.Arg[${a},|].Find[all]}) {
 		/varset NumItems ${FindItemCount[=${ItemNames.Arg[${a},|]}]}
 	}
 	/if (${ItemCount.Arg[${a},|].Length} && !${ItemCount.Arg[${a},|].Find[all]}) /varset NumItems ${ItemCount.Arg[${a},|]}
 	/for b 1 to ${NumItems}
   /if (${Category.Find[${ItemNames.Arg[${a},|]}]}) {
| Search for items marked as Category in your loot.ini file...
		/echo Searching inventory for items marked ${ItemNames.Arg[${a},|]} in your loot file.
		/for PackNum 1 to 8
			/for SlotNum 0 to ${InvSlot[pack${PackNum}].Item.Container}
			  /if (!${Ini["${LootIni}",${InvSlot[pack${PackNum}].Item.Item[${SlotNum}].Name.Left[1]},"${InvSlot[pack${PackNum}].Item.Item[${SlotNum}].Name}"].Find[${ItemNames.Arg[${a},|]}]}) {
					/next SlotNum
			  	/next PackNum
			  	/goto :tEnd
			  } else /if (${Ini["${LootIni}",${InvSlot[pack${PackNum}].Item.Item[${SlotNum}].Name.Left[1]},"${InvSlot[pack${PackNum}].Item.Item[${SlotNum}].Name}"].Find[${ItemNames.Arg[${a},|]}]}) {
			  	/echo Found matching item at pack ${PackNum} slot ${SlotNum}
			  	/multiline ; /varset tItemID ${InvSlot[pack${PackNum}].Item.Item[${SlotNum}].ID};/varset tItem ${InvSlot[pack${PackNum}].Item.Item[${SlotNum}].Name}
			  	/echo Item is ${tItemID} : ${tItem}
			  }
		} else {
 			/multiline ; /varset tItemID ${FindItem[=${ItemNames.Arg[${a},|]}].ID};/varset tItem ${FindItem[=${ItemNames.Arg[${a},|]}].Name}
 		}
   /if (!${tItemID}) /multiline ; /echo Can't find ${ItemNames.Arg[${a},|]} to give;/goto :tNextItem
   /varset MiscCheckTimer 15s
:loop
    /if (!${MiscCheckTimer}) /multiline ; /echo Problem with GiveCheck - MiscCheckTimer;/goto :tEnd
|    /if (!${tItemID}) /goto :tEnd
    /if (${InStacks} && ${FindItem[=${tItem}].InvSlot}) {
      /nomodkey /shift /itemnotify ${FindItem[=${tItem}].InvSlot} leftmouseup
      } else /if (!${InStacks} && ${FindItem[=${tItem}].InvSlot}) {
      /nomodkey /ctrl /itemnotify ${FindItem[=${tItem}].InvSlot} leftmouseup
    }
    /delay 2s ${Cursor.ID}==${tItemID}
    /if (${Cursor.ID}!=${tItemID}) /goto :loop
    /if (${Target.ID}) /click left target
    /delay 10s (${Window[GiveWnd].Open} || ${Window[TradeWnd].Open})
    /if (!${Target.Type.Equal[pc]} && !${Window[GiveWnd].Open} || ${Target.Type.Equal[pc]} && !${Window[TradeWnd].Open}) /multiline ; /echo failure;/varset MiscCheckTimer 0;/goto :loop

|  /next a
:tEnd
    /varcalc iCount ${iCount}+1
    /if (${Window[GiveWnd].Open}) {
    	/if (${iCount}<4 && (${PackNum}==1 && ${SlotNum}==1 || ${iCount}<4 && ${PackNum}<8 && ${SlotNum}<${InvSlot[pack8].Item.Container})) /goto :tNextItem   	
      /notify GiveWnd GVW_Give_Button LeftMouseUp
      /delay 5s !${Window[GiveWnd].Open}
      /if (${Target.ID}!=${tID}) {
        /squelch /target id ${tID}
        /delay 1s ${Target.ID}==${tID}
      }
    }
    /if (${Window[TradeWnd].Open}) {
|    	/echo Pack ${PackNum} slot ${SlotNum}
    	/if (${iCount}<8 && (${PackNum}==1 && ${SlotNum}==1 || ${iCount}<8 && ${PackNum}<8 && ${SlotNum}<${InvSlot[pack8].Item.Container})) /goto :tNextItem   	
      /if ( !${Window[TradeWnd].MyTradeReady}) {
        /delay 2
        /notify TradeWnd TRDW_Trade_Button LeftMouseUp
      }
      /delay 15s !${Window[TradeWnd].Open}
      } else {
      /if (${Window[GiveWnd].Open}) {
        /delay 2
        /notify GiveWnd GVW_Give_Button LeftMouseUp
      }
      /delay 5s !${Window[GiveWnd].Open}
    }
    /varset iCount 0
:tNextItem
	/next SlotNum
 	/next PackNum
  /next b
  /next a
/keypress CLOSE_INV_BAGS
|  /for a 1 to 8
|    /if (${Window[Pack${a}].Open}) /nomodkey /itemnotify pack${a} rightmouseup
|    /delay 3s !${Window[Pack${a}].Open}
|  /next a
/keypress esc
/delay 1s
  /call MBMoveTo ${y} ${x}
/return ${tGC}

Sub MBScript
/if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:MBScript:${Param0}
| /echo ${Param0} ${Param1} ${Param2}
  /declare a int local
  /declare b int local
  /declare c int local
  /declare Timer timer local
  /declare sVarL string local
  /declare sVarC string local
  /declare sVarM string local
  /declare ScriptINI string local ${IniFile}
  /if (!${Defined[Param0]}) /multiline ; /echo Syntax Error;/return
  /if (${Select[${Param0.Arg[1,-]},net,network]}) {
      /varset ScriptINI ${NetworkINI}
      /if (!${Param0.Find[ ]}) {
      	/varset Param0 ${Param0.Right[${Math.Calc[${Param0.Length}-${Param0.Find[-]}]}]}
      } else /varset Param0 ${Param0.Arg[2]}
  }
  /if (!${Defined[${Param0}C]}) {
    /declare ${Param0}C int outer ${Ini["${ScriptINI}",Script-${Param0},Commands]}
    /if (${Param0.Equal[Defense]} && !${${Param0}C}) /return
    /if (!${${Param0}C}) /multiline ; /ini "${ScriptINI}" "Script-${Param0}" "Commands" "0";/ini "${ScriptINI}" "Script-${Param0}" "C1" "/return";/echo MBScript - INI Entry Made;/return
|    /if (${Ini["${ScriptINI}",Script-${Param0},Commands]}==0) /return
    /declare ${Param0}[${${Param0}C}] string outer
    /for a 1 to ${${Param0}C}
      /varset ${Param0}[${a}] ${Ini["${ScriptINI}",Script-${Param0},C${a}]}
    /next a
  }
  /if (${Ini["${ScriptINI}",Script-${Param0},Commands]}==0) /return
  /varset Timer 15s
:Top
 /for a 1 to ${${Param0}C}
    /varset sVarC
    /for b 1 to ${${Param0}[${a}].Count[{]}
      /varset sVarL ${${Param0}[${a}].Arg[${b},{]}
      /varset sVarC ${sVarC}${sVarL}${
      }
    /next b
    /varset sVarC ${sVarC}${${Param0}[${a}].Arg[${b},{]}
    /if (!${${Param0}[${a}].Count[{]}) /varset sVarC ${${Param0}[${a}]}
    /varset sVarM
    /for b 1 to ${sVarC.Count[']}
      /if (${sVarC.Find['']}) {
        /varset sVarL ${sVarC.Mid[1,${Math.Calc[${sVarC.Find['']}-1]}]}"
| "
        /varset sVarC ${sVarC.Mid[${Math.Calc[${sVarC.Find['']}+2]},${sVarC.Length}]}
        /varset sVarM ${sVarM}${sVarL}
      }
    /next b
    /varset sVarM ${sVarM} ${sVarC}
    /if (${sVarM.Length}) /varset sVarC ${sVarM}
    /docommand ${sVarC}
    /if (!${Timer}) /multiline ; /echo Script [${Param0}] took too long, exiting;/return
  /next a
/return

Sub CastCall(CCMsgFrom,CCMsgTxt)
/if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:CastCall:${CCMsgFrom}-${CCMsgTxt}
|return <cast_???> <AH,AH,AC,AB,AE,AQ> <Count Number 1-??Count> <spellid> <targetid>
| /echo ${CCMsgFrom}-${CCMsgTxt}
  /declare a int local
  /declare b int local
  /declare c int local
  /declare tCastClass string local AH AD AC AB AE AQ
  /declare tCastCnt int local
  /declare tGrpMbr int local
  /declare sGem string local
  /declare sReturn string local
  /declare tGem int local
  /declare tSpell string local
  /declare tSpellID int local
  /declare tSpellRange int local
  /declare tCastTime timer local
  /declare tCT int local
  /declare tTargetID int local
  /declare CCtTargetID int local
  /declare tCCMsgTxt string local
  /declare ProjectIllu bool local FALSE
  /declare tFoci string local
  /declare pGem int local
  /if (${Me.Class.ShortName.Equal[brd]}) /declare tTwistGems string local ${Twist.List}
  /declare sNBClnt string local ${NetBots.Client}
:AliasLoop
  /for a 1 to 6
    /if (${${tCastClass.Arg[${a}]}Count}) {
      /varset tCastCnt ${${tCastClass.Arg[${a}]}Count}
      /for b 1 to ${tCastCnt}
         /for c 1 to ${Math.Calc[${${tCastClass.Arg[${a}]}SpellAlias[${b}].Count[|]}+1]}
            /if (${CCMsgTxt.Arg[2].Equal[${${tCastClass.Arg[${a}]}SpellAlias[${b}].Arg[${c},|]}]}) /multiline ; /varset sReturn ${tCastClass.Arg[${a}]} ${b};/varset tSpell ${${tCastClass.Arg[${a}]}Spell[${b}]};/varset sGem ${${tCastClass.Arg[${a}]}Gem[${b}]};/varset tFoci ${${tCastClass.Arg[${a}]}SpellFoci[${b}]};/goto :FndSpellAlias
|/echo /if (${CCMsgTxt.Arg[2].Equal[${${tCastClass.Arg[${a}]}SpellAlias[${b}].Arg[${c},|]}]}) /multiline ; /varset sReturn ${tCastClass.Arg[${a}]} ${b};/varset tSpell ${${tCastClass.Arg[${a}]}Spell[${b}]};/varset sGem ${${tCastClass.Arg[${a}]}Gem[${b}]};/goto :FndSpellAlias
         /next c
      /next b
    }
  /next a
  /if (${NetBots[${CCMsgFrom}].ID}) /echo Couldn't find Alias [${CCMsgTxt.Arg[2, ]}]
  /return NOALIASFND
:FndSpellAlias
  /if (${tCastClass.Arg[${a}].NotEqual[ab]} && !${MasterList.Find[${CCMsgFrom}]} && ${CCMsgFrom.NotEqual[${Me.CleanName}]}) /return
  /if (${CCMsgTxt.Arg[3].Equal[off]}) {
    /multiline ; /varset ${tCastClass.Arg[${a}]}TarCnt[${b}] 0;/bc ${CCMsgTxt.Arg[2]}-${tSpell} is Off
    /if (${Me.Class.ShortName.NotEqual[brd]}) /return SUCCESS
    /varset CCMsgTxt sung ${CCMsgTxt.Arg[2]}
  }
  /if (${CCMsgTxt.Arg[3].Equal[on]}) {
    /call LoadINIVar ${If[${tCastClass.Arg[${a}].Equal[ah]},AdvHeal,${If[${tCastClass.Arg[${a}].Equal[ab]},AdvBuff,${If[${tCastClass.Arg[${a}].Equal[ad]},AdvDebuff,${If[${tCastClass.Arg[${a}].Equal[ae]},AdvEvent,${If[${tCastClass.Arg[${a}].Equal[aq]},AdvCure,NOTFOUND]}]}]}]}]} ${tCastClass.Arg[${a}]}TarCnt${b} 0 ${tCastClass.Arg[${a}]}TarCnt[${b}] ${IniFile}
    /multiline ; /if (${${tCastClass.Arg[${a}]}TarCnt[${b}]}==0) /varset ${tCastClass.Arg[${a}]}TarCnt[${b}] 1;/bc ${CCMsgTxt.Arg[2]}-${tSpell} is On;/return SUCCESS
  }
  /if (${Select[${sGem},1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]}) {
|/echo was Spell
   /docommand ${If[${Me.Book[${Me.Book[${tSpell} Rk. III]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${tSpell} Rk. III]}].ID},${If[${Me.Book[${Me.Book[${tSpell} Rk. II]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${tSpell} Rk. II]}].ID},${If[${Me.Book[${Me.Book[${tSpell}]}].ID},/varset tSpellID ${Me.Book[${Me.Book[${tSpell}]}].ID},/multiline ; /bc Spell ${tSpell} Not found in your book.  Check your .ini settings.;/goto :CantCast]}]}]}
   /if (${Me.Book[${Me.Book[${tSpell}]}].ID}!=${tSpellID}) /varset tSpell ${Spell[${tSpellID}].Name}
    /if (${Me.CurrentMana}<${Spell[${tSpellID}].Mana} || ${Ini["${MBCommon}",RestrictedList,${Zone.ID}].Find[${tSpellID}|]}) /goto :CantCast
    /multiline ; /varset tGem ${sGem} ;/varcalc tCT ${Spell[${tSpellID}].MyCastTime}*10+40
|    /varset tSpellID ${Me.Book[${Me.Book[${tSpell}]}].ID}
    /if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) /delay 2s !${Me.Casting.ID}
    /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) {
      /if (${Me.Class.ShortName.Equal[BRD]}) /varset pGem ${Me.Gem[${tGem}].ID}
      /squelch /memorize "${Spell[${tSpellID}]}|gem${tGem}"
      /delay 5s ${Me.Gem[${tGem}].ID}==${tSpellID}
      /if (${Me.Gem[${tGem}].ID}!=${tSpellID}) /multiline ; /bc CAST - FAILED MEMORIZE;/goto :CantCast
      /varcalc tCT ${Spell[${tSpellID}].RecastTime}*10+${tCT}
    }
    /if (${Me.Class.ShortName.Equal[brd]}) {
|      /echo tTwistGems : ${tTwistGems} TarCnt : ${${sReturn.Arg[1]}TarCnt[${sReturn.Arg[2]}]}
      /if (!${CCMsgTxt.Find[sing]} && !${CCMsgTxt.Find[sung]} && (${MBPause} || !${tTwistGems.Length})) {
        /varset tTwistGems ${tGem}
      } else {
        /if (${CCMsgTxt.Find[sing]}) {
          /varset ${sReturn.Arg[1]}TarCnt[${sReturn.Arg[2]}] 1
          /if (${BardRestGems.Find[${tGem}]} || ${BardCombatGems.Find[${tGem}]}) /return
          /docommand ${If[${Spell[${tSpellID}].SpellType.Equal[detrimental]},${If[!${BardCombatGems.Find[${SGem}]},/varset BardCombatGems ${BardCombatGems} ${sGem},]},${If[!${BardRestGems.Find[${SGem}]},/varset BardRestGems ${BardRestGems} ${sGem},]}]}
      } else /if (${CCMsgTxt.Find[sung]}) {
        /varset ${sReturn.Arg[1]}TarCnt[${sReturn.Arg[2]}] 0
        /if (!${BardRestGems.Find[${tGem}]} && !${BardCombatGems.Find[${tGem}]}) /return
        /if (${BardRestGems.Find[${tGem}]}) {
          /multiline ; /varset sGem R ${BardRestGems};/varset tSpell
          /for a 1 to ${sGem.Count[ ]}+1
             /if (!${sGem.Arg[${a}].Equal[${tGem}]} && ${a}>1) /varset tSpell ${tSpell} ${sGem.Arg[${a}]}
          /next a
        }
        /if (${sGem.Arg[1].Equal[r]}) /varset BardRestGems ${tSpell}
        /if (${BardCombatGems.Find[${tGem}]}) {
          /multiline ; /varset sGem C ${BardCombatGems};/varset tSpell
          /for a 1 to ${sGem.Count[ ]}+1
             /if (!${sGem.Arg[${a}].Equal[${tGem}]} && ${a}>1) /varset tSpell ${tSpell} ${sGem.Arg[${a}]}
          /next a
        }
        /if (${sGem.Arg[1].Equal[c]}) /varset BardCombatGems ${tSpell}
        /varset tTwistGems ${tSpell}
        /varset tSpell Removed [${Spell[${tSpellID}].Name}] from list : TarCnt == ${${sReturn.Arg[1]}TarCnt[${sReturn.Arg[2]}]}
        /varcalc tCT 0
        /goto :tContCast
      }
      }
|      /squelch /stoptwist
     }
    } else /if (${sGem.Equal[item]} && ${Bool[${FindItem[=${tSpell}]}]}) {
|/echo was Item
      /if (${FindItem[=${tSpell}].Timer}) /goto :CantCast
      /multiline ; /varset tSpellID ${FindItem[=${tSpell}].Spell.ID};/varset tGem 20;/varcalc tCT ${FindItem[=${tSpell}].CastTime}*10+30
|      /echo ${tSpell}
      /if (${tSpell.Equal[Fellowship Registration Insignia]}) {
      	/varset tTargetID ${Me.ID}
      	/declare tItem int local ${InvSlot[ranged].Item.ID}
      	/if (${Me.Class.ShortName.Equal[rog]} && ${Me.Invis}) {
      		/declare ReHide bool local TRUE
      		/melee hide=0
      	}
      	/if (${Me.Invis}) {
      		/target clear
      		/delay 5 !${Target.ID}
      		/if (!${Target.ID}) /attack on
      		/delay 5 !${Me.Invis}
      		/if (${Me.Combat}) /attack off
      	}
      }
      } else /if (${sGem.Equal[alt]} && ${Me.AltAbility[${tSpell}].ID}) {
|/echo was Alt
        /if (!${Me.AltAbilityReady[${tSpell}]}) /goto :CantCast
        /multiline ; /varset tSpellID ${Me.AltAbility[${tSpell}].Spell.ID} ;/varset tGem 30;/varcalc tCT ${Me.AltAbility[${tSpell}].Spell.CastTime}*10+20
      } else /if (${sGem.Equal[script]}) {
      	/call MBScript "${tSpell}"
      	/return
      }

  /varset tSpellRange ${Spell[${tSpellID}].Range}
  /varset tTargetID 0

  /if (${CCMsgTxt.Arg[3].Length}) {
    /if (${CCMsgTxt.Arg[3].Find[%t]} && ${NetBots[${CCMsgFrom}].TargetID}) {
      /varset tTargetID ${NetBots[${CCMsgFrom}].TargetID}
     } else /if (${CCMsgTxt.Arg[3].Find[self]}) {
       /varset tTargetID ${Me.ID}
     } else /if (${sNBClnt.Find[${CCMsgTxt.Arg[3]}]}) {
       /varset tTargetID ${Spawn[${CCMsgTxt.Arg[3]}].ID}
     } else /if (${Spawn[${CCMsgTxt.Arg[3]}].ID}) {
       /varset tTargetID ${Spawn[${CCMsgTxt.Arg[3]}].ID}
    } else /if (${CCMsgTxt.Arg[3].Find[pet]} && ${Spawn[pc ${CCMsgFrom}].Pet.ID}) {
    	/varset tTargetID ${Spawn[pc ${CCMsgFrom}].Pet.ID}
    }
  }
  
| /echo ${tTargetID} ${Spawn[pc ${CCMsgFrom}].ID}
  /if (!${tTargetID}) /docommand ${If[${Spell[${tSpellID}].SpellType.Equal[detrimental]},/varset tTargetID ${NetBots[${CCMsgFrom}].TargetID},${If[${NetBots[${CCMsgFrom}].ID},/varset tTargetID ${NetBots[${CCMsgFrom}].ID},/varset tTargetID ${Spawn[${CCMsgFrom}].ID}]}]}
  /if (!${tTargetID} && !${NetBots[${CCMsgFrom}].ID} && (${Spell[${tSpellID}].SpellType.Equal[detrimental]} || ${CCMsgTxt.Arg[3].Find[%t]})) {
  	/target clear
  	/assist ${CCMsgFrom}
  	/delay 2s ${Target.ID}
  	/varset tTargetID ${Target.ID}
  }
:tPrepBuff
  /if (${CCMsgTxt.Arg[3].Equal[grp]}) {
    /varcalc tGrpMbr ${tGrpMbr}+1
    /if (${Group.Member[${tGrpMbr}].ID}) {
        /varset tTargetID ${Group.Member[${tGrpMbr}].ID}
        /if (!${Spawn[${tTargetID}].ID} || ${Spawn[${tTargetID}].Distance}>${Spell[${tSpellID}].MyRange}) /multiline ; /bc Cast canceled for ${Spawn[${tTargetID}].Name}.  Not in range!;/goto :tPrepBuff
    } else {
        /varset tTargetID ${Me.ID}
    }
  }
  /if (${Select[${Spell[${tSpellID}].Name},Gate,Mass Group Buff]} || (${Spell[${tSpellID}].TargetType.Equal[self]} && !${Me.AltAbility[Project Illusion]})) {
    /varset tTargetID ${Me.ID}
  } else /if (${Spell[${tSpellID}].TargetType.Equal[self]} && ${Me.AltAbility[Project Illusion]} && (${Spawn[${tTargetID}].ID} && ${tTargetID}!=${Me.ID} || ${CCMsgTxt.Arg[3].Find[%t]} || ${Spawn[${CCMsgTxt.Arg[3]}].ID})) {
    /varset ProjectIllu TRUE
    /echo Projecting
  }
  /if (${Spell[${tSpellID}].TargetType.Equal[pet]}) /varset tTargetID ${Me.Pet.ID}
  /if (!${tTargetID} || ${Spawn[${tTargetID}].Distance3D}>${tSpellRange} && ${Spell[${tSpellID}].TargetType.Find[single]}) /multiline ; /echo Dude 10;/goto :CantCast
  /multiline ; /varset CCtTargetID 0;/varset tCastTime ${tCT}
:tCasting
  /if (${CCtTargetID}) {
    /doevents ImHit
    /if (${APAggro}) /goto :tContCast
    /if (${Me.Class.ShortName.Equal[BRD]}) {
|      /echo Me is BRD
      /doevents CastRst
      /doevents CastStn
      /doevents CastImm
      /if (!${CastResult.Equal[pending]}) {
         /if (${Select[${CastResult},success,immune,resisted,reset]}) /goto :tContCast
         /if (${Select[${CastResult},stunned]}) /multiline ; /varset CCtTargetID 0;/goto :tCasting
      }
      /delay 1s
      /if (!${tCastTime} && !${Cast.Timing}) /varset CastResult SUCCESS
    }
    /if (${Cast.Timing} && !${Cast.Status.Equal[i]} || ${Me.Class.ShortName.Equal[BRD]}) /goto :tCasting
    /if (${Cast.Status.Equal[i]}) {
      /if (${Select[${Cast.Result},cast_fizzle,cast_recover]}) /multiline ; /varset tCastTime ${tCT};/varset CCtTargetID 0;/goto :tCasting
      /if (${Select[${Cast.Result},cast_immune,cast_takehold]}) /multiline ; /bc Target Immune or wont Take Hold;/goto :tContCast
      /if (${Select[${Cast.Result},cast_outofrange,cast_cannotsee,cast_distracted,cast_notarget,cast_interrupted]}) /goto :CantCast
      /if (${Cast.Result.Equal[cast_success]}) /goto :tContCast
    }
   } else /if (${Me.Class.ShortName.Equal[BRD]} || ${Cast.Ready[${tSpell}|${sGem}]} && !${Spell[tSpellID].TargetType.Equal[self]}) {
      /varset CCtTargetID ${tTargetID}
      /echo ${tTargetID}
      /if (${Target.ID}!=${CCtTargetID} && ${CCtTargetID} && (${tTargetID}!=${Me.ID} && ${Spell[${tSpellID}].TargetType.NotEqual[self]})) {
         /squelch /tar id ${CCtTargetID}
         /delay 1s ${Target.ID}==${CCtTargetID}
      }
      /if (${Me.Class.ShortName.Equal[BRD]}) {
        /multiline ; /if (${Twist}) /twist off;/varset tCastTime 40;/varset CastResult PENDING
        /echo /docommand ${If[${tGem}<20,/squelch /twist ${sGem},/casting "${tSpell}|${sGem}" ${If[${tTargetID},-targetid|${tTargetID},]}]}
        /docommand ${If[${tGem}<20,/squelch /twist ${sGem},/casting "${tSpell}|${sGem}" ${If[${tTargetID},-targetid|${tTargetID},]}]}
      } else {
        /if (${ProjectIllu}) /multiline ; /casting "Project Illusion|alt" -targetid|${tTargetID};/delay 1s
        /varset MBCurrentSpell CC|1|${tTargetID}
        /docommand ${If[${tGem}>19,/call MQ2Cast "${tSpell}" ${sGem} ${Spell[${tSpell}].MyCastTime}s -targetid|${tTargetID} -bandolier|${tFoci},/call MQ2Cast "${Spell[${tSpellID}]}" gem${tGem} ${Spell[${tSpell}].MyCastTime}s -targetid|${tTargetID} -bandolier|${tFoci}]}
        /if (${Macro.Return.Equal[cast_success]}) /goto :tContCast
      }
  }
  /if (${tCastTime} || ${Cast.Status.NotEqual[i]}) /goto :tCasting
  /echo WTF - Cast
  /goto :CantCast
:tContCast
  /varset sReturn ${Cast.Result} ${sReturn} ${tSpellID} ${CCtTargetID}
  /if (${Me.Class.ShortName.Equal[BRD]} && ${Twist.List.NotEqual[${tTwistGems}]}) /squelch /twist ${tTwistGems}
  /if (!${Select[${Cast.Result},cast_immune,cast_takehold]}) /docommand ${If[!${tSpell.Find[from list]},/bc Casted: -[ ${Spawn[${CCtTargetID}].CleanName} ]- with ${tSpell},/bc Casted: -[ Bard ]- ${tSpell}]}
  /if (${Cursor.ID}) /call ClearCursor
  /doevents eqbc
  /if (${CCMsgTxt.Arg[3].Equal[grp]} && ${Group.Member[${tGrpMbr}].ID}) /multiline ; /varcalc tCT ${tCT}+40;/goto :tPrepBuff
  /if (!${CCMsgTxt.Arg[3].Length} || (!${CCMsgTxt.Arg[4].Length} && (${CCMsgTxt.Arg[3].Equal[grp]} || ${CCMsgTxt.Arg[3].Equal[%t]}))) /return ${sReturn}
  /docommand ${If[${CCMsgTxt.Arg[3].Equal[%t]} || ${CCMsgTxt.Arg[3].Equal[grp]} || ${sNBClnt.Find[${CCMsgTxt.Arg[3]}]} || ${CCMsgTxt.Arg[3].Equal[self]},/varset a 4,/varset a 3]}
  /multiline ; /varset tCCMsgTxt ;/varset tGrpMbr 0
 :tArgloop
  /if (${CCMsgTxt.Arg[${a}].Length}) {
    /varset tCCMsgTxt ${tCCMsgTxt} ${CCMsgTxt.Arg[${a}]}
    /varcalc a ${a}+1
    /goto :tArgloop
    } else {
    /varset CCMsgTxt cast ${tCCMsgTxt}
    /goto :AliasLoop
  }
  /if (!${Me.ID}) /call Event_DelayOnZone
  /if (${ReHide}) /melee hide=1
  /if (${Defined[tItem]} && ${InvSlot[ranged].Item.ID}!=${tItem}) /exchange ${tItem} ranged
  /varset MBCurrentSpell
/if (${Me.Class.ShortName.Equal[BRD]} && !${MBPause} && ${Twist.List.NotEqual[${tTwistGems}]}) /squelch /twist ${tTwistGems}

  /return ${sReturn}
:CantCast
/if (${Me.Class.ShortName.Equal[BRD]} && !${MBPause} && ${Twist.List.NotEqual[${tTwistGems}]}) /squelch /twist ${tTwistGems}
/bc There was a problem and I can't cast ${Spell[${tSpellID}].Name} right now
/return FAIL ${sReturn}

Sub ProcessExcludeList
/if (${MBDebug}) /echo Entered Sub:ProcessExcludeList ${Param0} : ${ZoneName} - ${Zone.Name}
 /if (${Defined[Param0]} || !${Zone.Name.Equal[${ZoneName}]}) {
   /if (${Param0.Equal[save]}) {
    /ini "${MBCommon}" "ExcludeList" "${Zone.ShortName}" "${ExcludeList}"
    /return
  } else /if (${Param0.Equal[zone]} || !${Zone.Name.Equal[${ZoneName}]}) {
    /varset ExcludeList ${If[${Select[${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]},NOTFOUND,NULL]},|,${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]}]}
|    /echo /varset ExcludeList ${If[${Select[${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]},NOTFOUND,NULL]},|,${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]}]}
|    /echo List set
  }
 }
/if (!${ExcludeList.Left[1].Equal[|]}) /varset ExcludeList |${ExcludeList}
 /declare a int local
 /squelch /alert clear ${AlertList}
 /squelch /alert add ${AlertList} NPC "eye of "
 /if (${ExcludeList.Length}<=1) /return
 /for a 1 to ${ExcludeList.Count[|]}
   /if (${ExcludeList.Arg[${a},|].Find[#]}==1) {
    /squelch /alert add ${AlertList} NPC ${ExcludeList.Arg[${a},|].Right[-1]}
   } else /if (${Spawn[npc ${ExcludeList.Arg[${a},|]} noalert ${AlertList}].ID}) {
|   	/echo /squelch /alert add ${AlertList} NPC ${ExcludeList.Arg[${a},|]}
    /squelch /alert add ${AlertList} NPC "${ExcludeList.Arg[${a},|]}"
  }
 /next a
/return

Sub Login(string name,..)
| /echo ${name.Arg[2, ]}
/if (${Defined[MacToRun]}) /deletevar MacToRun
/if (${name.Find[|]}) {
	/declare MacToRun string global ${name.Arg[2,|]}
}
| /echo ${MacToRun}
  /declare toon int local
  /if (!${Defined[name]}) /multiline ; /echo Syntax error;/return
   /bc Logging into ${name.Arg[1,|]}
   /camp
   /delay 10m ${MacroQuest.GameState.Equal[CHARSELECT]}
   /varset toon ${Window[CharacterListWnd].Child[CLW_LeftScreen].Child[CLW_CharactersScreen].Child[CLW_Character_List].List[${name.Arg[1,|]},3]}
   /notify CharacterListWnd CLW_Character_List listselect ${toon}
   /notify CharacterListWnd CLW_Character_List leftmouse ${toon}
   /notify CharacterListWnd CLW_Play_Button leftmouseup
   /delay 10m ${MacroQuest.GameState.Equal[INGAME]}
|   /delay 3m ${Me.ID}
   /if (${Zone.ShortName.Equal[Bazaar]} && ${Me.Name.Equal[${TraderName.Arg[1,|]}]} && !${Defined[MacToRun]}) {
   	  /multiline ; /echo Trader logged in.;/if (!${Window[BazaarWnd]}) /timed 10 /trader;/timed 30 /if (${Window[BazaarWnd].Child[BZW_Start_Button]}) /nomodkey /notify BazaarWnd BZW_Start_Button leftmouseup;/endmacro
   	} else /if (${Zone.ShortName.Equal[Bazaar]} && ${Me.Name.Equal[${TraderName.Arg[1,|]}]} && ${Defined[MacToRun]}) {
   		/multiline ; /echo Trader logged in.  Starting ${MacToRun} macro.;/timed 100 /mac ${MacToRun};/timed 150 /deletevar MacToRun
   		/endmacro
   	} else {
   		/multiline ; /mqp;/timed 100 /macro modbot
   	}
/return

Sub Startup
/if (${MBDebug}) /multiline ; /echo Macro.RunTime:${Macro.RunTime} - Sub:Startup
/if (!${Plugin[MQ2Eqbc].Name.Length}) {
    /squelch /plugin MQ2Eqbc noauto
    /if (!${Plugin[MQ2Eqbc].Name.Length}) /multiline ; /echo Better get MQ2Eqbc loaded if you plan to use the macro; /endmacro
  }
/if (!${Plugin[MQ2NetHeal].Name.Length}) {
|  /squelch /plugin MQ2NetHeal noauto
  /if (!${Plugin[MQ2NetHeal].Name.Length}) /echo MISSING - MQ2NetHeal.  Using group and netbots info for heals.
|  } else {
|   /netheal on send=on grab=on
|   /netcure on
  }
/if (!${Plugin[MQ2Netbots].Name.Length}) {
    /squelch /plugin MQ2Netbots noauto
    /bc You best configure MQ2Netbots if you want to use this macro
    /if (!${Plugin[MQ2Netbots].Name.Length}) /echo MISSING - MQ2Netbots
  } else {
   /netbots on send=on grab=on
  }
/if (!${Plugin[MQ2Melee].Name.Length}) {
|     /if (!${Select[${Me.Class.ShortName},war,pal,rog,mnk,rng,brd]}) /squelch /plugin MQ2Melee noauto
    /bc You best configure MQ2Melee if you want to melee
    /if (!${Plugin[MQ2Melee].Name.Length}) /echo MISSING - MQ2Melee
  }
/squelch /stick off
/declare CurrentEQBCCommand string outer
/declare AlertList int outer 20
/declare MiscCheckTimer timer outer
/declare SitCheckTimer timer outer
/declare SenderName string outer
/declare FollowFlagZoneDelay int outer 20
/declare DoBuffBot bool outer
/declare CastResult string outer UNKNOWN
/declare CampStatus int outer
/declare MakeCampX int outer
/declare MakeCampY int outer
/declare MakeCampD int outer
/declare MBPause bool outer
/declare QueueCount int outer
/declare DoBuffQueue[25,2] int outer
/declare MBCurrentSpell string outer
/declare DoQuest bool outer TRUE
/declare DoCombines bool outer FALSE

/call LoadINIVar Settings DoMelee FALSE DoMelee "${IniFile}" bool
/call LoadINIVar Settings DoHeals TRUE DoHeals "${IniFile}" bool
/call LoadINIVar Settings DoBuffs FALSE DoBuffs "${IniFile}" bool
/call LoadINIVar Settings DoDebuffs FALSE DoDebuffs "${IniFile}" bool
/call LoadINIVar Settings DoEvents FALSE DoEvents "${IniFile}" bool
/call LoadINIVar Settings DoCures FALSE DoCures "${IniFile}" bool
/call LoadINIVar Settings DoPull FALSE DoPull "${IniFile}" bool
/call LoadINIVar Settings DoPet FALSE DoPet "${IniFile}" bool
/call LoadINIVar Settings DoSit FALSE DoSit "${IniFile}" bool
/call LoadINIVar Settings DoLoot FALSE DoLoot "${IniFile}" bool
/call LoadINIVar Settings DoFW FALSE DoFW "${IniFile}" bool
/call LoadINIVar Settings DoForage FALSE DoForage "${IniFile}" bool
/call LoadINIVar Settings ForageIni "forage.ini" ForageIni "${IniFile}" string
/call LoadINIVar Settings DoAfk FALSE DoAfk "${IniFile}" bool
/call LoadINIVar Settings DoMount FALSE DoMount "${IniFile}" bool
/call LoadINIVar Settings MountCast " " MountCast "${IniFile}" string
/if (!${Defined[DoAura]}) /declare DoAura bool outer ${Ini["${IniFile}",Settings,DoAura]}
/if (!${Defined[AuraCast]}) /declare AuraCast string outer ${Ini["${IniFile}",Settings,AuraCast]}
/call LoadINIVar Settings MasterList " " MasterList "${IniFile}" string
/call LoadINIVar Settings TankName " " AllTankName "${IniFile}" string
/declare TankName string outer ${AllTankName.Arg[1,,]}
| /call LoadINIVar Settings ExcludeList " " ExcludeList "${MBCommon}" string ${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]}
/declare ExcludeList string outer ${if[${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND].NotEqual[NOTFOUND]},${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]},|]}
/call LoadINIVar Settings Radius 100 Radius "${IniFile}" int
/call LoadINIVar Settings SitAggroRadiusCheck 75 SitAggroRadiusCheck "${IniFile}" int
/call LoadINIVar Settings AfkMessage "Not now, thanks" AFKMessage "${IniFile}" string
/call LoadINIVar Settings DeathSlot "FALSE" DeathSlot "${IniFile}" string
/if (!${Select[${DeathSlot},TRUE,FALSE]}) /ini "${IniFile}" "Settings" "DeathSlot" "FALSE"
/call LoadINIVar Settings NetworkINI " " NetworkINI "${IniFile}" string
/call LoadINIVar Settings TraderName " " TraderName "${IniFile}" string
/if (!${Defined[DoTells]}) /declare DoTells bool outer TRUE
/multiline ; /squelch /makecamp leash 200;/squelch /makecamp radius 6;/if (!${MakeCamp.Leash}) /squelch /makecamp leash;/makecamp off
/if (${Defined[CurrLoc]}) {
  /call LoadINIVar Settings FollowDistance 20 BotFollowDistance "${IniFile}" string
  /call DistanceFunction ${BotFollowDistance}
  } else {
  /call LoadINIVar Settings FollowDistance 20 FollowDistance "${IniFile}" string
  /if (!${Defined[FollowFlag]}) /declare FollowFlag int outer
  /if (!${Defined[PauseFlag]}) /declare PauseFlag int outer 0
  /if (!${Defined[FollowID]}) /declare FollowID int outer
  /if (!${Defined[FollowName]}) /declare FollowName string outer
  /call LoadINIVar Settings FollowStick "${FollowDistance} hold uw" FollowStick "${IniFile}" string

}
/if (${Select[${Me.Class.ShortName},shd,brd,bst,shm,clr,dru,wiz,mag,enc,nec]}) {
	/declare PetShrink bool outer
	/declare PetShrinkSpell string outer
  /call LoadINIVar Settings PetCast " " PetCast "${IniFile}" string
  /call LoadINIVar Settings PetAggro FALSE PetAggro "${IniFile}" bool
  /call LoadINIVar Settings PetAssist 0 PetAssist "${IniFile}" int
  /call LoadINIVar Settings PetFoci " " PetFoci "${IniFile}" string
  /call LoadINIVar Settings PetShrink TRUE PetShrink "${IniFile}" bool
  /call LoadINIVar Settings PetShrinkSpell " " PetShrinkSpell "${IniFile}" string

}
/if (${DoFW}) {
  /call LoadINIVar Settings SummonFood " " SumFood "${IniFile}" string
  /call LoadINIVar Settings SummonDrink " " SumDrink "${IniFile}" string
}

/if (${Select[${Me.Class.ShortName},brd]}) {
  /if ( !${Plugin[MQ2Twist].Name.Length} ) {
    /squelch /plugin MQ2Twist noauto
    /if ( !${Plugin[MQ2Twist].Name.Length} ) /bc I'm a Bard and I can't load MQ2Twist!!
  }
  /squelch /stoptwist
  /squelch /twist reset
  /squelch /twist reload
  /declare BardCombatGems string outer
  /declare BardRestGems string outer
}
 /call LoadINIVar Settings LoadAlias 1 LoadAlias "${MBCommon}" int
 /if (${LoadAlias}) {
     /alias /mb /echo MB-
     /ini "${MBCommon}" "Settings" "LoadAlias" 0
 }
 /call LoadINIVar Settings BeepCommand "beep ding.wav" BeepCommand "${MBCommon}" string
  /if (${Me.MaxMana}) {
   /if (${Me.AltAbility[Gift of Mana].ID}) /call LoadINIVar Settings GoMNuke " Alias of debuff for GoM " GoMNuke "${IniFile}" string
   /if (${Me.AltAbility[Gift of Radiant Mana].ID}) /call LoadINIVar Settings GoRMNuke " " GoRMNuke "${IniFile}" string
   /if (${Me.AltAbility[Gift of Exquisite Radiant Mana].ID}) /call LoadINIVar Settings GoERMNuke " " GoERMNuke "${IniFile}" string
   /if (${Me.AltAbility[Gift of Amazing Exquisite Radiant Mana].ID}) /call LoadINIVar Settings GoAERMNuke " " GoAERMNuke "${IniFile}" string
   /if (${Me.AltAbility[Gift of Dreamlike Exquisite Radiant Mana].ID}) /call LoadINIVar Settings GoDERMNuke " " GoDERMNuke "${IniFile}" string
  }
/call ProcessExcludeList
/tgb on
/doevents flush
/return

sub ItemSort
  /delay 5
  /declare DoubleCheck local
  /varset DoubleCheck ${Cursor.ID}
  /if (!${Ini["${ForageIni}",ForageList,${Cursor.Name}].Length}) {
    /ini "${ForageIni}" "ForageList" "${Cursor.Name}" "-1"
  } else {
    /if (${Ini["${ForageIni}",ForageList,${Cursor.Name}]}) {
      /if (${Ini["${ForageIni}",ForageList,${Cursor.Name}].Equal[Ignore]}) /return
      /if (${FindItemCount["=${Cursor.Name}"]}>=${Ini["${ForageIni}",ForageList,${Cursor.Name}]} && ${Ini["${ForageIni}",ForageList,${Cursor.Name}]}!=-1 && ${Ini["${ForageIni}",ForageList,${Cursor.Name}].NotEqual[Ignore]}) /goto :Destroy
    } else {
      :Destroy
			/if (${Cursor.ID} == ${DoubleCheck}) { 
        /echo Destroying ${Cursor.Name} 
        /destroy 
      } 
      /return
    }
  }
  :LootIt
     /echo Keeping ${Cursor.Name}
    /autoinventory
    /delay 5
|    /if (${Cursor.ID}) /goto :LootIt
/return

Sub Event_ResetMelee
    /melee reset
/return

Sub Event_WornOff(Line,SpellName,Dude)
/if (${SpellName.Equal[${ADCharmSpell}]}) {
    /call Event_CharmBroke
    /return
}
/if (${NetBots[${Dude}].ID}) /return
/declare i int local
/declare spwnType string local
  /if ((${Dude.Find['s warder]}||${Dude.Find['s Pet]})&& ${Spawn[Pet ${Dude}].ID}) {
    /varset spwnType Pet
  } else {
    /varset spwnType ${Spawn[${Dude}].Type}
  }
/if (${ABCount} && ${Select[${Spawn[${Dude}].Type},PC,Pet]}) {
    /for i 1 to ${ABCount}
        /if (${SpellName.Equal[${Spell[${ABSpell[${i}]}].Name}]} && ${ABRecast[${i}]}) /call AddToQueue ${Spawn[${spwnType} ${Dude}].ID} ${i}
    /next i
}
/return

Sub DoBuffEvents
/if (${MBPause}) /return
| /echo Dobuffevents
/declare a       int local
/declare TempVar int local ${Math.Calc[${ABCount}+1]}
/declare TempGuy int local 0
/for a 1 to 25
    /if (${DoBuffQueue[${a},2]}<${TempVar} && ${Spawn[${DoBuffQueue[${a},1]}].ID} && ${Spawn[${DoBuffQueue[${a},1]}].Type.NotEqual[corpse]} && (${Spawn[${DoBuffQueue[${a},1]}].Distance}<${Me.Book[${Me.Book[${ABSpell[${DoBuffQueue[${a},2]}]}]}].MyRange} || ${Spawn[${DoBuffQueue[${a},1]}].Distance}<${Me.Book[${Me.Book[${ABSpell[${DoBuffQueue[${a},2]}]}]}].AERange})) {
        /varset TempGuy ${DoBuffQueue[${a},1]}
        /varset TempVar ${DoBuffQueue[${a},2]}
    }
|   /if (!${Spawn[${DoBuffQueue[${a},1]}].ID}) /call RemoveFromQueue ${DoBuffQueue[${a},1]} ${DoBuffQueue[${a},2]}
/next a
| /echo ${TempGuy} ${TempVar}
/if (!${ACState} && ${DoMelee} && ${ADMobCount}) /return
/if (!${SpawnCount[npc radius ${Radius} zradius 100 noalert ${AlertList}]} || ${ABTarType[${TempVar}].Find[cbt]} || ${Select[${ABGem[${TempVar}]},item,alt]} || ${Me.Gem[${ABSpell[${TempVar}]}]}) {
    /if (${TempGuy}) /if (((${Me.SpellReady[${ABSpell[${TempVar}]}]} || ${Spell[${ABSpell[${TempVar}]}].RecastTime}<30) && ${Me.CurrentMana}>${Spell[${ABSpell[${TempVar}]}].Mana} && ${Me.PctMana}>=${ABSpellMinMana[${TempVar}]}) || (${ABGem[${TempVar}].Equal[item]} && ${FindItem[${ABSpell[${TempVar}]}].TimerReady}<30) || (${ABGem[${TempVar}].Equal[alt]} && ${Me.AltAbilityReady[${ABSpell[${TempVar}]}]}<30)) {
            /call MQ2Cast "${ABSpell[${TempVar}]}" ${If[!${Select[${ABGem[${TempVar}]},item,alt]},gem,]}${ABGem[${TempVar}]} 10s CastCheck -targetid|${TempGuy}
            /if (${castReturn.Equal[CAST_TAKEHOLD]} || ${castReturn.Equal[CAST_SUCCESS]} || ${Cast.Return.Equal[CAST_OUTDOORS]}) {
                /call RemoveFromQueue ${TempGuy} ${TempVar}
|                /if (${ABAnnounce[${TempVar}].Length}) /docommand ${ABAnnounce[${a}]} Recast: -[ ${Spawn[${ABtTargetID}].CleanName} ]- with ${ABSpell[${a}]}
            }
    }
}
/return

Sub AddToQueue(int Buffee,int BuffNumber)
/declare a           int local
/declare c           int local
/declare sVar        string local
/declare Timeto         int local
/declare CurBuffs string local
/if (!${RestrictedList.Find[|${DoBuffID[${BuffNumber}]}|]} && ${QueueCount}<25 && ${Spawn[${Buffee}].Type.NotEqual[corpse]} && ${Spawn[${Buffee}].ID}) {
  /varset CurBuffs ${If[${Spawn[${Buffee}].Type.Equal[pc]},${NetBots[${Spawn[${Buffee}].Name}].Buff},${NetBots[${Spawn[${Buffee}].Master.CleanName}].PetBuff}]}
            /if ((${Spawn[${Buffee}].Type.Equal[pc]} && ${CurBuffs.Find[${Spell[${ABSpell[${BuffNumber}]}].ID}]} || ${Spawn[${Buffee}].Type.Equal[pet]} && ${NetBots[${Spawn[${Buffee}].Master.CleanName}].PetBuff.Find[${Spell[${ABSpell[${BuffNumber}]}].ID}]})) /goto :tDone
             /if ((${Spawn[${Buffee}].Type.Equal[pc]} && !${CurBuffs.Find[${Spell[${ABSpell[${BuffNumber}]}].ID}]} || ${Spawn[${Buffee}].Type.Equal[pet]} && !${NetBots[${Spawn[${Buffee}].Master.CleanName}].PetBuff.Find[${Spell[${ABSpell[${BuffNumber}]}].ID}]})) {
               /for c 0 to ${If[${Spawn[${tTargetID}].Type.Equal[pc]},${Math.Calc[${NetBots[${Spawn[${Buffee}].Name}].Buff.Count[ ]}-1]},${Math.Calc[${NetBots[${Spawn[${tTargetID}].Master.CleanName}].PetBuff.Count[ ]}-1]}]}
                   /if (${CurBuffs.Arg[${c}]}) {
                   /if (${Spawn[${Buffee}].Type.Equal[pc]}) /docommand ${If[${Spell[${CurBuffs.Arg[${c}]}].WillStack[${Spell[${ABSpell[${BuffNumber}]}].ID}]},/next c,/multiline ; /echo Spell ${Spell[${CurBuffs.Arg[${c}]}].Name} won't Stack with ${ABSpell[${BuffNumber}]};/goto :tDone]}
                   /if (${Spawn[${Buffee}].Type.Equal[pet]}) /docommand ${If[${Spell[${CurBuffs.Arg[${c}]}].WillStack[${Spell[${ABSpell[${BuffNumber}]}].ID}]},/next c,/multiline ; /echo Spell ${Spell[${CurBuffs.Arg[${c}]}].Name} won't Stack with ${ABSpell[${BuffNumber}]};/goto :tDone]}
                    }
               /next c
                        }
        /for a 1 to 25
            /if (!${DoBuffQueue[${a},1]}) {
                /varset DoBuffQueue[${a},1] ${Buffee}
                /varset DoBuffQueue[${a},2] ${BuffNumber}
                /varcalc QueueCount ${QueueCount}+1
                /return
            }
        /next a
}
:tDone
/return

Sub RemoveFromQueue(int Buffee,int BuffNumber)
/doevents IRC
/doevents Chat
/declare a int local
/declare tABMob int local
/for a 1 to 25
    /if (((${Spawn[${DoBuffQueue[${a},1]}].State.Equal["DEAD"]} || !${Spawn[${DoBuffQueue[${a},1]}].ID}) || ((${DoBuffQueue[${a},1]}==${Buffee} || (!${Buffee} && (${Group.Member[${Spawn[${DoBuffQueue[${a},1]}].CleanName}]} || ${Group.Member[${Spawn[${DoBuffQueue[${a},1]}].Master.CleanName}]}))) && (${DoBuffID[${DoBuffQueue[${a},2]}]}==${BuffNumber} || ${DoBuffQueue[${a},2]}==${BuffNumber}))) && ${DoBuffQueue[${a},1]}) {
        /varset DoBuffQueue[${a},1] 0
        /varset DoBuffQueue[${a},2] 0
        /varcalc QueueCount ${QueueCount}-1
    }
/next a
/return

Sub TellEvent(string From, string MsgText)
    /if (!${DoTells}) /return
    /bc ${Time} [+g+]Tell Recieved from[+x+] ${From} : ${MsgText}
    /docommand /${BeepCommand}
/return

Sub Event_Camping
  /echo Campout detected.  Ending macro.
  /end
/return

Sub CastCheck
/if ( ${MBDebug} ) /echo Macro.RunTime:${Macro.RunTime} ${Time}- Sub:CastCheck
/call AHHealCheck
/if (${Me.Mount.ID} && !${AHAllowDismount} || ${Cast.Status.Equal[I]}) /return
/if (${Window[RespawnWnd].Open} || ${Debuff.Charmed}) /multiline ; /varset interruptFlag TRUE;/return
  /declare a int local 0
  /if ((${Target.Type.Equal[corpse]} || !${Target.ID}) && ${Spell[${Me.Casting.ID}].TargetType.Find[Single]}) {
    /call interrupt
    /return
  }
  /if (${MBCurrentSpell.Arg[1,|].Equal[AD]}) {
    /if ((${Select[${ADTarType[${MBCurrentSpell.Arg[2,|]}]},1,11]} && ${NetBots[${TankName}].TargetID}!=${MBCurrentSpell.Arg[3,|]} || ${Select[${ADTarType[${MBCurrentSpell.Arg[2,|]}]},2,12]} && ${NetBots[${TankName}].TargetID}==${MBCurrentSpell.Arg[3,|]}) && !${Me.Mount.ID}) /call interrupt
    /if ((${Select[${ADTarType[${MBCurrentSpell.Arg[2,|]}]},1,11]} && ${NetBots[${TankName}].TargetID}!=${MBCurrentSpell.Arg[3,|]} || ${Select[${ADTarType[${MBCurrentSpell.Arg[2,|]}]},2,12]} && ${NetBots[${TankName}].TargetID}==${MBCurrentSpell.Arg[3,|]}) && ${Me.Mount.ID}) /multiline ; /varset interruptFlag TRUE;/return
    /if (${Spawn[${Target.ID}].Type.Equal[corpse]} || ${Spell[${Me.Casting.ID}].TargetType.Equal[single]} && !${Target.ID}) /call interrupt
    /return
  }
  /if (${MBCurrentSpell.Arg[1,|].Equal[AB]}) {
    /if (${ABTarType[${MBCurrentSpell.Arg[2,|]}].Find[cbt]} && !${ABTarType[${MBCurrentSpell.Arg[2,|]}].Find[idle]} && ${ADMobCount} && ${SpawnCount[npc los radius ${Radius} zradius 100 noalert ${AlertList}]}<=${SpawnCount[npc untargetable los radius ${Radius} zradius 100 noalert ${AlertList}]} && !${Me.Mount.ID}) {
      /call interrupt
      /return
    }
    /if (${ADCount} && ${ADMobCount} && ${DoDebuffs} && !${ABTarType[${MBCurrentSpell.Arg[2,|]}].Find[cbt]}) {
      /for a 1 to ${ADMobMax}
      /docommand ${If[${ADMobList[${a}]} && !${MLDebuffed[${a}]},/call interrupt,/next a]}
    }
  }
/return

Sub ParseAnnounce(string AnnounceText, string tTarget, string tSpell)
/declare ATlft string local
/declare ATrght string local
| /echo ${AnnounceText} : ${tTarget} : ${tSpell}
  /if (${AnnounceText.Length}<5 || ${tTarget.Equal[NULL]}) /return
  /if (${AnnounceText.Find[,]}) /multiline ; /docommand ${AnnounceText};/return
  /if (${AnnounceText.Find[%t]} && ${tTarget.Length}) {
    /varset ATlft ${AnnounceText.Left[${Math.Calc[${AnnounceText.Find[%t]}-1]}]}
    /varset ATrght ${AnnounceText.Right[${Math.Calc[${AnnounceText.Length}-${AnnounceText.Left[${Math.Calc[${AnnounceText.Find[%t]}-1]}].Length}-2]}]}
    /varset AnnounceText ${ATlft} ${tTarget} ${ATrght}
  }
|  /if (${AnnounceText.Find[%t]} && ${tTarget.Length} && ${tTarget.NotEqual[NULL]}) /varset AnnounceText ${AnnounceText.Replace[%t,${tTarget}]}
  /if (${AnnounceText.Find[%s]} && ${tSpell.Length}) /varset AnnounceText ${AnnounceText.Replace[%s,${tSpell}]}
  /docommand ${AnnounceText}
/return

Sub IniCommand(string IniText)
| /echo IniCommand: Text = ${IniText}
/if (${IniText.Length}<3) /return
/declare b int local 1
/declare sVarC string local
/declare sVarL string local
/declare sVarM string local
    /for b 1 to ${IniText.Count[{]}
      /varset sVarL ${IniText.Arg[${b},{]}
      /varset sVarC ${sVarC}${sVarL}${
      }
    /next b
    /varset sVarC ${sVarC}${IniText.Arg[${b},{]}
    /if (!${IniText.Count[{]}) /varset sVarC ${IniText}
    /varset sVarM
    /for b 1 to ${sVarC.Count[']}
      /if (${sVarC.Find['']}) {
        /varset sVarL ${sVarC.Mid[1,${Math.Calc[${sVarC.Find['']}-1]}]}"
| "
        /varset sVarC ${sVarC.Mid[${Math.Calc[${sVarC.Find['']}+2]},${sVarC.Length}]}
        /varset sVarM ${sVarM}${sVarL}
      }
    /next b
    /varset sVarM ${sVarM} ${sVarC}
    /if (${sVarM.Length}) /varset sVarC ${sVarM}
|    /echo "${sVarC}"
    /docommand ${sVarC}
/return FALSE

Sub Event_GoM(string ELine)
/if (${MBPause} || ${ELine.Find[of Mana]} && (!${Me.Song[Gift of Mana].ID}  || !${GoMNuke.Length}) || ${ELine.Find[of Radiant Mana]} && (!${Me.Song[Gift of Radiant Mana].ID}  || !${GoRMNuke.Length}) || ${ELine.Find[of exquisite radiant mana]} && (!${Me.Song[Gift of Exquisite Radiant Mana].ID}  || !${GoERMNuke.Length}) || ${ELine.Find[of amazing exquisite radiant mana]} && (!${Me.Song[Gift of Amazing Exquisite Radiant Mana].ID}  || !${GoAERMNuke.Length})|| ${ELine.Find[of dreamlike exquisite radiant mana]} && (!${Me.Song[Gift of Dreamlike Exquisite Radiant Mana].ID}  || !${GoDERMNuke.Length})) /return
/declare x int local 0
/declare y int local 0
/declare IniSpell string local
/declare GoMAlias string local ${If[${ELine.Find[of mana]},${GoMNuke},${If[${ELine.Find[of Radiant Mana]},${GoRMNuke},${If[${ELine.Find[of exquisite radiant mana]},${GoERMNuke},${If[${ELine.Find[of amazing exquisite radiant mana]},${GoAERMNuke}, ]}]}]}]}
/echo GoM
/for x 1 to ${Math.Calc[${GoMAlias.Count[|]}+1]}
  /call FindAlias ${GoMAlias.Arg[${x},|]}
  | ${If[${ELine.Find[of mana]},${GoMNuke},${If[${ELine.Find[of Radiant Mana]},${GoRMNuke},${If[${ELine.Find[of exquisite radiant mana]},${GoERMNuke},${If[${ELine.Find[of amazing exquisite radiant mana]},${GoAERMNuke}, ]}]}]}]}
  /if (!${Macro.Return.Length} || ${Macro.Return.Equal[NULL]}) /return
  /varset IniSpell ${Macro.Return}
  /if (${Spell[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}].SpellType.Equal[detrimental]} && (!${Spawn[${ACMATarget}].ID} || ${Spawn[${ACMATarget}].Type.Equal[corpse]})) /return
  /delay 2s ${Cast.Ready[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}|gem${${IniSpell.Arg[1,|]}Gem[${IniSpell.Arg[2,|]}]}]}
/if (!${Cast.Ready[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}|gem${${IniSpell.Arg[1,|]}Gem[${IniSpell.Arg[2,|]}]}]} && ${GoMAlias.Find[|]} && ${Math.Calc[${x}-1]}<${GoMAlias.Count[|]}) /next x
  /call MQ2Cast "${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}" gem${${IniSpell.Arg[1,|]}Gem[${IniSpell.Arg[2,|]}]} 3s CastCheck ${If[${Spell[${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}].SpellType.Equal[detrimental]},-targetid|${ACMATarget},]}
  /echo ${If[${ELine.Find[of mana]},GoMNuke,${If[${ELine.Find[of Radiant Mana]},GoRMNuke,${If[${ELine.Find[of exquisite radiant mana]},GoERMNuke,${If[${ELine.Find[of amazing exquisite radiant mana]},GoAERMNuke, ]}]}]}]} : ${${IniSpell.Arg[1,|]}Spell[${IniSpell.Arg[2,|]}]}
| /next x
/return

Sub FindAlias(SpellAlias)
/declare tCastClass string local AH AD AC AB AE AQ
/declare d int local
/declare x int local
/declare y int local
/declare tSpell string local
/for d 1 to ${tCastClass.Count[ ]}
  /for x 1 to ${${tCastClass.Arg[${d}]}Count}
    /for y 1 to ${${tCastClass.Arg[${d}]}SpellAlias[${x}].Count[|]}
      /if (${${tCastClass.Arg[${d}]}SpellAlias[${x}].Arg[${y},|].Equal[${SpellAlias}]}) {
       /varset tSpell ${tSpell}${tCastClass.Arg[${d}]}|${x}|
      }
    /next y
  /next x
/next d
| AHSpell5 would return as AH|5|
/return ${tSpell}

Sub Event_OutOfStuff
   /if (${DoCombines}) /multiline ; /echo Out of materials.  Setting DoCombines FALSE;/varset DoCombines FALSE
/return

Sub IsNumber(Index)
	/if (!${Index.Length}) /return
	/declare x int local
	/for x 1 to ${Index.Length}
		/if (!${Select[${Index.Mid[${x},1]},0,1,2,3,4,5,6,7,8,9,.]}) /multiline ; /echo IsNumber - FALSE - Index was ${Index};/return FALSE
	/next x
| 	/echo IsNumber - TRUE
/return TRUE