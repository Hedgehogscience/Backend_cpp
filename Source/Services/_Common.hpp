/*
    Initial author: Convery (tcn@ayria.se)
    Started: 29-04-2018
    License: MIT
    Notes:
        Provides a single include-file for all modules.
*/

#pragma once

namespace Common
{
    // Service ports that nginx forwards to.
    enum class Serverports
    {
        Developer = 28000,
        Frontpage = 28001,
        Forumpost = 28002,
        Forumthread = 28003,
        Accountinformation = 28004,
        Accountregistration = 28005,
    };
}
