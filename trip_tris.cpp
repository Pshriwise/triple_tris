
#include "MBTagConventions.hpp"
#include "MBCore.hpp"
#include "MBCartVect.hpp"

#include "trip_tris.hpp"

#include <iostream>

using namespace moab;

ErrorCode triple_model_tris( Interface *mb )
{

  ErrorCode rval;
  //get the category tag
  Tag cat_tag;   
  rval = mb->tag_get_handle( CATEGORY_TAG_NAME, CATEGORY_TAG_SIZE, MB_TYPE_OPAQUE, cat_tag, MB_TAG_CREAT | MB_TAG_SPARSE );
  ERR_CHECK(rval);

  //now get all surfaces in the model
  char surf_category[32] = "Surface\0";
  const void *ptr = &surf_category;
  Range surfs;
  rval = mb->get_entities_by_type_and_tag( 0, MBENTITYSET, &cat_tag, &ptr, 1, surfs);
  ERR_CHECK(rval);

  std::cout << "Surfaces in this model: " << surfs.size() << std::endl; 

  //now for each surface, get the triangles
  for( Range::iterator i = surfs.begin(); i != surfs.end(); i++)
    {

      std::vector<EntityHandle> surf_tris;
      rval = mb->get_entities_by_type( *i, MBTRI, surf_tris);
      ERR_CHECK(rval);

      //and for every triangle, replace it with three triangles which together represent the original
      for( unsigned int j = 0; j < surf_tris.size(); j++)
	{
	  
	  rval = triple_triangle( mb, *i, surf_tris[j]);
	  ERR_CHECK(rval);

	}

    }

  return MB_SUCCESS;
}
  

ErrorCode triple_triangle( Interface* mb, EntityHandle meshset, EntityHandle triangle)
{
  
  ErrorCode rval; 
  //get the vertices of the triangle
  const EntityHandle* conn; 
  int n_verts; 
  
  rval = mb->get_connectivity( triangle, conn, n_verts);
  ERR_CHECK(rval); 
  
  assert(3 == n_verts);

  //get the coordinates of these vertices
  CartVect coords[3];
  
  rval = mb->get_coords( conn, n_verts, coords[0].array() ); 
  ERR_CHECK(rval);
  





}
