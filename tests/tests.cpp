// SPDX-License-Identifier: MIT
// Copyright 2024, Even Rouault <even.rouault at spatialys.com>

#define LIBERTIFF_C_FILE_READER
#include "libertiff.hpp"

#include "gtest_include.h"

// So argc, argv can be used from test fixtures
int global_argc = 0;
char **global_argv = nullptr;

GTEST_API_ int main(int argc, char **argv)
{
    int nRetCode;
    try
    {
        testing::InitGoogleTest(&argc, argv);

        global_argc = argc;
        global_argv = argv;

        nRetCode = RUN_ALL_TESTS();
    }
    catch (const std::exception &e)
    {
        nRetCode = 1;
        fprintf(stderr, "Caught exception %s\n", e.what());
    }
    catch (...)
    {
        nRetCode = 1;
        fprintf(stderr, "Caught exception of unknown type\n");
    }

    return nRetCode;
}

namespace
{
// Common fixture with test data
struct test : public ::testing::Test
{
};

TEST_F(test, le_strip_single_band)
{
    FILE *f = fopen("data/le_strip_single_band.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    EXPECT_FALSE(tiff->isBigTIFF());
    tiff->mustByteSwap();
    EXPECT_EQ(tiff->nextImageOffset(), 0);
    EXPECT_EQ(tiff->next(), nullptr);
    EXPECT_EQ(tiff->width(), 1);
    EXPECT_EQ(tiff->height(), 2);
    EXPECT_EQ(tiff->samplesPerPixel(), 1);
    EXPECT_EQ(tiff->bitsPerSample(), 8);
    EXPECT_EQ(tiff->nextImageOffset(), 0);
    EXPECT_EQ(tiff->planarConfiguration(),
              libertiff::PlanarConfiguration::Contiguous);
    EXPECT_EQ(tiff->compression(), libertiff::Compression::None);
    EXPECT_EQ(tiff->photometricInterpretation(),
              libertiff::PhotometricInterpretation::MinIsBlack);
    EXPECT_EQ(tiff->predictor(), 0);
    EXPECT_EQ(tiff->sampleFormat(), libertiff::SampleFormat::UnsignedInt);
    EXPECT_EQ(tiff->rowsPerStrip(), 2);
    EXPECT_EQ(tiff->strileCount(), 1);
    EXPECT_FALSE(tiff->isTiled());
    EXPECT_EQ(tiff->tileWidth(), 0);
    EXPECT_EQ(tiff->tileHeight(), 0);
    EXPECT_EQ(tiff->tilesPerRow(), 0);
    EXPECT_EQ(tiff->tilesPerCol(), 0);
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileCoordinateToIdx(0, 0, 0, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(0, ok), 146);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(1, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileByteCount(0, ok), 2);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileByteCount(1, ok), 0);
        EXPECT_FALSE(ok);
    }
    EXPECT_EQ(tiff->tags().size(), 11);
    {
        bool ok = true;
        const auto *tag = tiff->tag(libertiff::TagCode::StripOffsets);
        ASSERT_NE(tag, nullptr);
        const auto v = std::vector<uint32_t>{146};
        EXPECT_EQ(tiff->readTagAsVector<uint32_t>(*tag, ok), v);
    }
}

