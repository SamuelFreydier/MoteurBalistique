#include "Octree.h"
#include <cassert>
#include <iostream>
#include "Rigidbody.h"

/**
 * @brief Constructeur avec la zone couverte par l'octree
 * @param inBoundary 
*/
Octree::Octree( const Area& inBoundary ) : m_boundary( inBoundary ), m_rootNode( std::make_unique<Node>() ) {}

/**
 * @brief Calcule la zone d'un noeud enfant à partir du parent et de l'index de la nouvelle parcelle
 * @param parentArea 
 * @param childIndex 
 * @return Zone fille de parentArea, choisie selon childIndex
*/
Area Octree::computeBox( const Area& parentArea, int childIndex ) const
{
    // Décalage pour obtenir le centre d'un enfant à partir du centre du parent
    Vector3 parentToChildOffset = parentArea.m_lengths / 4.0f;
    Vector3 childLengths = parentArea.m_lengths / 2.0f;

    switch( childIndex )
    {
        // De 0 à 3 = en haut
        // Nord Ouest
        case 0:
            return Area( Vector3( parentArea.m_position.x - parentToChildOffset.x,
                                  parentArea.m_position.y + parentToChildOffset.y,
                                  parentArea.m_position.z - parentToChildOffset.z ),
                         childLengths );
            // Nord Est
        case 1:
            return Area( Vector3( parentArea.m_position.x + parentToChildOffset.x,
                                  parentArea.m_position.y + parentToChildOffset.y,
                                  parentArea.m_position.z - parentToChildOffset.z ),
                         childLengths );
            // Sud Ouest
        case 2:
            return Area( Vector3( parentArea.m_position.x - parentToChildOffset.x,
                                  parentArea.m_position.y + parentToChildOffset.y,
                                  parentArea.m_position.z + parentToChildOffset.z ),
                         childLengths );
            // Sud Est
        case 3:
            return Area( Vector3( parentArea.m_position.x + parentToChildOffset.x,
                                  parentArea.m_position.y + parentToChildOffset.y,
                                  parentArea.m_position.z + parentToChildOffset.z ),
                         childLengths );

            // De 4 à 7 = en bas
            // Nord Ouest
        case 4:
            return Area( Vector3( parentArea.m_position.x - parentToChildOffset.x,
                                  parentArea.m_position.y - parentToChildOffset.y,
                                  parentArea.m_position.z - parentToChildOffset.z ),
                         childLengths );
            // Nord Est
        case 5:
            return Area( Vector3( parentArea.m_position.x + parentToChildOffset.x,
                                  parentArea.m_position.y - parentToChildOffset.y,
                                  parentArea.m_position.z - parentToChildOffset.z ),
                         childLengths );
            // Sud Ouest
        case 6:
            return Area( Vector3( parentArea.m_position.x - parentToChildOffset.x,
                                  parentArea.m_position.y - parentToChildOffset.y,
                                  parentArea.m_position.z + parentToChildOffset.z ),
                         childLengths );
            // Sud Est
        case 7:
            return Area( Vector3( parentArea.m_position.x + parentToChildOffset.x,
                                  parentArea.m_position.y - parentToChildOffset.y,
                                  parentArea.m_position.z + parentToChildOffset.z ),
                         childLengths );
        default:
            assert( false && "Invalid child index" );
            return Area();
    }
}


