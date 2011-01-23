#include "weak_ptr_test.h"

#include "uni_weak_ptr.h"

#include <map>
#include <vector>

namespace WeakPtrTest
{
    class incomplete;

    UNI::SharedPtr<incomplete> create_incomplete();

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

    namespace n_constructors
    {

        void default_constructor()
        {
            {
                UNI::WeakPtr<int> wp;
                TEST_ASSERT(wp.GetUseCount() == 0);
            }

            {
                UNI::WeakPtr<void> wp;
                TEST_ASSERT(wp.GetUseCount() == 0);
            }

            {
                UNI::WeakPtr<incomplete> wp;
                TEST_ASSERT(wp.GetUseCount() == 0);
            }
        }

        void shared_ptr_constructor()
        {
            {
                UNI::SharedPtr<int> sp;

                UNI::WeakPtr<int> wp(sp);
                TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());

                UNI::WeakPtr<void> wp2(sp);
                TEST_ASSERT(wp2.GetUseCount() == sp.GetUseCount());
            }

            {
                UNI::SharedPtr<int> sp(static_cast<int*>(0));

                {
                    UNI::WeakPtr<int> wp(sp);
                    TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                    TEST_ASSERT(wp.GetUseCount() == 1);
                    UNI::SharedPtr<int> sp2(wp);
                    TEST_ASSERT(wp.GetUseCount() == 2);
                    TEST_ASSERT(!(sp < sp2 || sp2 < sp));
                }

                {
                    UNI::WeakPtr<void> wp(sp);
                    TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                    TEST_ASSERT(wp.GetUseCount() == 1);
                    UNI::SharedPtr<void> sp2(wp);
                    TEST_ASSERT(wp.GetUseCount() == 2);
                    TEST_ASSERT(!(sp < sp2 || sp2 < sp));
                }
            }

            {
                UNI::SharedPtr<int> sp(new int);

                {
                    UNI::WeakPtr<int> wp(sp);
                    TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                    TEST_ASSERT(wp.GetUseCount() == 1);
                    UNI::SharedPtr<int> sp2(wp);
                    TEST_ASSERT(wp.GetUseCount() == 2);
                    TEST_ASSERT(!(sp < sp2 || sp2 < sp));
                }

                {
                    UNI::WeakPtr<void> wp(sp);
                    TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                    TEST_ASSERT(wp.GetUseCount() == 1);
                    UNI::SharedPtr<void> sp2(wp);
                    TEST_ASSERT(wp.GetUseCount() == 2);
                    TEST_ASSERT(!(sp < sp2 || sp2 < sp));
                }
            }

            {
                UNI::SharedPtr<void> sp;

                UNI::WeakPtr<void> wp(sp);
                TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
            }

            {
                UNI::SharedPtr<void> sp(static_cast<int*>(0));

                UNI::WeakPtr<void> wp(sp);
                TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                TEST_ASSERT(wp.GetUseCount() == 1);
                UNI::SharedPtr<void> sp2(wp);
                TEST_ASSERT(wp.GetUseCount() == 2);
                TEST_ASSERT(!(sp < sp2 || sp2 < sp));
            }

            {
                UNI::SharedPtr<void> sp(new int);

                UNI::WeakPtr<void> wp(sp);
                TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                TEST_ASSERT(wp.GetUseCount() == 1);
                UNI::SharedPtr<void> sp2(wp);
                TEST_ASSERT(wp.GetUseCount() == 2);
                TEST_ASSERT(!(sp < sp2 || sp2 < sp));
            }

            {
                UNI::SharedPtr<incomplete> sp;

                UNI::WeakPtr<incomplete> wp(sp);
                TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());

                UNI::WeakPtr<void> wp2(sp);
                TEST_ASSERT(wp2.GetUseCount() == sp.GetUseCount());
            }

