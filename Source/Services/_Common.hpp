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
    enum class Serviceports
    {
        Developer = 28000,
        Account = 28001,        
        Profile = 28002,
        Forum = 28003,
    };
}
