gossip = {
  say = "AAA Moge zwrocic na cos uwage ? Bo ten count robi dokladnie to samo co w pfmgetsums, suma, czyli przechodzi po wszystkim i jezeli klient jest naprawde duzy czyli chcielisbysmy ustawic ta flage to w tym momencie juz jest po ptakach bo ten count zrobi timeouta",
  continue = function () 
	CurrentDialog = gossip2
  end
}
gossip2 = {
  say = "A wlasnie ze nie !\n Nie zrobi.",
  continue = function () 
	CurrentDialog = gossip3
  end
}
gossip3 = {
  say = "Nie zrobi ?",
  continue = function () 
	CurrentDialog = gossip4
  end
}
gossip4 = {
  say = "Nie...\nBo Piotr zrobil takiego counta ktory nie zrobi timeouta",
  done = 1
}

CurrentDialog = gossip