TEST_F(test, be_strip_single_band)
{
    FILE *f = fopen("data/be_strip_single_band.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    EXPECT_FALSE(tiff->isBigTIFF());
    tiff->mustByteSwap();
    EXPECT_EQ(tiff->nextImageOffset(), 0);
    EXPECT_EQ(tiff->next(), nullptr);
    EXPECT_EQ(tiff->width(), 1);
    EXPECT_EQ(tiff->height(), 2);
    EXPECT_EQ(tiff->samplesPerPixel(), 1);
    EXPECT_EQ(tiff->bitsPerSample(), 8);
    EXPECT_EQ(tiff->nextImageOffset(), 0);
    EXPECT_EQ(tiff->planarConfiguration(),
              libertiff::PlanarConfiguration::Contiguous);
    EXPECT_EQ(tiff->compression(), libertiff::Compression::None);
    EXPECT_EQ(tiff->photometricInterpretation(),
              libertiff::PhotometricInterpretation::MinIsBlack);
    EXPECT_EQ(tiff->predictor(), 0);
    EXPECT_EQ(tiff->sampleFormat(), libertiff::SampleFormat::UnsignedInt);
    EXPECT_EQ(tiff->rowsPerStrip(), 2);
    EXPECT_EQ(tiff->strileCount(), 1);
    EXPECT_FALSE(tiff->isTiled());
    EXPECT_EQ(tiff->tileWidth(), 0);
    EXPECT_EQ(tiff->tileHeight(), 0);
    EXPECT_EQ(tiff->tilesPerRow(), 0);
    EXPECT_EQ(tiff->tilesPerCol(), 0);
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileCoordinateToIdx(0, 0, 0, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(0, ok), 146);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(1, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileByteCount(0, ok), 2);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileByteCount(1, ok), 0);
        EXPECT_FALSE(ok);
    }
    EXPECT_EQ(tiff->tags().size(), 11);
}

TEST_F(test, le_bigtiff_strip_single_band)
{
    FILE *f = fopen("data/le_bigtiff_strip_single_band.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    EXPECT_TRUE(tiff->isBigTIFF());
    tiff->mustByteSwap();
    EXPECT_EQ(tiff->nextImageOffset(), 0);
    EXPECT_EQ(tiff->next(), nullptr);
    EXPECT_EQ(tiff->width(), 1);
    EXPECT_EQ(tiff->height(), 2);
    EXPECT_EQ(tiff->samplesPerPixel(), 1);
    EXPECT_EQ(tiff->bitsPerSample(), 8);
    EXPECT_EQ(tiff->nextImageOffset(), 0);
    EXPECT_EQ(tiff->planarConfiguration(),
              libertiff::PlanarConfiguration::Contiguous);
    EXPECT_EQ(tiff->compression(), libertiff::Compression::None);
    EXPECT_EQ(tiff->photometricInterpretation(),
              libertiff::PhotometricInterpretation::MinIsBlack);
    EXPECT_EQ(tiff->predictor(), 0);
    EXPECT_EQ(tiff->sampleFormat(), libertiff::SampleFormat::UnsignedInt);
    EXPECT_EQ(tiff->rowsPerStrip(), 2);
    EXPECT_EQ(tiff->strileCount(), 1);
    EXPECT_FALSE(tiff->isTiled());
    EXPECT_EQ(tiff->tileWidth(), 0);
    EXPECT_EQ(tiff->tileHeight(), 0);
    EXPECT_EQ(tiff->tilesPerRow(), 0);
    EXPECT_EQ(tiff->tilesPerCol(), 0);
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileCoordinateToIdx(0, 0, 0, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(0, ok), 252);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(1, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileByteCount(0, ok), 2);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileByteCount(1, ok), 0);
        EXPECT_FALSE(ok);
    }
    EXPECT_EQ(tiff->tags().size(), 11);
}

