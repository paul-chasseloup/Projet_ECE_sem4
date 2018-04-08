#include "graph.h"
#include <allegro.h>
#include <time.h>
#include <fstream>
#include <map>
#include <queue>
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    /// La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    /// Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  /// Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    /// Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    /// Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    /// Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}
/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}
/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}
/***************************************************
                    EDGE
****************************************************/

EdgeInterface::EdgeInterface(Vertex& from, Vertex& to,int numS1, int numS2)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    ///variable qui permet d'afficher les sommmets des arêtes
    std::string name;
    std::string sommet1;
    std::string sommet2;

    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(40,75);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 200.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(10,45);
    m_slider_weight.set_pos(13,13);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_pos(15,65);

    ///croix pour supprimer une arête
    m_box_edge.add_child(m_cross);
    m_cross.set_bg_color(BLEUCLAIR);
    m_cross.set_dim(7,7);
    m_cross.set_pos(0,63);

    ///texte qui affiche les sommets de l'arête
    m_box_edge.add_child(m_text_number);
    sommet1=std::to_string(numS1);
    sommet2=std::to_string(numS2);
    name=sommet1+"-"+sommet2;
    m_text_number.set_pos(0,1);
    m_text_number.set_message(name);
}
/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}
/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}
/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    ///Paramétrage de l'ensemble des boutons
    ///Bouton suppression de sommet
    m_top_box.add_child(m_supprimer);
    m_supprimer.add_child(m_text_supprimer);
    m_supprimer.set_dim(80,40);
    m_supprimer.set_posx(0);
    m_supprimer.set_posy(0);
    m_supprimer.set_bg_color(ROUGE);
    m_text_supprimer.set_message("SUP SOMMET");

    ///Bouton ajout de sommet
    m_top_box.add_child(m_ajouter_sommet);
    m_ajouter_sommet.add_child(m_text_ajouter_sommet);
    m_ajouter_sommet.set_dim(80,40);
    m_ajouter_sommet.set_posx(0);
    m_ajouter_sommet.set_posy(80);
    m_ajouter_sommet.set_bg_color(FUCHSIA);
    m_text_ajouter_sommet.set_message("ADD SOMMET");

    ///Bouton ajout d'arete
    m_top_box.add_child(m_ajouter_arete);
    m_ajouter_arete.add_child(m_text_ajouter_arete);
    m_ajouter_arete.set_dim(80,40);
    m_ajouter_arete.set_posx(0);
    m_ajouter_arete.set_posy(120);
    m_ajouter_arete.set_bg_color(ROUGECLAIR);
    m_text_ajouter_arete.set_message("ADD ARETE");

    ///Bouton de sauvegarde
    m_top_box.add_child(m_sauvegarde);
    m_sauvegarde.add_child(m_text_sauvegarde);
    m_sauvegarde.set_dim(80,40);
    m_sauvegarde.set_posx(0);
    m_sauvegarde.set_posy(160);
    m_sauvegarde.set_bg_color(BLEUCLAIR);
    m_text_sauvegarde.set_message("SAVE");

    ///Bouton pour quitter
    m_top_box.add_child(m_quitter);
    m_quitter.add_child(m_text_quitter);
    m_quitter.set_dim(80,40);
    m_quitter.set_posx(0);
    m_quitter.set_posy(280);
    m_quitter.set_bg_color(VIOLETCLAIR);
    m_text_quitter.set_message("QUITTER");

    ///Bouton suppression d'arete
    m_top_box.add_child(m_supprimer_arete);
    m_supprimer_arete.add_child(m_text_supprimer_arete);
    m_supprimer_arete.set_dim(80,40);
    m_supprimer_arete.set_posx(0);
    m_supprimer_arete.set_posy(40);
    m_supprimer_arete.set_bg_color(GRISCLAIR);
    m_text_supprimer_arete.set_message("SUP ARETE");

    ///Bouton pour visualiser la forte connexité
    m_top_box.add_child(m_connexite);
    m_connexite.add_child(m_text_connexite);
    m_connexite.set_dim(80,40);
    m_connexite.set_posx(0);
    m_connexite.set_posy(200);
    m_connexite.set_bg_color(BLANCBLEU);
    m_text_connexite.set_message("CONNEXITE");

    ///Bouton pour visualiser l'évolution temporelle
    m_top_box.add_child(m_flux);
    m_flux.add_child(m_text_flux);
    m_flux.set_dim(80,40);
    m_flux.set_posx(0);
    m_flux.set_posy(240);
    m_flux.set_bg_color(JAUNECLAIR);
    m_text_flux.set_message("TEMP");
}

