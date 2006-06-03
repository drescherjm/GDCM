/*
 * Pseudo code for ultimately a SAX-like (Simple API for DICOM)
 */
#include "gdcmParser.h"

#include <iostream>
#include <fstream>

namespace gdcm
{
  // FIXME
#define XML_FMT_INT_MOD "l"

static void startElement(void *userData, const Tag &name,
  const char *atts[])
{
  int i;
  int *depthPtr = (int *)userData;
  for (i = 0; i < *depthPtr; i++)
    putchar('\t');
  std::cout << name << std::endl;
  *depthPtr += 1;
}

static void endElement(void *userData, const Tag &name)
{
  int *depthPtr = (int *)userData;
  *depthPtr -= 1;
}

} // end namespace gdcm

int TestSADParser(int argc, char *argv[])
{
  if( argc < 2 )
    {
    return 1;
    }
  std::string filename = argv[1];
  std::ifstream is( filename.c_str() );

  char buf[BUFSIZ];
  gdcm::Parser parser;
  int done;
  int depth = 0;
  parser.SetUserData(&depth);
  parser.SetElementHandler(gdcm::startElement, gdcm::endElement);
  do {
    is.read(buf, sizeof(buf));
    size_t len = is.gcount();
    done = len < sizeof(buf);
    if (parser.Parse(buf, len, done) == gdcm::STATUS_ERROR) {
      fprintf(stderr,
        "%s at line %" XML_FMT_INT_MOD "u\n",
        gdcm::Parser::ErrorString(parser.GetErrorCode()),
        parser.GetCurrentOffset());
      return 1;
    }
  } while (!done);

  is.close();
  return 0;
}

