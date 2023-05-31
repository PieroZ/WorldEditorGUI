function FactUpdates()
  ReadLuaTableValues(FactIncTable)
end

function PickStartingPoint()
	CheckFactsTable = {}
	CheckFactsTable['Q_TALK_TO_NPC_3'] = 1
	result = CheckLuaFacts(CheckFactsTable)
	if result == true then
		CurrentDialog = S1 
	end
	if result == false then
		CurrentDialog = S0 
	end
end

S0 = {
  say = "Please leave me alone.",
}

S1 = {
  say = "So NPC_01 sent you to talk to me ? Very well, let's get down to business.",
}

