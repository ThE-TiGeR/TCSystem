#ifndef __SHARED_PTR_TEST_H__
#define __SHARED_PTR_TEST_H__

#include "cpptest.h"

#define UNI_OLD_AUTO_PTR
#define UNI_SHARED_PTR_TEST

namespace SharedPtrTest
{
    extern class Suite* m_suite;

    class Suite : public Test::Suite
    {
    public:
        Suite()
        {
            SharedPtrTest::m_suite = this;
            TEST_ADD(Suite::Test);
        }

        void Assertment(Test::Source s)
        {
            assertment(s);
        }
        bool ContinueAfterFailure()
        {
            return continue_after_failure();
        }

    protected:
        virtual void setup(){};
        virtual void tear_down(){};

    private:
        void Test();

    };

    inline void assertment(Test::Source s)
    {
        m_suite->Assertment(s);
    }

    inline bool continue_after_failure()
    {
        return m_suite->ContinueAfterFailure();
    }
}

#endif // __SHARED_PTR_TEST_H__