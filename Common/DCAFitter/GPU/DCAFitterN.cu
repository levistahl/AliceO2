// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifdef __HIPCC__
#include "hip/hip_runtime.h"
#else
#include <cuda.h>
#endif

#include "GPUCommonDef.h"
#include "DCAFitter/DCAFitterN.h"

namespace o2
{
namespace vertexing
{
GPUg() void __dummy_instance__()
{
#ifdef GPUCA_GPUCODE_DEVICE
#pragma message "Compiling device code"
#endif
  DCAFitter2 ft2;
  DCAFitter3 ft3;
  o2::track::TrackParCov tr;
  ft2.process(tr, tr);
  ft3.process(tr, tr, tr);
}

} // namespace vertexing
} // namespace o2