///BOUCLE
void Graph::boucle()
{
    /// tant qu'on n'appuie pas sur Echap
    while (!key[KEY_ESC])
    {
        /// si on clique sur le bouton QUITTER
        if(m_interface->m_quitter.clicked())
        {
            ///On retourne au menu
            vider_graph();
            vider_box();
            menu();

        }
        /// si on clique sur le bouton ADD SOMMET
        if(m_interface->m_ajouter_sommet.clicked())
        {
            ///On ajoute un sommet
            ajout_pic();
        }
        /// si on clique sur le bouton SUPP SOMMET
        if(m_interface->m_supprimer.clicked())
        {
            ///On supprime un vertexe
            supprimer_pic();
        }
        /// si on clique sur le bouton SAVE
        if(m_interface->m_sauvegarde.clicked())
        {
            /// on sauvegarde
            sauvegarde();
        }
        /// si on clique sur le bouton ADD ARETE
        if (m_interface->m_ajouter_arete.clicked())
        {
            /// On ajoute un edge
            ajouter_edge();
        }
        /// si on clique sur le bouton SUPP ARETE
        if (m_interface->m_supprimer_arete.clicked())
        {
            /// On supprime un edge
            supprimer_arete();
        }
        /// si on clique sur le bouton CONNEXITE
        if (m_interface->m_connexite.clicked())
        {
            ///On affiche le graphe réduit
            afficher_connex();
            init_k_connex();
        }
        /// si on clique sur le bouton TEMP
        if (m_interface->m_flux.clicked())
        {
            /// On lance le programme de temporalite
            Temporalite();
        }
        ///on update à chaque tour de boucle
        update1();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
}

///VIDER LE GRAPHE
void Graph::vider_graph()
{
    /// On déclare un vecteur d'int temporaire qui reçoit les indices des sommets à supprimer
    std::vector<int> temp;
    /// On remplit le vecteur avec les elements de la map de m_vertices
    for(std::map<int,Vertex>::iterator i=m_vertices.begin(); i!=m_vertices.end(); i++)
    {
        temp.push_back(i->first);
    }
    ///On supprime chaque element du vecteur
    for(auto elem : temp)
    {
        supprimer_pic_idx(elem);
    }
}

///SETTER DE FLECHE
///sous-programme d'élargissement des flèches en fonction du poids des aretes
void Edge::set_fleche(float a)
{
    m_interface->m_top_edge.set_fleche(a);
}
void Graph::set_fleche()
{
    for(auto& elem: m_edges)
    {
        elem.second.set_fleche(elem.second.m_weight);
    }
}

///UPDATE DU GRAPHE
void Graph::update1()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    ///Ici, on appelle le setter flèche
    this->set_fleche();
}

///AJOUT D'UN EDGE
void Graph::ajouter_edge()
{
    std::string nom;
    int lien_1, lien_2,poids;
    std::cout<<"Vous voulez creer un nouvel element " << std::endl;
    std::cout<<"Veuillez entrer ses liens : "<< std::endl;
    std::cout<<"lien 1 : ";
    std::cin>>lien_1;
    std::cout<<std::endl;
    std::cout<<"lien 2 : ";
    std::cin>>lien_2;
    std::cout<<std::endl;
    std::cout<< "poids :";
    std::cin>>poids;
    add_interfaced_edge(0,lien_1,lien_2,poids);
}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    while ( m_vertices.find(idx)!=m_vertices.end() )
    {
        idx++;
    }
    /// Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    /// Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    /// On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);

}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    while ( m_edges.find(idx)!=m_edges.end() )
    {
        idx++;
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2],id_vert1,id_vert2);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from =id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

