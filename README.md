
Projet du 4ème cercle du cursus 42 

# Stratégie
1. Créer les fonctions correspondant aux 3(4?) actions possibles: réfléchir, manger, dormir (, mourir?)
2. Créer une structure qui sera instanciée pour chaque philosophe, contenant: action en cours, statut de la fourchette (prise ou non), moment du dernier repas, nombre de repas consommés, statut (vivant/mort).
3. Trouver un moyen de vérifier que chaque philosophe a bien eu le nombre minimum de repas passé en argument!! et qui arrête le programme une fois que c'est vrai.
4. Trouver un moyen de vérifier s'il y a eu un mort et de faire arrêter le programme si c'est vrai.
5. Créer un main qui instancie autant de philosophes que passé en argument.
6. Affiner la fonction "manger" avec la recherche de fourchettes et les mutex.

# Threads
En utilisant des threads, on peut "diviser le travail" dans un même programme pour qu'il s'exécute plus rapidement.
## Création d'un thread
```
pthread_t   your_thread;

pthread_create(&your_thread, param, your_function, (void *)arg);
```
Le premier argument doit être un pointeur sur pthread_t. Il faut donc en déclarer un avant d'appeler **pthread_create().**
Le ``param`` est généralement set à NULL.
L'``arg`` peut être NULL également. C'est ce qui sera envoyé en argument à ``your_function``.

**REMARQUE:** La fonction lancée par ``pthread_create`` reçoit ET retourne strictement un pointeur sur void.

## Faire attendre qu'un thread soit fini avant de poursuivre
```
pthread_join(*your_thread, param);
```
Le premier arguent doit etre un pthread_t dereference.
Le ``param`` est généralement set à NULL.

# Mutex
Si une variable doit etre utilisee par plusieurs threads, il faut s'assurer qu'ils n'y aient pas acces en meme temps, pour eviter les resultats fausses voire les crashs. C'est la que les mutex interviennent: ils servent a verouiller la variable jusqu'a ce que le premier thread a l'avoir atteinte ait fini sa tache avec elle, et a la deverouiller ensuite pour que le thread suivant puisse accomplir sa tache.

## Utiliser un mutex
1. Tout d'abord, il faut initialiser votre mutex.
2. Ensuite, il faut le verouiller juste avant l'acces a la variable.
3. Une fois la tache accomplie par le thread, il faut le deverouiller.
4. Une fois que vous n'en avez plus besoin (par ex. quand tous les threads ont termine la tache), il faut le detruire.

**A SAVOIR:**
Pour que le mutex soit lie a la variable a verouiller, il faut (je crois) les mettre dans une meme structure.
```
typedef struct s_your_mutex_struct
{
    your_type           your_var;
    pthread_mutex_t     your_mutex;
}                       t_your_mutex_struct;
```

## Initialisation d'un mutex
```
pthread_mutex_t your_mutex;

pthread_mutex_init(&your_mutex, param);
```
Le premier argument doit être un pointeur sur pthread_mutex_t. Il faut donc en déclarer un avant d'appeler **pthread_mutex_init().**
Le ``param`` est généralement set à NULL.

## Verouillage d'un mutex
```
pthread_mutex_lock(&your_mutex);
```
L'argument doit doit être un pointeur sur pthread_mutex_t.

## Deverouillage d'un mutex
```
pthread_mutex_unlock(&your_mutex);
```
L'argument doit doit être un pointeur sur pthread_mutex_t.

## Destruction d'un mutex
```
pthread_mutex_destroy(&your_mutex);
```
L'argument doit doit être un pointeur sur pthread_mutex_t.