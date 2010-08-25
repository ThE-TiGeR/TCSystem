#include "shared_ptr_test.h"

#include "uni_shared_ptr.h"
#include "uni_weak_ptr.h"

#include <map>
#include <vector>

#ifdef UNI_USE_TS_PTR
#define SharedPtr SharedPtrTS
#endif

namespace SharedPtrTest
{
    namespace n_constructors
    {
        class incomplete;

        void default_constructor()
        {
            {
                UNI::SharedPtr<int> pi;
                TEST_ASSERT(pi? false: true);
                TEST_ASSERT(!pi);
                TEST_ASSERT(pi.GetPtr() == 0);
            }

            {
                UNI::SharedPtr<void> pv;
                TEST_ASSERT(pv? false: true);
                TEST_ASSERT(!pv);
                TEST_ASSERT(pv.GetPtr() == 0);
            }

            {
                UNI::SharedPtr<incomplete> px;
                TEST_ASSERT(px? false: true);
                TEST_ASSERT(!px);
                TEST_ASSERT(px.GetPtr() == 0);
            }
        }

        struct A
        {
            int dummy;
        };

        struct X
        {
            static long instances;

            X()
            {
                ++instances;
            }

            ~X()
            {
                --instances;
            }

        private:

            X(X const &);
            X & operator= (X const &);
        };

        long X::instances = 0;

        // virtual inheritance stresses the implementation

        struct Y: public A, public virtual X
        {
            static long instances;

            Y()
            {
                ++instances;
            }

            ~Y()
            {
                --instances;
            }

        private:

            Y(Y const &);
            Y & operator= (Y const &);
        };

        long Y::instances = 0;

        template<class T> void pc0_test(T * p)
        {
            TEST_ASSERT(p == 0);
            UNI::SharedPtr<T> pt(p);
            TEST_ASSERT(pt? false: true);
            TEST_ASSERT(!pt);
            TEST_ASSERT(pt.GetPtr() == 0);
            TEST_ASSERT(pt.GetUseCount() == 1);
            TEST_ASSERT(pt.IsUnique());
        }

        void pointer_constructor()
        {
            pc0_test(static_cast<int*>(0));

            pc0_test(static_cast<int*>(0));
            pc0_test(static_cast<int*>(0));
            pc0_test(static_cast<int*>(0));

            {
                UNI::SharedPtr<int> pi(static_cast<int*>(0));
                TEST_ASSERT(pi? false: true);
                TEST_ASSERT(!pi);
                TEST_ASSERT(pi.GetPtr() == 0);
                TEST_ASSERT(pi.GetUseCount() == 1);
                TEST_ASSERT(pi.IsUnique());
            }

            {
                UNI::SharedPtr<int> pi(static_cast<int*>(0));
                TEST_ASSERT(pi? false: true);
                TEST_ASSERT(!pi);
                TEST_ASSERT(pi.GetPtr() == 0);
                TEST_ASSERT(pi.GetUseCount() == 1);
                TEST_ASSERT(pi.IsUnique());
            }

            {
                UNI::SharedPtr<void> pv(static_cast<int*>(0));
                TEST_ASSERT(pv? false: true);
                TEST_ASSERT(!pv);
                TEST_ASSERT(pv.GetPtr() == 0);
                TEST_ASSERT(pv.GetUseCount() == 1);
                TEST_ASSERT(pv.IsUnique());
            }

            {
                UNI::SharedPtr<void const> pv(static_cast<int*>(0));
                TEST_ASSERT(pv? false: true);
                TEST_ASSERT(!pv);
                TEST_ASSERT(pv.GetPtr() == 0);
                TEST_ASSERT(pv.GetUseCount() == 1);
                TEST_ASSERT(pv.IsUnique());
            }

            pc0_test(static_cast<X*>(0));
            pc0_test(static_cast<X const*>(0));
            pc0_test(static_cast<X volatile*>(0));
            pc0_test(static_cast<X const volatile*>(0));

            {
                UNI::SharedPtr<X const> px(static_cast<X*>(0));
                TEST_ASSERT(px? false: true);
                TEST_ASSERT(!px);
                TEST_ASSERT(px.GetPtr() == 0);
                TEST_ASSERT(px.GetUseCount() == 1);
                TEST_ASSERT(px.IsUnique());
            }

            {
                UNI::SharedPtr<X> px(static_cast<Y*>(0));
                TEST_ASSERT(px? false: true);
                TEST_ASSERT(!px);
                TEST_ASSERT(px.GetPtr() == 0);
                TEST_ASSERT(px.GetUseCount() == 1);
                TEST_ASSERT(px.IsUnique());
            }

            {
                UNI::SharedPtr<X const> px(static_cast<Y*>(0));
                TEST_ASSERT(px? false: true);
                TEST_ASSERT(!px);
                TEST_ASSERT(px.GetPtr() == 0);
                TEST_ASSERT(px.GetUseCount() == 1);
                TEST_ASSERT(px.IsUnique());
            }

            {
                UNI::SharedPtr<void> pv(static_cast<X*>(0));
                TEST_ASSERT(pv? false: true);
                TEST_ASSERT(!pv);
                TEST_ASSERT(pv.GetPtr() == 0);
                TEST_ASSERT(pv.GetUseCount() == 1);
                TEST_ASSERT(pv.IsUnique());
            }

            {
                UNI::SharedPtr<void const> pv(static_cast<X*>(0));
                TEST_ASSERT(pv? false: true);
                TEST_ASSERT(!pv);
                TEST_ASSERT(pv.GetPtr() == 0);
                TEST_ASSERT(pv.GetUseCount() == 1);
                TEST_ASSERT(pv.IsUnique());
            }

            {
                int * p = new int(7);
                UNI::SharedPtr<int> pi(p);
                TEST_ASSERT(pi? true: false);
                TEST_ASSERT(!!pi);
                TEST_ASSERT(pi.GetPtr() == p);
                TEST_ASSERT(pi.GetUseCount() == 1);
                TEST_ASSERT(pi.IsUnique());
                TEST_ASSERT(*pi == 7);
            }

            {
                int * p = new int(7);
                UNI::SharedPtr<int> pi(p);
                TEST_ASSERT(pi? true: false);
                TEST_ASSERT(!!pi);
                TEST_ASSERT(pi.GetPtr() == p);
                TEST_ASSERT(pi.GetUseCount() == 1);
                TEST_ASSERT(pi.IsUnique());
                TEST_ASSERT(*pi == 7);
            }

            {
                int * p = new int(7);
                UNI::SharedPtr<void> pv(p);
                TEST_ASSERT(pv? true: false);
                TEST_ASSERT(!!pv);
                TEST_ASSERT(pv.GetPtr() == p);
                TEST_ASSERT(pv.GetUseCount() == 1);
                TEST_ASSERT(pv.IsUnique());
            }

            {
                int * p = new int(7);
                UNI::SharedPtr<void const> pv(p);
                TEST_ASSERT(pv? true: false);
                TEST_ASSERT(!!pv);
                TEST_ASSERT(pv.GetPtr() == p);
                TEST_ASSERT(pv.GetUseCount() == 1);
                TEST_ASSERT(pv.IsUnique());
            }

            TEST_ASSERT(X::instances == 0);

            {
                X * p = new X;
                UNI::SharedPtr<X> px(p);
                TEST_ASSERT(px? true: false);
                TEST_ASSERT(!!px);
                TEST_ASSERT(px.GetPtr() == p);
                TEST_ASSERT(px.GetUseCount() == 1);
                TEST_ASSERT(px.IsUnique());
                TEST_ASSERT(X::instances == 1);
            }

            TEST_ASSERT(X::instances == 0);

            {
                X * p = new X;
                UNI::SharedPtr<X const> px(p);
                TEST_ASSERT(px? true: false);
                TEST_ASSERT(!!px);
                TEST_ASSERT(px.GetPtr() == p);
                TEST_ASSERT(px.GetUseCount() == 1);
                TEST_ASSERT(px.IsUnique());
                TEST_ASSERT(X::instances == 1);
            }

            TEST_ASSERT(X::instances == 0);

            {
                X * p = new X;
                UNI::SharedPtr<void> pv(p);
                TEST_ASSERT(pv? true: false);
                TEST_ASSERT(!!pv);
                TEST_ASSERT(pv.GetPtr() == p);
                TEST_ASSERT(pv.GetUseCount() == 1);
                TEST_ASSERT(pv.IsUnique());
                TEST_ASSERT(X::instances == 1);
            }

            TEST_ASSERT(X::instances == 0);

            {
                X * p = new X;
                UNI::SharedPtr<void const> pv(p);
                TEST_ASSERT(pv? true: false);
                TEST_ASSERT(!!pv);
                TEST_ASSERT(pv.GetPtr() == p);
                TEST_ASSERT(pv.GetUseCount() == 1);
                TEST_ASSERT(pv.IsUnique());
                TEST_ASSERT(X::instances == 1);
            }

            TEST_ASSERT(X::instances == 0);
            TEST_ASSERT(Y::instances == 0);

            {
                Y * p = new Y;
                UNI::SharedPtr<X> px(p);
                TEST_ASSERT(px? true: false);
                TEST_ASSERT(!!px);
                TEST_ASSERT(px.GetPtr() == p);
                TEST_ASSERT(px.GetUseCount() == 1);
                TEST_ASSERT(px.IsUnique());
                TEST_ASSERT(X::instances == 1);
                TEST_ASSERT(Y::instances == 1);
            }

            TEST_ASSERT(X::instances == 0);
            TEST_ASSERT(Y::instances == 0);

            {
                Y * p = new Y;
                UNI::SharedPtr<X const> px(p);
                TEST_ASSERT(px? true: false);
                TEST_ASSERT(!!px);
                TEST_ASSERT(px.GetPtr() == p);
                TEST_ASSERT(px.GetUseCount() == 1);
                TEST_ASSERT(px.IsUnique());
                TEST_ASSERT(X::instances == 1);
                TEST_ASSERT(Y::instances == 1);
            }

            TEST_ASSERT(X::instances == 0);
            TEST_ASSERT(Y::instances == 0);
}

int m = 0;

void deleter(int * p)
{
    TEST_ASSERT(p == 0);
}

void deleter2(int * p)
{
    TEST_ASSERT(p == &m);
    ++*p;
}

void deleter3(incomplete * p)
{
    TEST_ASSERT(p == 0);
}

// Borland C++ 5.5.1 fails on static_cast<incomplete*>(0)

incomplete * p0 = 0;

void deleter_constructor()
{
    {
        UNI::SharedPtr<int> pi(static_cast<int*>(0), deleter);
        TEST_ASSERT(pi? false: true);
        TEST_ASSERT(!pi);
        TEST_ASSERT(pi.GetPtr() == 0);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());
    }

