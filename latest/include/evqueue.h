#pragma once
// simkernel event queue implementation

#include <sys/timeb.h>
#include <cstdio>
#include <cassert>
#include <vector>
#include <map>
//#include "sim_event.h"
//#include "sim_predicate.h"
#include <iostream>


sim_event EMPTY_EVENT;



struct qstats
{
  int add_insrt_count;
  int nadds;
  int nrets;
  int ret_insrt_count;
  int maxlen;
  int nbrea;
  int reasteps;

  qstats() : maxlen( 0 )
  {
    nadds = nrets = add_insrt_count = ret_insrt_count = nbrea = reasteps = 0;
  }
};



class Node
{
public:
  sim_event e;
  double time;

  //links for events tree
  Node * father;
  Node * left;
  Node * right;

  //links for  Id indexed links
  Node * prev;
  Node * next;

  bool isSmallest;
  bool isLargest;

  Node( sim_event & eTmp )
  {
    e = eTmp;
    time = e.event_time();
    father = NULL;
    left = NULL;
    right = NULL;
    prev = NULL;
    next = NULL;

    isSmallest  = false;
    isLargest = false;
  }

  ~Node()
  {
  }

  void link( Node * mySon, bool isLeft )
  {
    if ( isLeft ) this->left = mySon;
    else
      this->right = mySon;

    if ( mySon != NULL ) mySon->father = this;

  }


  bool isLeaf()
  {
    if ( ( left == NULL ) && ( right == NULL ) ) return true;
    else
      return false;

  }

  void print_node()
  {
//    using namespace std;
   std::cout<<"dest "<<e.get_dest()<<" time "<<e.event_time();

    if(isSmallest) std::cout<<"isSmallest";
    if(isLargest) std::cout<<"isLargest";
    std::cout<<std::endl;

  }
};



class evTree
{
public:
  Node * root;
  Node * smallest;
  Node * largest;
  int size;

  evTree()
  {
    root = NULL;
    smallest = NULL;
    largest = NULL;
    size = 0;

  }

  ~evTree()
  { //to write
  }

  Node* find( Node * startNd, double thisTime, qstats &qt )
  {
    //using namespace std;
    qt.add_insrt_count++;
    if ( thisTime < startNd->time )
    {
      if ( startNd->left == NULL )
        return startNd;
      else
        return find( startNd->left, thisTime, qt );
    }
    else
    {
      if ( startNd->right == NULL )
         return startNd;
      else
       return find( startNd->right, thisTime, qt );
    }
  }



  Node * findSmallest( Node * nd , qstats & qt)
  {
    qt.ret_insrt_count++;
    if ( nd->left == NULL ) return nd;
    else
     return findSmallest( nd->left, qt );
  }

  Node * findLargest( Node * nd, qstats & qt )
  {
    qt.ret_insrt_count++;
    if ( nd->right == NULL ) return nd;
    else
     return findLargest( nd->right, qt );
  }


  void insert( Node * nd, qstats & qt )
  {
    //using namespace std;
    assert( nd != NULL );

    double myTime = nd->time;
    bool isLeft;
    if ( root == NULL )
    {
      root = nd;
      smallest = nd;
      largest = nd;
      nd->isSmallest = true;
      nd->isLargest = true;
      qt.add_insrt_count++;
    }
    else
    {
      Node* father = NULL;
      //bool toLeft;

      if ( myTime < smallest->time )
      {
        smallest->isSmallest = false;
        father = smallest;
        smallest = nd;
        nd->isSmallest = true;
        qt.add_insrt_count++;

      }
      else if ( myTime >= largest->time )
      {
        largest->isLargest = false;
        father = largest;
        largest = nd;
        nd->isLargest = true;
        qt.add_insrt_count++;

      }
      else
      {
        father = find( root, myTime, qt );
      }

      assert( father != NULL );
      if ( nd->time < father->time ) isLeft = true;
      else isLeft = false;

      father->link( nd, isLeft );
    }

    size++;
    if (size>qt.maxlen) qt.maxlen=size;
  }

