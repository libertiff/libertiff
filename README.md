# libertiff

## What is libertiff?

Libertiff is a C++11 simple, [header-only](libertiff.hpp), TIFF reader. It is MIT licensed.

Handles both ClassicTIFF and BigTIFF, little-endian or big-endian ordered.

The library does not offer codec facilities (and probably won't). It is mostly aimed at
browsing through the linked chain of Image File Directory (IFD) and their tags.

"Offline" tag values are not loaded at IFD opening time, but only upon
request, which helps handling files with tags with an arbitrarily large
number of values.

The library is thread-safe (that is the instances that it returns can
be used from multiple threads), if passed FileReader instances are themselves
thread-safe.

The library does not throw exceptions (but underlying std library might
throw exceptions in case of out-of-memory situations)

Optional features:
- define LIBERTIFF_C_FILE_READER before including libertiff.hpp, so that
  the libertiff::CFileReader class is available

## How to use it?

Look at the [demo.cpp](demo.cpp) test program.

## Example

```console
$ ./demo ../tests/data/geotiff.tif
File: ../tests/data/geotiff.tif
IFD 0, at offset 408:
Width: 20
Height: 20
SamplesPerPixel: 1
PlanarConfiguration: Contiguous (1)
Compression: None (1)
PhotometricInterpretation: MinIsBlack (1)
Predictor: 0
SampleFormat: UnsignedInt (1)
Organization: strip
Number of strips: 1
RowsPerStrip: 20
StripOffsets: 8
StripByteCounts: 400
All tags (15):
  Code=ImageWidth(256, 0x100), Type=Short(3), Count=1, Value=20
  Code=ImageLength(257, 0x101), Type=Short(3), Count=1, Value=20
  Code=BitsPerSample(258, 0x102), Type=Short(3), Count=1, Value=8
  Code=Compression(259, 0x103), Type=Short(3), Count=1, Value=1
  Code=PhotometricInterpretation(262, 0x106), Type=Short(3), Count=1, Value=1
  Code=StripOffsets(273, 0x111), Type=Long(4), Count=1, Value=8
  Code=SamplesPerPixel(277, 0x115), Type=Short(3), Count=1, Value=1
  Code=RowsPerStrip(278, 0x116), Type=Short(3), Count=1, Value=20
  Code=StripByteCounts(279, 0x117), Type=Long(4), Count=1, Value=400
  Code=PlanarConfiguration(284, 0x11C), Type=Short(3), Count=1, Value=1
  Code=SampleFormat(339, 0x153), Type=Short(3), Count=1, Value=1
  Code=GeoTIFFPixelScale(33550, 0x830E), Type=Double(12), Count=3, Offset of value=594, Values=60,60,0
  Code=GeoTIFFTiePoints(33922, 0x8482), Type=Double(12), Count=6, Offset of value=618, Values=0,0,0,440720,3751320,0
  Code=GeoTIFFGeoKeyDirectory(34735, 0x87AF), Type=Short(3), Count=24, Offset of value=666, Values=1,1,0,5,1024,0,1,1,1025,0,1,1,1026,34737,21,0,3072,0,1,26711,3076,0,1,9001
  Code=GeoTIFFAsciiParams(34737, 0x87B1), Type=ASCII(2), Count=22, Offset of value=714, Value="NAD27 / UTM zone 11N|"
```
`
