#include "TCOutput.h"
#include "TCDebugNew.h"

#ifdef HAVE_UNIT_TESTS
#include "TCBasicTestSuite.h"
#include "TCMTThreadTestSuite.h"
#include "TCNetTestSuite.h"
#include "TCMathTestSuite.h"
#include "TCUnitTreeTestRunner.h"

int RunUnitTests()
{
   tc::unit::TestSuite::Ptr suite(new tc::unit::TestSuite("tc::Suite"));
   suite->AddTest(tc::unit::Test::Ptr(new tc::tests::BasicSuite));
   suite->AddTest(tc::unit::Test::Ptr(new tc::multi_threading::tests::Suite));
   suite->AddTest(tc::unit::Test::Ptr(new tc::net::tests::Suite));
   suite->AddTest(tc::unit::Test::Ptr(new tc::math::tests::Suite));

   tc::unit::TreeTestRunner runner;
   return runner.Run(suite, tc::unit::CleanlinessCheck::Ptr())? 0: 1;
}
#endif

class NullPrintTarget: public tc::output::PrintTarget
{
public:
    virtual void Print(const char*)
    {
    }
};

int main(int, char**)
{
   int status = 0;
   {
      tc::output::PrintTargetPtr print_target(new NullPrintTarget);
      tc::output::SetOuputTarget(print_target);

#ifdef HAVE_UNIT_TESTS
      status = RunUnitTests();
#else
      std::printf("ERROR: Compiled without unit tests enabled\n");
      status = 1;
#endif

      tc::output::SetOuputTarget(tc::output::PrintTargetPtr());
   }

   return status;
}

