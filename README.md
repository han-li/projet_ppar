# PPAR Projet README(pair-impair)
Pair-impair sert a un projet qui tri un grand nombre de chiffre en parallels par MPI.

## Progres
D'abord, chaque PC trie ses propres chiffres en facon de trie PRAM dans 4 thread, puis le main assemble 4 tableaux en O(1), mais j'ai oublie comment il s'appele le facon de trie assemblee par main. Ensuite, chaque PC communique entre eux, ils transmettent les plus grands a droite, les plus petits a gauche, en facon de pair-impair. Les resultats sont ecrivent dans un fichier de texte. J'ai fini de coder toutes les codes et le programme marche bien dans le teste. Logiquement, si je utilise 16 PC de 4 thread chacun, l'acceleration est environ 64, tout ce que j'ai fait est pour approcher cette acceleration.

## Run test
make; make exec

## Doutes
La complexite de PRAM et pair-impair sont bien O(n^2), mais franchement le PRAM a une boucle de n^2, par contre pair-impair a que n^2/2, alors je prefere utiliser pair-impair pour trie locale aussi. Dans le programme, la fonction sort est trie locale de pair-impair, et sort2 est trie locale de PRAM.