            {
                UNI::SharedPtr<incomplete> sp = create_incomplete();

                {
                    UNI::WeakPtr<incomplete> wp(sp);
                    TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                    TEST_ASSERT(wp.GetUseCount() == 1);
                    UNI::SharedPtr<incomplete> sp2(wp);
                    TEST_ASSERT(wp.GetUseCount() == 2);
                    TEST_ASSERT(!(sp < sp2 || sp2 < sp));
                }

                {
                    UNI::WeakPtr<void> wp(sp);
                    TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                    TEST_ASSERT(wp.GetUseCount() == 1);
                    UNI::SharedPtr<void> sp2(wp);
                    TEST_ASSERT(wp.GetUseCount() == 2);
                    TEST_ASSERT(!(sp < sp2 || sp2 < sp));
                }
            }

            {
                UNI::SharedPtr<void> sp = create_incomplete();

                UNI::WeakPtr<void> wp(sp);
                TEST_ASSERT(wp.GetUseCount() == sp.GetUseCount());
                TEST_ASSERT(wp.GetUseCount() == 1);
                UNI::SharedPtr<void> sp2(wp);
                TEST_ASSERT(wp.GetUseCount() == 2);
                TEST_ASSERT(!(sp < sp2 || sp2 < sp));
            }
}

void copy_constructor()
{
    {
        UNI::WeakPtr<int> wp;
        UNI::WeakPtr<int> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 0);
    }

    {
        UNI::WeakPtr<void> wp;
        UNI::WeakPtr<void> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 0);
    }

    {
        UNI::WeakPtr<incomplete> wp;
        UNI::WeakPtr<incomplete> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 0);
    }

    {
        UNI::SharedPtr<int> sp(static_cast<int*>(0));
        UNI::WeakPtr<int> wp(sp);

        UNI::WeakPtr<int> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<int> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<int> sp(new int);
        UNI::WeakPtr<int> wp(sp);

        UNI::WeakPtr<int> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<int> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<void> sp(static_cast<int*>(0));
        UNI::WeakPtr<void> wp(sp);

        UNI::WeakPtr<void> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<void> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<void> sp(new int);
        UNI::WeakPtr<void> wp(sp);

        UNI::WeakPtr<void> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<void> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<incomplete> sp = create_incomplete();
        UNI::WeakPtr<incomplete> wp(sp);

        UNI::WeakPtr<incomplete> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<incomplete> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }
}

void conversion_constructor()
{
    {
        UNI::WeakPtr<int> wp;
        UNI::WeakPtr<void> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 0);
    }

    {
        UNI::WeakPtr<incomplete> wp;
        UNI::WeakPtr<void> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 0);
    }

    {
        UNI::WeakPtr<Z> wp;

        UNI::WeakPtr<X> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 0);

        UNI::WeakPtr<Y> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
    }

    {
        UNI::SharedPtr<int> sp(static_cast<int*>(0));
        UNI::WeakPtr<int> wp(sp);

        UNI::WeakPtr<void> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<void> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<int> sp(new int);
        UNI::WeakPtr<int> wp(sp);

        UNI::WeakPtr<void> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<void> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<incomplete> sp = create_incomplete();
        UNI::WeakPtr<incomplete> wp(sp);

        UNI::WeakPtr<void> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<void> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<Z> sp(static_cast<Z*>(0));
        UNI::WeakPtr<Z> wp(sp);

        UNI::WeakPtr<X> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<X> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<Z> sp(static_cast<Z*>(0));
        UNI::WeakPtr<Z> wp(sp);

        UNI::WeakPtr<Y> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<Y> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<Z> sp(new Z);
        UNI::WeakPtr<Z> wp(sp);

        UNI::WeakPtr<X> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<X> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }

    {
        UNI::SharedPtr<Z> sp(new Z);
        UNI::WeakPtr<Z> wp(sp);

        UNI::WeakPtr<Y> wp2(wp);
        TEST_ASSERT(wp2.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp2.GetUseCount() == 1);
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        sp.Reset();
        TEST_ASSERT(!(wp < wp2 || wp2 < wp));

        UNI::WeakPtr<Y> wp3(wp);
        TEST_ASSERT(wp3.GetUseCount() == wp.GetUseCount());
        TEST_ASSERT(wp3.GetUseCount() == 0);
        TEST_ASSERT(!(wp < wp3 || wp3 < wp));
    }
}

void test()
{
    default_constructor();
    shared_ptr_constructor();
    copy_constructor();
    conversion_constructor();
}

} // namespace n_constructors

