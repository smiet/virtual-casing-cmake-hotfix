#include <virtual-casing.hpp>
#include <virtual-casing-test-data.hpp>

template <class Real> void test(long Nt, long Np, int digits, biest::SurfType surf_type) {
  // Construct the surface
  sctl::Vector<Real> X(3*Nt*Np);
  GenerateSurfaceCoordinates(X, Nt, Np, surf_type);
  //for (long t = 0; t < Nt; t++) { // toroidal direction
  //  for (long p = 0; p < Np; p++) { // poloidal direction
  //    Real x = (2 + 0.5*cos(2*M_PI*p/Np)) * cos(2*M_PI*t/Nt);
  //    Real y = (2 + 0.5*cos(2*M_PI*p/Np)) * sin(2*M_PI*t/Nt);
  //    Real z = 0.5*sin(2*M_PI*p/Np);
  //    X[0*Nt*Np+t*Np+p] = x;
  //    X[1*Nt*Np+t*Np+p] = y;
  //    X[2*Nt*Np+t*Np+p] = z;
  //  }
  //}

  // Generate B fields for testing virtual-casing principle
  sctl::Vector<Real> Bext, Bint, B;
  GenerateVirtualCasingTestData<Real>(Bext, Bint, Nt, Np, X);
  B = Bext + Bint;

  // Setup
  VirtualCasing<Real> virtual_casing;
  virtual_casing.SetSurface(Nt, Np, X);
  virtual_casing.SetAccuracy(digits);

  // Compute Bext field
  sctl::Vector<Real> Bext_;
  virtual_casing.ComputeBext(Bext_, B);

  // print error
  auto Berr = Bext - Bext_;
  Real max_err = 0, max_val = 0;
  for (const auto& x:B   ) max_val = std::max<Real>(max_val,fabs(x));
  for (const auto& x:Berr) max_err = std::max<Real>(max_err,fabs(x));
  std::cout<<"Maximum relative error: "<<max_err/max_val<<'\n';
}

int main() {
  for (long digits = 2; digits < 12; digits++) {
    //test<double>(40*digits, 10*digits, digits, biest::SurfType::AxisymNarrow);
    test<double>(500*digits, 42*digits, digits, biest::SurfType::W7X_);
  }
  return 0;
}