void Graph::menu()
{
    BITMAP *page2=NULL,*selec0=NULL,*selec1=NULL,*selec2=NULL,*selec3=NULL,*selec4=NULL;
    page2=create_bitmap(1228,748);
    selec0=load_bitmap("menu 0.bmp",NULL);
    if(selec0 == NULL)
    {
        printf("Erreur de chargement menu 0.bmp");
        exit(0);
    }
    selec1=load_bitmap("menu 1.bmp",NULL);
    if(selec1 == NULL)
    {
        printf("Erreur de chargement menu 1.bmp");
        exit(0);
    }
    selec2=load_bitmap("menu 2.bmp",NULL);
    if(selec2 == NULL)
    {
        printf("Erreur de chargement menu 2.bmp");
        exit(0);
    }
    selec3=load_bitmap("menu 3.bmp",NULL);
    if(selec3 == NULL)
    {
        printf("Erreur de chargement menu 3.bmp");
        exit(0);
    }
    selec4=load_bitmap("menu 4.bmp",NULL);
    if(selec4 == NULL)
    {
        printf("Erreur de chargement menu 4.bmp");
        exit(0);
    }

    int selec=0;
    while ((selec!=4&&selec!=3&&selec!=2&&selec!=1)||!mouse_b&1)
    {
        if (20<=mouse_x && mouse_x<=310 && 494<=mouse_y && mouse_y<=546)
        {
            selec=1;
            draw_sprite(page2,selec1,0,0);
        }
        else if (20<=mouse_x && mouse_x<=310 && 547<=mouse_y && mouse_y<=593)
        {
            selec=2;
            draw_sprite(page2,selec2,0,0);
        }
        else if (20<=mouse_x && mouse_x<=310 && 594<=mouse_y && mouse_y<=645)
        {
            selec=3;
            draw_sprite(page2,selec3,0,0);
        }
        else if (20<=mouse_x && mouse_x<=310 && 646<=mouse_y && mouse_y<=696)
        {
            selec=4;
            draw_sprite(page2,selec4,0,0);
        }
        else
        {
            selec=0;
            draw_sprite(page2,selec0,0,0);
        }

        draw_sprite(screen,page2,0,0);
        rest(20);
        clear_bitmap(page2);

        /// ici en fonction de la selection, on choisis quel graphe faire.
        if (mouse_b&1)
        {

            switch (selec)
            {
            case 1:
                fichier="Saves1.txt";
                back_pic(fichier);
                set_num_graph(1);
                break;

            case 2:
                fichier="Saves2.txt";
                back_pic(fichier);
                set_num_graph(2);
                break;

            case 3:
                fichier="Saves3.txt";
                back_pic(fichier);
                set_num_graph(3);
                break;
            case 4:
                exit(0);
                break;
            default:
                break;
            }
        }
    }
}


void Graph::save_pic(const std::string& nom_du_fichier)
{
    std::ofstream ofs(nom_du_fichier.c_str(), std::ios::out);
    /// si le fichier est ouvert
    int i;
    i=m_vertices.size();

    if(ofs)
    {
        /// On sauvegarde les Vertex
        ofs<< i << std::endl;
        /// on parcourt la map
        for (std::map<int, Vertex>::iterator it= m_vertices.begin(); it!= m_vertices.end(); it++)
        {
            it->first; ///key
            it->second; ///pointe sur le vertex

            ofs << it->first << " " /// on entre d'abord l'indice
                << it->second.m_value<<" "
                << it->second.m_interface->m_top_box.get_posx() << " " ///on entre la position en x
                << it->second.m_interface->m_top_box.get_posy() << " "  /// on entre la position en y
                << it->second.m_interface->m_img.get_pic_name()<< std::endl; /// on entre le nom de l'image
        }
        ///On sauvegarde les Edges
        ofs << m_edges.size()<<std::endl;
        for (std::map<int, Edge>::iterator it= m_edges.begin(); it!= m_edges.end(); it++)
        {
            it->first; ///key
            it->second; ///pointe sur le edge

            ofs << it->first << " " /// on entre d'abord l'indice
                << it->second.m_weight << " " ///on entre la poids
                << it->second.m_from<< " "  /// on entre le premier Vertex
                << it->second.m_to<< std::endl; /// on entre le deuxieme Vertex
        }

        /// On inscrit dans le fichier

        std::cout << "Ecriture reussie" << std::endl;
    }
    else
    {
        std::cout << "Cannot write " << nom_du_fichier << std::endl;
    }
};

void Graph::back_pic(const std::string& nom_du_fichier)
{
    ///déclaration des variables
    int a,b,d,y,i;
    double c;
    std::string nom;

    std::ifstream ifs(nom_du_fichier.c_str());
    /// si le fichier est ouvert
    if(ifs)
    {
        ///On adapte la fenetre
        m_interface = std::make_shared<GraphInterface>(50,0,750,600);
        ///On recupere d'abord les vertexs
        ifs >> y;
        /// on parcourt le fichier
        for (int i=0; i<y; i++)
        {
            ifs >> a >> c >> b >> d >> nom; /// indice >> valeur >> position_x >> position_y >> nom du fichier
            std:: cout << a << " " << b << " "<< c << " " << d << " " << nom << std::endl;
            ///On appelle le programme de création de vertexe
            add_interfaced_vertex(a,c,b,d,nom);
        }
        ///On recupere ensuite les Edges
        ifs >> i;
        for (int j=0; j<i ; j++)
        {
            ifs >> a >> c >> b >> d ; /// indice >> poids >> from >> to
            /// on appelle le programme de creation de edges
            Graph::add_interfaced_edge (a,b,d,c);

        }
        std::cout << "Lecture reussie" << std::endl;
    }
    else
    {
        std::cout << "Cannot read " << nom_du_fichier << std::endl;
    }
}

