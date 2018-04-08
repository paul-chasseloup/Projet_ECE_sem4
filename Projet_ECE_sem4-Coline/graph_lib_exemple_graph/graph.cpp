#include "graph.h"
#include <allegro.h>
#include <time.h>
#include <fstream>


/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
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

    m_top_box.add_child(m_supprimer);
    m_supprimer.add_child(m_text_supprimer);  ///boutton
    m_supprimer.set_dim(80,40);
    m_supprimer.set_posx(0);
    m_supprimer.set_posy(0);
    m_supprimer.set_bg_color(ROUGE);
    m_text_supprimer.set_message("SUPPRIMER");

    m_top_box.add_child(m_ajouter);
    m_ajouter.add_child(m_text_ajouter);
    m_ajouter.set_dim(80,40);
    m_ajouter.set_posx(0);
    m_ajouter.set_posy(40);
    m_ajouter.set_bg_color(FUCHSIA);
    m_text_ajouter.set_message("AJOUTER");

    m_top_box.add_child(m_sauvegarde);
    m_sauvegarde.add_child(m_text_sauvegarde);
    m_sauvegarde.set_dim(80,40);
    m_sauvegarde.set_posx(0);
    m_sauvegarde.set_posy(80);
    m_sauvegarde.set_bg_color(BLEUCLAIR);
    m_text_sauvegarde.set_message("SAVE");

    m_top_box.add_child(m_quitter);
    m_quitter.add_child(m_text_quitter);
    m_quitter.set_dim(80,40);
    m_quitter.set_posx(0);
    m_quitter.set_posy(120);
    m_quitter.set_bg_color(VIOLETCLAIR);
    m_text_quitter.set_message("QUITTER");
}

int GraphInterface::update()
{
    if (m_quitter.clicked())
    {
       return 1;
    }
    if(m_ajouter.clicked())
    {
        return 2;
    }

}
/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
/*void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}
*/
/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();
    int a=m_interface->update();
    if(a==1)
    {
        exit(1);
    }
    if(a==2)
    {
        ajout_pic();
    }
    m_interface->update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();


}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from =id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}



Graph Graph::menu(Graph g)
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
                g.back_pic("Saves1.txt");
                g.set_num_graph(1);
                break;

            case 2:
                g.back_pic("Saves2.txt");
                g.set_num_graph(2);
                break;

            case 3:
                g.back_pic("Saves3.txt");
                g.set_num_graph(3);
                break;

            default:
                break;
            }
        }
    }

