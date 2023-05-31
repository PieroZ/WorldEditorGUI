function FactUpdates()
  ReadLuaTableValues(FactIncTable)
end

function PickStartingPoint()
  CheckFactsTable = {}
  result = CheckLuaFacts(CheckFactsTable)
  if result == true then
    CurrentDialog = S1
  end
  CheckFactsTable = {}
  CheckFactsTable['Q_TALK_TO_NPC_3'] = 1
  result = CheckLuaFacts(CheckFactsTable)
  if result == true then
    CurrentDialog = S2
  end
end

S0 = {
  starting_point = function ()
    PickStartingPoint()
  end,
}

S1 = {
  say = "Please leave me alone.",
  starting_point = function ()
    PickStartingPoint()
  end,
}

S2 = {
  say = "So NPC_01 sent you to talk to me ? Very well, let's get down to business.",
  continue = function ()
    CurrentDialog = S3
  end,
  starting_point = function ()
    PickStartingPoint()
  end,
}

S3 = {
  say = "Lately the wolves have been wandering dangerously close to the village. In order to provide safety for women and children I must ask you to scare off the pack by killing off several of them.",
  continue = function ()
    CurrentDialog = S4
  end,
}

S4 = {
  say = "Do you think you can handle it?",
  continue = function ()
    CurrentDialog = S5
  end,
}

S5 = {
  choices = {
    choice0 = "Yes, we will do it.",
    choice1 = "We are not quite ready yet."
  },
  onSelection = function (choice)
    if choice == CurrentDialog["choices"]["choice0"] then
      CurrentDialog = S7
    elseif choice == CurrentDialog["choices"]["choice1"] then
      CurrentDialog = S8
      end
    end
}

S7 = {
  say = "Good ! Come back to me once you have slain at least 6 of wolves.",
}

S8 = {
  say = "Come back to me once you are ready.",
}

CurrentDialog = S0