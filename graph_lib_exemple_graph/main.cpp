#include "grman/grman.h"
#include <iostream>
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();
    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");
    /// Un exemple de graphe
    Graph g;
    ///Appel des sous programmes
    g.menu();
    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    g.boucle();
    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();


