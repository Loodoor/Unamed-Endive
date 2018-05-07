Comment commencer
============
Premi�rement, cr�ez un fichier `.py` dans le dossier `assets/scripts`, que vous nommerez selon vos envies.

Puis ouvrez-le dans un �diteur de scripts (comme [Notepad++](https://notepad-plus-plus.org/)), nous allons �crire notre premier code pour le jeu :3 !

Pour inclure quelques m�thodes "built in" pour modifier le jeu, nous allons inclure le module `Unamed` comme ceci :

```py
import Unamed
```

Puis nous allons d�clarer le type de fichier (� quoi servira-t-il) dans le fichier `register.py` : 

```py
Unamed.registerScript(type, nom)
```

`type` et `nom` devraient �tre des cha�nes de caract�res.  Pour le nom, il **doit** �tre le nom complet de votre script (par exemple `"myscript.py"`). Pour le type de script, cela va d�pendre de comment celui-ci va fonctionner:

* juste une fois, � l'ouverture du jeu ? utilisez `runOnceWhenStarting`

* juste une fois, � la fermeture du jeu ? utilisez `runOnceWhenClosing`

* chaque fois que le jeu est mis � jour ? utilisez `runWhenUpdatingGame`

* chaque fois que le jeu lance les �v�nements, vous avez besoin d'aller chercher ces �v�nements ? utilisez `runWhenProcessingEvents`

* chaque fois que le jeu affiche quelque chose ? utilisez `runWhenRenderingView`

Appeler l'ordre qui concerne le type de script
=============================================

* `runOnceWhenStarting`, seulement une fois, au lancement du jeu

puis :arrow_right: 

Pendant que le jeu tourne :

* `runWhenUpdatingGame`, chaque fois que le jeu est mis � jour

puis :arrow_right: 

* `runWhenProcessingEvents`, chaque fois que le jeu re�oit un �v�nement

puis :arrow_right: 

* `runWhenRenderingView`, chaque fois qu'une vue est affich�e

puis :arrow_right: 

* `runOnceWhenClosing`, seulement une fois, � la fermeture du jeu

Notez que les scripts de type `runOnceWhenStarting` ne peuvent �tre lanc�s que lorsque tous les autres composants du jeu (comme le joueur, son sac, son �quipe et tout ce dont il a besoin  ; ainsi que les maps, et les vues ... etc) sont charg�s, donc il est possible d'appliquer directement des modifications � l'�quipe ou au joueur avant que le jeu ne commence vraiment.

Ce que nous pouvons faire et ce que nous ne pouvons pas faire
==================================
Les scripts ayant cette forme : 

* `runOnceWhenStarting`

* `runOnceWhenClosing`

* `runWhenUpdatingGame`

* `runWhenProcessingEvents`

* `runWhenRenderingView`

ne devraient pas, ou le moins possible , et doivent tourner en vraiment peu de temps, parce qu'ils ne peuvent pas �tre mis sur un "fil" s�par�.

Script commen�ant par runWhen... mais qui a besoin de valeurs globales
=================================================
Si votre script devrait tourner lorsque le jeu se met � jour, mais qu'il a besoin d'une valeur globale, vous pouvez penser que votre script ne gardera pas la valeur entre chaque frame. Donc vous pouvez utiliser cette m�thode pour cr�er une valeur globale: 

```py
import Unamed

Unamed.createGlobal(name, value)
```

Le code C++ qui s'en occupe essaiera de d�duire le type lui-m�me ;) il retournera 1 si c'est un succ�s, sinon 0 (par exemple si 'name' existe d�j� dans le tableau des variables globales). `name` devrait �tre un 'string'.

Et quand vous voulez obtenir une valeur globale, vous avez juste � faire ceci : 

```py
import Unamed

Unamed.getGlobal(name)
```

avec `name` l'identifiant de votre variable globale (c'est aussi un 'string')

Mais il y a une autre astuce qui est que le moteur de script charge les fichiers en texte brut, et puis envoie ce "texte" � Python. So, what it means is that you can totally do this, sans aucun souci : 

fichier 1:
```py
import Unamed

# some actions

global_var = 1
```

fichier 2:
```py
import Unamed

# some actions

print(global_var)
```

Parce que c'est pareil que :+1: 
```py
import Unamed

# some actions

global_var = 1

import Unamed

# some actions

print(global_var)
```

Parce que les fichiers sont lus par le m�me interpr�teur, les uns apr�s les autres, et qu'ils ne sont pas encha�n�s (Donc, vous avez juste � appeler le module Unamed dans register.py et vous pouvez l'utiliser sans l'importer)

mais soyez s�r que le fichier 1 sera appel� avant le fichier 2, en v�rifiant l'ordre d'appel de ces scripts.

Chargement et affichage d'images
=============================
Rien de plus simple : 

loader.py (d�fini comme `runOnceWhenStarting` dans register.py)
```py
import Unamed

Unamed.loadImage("path/to/image.png", "id")
```

display.py (d�fini comme `runWhenRenderingView` dans register.py)
```py
import Unamed

Unamed.displayImage("id", 150, 140)
```

Jouer avec les musiques
=======================
Tout le syst�me a �t� simplifi�, donc vous avez une tr�s petite interface comme d�crit ci-dessous : 

* `Unamed.playMusic(name)` avec `name` le nom de la chanson, comme "01.ogg" par exemple
* `Unamed.stopMusic()` qui stoppe la musique, si une musique est jou�e

* `Unamed.getCurrentMusicName()` qui retourne le nom de la musique actuellement jou�e

Jouer avec la map
====================
Maintenant, vous pouvez obtenir la largeur et la hauteur de la map comme ceci :

* `Unamed.getMapWidth()`

* `Unamed.getMapHeight()`

Vous pouvez aussi v�rifier si un block est soit une arriv�e (un spawn) d'une autre map, soit une t�l�portation pour aller sur une autre map :

* `Unamed.isSpawn(x, y)` avec `x` and `y` repr�sente la position du block, en blocks (pas en pixels !). il retournera `1` si le block est un spawn, sinon `0`

* `Unamed.isTp(x, y)` avec `x` and `y` repr�sente la position du block, en blocks (pas en pixels !). il retournera `1` si le block est une t�l�portation, sinon 0

Puis vous pouvez �galement obtenir la position du spawn o� vous arrivez sur une map d'ID X comme ceci : `Unamed.getSpawnPosFromMapId(X)`. il retourne un tuple of two integers si un spawn avec la map id X est trouv�, sinon `-1`.

Finalement vous pouvez obtenir le lieu o� vous arrivez avec une TP, � une position donn�e : `Unamed.getMapIdFromTpPos(x, y)` avec `x` and `y` 2 entiers. Si une TP `x, y` n'est pas trouv�, il retournera `-1`.

Valeurs globales li�es au jeu
===========================
Si vous avez besoin de savoir quel est l'ID de vue actuel, par exemple pour estimer si vous devez afficher une image, vous pouvez utiliser cette m�thode: `Unamed.getCurrentViewId ()` qui renvoie un entier repr�sentant l'ID de vue (reportez-vous � la page `View` sur ce wiki pour en savoir plus sur eux

Aussi, si vous voulez v�rifier si la vue actuelle a un HUD actif traitant des �v�nements / affichage, utilisez `Unamed.hasActiveHud (vid)` avec `vid` un view_id (pour savoir si la vue actuelle a un HUD actif, vous pouvez utiliser `Unamed.hasActiveHud (Unamed.getCurrentView ())`)

Cr�er un PNJ
==============
Rien n'est plus facile que de cr�er un PNJ : 

```py
import Unamed

Unamed.createPNJ(0, "test", "je suis un test pnj", 0, "Test PNJ", 32, 32)
```

Liste des arguments : 

* `map id` : integer

* `folder` : string (ils sont tous dans `assets/pnj`)

* `text` : string (ce que le PNJ dira)

* `kind` : integer (allez sur la page PNJ du wiki pour en savoir plus)

* `display name` : string (Nom qui sera affich�)

* `x` : integer (pixels, position relative du coin sup�rieur gauche de la map)

* `y` : integer (pixels, position relative du coin sup�rieur gauche de la map)

Obtenir `sf::Event`s
====================
Ca ne peut pas �tre int�ressant si nous ne pouvons pas jouer avec les �vents que le jeu ex�cute, non?

Pour r�cup�rer le dernier Event que le jeu a ex�cut�, marquez votre script comme `runWhenProcessingEvents`, et puis vous devriez faire �a : 

```py
import Unamed

ev = Unamed.getEvent()
```

`Unamed.getEvent()` retourne un `dict`. Enfin, pour v�rifier si il a une cl� sp�cifique et retrouver sa valeur associ�e, utilisez la m�thode `.get(key, [default value])`.
Pour obtenir le type d'un �vent, vous pouvez �crire: 

```
type_ev = ev.get('type')
if type_ev:
    # nous pouvons l'ex�cuter, il a un type
```

Les types d'�vents support�s sont ici (notez qu'ils sont �crits en caract�res minuscules) : 

* closed
* resized
* lostfocus
* gainedfocus
* textentered => `{'unicode': character}`
* keypressed => `{'key': keycode, 'alt': bool, 'control': bool, 'shift': bool, 'system': bool}`
* keyreleased => pareil que keypressed
* mousewheelscrolled => `{'vertical': bool, 'delta': int, 'x': int, 'y': int}`
* mousebuttonpressed => `{'button': int, 'x': int, 'y': int}`
* mousebuttonreleased => pareil que mousebuttonpressed
* mousemoved => `{'x': int, 'y': int}`
* mouseentered
* mouseleft
* unrecognized => pour tous les autres �vents non-support�s par le jeu (principalement li�s au joystick)

Si vous n'�tes pas familier avec la SFML, cela peut vous aider � mieux comprendre comment fonctionnent les �vents : [SFML doc on sf::Event](https://www.sfml-dev.org/tutorials/2.4/window-events.php)