
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

  int orig_tri_count = 0;

  //now for each surface, get the triangles
  for( Range::iterator i = surfs.begin(); i != surfs.end(); i++)
    {

      std::vector<EntityHandle> surf_tris;
      rval = mb->get_entities_by_type( *i, MBTRI, surf_tris);
      ERR_CHECK(rval);
      
      orig_tri_count += int( surf_tris.size() );

      //and for every triangle, replace it with three triangles which together represent the original
      for( unsigned int j = 0; j < surf_tris.size(); j++)
	{
	  
	  rval = triple_triangle( mb, *i, surf_tris[j]);
	  ERR_CHECK(rval);

	}

    }

  std::cout << "Original number of triangles: " << orig_tri_count << std::endl; 

  Range all_tris; 

  rval = mb->get_entities_by_type( 0, MBTRI, all_tris);
  ERR_CHECK(rval); 

  std::cout << "New number of triangles: " << all_tris.size() << std::endl; 

  return MB_SUCCESS;
}
  

ErrorCode triple_triangle( Interface* mb, EntityHandle surfset, EntityHandle triangle)
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
  

  CartVect center_pnt = (1/3)*coords[0] + (1/3)*coords[1] + (1/3)*coords[2];
  EntityHandle center_vtx; 


  //create a new vertex for this point 
  rval = mb->create_vertex( &(center_pnt[0]), center_vtx); 
  ERR_CHECK(rval);

  //now create the new triangles
  EntityHandle new_triangles[3];
  EntityHandle new_tri_verts[3]; 
  
  //Triangle 1
  new_tri_verts[0] = center_vtx; 
  new_tri_verts[1] = conn[0];
  new_tri_verts[2] = conn[1];

  rval = mb->create_element( MBTRI, &new_tri_verts[0], 3, new_triangles[0]);
  ERR_CHECK(rval);

  //Triangle 2
  new_tri_verts[0] = center_vtx; 
  new_tri_verts[1] = conn[1];
  new_tri_verts[2] = conn[2];

  rval = mb->create_element( MBTRI, &new_tri_verts[0], 3, new_triangles[1]);
  ERR_CHECK(rval);
  
  //Triangle 3
  new_tri_verts[0] = center_vtx; 
  new_tri_verts[1] = conn[2];
  new_tri_verts[2] = conn[0];

  rval = mb->create_element( MBTRI, &new_tri_verts[0], 3, new_triangles[2]);
  ERR_CHECK(rval);

  //remove the original triangle from the model (original verts should remain)
  rval = mb->delete_entities( &triangle, 1); 
  ERR_CHECK(rval); 

  //now add the new vertex and the new triangles to the meshset
  rval = mb->add_entities( surfset, &center_vtx, 1); 
  ERR_CHECK(rval); 

  rval = mb->add_entities( surfset, &new_triangles[0], 3); 
  ERR_CHECK(rval); 


  return MB_SUCCESS;
}