  sim_event erase( Node * nd, qstats & qt )
  {
    assert( nd != NULL );
    Node * myFather = nd->father;
    Node * myLeft = nd->left;
    Node * myRight = nd->right;
    sim_event eTmp = nd->e;
    bool isLeft = false;

    if ( myFather == NULL )
    { //nd is root
      if ( myRight != NULL )
      { //rson replace myself, and my left insert to rson tree
        root = myRight;
        if ( myLeft != NULL )
        {
          Node * newFather = find( myRight, myLeft->time, qt );
          if ( myLeft->time < newFather->time ) isLeft = true;
          newFather->link( myLeft, isLeft );
        }
      }
      else
      { //no right son, lson will replace myself
        root = myLeft;
      }

      if ( root != NULL ) root->father = NULL;
    }
    else
    { //nd is not root
      if ( nd->time < myFather->time ) isLeft = true;

      if ( myRight != NULL ) //rson replace myself, and my left insert to rson tree
      {
        myFather->link( myRight, isLeft );

        if ( myLeft != NULL )
        {
          Node * newFather = find( myRight, myLeft->time, qt );
          assert(newFather!=NULL);
          if(myLeft->time<newFather->time) isLeft = true;
          newFather->link( myLeft, isLeft );
        }
      }
      else
      { //no right son, lson will replace myself
        myFather->link( myLeft, isLeft );
      }

    }

    //maintain smallest and largest
    if ( nd->isSmallest )
    {
      nd->isSmallest= false;
      if ( myRight != NULL ) smallest = findSmallest( myRight, qt );
      else if ( myFather != NULL )
        smallest = myFather;
      else
        smallest = NULL;

      if ( smallest != NULL ) smallest->isSmallest = true;
    }

    if ( nd->isLargest )
    {
      if ( myLeft != NULL )
        largest = findLargest( myLeft, qt );
      else if ( myFather != NULL )
        largest = myFather;
      else
        largest = NULL;


      if ( largest != NULL ) largest->isLargest = true;
    }

    size--;


    return eTmp;

  }

  void print_tree( Node * nd )
  {
    if ( nd->left != NULL )
      print_tree( nd->left );

    nd->print_node();

    if ( nd->right != NULL ) print_tree( nd->right );
  }

  void print_all_tree()
  {
    //using namespace std;
    if ( root != NULL )
    {
      print_tree( root );
      std::cout << "root is " << std::endl;
      root->print_node();

      std::cout << "smallest node: " << std::endl;
      smallest->print_node();
      std::cout << "largest node: " << std::endl;
      largest->print_node();
    }
    else
    {
      std::cout << "this is an empty tree" << std::endl;
    }

  }

#ifdef _POSIX
  void printTreeStruc( Node * node, int level )
  {
    std::string sb = "|";
    std::string tb = "";
    std::string lb = "|-----|";
    for ( int i = 0; i < level; i++ )
    {
      tb = tb + "\t";

    }

    sb = sb + tb + lb;
    if ( node == NULL )
    {
      std::cout << sb << "Empty Node" << std::endl;
    }
    else
    {
      std::cout << sb;
      assert( node != NULL );
      node->print_node();
      printTreeStruc( node->left, level + 1 );
      printTreeStruc( node->right, level + 1 );
    }

  }
#endif

#ifdef _WIN32 
	void printTreeStruc( Node * node, int level )
	{
    
	std::string tb = "";
	std::string lb = "|-----|";
	std::cout << "|";
	for ( int i = 0; i < level; i++ )
	{
		std::cout << "\t";

	}

	std::cout << "|-----|";
	if ( node == NULL )
	{
		std::cout << "Empty Node" << std::endl;
	}
	else
	{
		assert( node != NULL );
		node->print_node();
		printTreeStruc( node->left, level + 1 );
		printTreeStruc( node->right, level + 1 );
	}
  }
#endif

};



typedef std::pair < std::map < sim_entity_id, std::vector < Node * > >::iterator, bool > mapPair;



/** ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
 the events node is also organized as a link indexed by ID, these link are then orgnazied to a map
 the map record the first node and last node of the link
 ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff */
class evIdMap
{
public:
  std::map < sim_entity_id, std::vector < Node * > > idMap; //the vector<Node *> record the first node and last node of the link

  evIdMap()
  {
  }

  ~evIdMap()
  {
  }

  void insert( Node * nd )
  {
    assert( nd != NULL );
    sim_entity_id dID = nd->e.get_dest();
    std::map < sim_entity_id, std::vector < Node * > >::iterator pos = idMap.find( dID );

    if ( pos == idMap.end() )
    { //dID has no events before
      std::vector < Node * > vTmp;
      mapPair insert_pair = idMap.insert( std::map < sim_entity_id, std::vector < Node * > >::value_type( dID, vTmp ) );
      assert( insert_pair.second );
      insert_pair.first->second.push_back( nd ); //first node
      insert_pair.first->second.push_back( nd ); //last node
    }
    else
    {
      Node * lastNode = ( pos->second ) [1];

      if ( lastNode != NULL )
      {
        lastNode->next = nd;
        nd->prev = lastNode;
        ( pos->second ) [1] = nd;
      }
      else
      { //no node in the list
        ( pos->second ) [0] = nd;
        ( pos->second ) [1] = nd;
      }

    }

  }