namespace n_assignment
{

    template<class T> void copy_assignment(UNI::SharedPtr<T> & sp)
    {
        TEST_ASSERT(sp.IsUnique());

        UNI::WeakPtr<T> p1;

        p1 = p1;
        TEST_ASSERT(p1.GetUseCount() == 0);

        UNI::WeakPtr<T> p2;

        p1 = p2;
        TEST_ASSERT(p1.GetUseCount() == 0);

        UNI::WeakPtr<T> p3(p1);

        p1 = p3;
        TEST_ASSERT(p1.GetUseCount() == 0);

        UNI::WeakPtr<T> p4(sp);

        p4 = p4;
        TEST_ASSERT(p4.GetUseCount() == 1);

        p1 = p4;
        TEST_ASSERT(p1.GetUseCount() == 1);

        p4 = p2;
        TEST_ASSERT(p4.GetUseCount() == 0);

        sp.Reset();

        p1 = p1;
        TEST_ASSERT(p1.GetUseCount() == 0);

        p4 = p1;
        TEST_ASSERT(p4.GetUseCount() == 0);
    }

    void conversion_assignment()
    {
        {
            UNI::WeakPtr<void> p1;

            UNI::WeakPtr<incomplete> p2;

            p1 = p2;
            TEST_ASSERT(p1.GetUseCount() == 0);

            UNI::SharedPtr<incomplete> sp = create_incomplete();
            UNI::WeakPtr<incomplete> p3(sp);

            p1 = p3;
            TEST_ASSERT(p1.GetUseCount() == 1);

            sp.Reset();

            p1 = p3;
            TEST_ASSERT(p1.GetUseCount() == 0);

            p1 = p2;
            TEST_ASSERT(p1.GetUseCount() == 0);
        }

        {
            UNI::WeakPtr<X> p1;

            UNI::WeakPtr<Z> p2;

            p1 = p2;
            TEST_ASSERT(p1.GetUseCount() == 0);

            UNI::SharedPtr<Z> sp(new Z);
            UNI::WeakPtr<Z> p3(sp);

            p1 = p3;
            TEST_ASSERT(p1.GetUseCount() == 1);

            sp.Reset();

            p1 = p3;
            TEST_ASSERT(p1.GetUseCount() == 0);

            p1 = p2;
            TEST_ASSERT(p1.GetUseCount() == 0);
        }

        {
            UNI::WeakPtr<Y> p1;

            UNI::WeakPtr<Z> p2;

            p1 = p2;
            TEST_ASSERT(p1.GetUseCount() == 0);

            UNI::SharedPtr<Z> sp(new Z);
            UNI::WeakPtr<Z> p3(sp);

            p1 = p3;
            TEST_ASSERT(p1.GetUseCount() == 1);

            sp.Reset();

            p1 = p3;
            TEST_ASSERT(p1.GetUseCount() == 0);

            p1 = p2;
            TEST_ASSERT(p1.GetUseCount() == 0);
        }
    }

    template<class T, class U> void shared_ptr_assignment(UNI::SharedPtr<U> & sp, T * = 0)
    {
        TEST_ASSERT(sp.IsUnique());

        UNI::WeakPtr<T> p1;
        UNI::WeakPtr<T> p2(p1);
        UNI::WeakPtr<T> p3(sp);
        UNI::WeakPtr<T> p4(p3);

        p1 = sp;
        TEST_ASSERT(p1.GetUseCount() == 1);

        p2 = sp;
        TEST_ASSERT(p2.GetUseCount() == 1);

        p3 = sp;
        TEST_ASSERT(p3.GetUseCount() == 1);

        p4 = sp;
        TEST_ASSERT(p4.GetUseCount() == 1);

        sp.Reset();

        TEST_ASSERT(p1.GetUseCount() == 0);
        TEST_ASSERT(p2.GetUseCount() == 0);
        TEST_ASSERT(p3.GetUseCount() == 0);
        TEST_ASSERT(p4.GetUseCount() == 0);

        p1 = sp;
    }

