#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();
    Graph mat1;
     std::string fichier;
     //std::string fichier_sauvegarde="test.txt";
    std::string nom ="Homme";
    std::string nom1 ="Rongeur";
    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
   //Graph g;
    //mat1.make_example();
    mat1.back_pic("test.txt");


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        mat1.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

    }

    grman::fermer_allegro();
    mat1.save_pic("test.txt");



    // std::cout<<"Entree le nom du fichier : Graphe1.txt"<<std::endl;
    // std::cin>>fichier;
    // std::cout<<std::endl;
    ///Appel des sous programmes
    //mat1.lireFichier(fichier);
    //mat1.afficher();


    return 0;
}
END_OF_MAIN();