    {
        UNI::SharedPtr<void> pv(static_cast<int*>(0), &deleter);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
    }

    {
        UNI::SharedPtr<void const> pv(static_cast<int*>(0), deleter);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
    }

    {
        UNI::SharedPtr<incomplete> px(p0, deleter3);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
    }

    {
        UNI::SharedPtr<void> pv(p0, deleter3);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
    }

    {
        UNI::SharedPtr<void const> pv(p0, deleter3);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
    }

    TEST_ASSERT(m == 0);

    {
        UNI::SharedPtr<int> pi(&m, deleter2);
        TEST_ASSERT(pi? true: false);
        TEST_ASSERT(!!pi);
        TEST_ASSERT(pi.GetPtr() == &m);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());
    }

    TEST_ASSERT(m == 1);

    {
        UNI::SharedPtr<int> pi(&m, &deleter2);
        TEST_ASSERT(pi? true: false);
        TEST_ASSERT(!!pi);
        TEST_ASSERT(pi.GetPtr() == &m);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());
    }

    TEST_ASSERT(m == 2);

    {
        UNI::SharedPtr<void> pv(&m, deleter2);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == &m);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
    }

    TEST_ASSERT(m == 3);

    {
        UNI::SharedPtr<void const> pv(&m, &deleter2);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == &m);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
    }

    TEST_ASSERT(m == 4);
}

void copy_constructor()
{
    {
        UNI::SharedPtr<int> pi;

        UNI::SharedPtr<int> pi2(pi);
        TEST_ASSERT(pi2 == pi);
        TEST_ASSERT(pi2? false: true);
        TEST_ASSERT(!pi2);
        TEST_ASSERT(pi2.GetPtr() == 0);
        TEST_ASSERT(pi2.GetUseCount() == pi.GetUseCount());

        UNI::SharedPtr<void> pi3(pi);
        TEST_ASSERT(pi3 == pi);
        TEST_ASSERT(pi3? false: true);
        TEST_ASSERT(!pi3);
        TEST_ASSERT(pi3.GetPtr() == 0);
        TEST_ASSERT(pi3.GetUseCount() == pi.GetUseCount());

        UNI::SharedPtr<void> pi4(pi3);
        TEST_ASSERT(pi4 == pi3);
        TEST_ASSERT(pi4? false: true);
        TEST_ASSERT(!pi4);
        TEST_ASSERT(pi4.GetPtr() == 0);
        TEST_ASSERT(pi4.GetUseCount() == pi3.GetUseCount());
    }

    {
        UNI::SharedPtr<void> pv;

        UNI::SharedPtr<void> pv2(pv);
        TEST_ASSERT(pv2 == pv);
        TEST_ASSERT(pv2? false: true);
        TEST_ASSERT(!pv2);
        TEST_ASSERT(pv2.GetPtr() == 0);
        TEST_ASSERT(pv2.GetUseCount() == pv.GetUseCount());
    }

    {
        UNI::SharedPtr<incomplete> px;

        UNI::SharedPtr<incomplete> px2(px);
        TEST_ASSERT(px2 == px);
        TEST_ASSERT(px2? false: true);
        TEST_ASSERT(!px2);
        TEST_ASSERT(px2.GetPtr() == 0);
        TEST_ASSERT(px2.GetUseCount() == px.GetUseCount());

        UNI::SharedPtr<void> px3(px);
        TEST_ASSERT(px3 == px);
        TEST_ASSERT(px3? false: true);
        TEST_ASSERT(!px3);
        TEST_ASSERT(px3.GetPtr() == 0);
        TEST_ASSERT(px3.GetUseCount() == px.GetUseCount());
    }

    {
        UNI::SharedPtr<int> pi(static_cast<int*>(0));

        UNI::SharedPtr<int> pi2(pi);
        TEST_ASSERT(pi2 == pi);
        TEST_ASSERT(pi2? false: true);
        TEST_ASSERT(!pi2);
        TEST_ASSERT(pi2.GetPtr() == 0);
        TEST_ASSERT(pi2.GetUseCount() == 2);
        TEST_ASSERT(!pi2.IsUnique());
        TEST_ASSERT(pi2.GetUseCount() == pi.GetUseCount());
        TEST_ASSERT(!(pi < pi2 || pi2 < pi)); // shared ownership test

        UNI::SharedPtr<void> pi3(pi);
        TEST_ASSERT(pi3 == pi);
        TEST_ASSERT(pi3? false: true);
        TEST_ASSERT(!pi3);
        TEST_ASSERT(pi3.GetPtr() == 0);
        TEST_ASSERT(pi3.GetUseCount() == 3);
        TEST_ASSERT(!pi3.IsUnique());
        TEST_ASSERT(pi3.GetUseCount() == pi.GetUseCount());
        TEST_ASSERT(!(pi < pi3 || pi3 < pi)); // shared ownership test

        UNI::SharedPtr<void> pi4(pi2);
        TEST_ASSERT(pi4 == pi2);
        TEST_ASSERT(pi4? false: true);
        TEST_ASSERT(!pi4);
        TEST_ASSERT(pi4.GetPtr() == 0);
        TEST_ASSERT(pi4.GetUseCount() == 4);
        TEST_ASSERT(!pi4.IsUnique());
        TEST_ASSERT(pi4.GetUseCount() == pi2.GetUseCount());
        TEST_ASSERT(!(pi2 < pi4 || pi4 < pi2)); // shared ownership test

        TEST_ASSERT(pi3.GetUseCount() == pi4.GetUseCount());
        TEST_ASSERT(!(pi3 < pi4 || pi4 < pi3)); // shared ownership test
    }

    {
        UNI::SharedPtr<X> px(static_cast<X*>(0));

        UNI::SharedPtr<X> px2(px);
        TEST_ASSERT(px2 == px);
        TEST_ASSERT(px2? false: true);
        TEST_ASSERT(!px2);
        TEST_ASSERT(px2.GetPtr() == 0);
        TEST_ASSERT(px2.GetUseCount() == 2);
        TEST_ASSERT(!px2.IsUnique());
        TEST_ASSERT(px2.GetUseCount() == px.GetUseCount());
        TEST_ASSERT(!(px < px2 || px2 < px)); // shared ownership test

        UNI::SharedPtr<void> px3(px);
        TEST_ASSERT(px3 == px);
        TEST_ASSERT(px3? false: true);
        TEST_ASSERT(!px3);
        TEST_ASSERT(px3.GetPtr() == 0);
        TEST_ASSERT(px3.GetUseCount() == 3);
        TEST_ASSERT(!px3.IsUnique());
        TEST_ASSERT(px3.GetUseCount() == px.GetUseCount());
        TEST_ASSERT(!(px < px3 || px3 < px)); // shared ownership test

        UNI::SharedPtr<void> px4(px2);
        TEST_ASSERT(px4 == px2);
        TEST_ASSERT(px4? false: true);
        TEST_ASSERT(!px4);
        TEST_ASSERT(px4.GetPtr() == 0);
        TEST_ASSERT(px4.GetUseCount() == 4);
        TEST_ASSERT(!px4.IsUnique());
        TEST_ASSERT(px4.GetUseCount() == px2.GetUseCount());
        TEST_ASSERT(!(px2 < px4 || px4 < px2)); // shared ownership test

        TEST_ASSERT(px3.GetUseCount() == px4.GetUseCount());
        TEST_ASSERT(!(px3 < px4 || px4 < px3)); // shared ownership test
    }

    {
        int * p = new int(7);
        UNI::SharedPtr<int> pi(p);

        UNI::SharedPtr<int> pi2(pi);
        TEST_ASSERT(pi2 == pi);
        TEST_ASSERT(pi2? true: false);
        TEST_ASSERT(!!pi2);
        TEST_ASSERT(pi2.GetPtr() == p);
        TEST_ASSERT(pi2.GetUseCount() == 2);
        TEST_ASSERT(!pi2.IsUnique());
        TEST_ASSERT(*pi2 == 7);
        TEST_ASSERT(pi2.GetUseCount() == pi.GetUseCount());
        TEST_ASSERT(!(pi < pi2 || pi2 < pi)); // shared ownership test
    }

    {
        int * p = new int(7);
        UNI::SharedPtr<void> pv(p);
        TEST_ASSERT(pv.GetPtr() == p);

        UNI::SharedPtr<void> pv2(pv);
        TEST_ASSERT(pv2 == pv);
        TEST_ASSERT(pv2? true: false);
        TEST_ASSERT(!!pv2);
        TEST_ASSERT(pv2.GetPtr() == p);
        TEST_ASSERT(pv2.GetUseCount() == 2);
        TEST_ASSERT(!pv2.IsUnique());
        TEST_ASSERT(pv2.GetUseCount() == pv.GetUseCount());
        TEST_ASSERT(!(pv < pv2 || pv2 < pv)); // shared ownership test
    }

    TEST_ASSERT(X::instances == 0);

    {
        X * p = new X;
        UNI::SharedPtr<X> px(p);
        TEST_ASSERT(px.GetPtr() == p);

        UNI::SharedPtr<X> px2(px);
        TEST_ASSERT(px2 == px);
        TEST_ASSERT(px2? true: false);
        TEST_ASSERT(!!px2);
        TEST_ASSERT(px2.GetPtr() == p);
        TEST_ASSERT(px2.GetUseCount() == 2);
        TEST_ASSERT(!px2.IsUnique());

        TEST_ASSERT(X::instances == 1);

        TEST_ASSERT(px2.GetUseCount() == px.GetUseCount());
        TEST_ASSERT(!(px < px2 || px2 < px)); // shared ownership test

        UNI::SharedPtr<void> px3(px);
        TEST_ASSERT(px3 == px);
        TEST_ASSERT(px3? true: false);
        TEST_ASSERT(!!px3);
        TEST_ASSERT(px3.GetPtr() == p);
        TEST_ASSERT(px3.GetUseCount() == 3);
        TEST_ASSERT(!px3.IsUnique());
        TEST_ASSERT(px3.GetUseCount() == px.GetUseCount());
        TEST_ASSERT(!(px < px3 || px3 < px)); // shared ownership test

        UNI::SharedPtr<void> px4(px2);
        TEST_ASSERT(px4 == px2);
        TEST_ASSERT(px4? true: false);
        TEST_ASSERT(!!px4);
        TEST_ASSERT(px4.GetPtr() == p);
        TEST_ASSERT(px4.GetUseCount() == 4);
        TEST_ASSERT(!px4.IsUnique());
        TEST_ASSERT(px4.GetUseCount() == px2.GetUseCount());
        TEST_ASSERT(!(px2 < px4 || px4 < px2)); // shared ownership test

        TEST_ASSERT(px3.GetUseCount() == px4.GetUseCount());
        TEST_ASSERT(!(px3 < px4 || px4 < px3)); // shared ownership test
    }

    TEST_ASSERT(X::instances == 0);
    TEST_ASSERT(Y::instances == 0);

    {
        Y * p = new Y;
        UNI::SharedPtr<Y> py(p);
        TEST_ASSERT(py.GetPtr() == p);

        UNI::SharedPtr<X> px(py);
        TEST_ASSERT(px == py);
        TEST_ASSERT(px? true: false);
        TEST_ASSERT(!!px);
        TEST_ASSERT(px.GetPtr() == p);
        TEST_ASSERT(px.GetUseCount() == 2);
        TEST_ASSERT(!px.IsUnique());
        TEST_ASSERT(px.GetUseCount() == py.GetUseCount());
        TEST_ASSERT(!(px < py || py < px)); // shared ownership test

        TEST_ASSERT(X::instances == 1);
        TEST_ASSERT(Y::instances == 1);

        UNI::SharedPtr<void const> pv(px);
        TEST_ASSERT(pv == px);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == px.GetPtr());
        TEST_ASSERT(pv.GetUseCount() == 3);
        TEST_ASSERT(!pv.IsUnique());
        TEST_ASSERT(pv.GetUseCount() == px.GetUseCount());
        TEST_ASSERT(!(px < pv || pv < px)); // shared ownership test

        UNI::SharedPtr<void const> pv2(py);
        TEST_ASSERT(pv2 == py);
        TEST_ASSERT(pv2? true: false);
        TEST_ASSERT(!!pv2);
        TEST_ASSERT(pv2.GetPtr() == py.GetPtr());
        TEST_ASSERT(pv2.GetUseCount() == 4);
        TEST_ASSERT(!pv2.IsUnique());
        TEST_ASSERT(pv2.GetUseCount() == py.GetUseCount());
        TEST_ASSERT(!(py < pv2 || pv2 < py)); // shared ownership test

        TEST_ASSERT(pv.GetUseCount() == pv2.GetUseCount());
        TEST_ASSERT(!(pv < pv2 || pv2 < pv)); // shared ownership test
    }

    TEST_ASSERT(X::instances == 0);
    TEST_ASSERT(Y::instances == 0);
}


