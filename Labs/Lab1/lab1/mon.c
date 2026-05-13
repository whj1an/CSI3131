#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>


/* l'execution du programme commence ici */
int main(int argc, char **argv)
{
    char    *program;

    if (argc != 2) {
        printf("Usage: mon <nomFichier>\n - nomFichier est le nom d'un fichier executable.\n");
        exit(-1);
    } else
        program = argv[1];

    /* Maintenant pour votre code.*/
    /* Il devrait faire le suivant:
        1. lancer un processus qui roule le programme specifie dans la variable 'program' et obtenir son pid
        2. lancer 'procmon pid' - pid est le pid du processus lance dans etape 1.
        3. attendre 20 secondes
        4. tuer (kill) le premier processus
        5. attendre 2 secondes
        6. tuer le processus roulant procmon
    */
}
