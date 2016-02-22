Sub ReqAdv
  /declare AdvRisk int 0
  /declare AdvType int 0
  /echo ReqAdv started:
  /if (${Defined[Param0]}) /varset AdvRisk ${Param0}
  /if (${Defined[Param1]}) /varset AdvType ${Param1}
  /echo ReqAdv started - Selected Risk: ${AdvRisk} - Selected Type: ${AdvType}
  /target clear
  /delay 5
  /if (${Zone.ShortName.Equal[Everfrost]}) /target Mannis McGuyett
  /if (${Zone.ShortName.Equal[Butcher]}) /target Xyzelauna Tu`Valzir
  /if (${Zone.ShortName.Equal[Sro]}) /target Kallei Ribblok
  /if (${Zone.ShortName.Equal[Nro]}) /target Escon Quickbow
  /if (${Zone.ShortName.Equal[Ecommons]}) /target Periac Windfell
  /delay 5
  /stick 9
  :ReqAdvMoveLoop
  /if (${Math.Distance[${Target.Y},${Target.X}]}<=10) /goto :ReqAdvMoveLoop
  /stick off
  /face
  /if (${AdvRisk}>0 && ${AdvType}>0) {
     /click right target
     /delay 1s
     /notify AdventureRequestWnd AdvRqst_RiskCombobox listselect ${AdvRisk}
     /delay 5
     /notify AdventureRequestWnd AdvRqst_TypeCombobox listselect ${AdvType}
     /delay 5
     /notify AdventureRequestWnd AdvRqst_RequestButton leftmouseup
     /delay 1s
     /notify AdventureReqeustWnd AdvRqst_AcceptButton leftmouseup
  }
  /echo ReqAdv complete.
/return

Sub Login(string name)
  /declare toon int local
  /if (!${Defined[name]}) /multiline ; /echo Syntax error;/return
   /bc Logging into ${name}
   /camp
   /delay 10m ${MacroQuest.GameState.Equal[CHARSELECT]}
   /varset toon ${Window[CharacterListWnd].Child[CLW_LeftScreen].Child[CLW_CharactersScreen].Child[CLW_Character_List].List[${name},3]}
   /notify CharacterListWnd CLW_Character_List listselect ${toon}
   /notify CharacterListWnd CLW_Character_List leftmouse ${toon}
   /notify CharacterListWnd CLW_Play_Button leftmouseup
   /delay 10m ${MacroQuest.GameState.Equal[INGAME]}
/return


#event Broke "#*#you cannot afford#*#" 
#event Broke "#*#you can't afford#*#" 

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

Sub Event_Broke 
   /varset Broke TRUE
   /echo Error: You are out of money! 
   /docommand /${BeepCommand} 
/return TRUE

|**********************************************************
 Macro: TeamUtilities.inc

 Description: Contains utility functions.

- PickupInventoryItem:  Takes item from inventory onto cursor
- GiveInventoryItemToTarget: Gives item from inventory to current target
- GiveCursorItemToTarget: Gives item on cursor to current target

- BuyItemFromTarget: Purchases specified item from target
- ApproveNpcTrade: Approves giving an item to an NPC
- ApproveGuildPort: Approves porting from guild portal
- ApproveLargeDialog: Generic approval of any LargeDialogWindow query
- AnswerLargeDialog: Allows yes or no answer to LargeDialogWindow query
- ClickWindowButton: Waits for window and clicks appropriate button

- LootMyCorpse

 Method: PickupInventoryItem

 Description: Picks up an item in inventory.

 Notes:      Will not work for items in Traders Satchels while
         in Trader Mode.

Usage:      /call PickupInventoryItem ${itemName}

 TODOs:
   1. Allow picking up of variable quantity


#include buy.inc


Sub PickupInventoryItem(itemName)
   /declare mypack   string local
   /declare myslot   string local
   /if (${Cursor.ID}) {
      /if (${Cursor.Name.Equal[${itemName}]})      /return
      /autoinv
      /delay 1s
   }

   /varset mypack ${FindItem[${itemName}].InvSlot.Pack}
   /varset myslot ${FindItem[${itemName}].InvSlot}

   /if (${myslot}==0) {
      /echo No item by that name in inventory.
      /return
   }

   /if (${myslot}<31) {
      /nomodkey /ctrl /itemnotify ${FindItem[${itemName}].InvSlot} leftmouseup
   } else /if (${mypack}) {
      /if (!${Window[${InvSlot[${mypack}].Name}].Open}) /nomodkey /itemnotify ${mypack} rightmouseup
      :WaitForBagOpen
      /nomodkey /ctrl /itemnotify ${myslot} leftmouseup
      /if (!${Cursor.ID})  /goto :WaitForBagOpen

      /declare previousTarget   int   local
      /varset previousTarget   ${Target.ID}
      /keypress esc
      /delay 1
      /if (${previousTarget})      /target id ${previousTarget}
   } else {
      /nomodkey /itemnotify ${myslot} rightmouseup
   }

   /delay 10s (${Cursor.Name.Find[${itemName}]})
/return

Sub GiveInventoryItemToTarget(itemName)
   /call PickupInventoryItem ${itemName}
   /call GiveCursorItemToTarget
/return

Sub GiveCursorItemToTarget
   /if (${Cursor.ID}==0 ) {
      /echo No item on cursor to give
      /return
   }

   /if (${Target.ID}==0 ) {
      /echo No target
      /return
   }

   /delay 1s
   /face
   /delay 1s

   /click left target

   /call ClickWindowButton Givewnd GVW_Give_Button
   /delay 5s (${Window[Givewnd]}==0)
/return

Sub ApproveNpcTrade
   /notify Givewnd GVW_Give_Button leftmouseup
|   /call ClickWindowButton Givewnd GVW_Give_Button
/return

Sub ApproveGuildPort
   /call AnswerLargeDialog Yes
/return

Sub ApproveLargeDialog
   /call AnswerLargeDialog Yes
/return

Sub AnswerLargeDialog(answer)
   /if (${answer.Equal[Yes]}) {
      /call ClickWindowButton LargeDialogWindow LDW_YesButton
   } else {
      /call ClickWindowButton LargeDialogWindow LDW_NoButton
   }
/return

Sub ClickWindowButton(windowName,button)
   :waitforwindow
   /if (!${Window[${windowName}].Open}) {
      /delay 5s ${Window[${windowName}].Open}
      /goto :waitforwindow
   }

   /if (${Window[${windowName}].Open}==0) {
      /echo Specified window not detected open
   }

   /nomodkey /notify ${windowName} ${button} leftmouseup
   /delay 1s
/return

Sub LootMyCorpse
   /declare loottotal int local
   /declare i int local

   /target mycorpse
   /delay 1
   /moveto ID
   /delay 6s ( ${Target.Distance} < 10 )
   /loot
   /delay 1s ${Window[LootWnd].Open}
:LootLag
   /varset loottotal ${Corpse.Items}
   /delay 1s ${loottotal}!=${Corpse.Items}
   /if (${loottotal}!=${Corpse.Items}) /goto :LootLag
   /for i 1 to ${loottotal}
:lootagain
      /itemnotify loot${i} rightmouseup
      /delay 10s !${Corpse.Item[${i}].ID}
      /if (${Corpse.Item[${i}].ID}) /goto :lootagain
   /next i
   /nomodkey /notify LootWnd DoneButton leftmouseup
   /delay 1s
   /keypress esc
   /corpsedrag
/return

**********************************************************|