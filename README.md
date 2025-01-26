**********
**README**
**********

### =COMPILATION=   
Une bibliothèque partagée est incluse dans le projet.  
**Pour compiler le projet, écrire les commandes suivantes:**  
```
  make lib
  make app
```

L'exécutable codec du projet sera situé dans le dossier build/ du projet.

### =UTILISATION=  
Plusieurs options de la ligne de commande sont mis à disposition de l'utilisateur, comme demandé dans l'énoncé.  
L'option -h indiquera toutes les options possibles.  

**Pour encoder un fichier, utiliser l'option -c.**  
Exemple d'utilisation :  
```
LD_LIBRARY_PATH=build ./build/codec -i PGM/buzz.512.pgm -c
```

**Pour decoder un fichier, utiliser l'option -u.**  
Exemple d'utilisation :  
```
LD_LIBRARY_PATH=build ./build/codec -i QTC/buzz.512.qtc -u  
```

**Pour renommer le fichier de sortie, utiliser l'option -o.**  
Exemple d'utilisation :  
```
LD_LIBRARY_PATH=build ./build/codec -i QTC/texavery.512.qtc -o PGM/tex.pgm -u  
```

Le mode verbeux est disponible pour la compression et la décompression et affiche diverses informations  

===================================
