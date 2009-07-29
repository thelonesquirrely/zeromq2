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

#include "../include/zs.h"

#include "sub.hpp"
#include "app_thread.hpp"
#include "session.hpp"
#include "err.hpp"

zs::sub_t::sub_t (app_thread_t *thread_, session_t *session_) :
    socket_base_t (thread_, session_)
{
    disable_out ();
}

int zs::sub_t::send (struct zs_msg *msg_, int flags_)
{
    //  Subscriber socket has no send function.
    errno = ENOTSUP;
    return -1;
}

int zs::sub_t::flush ()
{
    //  Subscriber socket has no flush function.
    errno = ENOTSUP;
    return -1;
}