void Graph::ajout_pic()
{
    ///Ajout via console
    ///Declaration des variables
    std::string nom;
    int poids;
    ///On demande la saisie du nom de l'image
    std::cout<<"Vous voulez creer un nouvel element " << std::endl;
    std::cout<<"Veuillez entrer le nom de l'image qui lui est associee: " << std::endl;
    std::cin>>nom;
    /// On demande la saisie du poids
    std::cout<< "poids :";
    std::cin>>poids;
    /// On crée le vertexe
    add_interfaced_vertex(0,0,400,400,nom);

}

void Graph::supprimer_pic()
{
    ///Suppression via console
    ///Déclaration des variables
    /// Un vecteur de stockage des idx_edge des edges afin de pallier au problème de out_of_range
    std::vector<int> temp;
    /// Le nom du fichier à supprimer
    std::string nom;
    /// L'indice du fichier
    int vidx;
    /// Demande de saisie du nom
    std::cout<<"Suppression"<<std::endl;
    std::cin>>nom;

    ///Boucle de recherche dans la map de vertexe de l'idx du vertexe saisi
    for (std::map<int, Vertex>::iterator it1= m_vertices.begin(); it1!= m_vertices.end(); it1++)
    {
        if(it1->second.m_interface->m_img.get_pic_name()==nom)
        {
            vidx=it1->first;
        }

    }
    ///Boucle de recherche d'un Edge comprenant notre vertexe
    for(std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
    {
        if (it->second.m_from==vidx||it->second.m_to==vidx)
        {
            temp.push_back(it->first);
        }
    }
    ///Pour chaque iodx_edge du vecteur, on supprime l'Edge qui lui est associé
    for(auto elem : temp)
    {
        test_remove_edge(elem);
    }
    ///Lorsque le vertexe n'a plus d'Edge à proximité on le supprime
    for (std::map<int, Vertex>::iterator it= m_vertices.begin(); it!= m_vertices.end(); it++)
    {
        it->first; ///key
        it->second; ///pointe sur le vertex
        if(it->second.m_interface->m_img.get_pic_name()==nom)
        {
            test_remove_vertex(it->first);
            break;

        }

    }
}

void Graph::supprimer_arete()
{
    ///Suppression via console
    ///Déclaration des variables
    /// Un vecteur de stockage des idx_edge des edges afin de pallier au problème de out_of_range
    std::vector<int> temp;
    /// Le nom du predecesseur
    std::string nompred;
    /// Le nom du successeur
    std::string nomsuc;
    /// Les indices du pred et du suc
    int vidx1;
    int vidx2;
    /// Demande de saisie du predecesseur et du successeur
    std::cout<<"Suppression"<<std::endl;
    std::cout<<"Entrer nom predecesseur"<<std::endl;
    std::cin>>nompred;
    std::cout<<"Entrer nom successeur"<<std::endl;
    std::cin>>nomsuc;

    ///Boucle de recherche des idx_vertex des vertex associés au pred et au suc
    for (std::map<int, Vertex>::iterator it1= m_vertices.begin(); it1!= m_vertices.end(); it1++)
    {
        if(it1->second.m_interface->m_img.get_pic_name()==nompred)
        {
            vidx1=it1->first;

        }
        if(it1->second.m_interface->m_img.get_pic_name()==nomsuc)
        {
            vidx2=it1->first;

        }
    }

    ///Boucle de recherche des Edges qui correspondent
    for (std::map<int, Edge>::iterator it= m_edges.begin(); it!= m_edges.end(); it++)
    {
        it->first; ///key
        it->second; ///pointe sur le vertex
        if(it->second.m_from==vidx1 && it->second.m_to==vidx2)
        {
            ///Supression du Edge
            test_remove_edge(it->first);
            break;

        }

    }
}

void Graph::test_remove_vertex(int vidx)
{
/// référence vers le Vertex à enlever
    Vertex &remve=m_vertices.at(vidx);
    /// test : on a bien des éléments interfacés
    if (m_interface && remve.m_interface)
    {
        ///enlever le conteneur d'interface m_top_box du vertex de la main_box du graphe
        m_interface->m_main_box.remove_child( remve.m_interface->m_top_box );
    }
    ///On supprime le vertex de la map
    m_vertices.erase( vidx );
}

void Graph::test_remove_edge(int eidx)
{
/// référence vers le Edge à enlever
    Edge &remed=m_edges.at(eidx);

/// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
/// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

/// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );
}

///Generer une matrice à partir des vecteurs issus des fichiers de sauvegarde
void Graph::generate_matrice()
{
    ///On idique la taille de la matrice carée
    m_ordre=m_vertices.size();
    ///On alloue la place
    allouer_mat(m_ordre);
    ///Boucle de recherche des dépendances
    for(int i=0; i< m_ordre; i++)
    {
        for(int j=0; j<m_ordre; j++)
        {
            ///Si deux éléments sont connectés ...
            for (std::map <int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); it++)
            {
                if (it->second.m_from==i&&it->second.m_to==j)
                {
                    ///...On set la valeur de la matrice à 1
                    m_matrice1[i][j]=1;
                }
            }
        }
    }
    ///Boucle d'affichage de la matrice
    for(int i=0; i< m_ordre; i++)
    {
        for(int j=0; j<m_ordre; j++)
        {
            std::cout<<m_matrice1[i][j]<< " " ;
        }
        std::cout<<std::endl;
    }
}

