function FactUpdates()
  ReadLuaTableValues(FactIncTable)
end

S0 = {
  say = "Why don't you ask us something? Something only the real Connor would know.",
  continue = function ()
    CurrentDialog = S1
  end
}

S1 = {
  say = "Uh, where did we first meet?",
  continue = function ()
    CurrentDialog = S2
  end
}

S2 = {
  say = "Jimmy's bar! I checked four other bars before I found you.",
  continue = function ()
    CurrentDialog = S3
  end
}

S3 = {
  say = "He uploaded my memories...",
  continue = function ()
    CurrentDialog = S4
  end
}

S4 = {
  say = "What's my dog's name?",
  continue = function ()
    CurrentDialog = S5
  end
}

S5 = {
  choices = {
    choice0 = "Sumo",
    choice1 = "Scout",
    choice2 = "Jack",
    choice3 = "Buddy"
  },
  onSelection = function (choice)
    if choice == CurrentDialog["choices"]["choice0"] then
      CurrentDialog = S9
    elseif choice == CurrentDialog["choices"]["choice1"] then
      CurrentDialog = S10
    elseif choice == CurrentDialog["choices"]["choice2"] then
      CurrentDialog = S10
    elseif choice == CurrentDialog["choices"]["choice3"] then
      CurrentDialog = S10
      end
    end
}

S9 = {
  say = "I knew that too!",
  continue = function ()
    CurrentDialog = S11
  end
}

S10 = {
  say = "Sumo. His name's Sumo.",
  continue = function ()
    CurrentDialog = S12
  end
}

S11 = {
  say = "My son, what's his name?",
  continue = function ()
    CurrentDialog = S13
  end
}

S12 = {
  say = "That settles it, shoot him, Lieutenant!",
  continue = function ()
    CurrentDialog = S11
  end
}

S13 = {
  choices = {
    choice0 = "Anthony",
    choice1 = "Gabriel",
    choice2 = "Elliott",
    choice3 = "Cole"
  },
  onSelection = function (choice)
    if choice == CurrentDialog["choices"]["choice0"] then
      CurrentDialog = S17
    elseif choice == CurrentDialog["choices"]["choice1"] then
      CurrentDialog = S17
    elseif choice == CurrentDialog["choices"]["choice2"] then
      CurrentDialog = S17
    elseif choice == CurrentDialog["choices"]["choice3"] then
      end
    end
}

S17 = {
  say = "Wrong choice, Lieutenant! ",
}

CurrentDialog = S0