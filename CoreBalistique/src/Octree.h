#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <array>
#include <memory>
#include "Vector.h"

// CLASSE TEMPORAIRE POUR REPRESENTER SIMPLEMENT UNE SPHERE ENGLOBANTE EN ATTENDANT SA VRAIE IMPLEMENTATION
class BoundingSphere
{
public:
    float m_radius;
    Vector3 m_position;

    // TODO
    bool collides( const BoundingSphere& other ) const;
};

class Area
{
public:
    Area( const Vector3& position = Vector3(), const Vector3& lengths = Vector3( 1.f, 1.f, 1.f ) );

    Vector3 m_position;
    Vector3 m_lengths;

    bool intersects( const BoundingSphere& collider ) const;
};

// Source d'une grande partie de l'implémentation d'un quadtree :
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

private:
    static constexpr auto Threshold = std::size_t( 16 );
    static constexpr auto MaxDepth = std::size_t( 8 );

    struct Node
    {
        // Subdivisions spatiales du noeud
        std::array<std::unique_ptr<Node>, 8> children;

        // Colliders contenus dans le noeud
        std::vector<const BoundingSphere*> colliders;
    };

    // Position et taille de la zone couverte par le octree
    Area boundary;

    // Racine du octree
    std::unique_ptr<Node> rootNode;

    // Plus aucun enfant
    bool isLeaf( const Node* node ) const { return !static_cast< bool >( node->children[ 0 ] ); }

    // Calcule la zone d'un noeud enfant à partir du parent et de l'index de la nouvelle parcelle
    Area computeBox( const Area& parentArea, int childIndex ) const;

    // Retourne la subdivision dans laquelle un collider se trouve
    int getSubdivision( const Area& nodeArea, const BoundingSphere& collider ) const;

    // Méthode auxiliaire à la méthode publique add
    void add( Node* node, std::size_t depth, const Area& area, const BoundingSphere* collider );

    // Séparation d'un noeud en 4 subdivisions (4 autres noeuds)
    void split( Node* node, const Area& area );

    // Méthode auxiliaire à la méthode publique remove
    bool remove( Node* node, const Area& area, const BoundingSphere* collider );

    // Suppression d'un collider depuis un noeud
    void removeValue( Node* node, const BoundingSphere* collider );

    // Si la quantité de colliders cumulée des noeuds enfants est inférieure au seuil, on les fusionne
    // et tous les colliders sont stockés dans le noeud mère
    bool tryMerge( Node* node );

    // Méthode auxiliaire à la méthode publique findAllIntersections
    void findAllIntersections(
        Node* node, std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>>& intersections ) const;

    // Trouve les intersections entre les colliders du node courant et les colliders de ses noeuds enfants
    void findIntersectionsInDescendants(
        Node* node, const BoundingSphere* collider,
        std::vector<std::pair<const BoundingSphere*, const BoundingSphere*>>& intersections ) const;
};

/*
class PhysicSystem : public ISystem
{
public:
  static PhysicSystem& Instance()
  {
    static PhysicSystem instance;
    return instance;
  }

  PhysicSystem(const PhysicSystem&) = delete;
  PhysicSystem(PhysicSystem&&) = delete;
  PhysicSystem& operator=(const PhysicSystem&) = delete;
  PhysicSystem& operator=(PhysicSystem&&) = delete;

  void Register(gsl::not_null<ColliderComponent*> comp) { colliders.insert(comp); }

  void Unregister(gsl::not_null<ColliderComponent*> comp) { colliders.erase(comp); }

  void Iterate(const Timing& timing) override
  {
    for (auto c : colliders)
    {
      if (c->Enabled() && c->Owner().Active())
      {
        // Ajout du collider au quadtree
        quadtree.add(c);
      }
    }

    // Utilisation du quadtree pour trouver les intersections pertinentes dans l'espace
    auto collisionPairs = quadtree.findAllIntersections();

    // Déclenchement des collisions
    for (const auto& collisionPair : collisionPairs)
    {
      auto c1 = collisionPair.first;
      auto c2 = collisionPair.second;

      c1->OnCollision(*c2);
      c2->OnCollision(*c1);
    }

    // Nettoyage du quadtree
    quadtree.removeAll();
  }

private:
  PhysicSystem()
      : quadtree(Area(0, 0, DisplaySystem::Instance().GetWindow().GetSize().width,
                      DisplaySystem::Instance().GetWindow().GetSize().height))
  {
  }

  ~PhysicSystem() override { Expects(colliders.empty()); }

  std::unordered_set<gsl::not_null<ColliderComponent*>> colliders;
  Quadtree quadtree;
};
*/

#endif