void auto_ptr_constructor()
{
    {
        std::auto_ptr<int> p;
        UNI::SharedPtr<int> pi(p);
        TEST_ASSERT(pi? false: true);
        TEST_ASSERT(!pi);
        TEST_ASSERT(pi.GetPtr() == 0);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<int> p;
        UNI::SharedPtr<int> pi(p);
        TEST_ASSERT(pi? false: true);
        TEST_ASSERT(!pi);
        TEST_ASSERT(pi.GetPtr() == 0);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<int> p;
        UNI::SharedPtr<void> pv(p);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<int> p;
        UNI::SharedPtr<void const> pv(p);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<X> p;
        UNI::SharedPtr<X> px(p);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<X> p;
        UNI::SharedPtr<X const> px(p);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<Y> p;
        UNI::SharedPtr<X> px(p);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<Y> p;
        UNI::SharedPtr<X const> px(p);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<Y> p;
        UNI::SharedPtr<void> pv(p);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<Y> p;
        UNI::SharedPtr<void const> pv(p);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
        TEST_ASSERT(p.get() == 0);
    }

    {
        std::auto_ptr<int> p(new int(7));
        int * q = p.get();
        UNI::SharedPtr<int> pi(p);
        TEST_ASSERT(pi? true: false);
        TEST_ASSERT(!!pi);
        TEST_ASSERT(pi.GetPtr() == q);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());
        TEST_ASSERT(*pi == 7);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    {
        std::auto_ptr<int> p(new int(7));
        int * q = p.get();
        UNI::SharedPtr<int> pi(p);
        TEST_ASSERT(pi? true: false);
        TEST_ASSERT(!!pi);
        TEST_ASSERT(pi.GetPtr() == q);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());
        TEST_ASSERT(*pi == 7);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    {
        std::auto_ptr<int> p(new int(7));
        int * q = p.get();
        UNI::SharedPtr<void> pv(p);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == q);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    {
        std::auto_ptr<int> p(new int(7));
        int * q = p.get();
        UNI::SharedPtr<void const> pv(p);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == q);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    TEST_ASSERT(X::instances == 0);

    {
        std::auto_ptr<X> p(new X);
        X * q = p.get();
        UNI::SharedPtr<X> px(p);
        TEST_ASSERT(px? true: false);
        TEST_ASSERT(!!px);
        TEST_ASSERT(px.GetPtr() == q);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
        TEST_ASSERT(X::instances == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    TEST_ASSERT(X::instances == 0);

    {
        std::auto_ptr<X> p(new X);
        X * q = p.get();
        UNI::SharedPtr<X const> px(p);
        TEST_ASSERT(px? true: false);
        TEST_ASSERT(!!px);
        TEST_ASSERT(px.GetPtr() == q);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
        TEST_ASSERT(X::instances == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    TEST_ASSERT(X::instances == 0);

    {
        std::auto_ptr<X> p(new X);
        X * q = p.get();
        UNI::SharedPtr<void> pv(p);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == q);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
        TEST_ASSERT(X::instances == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    TEST_ASSERT(X::instances == 0);

    {
        std::auto_ptr<X> p(new X);
        X * q = p.get();
        UNI::SharedPtr<void const> pv(p);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == q);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());
        TEST_ASSERT(X::instances == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    TEST_ASSERT(X::instances == 0);
    TEST_ASSERT(Y::instances == 0);

    {
        std::auto_ptr<Y> p(new Y);
        Y * q = p.get();
        UNI::SharedPtr<X> px(p);
        TEST_ASSERT(px? true: false);
        TEST_ASSERT(!!px);
        TEST_ASSERT(px.GetPtr() == q);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
        TEST_ASSERT(X::instances == 1);
        TEST_ASSERT(Y::instances == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    TEST_ASSERT(X::instances == 0);
    TEST_ASSERT(Y::instances == 0);

    {
        std::auto_ptr<Y> p(new Y);
        Y * q = p.get();
        UNI::SharedPtr<X const> px(p);
        TEST_ASSERT(px? true: false);
        TEST_ASSERT(!!px);
        TEST_ASSERT(px.GetPtr() == q);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());
        TEST_ASSERT(X::instances == 1);
        TEST_ASSERT(Y::instances == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p.get() == 0);
#endif
    }

    TEST_ASSERT(X::instances == 0);
    TEST_ASSERT(Y::instances == 0);
}

void test()
{
    default_constructor();
    pointer_constructor();
    deleter_constructor();
    copy_constructor();
    auto_ptr_constructor();
}

} // namespace n_constructors

namespace n_assignment
{

    class incomplete;

    struct A
    {
        int dummy;
    };

    struct X
    {
        static long instances;

        X()
        {
            ++instances;
        }

        ~X()
        {
            --instances;
        }

    private:

        X(X const &);
        X & operator= (X const &);
    };

    long X::instances = 0;

    struct Y: public A, public virtual X
    {
        static long instances;

        Y()
        {
            ++instances;
        }

        ~Y()
        {
            --instances;
        }

    private:

        Y(Y const &);
        Y & operator= (Y const &);
    };

    long Y::instances = 0;

    void copy_assignment()
    {
        {
            UNI::SharedPtr<incomplete> p1;

            p1 = p1;

            TEST_ASSERT(p1 == p1);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);

            UNI::SharedPtr<incomplete> p2;

            p1 = p2;

            TEST_ASSERT(p1 == p2);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);

            UNI::SharedPtr<incomplete> p3(p1);

            p1 = p3;

            TEST_ASSERT(p1 == p3);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<void> p1;

            p1 = p1;

            TEST_ASSERT(p1 == p1);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);

            UNI::SharedPtr<void> p2;

            p1 = p2;

            TEST_ASSERT(p1 == p2);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);

            UNI::SharedPtr<void> p3(p1);

            p1 = p3;

            TEST_ASSERT(p1 == p3);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);

            UNI::SharedPtr<void> p4(new int);
            TEST_ASSERT(p4.GetUseCount() == 1);

            p1 = p4;

            TEST_ASSERT(p1 == p4);
            TEST_ASSERT(!(p1 < p4 || p4 < p1));
            TEST_ASSERT(p1.GetUseCount() == 2);
            TEST_ASSERT(p4.GetUseCount() == 2);

            p1 = p3;

            TEST_ASSERT(p1 == p3);
            TEST_ASSERT(p4.GetUseCount() == 1);
        }

        {
            UNI::SharedPtr<X> p1;

            p1 = p1;

            TEST_ASSERT(p1 == p1);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);

            UNI::SharedPtr<X> p2;

            p1 = p2;

            TEST_ASSERT(p1 == p2);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);

            UNI::SharedPtr<X> p3(p1);

            p1 = p3;

            TEST_ASSERT(p1 == p3);
            TEST_ASSERT(p1? false: true);
            TEST_ASSERT(!p1);
            TEST_ASSERT(p1.GetPtr() == 0);

            TEST_ASSERT(X::instances == 0);

            UNI::SharedPtr<X> p4(new X);

            TEST_ASSERT(X::instances == 1);

            p1 = p4;

            TEST_ASSERT(X::instances == 1);

            TEST_ASSERT(p1 == p4);
            TEST_ASSERT(!(p1 < p4 || p4 < p1));

            TEST_ASSERT(p1.GetUseCount() == 2);

            p1 = p2;

            TEST_ASSERT(p1 == p2);
            TEST_ASSERT(X::instances == 1);

            p4 = p3;

            TEST_ASSERT(p4 == p3);
            TEST_ASSERT(X::instances == 0);
        }
}

