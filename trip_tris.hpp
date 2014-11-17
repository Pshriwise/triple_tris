

#include "MBTagConventions.hpp"
#include "MBCore.hpp"
#include <assert.h>

using namespace moab;

ErrorCode triple_model_tris( Interface *mb ); 

ErrorCode triple_triangle( Interface* mb, EntityHandle meshset, EntityHandle triangle);


inline void ERR_CHECK( ErrorCode rval)
{
  if ( MB_SUCCESS != rval ) assert(false);
}