    void test()
    {
        {
            UNI::SharedPtr<int> p( new int );
            copy_assignment( p );
        }

        {
            UNI::SharedPtr<X> p( new X );
            copy_assignment( p );
        }

        {
            UNI::SharedPtr<void> p( new int );
            copy_assignment( p );
        }

        {
            UNI::SharedPtr<incomplete> p = create_incomplete();
            copy_assignment( p );
        }

        conversion_assignment();

        {
            UNI::SharedPtr<int> p( new int );
            shared_ptr_assignment<int>( p );
        }

        {
            UNI::SharedPtr<int> p( new int );
            shared_ptr_assignment<void>( p );
        }

        {
            UNI::SharedPtr<X> p( new X );
            shared_ptr_assignment<X>( p );
        }

        {
            UNI::SharedPtr<X> p( new X );
            shared_ptr_assignment<void>( p );
        }

        {
            UNI::SharedPtr<void> p( new int );
            shared_ptr_assignment<void>( p );
        }

        {
            UNI::SharedPtr<incomplete> p = create_incomplete();
            shared_ptr_assignment<incomplete>( p );
        }

        {
            UNI::SharedPtr<incomplete> p = create_incomplete();
            shared_ptr_assignment<void>( p );
        }
    }

} // namespace n_assignment

namespace n_reset
{

    template<class T, class U> void test2( UNI::SharedPtr<U> & sp, T * = 0 )
    {
        TEST_ASSERT(sp.IsUnique());

        UNI::WeakPtr<T> p1;
        UNI::WeakPtr<T> p2(p1);
        UNI::WeakPtr<T> p3(sp);
        UNI::WeakPtr<T> p4(p3);
        UNI::WeakPtr<T> p5(sp);
        UNI::WeakPtr<T> p6(p5);

        p1.Reset();
        TEST_ASSERT(p1.GetUseCount() == 0);

        p2.Reset();
        TEST_ASSERT(p2.GetUseCount() == 0);

        p3.Reset();
        TEST_ASSERT(p3.GetUseCount() == 0);

        p4.Reset();
        TEST_ASSERT(p4.GetUseCount() == 0);

        sp.Reset();

        p5.Reset();
        TEST_ASSERT(p5.GetUseCount() == 0);

        p6.Reset();
        TEST_ASSERT(p6.GetUseCount() == 0);
    }

    void test()
    {
        {
            UNI::SharedPtr<int> p( new int );
            test2<int>( p );
        }

        {
            UNI::SharedPtr<int> p( new int );
            test2<void>( p );
        }

        {
            UNI::SharedPtr<X> p( new X );
            test2<X>( p );
        }

        {
            UNI::SharedPtr<X> p( new X );
            test2<void>( p );
        }

        {
            UNI::SharedPtr<void> p( new int );
            test2<void>( p );
        }

        {
            UNI::SharedPtr<incomplete> p = create_incomplete();
            test2<incomplete>( p );
        }

        {
            UNI::SharedPtr<incomplete> p = create_incomplete();
            test2<void>( p );
        }
    }

} // namespace n_reset

namespace n_use_count
{

    void test()
    {
        {
            UNI::WeakPtr<X> wp;
            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp.IsExpired());

            UNI::WeakPtr<X> wp2;
            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp.IsExpired());