return g;
}
/*void Graph::Creation(const std::string& nom_du_fichier)
{
    std::ofstream ofs(nom_du_fichier.c_str(), std::ios::out);
	// D�claration des variables
	// pour la boucle de remplissage
    int i=0;
	// pour les donn�es saisies
    std::string A1,A2;
    int pos_x, pos_y;

    if(ofs)
    {
        // On montre que le fichier est bien ouvert
        std::cout << "Writing " << nom_du_fichier << " => OK" << std::endl;
	// On remplit autant qu'on veut
        while (i!=1)
        {
		//Boucle int�ractive
            std::cout << "Premier element : " << std::endl;
		//On saisit le premier element
            std::cin >>A1;
		//On saisit le deuxieme element
            std::cout << "Deuxieme element : " << std::endl;
            std::cin >>A2;
            //Boucle int�ractive
            std::cout << "Pos_x : " << std::endl;
		//On saisit le premier element
            std::cin >>pos_x;
		//On saisit le deuxieme element
            std::cout << "Pos_y : " << std::endl;
            std::cin >>pos_y;
		// On inscrit dans le fichier
             ofs << A1 << A2 << pos_x << poaves1.txts_y << std::endl;
             std::cout << "Appuyez sur 1 pour arreter, sur une autre touche sinon : " << std::endl;
             std::cin >>i;
        }

	std::cout << "Ecriture reussie" << std::endl;
    }
    else
    {
        std::cout << "Cannot write " << nom_du_fichier << std::endl;
    }
};*/
void Graph::save_pic(const std::string& nom_du_fichier)
{
    std::ofstream ofs(nom_du_fichier.c_str(), std::ios::out);
    // si le fichier est ouvert
    int i;
    i=m_vertices.size();

    if(ofs)
    {
        /// On sauvegarde les Vertex
        ofs<< i << std::endl;
        // on parcourt la map
        for (std::map<int, Vertex>::iterator it= m_vertices.begin(); it!= m_vertices.end();it++)
        {
            it->first; //key
            it->second; //pointe sur le vertex

            ofs << it->first << " " // on entre d'abord l'indice
            << it->second.m_value<<" "
            << it->second.m_interface->m_top_box.get_posx() << " " //on entre la position en x
            << it->second.m_interface->m_top_box.get_posy() << " "  // on entre la position en y
            << it->second.m_interface->m_img.get_pic_name()<< std::endl; // on entre le nom de l'image
        }
        ///On sauvegarde les Edges
        ofs << m_edges.size()<<std::endl;
        for (std::map<int, Edge>::iterator it= m_edges.begin(); it!= m_edges.end();it++)
        {
            it->first; //key
            it->second; //pointe sur le edge

            ofs << it->first << " " // on entre d'abord l'indice
            << it->second.m_weight << " " //on entre la poids
            << it->second.m_from<< " "  // on entre le premier Vertex
            << it->second.m_to<< std::endl; // on entre le deuxieme Vertex
        }

		// On inscrit dans le fichier

	std::cout << "Ecriture reussie" << std::endl;
    }
    else
    {
        std::cout << "Cannot write " << nom_du_fichier << std::endl;
    }
};

void Graph::back_pic(const std::string& nom_du_fichier)
{
    //d�claration des variables
    int a,b,d,y,i;
    double c;
    std::string nom;

    std::ifstream ifs(nom_du_fichier.c_str());
    // si le fichier est ouvert
    if(ifs)
    {
        ///On affiche d'abord les vertexs
        m_interface = std::make_shared<GraphInterface>(50,0,750,600);
        ifs >> y;
        // on parcourt la map
        for (int i=0; i<y;i++)
        {

            ifs >> a >> b >> c >> d >> nom;
            std:: cout << a << " " << b << " "<< c << " " << d << " " << nom << std::endl;
            add_interfaced_vertex(a,b,c,d,nom);
        }
        ///On affiche ensuite les Edges
        ifs >> i;
        for (int j=0; j<i ;j++)
        {
            ifs >> a >> b >> c >> d ;
            Graph::add_interfaced_edge (a ,c,d,b);

        }

		// On inscrit dans le fichier

	std::cout << "Lecture reussie" << std::endl;
    }
    else
    {
        std::cout << "Cannot read " << nom_du_fichier << std::endl;
    }
}

void Graph::ajout_pic()
{
    std::string nom;
    int lien_1, poids;
    std::cout<<"Vous voulez creer un nouvel element " << std::endl;
    std::cout<<"Veuillez entrer le nom de l'image qui lui est associee: " << std::endl;
    std::cin>>nom;
    std::cout<<"Veuillez entrer ses liens : "<< std::endl;
    std::cout<<"lien 1 : ";
    std::cin>>lien_1;
    std::cout<< "poids :";
    std::cin>>poids;
    add_interfaced_vertex(m_vertices.size()+1,0,400,400,nom);
    add_interfaced_edge(m_edges.size()+1,m_edges.size()+1,lien_1,poids);
}

