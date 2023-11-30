#include "Octree.h"
#include <cassert>

Octree::Octree( const Area& inBoundary ) : m_boundary( inBoundary ), m_rootNode( std::make_unique<Node>() ) {}

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
                                  parentArea.m_position.z + parentToChildOffset.z,
                                  parentArea.m_position.y + parentToChildOffset.y ),
                         childLengths );
            // Nord Est
        case 1:
            return Area( Vector3( parentArea.m_position.x + parentToChildOffset.x,
                                  parentArea.m_position.z + parentToChildOffset.z,
                                  parentArea.m_position.y + parentToChildOffset.y ),
                         childLengths );
            // Sud Ouest
        case 2:
            return Area( Vector3( parentArea.m_position.x - parentToChildOffset.x,
                                  parentArea.m_position.z - parentToChildOffset.z,
                                  parentArea.m_position.y + parentToChildOffset.y ),
                         childLengths );
            // Sud Est
        case 3:
            return Area( Vector3( parentArea.m_position.x + parentToChildOffset.x,
                                  parentArea.m_position.z - parentToChildOffset.z,
                                  parentArea.m_position.y + parentToChildOffset.y ),
                         childLengths );

            // De 4 à 7 = en bas
            // Nord Ouest
        case 4:
            return Area( Vector3( parentArea.m_position.x - parentToChildOffset.x,
                                  parentArea.m_position.z + parentToChildOffset.z,
                                  parentArea.m_position.y - parentToChildOffset.y ),
                         childLengths );
            // Nord Est
        case 5:
            return Area( Vector3( parentArea.m_position.x + parentToChildOffset.x,
                                  parentArea.m_position.z + parentToChildOffset.z,
                                  parentArea.m_position.y - parentToChildOffset.y ),
                         childLengths );
            // Sud Ouest
        case 6:
            return Area( Vector3( parentArea.m_position.x - parentToChildOffset.x,
                                  parentArea.m_position.z - parentToChildOffset.z,
                                  parentArea.m_position.y - parentToChildOffset.y ),
                         childLengths );
            // Sud Est
        case 7:
            return Area( Vector3( parentArea.m_position.x + parentToChildOffset.x,
                                  parentArea.m_position.z - parentToChildOffset.z,
                                  parentArea.m_position.y - parentToChildOffset.y ),
                         childLengths );
        default:
            assert( false && "Invalid child index" );
            return Area();
    }
}

int Octree::getSubdivision( const Area& nodeArea, const BoundingSphere& collider ) const
{
    // En haut
    if( collider.m_position.y - collider.m_radius > nodeArea.m_position.y ) {
        // Haut Ouest
        if( collider.m_position.x + collider.m_radius < nodeArea.m_position.x ) {
            // Haut Sud Ouest
            if( collider.m_position.z + collider.m_radius < nodeArea.m_position.z )
                return 2;
            // Haut Nord Ouest
            else if( collider.m_position.z - collider.m_radius >= nodeArea.m_position.z )
                return 0;
            // Nul part
            else
                return -1;
        }
        // Est
        else if( collider.m_position.x - collider.m_radius >= nodeArea.m_position.x ) {
            // Haut Sud Est
            if( collider.m_position.z + collider.m_radius < nodeArea.m_position.z )
                return 3;
            // Haut Nord Est
            else if( collider.m_position.z - collider.m_radius >= nodeArea.m_position.z )
                return 1;
            // Nul part
            else
                return -1;
        }
        else {
            return -1;
        }
    }
    // En bas
    else if( collider.m_position.y + collider.m_radius <= nodeArea.m_position.y ) {
        // Bas Ouest
        if( collider.m_position.x + collider.m_radius < nodeArea.m_position.x ) {
            // Bas Sud Ouest
            if( collider.m_position.z + collider.m_radius < nodeArea.m_position.z )
                return 6;
            // Bas Nord Ouest
            else if( collider.m_position.z - collider.m_radius >= nodeArea.m_position.z )
                return 4;
            // Nul part
            else
                return -1;
        }
        // Est
        else if( collider.m_position.x - collider.m_radius >= nodeArea.m_position.x ) {
            // Bas Sud Est
            if( collider.m_position.z + collider.m_radius < nodeArea.m_position.z )
                return 7;
            // Bas Nord Est
            else if( collider.m_position.z - collider.m_radius >= nodeArea.m_position.z )
                return 5;
            // Nul part
            else
                return -1;
        }
        else {
            return -1;
        }
    }
    // Nul part
    else
        return -1;
}

void Octree::add( const BoundingSphere* collider ) { add( m_rootNode.get(), 0, m_boundary, collider ); }

void Octree::add( Node* node, std::size_t depth, const Area& area, const BoundingSphere* collider )
{
    assert( node != nullptr );

    // Si le collider est hors de la zone couverte par le quadtree, on ne le considere pas
    if( !area.intersects( *collider ) )
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
        // Sinon, on subdivise le quad en 4 sous-quads et on y met le collider au bon endroit
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
    std::vector<const BoundingSphere*> newColliders;
    for( const BoundingSphere* collider : node->colliders )
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

void Octree::remove( const BoundingSphere* collider ) { remove( m_rootNode.get(), m_boundary, collider ); }

bool Octree::remove( Node* node, const Area& area, const BoundingSphere* collider )
{
    assert( node != nullptr );
    assert( area.intersects( *collider ) );

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

void Octree::removeValue( Node* node, const BoundingSphere* collider )
{
    // On trouve le collider dans le tableau de colliders du noeud
    auto it = std::find_if( std::begin( node->colliders ), std::end( node->colliders ),
                            [ this, &collider ] ( const auto& rhs ) { return collider == rhs; } );
    assert( it != std::end( node->colliders ) && "Trying to remove a value that is not present in the node" );

    // Swap le collider avec le dernier element puis suppression du dernier element
    *it = std::move( node->colliders.back() );
    node->colliders.pop_back();
}

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

std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>> Octree::findAllIntersections() const
{
    std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>> intersections;
    findAllIntersections( m_rootNode.get(), intersections );
    return intersections;
}

void Octree::findAllIntersections(
    Node* node, std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>>& intersections ) const
{
    // Trouve les intersections entre les colliders stockes dans le noeud courant
    // Une meme intersection ne doit pas etre reperee deux fois
    for( auto childIndex = std::size_t( 0 ); childIndex < node->colliders.size(); ++childIndex )
    {
        for( auto secondChildIndex = std::size_t( 0 ); secondChildIndex < childIndex; ++secondChildIndex )
        {
            if( node->colliders[ childIndex ]->collides( *node->colliders[ secondChildIndex ] ) )
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

void Octree::draw() const
{
    draw( m_rootNode.get(), m_boundary );
}

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

void Octree::findIntersectionsInDescendants(
    Node* node, const BoundingSphere* collider,
    std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>>& intersections ) const
{
    // Test d'intersection entre le collider courant et les colliders du noeud enfant
    for( const auto& other : node->colliders )
    {
        if( collider->collides( *other ) )
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


Area::Area( const Vector3& position, const Vector3& lengths )
    : m_position( position ), m_lengths( lengths )
{
}

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

bool BoundingSphere::collides( const BoundingSphere& other ) const
{
    // C'est le travail de Sébastien :eyes:
    return false;
}
