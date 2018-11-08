#! /usr/bin/env python
# -*- coding: utf-8 -*-
"""Construct information of hydrologically connected fields.

    @author   : Liangjun Zhu

    @changelog:
    - 18-11-06  lj - initial implementation.
"""
from __future__ import absolute_import, unicode_literals, division

import json
import os
import sys
from io import open
from future.utils import viewitems

if os.path.abspath(os.path.join(sys.path[0], '..')) not in sys.path:
    sys.path.insert(0, os.path.abspath(os.path.join(sys.path[0], '..')))

from typing import List, Dict, Union, AnyStr
from pygeoc.raster import RasterUtilClass
from pygeoc.utils import FileClass

from utility.io_plain_text import read_data_items_from_txt


def read_field_relationships(fieldtxt):
    # type: (AnyStr) -> Dict[AnyStr, Dict[Union[int, AnyStr], Dict[AnyStr, Union[int, float, List[Union[int,float]], AnyStr, Dict[int, float]]]]]
    """Read the up-downstream relationships of fields generated by `fieldpartition` program."""
    lines = read_data_items_from_txt(fieldtxt)
    if len(lines) < 4:
        raise ValueError('The %s MUST contains at least 4 lines!' % fieldtxt)
    fields_info = dict()
    fields_info.setdefault('overview', dict())
    fields_info['overview'].setdefault('all_units', int(lines[1][0]))
    fields_info.setdefault('units', dict())
    count = 0
    for i in range(3, len(lines)):
        if len(lines[i]) < 5:
            continue
        fid = int(lines[i][0])
        downid = int(lines[i][1])
        area = float(lines[i][2]) / 100.  # ha -> km2
        landuse = int(lines[i][3])  # primary landuse ID
        degree = float(lines[i][4])  # slope degree
        if fid not in fields_info['units']:
            fields_info['units'][fid] = {'upslope': [-1],  # may has multiple upslopes
                                         'downslope': downid,  # downslope is unique
                                         'area': area,
                                         'primarylanduse': landuse,
                                         'landuse': dict(),
                                         'degree': degree}
        count += 1
    if count != fields_info['overview']['all_units']:
        raise ValueError('The fields number recorded in line 2 %d is not consistent with the '
                         'derived number %d' % (fields_info['overview']['all_units'], count))
    # loop to complete the upslope values
    for k, v in viewitems(fields_info['units']):
        curdownid = v['downslope']
        if curdownid <= 0:
            v['downslope'] = -1
            continue
        if curdownid not in fields_info['units']:
            raise ValueError('The downslope ID %d of field %d is not existed!' % (curdownid, k))
        if -1 in fields_info['units'][curdownid]['upslope']:
            fields_info['units'][curdownid]['upslope'].remove(-1)
        if k not in fields_info['units'][curdownid]['upslope']:
            fields_info['units'][curdownid]['upslope'].append(k)

    return fields_info


def main(landusef, fieldf, fieldtxt, jsonout):
    """Construct hydrologically connected fields units data in JSON file format."""
    # Check the file existence
    FileClass.check_file_exists(landusef)
    FileClass.check_file_exists(fieldf)
    FileClass.check_file_exists(fieldtxt)
    # read raster data and check the extent based on landuse.
    landuser = RasterUtilClass.read_raster(landusef)
    data_landuse = landuser.data
    nrows = landuser.nRows
    ncols = landuser.nCols
    dx = landuser.dx
    nodata_landuse = landuser.noDataValue

    fieldr = RasterUtilClass.read_raster(fieldf)
    if fieldr.nRows != nrows or fieldr.nCols != ncols:
        raise ValueError('The connected_fields raster MUST have the same dimensions'
                         ' with landuse!')
    data_fields = fieldr.data
    nodata_fields = fieldr.noDataValue

    # Read the initial relationships between fields
    fields_info = read_field_relationships(fields_txt)

    # add landuse types and areas
    for m in range(nrows):
        for n in range(ncols):
            cur_lu = int(data_landuse[m][n])
            cur_fld = int(data_fields[m][n])
            if cur_fld == nodata_fields or cur_lu == nodata_landuse or cur_lu <= 0:
                continue
            if cur_fld not in fields_info['units']:
                raise ValueError('%d is not recorded in field relationship text!' % cur_fld)
            if cur_lu not in fields_info['units'][cur_fld]['landuse']:
                fields_info['units'][cur_fld]['landuse'][cur_lu] = 1
            else:
                fields_info['units'][cur_fld]['landuse'][cur_lu] += 1
    for k, v in viewitems(fields_info['units']):
        area_field = 0.
        area_max = 0.
        area_max_lu = 0
        for luid, luarea in viewitems(v['landuse']):
            v['landuse'][luid] = luarea * dx * dx * 1.e-6
            area_field += v['landuse'][luid]
            if v['landuse'][luid] > area_max:
                area_max = v['landuse'][luid]
                area_max_lu = luid
        v['area'] = area_field
        if v['primarylanduse'] != area_max_lu:
            print(k, v['primarylanduse'], area_max_lu)
            v['primarylanduse'] = area_max_lu

    # save to json
    json_updown_data = json.dumps(fields_info, indent=4)
    with open(jsonout, 'w', encoding='utf-8') as f:
        f.write('%s' % json_updown_data)


if __name__ == '__main__':
    landuse_file = r'C:\z_data_m\SEIMS2018\youwuzhen_10m\spatial_raster\landuse.tif'
    ws = r'D:\data_m\youwuzhen\seims_models_phd\data_prepare\spatial\spatial_units'
    fields_raster = ws + os.sep + 'hydrologically_connected_fields.tif'
    fields_txt = ws + os.sep + 'fields_70.txt'
    json_out = ws + os.sep + 'connected_field_units_updown.json'
    main(landuse_file, fields_raster, fields_txt, json_out)
