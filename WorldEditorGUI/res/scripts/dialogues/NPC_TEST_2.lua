gossip = {
  say = "Witam czy my sie skads znamy",
    continue = function ()
    CurrentDialog = gossip2
  end
}
gossip2 = {
  choices = {
    choice0 = "Bardzo mozliwe, czesto tu bywam",
    choice1 = "Nie sadze",
    choice2 = "omae wa mou shindeiru"
  },
  onSelection = function (choice)
    if choice == CurrentDialog["choices"]["choice0"] then CurrentDialog = gossip3
    elseif choice == CurrentDialog["choices"]["choice1"] then CurrentDialog = gossip4
    elseif choice == CurrentDialog["choices"]["choice2"] then CurrentDialog = gossip5 end
    end
}
gossip3 = {
  say = "Tak, pamietam Cie! Pomogles nam pokonac armie szczurow atakujacych Carlin! Czym moge Ci pomoc?",
}
gossip4 = {
  say = "Musialem Cie z kims pomylic, wybacz. Czym moge sluzyc?",
}
gossip5 = {
  say = "NANI?!",
}
CurrentDialog = gossip