/**
 * @brief Retourne la subdivision dans laquelle un collider se trouve
 * @param nodeArea 
 * @param collider 
 * @return Indice de la subdivision. -1 pour le noeud parent en cas d'overlap du collider sur plusieurs subdivisions
*/
int Octree::getSubdivision( const Area& nodeArea, const Rigidbody& rb ) const
{
    const BoundingSphere& collider = rb.getSphereCollider();
    // En haut
    if( collider.m_position.y - collider.m_radius > nodeArea.m_position.y ) {
        // Haut Ouest
        if( collider.m_position.x + collider.m_radius < nodeArea.m_position.x ) {
            // Haut Nord Ouest
            if( collider.m_position.z + collider.m_radius < nodeArea.m_position.z ) {
                return 0;
            }
                
            // Haut Sud Ouest
            else if( collider.m_position.z - collider.m_radius >= nodeArea.m_position.z ) {
                return 2;

            }
            // Nul part
            else {
                return -1;
            }
        }
        // Haut Est
        else if( collider.m_position.x - collider.m_radius >= nodeArea.m_position.x ) {
            // Haut Nord Est
            if( collider.m_position.z + collider.m_radius < nodeArea.m_position.z ) {
                return 1;
            }
            // Haut Sud Est
            else if( collider.m_position.z - collider.m_radius >= nodeArea.m_position.z ) {
                return 3;
            }
            // Nul part
            else {
                return -1;
            }
        }
        else {
            return -1;
        }
    }
    // En bas
    else if( collider.m_position.y + collider.m_radius <= nodeArea.m_position.y ) {
        // Bas Ouest
        if( collider.m_position.x + collider.m_radius < nodeArea.m_position.x ) {
            // Bas Nord Ouest
            if( collider.m_position.z + collider.m_radius < nodeArea.m_position.z ) {
                return 4;
            }
            // Bas Sud Ouest
            else if( collider.m_position.z - collider.m_radius >= nodeArea.m_position.z ) {
                return 6;
            }
            // Nul part
            else {
                return -1;
            }
        }
        // Bas Est
        else if( collider.m_position.x - collider.m_radius >= nodeArea.m_position.x ) {
            // Bas Nord Est
            if( collider.m_position.z + collider.m_radius < nodeArea.m_position.z ) {
                return 5;
            }
            // Bas Sud Est
            else if( collider.m_position.z - collider.m_radius >= nodeArea.m_position.z ) {
                return 7;
            }
            // Nul part
            else {
                return -1;
            }
        }
        else {
            return -1;
        }
    }
    // Nul part
    else {
        return -1;
    }
}


/**
 * @brief Ajoute un collider au octree
 * @param collider 
*/
void Octree::add( std::shared_ptr<Rigidbody> collider ) { add( m_rootNode.get(), 0, m_boundary, collider ); }


/**
 * @brief Méthode auxiliaire récursive à la méthode publique add
 * @param node 
 * @param depth 
 * @param area 
 * @param collider 
*/
void Octree::add( Node* node, std::size_t depth, const Area& area, std::shared_ptr<Rigidbody> collider )
{
    assert( node != nullptr );

    // Si le collider est hors de la zone couverte par le quadtree, on ne le considere pas
    if( !area.intersects( collider->getSphereCollider() ) )
    {
        return;
    }

    // Si le noeud est une feuille
    if( isLeaf( node ) )
    {
        // On y insere le collider si possible
        if( depth >= MaxDepth || node->colliders.size() < Threshold )
        {
            node->colliders.push_back( collider );
        }
        // Sinon, on subdivise la parcelle en 8 subdivisions et on y met le collider au bon endroit
        else
        {
            split( node, area );
            add( node, depth, area, collider );
        }
    }
    else
    {
        int subdivIndex = getSubdivision( area, *collider );
        // Ajoute le collider dans un noeud enfant s'il est entierement contenu dedans
        if( subdivIndex != -1 )
        {
            add( node->children[ static_cast< std::size_t >( subdivIndex ) ].get(), depth + 1, computeBox( area, subdivIndex ),
                 collider );
        }
        // Sinon, on l'ajoute au noeud courant
        else
        {
            node->colliders.push_back( collider );
        }
    }
}


/**
 * @brief Séparation d'un noeud en 8 subdivisions (8 autres noeuds)
 * @param node 
 * @param area 
*/
void Octree::split( Node* node, const Area& area )
{
    assert( node != nullptr );
    assert( isLeaf( node ) && "Only leaves can be split" );

    // Creation des noeuds enfants
    for( auto& child : node->children )
    {
        child = std::make_unique<Node>();
    }

    // Assignation des colliders aux noeuds enfants
    std::vector<std::shared_ptr<Rigidbody>> newColliders;
    for( std::shared_ptr<Rigidbody> collider : node->colliders )
    {
        auto quadIndex = getSubdivision( area, *collider );
        if( quadIndex != -1 )
        {
            node->children[ static_cast< std::size_t >( quadIndex ) ]->colliders.push_back( collider );
        }
        else
        {
            newColliders.push_back( collider );
        }
    }
    node->colliders = std::move( newColliders );
}


/**
 * @brief Supprime un collider du octree
 * @param collider 
*/
void Octree::remove( std::shared_ptr<Rigidbody> collider ) { remove( m_rootNode.get(), m_boundary, collider ); }


