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

#ifndef __ZS_EPOLL_HPP_INCLUDED__
#define __ZS_EPOLL_HPP_INCLUDED__

#include "platform.hpp"

#ifdef ZS_HAVE_LINUX

#include <vector>
#include <sys/epoll.h>

#include "i_poller.hpp"
//#include "i_poll_events.hpp"
#include "fd.hpp"
#include "thread.hpp"
#include "atomic_counter.hpp"

namespace zs
{

    //  This class implements socket polling mechanism using the Linux-specific
    //  epoll mechanism.

    class epoll_t : public i_poller
    {
    public:

        epoll_t ();
        virtual ~epoll_t ();

        //  i_poller implementation.
        handle_t add_fd (fd_t fd_, i_poll_events *events_);
        void rm_fd (handle_t handle_);
        void set_pollin (handle_t handle_);
        void reset_pollin (handle_t handle_);
        void set_pollout (handle_t handle_);
        void reset_pollout (handle_t handle_);
        void add_timer (i_poll_events *events_);
        void cancel_timer (i_poll_events *events_);
        int get_load ();
        void start ();
        void stop ();
        void join ();

    private:

        //  Main worker thread routine.
        static void worker_routine (void *arg_);

        //  Main event loop.
        void loop ();

        //  Main epoll file descriptor
        fd_t epoll_fd;

        struct poll_entry_t
        {
            fd_t fd;
            epoll_event ev;
            struct i_poll_events *events;
        };

        //  List of retired event sources.
        typedef std::vector <poll_entry_t*> retired_t;
        retired_t retired;

        //  List of all the engines waiting for the timer event.
        typedef std::vector <struct i_poll_events*> timers_t;
        timers_t timers;

        //  If true, thread is in the process of shutting down.
        bool stopping;

        //  Handle of the physical thread doing the I/O work.
        thread_t worker;

        //  Load of the poller. Currently number of file descriptors
        //  registered with the poller.
        atomic_counter_t load;

        epoll_t (const epoll_t&);
        void operator = (const epoll_t&);
    };

}

#endif

#endif