void conversion_assignment()
{
    {
        UNI::SharedPtr<void> p1;

        UNI::SharedPtr<incomplete> p2;

        p1 = p2;

        TEST_ASSERT(p1 == p2);
        TEST_ASSERT(p1? false: true);
        TEST_ASSERT(!p1);
        TEST_ASSERT(p1.GetPtr() == 0);

        UNI::SharedPtr<int> p4(new int);
        TEST_ASSERT(p4.GetUseCount() == 1);

        UNI::SharedPtr<void> p5(p4);
        TEST_ASSERT(p4.GetUseCount() == 2);

        p1 = p4;

        TEST_ASSERT(p1 == p4);
        TEST_ASSERT(!(p1 < p5 || p5 < p1));
        TEST_ASSERT(p1.GetUseCount() == 3);
        TEST_ASSERT(p4.GetUseCount() == 3);

        p1 = p2;

        TEST_ASSERT(p1 == p2);
        TEST_ASSERT(p4.GetUseCount() == 2);
    }

    {
        UNI::SharedPtr<X> p1;

        UNI::SharedPtr<Y> p2;

        p1 = p2;

        TEST_ASSERT(p1 == p2);
        TEST_ASSERT(p1? false: true);
        TEST_ASSERT(!p1);
        TEST_ASSERT(p1.GetPtr() == 0);

        TEST_ASSERT(X::instances == 0);
        TEST_ASSERT(Y::instances == 0);

        UNI::SharedPtr<Y> p4(new Y);

        TEST_ASSERT(X::instances == 1);
        TEST_ASSERT(Y::instances == 1);
        TEST_ASSERT(p4.GetUseCount() == 1);

        UNI::SharedPtr<X> p5(p4);
        TEST_ASSERT(p4.GetUseCount() == 2);

        p1 = p4;

        TEST_ASSERT(X::instances == 1);
        TEST_ASSERT(Y::instances == 1);

        TEST_ASSERT(p1 == p4);
        TEST_ASSERT(!(p1 < p5 || p5 < p1));

        TEST_ASSERT(p1.GetUseCount() == 3);
        TEST_ASSERT(p4.GetUseCount() == 3);

        p1 = p2;

        TEST_ASSERT(p1 == p2);
        TEST_ASSERT(X::instances == 1);
        TEST_ASSERT(Y::instances == 1);
        TEST_ASSERT(p4.GetUseCount() == 2);

        p4 = p2;
        p5 = p2;

        TEST_ASSERT(p4 == p2);
        TEST_ASSERT(X::instances == 0);
        TEST_ASSERT(Y::instances == 0);
    }
}

void auto_ptr_assignment()
{
    {
        UNI::SharedPtr<int> p1;

        std::auto_ptr<int> p2;

        p1 = p2;
        TEST_ASSERT(p1? false: true);
        TEST_ASSERT(!p1);
        TEST_ASSERT(p1.GetPtr() == 0);
        TEST_ASSERT(p1.GetUseCount() == 1);

        int * p = new int;
        std::auto_ptr<int> p3(p);

        p1 = p3;
        TEST_ASSERT(p1.GetPtr() == p);
        TEST_ASSERT(p1.GetUseCount() == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p3.get() == 0);
#endif

        p1 = p2;
        TEST_ASSERT(p1? false: true);
        TEST_ASSERT(!p1);
        TEST_ASSERT(p1.GetPtr() == 0);
        TEST_ASSERT(p1.GetUseCount() == 1);
    }

    {
        UNI::SharedPtr<void> p1;

        std::auto_ptr<int> p2;

        p1 = p2;
        TEST_ASSERT(p1? false: true);
        TEST_ASSERT(!p1);
        TEST_ASSERT(p1.GetPtr() == 0);
        TEST_ASSERT(p1.GetUseCount() == 1);

        int * p = new int;
        std::auto_ptr<int> p3(p);

        p1 = p3;
        TEST_ASSERT(p1.GetPtr() == p);
        TEST_ASSERT(p1.GetUseCount() == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p3.get() == 0);
#endif

        p1 = p2;
        TEST_ASSERT(p1? false: true);
        TEST_ASSERT(!p1);
        TEST_ASSERT(p1.GetPtr() == 0);
        TEST_ASSERT(p1.GetUseCount() == 1);
    }


    {
        UNI::SharedPtr<X> p1;

        std::auto_ptr<Y> p2;

        p1 = p2;
        TEST_ASSERT(p1? false: true);
        TEST_ASSERT(!p1);
        TEST_ASSERT(p1.GetPtr() == 0);
        TEST_ASSERT(p1.GetUseCount() == 1);
        TEST_ASSERT(X::instances == 0);
        TEST_ASSERT(Y::instances == 0);

        Y * p = new Y;
        std::auto_ptr<Y> p3(p);

        TEST_ASSERT(X::instances == 1);
        TEST_ASSERT(Y::instances == 1);

        p1 = p3;
        TEST_ASSERT(p1.GetPtr() == p);
        TEST_ASSERT(p1.GetUseCount() == 1);
        TEST_ASSERT(X::instances == 1);
        TEST_ASSERT(Y::instances == 1);

#if !defined(UNI_OLD_AUTO_PTR)
        TEST_ASSERT(p3.get() == 0);
#endif

        p1 = p2;
        TEST_ASSERT(p1? false: true);
        TEST_ASSERT(!p1);
        TEST_ASSERT(p1.GetPtr() == 0);
        TEST_ASSERT(p1.GetUseCount() == 1);
        TEST_ASSERT(X::instances == 0);
        TEST_ASSERT(Y::instances == 0);
    }
}

void test()
{
    copy_assignment();
    conversion_assignment();
    auto_ptr_assignment();
}

} // namespace n_assignment

namespace n_reset
{

    class incomplete;

    incomplete * p0 = 0;

    void deleter(incomplete *)
    {
    }

    struct X
    {
        static long instances;

        X()
        {
            ++instances;
        }

        ~X()
        {
            --instances;
        }

    private:

        X(X const &);
        X & operator= (X const &);
    };

    long X::instances = 0;

