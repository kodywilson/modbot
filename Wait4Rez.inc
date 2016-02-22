|Wait4rez.inc v2.23  /lootall command and 105 minute timer added.
Sub Wait4Rez
/stick off
/makecamp off
/if (!${Defined[RezGiveUpSeconds]}) {
	/declare RezWaitTimer timer local 105m
} else {
	/declare RezWaitTimer ${RezGiveUpSeconds}
}
/echo Awaiting rez: auto-accepting, and looting my corpse 
:rezmeffs 
/doevents 
/if (${RezWaitTimer}<50) /echo ${Math.Calc[${RezWaitTimer}/10]} seconds left to rez before campout.
/if (!${Window[ConfirmationDialogBox].Open} && !${RezWaitTimer}) {
  /docommand ${If[${Plugin[MQ2Eqbc].Name.Length},/bc,/echo]} Rez timer expired.  Camping to give 15 minutes to rez me later.
  /ini "${IniFile}" "Settings" "DeathSlot" "TRUE"
  /camp desk
  /delay 5s
  /end
}
/delay 10s ${Window[ConfirmationDialogBox].Open} 
/if (!${Window[ConfirmationDialogBox].Open}) /goto :rezmeffs
/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup 
/delay 2s 
/if (${Window[RespawnWnd].Open}) /nomodkey /notify RespawnWnd RW_OptionsList listselect 2  
/delay 1s 
/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup 
/delay 2s 
/ini "${IniFile}" "Settings" "DeathSlot" "FALSE"
/delay 3s !${Window[RespawnWnd].Open}
/return