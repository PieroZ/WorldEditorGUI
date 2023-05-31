function say_hi()	
	FactIncTable = {}
	FactIncTable['1'] = 'FactInc'
	FactIncTable['2'] = 'HunterInit'
	
	TestFunction(FactIncTable)
end

gossip = {
  say = "Oranges or apples? I really hope you prefer apples !",
    continue = function ()
    CurrentDialog = gossip2
  end
}
gossip2 = {
  choices = {
    choice0 = "Apples",
    choice1 = "Oranges"
  },
  onSelection = function (choice)
    if choice == CurrentDialog["choices"]["choice0"] then 
		CurrentDialog = gossip3
		FactInc = 1
    elseif choice == CurrentDialog["choices"]["choice1"] then
		CurrentDialog = gossip4 
		FactInc = 34
		end
    end
}
gossip3 = {
  say = "Oh me too ! That's good because otherwise I'd have to kill you",
    continue = function ()
    CurrentDialog = gossip5
  end
}
gossip4 = {
  say = "Too bad, I prefer apples!",
    continue = function ()
    CurrentDialog = gossip6
  end
}
gossip5 = {
  say = "Apples are simply amazing !",
}
gossip6 = {
  say = "If only you loved apples as much as I do !",
}

CurrentDialog = gossip
FactInc = 0