///Allocation de la matrice
void Graph::allouer_mat(int ordre)
{
    m_matrice1=new int*[ordre];           ///Création matrice en fonction de l'ordre.

    for(int i=0; i<ordre; i++)
    {
        m_matrice1[i]=new int[ordre];
    }
    for (int j=0; j<ordre; j++)           ///Initialisation de la matrice.
    {
        for (int l=0 ; l<ordre; l++)
        {
            m_matrice1[j][l]=0;
        }
    }
}
void Graph::lireFichier(std::string nomFichier)
{
    std::string nom;
    int indice=0;
    int poids,x,y;

    std::ifstream fichier (nomFichier.c_str(), std::ios:: in); /// ouverture du fichier
    if(fichier)
    {
        m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
        fichier>>m_ordre>>m_arete;
        allouer_mat(m_ordre);
        for(int l=0; l<m_ordre; l++)        ///Remplissage de la matrice
        {
            fichier>> indice>>poids>>x>>y>>nom;
            add_interfaced_vertex(indice,poids,x,y,nom);
        }
        int k=0;

        for(int i=0; i< m_ordre; i++)
        {
            for(int j=0; j<m_ordre; j++)
            {
                fichier>>m_matrice1[i][j];
                if(m_matrice1[i][j]!=0)
                {
                    add_interfaced_edge(k,i,j,m_matrice1[i][j]);
                    k++;
                }
            }
        }

        fichier.close();
    }

    else
        std::cerr <<"Impossible d'ouvrir le fichier"<< std::endl;

}

void Graph::sauvegarde()
{
    int nb;
    nb = get_num_graph();
    switch (nb)
    {
    case 1:
        save_pic("Saves1.txt");
        break;
    case 2:
        save_pic("Saves2.txt");
        break;
    case 3:
        save_pic("Saves3.txt");
        break;
    }
}

int Graph::get_num_graph()
{
    return m_num_graph;
}
void Graph::set_num_graph(int num_graph)
{
    m_num_graph=num_graph;
}
int** Graph::toutesComposantesConnexes(int**matrice, int ordre)
{

    int **tabc;
    int *marque;
    int x,y;

    tabc=new int*[ordre];
    for(int i=0; i<ordre; i++)
    {
        tabc[i]=new int[ordre];
    }
    marque=new int[ordre];

    for(int i=0; i<ordre; i++)
    {
        for(int j=0; j<ordre; j++)
        {
            tabc[i][j]=0;
        }
        marque[i]=0;
    }

    for(x=0; x<ordre; x++)
    {
        if(!marque[x])
        {
            tabc[x]=uneComposanteFortementConnexe(matrice,ordre,x);
            marque[x]=1;
            for(y=0; y<ordre; y++)
            {
                if(tabc[x][y] && !marque[y])
                {
                    marque[y]=1;
                }
            }

        }
    }
    return tabc;
}
int* Graph::uneComposanteFortementConnexe (int** matrice, int ordre, int s)
{
///Variables locales
    int *c1, *c2 ; /// composantes connexes directes partant de s et indirectes arrivant vers s
    int *c ; /// composante fortement connexe = c1  c2 à retourner
    int *marques ; /// tableau dynamique indiquant si les sommets sont marqués ou non
    int x, y ; /// numéros de sommets intermédiaires des composantes connexes
    int ajoute = 1 ; /// booléen indiquant si une nouvelle composante connexe est ajouté e
/// Allouer les tableaux dynamiques c1, c2, c et marques de taille « ordre »
    c1=new int[ordre];
    c2=new int[ordre];
    c=new int[ordre];
    marques=new int[ordre];

/// Initialiser les valeurs de ces tableaux à 0
    for(int i=0; i<ordre; i++)
    {
        c1[i]=0;
        c2[i]=0;
        c[i]=0;
        marques[i]=0;
    }
/// Rendre le sommet s connexe
    c1[s] = 1 ;
    c2[s] = 1 ;
/// Recherche des composantes connexes partant de s à ajouter dans c1 :
    while (ajoute==1)
    {
        ajoute = 0;
/// à chaque tour, recherche d’une nouvelle composante connexe à ajouter
/// Pour tous les sommets x non marqués et connectés en partant de s
/// Marquer chaque sommet x et connecter les sommets non marqués y adjacents à x
        for (x=0 ; x<ordre ; x++)
        {
            if (!marques[x] && c1[x])
            {
                marques[x] = 1 ;
                for (y=0 ; y<ordre ; y++)
                {
                    if (matrice[x][y] && !marques[y])
                    {
                        c1[y] = 1 ;
                        ajoute = 1 ; /// nouvelle composante connexe ajoutée
                    }
                }
            }
        }
        ajoute=1;
        for(int i=0; i<ordre; i++)
            marques[i]=0;
/// Recherche des composantes connexes arrivant à s à ajouter dans c2 :
        for (x=0 ; x<ordre ; x++)
        {
            if (!marques[x] && c2[x])
            {
                marques[x] = 1 ;
                for (y=0 ; y<ordre ; y++)
                {
                    if (matrice[y][x] && !marques[y])
                    {
                        c2[y] = 1 ;
                        ajoute = 1 ; /// nouvelle composante connexe ajoutée
                    }
                }
            }
        }
/// Composante fortement connexe c = intersection de c1 et c2
        for (x=0 ; x<ordre ; x++)
            c[x] = c1[x] & c2[x] ;
/// Retourner la composante fortement connexe c
        return c ;
    }
    return 0;
}

