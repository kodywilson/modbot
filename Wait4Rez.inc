|Wait4rez.inc v2.24 - Version for ModBot
Sub Wait4Rez
/stick off
/makecamp off
/if (!${Defined[RezGiveUpSeconds]}) {
	/declare RezWaitTimer timer local 105m
} else {
	/declare RezWaitTimer ${RezGiveUpSeconds}
}
/declare RezTimer timer local 180s
/call CreateTimer RezTimer
   /if (${Me.State.Equal[HOVER]}) /varset RezTimer 180s
/echo Awaiting rez: auto-accepting
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
/if (!${Window[ConfirmationDialogBox].Open} && ${RezTimer}>0) /goto :rezmeffs
/if (!${Window[ConfirmationDialogBox].Open} && !${RezTimer}) {
      /nomodkey /notify RespawnWnd RW_OptionsList listselect 1
      /delay 1s
      /nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
      /delay 20s
      }
/if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
   /delay 2s
   /if (${Window[RespawnWnd].Open}) {
      /nomodkey /notify RespawnWnd RW_OptionsList listselect 2
      /delay 1s
      /nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
      /delay 2s
      }
/if (${Me.State.Equal[HOVER]}) /goto :rezmeffs
/ini "${IniFile}" "Settings" "DeathSlot" "FALSE"
/delay 3s !${Window[RespawnWnd].Open}
/return
