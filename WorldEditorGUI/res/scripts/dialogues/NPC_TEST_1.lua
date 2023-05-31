function FactUpdates()
  ReadLuaTableValues(FactIncTable)
end

S1 = {
  say = "Did you ever hear the Tragedy of Darth Plagueis the wise?",
    facts_update = function()
      FactIncTable = {}
      FactIncTable["HUNTER_INIT"] = 1
      FactIncTable["HUNTER_QUEST_IN_PROGRESS"] = 1
      FactUpdates()
    end,
  continue = function ()
    CurrentDialog = S2
  end,
}

S2 = {
  say = "I thought not. It's not a story the Jedi would tell you. ",
    facts_update = function()
      FactIncTable = {}
      FactIncTable["TE"] = 1
      FactUpdates()
    end,
  continue = function ()
    CurrentDialog = S3
  end,
}

S3 = {
  say = "It's a Sith legend.",
  continue = function ()
    CurrentDialog = S4
  end,
}

S4 = {
  say = "Darth Plagueis was a Dark Lord of the Sith, so powerful and so wise he could use the Force to influence the midichlorians to create life... ",
  done = 1,
}

CurrentDialog = S1