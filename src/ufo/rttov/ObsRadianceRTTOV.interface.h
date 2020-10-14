/*
 * (C) Copyright 2017 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef UFO_RTTOV_OBSRADIANCERTTOV_INTERFACE_H_
#define UFO_RTTOV_OBSRADIANCERTTOV_INTERFACE_H_

#include "ioda/ObsSpace.h"
#include "oops/base/Variables.h"
#include "ufo/Fortran.h"

namespace ufo {

/// Interface to Fortran UFO routines
/*!
 * The core of the UFO is coded in Fortran.
 * Here we define the interfaces to the Fortran code.
 */

extern "C" {

// -----------------------------------------------------------------------------
//  Radiance observation operator
// -----------------------------------------------------------------------------
  void ufo_radiancerttov_setup_f90(F90hop &, const eckit::Configuration &,
                                  const int &, const int &,
                                  oops::Variables &);
  void ufo_radiancerttov_delete_f90(F90hop &);
  void ufo_radiancerttov_simobs_f90(const F90hop &, const F90goms &, const ioda::ObsSpace &,
                               const int &, const int &, double &, const F90goms &);
// -----------------------------------------------------------------------------

}  // extern C

}  // namespace ufo
#endif  // UFO_RTTOV_OBSRADIANCERTTOV_INTERFACE_H_
