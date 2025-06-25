
Projet du 4ème cercle du cursus 42 

# Stratégie
1. Créer une structure principale contenant les arguments reçus à l'exécution, un booléen signalant la mort d'au moins un philosophe, les mutex généraux, ainsi que le chaînon de tête (donc un pointeur) de la liste chaînée (point 2)
2. Créer une liste chaînée (et donc une structure) listant les philosophes; la structure contient l'id du philo, l'id du thread, sa fourchette et le mutex de sa fourchette, le nombre de repas consommés, le moment du dernier repas, et un pointeur sur la structure principale.
3. Créer un main qui instancie les structures, initialise les variables et les mutex, lance la simulation (allocation et création des threads, lancement de la routine) et gère son arrêt (join les thread, destroy les mutex, free la mémoire allouée).
4. Créer une fonction qui imprime les output avec un mutex pour s'assurer que chaque thread attende son tour avant d'écrire: on évite ainsi une sortie chaotique.
5. Créer la fonction routine(): vérifier s'il n'y a qu'un seul philosophe, si oui usleep(time_to_die) puis le faire mourir (death()), sinon, on continue. Assigner le temps actuel au temps du dernier repas. Si l'id du philosophe est pair, le faire dormir (go_sleep()) afin de donner des départs différents une fois sur deux, sinon on continue. Vérifier en boucle si une des conditions de fin est déjà atteinte, si oui return, sinon on continue. Vérifier si le philosophe du thread devrait être mort (temps actuel - moment du dernier repas >= time_to_die), si oui, le faire mourir (death()), sinon, on continue: tenter de le faire manger (try_eating()).
6. Créer la fonction try_eating(): tenter de lock sa fourchette, si réussi écrire qu'il a une fourchette (*has taken a fork*), tenter de lock la prochaine, si réussi écrire qu'il a une fourchette (*has taken a fork*), écrire qu'il mange (*is eating*), réassigner le temps actuel au temps du dernier repas, puis usleep(time_to_eat) et incrémenter le nombre de repas consommés, unlock les fourchettes, le faire dormir (go_sleep()).
7. Créer la fonction go_sleep(): vérifier si une condition de fin a été atteinte, si oui return, sinon on continue. Ecrire qu'il dort (*is sleeping*), usleep(time_to_sleep). Au réveil, vérifier à nouveau si une condition de fin a été atteinte, si oui return, sinon on continue: écrire qu'il pense (*is thinking*).
8. Créer la fonction death(): lock le boléen et le passer à TRUE, unlock et écrire que le philo est mort (*is dead*).
9. Créer la fonction has_ended(): vérifier avec un mutex si le boléen est à TRUE, si oui return TRUE, sinon on continue. Vérifier avec un mutex si chaque philo a consommé au moins autant que le nombre de repas min reçu en argument, si oui return TRUE. Si aucune des deux conditions, return FALSE.

**REMARQUE:** Oui, la structure principale contient le chaînon de tête (donc un pointeur) de la liste chaînée, et la structure de la liste chaînée contient un pointeur sur la structure principale. Cela peut paraître bizarre, mais c'est nécessaire pour que l'on puisse lancer les threads et accéder à toutes les infos tout au long du programme. 

*On doit pouvoir accéder à la liste chaînée depuis la structure principale et accéder à la structure principale depuis n'importe quel chaînon de la liste chaînée.*

Aussi, la liste chaînée doit être circulaire: le dernier chaînon doit pointer sur le premier, afin qu'il puisse lui aussi accéder à une deuxième fourchette. Rappelez-vous que le premier philosophe a besoin de sa propre fourchette (1) et de celle de son voisin (2), et que le deuxième philsophe a besoin de sa propre fourchette (2) et de celle de son voisin (3), et ainsi de suite, le dernier philosophe ayant besoin de sa propre fourchette (ex. 5) et de celle de son voisin (1). C'est cela même qui fait que vous aurez besoin de mutex: quand 1 mange, 2 ne peut pas manger, quand 2 mange, 1 ne peut pas manger, etc, quand le dernier (ex.5) mange, 1 ne peut pas manger. Votre programme doit fonctionner de la même manière peut importe le nombre de philosophes, pair ou impair.

Enfin, l'ordre des actions est le suivant: manger, dormir, penser (répéter). Seulement, pour mieux répartir les threads, on en fait commencer un sur deux par dormir; de plus, l'action de penser consiste en réalité en un simple print (ou ajouter un usleep()?): il faut la voir comme une action tampon le temps que les fourchettes se libèrent et qu'on puisse manger. Ainsi, vous pourriez décider de les faire commencer par penser, cela respecterait également le sujet, du moment que vous n'écrivez *is thinking* qu'une fois par action de penser.

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