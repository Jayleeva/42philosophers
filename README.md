
Projet de groupe du 4ème cercle du cursus 42 

# Stratégie
1. Créer les fonctions correspondant aux 3(4?) actions possibles: réfléchir, manger, dormir (, mourir?)
2. Créer une structure qui sera instanciée pour chaque philosophe, contenant: action en cours, statut de la fourchette (prise ou non), moment du dernier repas, nombre de repas consommés, statut (vivant/mort).
3. Trouver un moyen de vérifier que chaque philosophe a bien eu le nombre minimum de repas passé en argument!! et qui arrête le programme une fois que c'est vrai.
4. Trouver un moyen de vérifier s'il y a eu un mort et de faire arrêter le programme si c'est vrai.
5. Créer un main qui instancie autant de philosophes que passé en argument.
6. Affiner la fonction "manger" avec la recherche de fourchettes et les mutex.

# Threads
En utilisant des threads, on peut "diviser le travail" dans un meme programme pour qu'il s'execute plus rapidement.
## Creation d'un thread
```
pthread_t   thread;

pthread_create(&thread, param, function, (void *)arg);
```
Le premier argument doit etre un pointeur sur pthread_t. Il faut donc en declarer voir l'allouer avant d'appeler pthread_create().
Le ``param`` est generalement set a NULL.
L'``arg`` peut etre NULL egalement. C'est ce qui sera envoye en argument a la ``function``.

**REMARQUE:** La fonction lancee par ``pthread_create`` recoit ET retourne strictement un pointeur sur void.

## Faire attendre qu'un thread soit fini avant de poursuivre
```
pthread_join(*thread, NULL);
```


## Suppression d'un thread

