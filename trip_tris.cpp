
#include "MBTagConventions.hpp"
#include "MBCore.hpp"

using namespace moab;

ErrorCode triple_model_tris( Interface *mb )
{

  ErrorCode rval;
  //get the category tag
  Tag cat_tag;   
  rval = mb->tag_get_handle( CATEGORY_TAG_NAME, CATEGORY_TAG_SIZE, MB_TAG_SPARSE, cat_tag, MB_TYPE_OPAQUE );
  ERR_CHECK(rval);

  

  return MB_SUCCESS;
}
  