void Graph::allouer_mat(int ordre)
{
    m_matrice1=new int*[ordre];           ///Cr�ation matrice en fonction de l'ordre.

    for(int i=0;i<ordre;i++)
    {
        m_matrice1[i]=new int[ordre];
    }
    for (int j=0;j<ordre;j++)             ///Initialisation de la matrice.
    {
        for (int l=0 ;l<ordre; l++)
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

void Graph::sauvegarde(Graph g)
{
    int nb;
    nb = g.get_num_graph();
    switch (nb)
    {
    case 1:
        g.save_pic("Saves1.txt");
        break;
    case 2:
        g.save_pic("Saves2.txt");
        break;
    case 3:
        g.save_pic("Saves3.txt");
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


bool Vertex::get_marque()
{
    return m_marque;
}

void Vertex::set_marque(bool marque)
{
    m_marque=marque;
}



/// utilis�s pour la connexit�

void Graph::DFS(int i, bool marquage[]) ///Parcours en profondeur (fonctionne)
{
    marquage[i]=true;
    std::cout << i << " ";
    for(unsigned int j=0; j<m_vertices[i].m_out.size(); j++)
    {
        if(!marquage[m_vertices[i].m_out[j]])
            DFS(m_vertices[i].m_out[j], marquage);
    }
}

Graph Graph::Transposee(std::string nom_du_fichier) ///Fonctionne
{
    Graph g;
    g.lireFichier("Saves2.txt"); ///Exemple � adapter pour chaque graphe
    for(int i=0; i<g.m_ordre; i++) ///Pour chaque sommet du graphe, ses pr�decesseurs deviennent ses successeurs et vice versa
    {
        g.m_vertices[i].m_in.clear();
        g.m_vertices[i].m_out.clear();
        for(unsigned int j=0; j<m_vertices[i].m_in.size(); j++)
            g.m_vertices[i].m_out.push_back(m_vertices[i].m_in[j]);
        for(unsigned int j=0; j<m_vertices[i].m_out.size(); j++)
            g.m_vertices[i].m_in.push_back(m_vertices[i].m_out[j]);
    }
    for(int i=0; i<m_arete; i++) ///Inversion des ar�tes
    {
        g.m_edges[i].set_from(m_edges[i].m_to);
        g.m_edges[i].set_to(m_edges[i].m_from);
    }
    return g; ///On retourne le graphe inverse g
}

void Graph::remplissage_pile(int i, bool marquage[], std::stack<int> sommets) ///Petit probleme ici. Semblable � un DFS, mais on range le sommet dans la pile de traitement � la fin
{
    marquage[i]=true;
    for(unsigned int j=0; j<m_vertices[i].m_out.size(); j++)
    {
        if(!marquage[m_vertices[i].m_out[j]])
            remplissage_pile(m_vertices[i].m_out[j], marquage, sommets);
    }
    sommets.push(i);
}

void Graph::Kosaraju(std::string nom_du_fichier) ///Algorithme qui donne les composantes fortement connexes et de quels somemts sont-ils compos�s
{
    std::stack<int> pile_de_visite; ///Pile pour traiter les diff�rents
    Graph trp=Graph(); ///On d�clare le graphe inverse du notre, en le construisant d'abord
    int top_de_reserve;
    bool* marques = new bool[m_ordre]; ///D�claration d'un tableau de marquage
    for(int i=0; i<m_ordre; i++) ///Initialisation du tableau de marquage
        marques[i]=false;
    for(int i=0; i<m_ordre; i++) ///Remplissage de la pile (petit malentendu pour le graphe des clowns o� cette pile est vide tout le long...)
    {
        if(!marques[i])
            remplissage_pile(i, marques, pile_de_visite);
    }
    trp=Transposee("Saves2.txt"); ///On inclut le graphe inverse
    for(int i=0; i<m_ordre; i++) ///R�initialisation du tableau de marquage
        marques[i]=false;
    while(!pile_de_visite.empty()) ///Tant que la pile n'est pas vide, faire un DFS du graphe inverse
    {
        top_de_reserve=pile_de_visite.top(); ///Garder le haut de pile en r�serve
        pile_de_visite.pop(); ///Faire d�filer la pile
        if(!marques[top_de_reserve]) ///Si ce fameux haut de pile n'a pas �t� visit�, on le marque comme visit�
        {
            trp.DFS(top_de_reserve, marques); ///Une ligne correspond � une composante fortement connexe
            std::cout << std::endl; ///Saut de ligne pour la prochaine composante
        }
    }
}

void Edge::set_from(int from)
{
    m_from=from;
}

void Edge::set_to(int to)
{
    m_to=to;
}