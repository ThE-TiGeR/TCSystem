// CONFIX:EXENAME('TCUnitTest')
#define TC_USE_MEM_CHECK 0

#include "TCOutput.h"
#include "TCNewDisable.h"

#ifdef HAVE_UNIT_TESTS
#include "TCBasicTestSuite.h"
#include "TCMTThreadTestSuite.h"
#include "TCNetTestSuite.h"

#include <jf/unittest/tree_test_runner.h>
#include <jf/unittest/tests/stage2_suite.h>

int RunUnitTests()
{
   Unit::TestSuite suite("TC::Suite");
   suite.add_test(new jf::unittest::tests::Stage2Suite);
   suite.add_test(new TC::Tests::BasicSuite);
   suite.add_test(new TC::MT::Tests::Suite);
   suite.add_test(new TC::Net::Tests::Suite);

   jf::unittest::TreeTestRunner runner;
   return runner.run(&suite)? 0: 1;
}
#endif

class NullPrintTarget: public TC::Output::PrintTarget
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
      TC::Output::PrintTargetPtr print_target(new NullPrintTarget);
      TC::Output::SetOuputTarget(print_target);

#ifdef HAVE_UNIT_TESTS
      status = RunUnitTests();
#else
      std::printf("ERROR: Compiled without unit tests enabled\n");
      status = 1;
#endif

      TC::Output::SetOuputTarget(TC::Output::PrintTargetPtr());
   }

   return status;
}