    void plain_reset()
    {
        {
            UNI::SharedPtr<int> pi;
            pi.Reset();
            TEST_ASSERT(pi? false: true);
            TEST_ASSERT(!pi);
            TEST_ASSERT(pi.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<int> pi(static_cast<int*>(0));
            pi.Reset();
            TEST_ASSERT(pi? false: true);
            TEST_ASSERT(!pi);
            TEST_ASSERT(pi.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<int> pi(new int);
            pi.Reset();
            TEST_ASSERT(pi? false: true);
            TEST_ASSERT(!pi);
            TEST_ASSERT(pi.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<incomplete> px;
            px.Reset();
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
            TEST_ASSERT(px.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<incomplete> px(p0, deleter);
            px.Reset();
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
            TEST_ASSERT(px.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<X> px;
            px.Reset();
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
            TEST_ASSERT(px.GetPtr() == 0);
        }

        {
            TEST_ASSERT(X::instances == 0);
            UNI::SharedPtr<X> px(new X);
            TEST_ASSERT(X::instances == 1);
            px.Reset();
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(X::instances == 0);
        }

        {
            UNI::SharedPtr<void> pv;
            pv.Reset();
            TEST_ASSERT(pv? false: true);
            TEST_ASSERT(!pv);
            TEST_ASSERT(pv.GetPtr() == 0);
        }

        {
            TEST_ASSERT(X::instances == 0);
            UNI::SharedPtr<void> pv(new X);
            TEST_ASSERT(X::instances == 1);
            pv.Reset();
            TEST_ASSERT(pv? false: true);
            TEST_ASSERT(!pv);
            TEST_ASSERT(pv.GetPtr() == 0);
            TEST_ASSERT(X::instances == 0);
        }
    }

    struct A
    {
        int dummy;
    };

    struct Y: public A, public virtual X
    {
        static long instances;

        Y()
        {
            ++instances;
        }

        ~Y()
        {
            --instances;
        }

    private:

        Y(Y const &);
        Y & operator= (Y const &);
    };

    long Y::instances = 0;

    void pointer_reset()
    {
        {
            UNI::SharedPtr<int> pi;

            pi.Reset(static_cast<int*>(0));
            TEST_ASSERT(pi? false: true);
            TEST_ASSERT(!pi);
            TEST_ASSERT(pi.GetPtr() == 0);
            TEST_ASSERT(pi.GetUseCount() == 1);
            TEST_ASSERT(pi.IsUnique());

            int * p = new int;
            pi.Reset(p);
            TEST_ASSERT(pi? true: false);
            TEST_ASSERT(!!pi);
            TEST_ASSERT(pi.GetPtr() == p);
            TEST_ASSERT(pi.GetUseCount() == 1);
            TEST_ASSERT(pi.IsUnique());

            pi.Reset(static_cast<int*>(0));
            TEST_ASSERT(pi? false: true);
            TEST_ASSERT(!pi);
            TEST_ASSERT(pi.GetPtr() == 0);
            TEST_ASSERT(pi.GetUseCount() == 1);
            TEST_ASSERT(pi.IsUnique());
        }

        {
            UNI::SharedPtr<X> px;

            px.Reset(static_cast<X*>(0));
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px.IsUnique());
            TEST_ASSERT(X::instances == 0);

            X * p = new X;
            px.Reset(p);
            TEST_ASSERT(px? true: false);
            TEST_ASSERT(!!px);
            TEST_ASSERT(px.GetPtr() == p);
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px.IsUnique());
            TEST_ASSERT(X::instances == 1);

            px.Reset(static_cast<X*>(0));
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px.IsUnique());
            TEST_ASSERT(X::instances == 0);
            TEST_ASSERT(Y::instances == 0);

            Y * q = new Y;
            px.Reset(q);
            TEST_ASSERT(px? true: false);
            TEST_ASSERT(!!px);
            TEST_ASSERT(px.GetPtr() == q);
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px.IsUnique());
            TEST_ASSERT(X::instances == 1);
            TEST_ASSERT(Y::instances == 1);

            px.Reset(static_cast<Y*>(0));
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px.IsUnique());
            TEST_ASSERT(X::instances == 0);
            TEST_ASSERT(Y::instances == 0);
        }

        {
            UNI::SharedPtr<void> pv;

            pv.Reset(static_cast<X*>(0));
            TEST_ASSERT(pv? false: true);
            TEST_ASSERT(!pv);
            TEST_ASSERT(pv.GetPtr() == 0);
            TEST_ASSERT(pv.GetUseCount() == 1);
            TEST_ASSERT(pv.IsUnique());
            TEST_ASSERT(X::instances == 0);

            X * p = new X;
            pv.Reset(p);
            TEST_ASSERT(pv? true: false);
            TEST_ASSERT(!!pv);
            TEST_ASSERT(pv.GetPtr() == p);
            TEST_ASSERT(pv.GetUseCount() == 1);
            TEST_ASSERT(pv.IsUnique());
            TEST_ASSERT(X::instances == 1);

            pv.Reset(static_cast<X*>(0));
            TEST_ASSERT(pv? false: true);
            TEST_ASSERT(!pv);
            TEST_ASSERT(pv.GetPtr() == 0);
            TEST_ASSERT(pv.GetUseCount() == 1);
            TEST_ASSERT(pv.IsUnique());
            TEST_ASSERT(X::instances == 0);
            TEST_ASSERT(Y::instances == 0);

            Y * q = new Y;
            pv.Reset(q);
            TEST_ASSERT(pv? true: false);
            TEST_ASSERT(!!pv);
            TEST_ASSERT(pv.GetPtr() == q);
            TEST_ASSERT(pv.GetUseCount() == 1);
            TEST_ASSERT(pv.IsUnique());
            TEST_ASSERT(X::instances == 1);
            TEST_ASSERT(Y::instances == 1);

            pv.Reset(static_cast<Y*>(0));
            TEST_ASSERT(pv? false: true);
            TEST_ASSERT(!pv);
            TEST_ASSERT(pv.GetPtr() == 0);
            TEST_ASSERT(pv.GetUseCount() == 1);
            TEST_ASSERT(pv.IsUnique());
            TEST_ASSERT(X::instances == 0);
            TEST_ASSERT(Y::instances == 0);
        }
}

void * deleted = 0;

void deleter2(int * p)
{
    deleted = p;
}

void deleter2(X * p)
{
    deleted = p;
}

void deleter2(Y * p)
{
    deleted = p;
}

void deleter2(incomplete * p)
{
    deleted = p;
}

void deleter_reset()
{
    {
        UNI::SharedPtr<int> pi;

        pi.Reset(static_cast<int*>(0), deleter2);
        TEST_ASSERT(pi? false: true);
        TEST_ASSERT(!pi);
        TEST_ASSERT(pi.GetPtr() == 0);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());

        deleted = &pi;

        int m = 0;
        pi.Reset(&m, deleter2);
        TEST_ASSERT(deleted == 0);
        TEST_ASSERT(pi? true: false);
        TEST_ASSERT(!!pi);
        TEST_ASSERT(pi.GetPtr() == &m);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());

        pi.Reset(static_cast<int*>(0), deleter2);
        TEST_ASSERT(deleted == &m);
        TEST_ASSERT(pi? false: true);
        TEST_ASSERT(!pi);
        TEST_ASSERT(pi.GetPtr() == 0);
        TEST_ASSERT(pi.GetUseCount() == 1);
        TEST_ASSERT(pi.IsUnique());

        pi.Reset();
        TEST_ASSERT(deleted == 0);
    }

    {
        UNI::SharedPtr<X> px;

        px.Reset(static_cast<X*>(0), deleter2);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());

        deleted = &px;

        X x;
        px.Reset(&x, deleter2);
        TEST_ASSERT(deleted == 0);
        TEST_ASSERT(px? true: false);
        TEST_ASSERT(!!px);
        TEST_ASSERT(px.GetPtr() == &x);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());

        px.Reset(static_cast<X*>(0), deleter2);
        TEST_ASSERT(deleted == &x);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());

        Y y;
        px.Reset(&y, deleter2);
        TEST_ASSERT(deleted == 0);
        TEST_ASSERT(px? true: false);
        TEST_ASSERT(!!px);
        TEST_ASSERT(px.GetPtr() == &y);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());

        px.Reset(static_cast<Y*>(0), deleter2);
        TEST_ASSERT(deleted == &y);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());

        px.Reset();
        TEST_ASSERT(deleted == 0);
    }

    {
        UNI::SharedPtr<void> pv;

        pv.Reset(static_cast<X*>(0), deleter2);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());

        deleted = &pv;

        X x;
        pv.Reset(&x, deleter2);
        TEST_ASSERT(deleted == 0);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == &x);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());

        pv.Reset(static_cast<X*>(0), deleter2);
        TEST_ASSERT(deleted == &x);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());

        Y y;
        pv.Reset(&y, deleter2);
        TEST_ASSERT(deleted == 0);
        TEST_ASSERT(pv? true: false);
        TEST_ASSERT(!!pv);
        TEST_ASSERT(pv.GetPtr() == &y);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());

        pv.Reset(static_cast<Y*>(0), deleter2);
        TEST_ASSERT(deleted == &y);
        TEST_ASSERT(pv? false: true);
        TEST_ASSERT(!pv);
        TEST_ASSERT(pv.GetPtr() == 0);
        TEST_ASSERT(pv.GetUseCount() == 1);
        TEST_ASSERT(pv.IsUnique());

        pv.Reset();
        TEST_ASSERT(deleted == 0);
    }

    {
        UNI::SharedPtr<incomplete> px;

        px.Reset(p0, deleter2);
        TEST_ASSERT(px? false: true);
        TEST_ASSERT(!px);
        TEST_ASSERT(px.GetPtr() == 0);
        TEST_ASSERT(px.GetUseCount() == 1);
        TEST_ASSERT(px.IsUnique());

        deleted = &px;
        px.Reset(p0, deleter2);
        TEST_ASSERT(deleted == 0);
    }
}

void test()
{
    plain_reset();
    pointer_reset();
    deleter_reset();
}

} // namespace n_reset

namespace n_access
{

    struct X
    {
    };

    void test()
    {
        {
            UNI::SharedPtr<X> px;
            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
        }

        {
            UNI::SharedPtr<X> px(static_cast<X*>(0));
            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);

        }

        {
            UNI::SharedPtr<X> px(static_cast<X*>(0));
            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px? false: true);
            TEST_ASSERT(!px);
        }

        {
            X * p = new X;
            UNI::SharedPtr<X> px(p);
            TEST_ASSERT(px.GetPtr() == p);
            TEST_ASSERT(px? true: false);
            TEST_ASSERT(!!px);
            TEST_ASSERT(&*px == px.GetPtr());
            TEST_ASSERT(px.operator ->() == px.GetPtr());
        }

        {
            X * p = new X;
            UNI::SharedPtr<X> px(p);
            TEST_ASSERT(px.GetPtr() == p);
            TEST_ASSERT(px? true: false);
            TEST_ASSERT(!!px);
            TEST_ASSERT(&*px == px.GetPtr());
            TEST_ASSERT(px.operator ->() == px.GetPtr());
        }
    }

} // namespace n_access

namespace n_use_count
{

    struct X
    {
    };

    void test()
    {
        {
            UNI::SharedPtr<X> px(static_cast<X*>(0));
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px.IsUnique());

            UNI::SharedPtr<X> px2(px);
            TEST_ASSERT(px2.GetUseCount() == 2);
            TEST_ASSERT(!px2.IsUnique());
            TEST_ASSERT(px.GetUseCount() == 2);
            TEST_ASSERT(!px.IsUnique());
        }

