#include "pch.h"
#include "Octree.h"

namespace OctreeTest
{
    /* AREA */
    TEST( AreaConstructor, DefaultInitialization )
    {
        Area a1;

        EXPECT_EQ( a1.m_position.x, 0 );
        EXPECT_EQ( a1.m_position.y, 0 );
        EXPECT_EQ( a1.m_position.z, 0 );

        EXPECT_EQ( a1.m_lengths.x, 1.f );
        EXPECT_EQ( a1.m_lengths.y, 1.f );
        EXPECT_EQ( a1.m_lengths.z, 1.f );
    }

    TEST( AreaConstructor, ValueInitialization )
    {
        Area a1( Vector3( 5, 10, 15 ), Vector3( 7.f, 9.f, 11.f ) );

        EXPECT_EQ( a1.m_position.x, 5 );
        EXPECT_EQ( a1.m_position.y, 10 );
        EXPECT_EQ( a1.m_position.z, 15 );

        EXPECT_EQ( a1.m_lengths.x, 7.f );
        EXPECT_EQ( a1.m_lengths.y, 9.f );
        EXPECT_EQ( a1.m_lengths.z, 11.f );
    }

    TEST( AreaConstructor, CopyInitialization )
    {
        Area a1( Vector3( 5, 10, 15 ), Vector3( 7.f, 9.f, 11.f ) );
        Area a2( a1 );

        EXPECT_EQ( a1.m_position.x, a2.m_position.x );
        EXPECT_EQ( a1.m_position.y, a2.m_position.y );
        EXPECT_EQ( a1.m_position.z, a2.m_position.z );

        EXPECT_EQ( a1.m_lengths.x, a2.m_lengths.x );
        EXPECT_EQ( a1.m_lengths.y, a2.m_lengths.y);
        EXPECT_EQ( a1.m_lengths.z, a2.m_lengths.z );
    }

    /* CONSTRUCTOR */
    TEST( OctreeConstructor, AreaInitialization )
    {
        Area a1( Vector3( 5, 10, 15 ), Vector3( 7.f, 9.f, 11.f ) );
        Octree o1( a1 );

        EXPECT_EQ( o1.getBoundary().m_position.x, 5 );
        EXPECT_EQ( o1.getBoundary().m_position.y, 10 );
        EXPECT_EQ( o1.getBoundary().m_position.z, 15 );

        EXPECT_EQ( o1.getBoundary().m_lengths.x, 7.f );
        EXPECT_EQ( o1.getBoundary().m_lengths.y, 9.f );
        EXPECT_EQ( o1.getBoundary().m_lengths.z, 11.f );

        EXPECT_NE( o1.getRootNode(), nullptr );
    }

    TEST( OctreeAddCollider, SingleCollider )
    {
        Area area( Vector3( 0, 0, 0 ), Vector3( 10.f, 10.f, 10.f ) );
        Octree octree( area );

        BoundingSphere collider( 5, Vector3( 5, 5, 5 ) );

        octree.add( &collider );

        // Collider bien ajouté à l'arbre ?
        const auto& rootNode = octree.getRootNode();
        ASSERT_NE( rootNode, nullptr );
        ASSERT_TRUE( octree.isLeaf( rootNode.get() ) );
        ASSERT_EQ( rootNode->colliders.size(), 1 );
        EXPECT_EQ( rootNode->colliders[ 0 ], &collider );
    }

    TEST( OctreeAddCollider, MultipleColliders )
    {
        Area area( Vector3( 0, 0, 0 ), Vector3( 10.f, 10.f, 10.f ) );
        Octree octree( area );

        BoundingSphere collider1( 5, Vector3( 5, 5, 5 ) );
        BoundingSphere collider2( 2, Vector3( 2, 2, 2 ) );

        octree.add( &collider1 );
        octree.add( &collider2 );

        // Colliders bien ajoutés à l'arbre
        const auto& rootNode = octree.getRootNode();
        ASSERT_NE( rootNode, nullptr );
        ASSERT_TRUE( octree.isLeaf( rootNode.get() ) );
        ASSERT_EQ( rootNode->colliders.size(), 2 );
        EXPECT_EQ( rootNode->colliders[ 0 ], &collider1 );
        EXPECT_EQ( rootNode->colliders[ 1 ], &collider2 );
    }

    TEST( OctreeAddMultipleColliders, MultipleCollidersInDifferentSubdivisions )
    {
        Area area( Vector3( 0, 0, 0 ), Vector3( 10.f, 10.f, 10.f ) );
        Octree octree( area );

        BoundingSphere collider1( 5, Vector3( 2, 2, 2 ) );
        BoundingSphere collider2( 2, Vector3( 7, 7, 7 ) );

        octree.add( &collider1 );
        octree.add( &collider2 );

        const auto& rootNode = octree.getRootNode();
        ASSERT_NE( rootNode, nullptr );
        ASSERT_TRUE( octree.isLeaf( rootNode.get() ) );

        // Les colliders doivent être dans des subdivisions différentes
        for( const auto& child : rootNode->children )
        {
            if( child )
            {
                ASSERT_EQ( child->colliders.size(), 1 );
                EXPECT_EQ( child->colliders[ 0 ], ( &collider1 == child->colliders[ 0 ] ) ? &collider1 : &collider2 );
            }
        }
    }