            UNI::WeakPtr<X> wp3(wp);
            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp.IsExpired());
            TEST_ASSERT(wp3.GetUseCount() == 0);
            TEST_ASSERT(wp3.IsExpired());
        }

        {
            UNI::SharedPtr<X> sp(static_cast<X*>(0));

            UNI::WeakPtr<X> wp(sp);
            TEST_ASSERT(wp.GetUseCount() == 1);
            TEST_ASSERT(!wp.IsExpired());

            UNI::WeakPtr<X> wp2(sp);
            TEST_ASSERT(wp.GetUseCount() == 1);
            TEST_ASSERT(!wp.IsExpired());

            UNI::WeakPtr<X> wp3(wp);
            TEST_ASSERT(wp.GetUseCount() == 1);
            TEST_ASSERT(!wp.IsExpired());
            TEST_ASSERT(wp3.GetUseCount() == 1);
            TEST_ASSERT(!wp3.IsExpired());

            UNI::SharedPtr<X> sp2(sp);

            TEST_ASSERT(wp.GetUseCount() == 2);
            TEST_ASSERT(!wp.IsExpired());
            TEST_ASSERT(wp2.GetUseCount() == 2);
            TEST_ASSERT(!wp2.IsExpired());
            TEST_ASSERT(wp3.GetUseCount() == 2);
            TEST_ASSERT(!wp3.IsExpired());

            UNI::SharedPtr<void> sp3(sp);

            TEST_ASSERT(wp.GetUseCount() == 3);
            TEST_ASSERT(!wp.IsExpired());
            TEST_ASSERT(wp2.GetUseCount() == 3);
            TEST_ASSERT(!wp2.IsExpired());
            TEST_ASSERT(wp3.GetUseCount() == 3);
            TEST_ASSERT(!wp3.IsExpired());

            sp.Reset();

            TEST_ASSERT(wp.GetUseCount() == 2);
            TEST_ASSERT(!wp.IsExpired());
            TEST_ASSERT(wp2.GetUseCount() == 2);
            TEST_ASSERT(!wp2.IsExpired());
            TEST_ASSERT(wp3.GetUseCount() == 2);
            TEST_ASSERT(!wp3.IsExpired());

            sp2.Reset();

            TEST_ASSERT(wp.GetUseCount() == 1);
            TEST_ASSERT(!wp.IsExpired());
            TEST_ASSERT(wp2.GetUseCount() == 1);
            TEST_ASSERT(!wp2.IsExpired());
            TEST_ASSERT(wp3.GetUseCount() == 1);
            TEST_ASSERT(!wp3.IsExpired());

            sp3.Reset();

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp.IsExpired());
            TEST_ASSERT(wp2.GetUseCount() == 0);
            TEST_ASSERT(wp2.IsExpired());
            TEST_ASSERT(wp3.GetUseCount() == 0);
            TEST_ASSERT(wp3.IsExpired());
        }
    }

} // namespace n_use_count

namespace n_swap
{

    void test()
    {
        {
            UNI::WeakPtr<X> wp;
            UNI::WeakPtr<X> wp2;

            wp.Swap(wp2);

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp2.GetUseCount() == 0);

            using std::swap;
            swap(wp, wp2);

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp2.GetUseCount() == 0);
        }

        {
            UNI::SharedPtr<X> sp(new X);
            UNI::WeakPtr<X> wp;
            UNI::WeakPtr<X> wp2(sp);
            UNI::WeakPtr<X> wp3(sp);

            wp.Swap(wp2);

            TEST_ASSERT(wp.GetUseCount() == 1);
            TEST_ASSERT(wp2.GetUseCount() == 0);
            TEST_ASSERT(!(wp < wp3 || wp3 < wp));

            using std::swap;
            swap(wp, wp2);

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp2.GetUseCount() == 1);
            TEST_ASSERT(!(wp2 < wp3 || wp3 < wp2));

            sp.Reset();

            wp.Swap(wp2);

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp2.GetUseCount() == 0);
            TEST_ASSERT(!(wp < wp3 || wp3 < wp));

            using std::swap;
            swap(wp, wp2);

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp2.GetUseCount() == 0);
            TEST_ASSERT(!(wp2 < wp3 || wp3 < wp2));
        }

        {
            UNI::SharedPtr<X> sp(new X);
            UNI::SharedPtr<X> sp2(new X);
            UNI::WeakPtr<X> wp(sp);
            UNI::WeakPtr<X> wp2(sp2);
            UNI::WeakPtr<X> wp3(sp2);

            wp.Swap(wp2);

            TEST_ASSERT(wp.GetUseCount() == 1);
            TEST_ASSERT(wp2.GetUseCount() == 1);
            TEST_ASSERT(!(wp < wp3 || wp3 < wp));

            using std::swap;
            swap(wp, wp2);

            TEST_ASSERT(wp.GetUseCount() == 1);
            TEST_ASSERT(wp2.GetUseCount() == 1);
            TEST_ASSERT(!(wp2 < wp3 || wp3 < wp2));

            sp.Reset();

            wp.Swap(wp2);

            TEST_ASSERT(wp.GetUseCount() == 1);
            TEST_ASSERT(wp2.GetUseCount() == 0);
            TEST_ASSERT(!(wp < wp3 || wp3 < wp));

            using std::swap;
            swap(wp, wp2);

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp2.GetUseCount() == 1);
            TEST_ASSERT(!(wp2 < wp3 || wp3 < wp2));

            sp2.Reset();

            wp.Swap(wp2);

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp2.GetUseCount() == 0);
            TEST_ASSERT(!(wp < wp3 || wp3 < wp));

            using std::swap;
            swap(wp, wp2);

            TEST_ASSERT(wp.GetUseCount() == 0);
            TEST_ASSERT(wp2.GetUseCount() == 0);
            TEST_ASSERT(!(wp2 < wp3 || wp3 < wp2));
        }
}

} // namespace n_swap