        {
            UNI::SharedPtr<X> px(new X);
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px.IsUnique());

            UNI::SharedPtr<X> px2(px);
            TEST_ASSERT(px2.GetUseCount() == 2);
            TEST_ASSERT(!px2.IsUnique());
            TEST_ASSERT(px.GetUseCount() == 2);
            TEST_ASSERT(!px.IsUnique());
        }

        {
            UNI::SharedPtr<X> px(new X);
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px.IsUnique());

            UNI::SharedPtr<X> px2(px);
            TEST_ASSERT(px2.GetUseCount() == 2);
            TEST_ASSERT(!px2.IsUnique());
            TEST_ASSERT(px.GetUseCount() == 2);
            TEST_ASSERT(!px.IsUnique());
        }
    }

} // namespace n_use_count

namespace n_swap
{

    struct X
    {
    };

    void test()
    {
        {
            UNI::SharedPtr<X> px;
            UNI::SharedPtr<X> px2;

            px.Swap(px2);

            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px2.GetPtr() == 0);

            using std::swap;
            swap(px, px2);

            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px2.GetPtr() == 0);
        }

        {
            X * p = new X;
            UNI::SharedPtr<X> px;
            UNI::SharedPtr<X> px2(p);
            UNI::SharedPtr<X> px3(px2);

            px.Swap(px2);

            TEST_ASSERT(px.GetPtr() == p);
            TEST_ASSERT(px.GetUseCount() == 2);
            TEST_ASSERT(px2.GetPtr() == 0);
            TEST_ASSERT(px3.GetPtr() == p);
            TEST_ASSERT(px3.GetUseCount() == 2);

            using std::swap;
            swap(px, px2);

            TEST_ASSERT(px.GetPtr() == 0);
            TEST_ASSERT(px2.GetPtr() == p);
            TEST_ASSERT(px2.GetUseCount() == 2);
            TEST_ASSERT(px3.GetPtr() == p);
            TEST_ASSERT(px3.GetUseCount() == 2);
        }

        {
            X * p1 = new X;
            X * p2 = new X;
            UNI::SharedPtr<X> px(p1);
            UNI::SharedPtr<X> px2(p2);
            UNI::SharedPtr<X> px3(px2);

            px.Swap(px2);

            TEST_ASSERT(px.GetPtr() == p2);
            TEST_ASSERT(px.GetUseCount() == 2);
            TEST_ASSERT(px2.GetPtr() == p1);
            TEST_ASSERT(px2.GetUseCount() == 1);
            TEST_ASSERT(px3.GetPtr() == p2);
            TEST_ASSERT(px3.GetUseCount() == 2);

            using std::swap;
            swap(px, px2);

            TEST_ASSERT(px.GetPtr() == p1);
            TEST_ASSERT(px.GetUseCount() == 1);
            TEST_ASSERT(px2.GetPtr() == p2);
            TEST_ASSERT(px2.GetUseCount() == 2);
            TEST_ASSERT(px3.GetPtr() == p2);
            TEST_ASSERT(px3.GetUseCount() == 2);
        }
    }

} // namespace n_swap

namespace n_comparison
{

    struct X
    {
        int dummy;
    };

    struct Y
    {
        int dummy2;
    };

    struct Z: public X, public virtual Y
    {
    };

    void test()
    {
        {
            UNI::SharedPtr<X> px;
            TEST_ASSERT(px == px);
            TEST_ASSERT(!(px != px));
            TEST_ASSERT(!(px < px));

            UNI::SharedPtr<X> px2;

            TEST_ASSERT(px.GetPtr() == px2.GetPtr());
            TEST_ASSERT(px == px2);
            TEST_ASSERT(!(px != px2));
            TEST_ASSERT(!(px < px2 && px2 < px));
        }

        {
            UNI::SharedPtr<X> px;
            UNI::SharedPtr<X> px2(px);

            TEST_ASSERT(px2 == px2);
            TEST_ASSERT(!(px2 != px2));
            TEST_ASSERT(!(px2 < px2));

            TEST_ASSERT(px.GetPtr() == px2.GetPtr());
            TEST_ASSERT(px == px2);
            TEST_ASSERT(!(px != px2));
            TEST_ASSERT(!(px < px2 && px2 < px));
        }

        {
            UNI::SharedPtr<X> px;
            UNI::SharedPtr<X> px2(new X);

            TEST_ASSERT(px2 == px2);
            TEST_ASSERT(!(px2 != px2));
            TEST_ASSERT(!(px2 < px2));

            TEST_ASSERT(px.GetPtr() != px2.GetPtr());
            TEST_ASSERT(px != px2);
            TEST_ASSERT(!(px == px2));
            TEST_ASSERT(px < px2 || px2 < px);
            TEST_ASSERT(!(px < px2 && px2 < px));
        }

        {
            UNI::SharedPtr<X> px(new X);
            UNI::SharedPtr<X> px2(new X);

            TEST_ASSERT(px.GetPtr() != px2.GetPtr());
            TEST_ASSERT(px != px2);
            TEST_ASSERT(!(px == px2));
            TEST_ASSERT(px < px2 || px2 < px);
            TEST_ASSERT(!(px < px2 && px2 < px));
        }

        {
            UNI::SharedPtr<X> px(new X);
            UNI::SharedPtr<X> px2(px);

            TEST_ASSERT(px2 == px2);
            TEST_ASSERT(!(px2 != px2));
            TEST_ASSERT(!(px2 < px2));

            TEST_ASSERT(px.GetPtr() == px2.GetPtr());
            TEST_ASSERT(px == px2);
            TEST_ASSERT(!(px != px2));
            TEST_ASSERT(!(px < px2 || px2 < px));
        }

        {
            UNI::SharedPtr<X> px(new X);
            UNI::SharedPtr<Y> py(new Y);
            UNI::SharedPtr<Z> pz(new Z);

            TEST_ASSERT(px.GetPtr() != pz.GetPtr());
            TEST_ASSERT(px != pz);
            TEST_ASSERT(!(px == pz));

            TEST_ASSERT(py.GetPtr() != pz.GetPtr());
            TEST_ASSERT(py != pz);
            TEST_ASSERT(!(py == pz));

            TEST_ASSERT(px < py || py < px);
            TEST_ASSERT(px < pz || pz < px);
            TEST_ASSERT(py < pz || pz < py);

            TEST_ASSERT(!(px < py && py < px));
            TEST_ASSERT(!(px < pz && pz < px));
            TEST_ASSERT(!(py < pz && pz < py));

            UNI::SharedPtr<void> pvx(px);

            TEST_ASSERT(pvx == pvx);
            TEST_ASSERT(!(pvx != pvx));
            TEST_ASSERT(!(pvx < pvx));

            UNI::SharedPtr<void> pvy(py);
            UNI::SharedPtr<void> pvz(pz);

            TEST_ASSERT(pvx < pvy || pvy < pvx);
            TEST_ASSERT(pvx < pvz || pvz < pvx);
            TEST_ASSERT(pvy < pvz || pvz < pvy);

            TEST_ASSERT(!(pvx < pvy && pvy < pvx));
            TEST_ASSERT(!(pvx < pvz && pvz < pvx));
            TEST_ASSERT(!(pvy < pvz && pvz < pvy));
        }

        {
            UNI::SharedPtr<Z> pz(new Z);
            UNI::SharedPtr<X> px(pz);

            TEST_ASSERT(px == px);
            TEST_ASSERT(!(px != px));
            TEST_ASSERT(!(px < px));

            UNI::SharedPtr<Y> py(pz);

            TEST_ASSERT(px.GetPtr() == pz.GetPtr());
            TEST_ASSERT(px == pz);
            TEST_ASSERT(!(px != pz));

            TEST_ASSERT(py.GetPtr() == pz.GetPtr());
            TEST_ASSERT(py == pz);
            TEST_ASSERT(!(py != pz));

            TEST_ASSERT(!(px < py || py < px));
            TEST_ASSERT(!(px < pz || pz < px));
            TEST_ASSERT(!(py < pz || pz < py));

            UNI::SharedPtr<void> pvx(px);
            UNI::SharedPtr<void> pvy(py);
            UNI::SharedPtr<void> pvz(pz);

            // pvx and pvy aren't equal...
            TEST_ASSERT(pvx.GetPtr() != pvy.GetPtr());
            TEST_ASSERT(pvx != pvy);
            TEST_ASSERT(!(pvx == pvy));

            // ... but they share ownership ...
            TEST_ASSERT(!(pvx < pvy || pvy < pvx));

            // ... with pvz
            TEST_ASSERT(!(pvx < pvz || pvz < pvx));
            TEST_ASSERT(!(pvy < pvz || pvz < pvy));
        }
}

} // namespace n_comparison

namespace n_static_cast
{

    struct X
    {
    };

    struct Y: public X
    {
    };

