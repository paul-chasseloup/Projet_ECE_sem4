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
    g.make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();
    Graph mat1;
    std::string fichier;
    std::string nom ="Homme";
    std::string nom1 ="Rongeur";

    // std::cout<<"Entree le nom du fichier : Graphe1.txt"<<std::endl;
    // std::cin>>fichier;
    // std::cout<<std::endl;
    ///Appel des sous programmes
    //mat1.lireFichier(fichier);
    //mat1.afficher();
    mat1.Sauvegarde(fichier);

    return 0;
}
END_OF_MAIN();