void Graph::afficher_connex()
{
    int** valeur;
    int ctr=0;
    std::vector<int> v;
    std::string a;
    generate_matrice();
    std::cout<<"matrice de forte connexite"<<std::endl;
    valeur=toutesComposantesConnexes(m_matrice1,m_ordre);
    for (int i=0; i<m_ordre; i++)
    {

        ctr=0;
        for (int j=0; j<m_ordre; j++)
        {
            ///std::cout<<valeur[i][j]<<" ";
            if (valeur[i][j]==1)
            {
                ctr++;
                v.push_back(j);
            }
        }
        back_FC(v);

        if (ctr>0)
        {
            Graph_Reduit(v);
            std::cout<<"composante fortement connexe : ";
            for (int k=0; k<ctr; k++)
            {
                std::cout<<v.back()<<" ";
                v.pop_back();

            }
            std::cout<<std::endl;
        }
    }
    /// On affiche le graphe réduit


}
void Graph::calcul_K(int idx)
{
    ///Initialisation / Réinitialisation de la valeur m_K. Notons qu'on préfère 0.001 plutot que 0 car la division par 0 est impossible.
    m_vertices[idx].m_K=0.001;
    for (auto elem : m_vertices[idx].m_out)
    {
        m_vertices[idx].m_K+=m_edges[elem].m_weight*m_vertices[m_edges[elem].m_to].m_value;
    }
}
///Calcul de la fluctuation des populations entre les temps n et n+1
void Graph::calcul_Npop(int idx)
{
    ///Composante de la reproduction
    m_vertices[idx].m_value= m_vertices[idx].m_value + 0.001*m_vertices[idx].m_value*(1-m_vertices[idx].m_value/m_vertices[idx].m_K);
    ///Composante de la prédation
    for (auto elem : m_vertices[idx].m_in)
    {
        m_vertices[idx].m_value-=m_edges[elem].m_weight*m_vertices[m_edges[elem].m_from].m_value/1000;
    }
    ///Conditions de réalisme et de compréhensibilité des flux
    if(m_vertices[idx].m_value<0)
    {
        m_vertices[idx].m_value=0;
    }
    if(m_vertices[idx].m_value>100)
    {
        m_vertices[idx].m_value=100;
    }

}
void Graph::Temporalite()
{
    ///Boucle de flux pour n tours de boucle
    while (!key[KEY_C])
    {
        ///On calcule la valeur de chaque Vertex de la map m_vertices
        for (std::map<int, Vertex>::iterator it= m_vertices.begin(); it!= m_vertices.end(); it++)
        {
            ///On considère que la circulation est constante et que l'herbe est une ressource inépuisable
            if (it->second.m_interface->m_img.get_pic_name()=="voiture.bmp"||it->second.m_interface->m_img.get_pic_name()=="herbe.bmp"||
                    it->second.m_interface->m_img.get_pic_name()=="voiture.jpg"||it->second.m_interface->m_img.get_pic_name()=="herbe.jpg")
            {
                it->second.m_value=100;
            }
            else
            {
                /// Pour tout autre vertex on calcule d'abord le coeficient de reproduction
                calcul_K(it->first);
                /// Puis le nombre d'individus
                calcul_Npop(it->first);
                /// On affiche dans la console
                std::cout<<it->second.m_interface->m_img.get_pic_name() << " " <<it->second.m_value<< " " << it->second.m_K<< std::endl;
            }
            /// On update le graph
            update1();
            /// On update l'interface
            grman::mettre_a_jour();
        }

    }


}
///Pour afficher le graphe réduit on décide de supprimer toutes les parties non fortement connexes
///Idx est l'indice du vertexe qui ne fait pas partie de la matrice de connexité
void Graph::supprimer_pic_idx(int idx)
{
    ///Création d'un vecteur temp qui contient tous les indices des edges voisins au vertex
    std::vector<int> temp;
    if (m_vertices[idx].m_in.size()!=0)
    {
        for (auto elem : m_vertices[idx].m_in)
        {
            ///on remplit avec les edges arrivants
            temp.push_back(elem);
        }
    }
    if (m_vertices[idx].m_out.size()!=0)
    {
        for (auto elem : m_vertices[idx].m_out)
        {
            ///on remplit avec les edges partants
            temp.push_back(elem);
        }
    }
    ///S'il y a bien des edges voisins
    if(temp.size()!=0)
    {
        for(auto elem : temp)
        {
            ///On remplit le vecteur
            test_remove_edge(elem);
        }
    }
    /// On remove alors le vertex de la map
    for (std::map<int, Vertex>::iterator it= m_vertices.begin(); it!= m_vertices.end(); it++)
    {
        if(it->first==idx)
        {
            test_remove_vertex(it->first);
            break;

        }

    }

}
void Graph::supprimer_arete_idx(int idx)
{
    ///Déclaration des variables
    /// Un vecteur de stockage des idx_edge des edges afin de pallier au problème de out_of_range
    std::vector<int> temp;

    ///Boucle de recherche des Edges qui correspondent
    for (std::map<int, Edge>::iterator it= m_edges.begin(); it!= m_edges.end(); it++)
    {
        if(it->first==idx)
        {
            ///Supression du Edge
            test_remove_edge(it->first);
            break;
        }
    }
}
void Graph::back_FC(std::vector <int> vec)
{
    /// On parcout le vecteur
    for (auto elem: vec)
    {
        for(auto elem2: vec)
        {
            ///On cherche les aretes constituées par nos elements
            for(std::map<int,Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
            {
                if (it->second.m_from==elem&&it->second.m_to==elem2)
                {
                    ///On les colorie en violet
                    it->second.m_interface->m_top_edge.set_color_edge(VIOLETCLAIR);
                }
            }
        }
    }
    ///On supprime toutes les aretes de couleur autre que violet
    for (std::map<int,Edge>::iterator it= m_edges.begin(); it!=m_edges.end(); it++)
    {
        if(it->second.m_interface->m_top_edge.get_color_edge()!=VIOLETCLAIR)
        {
            supprimer_arete_idx(it->first);
        }
    }
};
void Graph::Graph_Reduit(std::vector <int> vec)
{
    std::string a;
    for(auto elem: vec)
    {
        for(std::map<int,Vertex>::iterator it=m_vertices.begin(); it!=m_vertices.end(); it++)
        {
            if (it->first==elem)
            {
                std::cout << it->first<< std::endl;
                a+=it->second.m_interface->m_img.get_pic_name();
            }
        }
    }
    std::cout<< a <<std::endl;
    create_box(a);
}
void Graph::create_box(std::string a)
{
    std::cout<<"m_boxes size" <<m_boxes.size()<< std::endl;
    std::cout<<m_boxes.size()<< std::endl;
    grman::WidgetBox *Nom= new grman::WidgetBox();
    grman::WidgetText *Text= new grman::WidgetText();



    m_interface->m_top_box.add_child(*Nom);
    Nom->set_dim(100,60);
    Nom->set_posx(rand()%1000);
    Nom->set_posy(rand()%700);
    Nom->set_moveable(true);
    Nom->add_child(*Text);
    Text->set_dim(90,50);
    Text->set_message(a);

    m_boxes.push_back(Nom);

    for (unsigned int i=0; i<m_boxes.size()-1; i++)
    {
        grman::WidgetEdge *Arrow=new grman::WidgetEdge();
        m_interface->m_main_box.add_child(*Arrow);
        Arrow->attach_from(*m_boxes[i]);
        Arrow->attach_to(*m_boxes[i+1]);
        Arrow->reset_arrow_with_bullet();
        m_arrow.push_back(Arrow);

    }


}
void Graph::vider_box()
{
    m_boxes.clear();
    m_arrow.clear();
}
void Graph::init_k_connex()
{
    int indice;
    bool noNeed=false;
    int nbAdj=0;
    std::vector<std::vector <int>> allCombi;
    std::vector<int> inter;
    for(auto& el:m_vertices)
    {

        nbAdj=el.second.m_out.size()+el.second.m_in.size();
        if(nbAdj==0)
        {
            noNeed=true;
        }
    }
    if(noNeed)
    {
        std::cout<<"le graphe est deja non connexe"<<std::endl;
    }
    else if(!noNeed)
    {
        for(auto &el:m_vertices)
        {
            indice=el.first;
            el.second.m_existe=true;
        }
        indice++;
        m_toursDeBoucle=0;
        m_toursDeBoucleMax=9999;
        k_connex(inter,allCombi);
        afficher_k_connex(inter,allCombi);
    }
}

void Graph::k_connex(std::vector<int>& inter, std::vector<std::vector <int>>& allCombi)
{
    bool good;
    for(auto & el:m_vertices)
    {
        if(el.second.m_existe)
        {
            el.second.m_existe=false;
            if(graphConnex(el.first))
            {
                inter.push_back(el.first);
                m_toursDeBoucle++;
                if(m_toursDeBoucle<m_toursDeBoucleMax)
                {
                    k_connex(inter,allCombi);
                }
                inter.pop_back();
            }
            else if(!graphConnex(el.first))
            {
                m_toursDeBoucle++;
                good=true;
                for(unsigned int i=0; i<allCombi.size(); i++)
                {
                    if(allCombi[i]==inter)
                    {
                        good=false;
                    }
                }
                if(good)
                    allCombi.push_back(inter);
                if(m_toursDeBoucle<m_toursDeBoucleMax)
                {
                    m_toursDeBoucleMax=m_toursDeBoucle;
                }
            }
            m_toursDeBoucle--;
            el.second.m_existe=true;
        }
    }
}

bool Graph::graphConnex(int idx)
{
    int inter;
    int indice1=0;
    int indice2=0;
    std::queue<int> file;
    for(auto & el:m_vertices)
    {
        el.second.m_marqueur=false;
    }
    m_vertices[idx].m_marqueur=true;
    file.push(idx);
    while(file.size()!=0)
    {
        inter=file.front();
        file.pop();
        for(unsigned int i=0; i<m_vertices[inter].m_in.size(); i++)
        {
            if(m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_existe&&!m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_marqueur)
            {
                file.push(m_edges[m_vertices[inter].m_in[i]].m_from);
                m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_marqueur=true;
            }
        }
        for(unsigned int i=0; i<m_vertices[inter].m_out.size(); i++)
        {
            if(m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_existe&&!m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_marqueur)
            {
                file.push(m_edges[m_vertices[inter].m_out[i]].m_to);
                m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_marqueur=true;
            }
        }
    }
    for(auto& el:m_vertices)
    {
        if(el.second.m_existe)
        {
            indice1++;
        }
        if(el.second.m_marqueur)
        {
            indice2++;
        }
    }
    if(indice1>indice2)
    {
        return false;
    }
    else if(indice1==indice2)
    {
        return true;
    }
    return 0;
}

void Graph::afficher_k_connex(std::vector<int>& inter,std::vector<std::vector <int>>& allCombi)
{
    int poids_min=888888;
    std::vector<std::vector <int>> allCombMin;
    std::vector<std::vector <int>> allCombiMin2;
    std::vector<std::vector <int>> alreadyUse;
    int indice=0;
    bool good;
    for(unsigned int i=0; i<allCombi.size(); i++)
    {
        if(allCombi[i].size()<(unsigned int)poids_min)
        {
            poids_min=allCombi[i].size();
        }
    }
    for(unsigned int i=0; i<allCombi.size(); i++)
    {
        if(allCombi[i].size()==(unsigned int)poids_min)
        {
            allCombMin.push_back(allCombi[i]);
        }
    }
    for(unsigned int i=0; i<allCombMin.size(); i++)
    {
        inter=allCombMin[i];

        indice=0;
        good=true;
        for(unsigned int j=0; j<alreadyUse.size(); j++)
        {
            for(unsigned int k=0; k<alreadyUse[i].size(); i++)
            {
                for(unsigned int l=0; l<inter.size(); l++)
                {
                    if(inter[l]==alreadyUse[j][k])
                    {
                        indice++;
                    }
                    if(indice==poids_min)
                    {
                        good=false;
                    }
                }
            }
        }
        if(good)
        {
            allCombiMin2.push_back(inter);
            alreadyUse.push_back(inter);
        }
    }
    /// std::cout<<poids_min<<std::endl;
    std::cout<<"Le graphe n'es plus connexe si on enleve le(s) au moins le(s) sommet(s) :"<<std::endl;

    for(unsigned int i=0; i<allCombiMin2.size(); i++)
    {
        if(allCombiMin2.size()!=0)
        {
            std::cout<<"-> ";
            for(unsigned int j=0; j<allCombiMin2[i].size(); j++)
            {
                std::cout<<allCombiMin2[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
    }
}
///ss prog qui initialise les bouton
void Graph::initButton()
{
    for(auto & elt: m_vertices)
    {
        elt.second.m_interface->m_button_addEdge.set_value(false);
    }
}

void Edge::setFrom(int from)
{
    m_from=from;
}

void Edge::setTo(int to)
{
    m_to=to;
}