/**
 * @brief Méthode auxiliaire récursive à la méthode publique remove
 * @param node 
 * @param area 
 * @param collider 
 * @return true = suppression réussie
*/
bool Octree::remove( Node* node, const Area& area, std::shared_ptr<Rigidbody> collider )
{
    assert( node != nullptr );
    assert( area.intersects( collider->getSphereCollider() ) );

    // Si le noeud est une feuille
    if( isLeaf( node ) )
    {
        // Supprime le collider du noeud
        removeValue( node, collider );
        return true;
    }
    else
    {
        // Supprime le collider d'un noeud enfant s'il est entierement contenu dans ledit noeud
        auto quadIndex = getSubdivision( area, *collider );
        if( quadIndex != -1 )
        {
            if( remove( node->children[ static_cast< std::size_t >( quadIndex ) ].get(), computeBox( area, quadIndex ), collider ) )
            {
                return tryMerge( node );
            }
        }
        // Sinon, le supprimer du noeud courant
        else
        {
            removeValue( node, collider );
        }
        return false;
    }
}


/**
 * @brief Suppression d'un collider depuis un noeud
 * @param node 
 * @param collider 
*/
void Octree::removeValue( Node* node, std::shared_ptr<Rigidbody> collider )
{
    // On trouve le collider dans le tableau de colliders du noeud
    auto it = std::find_if( std::begin( node->colliders ), std::end( node->colliders ),
                            [ this, &collider ] ( const auto& rhs ) { return collider == rhs; } );
    assert( it != std::end( node->colliders ) && "Trying to remove a value that is not present in the node" );

    // Swap le collider avec le dernier element puis suppression du dernier element
    *it = std::move( node->colliders.back() );
    node->colliders.pop_back();
}


/**
 * @brief Si la quantité de colliders cumulée des noeuds enfants est inférieure au seuil, on les fusionne et tous les colliders sont stockés dans le noeud mère
 * @param node 
 * @return true = fusion réussie
*/
bool Octree::tryMerge( Node* node )
{
    assert( node != nullptr );
    assert( !isLeaf( node ) && "Only interior nodes can be merged" );

    auto nbValues = node->colliders.size();

    // On compte les colliders dans les enfants
    for( const auto& child : node->children )
    {
        if( !isLeaf( child.get() ) )
        {
            return false;
        }
        nbValues += child->colliders.size();
    }

    // Quantite cumulee de colliders sous le seuil
    if( nbValues <= Threshold )
    {
        node->colliders.reserve( nbValues );
        // Fusion des tableaux de colliders de tous les noeuds enfants
        for( const auto& child : node->children )
        {
            for( const auto& value : child->colliders )
            {
                node->colliders.push_back( value );
            }
        }
        // Suppression des noeuds enfants
        for( auto& child : node->children )
        {
            child.reset();
        }
        return true;
    }
    else
        return false;
}


/**
 * @brief Supprime tous les colliders du octree
*/
void Octree::removeAll()
{
    // Suppression des noeuds enfants
    for( auto& child : m_rootNode->children )
    {
        child.reset();
    }

    // Nettoyage du tableau de colliders
    m_rootNode->colliders.clear();
}


/**
 * @brief Trouve tous les couples de colliders en intersection
 * @return Tableau de tous les couples de colliders en intersection
*/
std::vector<std::pair<std::shared_ptr<Rigidbody>, std::shared_ptr<Rigidbody>>> Octree::findAllIntersections() const
{
    std::vector<std::pair<std::shared_ptr<Rigidbody>, std::shared_ptr<Rigidbody>>> intersections;
    findAllIntersections( m_rootNode.get(), intersections );
    return intersections;
}


/**
 * @brief Méthode auxiliaire récursive à la méthode publique findAllIntersections
 * @param node 
 * @param intersections 
*/
void Octree::findAllIntersections(
    Node* node, std::vector<std::pair<std::shared_ptr<Rigidbody>, std::shared_ptr<Rigidbody>>>& intersections ) const
{
    // Trouve les intersections entre les colliders stockes dans le noeud courant
    // Une meme intersection ne doit pas etre reperee deux fois
    for( auto childIndex = std::size_t( 0 ); childIndex < node->colliders.size(); ++childIndex )
    {
        for( auto secondChildIndex = std::size_t( 0 ); secondChildIndex < childIndex; ++secondChildIndex )
        {
            if( node->colliders[ childIndex ]->getSphereCollider().collides(node->colliders[secondChildIndex]->getSphereCollider()) )
            {
                intersections.emplace_back( node->colliders[ childIndex ], node->colliders[ secondChildIndex ] );
            }
        }
    }

    // Si le noeud n'est pas une feuille
    if( !isLeaf( node ) )
    {
        // Les colliders du noeud courant peuvent etre en intersection avec des colliders des noeuds enfants
        for( const auto& child : node->children )
        {
            for( const auto& collider : node->colliders )
            {
                findIntersectionsInDescendants( child.get(), collider, intersections );
            }
        }
        // Appel recursif pour effectuer la meme detection d'intersection sur les noeuds enfants
        for( const auto& child : node->children )
        {
            findAllIntersections( child.get(), intersections );
        }
    }
}


