function FactUpdates()
  ReadLuaTableValues(FactIncTable)
end

S0 = {
  say = "Bardzo dlugi tekst ktory nie zmiesci sie w jednym oknie
12345",
  continue = function ()
    CurrentDialog = S1
  end
}

S1 = {
  say = "kontynnuacja dlugiego dialogu
67890",
}

CurrentDialog = S0