TEST_F(test, be_bigtiff_strip_single_band)
{
    FILE *f = fopen("data/be_bigtiff_strip_single_band.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    EXPECT_TRUE(tiff->isBigTIFF());
    tiff->mustByteSwap();
    EXPECT_EQ(tiff->nextImageOffset(), 0);
    EXPECT_EQ(tiff->next(), nullptr);
    EXPECT_EQ(tiff->width(), 1);
    EXPECT_EQ(tiff->height(), 2);
    EXPECT_EQ(tiff->samplesPerPixel(), 1);
    EXPECT_EQ(tiff->bitsPerSample(), 8);
    EXPECT_EQ(tiff->nextImageOffset(), 0);
    EXPECT_EQ(tiff->planarConfiguration(),
              libertiff::PlanarConfiguration::Contiguous);
    EXPECT_EQ(tiff->compression(), libertiff::Compression::None);
    EXPECT_EQ(tiff->photometricInterpretation(),
              libertiff::PhotometricInterpretation::MinIsBlack);
    EXPECT_EQ(tiff->predictor(), 0);
    EXPECT_EQ(tiff->sampleFormat(), libertiff::SampleFormat::UnsignedInt);
    EXPECT_EQ(tiff->rowsPerStrip(), 2);
    EXPECT_EQ(tiff->strileCount(), 1);
    EXPECT_FALSE(tiff->isTiled());
    EXPECT_EQ(tiff->tileWidth(), 0);
    EXPECT_EQ(tiff->tileHeight(), 0);
    EXPECT_EQ(tiff->tilesPerRow(), 0);
    EXPECT_EQ(tiff->tilesPerCol(), 0);
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileCoordinateToIdx(0, 0, 0, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(0, ok), 252);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(1, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileByteCount(0, ok), 2);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileByteCount(1, ok), 0);
        EXPECT_FALSE(ok);
    }
    EXPECT_EQ(tiff->tags().size(), 11);
}

TEST_F(test, two_ifds)
{
    FILE *f = fopen("data/two_ifds.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    {
        bool ok = true;
        EXPECT_EQ(tiff->strileOffset(0, ok), 146);
        EXPECT_TRUE(ok);
    }
    EXPECT_EQ(tiff->nextImageOffset(), 148);
    auto next = tiff->next();
    ASSERT_NE(next, nullptr);
    {
        bool ok = true;
        EXPECT_EQ(next->strileOffset(0, ok), 286);
        EXPECT_TRUE(ok);
    }
    EXPECT_EQ(next->nextImageOffset(), 0);
    EXPECT_EQ(next->next(), nullptr);
}

TEST_F(test, tiled)
{
    FILE *f = fopen("data/tiled.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    EXPECT_EQ(tiff->width(), 33);
    EXPECT_EQ(tiff->height(), 46);
    EXPECT_TRUE(tiff->isTiled());
    EXPECT_EQ(tiff->tileWidth(), 32);
    EXPECT_EQ(tiff->tileHeight(), 16);
    EXPECT_EQ(tiff->tilesPerRow(), 2);
    EXPECT_EQ(tiff->tilesPerCol(), 3);
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileOffset(0, 0, 0, ok), 194);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileOffset(1, 0, 0, ok), 706);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        // invalid xtile
        EXPECT_EQ(tiff->tileOffset(2, 0, 0, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileOffset(0, 1, 0, ok), 1218);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileOffset(1, 1, 0, ok), 1730);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileOffset(0, 2, 0, ok), 2242);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        EXPECT_EQ(tiff->tileOffset(1, 2, 0, ok), 2754);
        EXPECT_TRUE(ok);
    }
    {
        bool ok = true;
        // invalid ytile
        EXPECT_EQ(tiff->tileOffset(0, 3, 0, ok), 0);
        EXPECT_FALSE(ok);
    }
    {
        bool ok = true;
        const auto *tag = tiff->tag(libertiff::TagCode::TileOffsets);
        ASSERT_NE(tag, nullptr);
        const auto v = std::vector<uint32_t>{194, 706, 1218, 1730, 2242, 2754};
        EXPECT_EQ(tiff->readTagAsVector<uint32_t>(*tag, ok), v);
    }
    {
        bool ok = true;
        const auto *tag = tiff->tag(libertiff::TagCode::TileByteCounts);
        ASSERT_NE(tag, nullptr);
        const auto v = std::vector<uint16_t>{512, 512, 512, 512, 512, 512};
        EXPECT_EQ(tiff->readTagAsVector<uint16_t>(*tag, ok), v);
    }
}

