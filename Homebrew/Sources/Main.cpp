#include <3ds.h>
#include "App.hpp"

int main() {
    return App::main();
}

/*
TODO
- faire l'appli desktop
- marquer latence reçue, et packet loss

CODE
- mettre les variables consts dans les namesapces / class
- mettre les * et & des types à gauche de la variable
- quand on escape le keyboard, ça considère qu'on a confimé le bouton en contournant le callback de vérif (EMU ONLY)
- faire un truc pour dire que ça a aucun intéret sur emu ?

PLUS TARD
- faire l'envoie de texte ? avec l'ouverture du clavier et tout
- ajouter le micro ? (micInit)
*/