    void test()
    {
        {
            UNI::SharedPtr<void> pv;

            UNI::SharedPtr<int> pi = UNI::SharedPtr<int>::StaticCast(pv);
            TEST_ASSERT(pi.GetPtr() == 0);

            UNI::SharedPtr<X> px = UNI::SharedPtr<X>::StaticCast(pv);
            TEST_ASSERT(px.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<int> pi(new int);
            UNI::SharedPtr<void> pv(pi);

            UNI::SharedPtr<int> pi2 = UNI::SharedPtr<int>::StaticCast(pv);
            TEST_ASSERT(pi.GetPtr() == pi2.GetPtr());
            TEST_ASSERT(!(pi < pi2 || pi2 < pi));
            TEST_ASSERT(pi.GetUseCount() == 3);
            TEST_ASSERT(pv.GetUseCount() == 3);
            TEST_ASSERT(pi2.GetUseCount() == 3);
        }

        {
            UNI::SharedPtr<X> px(new X);
            UNI::SharedPtr<void> pv(px);

            UNI::SharedPtr<X> px2 = UNI::SharedPtr<X>::StaticCast(pv);
            TEST_ASSERT(px.GetPtr() == px2.GetPtr());
            TEST_ASSERT(!(px < px2 || px2 < px));
            TEST_ASSERT(px.GetUseCount() == 3);
            TEST_ASSERT(pv.GetUseCount() == 3);
            TEST_ASSERT(px2.GetUseCount() == 3);
        }

        {
            UNI::SharedPtr<X> px(new Y);

            UNI::SharedPtr<Y> py = UNI::SharedPtr<Y>::StaticCast(px);
            TEST_ASSERT(px.GetPtr() == py.GetPtr());
            TEST_ASSERT(px.GetUseCount() == 2);
            TEST_ASSERT(py.GetUseCount() == 2);

            UNI::SharedPtr<X> px2(py);
            TEST_ASSERT(!(px < px2 || px2 < px));
        }
    }

} // namespace n_static_cast

namespace n_const_cast
{

    struct X;

