#include "grman/grman.h"
#include <iostream>
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");


    /// Un exemple de graphe
    Graph g;

    std::string fichier;
    std::cout<<"Entree le nom du fichier : Graphe1.txt, Graphe2.txt ou Graphe3.txt"<<std::endl;
    std::cin>>fichier;
    std::cout<<std::endl;
    ///Appel des sous programmes
    //g.lireFichier(fichier);
    //g.make_example();
    if (fichier == "Graphe1.txt")
    {
        g.back_pic("Saves1.txt");
    }
     if (fichier == "Graphe2.txt")
    {
        g.back_pic("Saves2.txt");
    }
     if (fichier == "Graphe3.txt")
    {
        g.back_pic("Saves3.txt");
    }

    g.menu();

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
     if (fichier == "Graphe1.txt")
    {
        g.save_pic("Saves1.txt");
    }
     if (fichier == "Graphe2.txt")
    {
        g.save_pic("Saves2.txt");
    }
     if (fichier == "Graphe3.txt")
    {
        g.save_pic("Saves3.txt");
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


