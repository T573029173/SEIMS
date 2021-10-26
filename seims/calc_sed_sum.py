# coding:utf-8

from pygeoc.raster import RasterUtilClass
from pygeoc.utils import FileClass, StringClass, UtilClass, get_config_parser, is_string

MODEL_PATH = 'D:/Programs/SEIMS/data/youwuzhen/ss_youwuzhen10m_longterm_model'


def calc_sed_sum(sceid):
    seds = []
    raster_file = '%s/OUTPUT%d/SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('13-17年总产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/SED_OL_AVE.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('日平均产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/1_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('11年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/2_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('12年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/3_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('13年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/4_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('14年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/5_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('15年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/6_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('16年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/7_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('17年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/8_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('18年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/9_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('19年产沙量：%f' % (sed_sum,))

    raster_file = '%s/OUTPUT%d/10_SED_OL_SUM.tif' % (MODEL_PATH, sceid)
    rr = RasterUtilClass.read_raster(raster_file)
    sed_sum = rr.get_sum()
    seds.append(sed_sum)
    print('20年产沙量：%f' % (sed_sum,))

    return seds

if __name__ == '__main__':
    sceid = 0
    print('-----Base 情景：-----')
    seds_base = calc_sed_sum(sceid)

    # sceid = 10
    # print('-----情景 %d-----'%(sceid,))
    # seds_sce = calc_sed_sum(sceid)

    # print('-----对比-----')
    # print('总削减率：%f'%((seds_base[0]-seds_sce[0])/seds_base[0]))
    # print('日均削减率：%f'%((seds_base[1]-seds_sce[1])/seds_base[1]))
    # print('11年削减率：%f'%((seds_base[2]-seds_sce[2])/seds_base[2]))
    # print('12年削减率：%f'%((seds_base[3]-seds_sce[3])/seds_base[3]))
    # print('13年削减率：%f'%((seds_base[4]-seds_sce[4])/seds_base[4]))
    # print('14年削减率：%f'%((seds_base[5]-seds_sce[5])/seds_base[5]))
    # print('15年削减率：%f'%((seds_base[6]-seds_sce[6])/seds_base[6]))
    # sceid = 1052
    # print('-----情景 %d-----'%(sceid,))
    # seds_sce = calc_sed_sum(sceid)

    # sceid = 10513
    # print('-----情景 %d-----'%(sceid,))
    # seds_sce = calc_sed_sum(sceid)

    # sceid = 101
    # print('-----情景 %d-----'%(sceid,))
    # seds_sce = calc_sed_sum(sceid)
    # sceid = 102
    # print('-----情景 %d-----'%(sceid,))
    # seds_sce = calc_sed_sum(sceid)
    # sceid = 103
    # print('-----情景 %d-----'%(sceid,))
    # seds_sce = calc_sed_sum(sceid)
    # sceid = 104
    # print('-----情景 %d-----'%(sceid,))
    # seds_sce = calc_sed_sum(sceid)
    # sceid = 105
    # print('-----情景 %d-----'%(sceid,))
    # seds_sce = calc_sed_sum(sceid)
    # print('-----对比-----')
    # print('总削减率：%f'%((seds_base[0]-seds_sce[0])/seds_base[0]))
    # print('日均削减率：%f'%((seds_base[1]-seds_sce[1])/seds_base[1]))
    # print('11年削减率：%f'%((seds_base[2]-seds_sce[2])/seds_base[2]))
    # print('12年削减率：%f'%((seds_base[3]-seds_sce[3])/seds_base[3]))
    # print('13年削减率：%f'%((seds_base[4]-seds_sce[4])/seds_base[4]))
    # print('14年削减率：%f'%((seds_base[5]-seds_sce[5])/seds_base[5]))
    # print('15年削减率：%f'%((seds_base[6]-seds_sce[6])/seds_base[6]))
