function FactUpdates()
  ReadLuaTableValues(FactIncTable)
end

S0 = {
  say = "Dialog bez impaktu na gre z wyborem gracza odpowiedzi",
}

S1 = {
  choices = {
    choice0 = "Kedar wez sie ogarnij",
    choice1 = "Kedar Twoj wklad w CEKE jest nieoceniony"
  },
  onSelection = function (choice)
    if choice == CurrentDialog["choices"]["choice0"] then
    elseif choice == CurrentDialog["choices"]["choice1"] then
      end
    end
}

S3 = {
  say = "OK, sorry",
}

S4 = {
  say = "Dzieki!",
}

CurrentDialog = S0