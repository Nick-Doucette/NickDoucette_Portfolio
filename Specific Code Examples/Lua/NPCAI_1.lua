--[[

DESCRIPTION COMMENT FOR PORTFOLLIO:

This script was created to handle all player interaction with the NPC in the game "Lost and Found Delivery Service".
It also handles all logic related to the players objective in the game (to find and return certain objects in the level).
All NPC dialogue is placeholder text in this version.

]]


local switch = script.parent                                                            --Get our parent object in the scene
local switchTrigger = switch:FindChildByName("Trigger")                                 --Find the trigger object on our parent object
local equipList = nil                                                                   --Empty variable that will store our list of items equiped by the player
World.FindObjectByName("Phone").isEnabled = false                                       --Make sure our phone object in the scene is disabled on startup

--Define our quest states
searchState = {

		["None"] = 1,
		["Key"] = 2,
		["Win"] = 3,
		["Phone"] = 4,

}

--stores our current quest status
local search = searchState.None

--Checs if our player has a specific item equiped
--It does this by looping over the player equipment list and returning true if any of their name values match the input variable
local function CheckEquip(input)
    local i = 1
    while equipList[i] ~= nil do
       if equipList[i].name == input then
          return true
       end
       i = i + 1
    end
    return false
end

--Deletes a specific item from the player
--It does this by looping over the player equipment, then unequiping and destroying any items that match the input variable
local function RemoveObject(input)
   local i = 1
    while equipList[i] ~= nil do
       if equipList[i].name == input then
          equipList[i]:Unequip()
          equipList[i]:Destroy()
          break
       end
       i = i + 1
    end
end

--Handles player collision with the NPC trigger
--This also displays NPC dialogue to the player based on their quest state
local function OnBeginOverlap(Trigger, other)
   if search == searchState.None then
   		 switchTrigger.interactionLabel = "Placeholder_Idle_Default"
   elseif search == searchState.Key then
      switchTrigger.interactionLabel = "Placeholder_Idle_Key"
   elseif search == searchState.Win then
      switchTrigger.interactionLabel = "Placeholder_Idle_Win"
   elseif search == searchState.Phone then
      switchTrigger.interactionLabel = "Placeholder_Idle_Phone"
   end
end

--Handles player interaction with the NPC
--This also displays NPC dialogue to the player based on their quest state
local function OnSwitchInteraction(newTrigger, player)
    equipList = player:GetEquipment()                                                                       --Get all items equiped by the player and store them in a list
    if search == searchState.None then
       switchTrigger.interactionLabel = "Placeholder_Interact_Default"
       search = searchState.Key                                                                             --Change our quest state
    elseif search == searchState.Key then
       if CheckEquip("Bigkey") == true then
          switchTrigger.interactionLabel = "Placeholder_Interact_KeyTrue"
          RemoveObject("Bigkey")                                                                            --Remove the Key item from the player
          World.FindObjectByName("Phone").isEnabled = true
          search = searchState.Phone                                                                        
       else
          switchTrigger.interactionLabel = "Placeholder_Interact_KeyFalse"
       end
    elseif search == searchState.Win then
       switchTrigger.interactionLabel = "Placeholder_Interact_Win"
    elseif search == searchState.Phone then
       if CheckEquip("Phone") == true then
          switchTrigger.interactionLabel = "Placeholder_Interact_PhoneTrue"
          RemoveObject("Phone")                                                                             --Remove Phone item from the player
          search = searchState.Win
       else
          switchTrigger.interactionLabel = "Placeholder_Interact_PhoneFalse"
       end
    end
end

--Tells the engine to call our trigger overlap and interaction functions at the appropriate places
switchTrigger.beginOverlapEvent:Connect(OnBeginOverlap)
switchTrigger.interactedEvent:Connect(OnSwitchInteraction)