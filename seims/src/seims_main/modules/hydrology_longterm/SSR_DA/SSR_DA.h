/*!
 * \file SSR_DA.h
 * \brief Subsurface runoff using Darcy's law and the kinematic approximation
 *        Water is routed cell-to-cell according to D8 flow direction
 *
 * Changelog:
 *   - 1. 2016-7-24 - lj - Add support of multi soil layers of each cells.
 *   - 2. 2017-8-23 - lj - Solve inconsistent results when using openmp to reducing
 *                           raster data according to subbasin ID.
 *   - 3. 2021-04-07 - lj - Support different flow direction algorithms
 *
 * \author Zhiqiang Yu, Junzhi Liu, Liangjun Zhu
 */
#ifndef SEIMS_MODULE_SSR_DA_H
#define SEIMS_MODULE_SSR_DA_H

#include "SimulationModule.h"

/*!
 * \defgroup SSR_DA
 * \ingroup Hydrology_longterm
 * \brief Subsurface runoff using Darcy's law and the kinematic approximation
 *
 */

/*!
 * \class SSR_DA
 * \ingroup SSR_DA
 *
 */
class SSR_DA : public SimulationModule {
public:
    SSR_DA();

    ~SSR_DA();

    void SetValue(const char *key, float value) OVERRIDE;

    void Set1DData(const char *key, int nrows, float *data) OVERRIDE;

    void Set2DData(const char *key, int nrows, int ncols, float **data) OVERRIDE;

    bool CheckInputData() OVERRIDE;

    void InitialOutputs() OVERRIDE;

    int Execute() OVERRIDE;

    void Get1DData(const char *key, int *n, float **data) OVERRIDE;

    void Get2DData(const char *key, int *nrows, int *ncols, float ***data) OVERRIDE;

private:
    bool FlowInSoil(int id);
private:
    /// current subbasin ID, 0 for the entire watershed
    int m_inputSubbsnID;
    /// valid cell numbers
    int m_nCells;
    /// width of cell (m)
    float m_CellWth;
    /// max number of soil layers
    int m_maxSoilLyrs;
    /// number of soil layers of each cell
    float *m_nSoilLyrs;
    /// soil thickness
    float **m_soilThk;

    ///// depth of the up soil layer
    //float m_upSoilDepth;
    ///
    //float *m_rootDepth;

    /// timestep
    int m_dt;
    /// Interflow scale factor
    float m_ki;
    /// soil freezing temperature threshold, deg C
    float m_soilFrozenTemp;
    /// slope (tan)
    float *m_slope;
    /// conductivity
    float **m_ks;
    ///// porosity (mm/mm)
    //float **m_porosity;

    /// amount of water held in the soil layer at saturation (sat - wp water), mm
    float **m_soilSat;
    /// pore size distribution index
    float **m_poreIdx;

    /// amount of water available to plants in soil layer at field capacity (AWC=FC-WP), mm
    float **m_soilFC;
    /// water content of soil at -1.5 MPa (wilting point) mm H2O
    float **m_soilWP;
    /// soil water storage (mm)
    float **m_soilWtrSto;
    /// soil water storage in soil profile (mm)
    float *m_soilWtrStoPrfl;
    /// soil temperature, deg C
    float *m_soilTemp;

    /// channel width, m
    float *m_chWidth;
    /// stream link
    float *m_rchID;

    /*!
     * \brief 2d array recording indexes of flow in cells
     *
     *	The first element in each sub-array is the number of flow in cells in this sub-array
     */
    float **m_flowInIdx;

    /*!
     * \brief Flow fractions of flow in cells to the current cell
     *
     * It has the same data structure as m_flowInIndex.
     */
    float **m_flowInFrac;

    /*!
     * \brief Routing layers according to the flow direction
     *
     *  There are not flow relationships within each layer.
     *  The first element in each layer is the number of cells in the layer
     */
    float **m_rteLyrs;
    /// number of routing layers
    int m_nRteLyrs;
    /// number of subbasin
    int m_nSubbsns;
    /// subbasin grid (ID of subbasin)
    float *m_subbsnID;

    // outputs

    /// subsurface runoff (mm), VAR_SSRU[0]
    float **m_subSurfRf;
    /// subsurface runoff volume (m3), VAR_SSRUVOL[0]
    float **m_subSurfRfVol;
    /// subsurface to streams from each subbasin, the first element is the whole watershed, m3/s, VAR_SBIF[0]
    float *m_ifluQ2Rch;
};

#endif /* SEIMS_MODULE_SSR_DA_H */
