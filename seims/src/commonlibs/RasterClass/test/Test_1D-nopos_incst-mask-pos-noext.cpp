/*!
 * @brief Test description:
 *                      CalcPositions UseMaskExtent ExtentConsistent  SingleLayer
 *        Raster data:      NO             NO            NO               YES
 *        Mask data  :      YES            --            NO               YES
 *
 *        TEST CASE NAME (or TEST SUITE): 
 *            clsRasterDataTestNoPosIncstMaskNoPosExt
 *
 *        Since we mainly support ASC and GDAL(e.g., TIFF),
 *        value-parameterized tests of Google Test will be used.
 * @cite https://github.com/google/googletest/blob/master/googletest/samples/sample7_unittest.cc
 * @version 1.0
 * @authors Liangjun Zhu (zlj@lreis.ac.cn)
 * @revised 12/05/2017 lj Initial version.
 *
 */
#include "gtest/gtest.h"
#include "utilities.h"
#include "clsRasterData.h"

using namespace std;

namespace {
#if GTEST_HAS_PARAM_TEST

using ::testing::TestWithParam;
using ::testing::Values;

string apppath = GetAppPath();
string corename = "dem_2";
string maskcorename = "mask1";
string asc_file = apppath + "../data/" + corename + ".asc";
string tif_file = apppath + "../data/" + corename + ".tif";
string mask_asc_file = apppath + "../data/" + maskcorename + ".asc";
string mask_tif_file = apppath + "../data/" + maskcorename + ".tif";

struct inputRasterFiles {
public:
    inputRasterFiles(const string &rsf, const string &maskf) {
        raster_name = rsf.c_str();
        mask_name = maskf.c_str();
    };
    const char *raster_name;
    const char *mask_name;
};

//Inside the test body, fixture constructor, SetUp(), and TearDown() you
//can refer to the test parameter by GetParam().  In this case, the test
//parameter is a factory function which we call in fixture's SetUp() to
//create and store an instance of clsRasterData<float>.
class clsRasterDataTestNoPosIncstMaskPosNoExt : public TestWithParam<inputRasterFiles *> {
public:
    clsRasterDataTestNoPosIncstMaskPosNoExt() : rs(nullptr), maskrs(nullptr) {}
    ~clsRasterDataTestNoPosIncstMaskPosNoExt() override { delete rs; }
    void SetUp() override {
        // Read mask data with default parameters, i.e., calculate valid positions.
        maskrs = clsRasterData<int>::Init(GetParam()->mask_name);
        ASSERT_NE(nullptr, maskrs);
        // Read raster data with the masked data
        rs = clsRasterData<float, int>::Init(GetParam()->raster_name, false, maskrs, false);
        ASSERT_NE(nullptr, rs);
    }
    void TearDown() override {
        delete rs;
        delete maskrs;
        rs = nullptr;
        maskrs = nullptr;
    }
protected:
    clsRasterData<float, int> *rs;
    clsRasterData<int> *maskrs;
};

// Since each TEST_P will invoke SetUp() and TearDown()
// once, we put all tests in once test case. by lj.
TEST_P(clsRasterDataTestNoPosIncstMaskPosNoExt, RasterIO) {
    /// 1. Test members after constructing.
    EXPECT_EQ(80, rs->getDataLength());  // m_nCells, which will be nRows * nCols
    EXPECT_EQ(80, rs->getCellNumber());  // m_nCells, which is the extent of mask data

    EXPECT_FLOAT_EQ(-9999.f, rs->getNoDataValue());  // m_noDataValue
    EXPECT_FLOAT_EQ(-9999.f, rs->getDefaultValue());  // m_defaultValue

    // m_filePathName depends on the path of build, so no need to test.
    EXPECT_EQ(corename, rs->getCoreName());  // m_coreFileName

    EXPECT_TRUE(rs->Initialized());  // m_initialized
    EXPECT_FALSE(rs->is2DRaster());  // m_is2DRaster
    /* When the extents of maskrs and rs are inconsistent,
     * and m_calcPositions is false, and the extent of mask data is not used,
     * the m_rasterPositionData will be reCalculated and allocated space
     * for further usage, but the m_rasterData still contains NODATA.
     */
    EXPECT_FALSE(rs->PositionsCalculated());  // m_calcPositions
    EXPECT_FALSE(rs->PositionsAllocated());  // m_storePositions
    EXPECT_FALSE(rs->MaskExtented());  // m_useMaskExtent
    EXPECT_FALSE(rs->StatisticsCalculated());  // m_statisticsCalculated

    ASSERT_TRUE(rs->validate_raster_data());

    EXPECT_NE(nullptr, rs->getRasterDataPointer());  // m_rasterData
    EXPECT_EQ(nullptr, rs->get2DRasterDataPointer());  // m_raster2DData
    EXPECT_EQ(nullptr, rs->getRasterPositionDataPointer());  // m_rasterPositionData

    /** Get metadata, m_headers **/
    EXPECT_EQ(8, rs->getRows());
    EXPECT_EQ(10, rs->getCols());
    EXPECT_FLOAT_EQ(19.f, rs->getXllCenter());
    EXPECT_FLOAT_EQ(25.f, rs->getYllCenter());
    EXPECT_FLOAT_EQ(2.f, rs->getCellWidth());
    EXPECT_EQ(1, rs->getLayers());
    EXPECT_STREQ("", rs->getSRS());
    EXPECT_EQ("", rs->getSRSString());

    /** Calc and get basic statistics, m_statsMap **/
    EXPECT_EQ(60, rs->getValidNumber());
    EXPECT_FLOAT_EQ(7.07f, rs->getMinimum());
    EXPECT_FLOAT_EQ(98.49f, rs->getMaximum());
    EXPECT_FLOAT_EQ(10.23766667f, rs->getAverage());
    EXPECT_FLOAT_EQ(11.52952953f, rs->getSTD());
    EXPECT_FLOAT_EQ(91.42f, rs->getRange());
    EXPECT_TRUE(rs->StatisticsCalculated());

    EXPECT_NE(nullptr, rs->getMask());  // m_mask

    /** Test getting raster data **/
    int ncells = 0;
    float *rs_data = nullptr;
    EXPECT_TRUE(rs->getRasterData(&ncells, &rs_data));  // m_rasterData
    EXPECT_EQ(80, ncells);
    EXPECT_NE(nullptr, rs_data);
    EXPECT_FLOAT_EQ(7.94f, rs_data[0]);
    EXPECT_FLOAT_EQ(9.85f, rs_data[79]);
    EXPECT_FLOAT_EQ(9.43f, rs_data[6]);

    float **rs_2ddata = nullptr;
    int nlyrs = -1;
    EXPECT_FALSE(rs->get2DRasterData(&ncells, &nlyrs, &rs_2ddata));  // m_raster2DData
    EXPECT_EQ(-1, ncells);
    EXPECT_EQ(-1, nlyrs);
    EXPECT_EQ(nullptr, rs_2ddata);

    /** Get raster cell value by various way **/
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(-1));
    EXPECT_FLOAT_EQ(7.94f, rs->getValueByIndex(0));
    EXPECT_FLOAT_EQ(9.85f, rs->getValueByIndex(79, 1));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(80, 1));
    EXPECT_FLOAT_EQ(9.43f, rs->getValueByIndex(6));
    EXPECT_FLOAT_EQ(9.8f, rs->getValueByIndex(8));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(29, 0));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(-1, 2));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(90, 2));

    int tmp_lyr;
    float *tmp_values;
    rs->getValueByIndex(-1, &tmp_lyr, &tmp_values);
    EXPECT_EQ(-1, tmp_lyr);
    EXPECT_EQ(nullptr, tmp_values);
    rs->getValueByIndex(1, &tmp_lyr, &tmp_values);
    EXPECT_EQ(1, tmp_lyr);
    EXPECT_NE(nullptr, tmp_values);
    EXPECT_FLOAT_EQ(7.62f, tmp_values[0]);

    EXPECT_FLOAT_EQ(-9999.f, rs->getValue(-1, 0));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValue(8, 0));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValue(0, -1));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValue(0, 10));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValue(2, 4, -1));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValue(2, 4, 2));
    EXPECT_FLOAT_EQ(9.95f, rs->getValue(2, 4));
    EXPECT_FLOAT_EQ(9.95f, rs->getValue(2, 4, 1));

    rs->getValue(-1, 0, &tmp_lyr, &tmp_values);
    EXPECT_EQ(-1, tmp_lyr);
    EXPECT_EQ(nullptr, tmp_values);
    rs->getValue(0, -1, &tmp_lyr, &tmp_values);
    EXPECT_EQ(-1, tmp_lyr);
    EXPECT_EQ(nullptr, tmp_values);
    rs->getValue(0, 0, &tmp_lyr, &tmp_values);
    EXPECT_EQ(1, tmp_lyr);
    EXPECT_NE(nullptr, tmp_values);
    EXPECT_FLOAT_EQ(7.94f, tmp_values[0]);
    rs->getValue(1, 1, &tmp_lyr, &tmp_values);
    EXPECT_EQ(1, tmp_lyr);
    EXPECT_NE(nullptr, tmp_values);
    EXPECT_FLOAT_EQ(9.54f, tmp_values[0]);

    // Get position
    EXPECT_EQ(12, rs->getPosition(22.05f, 37.95f));  // row 1, col 2
    EXPECT_EQ(12, rs->getPosition(23.95f, 36.05f));

    /** Set value **/

    // Set raster data value
    rs->setValue(1, 4, 0.877f);
    EXPECT_FLOAT_EQ(0.877f, rs->getValue(1, 4));

    // update statistics
    rs->updateStatistics();  // Should be manually invoked in your project!
    EXPECT_FLOAT_EQ(0.877f, rs->getMinimum());
    EXPECT_FLOAT_EQ(10.10611667f, rs->getAverage());
    EXPECT_FLOAT_EQ(11.59039314f, rs->getSTD());
    EXPECT_FLOAT_EQ(97.613f, rs->getRange());

    // Set core file name
    string newcorename = corename + "_1D-nopos_incst-mask-pos-noext";
    rs->setCoreName(newcorename);
    EXPECT_EQ(newcorename, rs->getCoreName());

    /** Output to new file **/
    string oldfullname = rs->getFilePath();
    string fakefullname = GetPathFromFullName(oldfullname) + "noExistDir" + SEP +
        "noOut" + "." + GetSuffix(oldfullname);
    EXPECT_FALSE(rs->outputToFile(fakefullname));
    string newfullname = GetPathFromFullName(oldfullname) + "result" + SEP +
        newcorename + "." + GetSuffix(oldfullname);
    EXPECT_TRUE(rs->outputToFile(newfullname));
    EXPECT_TRUE(FileExists(newfullname));

    /* Get position data, which will be calculated if not existed */
    ncells = -1;
    int **positions = nullptr;
    EXPECT_TRUE(maskrs->PositionsCalculated());
    rs->getRasterPositionData(&ncells, &positions);  // m_rasterPositionData
    EXPECT_TRUE(rs->PositionsCalculated());
    EXPECT_TRUE(rs->PositionsAllocated());
    EXPECT_NE(nullptr, rs->getRasterPositionDataPointer());
    EXPECT_EQ(60, ncells);
    EXPECT_NE(nullptr, positions);
    // index = 0, row = 0 and col = 0
    EXPECT_EQ(0, positions[0][0]);
    EXPECT_EQ(0, positions[0][1]);
    // index = 59, row = 7 and col = 9
    EXPECT_EQ(7, positions[59][0]);
    EXPECT_EQ(9, positions[59][1]);

    // In the meantime, other related variables are updated, test some of them.
    EXPECT_EQ(60, rs->getCellNumber());  // m_nCells
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(-1));
    EXPECT_FLOAT_EQ(7.94f, rs->getValueByIndex(0));
    EXPECT_FLOAT_EQ(9.85f, rs->getValueByIndex(59, 1));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(60, 1));
    EXPECT_FLOAT_EQ(9.64f, rs->getValueByIndex(15));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(15, 0));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(-1, 2));
    EXPECT_FLOAT_EQ(-9999.f, rs->getValueByIndex(541, 2));
}

INSTANTIATE_TEST_CASE_P(SingleLayer, clsRasterDataTestNoPosIncstMaskPosNoExt,
                        Values(new inputRasterFiles(asc_file, mask_asc_file),
                               new inputRasterFiles(tif_file, mask_tif_file)));
#else
TEST(DummyTest, ValueParameterizedTestsAreNotSupportedOnThisPlatform) {}

#endif /* GTEST_HAS_PARAM_TEST */
} /* namespace */
