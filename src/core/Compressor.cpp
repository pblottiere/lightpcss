/**
 * @file Compressor.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 29 Jun 2016
 */

#include "utils.hpp"
#include "Compressor.hpp"

// ----------------------------------------------------------------------------
// LAZ compressor
// ----------------------------------------------------------------------------
CompressorLAZ::CompressorLAZ( SchemaPotreeGreyhoundRead &schema )
  : Compressor()
  , _encoder( _buf )
  , _compressor( laszip::formats::make_dynamic_compressor(_encoder) )
  , _pointsize( 0 )
{
  for ( size_t j = 0; j < schema.dimensions_order.size(); j++ )
  {
    std::string dim_name = schema.dimensions_order[j];
    Dimension dim = schema.dimensions[ dim_name ];
    add_field( dim );
  }
}

bool CompressorLAZ::compress( const std::vector<unsigned char> &input,
   std::vector<unsigned char> &output )
{
  bool rc = false;

  const uint8_t *begin = &input[0];
  const uint8_t *end = begin + input.size();

  //std::cout << "pointsize: " << _pointsize << std::endl;

  while (begin + _pointsize <= end)
  {
    //printf("-------------\n");
    //for ( int j=0; j<_pointsize; j++ )
    //  printf("%02x ", begin[j]);
    //printf("\n");

    _compressor->compress((const char*) begin);
    begin += _pointsize;
  }

  _encoder.done();

  output = _buf.buf;

  return rc;
}

bool CompressorLAZ::add_field( Dimension &dim )
{
  switch( dim.size )
  {
    case 2:
      {
        _compressor->template add_field<U16>();
        break;
      }
    case 4:
      {
        _compressor->template add_field<I32>();
        break;
      }
    case 1:
      {
        _compressor->template add_field<U8>();
        break;
      }
  }

  _pointsize += dim.size;

  return true;
}