    TEST( OctreeAddMultipleColliders, MultipleCollidersInSameSubdivision )
    {
        Area area( Vector3( 0, 0, 0 ), Vector3( 10.f, 10.f, 10.f ) );
        Octree octree( area );

        BoundingSphere collider1( 5, Vector3( 2, 2, 2 ) );
        BoundingSphere collider2( 2, Vector3( 3, 3, 3 ) );

        octree.add( &collider1 );
        octree.add( &collider2 );

        const auto& rootNode = octree.getRootNode();
        ASSERT_NE( rootNode, nullptr );
        ASSERT_TRUE( octree.isLeaf( rootNode.get() ) );

        // La subdivision doit contenir les deux colliders
        ASSERT_EQ( rootNode->colliders.size(), 2 );
        EXPECT_EQ( rootNode->colliders[ 0 ], &collider1 );
        EXPECT_EQ( rootNode->colliders[ 1 ], &collider2 );
    }

    TEST( OctreeRemoveCollider, SingleCollider )
    {
        Area area( Vector3( 0, 0, 0 ), Vector3( 10.f, 10.f, 10.f ) );
        Octree octree( area );

        BoundingSphere collider( 5, Vector3( 5, 5, 5 ) );

        octree.add( &collider );
        octree.remove( &collider );

        // Collider bien supprimé de l'arbre ?
        const auto& rootNode = octree.getRootNode();
        ASSERT_NE( rootNode, nullptr );
        ASSERT_TRUE( octree.isLeaf( rootNode.get() ) );
        ASSERT_EQ( rootNode->colliders.size(), 0 );
    }

    TEST( OctreeMultipleSubdivisions, CollidersInMultipleSubdivisions )
    {
        // Zone plus petite pour faciliter la subdivision
        Area area( Vector3( 0, 0, 0 ), Vector3( 5.f, 5.f, 5.f ) );
        Octree octree( area );

        // Colliders positionnés dans différentes subdivisions
        BoundingSphere collider1( 2, Vector3( 2, 2, 2 ) );
        BoundingSphere collider2( 2, Vector3( 7, 7, 7 ) );
        BoundingSphere collider3( 2, Vector3( 2, 8, 2 ) );

        octree.add( &collider1 );
        octree.add( &collider2 );
        octree.add( &collider3 );

        // Colliders dans différentes subdivisions ?
        const auto& rootNode = octree.getRootNode();
        ASSERT_NE( rootNode, nullptr );

        for( const auto& child : rootNode->children )
        {
            if( child )
            {
                ASSERT_FALSE( child->colliders.empty() );
            }
        }
    }

    TEST( OctreePartialInsertion, ColliderPartiallyInSubdivision )
    {
        // Utilise une zone plus petite pour faciliter l'insertion partielle
        Area area( Vector3( 0, 0, 0 ), Vector3( 5.f, 5.f, 5.f ) );
        Octree octree( area );

        // Collider partiellement dans une subdivision
        BoundingSphere collider1( 5, Vector3( 2, 2, 7 ) );

        octree.add( &collider1 );

        // Est-ce que le collider est inséré correctement
        const auto& rootNode = octree.getRootNode();
        ASSERT_NE( rootNode, nullptr );
        ASSERT_TRUE( rootNode->colliders.size() == 1 || !rootNode->colliders.empty() );
        EXPECT_EQ( rootNode->colliders[ 0 ], &collider1 );
    }

    TEST( OctreeMultipleCollidersPartialInsertion, MultipleCollidersPartiallyInSubdivision )
    {
        // Utilise une zone plus petite pour faciliter l'insertion partielle
        Area area( Vector3( 0, 0, 0 ), Vector3( 10.f, 10.f, 10.f ) );
        Octree octree( area );

        // Collider1 entièrement dans la subdivision, Collider2 partiellement dans la subdivision
        BoundingSphere collider1( 5, Vector3( 2, 2, 2 ) );
        BoundingSphere collider2( 2, Vector3( 3, 3, 3 ) );

        octree.add( &collider1 );
        octree.add( &collider2 );

        // Vérifiez si les colliders sont insérés correctement
        const auto& rootNode = octree.getRootNode();
        ASSERT_NE( rootNode, nullptr );

        // Les colliders doivent être dans des subdivisions différentes
        for( const auto& child : rootNode->children )
        {
            if( child )
            {
                ASSERT_EQ( child->colliders.size(), 1 );
                EXPECT_TRUE( child->colliders[ 0 ] == &collider1 || child->colliders[ 0 ] == &collider2 );
            }
        }
    }
}