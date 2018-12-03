/*
 * (C) Copyright 2017-2018 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "ufo/atmosphere/gnssro/RefGSI/ObsGnssroRef.h"

#include <ostream>
#include <string>
#include <vector>

#include "ioda/ObsVector.h"

#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"
#include "oops/util/Logger.h"

#include "ufo/GeoVaLs.h"
#include "ufo/Locations.h"
#include "ufo/ObsBias.h"

namespace ufo {

// -----------------------------------------------------------------------------
static ObsOperatorMaker<ObsGnssroRef> makerGnssroRef_("GnssroRef");
// -----------------------------------------------------------------------------

ObsGnssroRef::ObsGnssroRef(const ioda::ObsSpace & odb, const eckit::Configuration & config)
  : keyOperGnssroRef_(0), odb_(odb), varin_(), varout_()
{
  const std::vector<std::string> vv{"temperature", "specific_humidity", "air_pressure",
                                    "geopotential_height"};
  varin_.reset(new oops::Variables(vv));

  const std::vector<std::string> vout{"zz"};
  varout_.reset(new oops::Variables(vout));

  const eckit::Configuration * configc = &config;
  ufo_gnssro_ref_setup_f90(keyOperGnssroRef_, &configc);
  oops::Log::trace() << "ObsGnssroRef created." << std::endl;
}

// -----------------------------------------------------------------------------

ObsGnssroRef::~ObsGnssroRef() {
  ufo_gnssro_ref_delete_f90(keyOperGnssroRef_);
  oops::Log::trace() << "ObsGnssroRef destructed" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGnssroRef::simulateObs(const GeoVaLs & gom, ioda::ObsVector & ovec,
                                const ObsBias & bias) const {
  ufo_gnssro_ref_simobs_f90(keyOperGnssroRef_, gom.toFortran(), odb_,
                            ovec.size(), ovec.toFortran(), bias.toFortran());
}

// -----------------------------------------------------------------------------

Locations * ObsGnssroRef::locateObs(const util::DateTime & t1,
                                    const util::DateTime & t2) const {
  const util::DateTime * p1 = &t1;
  const util::DateTime * p2 = &t2;
  int keylocs;
  ufo_gnssro_ref_locateobs_f90(keyOperGnssroRef_, odb_, &p1, &p2, keylocs);

  return new Locations(keylocs);
}

// -----------------------------------------------------------------------------

void ObsGnssroRef::print(std::ostream & os) const {
  os << "ObsGnssroRef::print not implemented";
}

// -----------------------------------------------------------------------------

}  // namespace ufo
