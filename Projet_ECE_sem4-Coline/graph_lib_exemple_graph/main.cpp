#include "grman/grman.h"
#include <iostream>
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    //grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    //grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;

    //g.back_pic("Saves2.txt");
    //g.Kosaraju("Saves2.txt"); /// erreur a adapter
    ///Appel des sous programmes
    g.lireFichier("Saves2.txt");
    //g.make_example();

    //g=g.menu(g);
    //g.ajout_pic();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    /*while (!key[KEY_ESC])
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();
        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
    g.sauvegarde(g);
    grman::fermer_allegro();*/

    return 0;
}
END_OF_MAIN();


