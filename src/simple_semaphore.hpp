/*
    Copyright (c) 2007-2009 FastMQ Inc.

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ZS_SIMPLE_SEMAPHORE_HPP_INCLUDED__
#define __ZS_SIMPLE_SEMAPHORE_HPP_INCLUDED__

#include "platform.hpp"
#include "err.hpp"

#if defined ZS_HAVE_LINUX || defined ZS_HAVE_OSX || defined ZS_HAVE_OPENVMS
#include <pthread.h>
#elif defined ZS_HAVE_WINDOWS
#include "windows.hpp"
#else
#include <semaphore.h>
#endif

namespace zs
{

    //  Simple semaphore. Only single thread may be waiting at any given time.
    //  Also, the semaphore may not be posted before the previous post
    //  was matched by corresponding wait and the waiting thread was
    //  released.

#if defined ZS_HAVE_LINUX || defined ZS_HAVE_OSX || defined ZS_HAVE_OPENVMS

    //  On platforms that allow for double locking of a mutex from the same
    //  thread, simple semaphore is implemented using mutex, as it is more
    //  efficient than full-blown semaphore.

    class simple_semaphore_t
    { 
    public:

        //  Initialise the semaphore.
        inline simple_semaphore_t ()
        {
            int rc = pthread_mutex_init (&mutex, NULL);
	    errno_assert (rc == 0);
            rc = pthread_mutex_lock (&mutex);
	    errno_assert (rc == 0);
        }

        //  Destroy the semaphore.
        inline ~simple_semaphore_t ()
        {
            int rc = pthread_mutex_unlock (&mutex);
            errno_assert (rc == 0);
            rc = pthread_mutex_destroy (&mutex);
            errno_assert (rc == 0);
        }

        //  Wait for the semaphore.
        inline void wait ()
        {
             int rc = pthread_mutex_lock (&mutex);
             errno_assert (rc == 0);
        }

        //  Post the semaphore.
        inline void post ()
        {
            int rc = pthread_mutex_unlock (&mutex);
            errno_assert (rc == 0);
        }

    private:

        pthread_mutex_t mutex;

        //  Disable copying of the object.
        simple_semaphore_t (const simple_semaphore_t&);
        void operator = (const simple_semaphore_t&);
    };

#elif defined ZMQ_HAVE_WINDOWS

    //  On Windows platform simple semaphore is implemeted using event object.

    class simple_semaphore_t
    { 
    public:

        //  Initialise the semaphore.
        inline simple_semaphore_t ()
        {
            ev = CreateEvent (NULL, FALSE, FALSE, NULL);
            win_assert (ev != NULL);
        }

        //  Destroy the semaphore.
        inline ~simple_semaphore_t ()
        {
            int rc = CloseHandle (ev);
            win_assert (rc != 0);    
        }

        //  Wait for the semaphore.
        inline void wait ()
        {
            DWORD rc = WaitForSingleObject (ev, INFINITE);
            win_assert (rc != WAIT_FAILED);
        }

        //  Post the semaphore.
        inline void post ()
        {
            int rc = SetEvent (ev);
            win_assert (rc != 0);
        }

    private:

        HANDLE ev;

        //  Disable copying of the object.
        simple_semaphore_t (const simple_semaphore_t&);
        void operator = (const simple_semaphore_t&);
    };

#else

    //  Default implementation maps simple semaphore to standard semaphore.

    class simple_semaphore_t
    { 
    public:

        //  Initialise the semaphore.
        inline simple_semaphore_t ()
        {
             int rc = sem_init (&sem, 0, 0);
             errno_assert (rc != -1);
        }

        //  Destroy the semaphore.
        inline ~simple_semaphore_t ()
        {
             int rc = sem_destroy (&sem);
             errno_assert (rc != -1);
        }

        //  Wait for the semaphore.
        inline void wait ()
        {
             int rc = sem_wait (&sem);
             errno_assert (rc != -1);
        }

        //  Post the semaphore.
        inline void post ()
        {
            int rc = sem_post (&sem);
            errno_assert (rc != -1);
        }

    private:

        //  Underlying system semaphore object.
        sem_t sem;

        //  Disable copying of the object.
        simple_semaphore_t (const simple_semaphore_t&);
        void operator = (const simple_semaphore_t&);
    };

#endif

}

#endif