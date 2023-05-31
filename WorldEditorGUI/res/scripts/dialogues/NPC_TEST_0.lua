function FactUpdates()
  ReadLuaTableValues(FactIncTable)
end

S1 = {
  say = "Greetings ! I have a very important quest for you.",
  continue = function ()
    CurrentDialog = S2
  end,
}

S2 = {
  say = "Talk to NPC_3",
    facts_update = function()
      FactIncTable = {}
      FactIncTable["Q_TALK_TO_NPC_3"] = 1
      FactUpdates()
    end
}

CurrentDialog = S1