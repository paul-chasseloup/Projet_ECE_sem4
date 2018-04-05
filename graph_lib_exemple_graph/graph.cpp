#include "graph.h"
#include <allegro.h>
#include <time.h>
#include <fstream>


/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
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

/// Le constructeur met en place les éléments de l'interface
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

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

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
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
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

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
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

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
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
	// Déclaration des variables
	// pour la boucle de remplissage
    int i=0;
	// pour les données saisies
    std::string A1,A2;
    int pos_x, pos_y;

    if(ofs)
    {
        // On montre que le fichier est bien ouvert
        std::cout << "Writing " << nom_du_fichier << " => OK" << std::endl;
	// On remplit autant qu'on veut
        while (i!=1)
        {
		//Boucle intéractive
            std::cout << "Premier element : " << std::endl;
		//On saisit le premier element
            std::cin >>A1;
		//On saisit le deuxieme element
            std::cout << "Deuxieme element : " << std::endl;
            std::cin >>A2;
            //Boucle intéractive
            std::cout << "Pos_x : " << std::endl;
		//On saisit le premier element
            std::cin >>pos_x;
		//On saisit le deuxieme element
            std::cout << "Pos_y : " << std::endl;
            std::cin >>pos_y;
		// On inscrit dans le fichier
             ofs << A1 << A2 << pos_x << pos_y << std::endl;
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
    //déclaration des variables
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
void Graph::allouer_mat(int ordre)
{
    m_matrice1=new int*[ordre];           ///Création matrice en fonction de l'ordre.

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
