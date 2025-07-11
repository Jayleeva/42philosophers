
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

*Si les arguments passes a l'execution sont stockes dans la structure principale, on doit pouvoir accéder à la liste chaînée depuis la structure principale et accéder à la structure principale depuis n'importe quel chaînon de la liste chaînée.*

Aussi, la liste chaînée doit être circulaire: le dernier chaînon doit pointer sur le premier, afin qu'il puisse lui aussi accéder à une deuxième fourchette. Rappelez-vous que le premier philosophe a besoin de sa propre fourchette (1) et de celle de son voisin (2), et que le deuxième philsophe a besoin de sa propre fourchette (2) et de celle de son voisin (3), et ainsi de suite, le dernier philosophe ayant besoin de sa propre fourchette (ex. 5) et de celle de son voisin (1). C'est cela même qui fait que vous aurez besoin de mutex: quand 1 mange, 2 ne peut pas manger, quand 2 mange, 1 ne peut pas manger, etc, quand le dernier (ex.5) mange, 1 ne peut pas manger. Votre programme doit fonctionner de la même manière peut importe le nombre de philosophes, pair ou impair.

Enfin, l'ordre des actions est le suivant: manger, dormir, penser (répéter). Seulement, pour mieux répartir les threads, on en fait commencer un sur deux par dormir; de plus, l'action de penser consiste en réalité en un simple print (ou ajouter un usleep()?): il faut la voir comme une action tampon le temps que les fourchettes se libèrent et qu'on puisse manger. Ainsi, vous pourriez décider de les faire commencer par penser, cela respecterait également le sujet, du moment que vous n'écrivez *is thinking* qu'une fois par action de penser.

# Conseils
Pour être moins submergé-e, commencez peut-être par un programme plus simple, par ex.: 3 threads, le premier qui complète la modification d'une var à laquelle ils ont tous accès (par ex.: changer une string "hello" en "goodbye", incrémenter un int de 1 jusqu'à arriver à 10, ...), met fin au programme. Une fois que vous êtes sûr-e que ça marche, rajoutez une couche, par ex.: le nombre de threads à créer est passé en argument, chaque thread a un temps limité pour réussir, etc. Et rapprochez-vous petit à petit du vrai programme.

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
Le premier argument doit être un pthread_t déréférencé.
Le ``param`` est généralement set à NULL.

# Mutex
Si une variable doit etre utilisee par plusieurs threads, il faut s'assurer qu'ils n'y aient pas acces en meme temps (data race / race condition), pour eviter les resultats fausses voire les crashs. C'est la que les mutex interviennent: ils servent a verouiller une partie du code jusqu'a ce que le premier thread a l'avoir atteinte ait fini sa tache avec elle, et a la deverouiller ensuite pour que le thread suivant puisse accomplir sa tache.

## Utiliser un mutex
1. Tout d'abord, il faut initialiser votre mutex.
2. Ensuite, il faut le verouiller juste avant l'acces a la variable commune.
3. Une fois la tache accomplie par le thread, il faut le deverouiller.
4. Une fois que vous n'en avez plus besoin (typiquement: fin du programme), il faut le detruire.

## Initialisation d'un mutex
```
pthread_mutex_t your_mutex;

pthread_mutex_init(&your_mutex, param);
```
Le premier argument doit être un pointeur sur pthread_mutex_t. Il faut donc en déclarer un avant d'appeler **pthread_mutex_init().**
Le ``param`` est généralement set à NULL.

## Verouillage d'un mutex
```
if (!pthread_mutex_lock(&your_mutex))
```
L'argument doit être un pointeur sur pthread_mutex_t. En cas de succes, la fonction retourne 0.

On appelle la fonction dans une condition ``if (!)`` : si la fonction retourne autre chose que 0, c'est que le lock n'a pas fonctionne, donc possiblement que le mutex a deja ete locked par un autre thread. Si elle retourne 0, le lock a fonctionne, et tout ce qui est defini dans le if ne sera accessible que par le thread qui a pu lock. Le mutex n'est donc pas lie a une variable en particulier: il se fait dans une condition qui si vraie, bloque l'acces a la partie du code contenu entre lock et unlock pour les autres threads. Cependant, s'il y a plusieurs variables communes dans votre projet (ce qui est le cas dans philosophers), vous aurez besoin de plusieurs mutex, pour que le lock d'une partie du code n'empeche pas le lock d'une autre. 

## Deverouillage d'un mutex
```
pthread_mutex_unlock(&your_mutex);
```
L'argument doit être un pointeur sur pthread_mutex_t.

A(aux) la sortie(s) du if (!lock), il faut unlock. 

## Destruction d'un mutex
```
pthread_mutex_destroy(&your_mutex);
```
L'argument doit être un pointeur sur pthread_mutex_t.

# Race condition ou data race
Quand plusieurs threads tentent d'acceder a la meme variable, il y a ce qu'on appelle des "data race", ou "race conditions". On les empeche avec des mutex soigneusement initialises, places, et detruits. Tout mutex lock doit etre unlock avant la fin du programme.

**REMARQUE:**
Attention a ne pas destroy les mutex avant que tous vos threads soient joined.

Pour vérifier si vous avez des data race, utilisez l'outil DRD ou helgrind de Valgrind. Helgrind est plus facile à lire.

Pour utiliser un outil valgrind, on ajoute le flag ``--tool=`` et l'outil en question: ``valgrind --tool=drd`` ou ``valgrind --tool=helgrind``. 