    void test()
    {
        {
            UNI::SharedPtr<void const volatile> px;

            UNI::SharedPtr<void> px2 = UNI::SharedPtr<void>::ConstCast(px);
            TEST_ASSERT(px2.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<int> px;

            UNI::SharedPtr<int> px2 = UNI::SharedPtr<int>::ConstCast(px);
            TEST_ASSERT(px2.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<X const volatile> px;

            UNI::SharedPtr<X> px2 = UNI::SharedPtr<X>::ConstCast(px);
            TEST_ASSERT(px2.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<void const volatile> px(new int);

            UNI::SharedPtr<void> px2 = UNI::SharedPtr<void>::ConstCast(px);
            TEST_ASSERT(px.GetPtr() == px2.GetPtr());
            TEST_ASSERT(!(px < px2 || px2 < px));
            TEST_ASSERT(px.GetUseCount() == 2);
            TEST_ASSERT(px2.GetUseCount() == 2);
        }

        {
            UNI::SharedPtr<int> px(new int);

            UNI::SharedPtr<int> px2 = UNI::SharedPtr<int>::ConstCast(px);
            TEST_ASSERT(px.GetPtr() == px2.GetPtr());
            TEST_ASSERT(!(px < px2 || px2 < px));
            TEST_ASSERT(px.GetUseCount() == 2);
            TEST_ASSERT(px2.GetUseCount() == 2);
        }
    }

} // namespace n_const_cast

namespace n_dynamic_cast
{

    struct V
    {
        virtual ~V() {}
    };

    struct W: public V
    {
    };

    void test()
    {
        {
            UNI::SharedPtr<V> pv;
            UNI::SharedPtr<W> pw = UNI::SharedPtr<W>::DynamicCast(pv);
            TEST_ASSERT(pw.GetPtr() == 0);
        }

        {
            UNI::SharedPtr<V> pv(static_cast<V*>(0));

            UNI::SharedPtr<W> pw = UNI::SharedPtr<W>::DynamicCast(pv);
            TEST_ASSERT(pw.GetPtr() == 0);

            UNI::SharedPtr<V> pv2(pw);
            TEST_ASSERT(pv < pv2 || pv2 < pv);
        }

        {
            UNI::SharedPtr<V> pv(static_cast<W*>(0));

            UNI::SharedPtr<W> pw = UNI::SharedPtr<W>::DynamicCast(pv);
            TEST_ASSERT(pw.GetPtr() == 0);

            UNI::SharedPtr<V> pv2(pw);
            TEST_ASSERT(pv < pv2 || pv2 < pv);
        }

        {
            UNI::SharedPtr<V> pv(new V);

            UNI::SharedPtr<W> pw = UNI::SharedPtr<W>::DynamicCast(pv);
            TEST_ASSERT(pw.GetPtr() == 0);

            UNI::SharedPtr<V> pv2(pw);
            TEST_ASSERT(pv < pv2 || pv2 < pv);
        }

        {
            UNI::SharedPtr<V> pv(new W);

            UNI::SharedPtr<W> pw = UNI::SharedPtr<W>::DynamicCast(pv);
            TEST_ASSERT(pw.GetPtr() == pv.GetPtr());
            TEST_ASSERT(pv.GetUseCount() == 2);
            TEST_ASSERT(pw.GetUseCount() == 2);

            UNI::SharedPtr<V> pv2(pw);
            TEST_ASSERT(!(pv < pv2 || pv2 < pv));
        }
    }

} // namespace n_dynamic_cast

namespace n_map
{

    struct X
    {
    };

    void test()
    {
        std::vector< UNI::SharedPtr<int> > vi;

        {
            UNI::SharedPtr<int> pi1(new int);
            UNI::SharedPtr<int> pi2(new int);
            UNI::SharedPtr<int> pi3(new int);

            vi.push_back(pi1);
            vi.push_back(pi1);
            vi.push_back(pi1);
            vi.push_back(pi2);
            vi.push_back(pi1);
            vi.push_back(pi2);
            vi.push_back(pi1);
            vi.push_back(pi3);
            vi.push_back(pi3);
            vi.push_back(pi2);
            vi.push_back(pi1);
        }

        std::vector< UNI::SharedPtr<X> > vx;

        {
            UNI::SharedPtr<X> px1(new X);
            UNI::SharedPtr<X> px2(new X);
            UNI::SharedPtr<X> px3(new X);

            vx.push_back(px2);
            vx.push_back(px2);
            vx.push_back(px1);
            vx.push_back(px2);
            vx.push_back(px1);
            vx.push_back(px1);
            vx.push_back(px1);
            vx.push_back(px2);
            vx.push_back(px1);
            vx.push_back(px3);
            vx.push_back(px2);
        }

        std::map< UNI::SharedPtr<void>, long > m;

        {
            for(std::vector< UNI::SharedPtr<int> >::iterator i = vi.begin(); i != vi.end(); ++i)
            {
                ++m[*i];
            }
        }

        {
            for(std::vector< UNI::SharedPtr<X> >::iterator i = vx.begin(); i != vx.end(); ++i)
            {
                ++m[*i];
            }
        }

        {
            for(std::map< UNI::SharedPtr<void>, long >::iterator i = m.begin(); i != m.end(); ++i)
            {
                TEST_ASSERT(i->first.GetUseCount() == i->second + 1);
            }
        }
    }

} // namespace n_map

namespace n_transitive
{

    struct X
    {
        X(): next() {}
        UNI::SharedPtr<X> next;
    };

    void test()
    {
        UNI::SharedPtr<X> p(new X);
        p->next = UNI::SharedPtr<X>(new X);
        TEST_ASSERT(!p->next->next);
        p = p->next;
        TEST_ASSERT(!p->next);
    }

} // namespace n_transitive

namespace n_report_1
{

    class foo
    {
    public:

        foo(): m_self(this)
        {
        }

        void suicide()
        {
            m_self.Reset();
        }

    private:

        UNI::SharedPtr<foo> m_self;
    };

    void test()
    {
        foo * foo_ptr = new foo;
        foo_ptr->suicide();
    }

} // namespace n_report_1

// Test case by Per Kristensen
namespace n_report_2
{

    class foo
    {
    public:

        void setWeak(UNI::SharedPtr<foo> s)
        {
            w = s;
        }

    private:

        UNI::WeakPtr<foo> w;
    };

    class deleter
    {
    public:

        deleter(): lock(0)
        {
        }

        ~deleter()
        {
            TEST_ASSERT(lock == 0);
        }

        void operator() (foo * p)
        {
            ++lock;
            delete p;
            --lock;
        }

    private:

        int lock;
    };

    void test()
    {
        UNI::SharedPtr<foo> s(new foo, deleter());
        s->setWeak(s);
        s.Reset();
    }

} // namespace n_report_2

namespace n_spt_incomplete
{

    class file;

    UNI::SharedPtr<file> fopen(char const * name, char const * mode);
    void fread(UNI::SharedPtr<file> f, void * data, long size);

    int file_instances = 0;

    void test()
    {
        TEST_ASSERT(file_instances == 0);

        {
            UNI::SharedPtr<file> pf = fopen("name", "mode");
            TEST_ASSERT(file_instances == 1);
            fread(pf, 0, 17041);
        }

        TEST_ASSERT(file_instances == 0);
    }

} // namespace n_spt_incomplete

namespace n_spt_pimpl
{

    class file
    {
    private:

        class impl;
        UNI::SharedPtr<impl> pimpl_;

        public:

            file(char const * name, char const * mode);

            // compiler generated members are fine and useful

            void read(void * data, long size);

            long total_size() const;
    };

    int file_instances = 0;

    void test()
    {
        TEST_ASSERT(file_instances == 0);

        {
            file f("name", "mode");
            TEST_ASSERT(file_instances == 1);
            f.read(0, 152);

            file f2(f);
            TEST_ASSERT(file_instances == 1);
            f2.read(0, 894);

            TEST_ASSERT(f.total_size() == 152+894);

            {
                file f3("name2", "mode2");
                TEST_ASSERT(file_instances == 2);
            }

            TEST_ASSERT(file_instances == 1);
        }

        TEST_ASSERT(file_instances == 0);
    }

} // namespace n_spt_pimpl

namespace n_spt_abstract
{

    class X
    {
    public:

        virtual void f(int) = 0;
        virtual int g() = 0;

        ~X() {}
    };

    UNI::SharedPtr<X> createX();

    int X_instances = 0;

    void test()
    {
        TEST_ASSERT(X_instances == 0);

        {
            UNI::SharedPtr<X> px = createX();

            TEST_ASSERT(X_instances == 1);

            px->f(18);
            px->f(152);

            TEST_ASSERT(px->g() == 170);
        }

        TEST_ASSERT(X_instances == 0);
    }

} // namespace n_spt_abstract

namespace n_spt_preventing_delete
{

    int X_instances = 0;

    class X
    {
    private:

        X()
        {
            ++X_instances;
        }

        ~X()
        {
            --X_instances;
        }


        static void deleter(X* p) { delete p; }

    public:

        static UNI::SharedPtr<X> create()
        {
            UNI::SharedPtr<X> px(new X, X::deleter);
            return px;
        }
    };

    void test()
    {
        TEST_ASSERT(X_instances == 0);

        {
            UNI::SharedPtr<X> px = X::create();
            TEST_ASSERT(X_instances == 1);
        }

        TEST_ASSERT(X_instances == 0);
    }

} // namespace n_spt_preventing_delete

namespace n_spt_array
{

    int X_instances = 0;

    struct X
    {
        X()
        {
            ++X_instances;
        }

        ~X()
        {
            --X_instances;
        }
    };

    void array_delete(X* x)
    {
        delete []x;
    };

    void test()
    {
        TEST_ASSERT(X_instances == 0);

        {
            UNI::SharedPtr<X> px(new X[4], array_delete);
            TEST_ASSERT(X_instances == 4);
        }

        TEST_ASSERT(X_instances == 0);
    }

} // namespace n_spt_array

namespace n_spt_static
{

    class X
    {
    public:

        X()
        {
        }

    private:

        void operator delete(void *)
        {
            // Comeau 4.3.0.1 wants a definition
            TEST_FAIL("n_spt_static::X::operator delete() called.");
        }
    };

    void null_deleter(X *)
    {
    }

    static X x;

    void test()
    {
        UNI::SharedPtr<X> px(&x, null_deleter);
    }

} // namespace n_spt_static

namespace n_spt_intrusive
{

    int X_instances = 0;

    struct X
    {
        long count;

        X(): count(0)
        {
            ++X_instances;
        }

        ~X()
        {
            --X_instances;
        }
    };

    void intrusive_ptr_add_ref(X * p)
    {
        ++p->count;
    }

    void intrusive_ptr_release(X * p)
    {
        if(--p->count == 0) delete p;
    }

    void intrusive_deleter(X * p)
    {
        if(p != 0) intrusive_ptr_release(p);
    }

    UNI::SharedPtr<X> make_shared_from_intrusive(X * p)
    {
        if(p != 0) intrusive_ptr_add_ref(p);
        UNI::SharedPtr<X> px(p, intrusive_deleter);
        return px;
    }

    void test()
    {
        TEST_ASSERT(X_instances == 0);

        {
            X * p = new X;
            TEST_ASSERT(X_instances == 1);
            TEST_ASSERT(p->count == 0);
            UNI::SharedPtr<X> px = make_shared_from_intrusive(p);
            TEST_ASSERT(px.GetPtr() == p);
            TEST_ASSERT(p->count == 1);
            UNI::SharedPtr<X> px2(px);
            TEST_ASSERT(px2.GetPtr() == p);
            TEST_ASSERT(p->count == 1);
        }

        TEST_ASSERT(X_instances == 0);
    }

} // namespace n_spt_intrusive

namespace n_spt_another_sp
{

    template<class T> class another_ptr: private UNI::SharedPtr<T>
    {
private:

    typedef UNI::SharedPtr<T> base_type;

public:

    explicit another_ptr(T * p = 0): base_type(p)
    {
    }

    void Reset()
    {
        base_type::Reset();
    }

    T * GetPtr() const
    {
        return base_type::GetPtr();
    }
    };

    class event_handler
    {
    public:

        virtual ~event_handler() {}
        virtual void begin() = 0;
        virtual void handle(int event) = 0;
        virtual void end() = 0;
    };

    int begin_called = 0;
    int handle_called = 0;
    int end_called = 0;

    class event_handler_impl: public event_handler
    {
    public:

        virtual void begin()
        {
            ++begin_called;
        }

        virtual void handle(int event)
        {
            handle_called = event;
        }

        virtual void end()
        {
            ++end_called;
        }
    };

    another_ptr<event_handler> get_event_handler()
    {
        another_ptr<event_handler> p(new event_handler_impl);
        return p;
    }

    UNI::SharedPtr<event_handler> current_handler;

    void install_event_handler(UNI::SharedPtr<event_handler> p)
    {
        p->begin();
        current_handler = p;
    }

    void handle_event(int event)
    {
        current_handler->handle(event);
    }

    void remove_event_handler()
    {
        current_handler->end();
        current_handler.Reset();
    }

    template<class P> class smart_pointer_deleter
    {
    private:

        P p_;

    public:

        smart_pointer_deleter(P const & p): p_(p)
        {
        }

        void operator()(void const *)
        {
            p_.Reset();
        }
    };

    void test()
    {
        another_ptr<event_handler> p = get_event_handler();

        UNI::SharedPtr<event_handler> q(p.GetPtr(), smart_pointer_deleter< another_ptr<event_handler> >(p));

        p.Reset();

        TEST_ASSERT(begin_called == 0);

        install_event_handler(q);

        TEST_ASSERT(begin_called == 1);

        TEST_ASSERT(handle_called == 0);

        handle_event(17041);

        TEST_ASSERT(handle_called == 17041);

        TEST_ASSERT(end_called == 0);

        remove_event_handler();

        TEST_ASSERT(end_called == 1);
    }

} // namespace n_spt_another_sp

namespace n_spt_shared_from_this
{

    class X
    {
    public:

        virtual void f() = 0;

    protected:

        ~X() {}
    };

    class Y
    {
    public:

        virtual UNI::SharedPtr<X> getX() = 0;

    protected:

        ~Y() {}
    };

    class impl: public X, public Y
    {
    private:

        UNI::WeakPtr<impl> weak_this;

        impl(impl const &);
        impl & operator=(impl const &);

        impl() {}

    public:

        static UNI::SharedPtr<impl> create()
        {
            UNI::SharedPtr<impl> pi(new impl);
            pi->weak_this = pi;
            return pi;
        }

        virtual void f() {}

        virtual UNI::SharedPtr<X> getX()
        {
#ifdef UNI_USE_TS_PTR
            UNI::SharedPtrTS<X> px;
#else
            UNI::SharedPtr<X> px = weak_this.Lock();
#endif
            return px;
        }
    };

    void test()
    {
        UNI::SharedPtr<Y> py = impl::create();
        TEST_ASSERT(py.GetPtr() != 0);
        TEST_ASSERT(py.GetUseCount() == 1);

        UNI::SharedPtr<X> px = py->getX();
        TEST_ASSERT(px.GetPtr() != 0);
        TEST_ASSERT(py.GetUseCount() == 2);

        UNI::SharedPtr<Y> py2 = UNI::SharedPtr<Y>::DynamicCast(px);
        TEST_ASSERT(py.GetPtr() == py2.GetPtr());
        TEST_ASSERT(!(py < py2 || py2 < py));
        TEST_ASSERT(py.GetUseCount() == 3);
    }

} // namespace n_spt_shared_from_this

namespace n_spt_wrap
{

    void test()
    {
    }

} // namespace n_spt_wrap

namespace n_spt_incomplete
{
    class file
    {
    public:

        file(): fread_called(false)
        {
            ++file_instances;
        }

        ~file()
        {
            TEST_ASSERT(fread_called);
            --file_instances;
        }

        bool fread_called;
    };

    UNI::SharedPtr<file> fopen(char const *, char const *)
    {
        UNI::SharedPtr<file> pf(new file);
        return pf;
    }

    void fread(UNI::SharedPtr<file> pf, void *, long)
    {
        pf->fread_called = true;
    }

} // namespace n_spt_incomplete

namespace n_spt_pimpl
{

    class file::impl
    {
    private:

        impl(impl const &);
        impl & operator=(impl const &);

        long total_size_;

    public:

        impl(char const *, char const *): total_size_(0)
        {
            ++file_instances;
        }

        ~impl()
        {
            --file_instances;
        }

        void read(void *, long size)
        {
            total_size_ += size;
        }

        long total_size() const
        {
            return total_size_;
        }
    };

    file::file(char const * name, char const * mode): pimpl_(new impl(name, mode))
    {
    }

    void file::read(void * data, long size)
    {
        pimpl_->read(data, size);
    }

    long file::total_size() const
    {
        return pimpl_->total_size();
    }

} // namespace n_spt_pimpl

namespace n_spt_abstract
{

    class X_impl: public X
    {
    private:

        X_impl(X_impl const &);
        X_impl & operator=(X_impl const &);

        int n_;

    public:

        X_impl(): n_(0)
        {
            ++X_instances;
        }

        ~X_impl()
        {
            --X_instances;
        }

        virtual void f(int n)
        {
            n_ += n;
        }

        virtual int g()
        {
            return n_;
        }
    };

    UNI::SharedPtr<X> createX()
    {
        UNI::SharedPtr<X> px(new X_impl);
        return px;
    }

} // namespace n_spt_abstract

Suite* m_suite = 0;

void Suite::Test()
{
    n_constructors::test();
    n_assignment::test();
    n_reset::test();
    n_access::test();
    n_use_count::test();
    n_swap::test();
    n_comparison::test();
    n_static_cast::test();
    n_const_cast::test();
    n_dynamic_cast::test();

    n_map::test();

    //n_transitive::test();
    n_report_1::test();
    n_report_2::test();

    n_spt_incomplete::test();
    n_spt_pimpl::test();
    n_spt_abstract::test();
    n_spt_preventing_delete::test();
    n_spt_array::test();
    n_spt_static::test();
    n_spt_intrusive::test();
    n_spt_another_sp::test();
    n_spt_shared_from_this::test();
    n_spt_wrap::test();
}

}