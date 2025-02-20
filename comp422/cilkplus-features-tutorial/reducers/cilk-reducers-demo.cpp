/*
 * Copyright (C) 2012, Intel Corporation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * cilk-reducers-demo.cpp
 *
 * Demonstrate creating a list using locks and reducers.
 */

#include <cilk/cilk.h>
#include <cilk/reducer_list.h>
#include <list>
#include <iostream>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <time.h>
#endif

#ifdef _MSC_VER
// Windows implementation of a mutex
class mutex
{
public:
    mutex()
    {
        ::InitializeCriticalSection(&m_cs);
    }

    ~mutex()
    {
        ::DeleteCriticalSection(&m_cs);
    }

    void lock()
    {
        ::EnterCriticalSection(&m_cs);
    }

    void unlock()
    {
        ::LeaveCriticalSection(&m_cs);
    }

private:
    CRITICAL_SECTION m_cs;
};
#else
// Linux implementation of a mutex
class mutex
{
public:
    mutex()
    {
      m_mutex = PTHREAD_MUTEX_INITIALIZER;
    }

    ~mutex()
    {
    }

    void lock()
    {
        ::pthread_mutex_lock(&m_mutex);
    }

    void unlock()
    {
        ::pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
};
#endif

/*
 * simulated_work
 *
 * Function that calls OS-dependent sleep function to simulate doing something
 * for 1/10th of a second or so
 */
void simulated_work()
{
#ifdef _MSC_VER
    Sleep(100);         // 100 milliseconds
#else
    struct timespec request, remaining;

    // Request 100,000,00 nanoseconds
    request.tv_sec = 0;
    request.tv_nsec = 100 * 1000 * 1000;
    nanosleep(&request, &remaining);
#endif
}

/*
 * locked_list_test
 *
 * Example of creating an STL list using a mutex.  Note that:
 *  - STL lists are not thread-safe so we must use a mutex to guard
 *    access to the list.
 *  - While the mutex guarantees that the access is thread-safe, it doesn't
 *    make any guarantees about ordering, so the resulting list will be
 *    jumbled and diffent on every run with more than 1 worker.
 */
void locked_list_test()
{
    mutex m;
    std::list<char>letters;

    // Build the list
    cilk_for(char ch = 'a'; ch <= 'z'; ch++)
    {
        simulated_work();

        m.lock();
        letters.push_back(ch);
        m.unlock();
    }

    // Show the resulting list
    std::cout << "Letters from locked list: ";
    for(std::list<char>::iterator i = letters.begin(); i != letters.end(); i++)
    {
        std::cout << " " << *i;
    }
    std::cout << std::endl;
}

/*
 * reducer_list_test
 *
 * Example of creating an STL list using reducer_list_append.  Note that:
 *  - We don't need a lock to guard the list since each strand has it's own
 *    view of the list.  The views are combined by the Cilk runtime as strands
 *    sync by calling the reducer() function of the reducer's monoid.
 *  - The list is maintained in serial order.
 *  - You can only get the value of a reducer after all parallel work has been
 *    completed.
 */
void reducer_list_test()
{
    cilk::reducer_list_append<char> letters_reducer;

    // Build the list
    cilk_for(char ch = 'a'; ch <= 'z'; ch++)
    {
        simulated_work();
        letters_reducer.push_back(ch);
    }

    // Show the resulting list
    std::cout << "Letters from reducer_list:";
    const std::list<char> &letters = letters_reducer.get_value();
    for(std::list<char>::const_iterator i = letters.begin(); i != letters.end(); i++)
    {
        std::cout << " " << *i;
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    locked_list_test();
    reducer_list_test();

    return 0;
}