  Node * retrieveID( sim_entity_id dID, sim_predicate & p , qstats & qt)
  {
    Node* index = NULL;


    std::map < sim_entity_id, std::vector < Node * > >::iterator pos = idMap.find( dID );
    if ( pos != idMap.end() )
    {
      index = ( pos->second ) [0];

      while ( index != NULL )
      {
        if ( p.match( index->e ) ) break;
        index = index->next;
        qt.ret_insrt_count++;
      }
      //update first and last
      if ( index != NULL )
      {
        if(index->prev!=NULL) index->prev->next = index->next;
        else ( pos->second ) [0] = index->next;

        if(index->next!=NULL) index->next->prev = index->prev;
        else ( pos->second ) [1] = index->prev;
      }
    }
      return index;

  }

  void erase( Node * nd )
  {
    assert( nd != NULL );

    //cout << "try to delete nd from the idmap" << endl;
    //nd->print_node();

    sim_entity_id dID = nd->e.get_dest();
    std::map < sim_entity_id, std::vector < Node * > >::iterator pos = idMap.find( dID );

    assert( pos != idMap.end() );

    if ( nd->prev != NULL ) nd->prev->next = nd->next;
    else
      ( pos->second ) [0] = nd->next;

    if ( nd->next != NULL ) nd->next->prev = nd->prev;
    else
      ( pos->second ) [1] = nd->prev;
  }

  int waiting( sim_entity_id dID, sim_predicate & p, qstats & qt )
  {

    int w = 0;
    std::map < sim_entity_id, std::vector < Node * > >::iterator pos = idMap.find( dID );
    if ( pos != idMap.end() )
    {
      Node * index = ( pos->second ) [0];

      while ( index != NULL )
      {
        if ( p.match( index->e ) ) w++;
        index = index->next;
        qt.ret_insrt_count++;

      }
    }
    return w;
  }


  void print_evIdMap()
  {
    std::map < sim_entity_id, std::vector < Node * > >::iterator pos = idMap.begin();

    while ( pos != idMap.end() )
    {
      Node * index = ( pos->second ) [0]; //start node

      std::cout << "Node " << pos->first << ": " << std::endl;
      while ( index != NULL )
      {
        assert( index != NULL );
        index->print_node();
        index = index->next;
      }
      pos++;
    }

  }
};



class evqueue
{
  evTree events; //event queue, indexed by dest-entity id, and each vector sorted by time

  evIdMap myMap;

  int num; //number of events in the queue

  qstats stats;

public:
  evqueue()
  {
    num = 0;

    empty();
  }

  ~evqueue()
  {

  }

  void empty()
  {
    num = 0;
  }



  void add( sim_event & e )
  {
#ifdef _POSIX
    struct timeb start;
#endif

    Node * evNode = new Node( e );


    events.insert( evNode, stats );


    myMap.insert( evNode );

    stats.nadds++;


    num++;


  }



  void report( const char * title, bool verbose = true )
  {
    if ( verbose )
      printf( "%s : total adds %d, inserts %d\n : total rets %d, inserts %d\n : max qlen %d\n", title, stats.nadds,
           stats.add_insrt_count, stats.nrets, stats.ret_insrt_count, stats.maxlen );
  }

  int size()
  {
    return num;
  }


  sim_event inspect( int i )
  {
    assert( num > 0 );
    assert( i == 0 );
    return events.smallest->e;

  }


  sim_event retrieve( int i )
  {
    struct timeb start;
    ftime( & start );
    assert( num > 0 );
    assert( i == 0 );
    stats.nrets++;

    Node * nd = events.smallest;
    assert( nd != NULL );


    sim_event eTmp = events.erase( nd, stats);
    myMap.erase( nd );

    delete nd;
    num--;


    return eTmp;

  }


  int waiting( sim_entity_id d, sim_predicate & p )
  {
    return myMap.waiting( d, p, stats );


  }

  sim_event retrieve_dest( sim_entity_id d, sim_predicate & p )
  {
    stats.nrets++;
    Node * nd = myMap.retrieveID( d, p, stats );
    sim_event eTmp = EMPTY_EVENT;

    if ( nd != NULL )
    {
      eTmp = events.erase( nd, stats );
      delete nd;
      num--;
    }

    return eTmp;

  }

  void print_events()
  {
    events.printTreeStruc( events.root, 0 );
  }

  void print_idMap()
  {
    myMap.print_evIdMap();
  }
  //
};