TEST_F(test, le_strip_three_band_contig)
{
    FILE *f = fopen("data/le_strip_three_band_contig.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    EXPECT_EQ(tiff->samplesPerPixel(), 3);
    EXPECT_EQ(tiff->planarConfiguration(),
              libertiff::PlanarConfiguration::Contiguous);
}

TEST_F(test, le_strip_three_band_separate)
{
    FILE *f = fopen("data/le_strip_three_band_separate.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    EXPECT_EQ(tiff->samplesPerPixel(), 3);
    EXPECT_EQ(tiff->planarConfiguration(),
              libertiff::PlanarConfiguration::Separate);
}

TEST_F(test, self_cycling)
{
    FILE *f = fopen("data/self_cycling.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    EXPECT_EQ(tiff->nextImageOffset(), 8);
    EXPECT_EQ(tiff->next(), nullptr);
}

TEST_F(test, image_description)
{
    FILE *f = fopen("data/image_description.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    const auto *tag = tiff->tag(libertiff::TagCode::ImageDescription);
    ASSERT_NE(tag, nullptr);
    ASSERT_EQ(tag->type, libertiff::TagType::ASCII);
    bool ok = true;
    const auto str = tiff->readTagAsString(*tag, ok);
    EXPECT_EQ(str.size(), tag->count - 1);
    EXPECT_STREQ(str.c_str(), "image_description");
}

TEST_F(test, image_description_one_char)
{
    FILE *f = fopen("data/image_description_one_char.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    const auto *tag = tiff->tag(libertiff::TagCode::ImageDescription);
    ASSERT_NE(tag, nullptr);
    ASSERT_EQ(tag->type, libertiff::TagType::ASCII);
    bool ok = true;
    const auto str = tiff->readTagAsString(*tag, ok);
    EXPECT_EQ(str.size(), tag->count - 1);
    EXPECT_STREQ(str.c_str(), "a");
}

TEST_F(test, geotiff)
{
    FILE *f = fopen("data/geotiff.tif", "rb");
    ASSERT_NE(f, nullptr);
    auto tiff = libertiff::open(std::make_shared<libertiff::CFileReader>(f));
    ASSERT_NE(tiff, nullptr);
    {
        const auto *tag = tiff->tag(libertiff::TagCode::GeoTIFFAsciiParams);
        ASSERT_NE(tag, nullptr);
        ASSERT_EQ(tag->type, libertiff::TagType::ASCII);
        bool ok = true;
        const auto str = tiff->readTagAsString(*tag, ok);
        EXPECT_EQ(str.size(), tag->count - 1);
        EXPECT_STREQ(str.c_str(), "NAD27 / UTM zone 11N|");
    }
    {
        bool ok = true;
        const auto *tag = tiff->tag(libertiff::TagCode::GeoTIFFPixelScale);
        ASSERT_NE(tag, nullptr);
        const auto v = std::vector<double>{60, 60, 0};
        EXPECT_EQ(tiff->readTagAsVector<double>(*tag, ok), v);
    }
    {
        bool ok = true;
        const auto *tag = tiff->tag(libertiff::TagCode::GeoTIFFTiePoints);
        ASSERT_NE(tag, nullptr);
        const auto v = std::vector<double>{0, 0, 0, 440720, 3751320, 0};
        EXPECT_EQ(tiff->readTagAsVector<double>(*tag, ok), v);
    }
    {
        bool ok = true;
        const auto *tag = tiff->tag(libertiff::TagCode::GeoTIFFGeoKeyDirectory);
        ASSERT_NE(tag, nullptr);
        const auto v = std::vector<uint16_t>{
            1,    1,     0,  5, 1024, 0, 1, 1,     1025, 0, 1, 1,
            1026, 34737, 21, 0, 3072, 0, 1, 26711, 3076, 0, 1, 9001};
        EXPECT_EQ(tiff->readTagAsVector<uint16_t>(*tag, ok), v);
    }
}

}  // namespace
