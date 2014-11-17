

#include "trip_tris.hpp"
#include "MBCore.hpp"
#include "moab/ProgOptions.hpp"



using namespace moab;


int main( int argc, char **argv)
{

  std::string filename; 

  ProgOptions po("Triple Triangles: A program for generating 3x the number of triangles in a model, but with the same representation as before"); 

  po.addRequiredArg<std::string>("filename", "File containing the model for which the user would like to triple the number of triangles.", &filename); 

  po.parseCommandLine( argc, argv );

  //create a new moab instance
  Interface *mb = new MBCore(); 
  // load the file 

  return 0;
}