/**
 * @brief Dessine les subdivisions graphiquement
*/
void Octree::draw() const
{
    draw( m_rootNode.get(), m_boundary );
}


/**
 * @brief Méthode auxiliaire récursive de dessin des zones de l'arbre
 * @param parentNode 
 * @param parentArea 
*/
void Octree::draw( Node* parentNode, const Area& parentArea ) const
{
    parentArea.draw();

    int childIndex;
    for( childIndex = 0; childIndex < 8; childIndex++ )
    {
        const std::unique_ptr<Node>& child = parentNode->children[ childIndex ];

        if( child )
        {
            draw( child.get(), computeBox( parentArea, childIndex ) );
        }
    }
}


/**
 * @brief Trouve les intersections entre les colliders du node courant et les colliders de ses noeuds enfants
 * @param node 
 * @param collider 
 * @param intersections 
*/
void Octree::findIntersectionsInDescendants(
    Node* node, std::shared_ptr<Rigidbody> collider,
    std::vector<std::pair<std::shared_ptr<Rigidbody>, std::shared_ptr<Rigidbody>>>& intersections ) const
{
    // Test d'intersection entre le collider courant et les colliders du noeud enfant
    for( const auto& other : node->colliders )
    {
        if( collider->getSphereCollider().collides(other->getSphereCollider()) )
        {
            intersections.emplace_back( collider, other );
        }
    }
    // Test d'intersection entre le collider courant et les colliders des noeuds enfants du noeud enfant
    if( !isLeaf( node ) )
    {
        for( const auto& child : node->children )
        {
            findIntersectionsInDescendants( child.get(), collider, intersections );
        }
    }
}

/**
 * @brief Constructeur d'une zone 3D (forme de parallélépipède)
 * @param position : centre de la zone
 * @param lengths : longueurs des côtés de la zone
*/
Area::Area( const Vector3& position, const Vector3& lengths )
    : m_position( position ), m_lengths( lengths )
{
}


/**
 * @brief Détecte si un collider de type sphère est en contact avec la zone courante
 * @param collider 
 * @return true = le collider se trouve au moins partiellement dans la zone
*/
bool Area::intersects( const BoundingSphere& collider ) const
{
    float distSquared = collider.m_radius * collider.m_radius;

    Vector3 areaCorner = Vector3( m_position.x - m_lengths.x / 2.0f,
                                  m_position.y - m_lengths.y / 2.0f,
                                  m_position.z - m_lengths.z / 2.0f );
    Vector3 areaOppositeCorner = areaCorner + m_lengths;

    if( collider.m_position.x < areaCorner.x ) distSquared -= pow( collider.m_position.x - areaCorner.x, 2 );
    else if( collider.m_position.x > areaOppositeCorner.x ) distSquared -= pow( collider.m_position.x - areaOppositeCorner.x, 2 );
    if( collider.m_position.y < areaCorner.y ) distSquared -= pow( collider.m_position.y - areaCorner.y, 2 );
    else if( collider.m_position.y > areaOppositeCorner.y ) distSquared -= pow( collider.m_position.y - areaOppositeCorner.y, 2 );
    if( collider.m_position.z < areaCorner.z ) distSquared -= pow( collider.m_position.z - areaCorner.z, 2 );
    else if( collider.m_position.z > areaOppositeCorner.z ) distSquared -= pow( collider.m_position.z - areaOppositeCorner.z, 2 );

    return distSquared > 0;
}


/**
 * @brief Dessine le wireframe d'une zone de l'octree
*/
void Area::draw() const
{
    ofBoxPrimitive graphicCube;
    graphicCube.setPosition( m_position.v3() );
    graphicCube.setWidth( m_lengths.x );
    graphicCube.setHeight( m_lengths.y );
    graphicCube.setDepth( m_lengths.z );

    ofSetColor( ofColor( 0, 0, 0 ) );
    graphicCube.drawWireframe();
}