namespace n_comparison
{

    void test()
    {
        {
            UNI::WeakPtr<X> wp;
            TEST_ASSERT(!(wp < wp));

            UNI::WeakPtr<X> wp2;
            TEST_ASSERT(!(wp < wp2 && wp2 < wp));

            UNI::WeakPtr<X> wp3(wp);
            TEST_ASSERT(!(wp3 < wp3));
            TEST_ASSERT(!(wp < wp3 && wp3 < wp));
        }

        {
            UNI::SharedPtr<X> sp(new X);

            UNI::WeakPtr<X> wp(sp);
            TEST_ASSERT(!(wp < wp));

            UNI::WeakPtr<X> wp2;
            TEST_ASSERT(wp < wp2 || wp2 < wp);
            TEST_ASSERT(!(wp < wp2 && wp2 < wp));

            bool b1 = wp < wp2;
            bool b2 = wp2 < wp;

            {
                UNI::WeakPtr<X> wp3(wp);

                TEST_ASSERT(!(wp < wp3 || wp3 < wp));
                TEST_ASSERT(!(wp < wp3 && wp3 < wp));

                TEST_ASSERT(wp2 < wp3 || wp3 < wp2);
                TEST_ASSERT(!(wp2 < wp3 && wp3 < wp2));

                UNI::WeakPtr<X> wp4(wp2);

                TEST_ASSERT(wp4 < wp3 || wp3 < wp4);
                TEST_ASSERT(!(wp4 < wp3 && wp3 < wp4));
            }

            sp.Reset();

            TEST_ASSERT(b1 == (wp < wp2));
            TEST_ASSERT(b2 == (wp2 < wp));

            {
                UNI::WeakPtr<X> wp3(wp);

                TEST_ASSERT(!(wp < wp3 || wp3 < wp));
                TEST_ASSERT(!(wp < wp3 && wp3 < wp));

                TEST_ASSERT(wp2 < wp3 || wp3 < wp2);
                TEST_ASSERT(!(wp2 < wp3 && wp3 < wp2));

                UNI::WeakPtr<X> wp4(wp2);

                TEST_ASSERT(wp4 < wp3 || wp3 < wp4);
                TEST_ASSERT(!(wp4 < wp3 && wp3 < wp4));
            }
        }

        {
            UNI::SharedPtr<X> sp(new X);
            UNI::SharedPtr<X> sp2(new X);

            UNI::WeakPtr<X> wp(sp);
            UNI::WeakPtr<X> wp2(sp2);

            TEST_ASSERT(wp < wp2 || wp2 < wp);
            TEST_ASSERT(!(wp < wp2 && wp2 < wp));

            bool b1 = wp < wp2;
            bool b2 = wp2 < wp;

            {
                UNI::WeakPtr<X> wp3(wp);

                TEST_ASSERT(!(wp < wp3 || wp3 < wp));
                TEST_ASSERT(!(wp < wp3 && wp3 < wp));

                TEST_ASSERT(wp2 < wp3 || wp3 < wp2);
                TEST_ASSERT(!(wp2 < wp3 && wp3 < wp2));

                UNI::WeakPtr<X> wp4(wp2);

                TEST_ASSERT(wp4 < wp3 || wp3 < wp4);
                TEST_ASSERT(!(wp4 < wp3 && wp3 < wp4));
            }

            sp.Reset();

            TEST_ASSERT(b1 == (wp < wp2));
            TEST_ASSERT(b2 == (wp2 < wp));

            {
                UNI::WeakPtr<X> wp3(wp);

                TEST_ASSERT(!(wp < wp3 || wp3 < wp));
                TEST_ASSERT(!(wp < wp3 && wp3 < wp));

                TEST_ASSERT(wp2 < wp3 || wp3 < wp2);
                TEST_ASSERT(!(wp2 < wp3 && wp3 < wp2));

                UNI::WeakPtr<X> wp4(wp2);

                TEST_ASSERT(wp4 < wp3 || wp3 < wp4);
                TEST_ASSERT(!(wp4 < wp3 && wp3 < wp4));
            }

            sp2.Reset();

            TEST_ASSERT(b1 == (wp < wp2));
            TEST_ASSERT(b2 == (wp2 < wp));

            {
                UNI::WeakPtr<X> wp3(wp);

                TEST_ASSERT(!(wp < wp3 || wp3 < wp));
                TEST_ASSERT(!(wp < wp3 && wp3 < wp));

                TEST_ASSERT(wp2 < wp3 || wp3 < wp2);
                TEST_ASSERT(!(wp2 < wp3 && wp3 < wp2));

                UNI::WeakPtr<X> wp4(wp2);

                TEST_ASSERT(wp4 < wp3 || wp3 < wp4);
                TEST_ASSERT(!(wp4 < wp3 && wp3 < wp4));
            }
        }

        {
            UNI::SharedPtr<X> sp(new X);
            UNI::SharedPtr<X> sp2(sp);

            UNI::WeakPtr<X> wp(sp);
            UNI::WeakPtr<X> wp2(sp2);

            TEST_ASSERT(!(wp < wp2 || wp2 < wp));
            TEST_ASSERT(!(wp < wp2 && wp2 < wp));

            bool b1 = wp < wp2;
            bool b2 = wp2 < wp;

            {
                UNI::WeakPtr<X> wp3(wp);

                TEST_ASSERT(!(wp < wp3 || wp3 < wp));
                TEST_ASSERT(!(wp < wp3 && wp3 < wp));

                TEST_ASSERT(!(wp2 < wp3 || wp3 < wp2));
                TEST_ASSERT(!(wp2 < wp3 && wp3 < wp2));

                UNI::WeakPtr<X> wp4(wp2);

                TEST_ASSERT(!(wp4 < wp3 || wp3 < wp4));
                TEST_ASSERT(!(wp4 < wp3 && wp3 < wp4));
            }

            sp.Reset();
            sp2.Reset();

            TEST_ASSERT(b1 == (wp < wp2));
            TEST_ASSERT(b2 == (wp2 < wp));

            {
                UNI::WeakPtr<X> wp3(wp);

                TEST_ASSERT(!(wp < wp3 || wp3 < wp));
                TEST_ASSERT(!(wp < wp3 && wp3 < wp));

                TEST_ASSERT(!(wp2 < wp3 || wp3 < wp2));
                TEST_ASSERT(!(wp2 < wp3 && wp3 < wp2));

                UNI::WeakPtr<X> wp4(wp2);

                TEST_ASSERT(!(wp4 < wp3 || wp3 < wp4));
                TEST_ASSERT(!(wp4 < wp3 && wp3 < wp4));
            }
        }

        {
            UNI::SharedPtr<X> spx(new X);
            UNI::SharedPtr<Y> spy(new Y);
            UNI::SharedPtr<Z> spz(new Z);

            UNI::WeakPtr<X> px(spx);
            UNI::WeakPtr<Y> py(spy);
            UNI::WeakPtr<Z> pz(spz);

            TEST_ASSERT(px < py || py < px);
            TEST_ASSERT(px < pz || pz < px);
            TEST_ASSERT(py < pz || pz < py);

            TEST_ASSERT(!(px < py && py < px));
            TEST_ASSERT(!(px < pz && pz < px));
            TEST_ASSERT(!(py < pz && pz < py));

            UNI::WeakPtr<void> pvx(px);
            TEST_ASSERT(!(pvx < pvx));

            UNI::WeakPtr<void> pvy(py);
            TEST_ASSERT(!(pvy < pvy));

            UNI::WeakPtr<void> pvz(pz);
            TEST_ASSERT(!(pvz < pvz));

            TEST_ASSERT(pvx < pvy || pvy < pvx);
            TEST_ASSERT(pvx < pvz || pvz < pvx);
            TEST_ASSERT(pvy < pvz || pvz < pvy);

            TEST_ASSERT(!(pvx < pvy && pvy < pvx));
            TEST_ASSERT(!(pvx < pvz && pvz < pvx));
            TEST_ASSERT(!(pvy < pvz && pvz < pvy));

            spx.Reset();
            spy.Reset();
            spz.Reset();

            TEST_ASSERT(px < py || py < px);
            TEST_ASSERT(px < pz || pz < px);
            TEST_ASSERT(py < pz || pz < py);

            TEST_ASSERT(!(px < py && py < px));
            TEST_ASSERT(!(px < pz && pz < px));
            TEST_ASSERT(!(py < pz && pz < py));

            TEST_ASSERT(!(pvx < pvx));
            TEST_ASSERT(!(pvy < pvy));
            TEST_ASSERT(!(pvz < pvz));

            TEST_ASSERT(pvx < pvy || pvy < pvx);
            TEST_ASSERT(pvx < pvz || pvz < pvx);
            TEST_ASSERT(pvy < pvz || pvz < pvy);

            TEST_ASSERT(!(pvx < pvy && pvy < pvx));
            TEST_ASSERT(!(pvx < pvz && pvz < pvx));
            TEST_ASSERT(!(pvy < pvz && pvz < pvy));
        }

        {
            UNI::SharedPtr<Z> spz(new Z);
            UNI::SharedPtr<X> spx(spz);

            UNI::WeakPtr<Z> pz(spz);
            UNI::WeakPtr<X> px(spx);
            UNI::WeakPtr<Y> py(spz);

            TEST_ASSERT(!(px < px));
            TEST_ASSERT(!(py < py));

            TEST_ASSERT(!(px < py || py < px));
            TEST_ASSERT(!(px < pz || pz < px));
            TEST_ASSERT(!(py < pz || pz < py));

            UNI::WeakPtr<void> pvx(px);
            UNI::WeakPtr<void> pvy(py);
            UNI::WeakPtr<void> pvz(pz);

            TEST_ASSERT(!(pvx < pvy || pvy < pvx));
            TEST_ASSERT(!(pvx < pvz || pvz < pvx));
            TEST_ASSERT(!(pvy < pvz || pvz < pvy));

            spx.Reset();
            spz.Reset();

            TEST_ASSERT(!(px < px));
            TEST_ASSERT(!(py < py));

            TEST_ASSERT(!(px < py || py < px));
            TEST_ASSERT(!(px < pz || pz < px));
            TEST_ASSERT(!(py < pz || pz < py));

            TEST_ASSERT(!(pvx < pvy || pvy < pvx));
            TEST_ASSERT(!(pvx < pvz || pvz < pvx));
            TEST_ASSERT(!(pvy < pvz || pvz < pvy));
        }
}

} // namespace n_comparison

namespace n_lock
{

    void test()
    {
    }

} // namespace n_lock

namespace n_map
{

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

        std::map< UNI::WeakPtr<void>, long > m;

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
            for(std::map< UNI::WeakPtr<void>, long >::iterator i = m.begin(); i != m.end(); ++i)
            {
                TEST_ASSERT(i->first.GetUseCount() == i->second);
            }
        }
    }

} // namespace n_map

class incomplete
{
};

UNI::SharedPtr<incomplete> create_incomplete()
{
    UNI::SharedPtr<incomplete> px(new incomplete);
    return px;
}

Suite* m_suite = 0;

void Suite::Test()
{
    n_constructors::test();
    n_assignment::test();
    n_reset::test();
    n_use_count::test();
    n_swap::test();
    n_comparison::test();
    n_lock::test();

    n_map::test();
}

}