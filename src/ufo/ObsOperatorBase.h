/*
 * (C) Copyright 2017-2018 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef UFO_OBSOPERATORBASE_H_
#define UFO_OBSOPERATORBASE_H_

#include <map>
#include <string>

#include <boost/noncopyable.hpp>

#include "eckit/config/Configuration.h"
#include "ioda/ObsSpace.h"
#include "oops/base/Variables.h"
#include "oops/util/abor1_cpp.h"
#include "oops/util/DateTime.h"
#include "oops/util/Printable.h"

namespace ioda {
class ObsVector;
}

namespace ufo {
class GeoVaLs;
class ObsBias;
class Locations;

// -----------------------------------------------------------------------------
/// Base class for observation operators

class ObsOperatorBase : public util::Printable,
                        private boost::noncopyable {
 public:
  ObsOperatorBase() {}
  virtual ~ObsOperatorBase() {}

/// Obs Operator
  virtual void simulateObs(const GeoVaLs &, ioda::ObsVector &, const ObsBias &) const = 0;

/// Operator input required from Model
  virtual const oops::Variables & variables() const = 0;

/// Operator output variables
  virtual const oops::Variables & observed() const = 0;

/// Operator locations
  virtual Locations * locateObs(const util::DateTime &, const util::DateTime &) const = 0;

 private:
  virtual void print(std::ostream &) const = 0;
};

// -----------------------------------------------------------------------------

/// Obs Operator Factory
class ObsOperatorFactory {
 public:
  static ObsOperatorBase * create(const ioda::ObsSpace &, const eckit::Configuration &);
  virtual ~ObsOperatorFactory() { getMakers().clear(); }
 protected:
  explicit ObsOperatorFactory(const std::string &);
 private:
  virtual ObsOperatorBase * make(const ioda::ObsSpace &, const eckit::Configuration &) = 0;
  static std::map < std::string, ObsOperatorFactory * > & getMakers() {
    static std::map < std::string, ObsOperatorFactory * > makers_;
    return makers_;
  }
};

// -----------------------------------------------------------------------------

template<class T>
class ObsOperatorMaker : public ObsOperatorFactory {
  virtual ObsOperatorBase * make(const ioda::ObsSpace & odb, const eckit::Configuration & conf)
    { return new T(odb, conf); }
 public:
  explicit ObsOperatorMaker(const std::string & name) : ObsOperatorFactory(name) {}
};

// -----------------------------------------------------------------------------

}  // namespace ufo

#endif  // UFO_OBSOPERATORBASE_H_
