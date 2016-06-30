/**
 * @file Compressor.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 29 Jun 2016
 */

#pragma COMPRESSOR_HPP

#include <vector>
#include <string>

#include <laz-perf/common/common.hpp>
#include <laz-perf/compressor.hpp>
#include <laz-perf/decompressor.hpp>
#include <laz-perf/encoder.hpp>
#include <laz-perf/decoder.hpp>
#include <laz-perf/formats.hpp>
#include <laz-perf/las.hpp>

class SchemaPotreeGreyhoundRead;
class Dimension;

// ----------------------------------------------------------------------------
// Compressor base
// ----------------------------------------------------------------------------
class Compressor
{
  public:
    virtual bool compress( const std::vector<unsigned char> &input,
        std::vector<unsigned char> &output ) = 0;
};

// ----------------------------------------------------------------------------
// LAZ compressor
// ----------------------------------------------------------------------------
struct LazPerfBuf
{
  LazPerfBuf() : buf(), idx(0) {}

  const uint8_t*  data()
  {
      return reinterpret_cast<const uint8_t*>(buf.data());
  }

  void putBytes(const unsigned char* b, size_t len)
  {
    while(len --)
      buf.push_back(*b++);
  }

  void putByte(const unsigned char b)
  {
    buf.push_back(b);
  }

  unsigned char getByte()
  {
    return buf[idx++];
  }

  void getBytes(unsigned char *b, int len)
  {
    for (int i = 0 ; i < len ; i ++)
      b[i] = getByte();
  }

  std::vector<unsigned char> buf;
  size_t idx;
};

typedef laszip::encoders::arithmetic<LazPerfBuf> LazPerfEncoder;
typedef laszip::formats::dynamic_field_compressor<LazPerfEncoder>::ptr LazPerfCompressor;

class CompressorLAZ : public Compressor
{
  public:
    CompressorLAZ( SchemaPotreeGreyhoundRead &schema );

    bool compress( const std::vector<unsigned char> &input,
        std::vector<unsigned char> &output ) override;

  private:
    bool add_field( Dimension &dim );

    LazPerfBuf _buf;
    LazPerfEncoder _encoder;
    LazPerfCompressor _compressor;
    size_t _pointsize;
};
