gossip = {
  say = "Czy jestes w stanie mi pomoc?",
    continue = function ()
    CurrentDialog = gossip2
  end
}
gossip2 = {
  choices = {
    choice0 = "Nie",
    choice1 = "Tak"
  },
  onSelection = function (choice)
    if choice == CurrentDialog["choices"]["choice0"] then CurrentDialog = help_no
    elseif choice == CurrentDialog["choices"]["choice1"] then CurrentDialog = help_yes end
    end
}
help_yes = {
  say = "Dziekuje!",
}
help_no = {
  say = "Mialem nadzieje ze mi pomozesz ! :(",
}
CurrentDialog = gossip
