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
    //g.lireFichier("Graphe1.txt");
    //g.make_example();

    g=g.menu(g);

    //g.update1();
    //g.supprimer_pic();
    bool m;

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while (!key[KEY_ESC])
    {
        //g.Temporalite();

        while(key[KEY_A])
        {

            m==true;
            while(m==true)
            {
                std::cout<<"coucou"<<std::endl;
                g=g.menu(g);
            }
        }
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update(g);
        g.update1();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
    //g.sauvegarde(g);
    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


