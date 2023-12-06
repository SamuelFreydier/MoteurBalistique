#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <array>
#include <memory>
#include "Vector.h"
#include "BoundingSphere.h"

class Area
{
public:
    Area( const Vector3& position = Vector3(), const Vector3& lengths = Vector3( 1.f, 1.f, 1.f ) );

    Vector3 m_position;
    Vector3 m_lengths;

    // Est-ce que le collider est dans la zone ?
    bool intersects( const BoundingSphere& collider ) const;

    // Dessin de la zone
    void draw() const;
};

// Arbre de partitionnement de l'espace en parallélépipèdes
// Inspiration (Quadtree en C++) :
// https://pvigier.github.io/2019/08/04/quadtree-collision-detection.html
class Octree
{
public:
    Octree( const Area& inBoundary );

    // Ajoute un collider au octree
    void add( const BoundingSphere* collider );

    // Supprime un collider du octree
    void remove( const BoundingSphere* collider );

    // Supprime tous les colliders du octree
    void removeAll();

    // Renvoie un tableau de tous les couples de colliders en intersection
    std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>> findAllIntersections() const;

    // Dessine les subdivisions graphiquement
    void draw() const;

private:
    // Nombre maximum de colliders par subdivision (ici 2 comme vu en cours)
    static constexpr auto Threshold = std::size_t( 2 );

    // Profondeur maximale de l'arbre
    static constexpr auto MaxDepth = std::size_t( 16 );

    struct Node
    {
        // Subdivisions spatiales du noeud
        std::array<std::unique_ptr<Node>, 8> children;

        // Colliders contenus dans le noeud
        std::vector<const BoundingSphere*> colliders;
    };

    // Position et taille de la zone couverte par le octree
    Area m_boundary;

    // Racine du octree
    std::unique_ptr<Node> m_rootNode;

    // Calcule la zone d'un noeud enfant à partir du parent et de l'index de la nouvelle parcelle
    Area computeBox( const Area& parentArea, int childIndex ) const;

    // Retourne la subdivision dans laquelle un collider se trouve
    int getSubdivision( const Area& nodeArea, const BoundingSphere& collider ) const;

    // Méthode auxiliaire récursive à la méthode publique add
    void add( Node* node, std::size_t depth, const Area& area, const BoundingSphere* collider );

    // Séparation d'un noeud en 8 subdivisions (8 autres noeuds)
    void split( Node* node, const Area& area );

    // Méthode auxiliaire récursive à la méthode publique remove
    bool remove( Node* node, const Area& area, const BoundingSphere* collider );

    // Suppression d'un collider depuis un noeud
    void removeValue( Node* node, const BoundingSphere* collider );

    // Si la quantité de colliders cumulée des noeuds enfants est inférieure au seuil, on les fusionne
    // et tous les colliders sont stockés dans le noeud mère
    bool tryMerge( Node* node );

    // Méthode auxiliaire récursive à la méthode publique findAllIntersections
    void findAllIntersections(
        Node* node, std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>>& intersections ) const;

    // Trouve les intersections entre les colliders du node courant et les colliders de ses noeuds enfants
    void findIntersectionsInDescendants(
        Node* node, const BoundingSphere* collider,
        std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>>& intersections ) const;

    // Méthode auxiliaire récursive de dessin des zones de l'arbre
    void draw( Node* parentNode, const Area& parentArea ) const;

public:
    // Plus aucun enfant
    bool isLeaf( const Node* node ) const { return !static_cast< bool >( node->children[ 0 ] ); }

    // Getters
    const Area& getBoundary() const { return m_boundary; }
    const std::unique_ptr<Node>& getRootNode() const { return m_rootNode; }
};

#endif